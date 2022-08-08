# define DO_ZBRAKSV "zbraksv(*)"
/*******************************************************************************
*                                                                              *
*   Function zbraksv(*); HELIOS, release v1.0r1                                *
*   [ Heat and Electric Losses In Ordinary wave guiding Structures ]           *
*                                                                              *
*   Given a function funct defined on the interval [xx1, xx2], subdivide the   *
*   interval into n equally spaced segments, and search for zero crossings of  *
*   the function. nb is iput as the maximum number of roots sought, and is     *
*   reset to the number of bracketing pairs xb1[1..nb], xb2[1..nb] that are    *
*   found.                                                                     *
*                                                                              *
*   [ Modified zbrak(*) from: Press et al., Numerical Recipes in C, chap 9.1,  *
*     version using short integer type ]                                       *
*                                                                              *
*   (C) SHEIN; Munich, December 2021                       Steffen Hein        *
*   [ Update: December 17, 2021 ]                       <contact@sfenx.de>     *
*                                                                              *
*******************************************************************************/
# ifndef MAXBRP
   # define MAXBRP 1001 /* maximum number of bracketing pairs */
# endif
/*----------------------------------------------------------------------------*/
static double 
   xb1[MAXBRP] = {null},
   xb2[MAXBRP] = {null};
/*============================================================================*/

short zbraksv ( double ( *funct )( double ), \
               double xx1, double xx2, \
               short nn, short *nb )
{
   static short
      ii = null,
      nbb = null;

   static double
      xx = ZERO,
      xx_ = ZERO,
      fp = ZERO,
      fc = ZERO,
      dx = ZERO;
/*----------------------------------------------------------------------------*/
   nbb = null;

   dx = ( xx2 - xx1 ) / nn; /* determine the spacing approriate to the mesh */

   xx = xx1;
   fp = ( *funct )( xx );

   ii = null;               /* loop over all intervalls */ 
   while(( ++ii ) <= nn )
   {
      if ( nbb == MAXBRP )
         return nbb;
      else
      {
	 xx_ = xx;
         xx += dx;
         fc = ( *funct )( xx );

         if (( fc*fp ) <= ZERO ) /* if a sign change occurs */
         {                       /* then record the values of the bounds */
            xb1[( ++nbb )] = xx_;
            xb2[nbb] = xx;

            if ( *nb == nbb )
               return nbb;
         };
         fp = fc;
      };
   };
   *nb = nbb;
   return nbb; /* returns the number of bracketing pairs */
}
/*============================================================================*/
# undef DO_ZBRAKSV
/********************** end of function zbraksv(*) ****************************/
