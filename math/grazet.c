# define DO_GRAZET "grazet(*)"
/*******************************************************************************
*                                                                              *
*   Function grazet(*); HELIOS, release v1.0r1                                 *
*   [ Heat and Electric Losses In Ordinary wave guiding Structures ]           *
*                                                                              *
*   This function returns the heat conduction coefficient zeta( gr )           *
*   for a system of horizontal coaxial cylinders.                              *
*   The range of the diameter ratio is 1.2 < Da/Di= ratio < 8.1.               *
*   - Values due to BECKMANN: Forschung Bd.2, Heft 5 (1931); pp.170-178        *
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
/*============================================================================*/

double grazet ( double gr, double ratio )
{
/* declarations: */

   static const double
      xx0 = 3.6,
      xx1 = 5.2,
      xx2 = 6.8,
      yy0 = 0.9,
      yy1 = 0.1,
      yy2 = 0.09,
      dnm0 = 0.128, /* for ratio = Da/Di = 2.063 */
      dnm1 = 0.152; /* for ratio = Da/Di = 5.790 */

   static double
      zeta = ZERO, /* heat conduction coefficient [ returned ] */
      aa = ZERO,
      bb = ZERO,
      xx = ZERO,
      yy = ZERO,
      apr = ZERO,
      bpr = ZERO,
      cn0 = ZERO,
      cn1 = ZERO,
      qt0 = ZERO,
      qt1 = ZERO,
      ep1 = ZERO,
      anm = ZERO,
      bnm = ZERO,
      cnm = ZERO,
      dnm = ZERO,
      xhr = ZERO,
      xxs = ZERO,
      xlm = ZERO,
      xlm0 = ZERO,
      xlm1 = ZERO,
      ylm = ZERO;

/* prototypes: */

   double log( double xx );
   double log10( double xx );
   double pow( double xx, double ep );
/*----------------------------------------------------------------------------*/

   qt0 = ( yy0 - yy1 ) / ( xx0 - xx1 ); 
   qt1 = ( yy1 - yy2 ) / ( xx1 - xx2 ); 
   cn0 = yy0 - xx0*qt0;
   cn1 = yy1 - xx1*qt1;
   cnm = .258;
/*............................................................................*/
   apr = 5.790;             /* xhr determines the upper bound of pure */
   bpr = 2.063;             /* heat conduction and radiation regime */

   aa = ( apr - ratio )/( apr - bpr ); /* absence of convection for gr < xhr */
   bb = 1. - aa;

   dnm = aa*dnm0 + bb*dnm1;

   if ( ratio < 2.063 )
   {
      apr = 2.063;
      bpr = 1.200;

      aa = (apr - ratio )/( apr-bpr );
      bb = 1.- aa;
      xhr = aa*4.9 + bb*2.8;
   }
   else
      xhr = aa*2.8 + bb*0.9;
/*............................................................................*/
/* xlm is the minimum value beyond that log(zeta) = f( log(gr), ratio ) */ 
/* can be treated as linear in log(gr */

   bnm = log10( dnm*log( ratio ));
   xxs = log10( gr );

   ylm = qt0*xxs + cn0;       /* ylm = f( xlm, ratio ) */
   xx = qt1*xxs + cn1;      

   if ( ylm < xx )
      ylm = xx;
   
   xlm0 = ( cn0 - bnm )/( cnm -qt0 );    
   xlm1 = ( cn1 - bnm )/( cnm -qt1 );    

   if ( xlm1 <= xlm0 )
      xlm = xlm0;
   else
      xlm = xlm1;

   yy = cnm*xxs + bnm;

   if ( yy <= ylm )
   {
      if ( xhr < xxs )
      {
         ep1 = cnm*( xhr - xlm )/( bnm + cnm*xhr );
         anm = ( cnm / ep1 ) / pow (( xlm-xhr ), ( ep1 - 1. ));
         yy += ( anm*pow (( xlm- xxs ), ep1 ));
      }
      else
         yy = ZERO;
   };

   zeta = pow( 10., yy );

   return zeta;
}
/*============================================================================*/
# undef DO_GRAZET
/************************** end of function grazet(*) *************************/
