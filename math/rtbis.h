# define DO_RTBIS "rtbis(*)"
/*******************************************************************************
*                                                                              *
*   Function rtbis(*); HELIOS, release v1.0r1                                  *
*   [ Heat and Electric Losses In Ordinary wave guiding Structures ]           *
*                                                                              *
*   Using bisection, find the root of a function func known to lie between     *
*   xx1 and xx2.                                                               *
*   The root, returned as rtbis, will be defined until acurracy is +i xacc     *
*   [ Largely following: Press et al., Numerical Recipes in C, chap 9.1 ]      *
*                                                                              *
*   (C) SHEIN; Munich, December 2021                       Steffen Hein        *
*   [ Update: December 17, 2021 ]                       <contact@sfenx.de>     *
*                                                                              *
*******************************************************************************/
# include "../math/maths.h"
# include "../math/consts.h"
/*----------------------------------------------------------------------------*/
# ifndef RTB_MAXITR
   # define RTB_MAXITR 1000
# endif
# ifndef GIANT_VAL
   # define GIANT_VAL ( double )( 1.e+277 )
# endif
# define RTB_ERROR  1             /* 1: do not exit on error and */
# if RTB_ERROR == 1               /* return value RTB_ERRTRN instead */
   # define RTB_ERRTRN ( HUGE_VALF )
# endif
/*----------------------------------------------------------------------------*/
# include "shutil.h"
/*============================================================================*/

double \
rtbis( double ( *funct ) ( double ), double xx1, double xx2, double xacc )
{
   static long
      ii = null;

   static double
      dx = ZERO,
      ff = ZERO,
      fmid = ZERO, 
      xmid = ZERO,
      rtb = ZERO;

   ff = ( *funct )( xx1 );
   if ( GIANT_VAL <= fabs( ff ))
      return HUGE_VALF;

   fmid = ( *funct ) ( xx2 );
   if ( GIANT_VAL <= fabs( fmid ))
      return HUGE_VALF;

   if ( ZERO <= ( ff*fmid ))
   {
# if RTB_ERROR == 0
      fprintf( stderr,
         "\n Root must be bracketed for bisection in function %s !\n",
         DO_RTBIS );
      exit( EXIT_FAILURE );
# else
      return RTB_ERRTRN;
# endif
   };

/* orient the search so that 0 < ff: */

   rtb = ff < ZERO ? ( dx = xx2 - xx1, xx1 ) : ( dx = xx1 - xx2, xx2 );

   ii = null; do
   {
      fmid = ( *funct )( xmid = rtb + ( dx *= 0.5 ));
      if ( GIANT_VAL <= fabs( fmid ))
         return HUGE_VALF;

      if ( fmid <= ZERO )
         rtb = xmid;

      if (( fabs( dx ) < xacc )|| ( fmid == ZERO ))
         return rtb;
   } while (( ++ii ) < RTB_MAXITR );

   fprintf( stderr,
      "\n Too many bisections in funtion %s !\n", DO_RTBIS );

   return ZERO;
}
/*============================================================================*/
# undef RTB_ERROR
# undef RTB_ERRTRN
# undef RTB_MAXITR
/*************************** end of function rtbis ****************************/
