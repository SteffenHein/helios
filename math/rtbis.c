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
# include <stdio.h>
# include <stddef.h>
# include <stdlib.h>
/*----------------------------------------------------------------------------*/
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
/*
# include "shutil.h"
*/
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
      "\n Too many bisections in funtion %s !\n",
      DO_RTBIS );
   return ZERO;
}
/*============================================================================*/
# undef RTB_ERROR
# undef RTB_ERRTRN
# undef RTB_MAXITR
/*************************** end of function rtbis ****************************/









# define DO_ZBRACV "zbracv(*)"
/*******************************************************************************
*                                                                              *
*  ANSI C function zbracv(*); release 1.0.0                                    *
*                                                                              *
*  Given a function funct and an initial guessed range xx1, xx2, this routine  *
*  expands the range geometrically until a root is bracketed by the returned   *
*  values xx1 and xx2 [ in which case zbrac returns 1 ] or until the range     *
*  becomes unacceptably large [ in which case zbrac returns null ].            *
*  In option opt=1 [-1] the range is expanded 'right-hand' [left-hand], viz.   *
*  only in positive [negative] direction, while in option opt=0 the range may  *
*  be expanded in either directions of the real line.                          *
*                                                                              *
*  [ Modified zbrac(*) from: Press et al., Numerical Recipes in C, chap 9.1 ]. *
*                                                                              *
*  SPINNER GmbH., Munich 08 July 2002                                          *
*  [ Update: 15 July 2002 ]                                                    *
*                                                                              *
*******************************************************************************/
# ifndef FACTOR
   # define FACTOR 1.6
# endif
# ifndef NBRTRY
   # define NBRTRY 1000
# endif
/*----------------------------------------------------------------------------*/
/*
# include <stdio.h>
# include <stddef.h>
# include <math.h>
*/
/*----------------------------------------------------------------------------*/
/*
# include "consts.h"
*/
/*----------------------------------------------------------------------------*/

/*============================================================================*/

short zbracv ( double ( *funct )( double ),
               double *xx1, double *xx2,
	       signed char opt )
{
   static long
      ii = null;

   static double
      ff1 = ZERO,
      ff2 = ZERO;

/*----------------------------------------------------------------------------*/

   if ( *xx1 == *xx2 )
   {
      fprintf( stderr, "\n Bad initial range in function %s !\n ", DO_ZBRACV );
      return -ONE;
   }
   else if ( *xx2 < *xx1 )
   {
      fprintf( stderr, "\n Bad initial range in function %s !", DO_ZBRACV );
      fprintf( stderr, "\n [ function requires *xx1 <= *xx2.]\n " );
      return -ONE;
   }
   else if (( null < opt )
          &&( *xx1 < ZERO ))
   {
      fprintf( stderr, "\n Bad initial range in function %s !", DO_ZBRACV );
      fprintf( stderr, "\n [ function requires 0 < *xx1 in\n" );
      fprintf( stderr, "\n   called option ( null < opt ).]\n " );
      return -ONE;
   }
   else if (( opt < null )
          &&( ZERO < *xx2 ))
   {
      fprintf( stderr, "\n Bad initial range in function %s !", DO_ZBRACV );
      fprintf( stderr, "\n [ function requires *xx2 < 0 in\n" );
      fprintf( stderr, "\n   called option ( opt < null ).]\n " );
      return -ONE;
   };
   
   ff1 = ( *funct )( *xx1 );
   ff2 = ( *funct )( *xx2 );

   ii = null; do
   {
      if (( ff1*ff2 ) < ZERO )
         return 1;

      switch ( opt ) 
      {
        default:
         if ( fabs( ff1 ) < fabs ( ff2 ))
         {
            ( *xx1 ) -= ( FACTOR*(( *xx2 ) - ( *xx1 )));
            ff1 = ( *funct )( *xx1 );
         }
         else
         {
            ( *xx2 ) += ( FACTOR*(( *xx2 ) - ( *xx1 )));
            ff2 = ( *funct )( *xx2 );
         };
         break;

        case (+1):
         if ( fabs( ff1 ) < fabs ( ff2 ))
         {
            ( *xx1 ) /= FACTOR;
            ff1 = ( *funct )( *xx1 );
         }
         else
         {
            ( *xx2 ) += ( FACTOR*(( *xx2 ) - ( *xx1 )));
            ff2 = ( *funct )( *xx2 );
         };
         break;

        case (-1):
         if ( fabs( ff1 ) < fabs ( ff2 ))
         {
            ( *xx1 ) -= ( FACTOR*(( *xx2 ) - ( *xx1 )));
            ff1 = ( *funct )( *xx1 );
         }
         else
         {
            ( *xx2 ) /= FACTOR;
            ff2 = ( *funct )( *xx2 );
         };
         break;

        case (+2):
         break;
         if ( fabs( ff1 ) < fabs ( ff2 ))
         {
            ( *xx1 ) /= 10.;
            ff1 = ( *funct )( *xx1 );
         }
         else
         {
            ( *xx2 ) += ( FACTOR*(( *xx2 ) - ( *xx1 )));
            ff2 = ( *funct )( *xx2 );
         };

        case (-2):
         if ( fabs( ff1 ) < fabs ( ff2 ))
         {
            ( *xx1 ) -= ( FACTOR*(( *xx2 ) - ( *xx1 )));
            ff1 = ( *funct )( *xx1 );
         }
         else
         {
            ( *xx2 ) /= 10.;
            ff2 = ( *funct )( *xx2 );
         };
      };
   } while (( ++ii ) < NBRTRY );
   return null;
}
/*============================================================================*/
# undef DO_ZBRACV
# undef FACTOR
# undef NBRTRY
/************************ end of function zbrac(*) ****************************/









# define DO_ZBRAKL "zbrakl(*)"
/*******************************************************************************
*                                                                              *
*  ANSI C function zbrakl; release 1.0.0                                       *
*                                                                              *
*  Given a function funct defined on the interval [xx1, xx2], subdivide the    *
*  interval into n equally spaced segments, and search for zero crossings of   *
*  the function. lb is iput as the maximum number of roots sought, and is      *
*  reset to the number of bracketing pairs xb1[1..lb], xb2[1..lb] that are     *
*  found.                                                                      *
*  [ Modified zbrak(*) from: Press et al., Numerical Recipes in C, chap 9.1,   *
*    version using long integer type ll ]                                      *
*                                                                              *
*  SPINNER GmbH., Munich 10 July 2002                                          *
*  [ Update: 11 July 2002 ]                                                    *
*                                                                              *
*******************************************************************************/

/*----------------------------------------------------------------------------*/

/*============================================================================*/

long zbrakl ( double ( *funct )( double ),
              double xx1, double xx2, long ll,
              double *xb1, double *xb2, long *lb )
{
   static long
      ii = null,
      lbb = null;

   static double
      xx = ZERO,
      xx_ = ZERO,
      fp = ZERO,
      fc = ZERO,
      dx = ZERO;
/*----------------------------------------------------------------------------*/
   lbb = null;

   dx = ( xx2 - xx1 ) / ll; /* determine the spacing approriate to the mesh */

   xx = xx1;
   fp = ( *funct )( xx );

   ii = null;               /* loop over all intervalls */ 
   while(( ++ii ) <= ll )
   {
      xx_ = xx;
      xx += dx;
      fc = ( *funct )( xx );

      if (( fc*fp ) < ZERO ) /* if a sign change occurs then record values */
      {                       /* of the bounds */
         xb1[(++lbb)] = xx_;
         xb2[lbb] = xx;

         if ( *lb == lbb )
            return lbb;
      };
      fp = fc;
   };
   *lb = lbb;
   return lbb;
}
/*============================================================================*/

/*********************** end of function zbrakl(*) ****************************/
/************************* end of file rtbis.c ********************************/
