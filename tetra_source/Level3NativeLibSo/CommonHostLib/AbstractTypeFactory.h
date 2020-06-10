#ifndef __ABSTRACT_TYPE_FACTORY_H__
#define __ABSTRACT_TYPE_FACTORY_H__

#include <exception>
#include <typeinfo>
#include <map>

#define ATF_INHERITED_ERROR_POLICY

struct EmptyConfig { // TODO
};

template< class ProductType, class IdentifierType, class PtrType >
class DefaultFactoryErrorPolicy {
public:
	class Exception : public std::exception {
	public:
		Exception( const IdentifierType& unknownId ) : unknownId_( unknownId ) {
		}
		virtual const char* what() {
			return "Unknown type to create";
		}
		const IdentifierType& getId() const {
			return unknownId_;
		}
	private:
		const IdentifierType& unknownId_;
	};
#ifdef ATF_INHERITED_ERROR_POLICY
protected:
#endif
	template< class ConfigType >
	PtrType CreateUnknownType( const IdentifierType& id, const ConfigType& config ) {
		throw Exception( id );
	}
};

template < 
	class Derived, 
	class AbstractProduct, 
	class PtrType = std::shared_ptr< AbstractProduct >, // std/boost::unique_ptr, native ptr etc.
	class IdType  = std::string, // good variant is [const] std::type_info*,
	class AbstractConfig = EmptyConfig, 
	class IdCompare = std::less< IdType >, //  = CompareTypeInfo
	class ProductCreator = PtrType (*)( const AbstractConfig& config )
	#ifdef ATF_INHERITED_ERROR_POLICY
	, template< class, class, class > class FactoryErrorPolicy = DefaultFactoryErrorPolicy
	#endif
>
class AbstractTypeFactory 
#ifdef ATF_INHERITED_ERROR_POLICY
: public FactoryErrorPolicy< AbstractProduct, IdType, PtrType >
#endif
{
public:
	// itself, for usage within descendants
	using Base = AbstractTypeFactory< 
		Derived, AbstractProduct, PtrType, IdType, AbstractConfig, IdCompare, ProductCreator 
		#ifdef ATF_INHERITED_ERROR_POLICY
		, FactoryErrorPolicy
		#endif
	>; 

	using ProductType = AbstractProduct;
	using ConfigType = AbstractConfig;
	using IdentifierType = IdType;
	using CompFn = IdCompare;
	using ProductPtr = PtrType;
	using BaseErrorPolicy = DefaultFactoryErrorPolicy< AbstractProduct, IdType, PtrType >;
#ifndef ATF_INHERITED_ERROR_POLICY
	using BaseErrorPolicyPtr = std::shared_ptr< BaseErrorPolicy >;
#endif

	static Derived& Instance() {
		static Derived instance;
		return instance;
	}
	bool Register( const IdentifierType& id, ProductCreator creator ) {
		return map_.insert( typename Map::value_type( id, creator ) ).second;
	}
	bool Unregister( const IdentifierType& id ) {
		return map_.erase( id ) == 1;
	}
	virtual ProductPtr Create( const IdentifierType& id, const AbstractConfig& config ) { // ConfigPtr
		typename Map::const_iterator i = map_.find( id );
		if( i == map_.end() )
			#ifdef ATF_INHERITED_ERROR_POLICY
			return FactoryErrorPolicy< AbstractProduct, IdType, PtrType >::CreateUnknownType( id, config );
			#else
			return errorPolicy->CreateUnknownType( id, config );
			#endif
		return ( i->second )( config );
	}
protected:
	#ifdef ATF_INHERITED_ERROR_POLICY
	AbstractTypeFactory() {
	}
	#else
	AbstractTypeFactory( BaseErrorPolicyPtr ep = BaseErrorPolicyPtr() ) : errorPolicy( ep ) {
		if( !errorPolicy ) {
			errorPolicy.reset( new DefaultFactoryErrorPolicy< AbstractProduct, IdType, PtrType >() );
		}
	}
	#endif
	AbstractTypeFactory( const AbstractTypeFactory& ) {
	}
	AbstractTypeFactory& operator=( const AbstractTypeFactory& ) {
	}
	~AbstractTypeFactory() {
	}
protected:
	using Map = std::map< IdentifierType, ProductCreator, IdCompare >;
	Map map_;
	#ifndef ATF_INHERITED_ERROR_POLICY
	BaseErrorPolicyPtr errorPolicy;
	#endif
};

#endif /*__ABSTRACT_TYPE_FACTORY_H__*/

