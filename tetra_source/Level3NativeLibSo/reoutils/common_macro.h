#ifndef COMMON_MACRO_H
#define COMMON_MACRO_H
#include <stdint.h>
#include <stdlib.h>
#define ABS( X ) 	( ( ( X ) >= 0 ) ? ( X ) : ( -X ) )
#define MAX_NUM( X, Y ) ( ( X >= Y ) ? ( X ) : ( Y ) )
#define MIN_NUM( X, Y )	( ( X <= Y ) ? ( X ) : ( Y ) )
#define COMPLEX_ABS( Re, Im ) ( MAX_NUM( ABS( Re ), ABS( Im ) ) + (( MIN_NUM( ABS( Re ), ABS( Im ) ) ) >> 1 ) )
#define ALIGN_SIZE( size, align ) ( ( size / ( align + 1 ) + 1 ) * align )

static inline int32_t complex_abs( int32_t im, int32_t re ) {
        re = abs( re ) ;
        im = abs( im ) ;
	return ( re > im ) ? ( re + (im >> 2) ) : ( im + (re >> 2) ) ;
}

static inline uint32_t ceil( uint32_t a, uint32_t b ) {
        uint32_t val = ( ( a % b ) != 0 ) ? ( ( a / b ) + 1 ) : ( a / b );
	return val;
}

static inline uint32_t align_size( uint32_t size, uint32_t align ) {
	return ceil( size, align ) * align;
}

#endif
