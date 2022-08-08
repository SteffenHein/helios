/* [ file: garnus.c ] */
# define DO_GRANUS "granus(*)"
/*******************************************************************************
*                                                                              *
*   Function granus(*); HELIOS, release v1.0r1                                 *
*   [ Heat and Electric Losses In Ordinary wave guiding Structures ]           *
*                                                                              *
*   This function returns the nusselt number Nu for a horizontal cylinder as   *
*   function of the Grashof number gr, in the domain                           *
*                                                                              *
*                     1.e-04  <=  gr  <= 1.e+08                                *
*                                                                              *
*   The values are due to Prandtl, Fuehrer durch die Stroemungslehre,          *
*   chapter 5.16.                                                              *
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
/*============================================================================*/

double granus ( double gr )
{
   static double
      nu = ZERO;

   static double
      aa0 = ZERO,
      aa1 = ZERO,
      aa2 = ZERO,
      aa3 = ZERO,
      ll0 = ZERO,
      ll1 = ZERO,
      xx0 = ZERO,
      xx1 = ZERO,
      bound1 = 1.e+05*( 1.+ GRN_SMOOTH ),
      bound2 = 1.e+05*( 1.- GRN_SMOOTH ),
      bound3 = 1.e+02*( 1.+ GRN_SMOOTH ),
      bound4 = 1.e+02*( 1.- GRN_SMOOTH ),
      bound5 = 1.e-01*( 1.+ GRN_SMOOTH ),
      bound6 = 1.e-01*( 1.- GRN_SMOOTH ),
      bound7 = 1.e-04;
/*----------------------------------------------------------------------------*/
   if ( gr <= ZERO )
   {
/*
      fprintf( stderr, "\n Error message from function %s:", DO_GRANUS );
      fprintf( stderr, "\n Illegal non-positive Grashof number !\n" );

      exit ( EXIT_FAILURE );
*/
      return ZERO;
   };

   xx1 = sqrt( sqrt( gr ));
   xx0 = xx1;

   if ( bound1 <= gr )
   {
      nu = .395*xx1; /* Ludwig Prandtl, 'Fuehrer durch die Stroemungslehre' */
                     /* p 390 f : Nu = .395*gr^.25 */
      return nu;
   }
   else if (( bound2 <= gr )&&( gr < bound1 ))
   {
      ll0 = ( gr - bound2 ) / ( bound1 - bound2 );
      ll1 = 1.- ll0;

      xx0 *= .395;

      aa0 =  1.036258122680e+00;
      aa1 =  3.740943660128e-01;
      aa2 = -4.319246132012e-03;
      aa3 =  1.247228303931e-04;

      nu = aa0 + xx1*( aa1 + xx1*( aa2 + xx1*aa3 ));
      nu = ll0*xx0 + ll1*nu;

      return nu;
   }
   else if (( bound3 <= gr )&&( gr < bound2 ))
   {
      aa0 =  1.036258122680e+00;
      aa1 =  3.740943660128e-01;
      aa2 = -4.319246132012e-03;
      aa3 =  1.247228303931e-04;

      nu = aa0 + xx1*( aa1 + xx1*( aa2 + xx1*aa3 ));
      return nu;
   }
   else if (( bound4 <= gr )&&( gr < bound3 ))
   {
      ll0 = ( gr - bound4 ) / ( bound3 - bound4 );
      ll1 = 1.- ll0;

      aa0 =  1.036258122680e+00;
      aa1 =  3.740943660128e-01;
      aa2 = -4.319246132012e-03;
      aa3 =  1.247228303931e-04;
      
      xx0 = aa0 + xx1*( aa1 + xx1*( aa2 + xx1*aa3 ));

      aa0 =  3.625104307573e-01;
      aa1 =  8.127569162631e-01;
      aa2 = -7.526734702040e-02;

      nu = aa0 + xx1*( aa1 + xx1*aa2 );
      nu = ll0*xx0 + ll1*nu;

      return nu;
   }
   else if (( bound5 <= gr )&&( gr < bound4 ))
   {
      aa0 =  3.625104307573e-01;
      aa1 =  8.127569162631e-01;
      aa2 = -7.526734702040e-02;

      nu = aa0 + xx1*( aa1 + xx1*aa2 );

      return nu;
   }
   else if (( bound6 <= gr )&&( gr < bound5 ))
   {
      ll0 = ( gr - bound6 ) / ( bound5 - bound6 );
      ll1 = 1.- ll0;

      aa0 =  3.625104307573e-01;
      aa1 =  8.127569162631e-01;
      aa2 = -7.526734702040e-02;

      xx0 = aa0 + xx1*( aa1 + xx1*aa2 );

      aa0 =  4.290799517673e-01;
      aa1 =  5.356760861151e-01;
      aa2 =  1.352439621176e-01;

      nu = aa0 + xx1*( aa1 + xx1*aa2 );
      nu = ll0*xx0 + ll1*nu;

      return nu;
   }
   else
   {
      aa0 =  4.290799517673e-01;
      aa1 =  5.356760861151e-01;
      aa2 =  1.352439621176e-01;

      nu = aa0 + xx1*( aa1 + xx1*aa2 );
   
      return nu;
   };

/* this can never happen in present implementation: */

   if ( gr < bound7 )
   {
      fprintf( stderr, "\n Error message from function %s:", DO_GRANUS );
      fprintf( stderr, "\n Grashof number gr = %.7e < %.7e out of range !\n ",
         gr, bound7 );

      exit ( EXIT_FAILURE );
   };
}
/*============================================================================*/
# undef GRN_SMOOTH
/************************* end of function granus(*) **************************/

