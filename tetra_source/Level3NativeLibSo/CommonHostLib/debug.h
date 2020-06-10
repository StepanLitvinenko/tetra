#ifndef _DSP_DEBUG_TRACE_DEFS_H_
#define _DSP_DEBUG_TRACE_DEFS_H_

#include <iostream>
#include <iomanip>

#if __cplusplus >= 201103L
#include <chrono>
#else
#include "commonnat/DateTime.h"
#endif

#define UNI_ASSERT_LOG( condition, what ) \
	if( !( condition ) ) { \
		_err << "CORBA_ANY_ASSERT: " << ( what ) << endl_; \
	}

#define UNI_ASSERT_BREAK( condition, what ) \
	if( !( condition ) ) { \
		_err << "CORBA_ANY_ASSERT: " << ( what ) << endl_; \
		break; \
	}

#define UNI_ASSERT_RETURN( condition, what ) \
	if( !( condition ) ) { \
		_err << "CORBA_ANY_ASSERT: " << ( what ) << endl_; \
		return; \
	}

#define UNI_ASSERT_THROW( condition, what ) \
	if( !( condition ) ) { \
		_err << "CORBA_ANY_ASSERT: " << ( what ) << endl_; \
		throw std::runtime_error( what ); \
	}

#define HEXW( width, value ) std::hex << std::setfill( '0' ) << std::setw( width ) << static_cast< uint64_t >( value ) << std::dec
#define HEX( value ) HEXW( sizeof( value ) << 1, value )

// this dummy declarations can be used for compile-time review 
// of types being inferenced (VIA COMPILE-TIME ERROR MESSAGES)
// eg:
// constexpr auto x = 5LL; // (simple type as sample)
// __TV< decltype( x ), x > td;
// error message (g++ 4.7.2): 
// "aggregate '__TV<const long long int, 5> td' has incomplete type and cannot be defined"
// use these declarations to review complex inferenced types that are no so obvious
template< typename T > class __TD;
template< typename T, T val > class __TV;

#ifdef LIB_VERSION

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)
#define _func_context_ __FILE__ "(" TOSTRING(__LINE__) "), " << __FUNCTION__

#ifdef LINUX
	#define clr_null      "\033[m"
	#define clr_black     "\033[30m"
	#define clr_red       "\033[31m"
	#define clr_green     "\033[32m"
	#define clr_brown     "\033[33m"
	#define clr_blue      "\033[34m"
	#define clr_magenta   "\033[35m"
	#define clr_cyan      "\033[36m"
	#define clr_ltgray    "\033[37m"
	#define clr_dkgray    "\033[1;30m"
	#define clr_ltred     "\033[1;31m"
	#define clr_ltgreen   "\033[1;32m"
	#define clr_yellow    "\033[1;33m"
	#define clr_ltblue    "\033[1;34m"
	#define clr_purple    "\033[1;35m"
	#define clr_skyblue   "\033[1;36m"
	#define clr_white     "\033[1;37m"

	#define bgclr_black   "\033[40m"
	#define bgclr_red     "\033[41m"
	#define bgclr_green   "\033[42m"
	#define bgclr_brown   "\033[43m"
	#define bgclr_blue    "\033[44m"
	#define bgclr_magenta "\033[45m"
	#define bgclr_cyan    "\033[46m"
	#define bgclr_ltgray  "\033[47m"
	#define bgclr_dkgray  "\033[1;40m"
	#define bgclr_ltred   "\033[1;41m"
	#define bgclr_ltgreen "\033[1;42m"
	#define bgclr_yellow  "\033[1;43m"
	#define bgclr_ltblue  "\033[1;44m"
	#define bgclr_purple  "\033[1;45m"
	#define bgclr_skyblue "\033[1;46m"
	#define bgclr_white   "\033[1;47m"

	#define out_red( x )     clr_red     x clr_null
	#define out_green( x )   clr_green   x clr_null
	#define out_brown( x )   clr_brown   x clr_null
	#define out_blue( x )    clr_blue    x clr_null
	#define out_magenta( x ) clr_magenta x clr_null
	#define out_cyan( x )    clr_cyan    x clr_null
	#define out_ltgray( x )  clr_ltgray  x clr_null
	#define out_dkgray( x )  clr_dkgray  x clr_null
	#define out_ltred( x )   clr_ltred   x clr_null
	#define out_ltgreen( x ) clr_ltgreen x clr_null
	#define out_yellow( x )  clr_yellow  x clr_null
	#define out_ltblue( x )  clr_ltblue  x clr_null
	#define out_purple( x )  clr_purple  x clr_null
	#define out_skyblue( x ) clr_skyblue x clr_null
	#define out_white( x )   clr_white   x clr_null

	#define _wrn std::cerr << clr_purple << "[w] " << _func_context_ << ": "
	#define _wrn2 std::cerr << clr_yellow << "[w] " << _func_context_ << ": "
	#define _err std::cerr << clr_red << "[!] " << _func_context_ << ": "
	#define _inf std::cerr << clr_white  << "[i] " << _func_context_ << ": "
	#define endl_ clr_null << std::endl

#else

	#define _wrn std::cerr << "[w] " << _func_context_ << ": "
	#define _err std::cerr << "[!] " << _func_context_ << ": "
	#define _inf std::cout << "[i] " << _func_context_ << ": "
	#define endl_ std::endl

#endif



#else // LIB_VERSION
//#error "NO LIB VERSION"
#include <sstream>
//#include "DateTime.h"


typedef unsigned short debug_level_t;

int SegvDescr(const char * descr);

#ifdef _WIN32
	#include "../common/constreams.h"
	typedef int int32_t;
	typedef unsigned int uint32_t;
	typedef short int16_t;
	typedef signed char  int8_t;
	typedef unsigned char uint8_t;
	typedef unsigned short uint16_t;

	#define ccout _ccout << lock_stream()
	#define wccout _wccout << lock_stream()

	#define clr_null      color(FG_LTGRAY | BG_BLACK)
	#define clr_black     color(FG_BLACK)
	#define clr_red       color(FG_RED)
	#define clr_green     color(FG_GREEN)
	#define clr_brown     color(FG_BROWN)
	#define clr_blue      color(FG_BLUE)
	#define clr_magenta   color(FG_MAGENTA)
	#define clr_cyan      color(FG_CYAN)
	#define clr_ltgray    color(FG_LTGRAY)
	#define clr_dkgray    color(FG_DKGRAY)
	#define clr_ltred     color(FG_LTRED)
	#define clr_ltgreen   color(FG_LTGREEN)
	#define clr_yellow    color(FG_YELLOW)
	#define clr_ltblue    color(FG_LTBLUE)
	#define clr_purple    color(FG_LTMAGENTA)
	#define clr_skyblue   color(FG_LTBLUE)
	#define clr_white     color(FG_WHITE)

	#define bgclr_null    color(BG_LTGRAY)
	#define bgclr_black   color(BG_BLACK)
	#define bgclr_red     color(BG_RED)
	#define bgclr_green   color(BG_GREEN)
	#define bgclr_brown   color(BG_BROWN)
	#define bgclr_blue    color(BG_BLUE)
	#define bgclr_magenta color(BG_MAGENTA)
	#define bgclr_cyan    color(BG_CYAN)
	#define bgclr_ltgray  color(BG_LTGRAY)
	#define bgclr_dkgray  color(BG_DKGRAY)
	#define bgclr_ltred   color(BG_LTRED)
	#define bgclr_ltgreen color(BG_LTGREEN)
	#define bgclr_yellow  color(BG_YELLOW)
	#define bgclr_ltblue  color(BG_LTBLUE)
	#define bgclr_purple  color(BG_LTMAGENTA)
	#define bgclr_skyblue color(BG_LTBLUE)
	#define bgclr_white   color(BG_WHITE)

	#define out_red( x )     x
	#define out_green( x )   x
	#define out_brown( x )   x
	#define out_blue( x )    x
	#define out_magenta( x ) x
	#define out_cyan( x )    x
	#define out_ltgray( x )  x
	#define out_dkgray( x )  x
	#define out_ltred( x )   x
	#define out_ltgreen( x ) x
	#define out_yellow( x )  x
	#define out_ltblue( x )  x
	#define out_purple( x )  x
	#define out_skyblue( x ) x
	#define out_white( x )   x

#else
	#define ccout std::cout

	#define clr_null      "\033[m"
	#define clr_black     "\033[30m"
	#define clr_red       "\033[31m"
	#define clr_green     "\033[32m"
	#define clr_brown     "\033[33m"
	#define clr_blue      "\033[34m"
	#define clr_magenta   "\033[35m"
	#define clr_cyan      "\033[36m"
	#define clr_ltgray    "\033[37m"
	#define clr_dkgray    "\033[1;30m"
	#define clr_ltred     "\033[1;31m"
	#define clr_ltgreen   "\033[1;32m"
	#define clr_yellow    "\033[1;33m"
	#define clr_ltblue    "\033[1;34m"
	#define clr_purple    "\033[1;35m"
	#define clr_skyblue   "\033[1;36m"
	#define clr_white     "\033[1;37m"

	#define bgclr_black   "\033[40m"
	#define bgclr_red     "\033[41m"
	#define bgclr_green   "\033[42m"
	#define bgclr_brown   "\033[43m"
	#define bgclr_blue    "\033[44m"
	#define bgclr_magenta "\033[45m"
	#define bgclr_cyan    "\033[46m"
	#define bgclr_ltgray  "\033[47m"
	#define bgclr_dkgray  "\033[1;40m"
	#define bgclr_ltred   "\033[1;41m"
	#define bgclr_ltgreen "\033[1;42m"
	#define bgclr_yellow  "\033[1;43m"
	#define bgclr_ltblue  "\033[1;44m"
	#define bgclr_purple  "\033[1;45m"
	#define bgclr_skyblue "\033[1;46m"
	#define bgclr_white   "\033[1;47m"

	#define out_red( x )     clr_red     x clr_null
	#define out_green( x )   clr_green   x clr_null
	#define out_brown( x )   clr_brown   x clr_null
	#define out_blue( x )    clr_blue    x clr_null
	#define out_magenta( x ) clr_magenta x clr_null
	#define out_cyan( x )    clr_cyan    x clr_null
	#define out_ltgray( x )  clr_ltgray  x clr_null
	#define out_dkgray( x )  clr_dkgray  x clr_null
	#define out_ltred( x )   clr_ltred   x clr_null
	#define out_ltgreen( x ) clr_ltgreen x clr_null
	#define out_yellow( x )  clr_yellow  x clr_null
	#define out_ltblue( x )  clr_ltblue  x clr_null
	#define out_purple( x )  clr_purple  x clr_null
	#define out_skyblue( x ) clr_skyblue x clr_null
	#define out_white( x )   clr_white   x clr_null

#endif

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)

#ifdef _USE_SHORT_CONTEXT_
	#define _func_context_ "(" TOSTRING(__LINE__) "), " << __FUNCTION__
#else
	#define _func_context_ __FILE__ "(" TOSTRING(__LINE__) "), " << __FUNCTION__
#endif

const int showTimePeriod = 1000;

inline std::string showtime()
{
#if __cplusplus >= 201103L
    //YYYY/mm/dd HH:MM:SS.SSS
    using namespace std;
    using namespace std::chrono;
    static system_clock::time_point last = system_clock::now();
    system_clock::time_point curr = system_clock::now();
    if(std::chrono::duration_cast<milliseconds>(curr - last).count() > 1) {
        time_t curr_time = system_clock::to_time_t(curr);
        char sRep[100];
        strftime(sRep,sizeof(sRep),"%Y/%m/%d %H:%M:%S",localtime(&curr_time));
        typename system_clock::time_point::duration since_epoch = curr.time_since_epoch();
        seconds s = duration_cast<seconds>(since_epoch);
        since_epoch -= s;
        milliseconds milli = duration_cast<milliseconds>(since_epoch);
        ostringstream ss;
        ss << "\n===================================== " << sRep << "." << milli.count() << " ==================================" << std::endl;
        last = curr;
        return ss.str();
    }
    return "";
#else
    static DateTime lastTime = DateTime(0);
	DateTime now = DateTime::Now();
	if((now.GetTicks() - lastTime.GetTicks()) / 10000 > showTimePeriod)
	{
		lastTime = now;
		std::ostringstream s;
		s << "\n===================================== " << now << " ==================================" << std::endl;
		return s.str();
	}
	return "";
#endif
}

#ifdef DFBANK_LIBRARY

// Output nothing when debug level value is negative

#define _wrn2 if(::global_debug_level >= 0) ccout << clr_ltblue << showtime() << clr_yellow << "[w] " << _func_context_ << ": "
#define _wrn  if(::global_debug_level >= 0) ccout << clr_ltblue << showtime() << clr_purple << "[w] " << _func_context_ << ": "
#define _err  if(::global_debug_level >= 0) ccout << clr_ltblue << showtime() << clr_red    << "[!] " << _func_context_ << ": "
#define _inf  if(::global_debug_level >= 0) ccout << clr_ltblue << showtime() << clr_white  << "[i] " << _func_context_ << ": " << clr_null

#else

#define _wrn2 ccout << clr_ltblue << showtime() << clr_yellow << "[w] " << _func_context_ << ": "
#define _wrn ccout << clr_ltblue << showtime() << clr_purple << "[w] " << _func_context_ << ": "
#define _err ccout << clr_ltblue << showtime() << clr_red    << "[!] " << _func_context_ << ": "
#define _inf ccout << clr_ltblue << showtime() << clr_white  << "[i] " << _func_context_ << ": " << clr_null

#endif // DFBANK_LIBRARY

#define _dbg          if(::global_debug_level > 0)           ccout << clr_ltblue << showtime() << clr_dkgray << "[d] " << _func_context_ << ": "

/// !!!! use with care!
///    enclose in curled braces { } when using within if/else !
#define _dbg_(level)  if(::global_debug_level >= level)  ccout << clr_ltblue << showtime() << clr_dkgray << "[d" << level << "] " << _func_context_ << ": "

#define endl_ clr_null << std::endl
#define _num(num) ccout << clr_skyblue << "[d] " << _func_context_ << ": " << num << " = " << clr_null << std::endl

#define DL_QUIET 0
#define DL_ERROR 1
#define DL_WARNING 2
#define DL_INFO 3
#define DL_DEBUG 4
#define DL_ALL 5

inline std::ostream & sout(debug_level_t debug_level)
{
	switch (debug_level) {
		case DL_ERROR:
			return ccout << clr_red;
		case DL_WARNING:
			return ccout << clr_purple;
		case DL_INFO:
			return ccout << clr_white;
		case DL_DEBUG:
			return ccout << clr_dkgray;
	}
	return ccout << clr_null;
}

extern int global_debug_level;

#endif // LIB_VERSION

#endif // _DSP_DEBUG_TRACE_DEFS_H_
