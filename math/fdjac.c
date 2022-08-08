# define DO_FDJAC "fdjac(*)"
/*******************************************************************************
*                                                                              *
*   Function fdjac(*); HELIOS, release v1.0r1                                  *
*   [ Heat and Electric Losses In Ordinary wave guiding Structures ]           *
*                                                                              *
*   Computes forward-difference approximation to Jacobian.                     *
*   On input, xx[0,...,dimension-1] is the point at which the Jacobian is to   *
*   be evaluated, ff[0,...,dimension-1] is the vector of function values at    *
*   the point, vfunct( dimension, xx, ff) is a user-supplied routine that      *
*   returns the vector of functions at xx.                                     *
*   On output, df[][] is the Jacobian array.                                   *
*                                                                              *
*   Modified version of fdjac(*) in Numerical Recipes in C, W.H.Press et al.   *
*   Cambridge University Press, 1999                                           *
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
# ifndef FDJAC_EPS
   # define FDJAC_EPS ( 1.0e-07 )
# endif
/*----------------------------------------------------------------------------*/
/*
# include "shutil.h"
*/
/*============================================================================*/

void fdjac ( void ( *vfunct )( double *xx, double *ff, short dimension ),
             double *xx, double *ff, double **df, double epsilon,
             short dimension )
{
   static short
      ii = null,
      jj = null;

   static double
      dx = ZERO,
      xtmp = ZERO,
     *fv;

   double
      *dvector( long nl, long nh );

   void
      free_dvector( double *dv, long nl, long nh );
/*----------------------------------------------------------------------------*/
/* allocate: */

   fv = dvector( 0, dimension-1 );

   ( *vfunct )( xx, ff, dimension );

   if ( epsilon <= ZERO )
      epsilon = FDJAC_EPS;

   for ( jj=null; jj<dimension; jj++ )
   {
      xtmp = xx[jj];
      dx = epsilon*fabs( xtmp );

      if ( dx == ZERO )
         dx = epsilon;
     
      xx[jj] += dx;      /* trick to reduce finite precision error */
      dx = xx[jj] - xtmp;

      ( *vfunct )( xx, fv, dimension );

      for ( ii=null; ii<dimension; ii++ )
         df[ii][jj] = ( fv[ii] - ff[ii] ) / dx;

      xx[jj] -= dx;
   };

   free_dvector( fv, null, dimension );
}
/*============================================================================*/
# undef FDJAC_EPS
# undef DO_FDJAC
/********************** end of function fdjac(*) ******************************/
