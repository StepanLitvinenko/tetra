#include <iostream>
#include <algorithm>
#include "debug.h"
#include "Core.h"

using namespace std;
using namespace CommonHostLib;

// ---- Message -----------------------------------------------

Sys::Message::~Message() {
}

Sys::Message& Sys::Message::operator=( const Sys::Message& ) {
	return *this;
}

void Sys::Message::Reply() {
}

void Sys::Message::Send() {
	;
}

void Sys::Message::Send( MessagePeer::Ptr destin ) {
	this->destin = destin;
//	this->destin->PostMessage( shared_from_this() );
}

// ---- SharedObject --------------------------------------------

Sys::SharedObject::HandleType Sys::SharedObject::nextHandle = 1;
Sys::SharedObject::Instances Sys::SharedObject::instances;

Sys::SharedObject::SharedObject() : handle( 0 ) {
//	cerr << "SharedObject::ctor in" << endl;
//	cerr << "SharedObject::ctor out" << endl;
}

Sys::SharedObject::~SharedObject() {
//	cerr << "SharedObject::dtor in" << endl;
//	cerr << "SharedObject::dtor out" << endl;
}

void Sys::SharedObject::OnCreate() {
	cout << "SharedObject::OnCreate in: nextHandle " << nextHandle << " instances: " << instances << endl;
	Ptr self = shared_from_this();
	for( const auto& instance : instances )
		if( instance.second.lock() == self )
			throw DuplicateInstance();
	HandleType h = nextHandle++;
	self->handle = h;
	instances[ h ] = self;
	cerr << "SharedObject::OnCreate out: instances: " << instances << endl;
}

Sys::SharedObject::Ptr Sys::SharedObject::HandleToInstance( HandleType handle ) {
	cerr << "SharedObject::HandleToInstance: handle " << handle << " instances: " << instances << endl;
	Instances::const_iterator it = instances.find( handle );
	if( it == instances.end() )
		throw MissingHandle( handle );
	return it->second.lock();
}

Sys::SharedObject::HandleType Sys::SharedObject::InstanceToHandle( const Ptr& instance ) {
	cerr << "SharedObject::HandleToInstance: instance " << instance << " instances: " << instances << endl;
	Instances::const_iterator it = find_if( 
		begin( instances ), 
		end( instances ), 
		[ instance ]( const Instances::value_type& curr ) { return curr.second.lock() == instance; } 
	);
	if( it == instances.end() )
		throw MissingInstance();
	return it->first;
}

// ---- EndPoint --------------------------------------------

void Sys::EndPoint::Connect( SharedObject::Ptr, ConnID connId ) {
	_wrn2 << "connId = \"" << connId << "\"" << endl_;
	// NOP
}

void Sys::EndPoint::Disconnect( SharedObject::Ptr, ConnID connId ) {
	_wrn2 << "connId = \"" << connId << "\"" << endl_;
	// NOP
}

Sys::EndPoint::~EndPoint() {
//	cerr << "EndPoint::dtor in" << endl;
//	cerr << "EndPoint::dtor out" << endl;
}

// ---- Resource --------------------------------------------

Sys::Resource::Resource() : active( false ), debugLevel( 0 ), debugMask( 0x0000 ) {
//	cerr << "Resource::ctor in" << endl;
//	cerr << "Resource::ctor out" << endl;
}

Sys::Resource::~Resource() {
//	cerr << "Resource::dtor in" << endl;
//	cerr << "Resource::dtor out" << endl;
}

int Sys::Resource::Initialize() {
	// NOP
	return 0;
}

int Sys::Resource::Finalize() {
	return 0;
}

void Sys::Resource::Configure( const Properties& properties ) {
	_wrn2 << "in" << endl_;
	Sys::Properties::const_iterator prop;
	for( prop = properties.begin(); prop != properties.end(); prop++ ) {
		cerr << "  name \"" << prop->first << "\" type: \"" << prop->second.type().name() << "\" value ";
		if( "DebugLevel" == prop->first ) {
			uint32_t val = 0;
			try {
				val = boost::any_cast< uint32_t >( prop->second );
			} catch( boost::bad_any_cast& e ) {
				cerr << "(uint failed, try int)";
				val = static_cast< uint32_t >( boost::any_cast< int32_t >( prop->second ) );
			}
			this->DebugLevel( ( val >> 0 ) & 0xFFFF );
			this->DebugMask( ( val >> 16 ) & 0xFFFF );
			cerr << this->DebugLevel() << ":" << HEXW( 4, this->DebugMask() );
		}
		cerr << endl;
	}
	_wrn2 << "out" << endl_;
}

void Sys::Resource::Start() {
	active = true;
}

void Sys::Resource::Stop() {
	active = false;
}

///int Sys::Resource::Configure( const BaseSettings* ) {
///	return 0;
///}

// ---- BasePeerTag -----------------------------------------------

Sys::BasePeerTag::~BasePeerTag() {
}

// ---- IRawDataPort --------------------------------------------------

Sys::IRawDataPort::~IRawDataPort() {
}

// ---- Service ------------------------------------------------

Sys::Service::Service() {
	cerr << "Service::ctor in" << endl;
	cerr << "Service::ctor out" << endl;
}

Sys::Service::~Service() {
	cerr << "Service::dtor in" << endl;
	cerr << "Service::dtor out" << endl;
}

// ---- MessagePeer -----------------------------------------------

Sys::MessagePeer::~MessagePeer() {
}

void Sys::MessagePeer::PostMessage( Message::Ptr ) {
	// NOP
}

Sys::Message::Ptr Sys::MessagePeer::Interact( Message::Ptr ) {
	return Message::Ptr();
}

#ifdef RTTI_SOF_PRODUCT_IDENTIFICATION
const bool Sys::AntSys::registered = Sys::SharedObjectFactory::Instance().Register( &typeid( Sys::AntSys ), &Sys::AntSys::Create );
#else
const bool Sys::AntSys::registered = Sys::SharedObjectFactory::Instance().Register( "DSP/AntennSystems", &Sys::AntSys::Create );
#endif

Sys::sof::ptr Sys::AntSys::Create( const Sys::Properties& initialConfig ) { 
	cerr << "AntSys::Create" << endl;
	return sof::ptr( new AntSys() );
}

#ifdef RTTI_SOF_PRODUCT_IDENTIFICATION
const bool Sys::Compensator::registered = Sys::SharedObjectFactory::Instance().Register( &typeid( Compensator ), &Compensator::Create );
#else
const bool Sys::Compensator::registered = Sys::SharedObjectFactory::Instance().Register( "DSP/CompensatorFactory", &Sys::Compensator::Create );
#endif

Sys::sof::ptr Sys::Compensator::Create( const Sys::Properties& initialConfig ) { 
	cerr << "Compensator::Create" << endl;
	return sof::ptr( new Compensator() );
}

#ifdef RTTI_SOF_PRODUCT_IDENTIFICATION
const bool Sys::DirFinder::registered = Sys::SharedObjectFactory::Instance().Register( &typeid( DirFinder ), &DirFinder::Create );
#else
const bool Sys::DirFinder::registered = Sys::SharedObjectFactory::Instance().Register( "DSP/SFBFactory", &Sys::DirFinder::Create );
#endif

Sys::sof::ptr Sys::DirFinder::Create( const Sys::Properties& initialConfig ) { 
	cerr << "DirFinder::Create" << endl;
	return sof::ptr( new DirFinder() );
}

// ---- Factory -------------------------------------------------------

Sys::SharedObjectFactory::ProductPtr Sys::SharedObjectFactory::Create( const IdentifierType& id, const ConfigType& config ) {
	cerr << "SOF::Create: id \"" << id << "\"" << endl;
	ProductPtr result = Base::Create( id, config );
	result->OnCreate();
	return result;
}

