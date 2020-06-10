/****************************************************************************/
/*  C6xSimulator_base_types.h                                                     */
/*  Copyright (c) 1997-2005  Texas Instruments Incorporated                 */
/****************************************************************************/

/*
 *  This header file defines the set of base type abstractions used in
 *  C6xSimulator and its tests. It has been refactored into its own file
 *  to enable users to see which base type typedefs are used in C6xSimulator.
 *  
 *  This allows customers to use their own familiar base type typedefs if they 
 *  already have such an abstraction e.g. can simply add a file myStdTypes.h
 *  which does e.g. 
 *      #if defined(SIMULATION)   
 *          typedef     int8      MyStdTypesInt8;
 *  etc.
 *
 *  Customers can then continue to use familiar typedefs. To use that code
 *  on the host, they would simply add -DSIMULATION to the makefile.
 *  
 *  In essence this allows for 0 src code changes on established codebases
 */

/****************************************************************************/
/* compiler directive to ensure only one copy of .h gets                    */
/* included                                                                 */
#ifndef C6XSIMULATOR_BASE_TYPES_H
#define C6XSIMULATOR_BASE_TYPES_H


/***************************************************************************/
/* the type defines                                                        */

typedef char                    int8;
typedef short                  int16;
typedef int                    int32;

typedef unsigned char          uint8;
typedef unsigned short        uint16;
typedef unsigned int          uint32;


#ifdef SIMULATION

#ifdef _MSC_VER       // handle Microsoft types
typedef __int64                int40;
typedef __int64                int64_d;
typedef __int64                int64_ll;

typedef __int64                uint40;
typedef __int64                uint64_d;
typedef __int64                uint64_ll;
#else                 // other targets we've used support long long >= 64 bits
typedef long long              int40;
typedef long long              int64_d;
typedef long long              int64_ll;

typedef unsigned long long     uint40;
typedef unsigned long long     uint64_d;
typedef unsigned long long     uint64_ll;
#endif                // MSC_VER

#else 

/* TI compiler data type map */
typedef long                   int40;
typedef double                 int64_d;
typedef long long              int64_ll; 

typedef unsigned long          uint40;
typedef double                 uint64_d;
typedef unsigned long long     uint64_ll;

#endif                // SIMULATION


#endif                // C6XSIMULATOR_BASE_TYPES_H

/*
 *  Automated Revision Information
 *  Changed: $Date: 2007-07-16 12:51:58 -0500 (Mon, 16 Jul 2007) $
 *  Revision: $Revision: 3324 $
 */

