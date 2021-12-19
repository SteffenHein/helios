# define DO_ZBRAK "zbrak(*)"
/*******************************************************************************
*                                                                              *
*   Function zbrak(*); HELIOS, release v1.0r1                                  *
*   [ Heat and Electric Losses In Ordinary wave guiding Structures ]           *
*                                                                              *
*   Given a function funct defined on the interval [xx1, xx2], subdivide the   *
*   interval into n equally spaced segments, and search for zero crossings of  *
*   the function. lb is iput as the maximum number of roots sought, and is     *
*   reset to the number of bracketing pairs xb1[1..lb], xb2[1..lb] that are    *
*   found.                                                                     *
*   [ Modified zbrak(*) from: Press et al., Numerical Recipes in C, chap 9.1,  *
*     version using long integer type ll ]                                     *
*                                                                              *
*   (C) SHEIN; Munich, December 2021                       Steffen Hein        *
*   [ Update: December 17, 2021 ]                       <contact@sfenx.de>     *
*                                                                              *
*******************************************************************************/
/*----------------------------------------------------------------------------*/

/*============================================================================*/

long zbrak ( double ( *funct )( double ), \
             double xx1, double xx2, long ll, \
             double xb1[], double xb2[], long *lb )
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
         xb1[( ++lbb )] = xx_;
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

/************************ end of function zbrak(*) ****************************/
