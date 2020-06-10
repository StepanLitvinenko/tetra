/****************************************************************************/
/*  _C6xSimulator_priv.h                                                    */
/*  Copyright (c) 1997-2005  Texas Instruments Incorporated                 */
/****************************************************************************/

/*                                                                          
 * Internal header file to support C6xSimulator intrinsics implementation  
 * Only for use by C6xSimulator implementation itself 
 * (not user code)
 */

#ifndef _C6XSIMULATOR_PRIV_H
#define _C6XSIMULATOR_PRIV_H


/* this internal header file is relevant *only* in host environment */
#if defined(SIMULATION) 


/****************************************************************************/
/* the defines                                                              */

#define PASS                                0
#define FAIL                                1

#define MIN_INT8         ((int32)(0xFFFFFF80))
#define MAX_INT8         ((int32)(0x0000007F))

#define MIN_UINT8       ((uint32)(0x00000000))
#define MAX_UINT8       ((uint32)(0x000000FF))

#define MIN_INT16        ((int32)(0xFFFF8000))
#define MAX_INT16        ((int32)(0x00007FFF))

#define MIN_UINT16      ((uint32)(0x00000000))
#define MAX_UINT16      ((uint32)(0x0000FFFF))

#define MIN_INT32        ((int32)(0x80000000))
#define MAX_INT32        ((int32)(0x7FFFFFFF))

#define MIN_UINT32      ((uint32)(0x00000000))
#define MAX_UINT32      ((uint32)(0xFFFFFFFF))

#define MIN_INT40_HI      ((int8)(0xFFFFFF80))
#define MIN_INT40_LO    ((uint32)(0x00000000))

#define MAX_INT40_HI      ((int8)(0x0000007F))
#define MAX_INT40_LO    ((uint32)(0xFFFFFFFF))

#define MIN_UINT40_HI    ((uint8)(0x00000000))
#define MIN_UINT40_LO   ((uint32)(0x00000000))

#define MAX_UINT40_HI    ((uint8)(0x000000FF))
#define MAX_UINT40_LO   ((uint32)(0xFFFFFFFF))

#define MIN_INT64_HI     ((int32)(0x80000000))
#define MIN_INT64_LO    ((uint32)(0x00000000))

#define MAX_INT64_HI     ((int32)(0x7FFFFFFF))
#define MAX_INT64_LO    ((uint32)(0xFFFFFFFF))

#define MIN_UINT64_HI   ((uint32)(0x00000000))
#define MIN_UINT64_LO   ((uint32)(0x00000000))

#define MAX_UINT64_HI   ((uint32)(0xFFFFFFFF))
#define MAX_UINT64_LO   ((uint32)(0xFFFFFFFF))


/****************************************************************************/
/* the data structures                                                      */

#ifdef LITTLE_ENDIAN_HOST

typedef struct _INT32X2
{
  int16 lo;
  int16 hi;
} int32x2;

typedef struct _INT32X2U
{
  uint16 lo;
  uint16 hi;
} int32x2u;

typedef struct _INT32X4
{
  int8 lo1;
  int8 lo2;
  int8 hi1;
  int8 hi2;
} int32x4;

typedef struct _INT32X4U
{
  uint8 lo1;
  uint8 lo2;
  uint8 hi1;
  uint8 hi2;
} int32x4u;

typedef struct _INT40X2
{
  int32 lo;
  int8  hi;
} int40x2;

typedef struct _INT40X2U
{
  uint32 lo;
  uint8  hi;
} int40x2u;

typedef struct _INT64X2
{
  int32 lo;
  int32 hi;
} int64x2;

typedef struct _INT64X2U
{
  uint32 lo;
  uint32 hi;
} int64x2u;

typedef struct _INT64X4
{
  int16 lo1;
  int16 lo2;
  int16 hi1;
  int16 hi2;
} int64x4;

typedef struct _INT64X4U
{
  uint16 lo1;
  uint16 lo2;
  uint16 hi1;
  uint16 hi2;
} int64x4u;

#endif     // LITTLE_ENDIAN_HOST


#ifdef BIG_ENDIAN_HOST

typedef struct _INT32X2
{
  int16 hi;
  int16 lo;
} int32x2;

typedef struct _INT32X2U
{
  uint16 hi;
  uint16 lo;
} int32x2u;

typedef struct _INT32X4
{
  int8 hi2;
  int8 hi1;
  int8 lo2;
  int8 lo1;
} int32x4;

typedef struct _INT32X4U
{
  uint8 hi2;
  uint8 hi1;
  uint8 lo2;
  uint8 lo1;
} int32x4u;

typedef struct _INT40X2
{
  int8  hi;
  int32 lo;
} int40x2;

typedef struct _INT40X2U
{
  uint8  hi;
  uint32 lo;
} int40x2u;

typedef struct _INT64X2
{
  int32 hi;
  int32 lo;
} int64x2;

typedef struct _INT64X2U
{
  uint32 hi;
  uint32 lo;
} int64x2u;

typedef struct _INT64X4
{
  int16 hi2;
  int16 hi1;
  int16 lo2;
  int16 lo1;
} int64x4;

typedef struct _INT64X4U
{
  uint16 hi2;
  uint16 hi1;
  uint16 lo2;
  uint16 lo1;
} int64x4u;

#endif    // BIG_ENDIAN_HOST


/***************************************************************************/
/* the unions                                                              */
#if defined(LITTLE_ENDIAN_HOST) || defined(BIG_ENDIAN_HOST)

union reg32 
{
  int32    x1;
  int32x2  x2;
  int32x4  x4;

  uint32   x1u;
  int32x2u x2u;
  int32x4u x4u;

  float    xf;
};

union reg40
{
  int40    x1;
  int40x2  x2;

  uint40   x1u;
  int40x2u x2u;
};

union reg64
{
  int64_d     x1_d;
  int64_ll    x1_ll;
  int64x2     x2;
  int64x4     x4;

  uint64_d   x1u_d;
  uint64_ll  x1u_ll;
  int64x2u   x2u;
  int64x4u   x4u;
};

#endif          // LITTLE_ENDIAN_HOST || BIG_ENDIAN_HOST


#endif          /* SIMULATION */

#endif          /* _C6XSIMULATOR_PRIV_H */
/*
 *  Automated Revision Information
 *  Changed: $Date: 2007-07-16 12:51:58 -0500 (Mon, 16 Jul 2007) $
 *  Revision: $Revision: 3324 $
 */

