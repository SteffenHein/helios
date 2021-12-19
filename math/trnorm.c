/* [ file: trnorml.c ] */
# define DO_TRNORM "trnorm(*)"
/*******************************************************************************
*                                                                              *
*   Function trnorm(*); HELIOS, release v1.0r1                                 *
*   [ Heat and Electric Losses In Ordinary wave guiding Structures ]           *
*                                                                              *
*   This function returns the normalized temperature maximum tnorm(ratio) in   *
*   a rectangular waveguide of width to heigth ratio = a/b, filled with any    *
*   homogeneous lossy dielectric, and running under TE10 mode operation.       *
*   The normalized temperature maximum is related to the physical temperature  *
*   maximum tmax [ viz. to the maximum temperature difference relative to      *
*   uniform temperature on the waveguide walls ] in a dielectric of relative   *
*   permittivity epsr, loss factor tgd, and heat conductivity hc by            *
*                                                                              *
*            tmax = c*tnorm,  with c := k*( a * Emax )^2 / hc                  *
*   where                                                                      *
*                   k = 2.*PI*frq*EPS_VAC*epsr*tgd                             *
*                                                                              *
*   denotes the loss current conductivity of the dielectric at frequency frq.  *
*   The degree of approximation is transferred to the calling program as the   *
*   [ short ] integer 'order'.                                                 *
*                                                                              *
*   (C) SHEIN; Munich, December 2021                       Steffen Hein        *
*   [ Update: December 17, 2021 ]                       <contact@sfenx.de>     *
*                                                                              *
*******************************************************************************/
# include <stdio.h>
# include <stdlib.h>
# include <stddef.h>
/*----------------------------------------------------------------------------*/
# include "../math/maths.h"
# include "../math/consts.h"
/*----------------------------------------------------------------------------*/
# define GRN_SMOOTH ( 5.e-03 )
# ifndef TRN_DIMNS1 
   # define TRN_DIMNS1 5000
# endif
# ifndef TRN_DIMNS2 
   # define TRN_DIMNS2 100
# endif
/*============================================================================*/

double trnorm ( short order, double ratio )
{
   static const double
      cst = ( 8./( PI*PI*PI*PI ));
	
   static short
      ii = null,
      jj = null,
      kk = null;

   static double
      ab2 = ZERO,
      mm0 = ZERO,
      mm2 = ZERO,
      nn0 = ZERO,
      nn2 = ZERO,
      nab = ZERO,
      xx = ZERO,
      yy = ZERO,
      tq = ZERO;
      
   static double
      fac[TRN_DIMNS1] = {ZERO},
      fbc[TRN_DIMNS1] = {ZERO};
/*
      fcf[TRN_DIMNS2][TRN_DIMNS2] = {{ZERO}};
*/
/*----------------------------------------------------------------------------*/
   if (( ratio < .25 )||( 10. < ratio ))
   {
      fprintf( stderr, "\n Error message from function %s:", DO_TRNORM );
      fprintf( stderr, "\n Waveguide width to height ratio a/b = %.12e ",
         ratio );
      fprintf( stderr, " out of domain !" );
      fprintf( stderr, "\n [ The legal domain is .25 <= ratio <= 10.]\n" );

      exit( EXIT_FAILURE );
   };

   tq = ZERO;
   ab2 = ratio*ratio;

   ii = null; do
   {
      mm0 = ( double ) 2.*ii + ONE;
      mm2 = mm0*mm0;
      xx = mm2 - 2.;
      yy = mm0*( mm2 - 4. );
      fac[ii] = xx / yy;
      fbc[ii] = 1. / mm0;

      jj = null; do
      {
         mm0 = 1. + ( double ) 2.*jj;
	 mm2 = mm0*mm0;
	 kk = ii-jj;
	 nn0 = 1. + ( double ) 2.*kk;
	 nn2 = nn0*nn0;
	 nab = ab2*nn2;
	 tq += ( fac[jj]*fbc[kk]/( mm2 + nab ));
/*
         if (( jj < TRN_DIMNS2 )&&( kk < TRN_DIMNS2 ))
            fcf[jj][kk] = tq;
*/
      } while (( ++jj ) <= ii );
   } while (( ++ii ) < order );
   tq = cst*tq;
/*----------------------------------------------------------------------------*/

   return tq;
}
/*============================================================================*/
/************************* end of function trnorm(*) **************************/

