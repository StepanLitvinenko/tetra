/****************************************************************************/
/*  C6xSimulator_type_modifiers.h                                           */
/*  Copyright (c) 1997-2005  Texas Instruments Incorporated                 */
/****************************************************************************/

/*  
 *  Header file which defines/undefines certain keywords.
 *  Abstracted into its own file because different environments may
 *  support/not-support different keywords.
 *  For example 'restrict' is newly supported in C99. Some environments
 *  and compilers may support this whilst others may not. Those that dont
 *  should undefine restrict.
 *
 *  By virtue of this abstraction, several options exist for the user: -
 *  1. use this file as is
 *  2. dont use this file. Instead do defines/undefines in your makefile
 *  3. use this file and modify it as per the keyword support in your host-env.
 */


#ifndef C6XSIMULATOR_TYPE_MODIFIERS_H
#define C6XSIMULATOR_TYPE_MODIFIERS_H

/* this internal header file is relevant *only* in host environment */
#if defined(SIMULATION) 


/* compiler directives : default settings used */
#define far
#define inline 
#define interrupt
#define _nassert(a)           assert(a)
#define near
#define restrict
#define volatile


#endif          /* SIMULATION */

#endif          /* C6XSIMULATOR_TYPE_MODIFIERS_H */
/*
 *  Automated Revision Information
 *  Changed: $Date: 2007-07-16 12:51:58 -0500 (Mon, 16 Jul 2007) $
 *  Revision: $Revision: 3324 $
 */

