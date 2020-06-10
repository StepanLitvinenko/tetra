/****************************************************************************/
/*  C6xSimulator.h                                                          */
/*  Copyright (c) 1997-2005  Texas Instruments Incorporated                 */
/****************************************************************************/

/*  
 *  Header file which defines the prototypes for the intrinsics.
 *
 *  Relevant/included *only* on the host-side. DSP-code could remove 
 *  dependencies on this host intrinsics lib by doing e.g.
 *  #if defined(SIMULATION)
 *  #include "C6xSimulator.h"
 *  #endif
 *  i.e. on DSP-side all intrinsics are naturally native & dont need 
 *  prototypes.
 */

/****************************************************************************/
/* compiler directive to ensure only one copy of C6xSimulator.h gets        */
/* included                                                                 */
#ifndef C6XSIMULATOR_H
#define C6XSIMULATOR_H


/****************************************************************************/
/* the includes                                                             */
#include "C6xSimulator_base_types.h"

/* this header file is relevant *only* in host environment */
#if defined(SIMULATION) 


/* check to ensure that the target DSP is defined */
#if !defined(TMS320C62X) && !defined(TMS320C64X) && !defined(TMS320C64PX)
#error "TMS320C62X, TMS320C64X or TMS320C64PX (i.e. the target DSP) must be defined."
#endif


/****************************************************************************/
/* the typedefs                                                             */

#ifdef TMS320C64X
#define TMS320C62X
#endif          // TMS320C64X

#ifdef TMS320C64PX
#define TMS320C62X
#define TMS320C64X
#endif          // TMS320C64PX 

/* memory intrinsics */
#ifdef TMS320C62X
#define _amem2(ptr)             (*((uint16 *) (ptr)))
#define _amem2_const(ptr)       (*((const uint16 *) (ptr)))
#define _amem4(ptr)             (*((uint32 *) (ptr)))
#define _amem4_const(ptr)       (*((const uint32 *) (ptr)))
#define _amemd8(ptr)            (*((int64_d *) (ptr)))
#define _amemd8_const(ptr)      (*((const int64_d *) (ptr)))
#endif          // TMS320C62X

#ifdef TMS320C64X
#define _amem8(ptr)             (*((int64_ll *) (ptr)))
#define _amem8_const(ptr)       (*((const int64_ll *) (ptr)))

#define _mem2(ptr)              (*((uint16 *) (ptr)))
#define _mem2_const(ptr)        (*((const uint16 *) (ptr)))
#define _mem4(ptr)              (*((uint32 *) (ptr)))
#define _mem4_const(ptr)        (*((const uint32 *) (ptr)))
#define _mem8(ptr)              (*((int64_ll *) (ptr)))
#define _mem8_const(ptr)        (*((const int64_ll *) (ptr)))
#define _memd8(ptr)             (*((int64_d *) (ptr)))
#define _memd8_const(ptr)       (*((const int64_d *) (ptr)))
#endif          // TMS320C64X

/* pseudo-operations */
#ifdef TMS320C64X
#define _cmplt2(a,b)      _cmpgt2(b,a)
#define _cmpltu4(a,b)     _cmpgtu4(b,a)
#define _dotpnrus2(a,b)   _dotpnrsu2(b,a)
#define _dotprus2(a,b)    _dotprsu2(b,a)
#define _dotpus4(a,b)     _dotpsu4(b,a)
#define _mpyih(a,b)       _mpyhi(b,a)
#define _mpyihr(a,b)      _mpyhir(b,a)
#define _mpyil(a,b)       _mpyli(b,a)
#define _mpyilr(a,b)      _mpylir(b,a)
#define _mpyus4(a,b)      _mpysu4(b,a)
#define _saddsu2(a,b)     _saddus2(b,a)
#define _swap2(a)         _packlh2(a,a)
#endif          // TMS320C64X



/****************************************************************************/
/* the typedefs                                                             */


/****************************************************************************/
/* the globals                                                              */


/****************************************************************************/
/* function prototypes for the TMS320C6X DSP                                */

#ifdef TMS320C62X

int32 _abs(int32 a);
int32 _add2(int32 a,int32 b);
uint32 _clr(uint32 a,uint32 b,uint32 c);
uint32 _clrr(uint32 a,int32 b);
uint40 _dtol(uint64_d a);
int32 _ext(int32 a,uint32 b,uint32 c);
int32 _extr(int32 a,int32 b);
uint32 _extu(uint32 a,uint32 b,uint32 c);
uint32 _extur(uint32 a,int32 b);
uint32 _ftoi(float a);
uint32 _hi(uint64_d a);
uint32 _hill(uint64_ll a);
uint64_d _itod(uint32 a,uint32 b);
float _itof(uint32 a);
/* Fix for BZ 1679.  Return type must be *signed* 64-bit long to match   */
/* c6x.h                                                                 */
int64_ll _itoll(uint32 a,uint32 b);
int40 _labs(int40 a);
uint32 _lmbd(uint32 a,uint32 b);
uint32 _lnorm(int40 a);
uint32 _lo(uint64_d a);
int40 _lsadd(int32 a,int40 b);
int40 _lssub(int32 a,int40 b);
uint32 _loll(uint64_ll a);
uint64_d _ltod(uint40 a);
int32 _mpy(int32 a,int32 b);
int32 _mpyh(int32 a,int32 b);
int32 _mpyhsu(int32 a,uint32 b);
uint32 _mpyhu(uint32 a,uint32 b);
int32 _mpyhus(uint32 a,int32 b);
int32 _mpyhl(int32 a,int32 b);
uint32 _mpyhlu(uint32 a,uint32 b);
int32 _mpyhslu(int32 a,uint32 b);
int32 _mpyhuls(uint32 a,int32 b);
int32 _mpylh(int32 a,int32 b);
uint32 _mpylhu(uint32 a,uint32 b);
int32 _mpylshu(int32 a,uint32 b);
int32 _mpyluhs(uint32 a,int32 b);
int32 _mpysu(int32 a,uint32 b);
uint32 _mpyu(uint32 a,uint32 b);
int32 _mpyus(uint32 a,int32 b);
uint32 _norm(int32 a);
int32 _sadd(int32 a,int32 b);
int32 _sat(int40 a);
uint32 _set(uint32 a,uint32 b,uint32 c);
uint32 _setr(uint32 a,int32 b);
int32 _smpy(int32 a,int32 b);
int32 _smpyh(int32 a,int32 b);
int32 _smpyhl(int32 a,int32 b);
int32 _smpylh(int32 a,int32 b);
int32 _sshl(int32 a,uint32 b);
int32 _ssub(int32 a,int32 b);
int32 _sub2(int32 a,int32 b);
uint32 _subc(uint32 a,uint32 b);

#endif      // TMS320C62X


/****************************************************************************/
/* additional function prototypes for the TMS320C64X DSP                    */

#ifdef TMS320C64X

int32 _abs2(int32 a);
int32 _add4(int32 a,int32 b);
int32 _avg2(int32 a,int32 b);
uint32 _avgu4(uint32 a,uint32 b);
uint32 _bitc4(uint32 a);
uint32 _bitr(uint32 a);
int32 _cmpeq2(int32 a,int32 b);
int32 _cmpeq4(int32 a,int32 b);
int32 _cmpgt2(int32 a,int32 b);
uint32 _cmpgtu4(uint32 a,uint32 b);
uint32 _deal(uint32 a);
int32 _dotp2(int32 a,int32 b);
int32 _dotpn2(int32 a,int32 b);
int32 _dotpnrsu2(int32 a,uint32 b);
int32 _dotprsu2(int32 a,uint32 b);
int32 _dotpsu4(int32 a,uint32 b);
uint32 _dotpu4(uint32 a,uint32 b);
int32 _gmpy4(int32 a,int32 b);
uint64_d _ldotp2(int32 a,int32 b);
int32 _max2(int32 a,int32 b);
uint32 _maxu4(uint32 a,uint32 b);
int32 _min2(int32 a,int32 b);
uint32 _minu4(uint32 a,uint32 b);
uint64_d _mpy2(int32 a,int32 b);
uint64_ll _mpy2ll(int32 a,int32 b);
uint64_d _mpyhi(int32 a,int32 b);
uint64_ll _mpyhill(int32 a,int32 b);
int32 _mpyhir(int32 a,int32 b);
uint64_d _mpyli(int32 a,int32 b);
uint64_ll _mpylill(int32 a,int32 b);
int32 _mpylir(int32 a,int32 b);
uint64_d _mpysu4(int32 a,uint32 b);
uint64_ll _mpysu4ll(int32 a,uint32 b);
uint64_d _mpyu4(uint32 a,uint32 b);
uint64_ll _mpyu4ll(uint32 a,uint32 b);
int32 _mvd(int32 a);
uint32 _pack2(uint32 a,uint32 b);
uint32 _packh2(uint32 a,uint32 b);
uint32 _packh4(uint32 a,uint32 b);
uint32 _packhl2(uint32 a,uint32 b);
uint32 _packl4(uint32 a,uint32 b);
uint32 _packlh2(uint32 a,uint32 b);

/* MSVC defines its own _rotl operation.  This re-definition effectively     */
/* disables that rotl.  That's OK with the vast majority of MSVC users.      */
/* Note the type of the second argument is signed int.  That matches how     */
/* MSVC prototypes _rotl in stdlib.h.  That change silences a useless        */
/* MSVC warning.                                                             */
#ifdef _MSC_VER
#define _rotl _ti_rotl
uint32 _rotl(uint32 a, int32 b);
#else
uint32 _rotl(uint32 a, uint32 b);
#endif

int32 _sadd2(int32 a,int32 b);
uint32 _saddu4(uint32 a,uint32 b);
int32 _saddus2(uint32 a,int32 b);
uint32 _shfl(uint32 a);
uint32 _shlmb(uint32 a,uint32 b);
int32 _shr2(int32 a,uint32 b);
uint32 _shrmb(uint32 a,uint32 b);
uint32 _shru2(uint32 a,uint32 b);
uint64_d _smpy2(int32 a,int32 b);
uint64_ll _smpy2ll(int32 a,int32 b);
int32 _spack2(int32 a,int32 b);
uint32 _spacku4(int32 a,int32 b);
int32 _sshvl(int32 a,int32 b);
int32 _sshvr(int32 a,int32 b);
int32 _sub4(int32 a,int32 b);
int32 _subabs4(int32 a,int32 b);
uint32 _swap4(uint32 a);
uint32 _unpkhu4(uint32 a);
uint32 _unpklu4(uint32 a);
uint32 _xpnd2(uint32 a);
uint32 _xpnd4(uint32 a);

#endif      // TMS320C64X


/****************************************************************************/
/* additional function prototypes for the TMS320C64+X DSP                   */

#ifdef TMS320C64PX

uint64_ll _addsub(int32 a,int32 b);
uint64_ll _addsub2(uint32 a,uint32 b);
uint64_ll _cmpy(uint32 a,uint32 b);
uint32 _cmpyr(uint32 a,uint32 b);
uint32 _cmpyr1(uint32 a,uint32 b);
uint64_ll _ddotp4(uint32 a,uint32 b);
uint64_ll _ddotph2(uint64_ll a,uint32 b);
uint32 _ddotph2r(uint64_ll a,uint32 b);
uint64_ll _ddotpl2(uint64_ll a,uint32 b);
uint32 _ddotpl2r(uint64_ll a,uint32 b);
uint64_ll _dmv(uint32 a,uint32 b);
uint64_ll _dpack2(uint32 a,uint32 b);
uint64_ll _dpackx2(uint32 a,uint32 b);
uint32 _gmpy(uint32 a,uint32 b);
uint64_ll _mpy2ir(uint32 a,int32 b);
int32 _mpy32(int32 a,int32 b);
uint64_ll _mpy32ll(int32 a,int32 b);
uint64_ll _mpy32su(int32 a,uint32 b);
uint64_ll _mpy32u(uint32 a,uint32 b);
uint64_ll _mpy32us(uint32 a,int32 b);
uint32 _rpack2(int32 a,int32 b);
uint64_ll _saddsub(int32 a,int32 b);
uint64_ll _saddsub2(uint32 a,uint32 b);
uint64_ll _shfl3(uint32 a,uint32 b);
int32 _smpy32(int32 a,int32 b);
int32 _ssub2(int32 a,int32 b);
uint32 _xormpy(uint32 a,uint32 b);

#endif      // TMS320C64PX


/****************************************************************************/
/* function prototypes for the TMS320C67X DSP                               */

#ifdef TMS320C67X

int32 _dpint(double a);
double _fabs(double a);
float _fabsf(float a);
double _mpyid(int32 a,int32 b);
double _rcpdp(double a);
float _rcpsp(float a);
double _rsqrdp(double a);
float _rsqrsp(float a);
int _spint(float a);

#endif      // TMS320C67X


#endif      // SIMULATION

#endif      // C6XSIMULATOR_H


/*
 *  Automated Revision Information
 *  Changed: $Date: 2008-12-10 11:17:41 -0600 (Wed, 10 Dec 2008) $
 *  Revision: $Revision: 7621 $
 */

