/****************************************************************************/
/*  C6xSimulator.c                                                          */
/*  Copyright (c) 1997-2005  Texas Instruments Incorporated                 */
/****************************************************************************/

/****************************************************************************/
/* the includes                                                             */
#include <math.h>
#include <float.h>

#include "C6xSimulator.h"                 // intrinsics prototypes
#include "_C6xSimulator_priv.h"           // private defs for C6xSimulator
#include "C6xSimulator_type_modifiers.h"  // define/undefine typing keywords


/****************************************************************************/
/* the defines                                                              */
#define DSP_CSR_SAT                       (1 << 9)


/****************************************************************************/
/* the globals                                                              */

/*
 *  The assumption is that users calling intrinsics such as _gmpy()
 *  will set the value of GPLYA before intrinsic entry.
 *  CSR is used in C6xSimulator to check if saturation occurred - since 
 *  same test is often done in DSP code, equivalent functionality is kept.
 */
volatile uint32 CSR;     /* control status register */

#ifdef TMS320C64X
volatile uint32 GFPGFR;  /* Galois field polynomial generator */
                                /* function register                 */
#endif

#ifdef TMS320C64PX
volatile uint32 GPLYA;  /* GMPY Polynomial - A Side */
volatile uint32 GPLYB;  /* GMPY Polynomial - B Side */
#endif



/****************************************************************************/
/* the functions for the TMS320C62X DSP, the TMS320C64X DSP and the         */
/* TMS320C64+X DSP                                                          */

#ifdef TMS320C62X
int32 _abs(int32 a)
{
  int32 y32;

  if(a == (int32)MIN_INT32)
    y32 = MAX_INT32;
  else if(a < 0)
    y32 = -a;
  else
    y32 = a;

  return(y32);
} /* end of _abs() function */


int32 _add2(int32 a,int32 b)
{
  union reg32 a32,b32,y32;

  a32.x1 = a;
  b32.x1 = b;

  y32.x2.lo = a32.x2.lo + b32.x2.lo;
  y32.x2.hi = a32.x2.hi + b32.x2.hi;
  return(y32.x1);
} /* end of _add2() function */


uint32 _clr(uint32 a,uint32 b,uint32 c)
{
  uint32 k,y32u;

  y32u = a;

  for(k=b;k<=c;k++)
    y32u &= ~(1 << k);

  return(y32u);
} /* end of _clr() function */


uint32 _clrr(uint32 a,int32 b)
{
  uint32 start,stop,y32u;

  if((b < 0) || (b > 0x03FF))
    return(FAIL);

  start = b >> 5;
  stop = b & 0x01F;

  y32u = _clr(a,start,stop);

  return(y32u);
} /* end of _clrr() function */


uint40 _dtol(uint64_d a)
{
  union reg40 y40;
  union reg64 a64;

  /* Fix for BZ 1590.  All reg40 local variables must have their .x1[u]  */
  /* fields initialized.  The underlying size of reg40 is 64-bits.  The  */
  /* fields in the .x2[u] struct are only 40-bits.  Thus, assigning only */
  /* to .x2[u] fields may leave garbage in the upper 24 bits.            */ 
  y40.x1u = 0; 

  a64.x1u_d = a;
  
  y40.x2u.hi = (uint8)(a64.x2u.hi & MAX_UINT8);
  y40.x2u.lo = a64.x2u.lo;

  return(y40.x1u);
} /* end of _dtol() function */


int32 _ext(int32 a,uint32 b,uint32 c)
{
  int32 y32;

  y32 = (a << b) >> c;

  return(y32);
} /* end of _ext() function */


int32 _extr(int32 a,int32 b)
{
  int32 y32;
  uint32 lshift,rshift;

  lshift = (b >> 5) & 0x1F;
  rshift = b & 0x01F;

  y32 = _ext(a,lshift,rshift);

  return(y32);
} /* end of _extr() function */
  

uint32 _extu(uint32 a,uint32 b,uint32 c)
{
  uint32 y32u;

  y32u = (a << b) >> c;

  return(y32u);
} /* end of _extu() function */


uint32 _extur(uint32 a,int32 b)
{
  uint32 lshift,rshift,y32u;

  if((b < 0) || (b > 0x03FF))
    return(FAIL);

  lshift = b >> 5;
  rshift = b & 0x01F;

  y32u = _extu(a,lshift,rshift);

  return(y32u);
} /* end of _extur() function */
  

uint32 _ftoi(float a)
{
  union reg32 y32;

  y32.xf = a;

  return(y32.x1u);
} /* end of _ftoi() function */


uint32 _hi(uint64_d a)
{
  uint32 y32u;
  union reg64 a64;

  a64.x1u_d = a;

  y32u = a64.x2u.hi;

  return(y32u);
} /* end of _hi() function */


uint32 _hill(uint64_ll a)
{
  uint32 y32u;
  union reg64 a64;

  a64.x1u_ll = a;

  y32u = _hi(a64.x1u_d);

  return(y32u);
} /* end of _hill() function */


uint64_d _itod(uint32 a,uint32 b)
{
  union reg64 y64;

  y64.x2u.hi = a;
  y64.x2u.lo = b;

  return(y64.x1u_d);
} /* end of _itod() function */


float _itof(uint32 a)
{
  union reg32 y32;

  y32.x1u = a;

  return(y32.xf);
} /* end of _itof() function */
  

/* Fix for BZ 1679.  Return type must be *signed* 64-bit long to match   */
/* c6x.h                                                                 */
int64_ll _itoll(uint32 a,uint32 b)
{
  union reg64 y64;

  y64.x1u_d = _itod(a,b);

  return(y64.x1_ll);
} /* end of _itoll() function */


int40 _labs(int40 a)
{
  union reg40 a40,y40;

  a40.x1 = a;

  /* Fix for BZ 1590.  All reg40 local variables must have their .x1[u]  */
  /* fields initialized.  The underlying size of reg40 is 64-bits.  The  */
  /* fields in the .x2[u] struct are only 40-bits.  Thus, assigning only */
  /* to .x2[u] fields may leave garbage in the upper 24 bits.            */ 
  y40.x1 = 0; 

  if((a40.x2.hi == MIN_INT40_HI) && (a40.x2.lo == MIN_INT40_LO))
    {
      y40.x2.hi = MAX_INT40_HI;
      y40.x2.lo = MAX_INT40_LO;
    }
  else if (a40.x2.hi < 0)
    {
      if(a40.x2.lo == 0)
    {
      y40.x2.hi = ~(a40.x2.hi-1);
      y40.x2.lo = ~a40.x2.lo;
    }
      else
    {
      y40.x2.hi = ~a40.x2.hi;
      y40.x2.lo = ~(a40.x2.lo-1);
    }
    }
  else
    {
      y40.x2.hi = a40.x2.hi;
      y40.x2.lo = a40.x2.lo;
    }

  return(y40.x1);
} /* end of _labs() function */


uint32 _lmbd(uint32 a,uint32 b)
{
  uint32 y32u;
  uint32 m32u;

  y32u = 0;
  m32u = MIN_INT32;

  if(a == 1)
    while((b&m32u) == 0)
      {
    m32u >>= 1;
    y32u++;
    if(y32u == 32) return(y32u);
      }
  else
    while((b&m32u) != 0)
      {
    m32u >>= 1;
    y32u++;
    if(y32u == 32) return(y32u);
      }

  return(y32u);
} /* end of _lmbd() function */


uint32 _lnorm(int40 a)
{
  uint32 y32u;
  union reg40 a40,b40,m40;

  a40.x1 = a;

  /* Fix for BZ 1590.  All reg40 local variables must have their .x1[u]  */
  /* fields initialized.  The underlying size of reg40 is 64-bits.  The  */
  /* fields in the .x2[u] struct are only 40-bits.  Thus, assigning only */
  /* to .x2[u] fields may leave garbage in the upper 24 bits.            */ 
  b40.x1u = 0; 
  m40.x1u = 0; 

  y32u = 0;
  m40.x2u.hi = (uint8)MIN_INT40_HI >> 1;
  m40.x2u.lo = (uint32)MIN_INT40_LO >> 1;

  if(a40.x2.hi < 0)
    {
      b40.x2u.hi = ~a40.x2.hi;
      b40.x2u.lo = ~a40.x2.lo;
    }
  else
    {
      b40.x2u.hi = a40.x2.hi;
      b40.x2u.lo = a40.x2.lo;
    }

  if((b40.x2u.hi == 0) && (b40.x2u.lo == 0))
    return(39);

  while((b40.x2u.hi & m40.x2u.hi) != m40.x2u.hi)
    {
      b40.x2u.hi = (b40.x2u.hi << 1) | (b40.x2u.lo >> 31);
      b40.x2u.lo <<= 1;
      y32u++;
    }
  
  return(y32u);
} /* end of _lnorm() function */


uint32 _lo(uint64_d a)
{
  uint32 y32u;
  union reg64 a64;

  a64.x1u_d = a;

  y32u = a64.x2u.lo;

  return(y32u);
} /* end of _lo() function */


int40 _lsadd(int32 a,int40 b)
{
  int16 k;
  union reg40 b40,c40,y40,y40tmp;

  b40.x1 = b;

  /* Fix for BZ 1590.  All reg40 local variables must have their .x1[u]  */
  /* fields initialized.  The underlying size of reg40 is 64-bits.  The  */
  /* fields in the .x2[u] struct are only 40-bits.  Thus, assigning only */
  /* to .x2[u] fields may leave garbage in the upper 24 bits.            */ 
  c40.x1    = 0; 
  y40.x1    = 0; 
  y40tmp.x1 = 0; 

  c40.x2.hi = (a<0) ? MAX_UINT8 : 0;
  c40.x2.lo = a;

  y40.x2.hi = b40.x2.hi;
  y40.x2.lo = b40.x2.lo;

  /* perform binary addition */
  for(k=0;k<40;k++)
    {
      y40tmp.x2.hi = y40.x2.hi ^ c40.x2.hi;
      y40tmp.x2.lo = y40.x2.lo ^ c40.x2.lo;

      c40.x2.hi &= y40.x2.hi;
      c40.x2.lo &= y40.x2.lo;

      c40.x2.hi <<= 1;
      if(c40.x2.lo & MIN_INT32)
    c40.x2.hi |= 0x1;
      c40.x2.lo <<= 1;

      y40.x2.hi = y40tmp.x2.hi;
      y40.x2.lo = y40tmp.x2.lo;
    }

  /* see if saturation is required */
  /* see if a and b have the same sign */
  if(((a^(int32)b40.x2.hi) & MIN_INT32) == 0)
    {
      /* if the sign changed, sat */
      if((a^(int32)y40.x2.hi) & MIN_INT32)
    {
      y40.x2.hi = (a<0) ? MIN_INT40_HI : MAX_INT40_HI;
      y40.x2.lo = (a<0) ? MIN_INT40_LO : MAX_INT40_LO;
      CSR |= DSP_CSR_SAT;
    }
    }

  return(y40.x1);
} /* end of _lsadd() function */


int40 _lssub(int32 a,int40 b)
{
  int16 k;
  union reg40 b40,c40,y40,y40tmp;

  b40.x1 = b;

  /* Fix for BZ 1590.  All reg40 local variables must have their .x1[u]  */
  /* fields initialized.  The underlying size of reg40 is 64-bits.  The  */
  /* fields in the .x2[u] struct are only 40-bits.  Thus, assigning only */
  /* to .x2[u] fields may leave garbage in the upper 24 bits.            */ 
  c40.x1    = 0; 
  y40.x1    = 0; 
  y40tmp.x1 = 0; 

  c40.x2.hi = (a<0) ? MAX_UINT8 : 0;
  c40.x2.lo = a;

  /* negate b, ie y = -b */
  if(b40.x2.lo == 0)
    {
      y40.x2.hi = ~b40.x2.hi+1;
      y40.x2.lo = ~b40.x2.lo;
    }
  else
    {
      y40.x2.hi = ~b40.x2.hi;
      y40.x2.lo = ~b40.x2.lo+1;
    }

  /* perform binary addition */
  for(k=0;k<40;k++)
    {
      y40tmp.x2.hi = y40.x2.hi ^ c40.x2.hi;
      y40tmp.x2.lo = y40.x2.lo ^ c40.x2.lo;

      c40.x2.hi &= y40.x2.hi;
      c40.x2.lo &= y40.x2.lo;

      c40.x2.hi <<= 1;
      if(c40.x2.lo & MIN_INT32) c40.x2.hi |= 0x1;
      c40.x2.lo <<= 1;

      y40.x2.hi = y40tmp.x2.hi;
      y40.x2.lo = y40tmp.x2.lo;
    }

  /* see if saturation is required */
  /* see if a and b are different sign */
  if(((a^(int32)b40.x2.hi) & MIN_INT32) != 0)
    {
      /* if the sign changed, sat */
      if((a^(int32)y40.x2.hi) & MIN_INT32)
    {
      y40.x2.hi = (a<0) ? MIN_INT40_HI : MAX_INT40_HI;
      y40.x2.lo = (a<0) ? MIN_INT40_LO : MAX_INT40_LO;
      CSR |= DSP_CSR_SAT;
    }
    }

  return(y40.x1);
} /* end of _lssub() function */


uint32 _loll(uint64_ll a)
{
  uint32 y32u;
  union reg64 a64;

  a64.x1u_ll = a;

  y32u = _lo(a64.x1u_d);

  return(y32u);
} /* end of _loll() function */


uint64_d _ltod(uint40 a)
{
  union reg40 a40;
  union reg64 y64;

  a40.x1u = a;

  y64.x2u.hi = (uint32)(a40.x2u.hi & MAX_UINT8);
  y64.x2u.lo = a40.x2u.lo;

  return(y64.x1u_d);
} /* end of _ltod() function */


int32 _mpy(int32 a,int32 b)
{
  int32 y32;
  union reg32 a32,b32;

  a32.x1 = a;
  b32.x1 = b;

  y32 = a32.x2.lo*b32.x2.lo;

  return(y32);
} /* end of _mpy() function */


int32 _mpyh(int32 a,int32 b)
{
  int32 y32;
  union reg32 a32,b32;

  a32.x1 = a;
  b32.x1 = b;

  y32 = a32.x2.hi*b32.x2.hi;

  return(y32);
} /* end of _mpyh() function */


int32 _mpyhsu(int32 a,uint32 b)
{
  int32 y32;
  union reg32 a32,b32;

  a32.x1 = a;
  b32.x1u = b;

  y32 = a32.x2.hi*b32.x2u.hi;

  return(y32);
} /* end of _mpyhsu() function */


uint32 _mpyhu(uint32 a,uint32 b)
{
  uint32 y32u;
  union reg32 a32,b32;

  a32.x1u = a;
  b32.x1u = b;

  y32u = a32.x2u.hi*b32.x2u.hi;

  return(y32u);
} /* end of _mpyhu() function */


int32 _mpyhus(uint32 a,int32 b)
{
  int32 y32;
  union reg32 a32,b32;

  a32.x1u = a;
  b32.x1 = b;

  y32 = a32.x2u.hi*b32.x2.hi;

  return(y32);
} /* end of _mpyhus() function */


int32 _mpyhl(int32 a,int32 b)
{
  int32 y32;
  union reg32 a32,b32;

  a32.x1 = a;
  b32.x1 = b;

  y32 = a32.x2.hi*b32.x2.lo;

  return(y32);
} /* end of _mpyhl() function */


uint32 _mpyhlu(uint32 a,uint32 b)
{
  uint32 y32u;
  union reg32 a32u,b32u;

  a32u.x1 = a;
  b32u.x1 = b;

  y32u = a32u.x2u.hi*b32u.x2u.lo;

  return(y32u);
} /* end of _mpyhlu() function */


int32 _mpyhslu(int32 a,uint32 b)
{
  int32 y32;
  union reg32 a32,b32;

  a32.x1 = a;
  b32.x1u = b;
  
  y32 = a32.x2.hi*b32.x2u.lo;

  return(y32);
} /* end of _mpyhslu() function */


int32 _mpyhuls(uint32 a,int32 b)
{
  int32 y32;
  union reg32 a32,b32;

  a32.x1u = a;
  b32.x1 = b;
  
  y32 = a32.x2u.hi*b32.x2.lo;

  return(y32);
} /* end of _mpyhuls() function */


int32 _mpylh(int32 a,int32 b)
{
  int32 y32;
  union reg32 a32,b32;

  a32.x1 = a;
  b32.x1 = b;
  
  y32 = a32.x2.lo*b32.x2.hi;

  return(y32);
} /* end of _mpylh() function */


uint32 _mpylhu(uint32 a,uint32 b)
{
  uint32 y32u;
  union reg32 a32,b32;

  a32.x1u = a;
  b32.x1u = b;
  
  y32u = a32.x2u.lo*b32.x2u.hi;

  return(y32u);
} /* end of _mpylhu() function */


int32 _mpylshu(int32 a,uint32 b)
{
  int32 y32;
  union reg32 a32,b32;

  a32.x1 = a;
  b32.x1u = b;
  
  y32 = a32.x2.lo*b32.x2u.hi;

  return(y32);
} /* end of _mpylshu() function */


int32 _mpyluhs(uint32 a,int32 b)
{
  int32 y32;
  union reg32 a32,b32;

  a32.x1u = a;
  b32.x1 = b;
  
  y32 = a32.x2u.lo*b32.x2.hi;

  return(y32);
} /* end of _mpyluhs() function */


int32 _mpysu(int32 a,uint32 b)
{
  int32 y32;
  union reg32 a32,b32;

  a32.x1 = a;
  b32.x1u = b;
  
  y32 = a32.x2.lo*b32.x2u.lo;

  return(y32);
} /* end of _mpysu() function */


uint32 _mpyu(uint32 a,uint32 b)
{
  uint32 y32u;
  union reg32 a32,b32;

  a32.x1u = a;
  b32.x1u = b;
  
  y32u = a32.x2u.lo*b32.x2u.lo;

  return(y32u);
} /* end of _mpyu() function */


int32 _mpyus(uint32 a,int32 b)
{
  int32 y32;
  union reg32 a32,b32;

  a32.x1u = a;
  b32.x1 = b;
  
  y32 = a32.x2u.lo*b32.x2.lo;

  return(y32);
} /* end of _mpyus() function */


uint32 _norm(int32 a)
{
  uint32 b32u,m32u,y32u;

  y32u = 0;
  m32u = (uint32)MIN_INT32 >> 1;

  if(a < 0)
    b32u = ~a;
  else
    b32u = a;
  
  if(b32u == 0)
    return(31);

  while((b32u & m32u) != m32u)
    {
      b32u <<= 1;
      y32u++;
    }
  
  return(y32u);
} /* end of _norm() function */


int32 _sadd(int32 a,int32 b)
{
  int32 y32;

  y32 = a+b;

  /* see if saturation is required */
  /* see if a and b have the same sign */
  if(((a^b) & MIN_INT32) == 0)
    {
      /* if the sign changed, sat */
      if((a^y32) & MIN_INT32)
    {
      y32 = (a<0) ? MIN_INT32 : MAX_INT32;
      CSR |= DSP_CSR_SAT;
    }
    }

  return(y32);
} /* end of _sadd() function */


int32 _sat(int40 a)
{
  int32 y32;
  union reg40 a40;

  a40.x1 = a;

  if(a40.x2.hi > 0 || ((a40.x2.hi == 0) && (a40.x2.lo & MIN_INT32)))
    y32 = MAX_INT32;
  else if((a40.x2.hi < 0) && (a40.x2.lo == 0))
    y32 = MIN_INT32;
  else
    y32 = a40.x2.lo;

  return(y32);
} /* end of _sat() function */


uint32 _set(uint32 a,uint32 b,uint32 c)
{
  uint32 k,y32u;

  y32u = a;

  for(k=b;k<=c;k++)
    y32u |= 1 << k;

  return(y32u);
} /* end of _set() function */


uint32 _setr(uint32 a,int32 b)
{
  uint32 start,stop,y32u;

  start = (b >> 5) & 0x1F;
  stop = b & 0x01F;

  y32u = _set(a,start,stop);

  return(y32u);
} /* end of _setr() function */


int32 _smpy(int32 a,int32 b)
{
  int32 y32;
  union reg32 a32,b32;

  a32.x1 = a;
  b32.x1 = b;

  y32 = (a32.x2.lo*b32.x2.lo) << 1;

  if(y32 == MIN_INT32)
    {
      y32 = MAX_INT32;
      CSR |= DSP_CSR_SAT;
    }

  return(y32);
} /* end of _smpy() function */


int32 _smpyh(int32 a,int32 b)
{
  int32 y32;
  union reg32 a32,b32;

  a32.x1 = a;
  b32.x1 = b;

  y32 = (a32.x2.hi*b32.x2.hi) << 1;

  if(y32 == MIN_INT32)
    {
      y32 = MAX_INT32;
      CSR |= DSP_CSR_SAT;
    }

  return(y32);
} /* end of _smpyh() function */


int32 _smpyhl(int32 a,int32 b)
{
  int32 y32;
  union reg32 a32,b32;

  a32.x1 = a;
  b32.x1 = b;

  y32 = (a32.x2.hi*b32.x2.lo) << 1;

  if(y32 == MIN_INT32)
    {
      y32 = MAX_INT32;
      CSR |= DSP_CSR_SAT;
    }

  return(y32);
} /* end of _smpyhl() function */


int32 _smpylh(int32 a,int32 b)
{
  int32 y32;
  union reg32 a32,b32;

  a32.x1 = a;
  b32.x1 = b;

  y32 = (a32.x2.lo*b32.x2.hi) << 1;

  if(y32 == MIN_INT32)
    {
      y32 = MAX_INT32;
      CSR |= DSP_CSR_SAT;
    }

  return(y32);
} /* end of _smpylh() function */


int32 _sshl(int32 a,uint32 b)
{
  int32 tmp;
  int32 y32;

  if(a == 0)
    y32 = 0;
  else if(b > 31)
    {
      if(a > 0)
    {
      y32 = MAX_INT32;
      CSR |= DSP_CSR_SAT;
    }
      else
    {
      y32 = MIN_INT32;
      CSR |= DSP_CSR_SAT;
    }
    }
  else
    {
      if(a > 0)
    {
      tmp = MAX_INT32 >> b;
      if(a > tmp)
        {
          y32 = MAX_INT32;
          CSR |= DSP_CSR_SAT;
        }
      else
        y32 = a << b;
    }
      else
    {
      tmp = MIN_INT32 >> b;
      if(a < tmp)
        {
          y32 = MIN_INT32;
          CSR |= DSP_CSR_SAT;
        }
      else
        y32 = a << b;
    }
    }
    
  return(y32);
} /* end of _sshl() function */


int32 _ssub(int32 a,int32 b)
{
  int32 y32;
  
  y32 = a-b;

  /* see if saturation is required */
  /* see if and b have the same sign */
  if(((a^b) & MIN_INT32) != 0)
    {
      /* if the sign changed, sat */
      if((a^y32) & MIN_INT32)
    {
      y32 = (a<0) ? MIN_INT32 : MAX_INT32;
      CSR |= DSP_CSR_SAT;
    }
    }

  return(y32);
} /* end of _ssub() function */


int32 _sub2(int32 a,int32 b)
{
  union reg32 a32,b32,y32;

  a32.x1 = a;
  b32.x1 = b;

  y32.x2.lo = a32.x2.lo - b32.x2.lo;
  y32.x2.hi = a32.x2.hi - b32.x2.hi;
  return(y32.x1);
} /* end of _sub2() function */


uint32 _subc(uint32 a,uint32 b)
{
  uint32 y32;
  
  if(a >= b)
    y32 = ((a-b) << 1) + 1;
  else
    y32 = a << 1;

  return(y32);
} /* end of _subc() function */
#endif


/***************************************************************************/
/* the additional functions for the TMS320C64X DSP and the TMS320C64+X DSP */

#ifdef TMS320C64X
int32 _abs2(int32 a)
{
  union reg32 a32,y32;

  a32.x1 = a;

  if(a32.x2.lo == MIN_INT16)
    y32.x2.lo = MAX_INT16;
  else if(a32.x2.lo < 0)
    y32.x2.lo = -a32.x2.lo;
  else
    y32.x2.lo = a32.x2.lo;

  if(a32.x2.hi == MIN_INT16)
    y32.x2.hi = MAX_INT16;
  else if(a32.x2.hi < 0)
    y32.x2.hi = -a32.x2.hi;
  else
    y32.x2.hi = a32.x2.hi;

  return(y32.x1);
} /* end of _abs2() function */


int32 _add4(int32 a,int32 b)
{
  union reg32 a32,b32,y32;

  a32.x1 = a;
  b32.x1 = b;

  y32.x4.lo1 = a32.x4.lo1+b32.x4.lo1;
  y32.x4.lo2 = a32.x4.lo2+b32.x4.lo2;
  y32.x4.hi1 = a32.x4.hi1+b32.x4.hi1;
  y32.x4.hi2 = a32.x4.hi2+b32.x4.hi2;

  return(y32.x1);
} /* end of _add4() function */


int32 _avg2(int32 a,int32 b)
{
  union reg32 a32,b32,y32;

  a32.x1 = a;
  b32.x1 = b;

  y32.x2.lo = (a32.x2.lo + b32.x2.lo + 1) >> 1;
  y32.x2.hi = (a32.x2.hi + b32.x2.hi + 1) >> 1;

  return(y32.x1);
} /* end of _avg2() function */


uint32 _avgu4(uint32 a,uint32 b)
{
  union reg32 a32,b32,y32;

  a32.x1u = a;
  b32.x1u = b;

  y32.x4u.lo1 = (a32.x4u.lo1+b32.x4u.lo1+1) >> 1;
  y32.x4u.lo2 = (a32.x4u.lo2+b32.x4u.lo2+1) >> 1;
  y32.x4u.hi1 = (a32.x4u.hi1+b32.x4u.hi1+1) >> 1;
  y32.x4u.hi2 = (a32.x4u.hi2+b32.x4u.hi2+1) >> 1;

  return(y32.x1u);
} /* end of _avgu4() function */


uint32 _bitc4(uint32 a)
{
  uint16 k;
  union reg32 a32,y32;

  a32.x1u = a;

  y32.x1u = 0;

  for(k=0;k<8;k++)
    {
      if(a32.x4u.lo1 & (1 << k)) y32.x4u.lo1++;
      if(a32.x4u.lo2 & (1 << k)) y32.x4u.lo2++;
      if(a32.x4u.hi1 & (1 << k)) y32.x4u.hi1++;
      if(a32.x4u.hi2 & (1 << k)) y32.x4u.hi2++;
    }

  return(y32.x1u);
} /* end of _bitc4() function */


uint32 _bitr(uint32 a)
{
  uint16 i;
  uint32 k,x,y32;

  x = a;
  y32 = 0;

  k = 1 << 31;

  for(i=0;i<32;i++)
    {
      y32=y32+k*(x&0x01);
      k = k >> 1;
      x = x >> 1;
    }

  return(y32);
} /* end of _bitr() function */


int32 _cmpeq2(int32 a,int32 b)
{
  int32 y32;
  union reg32 a32,b32;

  a32.x1 = a;
  b32.x1 = b;

  y32 = 0;

  if(a32.x2.lo == b32.x2.lo) y32 += 1;
  if(a32.x2.hi == b32.x2.hi) y32 += 2;

  return(y32);
} /* end of _cmpeq2() function */


int32 _cmpeq4(int32 a,int32 b)
{
  int32 y32;
  union reg32 a32,b32;

  a32.x1 = a;
  b32.x1 = b;

  y32 = 0;

  if(a32.x4.lo1 == b32.x4.lo1) y32 += 1;
  if(a32.x4.lo2 == b32.x4.lo2) y32 += 2;
  if(a32.x4.hi1 == b32.x4.hi1) y32 += 4;
  if(a32.x4.hi2 == b32.x4.hi2) y32 += 8;

  return(y32);
} /* end of _cmpeq4() function */


int32 _cmpgt2(int32 a,int32 b)
{
  int32 y32;
  union reg32 a32,b32;

  a32.x1 = a;
  b32.x1 = b;

  y32 = 0;

  if(a32.x2.lo > b32.x2.lo) y32 += 1;
  if(a32.x2.hi > b32.x2.hi) y32 += 2;

  return(y32);
} /* end of _cmpgt2() function */


uint32 _cmpgtu4(uint32 a,uint32 b)
{
  uint32 y32u;
  union reg32 a32,b32;

  a32.x1u = a;
  b32.x1u = b;

  y32u = 0;

  if(a32.x4u.lo1 > b32.x4u.lo1) y32u += 1;
  if(a32.x4u.lo2 > b32.x4u.lo2) y32u += 2;
  if(a32.x4u.hi1 > b32.x4u.hi1) y32u += 4;
  if(a32.x4u.hi2 > b32.x4u.hi2) y32u += 8;

  return(y32u);
} /* end of _cmpgtu4() function */


uint32 _deal(uint32 a)
{
  int16 k;
  union reg32 y32;

  y32.x1u = 0;

  for(k=31;k>=0;k-=2)
    {
      if(a & (1 << k)) y32.x2u.hi += 1 << (k >> 1);
      if(a & (1 << (k-1))) y32.x2u.lo += 1 << (k >> 1);
    }
  
  return(y32.x1u);
} /* end of _deal() function */


int32 _dotp2(int32 a,int32 b)
{
  int32 y32;
  union reg32 a32,b32;

  a32.x1 = a;
  b32.x1 = b;

  if((a == 0x80008000) && (b == 0x80008000))
    y32 = MIN_INT32;
  else
    y32 = a32.x2.hi*b32.x2.hi + a32.x2.lo*b32.x2.lo;

  return(y32);
} /* end of _dotp2() function */


int32 _dotpn2(int32 a,int32 b)
{
  int32 y32;
  union reg32 a32,b32;

  a32.x1 = a;
  b32.x1 = b;

  y32 = a32.x2.hi*b32.x2.hi - a32.x2.lo*b32.x2.lo;

  return(y32);
} /* end of _dotpn2() function */


int32 _dotpnrsu2(int32 a,uint32 b)
{
  int32 y32;
  union reg32 a32,b32;

  a32.x1 = a;
  b32.x1u = b;

  y32 = (a32.x2.hi*b32.x2u.hi - a32.x2.lo*b32.x2u.lo + 0x8000) >> 16;

  return(y32);
} /* end of _dotpnrsu2() function */


int32 _dotprsu2(int32 a,uint32 b)
{
  int32 y32;
  union reg32 a32,b32;

  a32.x1 = a;
  b32.x1u = b;

  y32 = (a32.x2.hi*b32.x2u.hi + a32.x2.lo*b32.x2u.lo + 0x8000) >> 16;

  return(y32);
} /* end of _dotprsu2() function */


int32 _dotpsu4(int32 a,uint32 b)
{
  int32 y32;
  union reg32 a32,b32;

  a32.x1 = a;
  b32.x1u = b;

  y32 = a32.x4.hi2*b32.x4u.hi2 + a32.x4.hi1*b32.x4u.hi1 
    + a32.x4.lo2*b32.x4u.lo2 + a32.x4.lo1*b32.x4u.lo1;

  return(y32);
} /* end of _dotpsu4() function */


uint32 _dotpu4(uint32 a,uint32 b)
{
  uint32 y32u;
  union reg32 a32,b32;

  a32.x1u = a;
  b32.x1u = b;

  y32u = a32.x4u.hi2*b32.x4u.hi2 + a32.x4u.hi1*b32.x4u.hi1 
    + a32.x4u.lo2*b32.x4u.lo2 + a32.x4u.lo1*b32.x4u.lo1;

  return(y32u);
} /* end of _dotpu4() function */


int32 _gmpy4(int32 a,int32 b)
{
  uint16 k,m,mask;
  int32 maxpower2;
  uint32 poly,c,ytmp[4];
  union reg32 a32,b32,y32;

  a32.x1 = a;
  b32.x1 = b;

  m = (GFPGFR >> 24) + 1;
  poly = (GFPGFR & 0xFF) | 0x100;

  mask = 1;

  /* zero out the tmp array */
  for(k=0;k<4;k++)
    ytmp[k] = 0;


  /* multiply the four sets of polynomials together */
  for(k=0;k<8;k++)
    {
      c = mask&a32.x4u.hi2;
      ytmp[3] ^= b32.x4u.hi2*c;

      c = mask&a32.x4u.hi1;
      ytmp[2] ^= b32.x4u.hi1*c;

      c = mask&a32.x4u.lo2;
      ytmp[1] ^= b32.x4u.lo2*c;

      c = mask&a32.x4u.lo1;
      ytmp[0] ^= b32.x4u.lo1*c;

      mask <<= 1;
    }

  /* divide each result by the generator polynomial */
  for(k=0;k<4;k++)
    {
      maxpower2 = 30-_norm(ytmp[k]);

      while(maxpower2 >= m)
    {
      c = poly << (maxpower2 - m);
      ytmp[k] ^= c;
      maxpower2 = 30-_norm(ytmp[k]);
    }
    }

  y32.x4u.hi2 = (uint8)ytmp[3];
  y32.x4u.hi1 = (uint8)ytmp[2];
  y32.x4u.lo2 = (uint8)ytmp[1];
  y32.x4u.lo1 = (uint8)ytmp[0];

  return(y32.x1);
} /* end of _gmpy4() function */


uint64_d _ldotp2(int32 a,int32 b)
{
  uint16 k;
  union reg32 a32,b32;
  union reg64 c64,y64,y64tmp;

  a32.x1 = a;
  b32.x1 = b;

  if((a == 0x80008000) && (b == 0x80008000))
    {
      y64.x2.hi = 0x00000000;
      y64.x2.lo = MIN_INT32;
    }
  else
    {
      c64.x2.lo = a32.x2.hi*b32.x2.hi;
      if(c64.x2.lo < 0)
    c64.x2.hi = MAX_UINT32;
      else
    c64.x2.hi = 0;

      y64.x2.lo = a32.x2.lo*b32.x2.lo;
      if(y64.x2.lo < 0)
    y64.x2.hi = MAX_UINT32;
      else
    y64.x2.hi = 0;

      /* perform binary addition */
      for(k=0;k<64;k++)
    {
      y64tmp.x2.hi = y64.x2.hi ^ c64.x2.hi;
      y64tmp.x2.lo = y64.x2.lo ^ c64.x2.lo;

      c64.x2.hi &= y64.x2.hi;
      c64.x2.lo &= y64.x2.lo;

      c64.x2.hi <<= 1;
      if(c64.x2.lo & MIN_INT32) c64.x2.hi |= 0x1;
      c64.x2.lo <<= 1;

      y64.x2.hi = y64tmp.x2.hi;
      y64.x2.lo = y64tmp.x2.lo;
    }
    }

  return(y64.x1_d);
} /* end of _ldotp2() function */


int32 _max2(int32 a,int32 b)
{
  union reg32 a32,b32,y32;

  a32.x1 = a;
  b32.x1 = b;

  if(a32.x2.hi > b32.x2.hi)
    y32.x2.hi = a32.x2.hi;
  else
    y32.x2.hi = b32.x2.hi;

  if(a32.x2.lo > b32.x2.lo)
    y32.x2.lo = a32.x2.lo;
  else
    y32.x2.lo = b32.x2.lo;

  return(y32.x1);
} /* end of _max2() function */


uint32 _maxu4(uint32 a,uint32 b)
{
  union reg32 a32,b32,y32;

  a32.x1u = a;
  b32.x1u = b;

  if(a32.x4u.hi2 > b32.x4u.hi2)
    y32.x4u.hi2 = a32.x4u.hi2;
  else
    y32.x4u.hi2 = b32.x4u.hi2;

  if(a32.x4u.hi1 > b32.x4u.hi1)
    y32.x4u.hi1 = a32.x4u.hi1;
  else
    y32.x4u.hi1 = b32.x4u.hi1;

  if(a32.x4u.lo2 > b32.x4u.lo2)
    y32.x4u.lo2 = a32.x4u.lo2;
  else
    y32.x4u.lo2 = b32.x4u.lo2;

  if(a32.x4u.lo1 > b32.x4u.lo1)
    y32.x4u.lo1 = a32.x4u.lo1;
  else
    y32.x4u.lo1 = b32.x4u.lo1;

  return(y32.x1u);
} /* end of _maxu4() function */


int32 _min2(int32 a,int32 b)
{
  union reg32 a32,b32,y32;

  a32.x1 = a;
  b32.x1 = b;

  if(a32.x2.hi < b32.x2.hi)
    y32.x2.hi = a32.x2.hi;
  else
    y32.x2.hi = b32.x2.hi;

  if(a32.x2.lo < b32.x2.lo)
    y32.x2.lo = a32.x2.lo;
  else
    y32.x2.lo = b32.x2.lo;

  return(y32.x1);
} /* end of _min2() function */


uint32 _minu4(uint32 a,uint32 b)
{
  union reg32 a32,b32,y32;

  a32.x1u = a;
  b32.x1u = b;

  if(a32.x4u.hi2 < b32.x4u.hi2)
    y32.x4u.hi2 = a32.x4u.hi2;
  else
    y32.x4u.hi2 = b32.x4u.hi2;

  if(a32.x4u.hi1 < b32.x4u.hi1)
    y32.x4u.hi1 = a32.x4u.hi1;
  else
    y32.x4u.hi1 = b32.x4u.hi1;

  if(a32.x4u.lo2 < b32.x4u.lo2)
    y32.x4u.lo2 = a32.x4u.lo2;
  else
    y32.x4u.lo2 = b32.x4u.lo2;

  if(a32.x4u.lo1 < b32.x4u.lo1)
    y32.x4u.lo1 = a32.x4u.lo1;
  else
    y32.x4u.lo1 = b32.x4u.lo1;

  return(y32.x1u);
} /* end of _minu4() function */


uint64_d _mpy2(int32 a,int32 b)
{
  union reg32 a32,b32;
  union reg64 y64;

  a32.x1 = a;
  b32.x1 = b;

  y64.x2.hi = a32.x2.hi*b32.x2.hi;
  y64.x2.lo = a32.x2.lo*b32.x2.lo;

  return(y64.x1_d);
} /* end of _mpy2() function */


uint64_ll _mpy2ll(int32 a,int32 b)
{
  union reg64 y64;

  y64.x1_d = _mpy2(a,b);

  return(y64.x1_ll);
} /* end of _mpy2ll() function */

uint64_d _mpyhi(int32 a,int32 b)
{
  int8 neg_y;
  uint16 abs_ahi,abs_ahi_error;
  uint32 abs_b_error;
  uint32 ahi_bhi,ahi_blo;
  uint32 tmp1,tmp2,tmp3;
  uint32 tmp1n,tmp2n,tmp3n;
  union reg32 a32,b32,abs_b32;
  union reg64 y64;

  a32.x1 = a;
  b32.x1 = b;

  if((a32.x2.hi == MIN_INT16) && (b == MIN_INT32))
    {
      y64.x2u.hi = 0x4000;
      y64.x2u.lo = 0x00000000;
    }
  /* Fix for CQ #24738.  Checking for zero here keeps general case  */
  /* code simpler.  Avoids odd check for neg_value * zero.          */
  else if((a32.x2.hi == 0) || (b == 0))
    {
       return 0;
    }
  else
    {
      /* determine sign of the result */
      neg_y = ((a32.x2.hi ^ b32.x2.hi) & MIN_INT16) ? 1 : 0;

      /* compute the absolute values */
      abs_ahi = _abs(a32.x2.hi);
      abs_b32.x1u = _abs(b);

      if(a32.x2.hi == MIN_INT16)
    abs_ahi_error = 1;
      else
    abs_ahi_error = 0;

      if(b == MIN_INT32)
    abs_b_error = 1;
      else
    abs_b_error = 0;

      /* perform the unsigned multiplies */
      ahi_bhi = abs_ahi * abs_b32.x2u.hi;
      ahi_blo = abs_ahi * abs_b32.x2u.lo;

      /* compute the first term */
      tmp1 = (ahi_blo & MAX_UINT16);
      
      /* compute the second term */
      tmp2 = (ahi_bhi & MAX_UINT16) + (ahi_blo >> 16);

      /* compute the third term */
      tmp3 = (ahi_bhi >> 16) + (tmp2 >> 16);

      /* remove any carry over bits */
      tmp2 &= MAX_UINT16;

      /* perform any correction for the error in the abs calculation */
      /* NOTE: only one error can occur due to earlier check         */
      if(abs_ahi_error)
    {
      tmp1 += abs_b32.x2u.lo;
      tmp2 += abs_b32.x2u.hi;

      tmp2 += (tmp1 >> 16);
      tmp3 += (tmp2 >> 16);

      /* remove any carry over bits */
      tmp1 &= MAX_UINT16;
      tmp2 &= MAX_UINT16;
    }
      else if(abs_b_error)
    {
      tmp1 += abs_ahi;

      tmp2 += (tmp1 >> 16);
      tmp3 += (tmp2 >> 16);

      /* remove any carry over bits */
      tmp1 &= MAX_UINT16;
      tmp2 &= MAX_UINT16;
    }

      if(neg_y)
    {
      /* convert to a negative number */
      /* ~x + 1                       */
      tmp1n = tmp1 ^ MAX_UINT16;
      tmp2n = tmp2 ^ MAX_UINT16;
      tmp3n = tmp3 ^ MAX_UINT16;

      tmp1n += 1;
      tmp2n += (tmp1n >> 16);
      tmp3n += (tmp2n >> 16);

      /* remove any carry over bits   */
      tmp1n &= MAX_UINT16;
      tmp2n &= MAX_UINT16;

      y64.x2u.hi = (MAX_UINT16 << 16) | tmp3n;
      y64.x2u.lo = (tmp2n << 16) | tmp1n;
    }
      else
    {
      y64.x2u.hi = tmp3;
      y64.x2u.lo = (tmp2 << 16) | tmp1;
    }
    }

  return(y64.x1u_d);
} /* end of _mpyhi() function */


uint64_ll _mpyhill(int32 a,int32 b)
{
  union reg64 y64;

  y64.x1u_d = _mpyhi(a,b);

  return(y64.x1u_ll);
}/* end of _mpyhill() function */


int32 _mpyhir(int32 a,int32 b)
{
  int8 neg_y;
  uint16 abs_ahi,abs_ahi_error;
  int32 y32;
  uint32 abs_b_error;
  uint32 ahi_bhi,ahi_blo;
  uint32 tmp1,tmp2,tmp3;
  union reg32 a32,b32,abs_b32;

  a32.x1 = a;
  b32.x1 = b;

  if((a32.x2.hi == MIN_INT16) && (b == MIN_INT32))
    {
      y32 = MAX_INT32;
      CSR |= DSP_CSR_SAT;
    }
  else
    {
      /* determine sign of the result */
      neg_y = ((a32.x2.hi ^ b32.x2.hi) & MIN_INT16) ? 1 : 0;

      /* compute the absolute values */
      abs_ahi = _abs(a32.x2.hi);
      abs_b32.x1u = _abs(b);

      if(a32.x2.hi == MIN_INT16)
    abs_ahi_error = 1;
      else
    abs_ahi_error = 0;

      if(b == MIN_INT32)
    abs_b_error = 1;
      else
    abs_b_error = 0;

      /* perform the unsigned multiplies */
      ahi_bhi = abs_ahi * abs_b32.x2u.hi;
      ahi_blo = abs_ahi * abs_b32.x2u.lo;

      /* compute the first term */
      tmp1 = (ahi_blo & MAX_UINT16) + 0x4000;
      
      /* compute the second term */
      tmp2 = (ahi_bhi & MAX_UINT16) + (ahi_blo >> 16) + (tmp1 >> 16);

      /* compute the third term */
      tmp3 = (ahi_bhi >> 16) + (tmp2 >> 16);

      /* remove any carry over bits */
      tmp1 &= MAX_UINT16;
      tmp2 &= MAX_UINT16;

      /* perform any correction for the error in the abs calculation */
      /* NOTE: only one error can occur due to earlier check         */
      if(abs_ahi_error)
    {
      tmp1 += abs_b32.x2u.lo;
      tmp2 += abs_b32.x2u.hi;

      tmp2 += (tmp1 >> 16);
      tmp3 += (tmp2 >> 16);

      /* remove any carry over bits */
      tmp1 &= MAX_UINT16;
      tmp2 &= MAX_UINT16;
    }
      else if(abs_b_error)
    {
      tmp1 += abs_ahi;

      tmp2 += (tmp1 >> 16);
      tmp3 += (tmp2 >> 16);

      /* remove any carry over bits */
      tmp1 &= MAX_UINT16;
      tmp2 &= MAX_UINT16;
    }

      y32 = (tmp3 << (32-15)) + (tmp2 << (16-15)) + (tmp1 >> 15);

      if(neg_y)
    y32 = -y32;
    }

  return(y32);
} /* end of _mpyhir() function */


uint64_d _mpyli(int32 a,int32 b)
{
  int8 neg_y;
  uint16 abs_alo,abs_alo_error;
  uint32 abs_b_error;
  uint32 alo_bhi,alo_blo;
  uint32 tmp1,tmp2,tmp3;
  uint32 tmp1n,tmp2n,tmp3n;
  union reg32 a32,b32,abs_b32;
  union reg64 y64;

  a32.x1 = a;
  b32.x1 = b;

  if((a32.x2.lo == MIN_INT16) && (b == MIN_INT32))
    {
      y64.x2u.hi = 0x4000;
      y64.x2u.lo = 0x00000000;
    }
  /* Fix for CQ #24738.  Checking for zero here keeps general case  */
  /* code simpler.  Avoids odd check for neg_value * zero.          */
  else if((a32.x2.lo == 0) || (b == 0))
    {
       return 0;
    }
  else
    {
      /* determine sign of the result */
      neg_y = ((a32.x2.lo ^ b32.x2.hi) & MIN_INT16) ? 1 : 0;

      /* compute the absolute values */
      abs_alo = _abs(a32.x2.lo);
      abs_b32.x1u = _abs(b);

      if(a32.x2.lo == MIN_INT16)
    abs_alo_error = 1;
      else
    abs_alo_error = 0;

      if(b == MIN_INT32)
    abs_b_error = 1;
      else
    abs_b_error = 0;

      /* perform the unsigned multiplies */
      alo_bhi = abs_alo * abs_b32.x2u.hi;
      alo_blo = abs_alo * abs_b32.x2u.lo;

      /* compute the first term */
      tmp1 = (alo_blo & MAX_UINT16);
      
      /* compute the second term */
      tmp2 = (alo_bhi & MAX_UINT16) + (alo_blo >> 16);

      /* compute the third term */
      tmp3 = (alo_bhi >> 16) + (tmp2 >> 16);

      /* remove any carry over bits */
      tmp2 &= MAX_UINT16;

      /* perform any correction for the error in the abs calculation */
      /* NOTE: only one error can occur due to earlier check         */
      if(abs_alo_error)
    {
      tmp1 += abs_b32.x2u.lo;
      tmp2 += abs_b32.x2u.hi;

      tmp2 += (tmp1 >> 16);
      tmp3 += (tmp2 >> 16);

      /* remove any carry over bits */
      tmp1 &= MAX_UINT16;
      tmp2 &= MAX_UINT16;
    }
      else if(abs_b_error)
    {
      tmp1 += abs_alo;

      tmp2 += (tmp1 >> 16);
      tmp3 += (tmp2 >> 16);

      /* remove any carry over bits */
      tmp1 &= MAX_UINT16;
      tmp2 &= MAX_UINT16;
    }

      if(neg_y)
    {
      /* convert to a negative number */
      /* ~x + 1                       */
      tmp1n = tmp1 ^ MAX_UINT16;
      tmp2n = tmp2 ^ MAX_UINT16;
      tmp3n = tmp3 ^ MAX_UINT16;

      tmp1n += 1;
      tmp2n += (tmp1n >> 16);
      tmp3n += (tmp2n >> 16);

      /* remove any carry over bits   */
      tmp1n &= MAX_UINT16;
      tmp2n &= MAX_UINT16;

      y64.x2u.hi = (MAX_UINT16 << 16) | tmp3n;
      y64.x2u.lo = (tmp2n << 16) | tmp1n;
    }
      else
    {
      y64.x2u.hi = tmp3;
      y64.x2u.lo = (tmp2 << 16) | tmp1;
    }
    }

  return(y64.x1u_d);
} /* end of _mpyli() function */


uint64_ll _mpylill(int32 a,int32 b)
{
  union reg64 y64;

  y64.x1u_d = _mpyli(a,b);

  return(y64.x1u_ll);
} /* end of _mpylill() function */


int32 _mpylir(int32 a,int32 b)
{
  int8 neg_y;
  uint16 abs_alo,abs_alo_error;
  int32 y32;
  uint32 abs_b_error;
  uint32 alo_bhi,alo_blo;
  uint32 tmp1,tmp2,tmp3;
  union reg32 a32,b32,abs_b32;

  a32.x1 = a;
  b32.x1 = b;

  if((a32.x2.lo == MIN_INT16) && (b == MIN_INT32))
    {
      y32 = MAX_INT32;
      CSR |= DSP_CSR_SAT;
    }
  else
    {
      /* determine sign of the result */
      neg_y = ((a32.x2.lo ^ b32.x2.hi) & MIN_INT16) ? 1 : 0;

      /* compute the absolute values */
      abs_alo = _abs(a32.x2.lo);
      abs_b32.x1u = _abs(b);

      if(a32.x2.lo == MIN_INT16)
    abs_alo_error = 1;
      else
    abs_alo_error = 0;

      if(b == MIN_INT32)
    abs_b_error = 1;
      else
    abs_b_error = 0;

      /* perform the unsigned multiplies */
      alo_bhi = abs_alo * abs_b32.x2u.hi;
      alo_blo = abs_alo * abs_b32.x2u.lo;

      /* compute the first term */
      tmp1 = (alo_blo & MAX_UINT16) + 0x4000;
      
      /* compute the second term */
      tmp2 = (alo_bhi & MAX_UINT16) + (alo_blo >> 16) + (tmp1 >> 16);

      /* compute the third term */
      tmp3 = (alo_bhi >> 16) + (tmp2 >> 16);

      /* remove any carry over bits */
      tmp1 &= MAX_UINT16;
      tmp2 &= MAX_UINT16;

      /* perform any correction for the error in the abs calculation */
      /* NOTE: only one error can occur due to earlier check         */
      if(abs_alo_error)
    {
      tmp1 += abs_b32.x2u.lo;
      tmp2 += abs_b32.x2u.hi;

      tmp2 += (tmp1 >> 16);
      tmp3 += (tmp2 >> 16);

      /* remove any carry over bits */
      tmp1 &= MAX_UINT16;
      tmp2 &= MAX_UINT16;
    }
      else if(abs_b_error)
    {
      tmp1 += abs_alo;

      tmp2 += (tmp1 >> 16);
      tmp3 += (tmp2 >> 16);

      /* remove any carry over bits */
      tmp1 &= MAX_UINT16;
      tmp2 &= MAX_UINT16;
    }

      y32 = (tmp3 << (32-15)) + (tmp2 << (16-15)) + (tmp1 >> 15);

      if(neg_y)
    y32 = -y32;
    }

  return(y32);
} /* end of _mpylir() function */


uint64_d _mpysu4(int32 a,uint32 b)
{
  union reg32 a32,b32;
  union reg64 y64;

  a32.x1 = a;
  b32.x1u = b;

  y64.x4u.hi2 = a32.x4.hi2*b32.x4u.hi2;
  y64.x4u.hi1 = a32.x4.hi1*b32.x4u.hi1;
  y64.x4u.lo2 = a32.x4.lo2*b32.x4u.lo2;
  y64.x4u.lo1 = a32.x4.lo1*b32.x4u.lo1;

  return(y64.x1u_d);
} /* end of _mpysu4() function */


uint64_ll _mpysu4ll(int32 a,uint32 b)
{
  union reg64 y64;

  y64.x1u_d = _mpysu4(a,b);

  return(y64.x1u_ll);
} /* end of _mpysu4ll() function */


uint64_d _mpyu4(uint32 a,uint32 b)
{
  union reg32 a32,b32;
  union reg64 y64;

  a32.x1u = a;
  b32.x1u = b;

  y64.x4u.hi2 = a32.x4u.hi2*b32.x4u.hi2;
  y64.x4u.hi1 = a32.x4u.hi1*b32.x4u.hi1;
  y64.x4u.lo2 = a32.x4u.lo2*b32.x4u.lo2;
  y64.x4u.lo1 = a32.x4u.lo1*b32.x4u.lo1;

  return(y64.x1u_d);
} /* end of _mpyu4() function */


uint64_ll _mpyu4ll(uint32 a,uint32 b)
{
  union reg64 y64;

  y64.x1u_d = _mpyu4(a,b);

  return(y64.x1u_ll);
} /* end of _mpyu4ll() function */


int32 _mvd(int32 a)
{
  return(a);
} /* end of _mvd() function */


uint32 _pack2(uint32 a,uint32 b)
{
  union reg32 a32,b32,y32;

  a32.x1u = a;
  b32.x1u = b;

  y32.x2u.hi = a32.x2u.lo;
  y32.x2u.lo = b32.x2u.lo;

  return(y32.x1u);
} /* end of _pack2() function */


uint32 _packh2(uint32 a,uint32 b)
{
  union reg32 a32,b32,y32;

  a32.x1u = a;
  b32.x1u = b;

  y32.x2u.hi = a32.x2u.hi;
  y32.x2u.lo = b32.x2u.hi;

  return(y32.x1u);
} /* end of _packh2() function */


uint32 _packh4(uint32 a,uint32 b)
{
  union reg32 a32,b32,y32;

  a32.x1u = a;
  b32.x1u = b;

  y32.x4u.hi2 = a32.x4u.hi2;
  y32.x4u.hi1 = a32.x4u.lo2;
  y32.x4u.lo2 = b32.x4u.hi2;
  y32.x4u.lo1 = b32.x4u.lo2;

  return(y32.x1u);
} /* end of _packh4() function */


uint32 _packhl2(uint32 a,uint32 b)
{
  union reg32 a32,b32,y32;

  a32.x1u = a;
  b32.x1u = b;

  y32.x2u.hi = a32.x2u.hi;
  y32.x2u.lo = b32.x2u.lo;

  return(y32.x1u);
} /* end of _packhl2() function */


uint32 _packl4(uint32 a,uint32 b)
{
  union reg32 a32,b32,y32;

  a32.x1u = a;
  b32.x1u = b;

  y32.x4u.hi2 = a32.x4u.hi1;
  y32.x4u.hi1 = a32.x4u.lo1;
  y32.x4u.lo2 = b32.x4u.hi1;
  y32.x4u.lo1 = b32.x4u.lo1;

  return(y32.x1u);
} /* end of _packl4() function */


uint32 _packlh2(uint32 a,uint32 b)
{
  union reg32 a32,b32,y32;

  a32.x1u = a;
  b32.x1u = b;

  y32.x2u.hi = a32.x2u.lo;
  y32.x2u.lo = b32.x2u.hi;

  return(y32.x1u);
} /* end of _packlh2() function */

#ifdef _MSC_VER
uint32 _rotl(uint32 a, int32 b)
#else
uint32 _rotl(uint32 a, uint32 b)
#endif
{
  uint16 shift;
  uint32 y;

  /* take 5 LSBs */
  shift = b & 0x1F;
  y = (a << shift) | (a >> (32-shift));

  return(y);
} /* end of _rotl() function */

int32 _sadd2(int32 a,int32 b)
{
  int32 x;
  union reg32 a32,b32,y32;

  a32.x1 = a;
  b32.x1 = b;

  x = a32.x2.hi+b32.x2.hi;
  if(x > MAX_INT16)
    {
      y32.x2.hi = MAX_INT16;
      CSR |= DSP_CSR_SAT;
    }
  else if(x < MIN_INT16)
    {
      y32.x2.hi = MIN_INT16;
      CSR |= DSP_CSR_SAT;
    }
  else
    y32.x2.hi = (int16)x;

  x = a32.x2.lo+b32.x2.lo;
  if(x > MAX_INT16)
    {
      y32.x2.lo = MAX_INT16;
      CSR |= DSP_CSR_SAT;
    }
  else if(x < MIN_INT16)
    {
      y32.x2.lo = MIN_INT16;
      CSR |= DSP_CSR_SAT;
    }
  else
    y32.x2.lo = (int16)x;

  return(y32.x1);
} /* end of _sadd2() function */


uint32 _saddu4(uint32 a,uint32 b)
{
  uint32 x;
  union reg32 a32,b32,y32;

  a32.x1u = a;
  b32.x1u = b;

  x = a32.x4u.hi2+b32.x4u.hi2;
  if(x > MAX_UINT8)
    y32.x4u.hi2 = MAX_UINT8;
  else
    y32.x4u.hi2 = (uint8)x;

  x = a32.x4u.hi1+b32.x4u.hi1;
  if(x > MAX_UINT8)
    y32.x4u.hi1 = MAX_UINT8;
  else
    y32.x4u.hi1 = (uint8)x;

  x = a32.x4u.lo2+b32.x4u.lo2;
  if(x > MAX_UINT8)
    y32.x4u.lo2 = MAX_UINT8;
  else
    y32.x4u.lo2 = (uint8)x;

  x = a32.x4u.lo1+b32.x4u.lo1;
  if(x > MAX_UINT8)
    y32.x4u.lo1 = MAX_UINT8;
  else
    y32.x4u.lo1 = (uint8)x;

  return(y32.x1u);
} /* end of _saddu4() function */


int32 _saddus2(uint32 a,int32 b)
{
  int32 x;
  union reg32 a32,b32,y32;

  a32.x1u = a;
  b32.x1 = b;

  x = a32.x2u.hi+b32.x2.hi;
  if(x > (int32)MAX_UINT16)
    y32.x2.hi = (uint16)MAX_UINT16;
  else if(x < (int32)MIN_UINT16)
    y32.x2.hi = (uint16)MIN_UINT16;
  else
    y32.x2.hi = (uint16)x;

  x = a32.x2u.lo+b32.x2.lo;
  if(x > (int32)MAX_UINT16)
    y32.x2.lo = (uint16)MAX_UINT16;
  else if(x < (int32)MIN_UINT16)
    y32.x2.lo = (uint16)MIN_UINT16;
  else
    y32.x2.lo = (uint16)x;

  return(y32.x1);
} /* end of _saddus2() function */


uint32 _shfl(uint32 a)
{
  int16 k;
  uint32 y32u;
  union reg32 a32;

  a32.x1u = a;

  y32u = 0;

  for(k=31;k>=0;k-=2)
    {
      if(a32.x2u.hi & (1 << (k >> 1))) y32u += 1 << k;
      if(a32.x2u.lo & (1 << (k >> 1))) y32u += 1 << (k-1);
    }
  
  return(y32u);
} /* end of _shfl() function */


uint32 _shlmb(uint32 a,uint32 b)
{
  union reg32 a32,b32,y32;

  a32.x1u = a;
  b32.x1u = b;

  y32.x4u.hi2 = b32.x4u.hi1;
  y32.x4u.hi1 = b32.x4u.lo2;
  y32.x4u.lo2 = b32.x4u.lo1;
  y32.x4u.lo1 = a32.x4u.hi2;

  return(y32.x1u);
} /* end of _shlmb() function */


int32 _shr2(int32 a,uint32 b)
{
  union reg32 a32,y32;

  a32.x1 = a;

  y32.x2.hi = a32.x2.hi >> b;
  y32.x2.lo = a32.x2.lo >> b;
  
  return(y32.x1);
} /* end of _shr2() function */


uint32 _shrmb(uint32 a,uint32 b)
{
  union reg32 a32,b32,y32;

  a32.x1u = a;
  b32.x1u = b;

  y32.x4u.hi2 = a32.x4u.lo1;
  y32.x4u.hi1 = b32.x4u.hi2;
  y32.x4u.lo2 = b32.x4u.hi1;
  y32.x4u.lo1 = b32.x4u.lo2;

  return(y32.x1u);
} /* end of _shrmb() function */


uint32 _shru2(uint32 a,uint32 b)
{
  union reg32 a32,y32;

  a32.x1u = a;

  y32.x2u.hi = a32.x2u.hi >> b;
  y32.x2u.lo = a32.x2u.lo >> b;
  
  return(y32.x1u);
} /* end of _shru2() function */


uint64_d _smpy2(int32 a,int32 b)
{
  int32 x;
  union reg32 a32,b32;
  union reg64 y64;

  a32.x1 = a;
  b32.x1 = b;

  x = (a32.x2.lo*b32.x2.lo) << 1;
  if(x != MIN_INT32)
    y64.x2.lo = x;
  else
    {
      y64.x2.lo = MAX_INT32;
      CSR |= DSP_CSR_SAT;
    }

  x = (a32.x2.hi*b32.x2.hi) << 1;
  if(x != MIN_INT32)
    y64.x2.hi = x;
  else
    {
      y64.x2.hi = MAX_INT32;
      CSR |= DSP_CSR_SAT;
    }

  return(y64.x1_d);
} /* end of _smpy2() function */


uint64_ll _smpy2ll(int32 a,int32 b)
{
  union reg64 y64;

  y64.x1_d = _smpy2(a,b);

  return(y64.x1_ll);
} /* end of _smpy2ll() function */


int32 _spack2(int32 a,int32 b)
{
  union reg32 y32;

  if(a > MAX_INT16)
    y32.x2.hi = MAX_INT16;
  else if(a < MIN_INT16)
    y32.x2.hi = MIN_INT16;
  else
    y32.x2.hi = (int16)a;

  if(b > MAX_INT16)
    y32.x2.lo = MAX_INT16;
  else if(b < MIN_INT16)
    y32.x2.lo = MIN_INT16;
  else
    y32.x2.lo = (int16)b;

  return(y32.x1);
} /* end of _spack2() function */


uint32 _spacku4(int32 a,int32 b)
{
  union reg32 a32,b32;
  union reg32 y32;

  a32.x1 = a;
  b32.x1 = b;

  if(a32.x2.hi > (int16)MAX_UINT8)
    y32.x4u.hi2 = (uint8)MAX_UINT8;
  else if(a32.x2.hi < (int16)MIN_UINT8)
    y32.x4u.hi2 = (uint8)MIN_UINT8;
  else
    y32.x4u.hi2 = (uint8)a32.x2.hi;

  if(a32.x2.lo > (int16)MAX_UINT8)
    y32.x4u.hi1 = (uint8)MAX_UINT8;
  else if(a32.x2.lo < (int16)MIN_UINT8)
    y32.x4u.hi1 = (uint8)MIN_UINT8;
  else
    y32.x4u.hi1 = (uint8)a32.x2.lo;

  if(b32.x2.hi > (int16)MAX_UINT8)
    y32.x4u.lo2 = (uint8)MAX_UINT8;
  else if(b32.x2.hi < (int16)MIN_UINT8)
    y32.x4u.lo2 = (uint8)MIN_UINT8;
  else
    y32.x4u.lo2 = (uint8)b32.x2.hi;

  if(b32.x2.lo > (int16)MAX_UINT8)
    y32.x4u.lo1 = (uint8)MAX_UINT8;
  else if(b32.x2.lo < (int16)MIN_UINT8)
    y32.x4u.lo1 = (uint8)MIN_UINT8;
  else
    y32.x4u.lo1 = (uint8)b32.x2.lo;

  return(y32.x1u);
} /* end of _spacku4() function */


int32 _sshvl(int32 a,int32 b)
{
  int32 tmp;
  int32 y32;

  if(a == 0)
    y32 = 0;
  else if((b > 31) || (b < -31))
    {
      if(a > 0)
    {
      y32 = MAX_INT32;
      CSR |= DSP_CSR_SAT;
    }
      else
    {
      y32 = MIN_INT32;
      CSR |= DSP_CSR_SAT;
    }
    }
  else
    {
      if(a > 0)
    {
      if(b > 0)
        {
          tmp = MAX_INT32 >> b;

          if(a > tmp)
        {
          y32 = MAX_INT32;
          CSR |= DSP_CSR_SAT;
        }
          else
        y32 = a << b;
        }
      else
        y32 = a >> (-b);
    }
      else
    {
      if(b > 0)
        {
          tmp = MIN_INT32 >> b;

          if(a < tmp)
        {
          y32 = MIN_INT32;
          CSR |= DSP_CSR_SAT;
        }
          else
        y32 = a << b;
        }
      else
        y32 = a >> (-b);
    }
    }

  return(y32);
} /* end of _sshvl() function */


int32 _sshvr(int32 a,int32 b)
{
  int32 tmp;
  int32 y32;

  if(a == 0)
    y32 = 0;
  else if((b > 31) || (b < -31))
    {
      if(a > 0)
    {
      y32 = MAX_INT32;
      CSR |= DSP_CSR_SAT;
    }
      else
    {
      y32 = MIN_INT32;
      CSR |= DSP_CSR_SAT;
    }
    }
  else
    {
      if(a > 0)
    {
      if(b < 0)
        {
          tmp = MAX_INT32 >> (-b);

          if(a > tmp)
        {
          y32 = MAX_INT32;
          CSR |= DSP_CSR_SAT;
        }
          else
        y32 = a << (-b);
        }
      else
        y32 = a >> b;
    }
      else
    {
      if(b < 0)
        {
          tmp = MIN_INT32 >> (-b);

          if(a < tmp)
        {
          y32 = MIN_INT32;
          CSR |= DSP_CSR_SAT;
        }
          else
        y32 = a << (-b);
        }
      else
        y32 = a >> b;
    }
    }

  return(y32);
} /* end of _sshvr() function */


int32 _sub4(int32 a,int32 b)
{
  union reg32 a32,b32,y32;

  a32.x1 = a;
  b32.x1 = b;

  y32.x4.lo1 = a32.x4.lo1-b32.x4.lo1;
  y32.x4.lo2 = a32.x4.lo2-b32.x4.lo2;
  y32.x4.hi1 = a32.x4.hi1-b32.x4.hi1;
  y32.x4.hi2 = a32.x4.hi2-b32.x4.hi2;

  return(y32.x1);
} /* end of _sub4() function */


int32 _subabs4(int32 a,int32 b)
{
  int32 x;
  union reg32 a32,b32,y32;

  a32.x1u = a;
  b32.x1u = b;

  x = (int32)a32.x4u.hi2-(int32)b32.x4u.hi2;
  if(x < (MIN_INT8 << 1))
    y32.x4u.hi2 = MAX_UINT8;
  else if(x < 0)
    y32.x4u.hi2 = (uint8)(-x);
  else
    y32.x4u.hi2 = (uint8)x;

  x = (int32)a32.x4u.hi1-(int32)b32.x4u.hi1;
  if(x < (MIN_INT8 << 1))
    y32.x4u.hi1 = MAX_UINT8;
  else if(x < 0)
    y32.x4u.hi1 = (uint8)(-x);
  else
    y32.x4u.hi1 = (uint8)x;

  x = (int32)a32.x4u.lo2-(int32)b32.x4u.lo2;
  if(x < (MIN_INT8 << 1))
    y32.x4u.lo2 = MAX_UINT8;
  else if(x < 0)
    y32.x4u.lo2 = (uint8)(-x);
  else
    y32.x4u.lo2 = (uint8)x;

  x = (int32)a32.x4u.lo1-(int32)b32.x4u.lo1;
  if(x < (MIN_INT8 << 1))
    y32.x4u.lo1 = MAX_UINT8;
  else if(x <  0)
    y32.x4u.lo1 = (uint8)(-x);
  else
    y32.x4u.lo1 = (uint8)x;

  return(y32.x1);
} /* end of _subabs4() function */


uint32 _swap4(uint32 a)
{
  union reg32 a32,y32;

  a32.x1u = a;

  y32.x4u.hi2 = a32.x4u.hi1;
  y32.x4u.hi1 = a32.x4u.hi2;
  y32.x4u.lo2 = a32.x4u.lo1;
  y32.x4u.lo1 = a32.x4u.lo2;

  return(y32.x1u);
} /* end of _swap4() function */


uint32 _unpkhu4(uint32 a)
{
  union reg32 a32,y32;

  a32.x1u = a;

  y32.x1u = 0;

  y32.x4u.hi1 = a32.x4u.hi2;
  y32.x4u.lo1 = a32.x4u.hi1;

  return(y32.x1u);
} /* end of _unpkhu4() function */


uint32 _unpklu4(uint32 a)
{
  union reg32 a32,y32;

  a32.x1u = a;

  y32.x1u = 0;

  y32.x4u.hi1 = a32.x4u.lo2;
  y32.x4u.lo1 = a32.x4u.lo1;

  return(y32.x1u);
} /* end of _unpklu4() function */


uint32 _xpnd2(uint32 a)
{
  union reg32 y32;

  y32.x1u = 0;

  if(a & 0x02) y32.x2u.hi = MAX_UINT16;
  if(a & 0x01) y32.x2u.lo = MAX_UINT16;

  return(y32.x1u);
} /* end of _xpnd2() function */


uint32 _xpnd4(uint32 a)
{
  union reg32 y32;

  y32.x1u = 0;

  if(a & 0x08) y32.x4u.hi2 = MAX_UINT8;
  if(a & 0x04) y32.x4u.hi1 = MAX_UINT8;
  if(a & 0x02) y32.x4u.lo2 = MAX_UINT8;
  if(a & 0x01) y32.x4u.lo1 = MAX_UINT8;

  return(y32.x1u);
} /* end of _xpnd4() function */
#endif


/***************************************************************************/
/* the additional functions for the TMS320C64+X DSP                        */

#ifdef TMS320C64PX
uint64_ll _addsub(int32 a,int32 b)
{
  union reg64 y64;

  y64.x2.lo = a-b;
  y64.x2.hi = a+b;

  return(y64.x1u_ll);
} /* end of _addsub() function */


uint64_ll _addsub2(uint32 a,uint32 b)
{
  union reg64 y64;

  y64.x2.lo = _sub2(a,b);
  y64.x2.hi = _add2(a,b);

  return(y64.x1u_ll);
} /* end of _addsub2() function */


uint64_ll _cmpy(uint32 a,uint32 b)
{
  int32 hi,lo;
  union reg32 a32,b32;
  union reg64 y64;

  a32.x1 = a;
  b32.x1 = b;

  if((a == 0x80008000) && (b == 0x80008000))
    {
      lo = MAX_INT32;
      hi = 0;
      CSR |= DSP_CSR_SAT;
    }
  else
    {
      lo = a32.x2.lo*b32.x2.hi + a32.x2.hi*b32.x2.lo;
      hi = a32.x2.hi*b32.x2.hi - a32.x2.lo*b32.x2.lo;
    }

  y64.x2.hi = hi;
  y64.x2.lo = lo;

  return(y64.x1u_ll);
} /* end of _cmpy() function */


uint32 _cmpyr(uint32 a,uint32 b)
{
  int32 hi,lo;
  union reg32 a32,b32,y32;

  a32.x1u = a;
  b32.x1u = b;

  if((a == 0x80008000) && (b == 0x80008000))
    {
      lo = MAX_INT32;
      hi = 0;
      CSR |= DSP_CSR_SAT;
    }
  else
    {
      lo = a32.x2.lo*b32.x2.hi + a32.x2.hi*b32.x2.lo;
      hi = a32.x2.hi*b32.x2.hi - a32.x2.lo*b32.x2.lo;
    }

  if(lo >= 0x7FFF8000)
    {
      y32.x2.lo = MAX_INT16;
      CSR |= DSP_CSR_SAT;
    }
  else
    y32.x2.lo = (lo + 0x8000) >> 16;

  if(hi >= 0x7FFF8000)
    {
      y32.x2.hi = MAX_INT16;
      CSR |= DSP_CSR_SAT;
    }
  else
    y32.x2.hi = (hi + 0x8000) >> 16;

  return(y32.x1u);
} /* end of _cmpyr() function */


uint32 _cmpyr1(uint32 a,uint32 b)
{
  int32 hi,lo;
  union reg32 a32,b32,y32;

  a32.x1u = a;
  b32.x1u = b;

  if((a == 0x80008000) && (b == 0x80008000))
    {
      lo = MAX_INT32;
      hi = 0;
      CSR |= DSP_CSR_SAT;
   }
  else
    {
      lo = a32.x2.lo*b32.x2.hi + a32.x2.hi*b32.x2.lo;
      hi = a32.x2.hi*b32.x2.hi - a32.x2.lo*b32.x2.lo;
    }

  /*--------------------------------------------------------------------------
  Saturation, on C6X, occurs when a value, computed in a 40-bit ALU, is too
  big to be represented within 32-bits.  Even on this instruction, which only
  keeps the upper 16-bits of two different computations.  The check for
  saturation here occurs a bit earlier than on the C6X, so that the check can
  still be done in 32-bit host math.  The check occurs after the main
  computation, but before the addition of 0x4000 and left shift by 1.  So,
  the constants compared for saturation are computed:
     val : first value, in 33-bit representation, too big to fit in 32-bits
     constant = (val >> 1) - 0x4000
  --------------------------------------------------------------------------*/

  if(lo >= (int32)0x3FFFC000)       // (0x080000000 >> 1) - 0x4000
    {
      y32.x2.lo = MAX_INT16;
      CSR |= DSP_CSR_SAT;
    }
  else if (lo <= (int32)0xBFFFBFFF) // (0x17FFFFFFF >> 1) - 0x4000
    {
      y32.x2.lo = MIN_INT16;
      CSR |= DSP_CSR_SAT;
    }
  else
    y32.x2.lo = (lo + 0x4000) >> (16-1);

  if(hi >= (int32)0x3FFFC000)       // (0x080000000 >> 1) - 0x4000h
    {
      y32.x2.hi = MAX_INT16;
      CSR |= DSP_CSR_SAT;
    }
  else if (hi <= (int32)0xBFFFBFFF) // 0x17FFFFFFF >> 1) - 0x4000
    {
      y32.x2.hi = MIN_INT16;
      CSR |= DSP_CSR_SAT;
    }
  else
    y32.x2.hi = (hi + 0x4000) >> (16-1);

  return(y32.x1u);
} /* end of _cmpyr1() function */


uint64_ll _ddotp4(uint32 a,uint32 b)
{
  union reg32 a32,b32;
  union reg64 y64;

  a32.x1u = a;
  b32.x1u = b;

  y64.x2.lo = a32.x2.hi*b32.x4.lo2+a32.x2.lo*b32.x4.lo1;
  y64.x2.hi = a32.x2.hi*b32.x4.hi2+a32.x2.lo*b32.x4.hi1;

  return(y64.x1u_ll);
} /* end of _ddotp4() function */


uint64_ll _ddotph2(uint64_ll a,uint32 b)
{
  union reg32 b32;
  union reg64 a64,y64;

  a64.x1u_ll = a;
  b32.x1u = b;

  if((a64.x4.hi2 == MIN_INT16) && 
     (a64.x4.hi1 == MIN_INT16) && 
     (b == 0x80008000))
    {
      y64.x2.hi = MAX_INT32;
      CSR |= DSP_CSR_SAT;
    }
  else
    y64.x2.hi = a64.x4.hi2*b32.x2.hi + a64.x4.hi1*b32.x2.lo;

  if((a64.x4.hi1 == MIN_INT16) && 
     (a64.x4.lo2 == MIN_INT16) && 
     (b == 0x80008000))
    {
      y64.x2.lo = MAX_INT32;
      CSR |= DSP_CSR_SAT;
    }
  else
    y64.x2.lo = a64.x4.hi1*b32.x2.hi + a64.x4.lo2*b32.x2.lo;

  return(y64.x1u_ll);
} /* end of _ddotph2() function */


uint32 _ddotph2r(uint64_ll a,uint32 b)
{
  int32 tmp1,tmp2;
  union reg32 b32,y32;
  union reg64 a64;

  a64.x1u_ll = a;
  b32.x1u = b;

  if((a64.x4.hi2 == MIN_INT16) && 
     (a64.x4.hi1 == MIN_INT16) && 
     (b == 0x80008000))
    {
      tmp1 = MAX_INT32;
      CSR |= DSP_CSR_SAT;
    }
  else
    {
      tmp1 = a64.x4.hi2*b32.x2.hi + a64.x4.hi1*b32.x2.lo;
      if((tmp1 >> 16) < MAX_INT16)
    tmp1 += 0x00008000;
    }

  if((a64.x4.hi1 == MIN_INT16) && 
     (a64.x4.lo2 == MIN_INT16) && 
     (b == 0x80008000))
    {
      tmp2 = MAX_INT32;
      CSR |= DSP_CSR_SAT;
    }
  else
    {
    tmp2 = a64.x4.hi1*b32.x2.hi + a64.x4.lo2*b32.x2.lo;
      if((tmp2 >> 16) < MAX_INT16)
    tmp2 += 0x00008000;
    }

  y32.x2.hi = tmp1 >> 16;
  y32.x2.lo = tmp2 >> 16;

  return(y32.x1u);
} /* end of _ddotph2r() function */


uint64_ll _ddotpl2(uint64_ll a,uint32 b)
{
  union reg32 b32;
  union reg64 a64,y64;

  a64.x1u_ll = a;
  b32.x1u = b;

  if((a64.x4.lo2 == MIN_INT16) && 
     (a64.x4.lo1 == MIN_INT16) &&
     (b == 0x80008000))
    {
      y64.x2.lo = MAX_INT32;
      CSR |= DSP_CSR_SAT;
    }
  else
    y64.x2.lo = a64.x4.lo2*b32.x2.hi + a64.x4.lo1*b32.x2.lo;

  if((a64.x4.hi1 == MIN_INT16) && 
     (a64.x4.lo2 == MIN_INT16) && 
     (b == 0x80008000))
    {
      y64.x2.hi = MAX_INT32;
      CSR |= DSP_CSR_SAT;
    }
  else
    y64.x2.hi = a64.x4.hi1*b32.x2.hi + a64.x4.lo2*b32.x2.lo;

  return(y64.x1u_ll);
} /* end of _ddotpl2() function */


uint32 _ddotpl2r(uint64_ll a,uint32 b)
{
  int32 tmp1,tmp2;
  union reg32 b32,y32;
  union reg64 a64;

  a64.x1u_ll = a;
  b32.x1u = b;

  if((a64.x4.lo2 == MIN_INT16) && 
     (a64.x4.lo1 == MIN_INT16) && 
     (b == 0x80008000))
    {
      tmp1 = MAX_INT32;
      CSR |= DSP_CSR_SAT;
    }
  else
    {
      tmp1 = a64.x4.lo2*b32.x2.hi + a64.x4.lo1*b32.x2.lo;
      if((tmp1 >> 16) < MAX_INT16)
    tmp1 += 0x00008000;
    }

  if((a64.x4.hi1 == MIN_INT16) && 
     (a64.x4.lo2 == MIN_INT16) && 
     (b == 0x80008000))
    {
      tmp2 = MAX_INT32;
      CSR |= DSP_CSR_SAT;
    }
  else
    {
      tmp2 = a64.x4.hi1*b32.x2.hi + a64.x4.lo2*b32.x2.lo;
      if((tmp2 >> 16) < MAX_INT16)
    tmp2 += 0x00008000;
    }

  y32.x2.hi = tmp2 >> 16;
  y32.x2.lo = tmp1 >> 16;

  return(y32.x1u);
} /* end of _ddotpl2r() function */


uint64_ll _dmv(uint32 a,uint32 b)
{
  union reg64 y64;

  y64.x2u.hi = a;
  y64.x2u.lo = b;
  
  return(y64.x1u_ll);
} /* end of _dmv() function */


uint64_ll _dpack2(uint32 a,uint32 b)
{
  union reg64 y64;

  y64.x2u.lo = _pack2(a,b);
  y64.x2u.hi = _packh2(a,b);

  return(y64.x1u_ll);
} /* end of _dpack2() function */


uint64_ll _dpackx2(uint32 a,uint32 b)
{
  union reg64 y64;

  y64.x2u.lo = _packlh2(a,b);
  y64.x2u.hi = _packlh2(b,a);

  return(y64.x1u_ll);
} /* end of _dpackx2() function */


uint32 _gmpy(uint32 a,uint32 b)
{
  uint32 k,mask;
  uint32 poly;
  uint32 y,ytmp;

  y = 0;

  poly = GPLYA;
  /*  poly = GPLYB; */

  mask = 0x0100;

  for(k=0;k<8;k++)
    {
      if(b & mask)
    y ^= a;
      mask >>= 1;
      ytmp = y << 1;

      if(y & MIN_INT32)
    y = poly ^ ytmp;
      else
    y = ytmp;
    }

  if(b & 0x01)
    y ^= a;

  return(y);
} /* end of _gmpy() function */


uint64_ll _mpy2ir(uint32 a,int32 b)
{
  union reg64 y64;

  y64.x2u.lo = _mpylir(a,b);
  y64.x2u.hi = _mpyhir(a,b);

  return(y64.x1u_ll);
} /* end of _mpy2ir() function */


int32 _mpy32(int32 a,int32 b)
{
  int32 y32;
  union reg64 y64;

  y64.x1u_ll = _mpy32ll(a,b);

  y32 = y64.x2.lo;

  return(y32);
} /* end of _mpy32() function */



uint64_ll _mpy32ll(int32 a,int32 b)
{
  int8 neg_y;
  uint32 abs_a_error;
  uint32 abs_b_error;
  uint32 ahi_bhi,ahi_blo,alo_bhi,alo_blo;
  uint32 tmp1,tmp2,tmp3,tmp4;
  uint32 tmp1n,tmp2n,tmp3n,tmp4n;
  union reg32 a32,b32,abs_a32,abs_b32;
  union reg64 y64;

  a32.x1 = a;
  b32.x1 = b;

  if((a == MIN_INT32) && (b == MIN_INT32))
    {
      y64.x2.hi = 0x40000000;
      y64.x2.lo = 0x00000000;
    }
  else
    {
      /* determine the sign of the result */
      neg_y = ((a32.x2.hi ^ b32.x2.hi) & MIN_INT16) ? 1 : 0;
  
      /* compute the absolute values */
      abs_a32.x1u = _abs(a);
      abs_b32.x1u = _abs(b);

      /* denote any errors in abs value */
      if(a == MIN_INT32)
    abs_a_error = 1;
      else
    abs_a_error = 0;

      if(b == MIN_INT32)
    abs_b_error = 1;
      else
    abs_b_error = 0;

      /* perform unsigned multiplications */
      ahi_bhi = abs_a32.x2u.hi * abs_b32.x2u.hi;
      ahi_blo = abs_a32.x2u.hi * abs_b32.x2u.lo;
      alo_bhi = abs_a32.x2u.lo * abs_b32.x2u.hi;
      alo_blo = abs_a32.x2u.lo * abs_b32.x2u.lo;

      /* compute the first term */
      tmp1 = (alo_blo & MAX_UINT16);

      /* compute the second term */
      tmp2 = (ahi_blo & MAX_UINT16) + (alo_bhi & MAX_UINT16) + (alo_blo >> 16);

      /* compute the third term */
      tmp3 = (ahi_bhi & MAX_UINT16) + (ahi_blo >> 16) + (alo_bhi >> 16) + (tmp2 >> 16);

      /* compute the fourth term */
      tmp4 = (ahi_bhi >> 16) + (tmp3 >> 16);

      /* remove any carry over bits */
      tmp2 &= MAX_UINT16;
      tmp3 &= MAX_UINT16;

      /* perform any correction for the error in the abs calculation */
      /* NOTE: only one error can occur due to earlier check         */
      if(abs_a_error)
    {
      tmp1 += abs_b32.x2u.lo;
      tmp2 += abs_b32.x2u.hi;

      tmp2 += (tmp1 >> 16);
      tmp3 += (tmp2 >> 16);
      tmp4 += (tmp3 >> 16);

      /* remove any carry over bits */
      tmp1 &= MAX_UINT16;
      tmp2 &= MAX_UINT16;
      tmp3 &= MAX_UINT16;
    }
      else if(abs_b_error)
    {
      tmp1 += abs_a32.x2u.lo;
      tmp2 += abs_a32.x2u.hi;

      tmp2 += (tmp1 >> 16);
      tmp3 += (tmp2 >> 16);
      tmp4 += (tmp3 >> 16);

      /* remove any carry over bits */
      tmp1 &= MAX_UINT16;
      tmp2 &= MAX_UINT16;
      tmp3 &= MAX_UINT16;
    }

      if(neg_y)
    {
      /* convert to a negative number */
      /* ~x + 1                       */
      tmp1n = tmp1 ^ MAX_UINT16;
      tmp2n = tmp2 ^ MAX_UINT16;
      tmp3n = tmp3 ^ MAX_UINT16;
      tmp4n = tmp4 ^ MAX_UINT16;

      tmp1n += 1;
      tmp2n += (tmp1n >> 16);
      tmp3n += (tmp2n >> 16);
      tmp4n += (tmp3n >> 16);

      /* remove any carry over bits   */
      tmp1n &= MAX_UINT16;
      tmp2n &= MAX_UINT16;
      tmp3n &= MAX_UINT16;

      y64.x2.hi = (tmp4n << 16) | tmp3n;
      y64.x2.lo = (tmp2n << 16) | tmp1n;
    }
      else
    {
      y64.x2.hi = (tmp4 << 16) | tmp3;
      y64.x2.lo = (tmp2 << 16) | tmp1;
    }
    }

  return(y64.x1u_ll);
} /* end of _mpy32ll() function */


uint64_ll _mpy32su(int32 a,uint32 b)
{
  int8 neg_y;
  uint32 abs_a_error;
  uint32 ahi_bhi,ahi_blo,alo_bhi,alo_blo;
  uint32 tmp1,tmp2,tmp3,tmp4;
  uint32 tmp1n,tmp2n,tmp3n,tmp4n;
  union reg32 a32,b32,abs_a32;
  union reg64 y64;

  a32.x1 = a;
  b32.x1u = b;

  /* determine the sign of the result */
  neg_y = (a32.x2.hi & MIN_INT16) ? 1 : 0;
  
  /* compute the absolute value */
  abs_a32.x1u = _abs(a);

  /* denote any errors in abs value */
  if(a == MIN_INT32)
    abs_a_error = 1;
  else
    abs_a_error = 0;

  /* perform unsigned multiplications */
  ahi_bhi = abs_a32.x2u.hi * b32.x2u.hi;
  ahi_blo = abs_a32.x2u.hi * b32.x2u.lo;
  alo_bhi = abs_a32.x2u.lo * b32.x2u.hi;
  alo_blo = abs_a32.x2u.lo * b32.x2u.lo;

  /* compute the first term */
  tmp1 = (alo_blo & MAX_UINT16);

  /* compute the second term */
  tmp2 = (ahi_blo & MAX_UINT16) + (alo_bhi & MAX_UINT16) + (alo_blo >> 16);

  /* compute the third term */
  tmp3 = (ahi_bhi & MAX_UINT16) + (ahi_blo >> 16) + (alo_bhi >> 16) + (tmp2 >> 16);

  /* compute the fourth term */
  tmp4 = (ahi_bhi >> 16) + (tmp3 >> 16);

  /* remove any carry over bits */
  tmp2 &= MAX_UINT16;
  tmp3 &= MAX_UINT16;

  /* perform any correction for the error in the abs calculation */
  if(abs_a_error)
    {
      tmp1 += b32.x2u.lo;
      tmp2 += b32.x2u.hi;

      tmp2 += (tmp1 >> 16);
      tmp3 += (tmp2 >> 16);
      tmp4 += (tmp3 >> 16);

      /* remove any carry over bits */
      tmp1 &= MAX_UINT16;
      tmp2 &= MAX_UINT16;
      tmp3 &= MAX_UINT16;
    }

  if(neg_y)
    {
      /* convert to a negative number */
      /* ~x + 1                       */
      tmp1n = tmp1 ^ MAX_UINT16;
      tmp2n = tmp2 ^ MAX_UINT16;
      tmp3n = tmp3 ^ MAX_UINT16;
      tmp4n = tmp4 ^ MAX_UINT16;

      tmp1n += 1;
      tmp2n += (tmp1n >> 16);
      tmp3n += (tmp2n >> 16);
      tmp4n += (tmp3n >> 16);

      /* remove any carry over bits   */
      tmp1n &= MAX_UINT16;
      tmp2n &= MAX_UINT16;
      tmp3n &= MAX_UINT16;

      y64.x2u.hi = (tmp4n << 16) | tmp3n;
      y64.x2u.lo = (tmp2n << 16) | tmp1n;
    }
  else
    {
      y64.x2u.hi = (tmp4 << 16) | tmp3;
      y64.x2u.lo = (tmp2 << 16) | tmp1;
    }

  return(y64.x1u_ll);
} /* end of _mpy32su() function */


uint64_ll _mpy32u(uint32 a,uint32 b)
{
  uint32 ahi_bhi,ahi_blo,alo_bhi,alo_blo;
  uint32 tmp1,tmp2,tmp3,tmp4;
  union reg32 a32,b32;
  union reg64 y64;

  a32.x1u = a;
  b32.x1u = b;

  /* perform unsigned multiplications */
  ahi_bhi = a32.x2u.hi * b32.x2u.hi;
  ahi_blo = a32.x2u.hi * b32.x2u.lo;
  alo_bhi = a32.x2u.lo * b32.x2u.hi;
  alo_blo = a32.x2u.lo * b32.x2u.lo;

  /* compute the first term */
  tmp1 = (alo_blo & MAX_UINT16);

  /* compute the second term */
  tmp2 = (ahi_blo & MAX_UINT16) + (alo_bhi & MAX_UINT16) + (alo_blo >> 16);

  /* compute the third term */
  tmp3 = (ahi_bhi & MAX_UINT16) + (ahi_blo >> 16) + (alo_bhi >> 16) + (tmp2 >> 16);

  /* compute the fourth term */
  tmp4 = (ahi_bhi >> 16) + (tmp3 >> 16);

  /* remove any carry over bits */
  tmp2 &= MAX_UINT16;
  tmp3 &= MAX_UINT16;

  y64.x2u.hi = (tmp4 << 16) | tmp3;
  y64.x2u.lo = (tmp2 << 16) | tmp1;

  return(y64.x1u_ll);
} /* end of _mpy32u() function */


uint64_ll _mpy32us(uint32 a,int32 b)
{
  union reg64 y64;

  y64.x1u_ll = _mpy32su(b,a);

  return(y64.x1u_ll);
} /* end of _mpy32us() function */


uint32 _rpack2(int32 a,int32 b)
{
  union reg32 y32;

  if(a > (MAX_INT32 >> 1))
    { 
      y32.x2.hi = MAX_INT16;
      CSR |= DSP_CSR_SAT;
    }
  else if(a < (MIN_INT32 >> 1))
    {
      y32.x2.hi = MIN_INT16;
      CSR |= DSP_CSR_SAT;
    }
  else
    y32.x2.hi = ((a << 1) >> 16) & MAX_UINT16;

  if(b > (MAX_INT32 >> 1)) 
    {
      y32.x2.lo = MAX_INT16;
      CSR |= DSP_CSR_SAT;
    }
  else if(b < (MIN_INT32 >> 1))
    {
      y32.x2.lo = MIN_INT16;
      CSR |= DSP_CSR_SAT;
    }
  else
    y32.x2.lo = ((b << 1) >> 16) & MAX_UINT16;

  return(y32.x1u);
} /* end of _rpack2() function */


uint64_ll _saddsub(int32 a,int32 b)
{
  union reg64 y64;

  y64.x2u.lo = _ssub(a,b);
  y64.x2u.hi = _sadd(a,b);

  return(y64.x1u_ll);
} /* end of _saddsub() function */


uint64_ll _saddsub2(uint32 a,uint32 b)
{
  union reg64 y64;

  y64.x2u.lo = _ssub2(a,b);
  y64.x2u.hi = _sadd2(a,b);

  return(y64.x1u_ll);
} /* end of _saddsub2() function */


uint64_ll _shfl3(uint32 a,uint32 b)
{
  int32 i;
  int32 b0,b1,b2;
  union reg64 y64;

  b0 = b & MAX_UINT16;
  b1 = a & MAX_UINT16;
  b2 = (a >> 16) & MAX_UINT16;

  y64.x2u.hi = 0;
  y64.x2u.lo = 0;

  /* compute bits 0 to 31 */
  for(i=0;i<11;i++)
    {
      y64.x2u.lo |= ((b0 >> i) & 0x1) << (3*i+0);
      y64.x2u.lo |= ((b1 >> i) & 0x1) << (3*i+1);
      if(i < 10)
    y64.x2u.lo |= ((b2 >> i) & 0x1) << (3*i+2);
    }

  /* compute bit 32 */
  /* NOTE: set i=10 */
  y64.x2u.hi |= ((b2 >> 10) & 0x1) << (3*10+2-32);

  /* compute bits 33 to 47 */
  for(i=11;i<16;i++)
    {
      y64.x2u.hi |= ((b0 >> i) & 0x1) << (3*i+0-32);
      y64.x2u.hi |= ((b1 >> i) & 0x1) << (3*i+1-32);
      y64.x2u.hi |= ((b2 >> i) & 0x1) << (3*i+2-32);
    }

  return(y64.x1u_ll);
} /* end of _shfl3() function */

int32 _smpy32(int32 a,int32 b)
{
  int8 neg_y;
  uint32 abs_a_error;
  uint32 abs_b_error;
  uint32 ahi_bhi,ahi_blo,alo_bhi,alo_blo;
  uint32 tmp1,tmp2,tmp3,tmp4;
  uint32 tmp1n,tmp2n,tmp3n,tmp4n;
  int32 y32;
  union reg32 a32,b32,abs_a32,abs_b32;

  a32.x1 = a;
  b32.x1 = b;

  if((a == MIN_INT32) && (b == MIN_INT32))
    {
      y32 = MAX_INT32;
      CSR |= DSP_CSR_SAT;
    }
  /* Fix for CQ #24738.  Checking for zero here keeps general case  */
  /* code simpler.  Avoids odd check for neg_value * zero.          */
  else if((a == 0) || (b == 0))
    {
       return 0;
    }
  else
    {
      a32.x1 = a;
      b32.x1 = b;

      /* determine the sign of the result */
      neg_y = ((a32.x2.hi ^ b32.x2.hi) & MIN_INT16) ? 1 : 0;
  
      /* compute the absolute values */
      abs_a32.x1u = _abs(a);
      abs_b32.x1u = _abs(b);

      /* denote any errors in abs value */
      if(a == MIN_INT32)
    abs_a_error = 1;
      else
    abs_a_error = 0;

      if(b == MIN_INT32)
    abs_b_error = 1;
      else
    abs_b_error = 0;

      /* perform unsigned multiplications */
      ahi_bhi = abs_a32.x2u.hi * abs_b32.x2u.hi;
      ahi_blo = abs_a32.x2u.hi * abs_b32.x2u.lo;
      alo_bhi = abs_a32.x2u.lo * abs_b32.x2u.hi;
      alo_blo = abs_a32.x2u.lo * abs_b32.x2u.lo;

      /* compute the first term */
      tmp1 = (alo_blo & MAX_UINT16);

      /* compute the second term */
      tmp2 = (ahi_blo & MAX_UINT16) + 
    (alo_bhi & MAX_UINT16) + (alo_blo >> 16);

      /* compute the third term */
      tmp3 = (ahi_bhi & MAX_UINT16) + 
    (ahi_blo >> 16) + (alo_bhi >> 16) + (tmp2 >> 16);

      /* compute the fourth term */
      tmp4 = (ahi_bhi >> 16) + (tmp3 >> 16);

      /* remove any carry over bits */
      tmp2 &= MAX_UINT16;
      tmp3 &= MAX_UINT16;

      /* perform any correction for the error in the abs calculation */
      /* NOTE: only one error can occur due to earlier check         */
      if(abs_a_error)
    {
      tmp1 += abs_b32.x2u.lo;
      tmp2 += abs_b32.x2u.hi;

      tmp2 += (tmp1 >> 16);
      tmp3 += (tmp2 >> 16);
      tmp4 += (tmp3 >> 16);

      /* remove any carry over bits */
      tmp1 &= MAX_UINT16;
      tmp2 &= MAX_UINT16;
      tmp3 &= MAX_UINT16;
    }
      else if(abs_b_error)
    {
      tmp1 += abs_a32.x2u.lo;
      tmp2 += abs_a32.x2u.hi;

      tmp2 += (tmp1 >> 16);
      tmp3 += (tmp2 >> 16);
      tmp4 += (tmp3 >> 16);

      /* remove any carry over bits */
      tmp1 &= MAX_UINT16;
      tmp2 &= MAX_UINT16;
      tmp3 &= MAX_UINT16;
    }

      if(neg_y)
    {
      /* convert to a negative number */
      /* ~x + 1                       */
      tmp1n = tmp1 ^ MAX_UINT16;
      tmp2n = tmp2 ^ MAX_UINT16;
      tmp3n = tmp3 ^ MAX_UINT16;
      tmp4n = tmp4 ^ MAX_UINT16;

      tmp1n += 1;
      tmp2n += (tmp1n >> 16);
      tmp3n += (tmp2n >> 16);
      tmp4n += (tmp3n >> 16);
      
      /* remove any carry over bits   */
      tmp1n &= MAX_UINT16;
      tmp2n &= MAX_UINT16;
      tmp3n &= MAX_UINT16;

      if(tmp4n > MAX_UINT16)
        y32 = MIN_INT32;
      else
        /* Fix for BZ 1583: Insure the MSB of tmp3n is not thrown away. */
        /* Note tmp3n is not anded with MAX_UINT16.                     */
        /* Fix for BZ 1602: Copy the MSB of tmp2n into the LSB of y32.  */
        y32 =   (((tmp4n << 1) & MAX_UINT16) << 16) | (tmp3n << 1)
              | ((tmp2n & 0x8000) != 0);
    }
      else
    {
      if(tmp4 > (MAX_INT16 >> 1))
        y32 = MAX_INT32;
      else
        /* Fix for BZ 1583. Insure the MSB of tmp3 is not thrown away. */
        /* Note tmp3 is not anded with MAX_UINT16.                     */
        /* Fix for BZ 1602: Copy the MSB of tmp2 into the LSB of y32.  */
        y32 =   (((tmp4 << 1) & MAX_UINT16) << 16) | (tmp3 << 1)
              | ((tmp2 & 0x8000) != 0);
    }
    }

  return(y32);
} /* end of _smpy32() function */


int32 _ssub2(int32 a,int32 b)
{
  int32 x;
  union reg32 a32,b32,y32;

  a32.x1 = a;
  b32.x1 = b;

  x = a32.x2.hi-b32.x2.hi;
  if(x > MAX_INT16)
    {
      y32.x2.hi = MAX_INT16;
      CSR |= DSP_CSR_SAT;
    }
  else if(x < MIN_INT16)
    {
      y32.x2.hi = MIN_INT16;
      CSR |= DSP_CSR_SAT;
    }
  else
    y32.x2.hi = (int16)x;

  x = a32.x2.lo-b32.x2.lo;
  if(x > MAX_INT16)
    {
      y32.x2.lo = MAX_INT16;
      CSR |= DSP_CSR_SAT;
    }
  else if(x < MIN_INT16)
    {
      y32.x2.lo = MIN_INT16;
      CSR |= DSP_CSR_SAT;
    }
  else
    y32.x2.lo = (int16)x;

  return(y32.x1);
} /* end of _ssub2() function */


uint32 _xormpy(uint32 a,uint32 b)
{
  uint32 i;
  uint32 mask,y;

  y = 0;
  mask = 0x100;

  for(i=0;i<8;i++)
    {
      if(b & mask)
    y ^= a;
      mask >>= 1;
      y <<= 1;
    }

  if(b & 0x1)
    y ^= a;
  
  return(y);
} /* end of _xormpy() function */

#endif

/*
 *  Automated Revision Information
 *  Changed: $Date: 2008-12-10 11:17:41 -0600 (Wed, 10 Dec 2008) $
 *  Revision: $Revision: 7621 $
 */

