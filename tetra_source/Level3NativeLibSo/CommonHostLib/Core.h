#ifndef __L3NL_COMMON_HOST_SYSTEM_CORE_H__
#define __L3NL_COMMON_HOST_SYSTEM_CORE_H__

#include <boost/any.hpp>
#include <cstdint>
#include <memory>
#include <iostream>
#include <string>
#include <queue>
#include <set>
#include <map>

#include "AbstractTypeFactory.h"

namespace CommonHostLib {

	namespace Sys {

		typedef std::map< std::string, boost::any > Properties;

		class MessagePeer;

		class Message 
			: public std::enable_shared_from_this< Message > 
		{
		public:
			typedef std::shared_ptr< Message > Ptr;
			virtual ~Message() = 0;
			virtual Message& operator=( const Message& );
			// TODO implement function family
			virtual void Reply();
			virtual void Send();
			virtual void Send( std::shared_ptr< MessagePeer > destin );
		private:
			std::shared_ptr< MessagePeer > source, destin;
		};

		class SharedObject 
			: public std::enable_shared_from_this< SharedObject > 
		//	, public boost::noncopyable (nahua here?)
		{
			friend class SharedObjectFactory;
		public:
			typedef std::shared_ptr< SharedObject > Ptr;
			typedef uint64_t HandleType;
			typedef std::map< HandleType, std::weak_ptr< SharedObject > > Instances;
			struct DuplicateInstance : public std::exception {
				DuplicateInstance() {}
				virtual ~DuplicateInstance() throw() {}
				virtual const char* what() { return "Registered instance is not unique"; }
			};
			struct MissingInstance : public std::exception {
				MissingInstance() {}
				virtual ~MissingInstance() throw() {}
				virtual const char* what() { return "Instance not found"; }
			};
			class MissingHandle : public std::exception {
			public:
				MissingHandle( const HandleType& handle ) : handle( handle ) {}
				virtual ~MissingHandle() throw() {}
				virtual const char* what() { return "Handle not found"; }
				const HandleType getId() { return handle; }
			private:
				HandleType handle;
			};
		protected:
			SharedObject();
		public:
			virtual ~SharedObject();
			inline HandleType GetHandle() const { return handle; }

			static Ptr HandleToInstance( HandleType );
			static HandleType InstanceToHandle( const Ptr& );
		protected:
			HandleType handle;
		private:
			void OnCreate();
			friend std::ostream& operator<<( std::ostream& os, const Instances& instances ) {
				for( const auto& instance : instances )
					os << "(" << instance.first << ":" << instance.second.use_count() << ")";
				return os;
			}
		private:
			static Instances instances;
			static HandleType nextHandle;
		};

		class MessagePeer 
			: public virtual SharedObject
		{
		public:
			typedef std::shared_ptr< MessagePeer > Ptr;
		public:
			virtual ~MessagePeer() = 0;
			virtual void PostMessage( Message::Ptr );
			virtual Message::Ptr Interact( Message::Ptr );
		};

		class EndPoint 
			: public virtual MessagePeer
		{
		public:
			typedef std::shared_ptr< EndPoint > Ptr;
			typedef std::string ConnID; // uint32_t
			//static const ConnID DefConnId = 0;
		public:
			virtual ~EndPoint() = 0;
			virtual void Connect( SharedObject::Ptr, ConnID connId );
			virtual void Disconnect( SharedObject::Ptr, ConnID connId );
		};

		class Resource 
			: public EndPoint
		{
		public:
			typedef std::shared_ptr< Resource > Ptr;
		protected:
			Resource();
		public:
			virtual ~Resource() = 0;
			virtual int Initialize();
			virtual int Finalize();
			virtual void Configure( const Properties& );
			virtual void Start();
			virtual void Stop();

			inline bool Active() const { return active; }
			inline void DebugLevel( uint16_t value ) { debugLevel = value; }
			inline uint16_t DebugLevel() const { return debugLevel; }
			inline void DebugMask( uint16_t value ) { debugMask = value; }
			inline uint16_t DebugMask() const { return debugMask; }
		protected:
			bool active;
			uint16_t debugLevel;
			uint16_t debugMask;
		};

		template< class T >
		bool SetProperty( Properties::const_iterator prop, const std::string& name, T& value ) {
			bool result = false;
			if( name == prop->first ) {
				try {
					value = boost::any_cast< T >( prop->second );
					result = true;
				} catch( boost::bad_any_cast& e ) {
					std::cerr << "\033[31m" << e.what() << "\033[m";
				}

			}
			return result;
		}

		template< typename T >
		bool CheckField( T& fieldValue, Properties::const_iterator field, const std::string& name ) {
			if( name == field->first ) {
				try {
					fieldValue = boost::any_cast< T >( field->second );
					return true;
				} catch( boost::bad_any_cast& e ) {
					std::cerr << "\033[31m" << e.what() << "\033[m";
				}
			}
			return false;
		}

		template< typename T >
		bool CheckField( T& fieldValue, const Properties& ps, const std::string& name ) {
			Properties::const_iterator field = ps.find( name );
			if( field != ps.end() ) {
				try {
					fieldValue = boost::any_cast< T >( field->second );
					return true;
				} catch( boost::bad_any_cast& e ) {
					std::cerr << "\033[31m" << e.what() << "\033[m";
				}
			}
			return false;
		}

		// factories

		// use external "Naming service" register + "Resource factory" TypeID-to-TypeID binder
		//#define USE_AUXILIARY_NAMING_SERVICE
		// identify Shared Objects types by RTTI (const type_info*), either by string (e.g. "DSP/GeoThreadServiceFactory")
		// used in conjunction with USE_AUXILIARY_NAMING_SERVICE only (config files can't store RTTI info, so we need binder - GenericResourceFactory)
		//#define RTTI_SOF_PRODUCT_IDENTIFICATION

		// recommended to undefine both macros: no external register and binder; SOF types are identified by string
		// all other combinations seem to be deprecated

		#if !defined( USE_AUXILIARY_NAMING_SERVICE ) && defined( RTTI_SOF_PRODUCT_IDENTIFICATION )
			#error "Uncompatible macro combination: RTTI_SOF_PRODUCT_IDENTIFICATION with !USE_AUXILIARY_NAMING_SERVICE (recommended: both NOT defined)"
		#endif

		class SharedObjectFactory;

		template< class IdType, class IdCompare = std::less< IdType > >
		using SofBase = AbstractTypeFactory < SharedObjectFactory, SharedObject, std::shared_ptr< SharedObject >, IdType, Properties, IdCompare >;

		#ifdef RTTI_SOF_PRODUCT_IDENTIFICATION
		struct SharedObjectFactory : public SofBase< const std::type_info*, []( const std::type_info* lhs, const std::type_info* rhs ) { return (*lhs).before(*rhs); } >
		#else
		struct SharedObjectFactory : public SofBase< std::string >
		#endif
		{
			using ptr = ProductPtr;
			virtual ptr Create( const IdentifierType& id, const ConfigType& config ); // with post-create init semantics
		};

		using sof = SharedObjectFactory;

		#ifdef USE_AUXILIARY_NAMING_SERVICE

		struct ResourceFactory : public SharedObject {
			virtual ~ResourceFactory() {
			}
			#ifdef RTTI_SOF_PRODUCT_IDENTIFICATION
			virtual Resource::Ptr create( const Properties& initialConfig ) = 0;
			#else
			virtual Resource::Ptr create( const std::string& ior, const Properties& initialConfig ) = 0;
			#endif
		};

		// its just a polymorphic binding of concrete product type to "ResourceFactory" base
		// (in case of using of NamingService its a mapping node between string and IdType) 
		template< class T >
		class GenericResourceFactory : public ResourceFactory {
		public:
			virtual ~GenericResourceFactory() {
			}
			#ifdef RTTI_SOF_PRODUCT_IDENTIFICATION
			Resource::Ptr create( const Properties& initialConfig ) {
				return std::dynamic_pointer_cast< Resource >(
					SharedObjectFactory::Instance().Create( &typeid( T ), initialConfig ) 
				);
			}
			#else
			Resource::Ptr create( const std::string& ior, const Properties& initialConfig ) {
				return std::dynamic_pointer_cast< Resource >(
					SharedObjectFactory::Instance().Create( ior, initialConfig ) 
				);
			}
			#endif
		};

		#endif

		// dummies for debug

		class AntSys : public Resource {
			static sof::ptr Create( const Properties& initialConfig );
			static const bool registered;
		protected:
			AntSys() {
			}
		public:
			typedef std::shared_ptr< AntSys > Ptr;
			virtual ~AntSys() {
			}
		};
		
		class Compensator : public Resource {
			static sof::ptr Create( const Properties& initialConfig );
			static const bool registered;
		protected:
			Compensator() {
			}
		public:
			typedef std::shared_ptr< Compensator > Ptr;
			virtual ~Compensator() {
			}
		};
		
		class DirFinder : public Resource {
			static sof::ptr Create( const Properties& initialConfig );
			static const bool registered;
		protected:
			DirFinder() {
			}
		public:
			typedef std::shared_ptr< DirFinder > Ptr;
			virtual ~DirFinder() {
			}
		};

		// TODO to be replaced with smth more reasonable

		struct BasePeerTag {
			virtual ~BasePeerTag() = 0;
		};

		struct IRawDataPort : public virtual SharedObject {
			typedef std::shared_ptr< IRawDataPort > Ptr;
			virtual ~IRawDataPort() = 0;
			#if 0
			virtual void Write( CharBlock ) = 0;
			virtual CharBlock Read() = 0;
			#else
			virtual ssize_t Write( const void* data, size_t size, BasePeerTag* = 0 ) = 0;
			virtual ssize_t Read( void* data, size_t size, BasePeerTag* = 0 ) = 0;
			#endif
		};

		typedef std::set< std::shared_ptr< IRawDataPort > > RawDataConsumers;

		// TODO enhance Service entity
#if 1
		class Service 
			: public Resource, public IRawDataPort
		{
		public:
			typedef std::shared_ptr< Service > Ptr;
			virtual ~Service();
		protected:
			Service();
		protected:
			RawDataConsumers consumers;
		};
#endif

	} // Sys

}

#endif /*__L3NL_COMMON_HOST_SYSTEM_CORE_H__*/

