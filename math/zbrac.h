# define DO_ZBRAC "zbrac(*)"
/*******************************************************************************
*                                                                              *
*   Function zbrac(*); HELIOS, release v1.0r1                                  *
*   [ Heat and Electric Losses In Ordinary wave guiding Structures ]           *
*                                                                              *
*   Given a function funct and an initial guessed range xx1, xx2, this routine *
*   expands the range geometrically until a root is bracketed by the returned  *
*   values xx1 and xx2 [ in which case zbrac returns 1 ] or until the range    *
*   becomes unacceptably large [ in which case zbrac returns null ].           *
*                                                                              *
*   [ Largely following: Press et al., Numerical Recipes in C, chap 9.1 ]      *
*                                                                              *
*   (C) SHEIN; Munich, December 2021                       Steffen Hein        *
*   [ Update: December 17, 2021 ]                       <contact@sfenx.de>     *
*                                                                              *
*******************************************************************************/
# ifndef FACTOR
   # define FACTOR 1.6
# endif
# ifndef NBRTRY
   # define NBRTRY 100
# endif
/*----------------------------------------------------------------------------*/

/*============================================================================*/

short zbrac ( double ( *funct )( double ), double *xx1, double *xx2 )
{
   static long
      ii = null;

   static double
      ff1 = ZERO,
      ff2 = ZERO;
/*----------------------------------------------------------------------------*/

   if ( *xx1 == *xx2 )
   {
      fprintf( stderr, "\n Bad initial range in function %s\n ", DO_ZBRAC );
      return -ONE;
   };
   
   ff1 = ( *funct )( *xx1 );
   ff2 = ( *funct )( *xx2 );

   ii = null; do
   {
      if (( ff1*ff2 ) < ZERO )
         return 1;

      if ( fabs( ff1 ) < fabs ( ff2 ))
         ff1 = ( *funct )(( *xx1 ) += ( FACTOR*(( *xx1 ) - ( *xx2 ))));
      else
         ff2 = ( *funct )(( *xx2 ) += ( FACTOR*(( *xx2 ) - ( *xx1 ))));

   } while (( ++ii ) < NBRTRY );
   return null;
}
/*============================================================================*/
# undef DO_ZBRAC
# undef FACTOR
# undef NBRTRY
/************************ end of function zbrac(*) ****************************/
