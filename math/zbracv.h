# define DO_ZBRACV "zbracv(*)"
/*******************************************************************************
*                                                                              *
*   Function zbracv(*); HELIOS, release v1.0r1                                 *
*   [ Heat and Electric Losses In Ordinary wave guiding Structures ]           *
*                                                                              *
*   Given a function funct and an initial guessed range xx1, xx2, this routine *
*   expands the range geometrically until a root is bracketed by the returned  *
*   values xx1 and xx2 [ in which case zbrac returns 1 ] or until the range    *
*   becomes unacceptably large [ in which case zbrac returns null ].           *
*   In option opt=1 [-1] the range is expanded 'right-hand' [left-hand], viz.  *
*   only in positive [negative] direction, while in option opt=0 the range may *
*   be expanded in either directions of the real line.                         *
*                                                                              *
*   [ Modified zbrac(*) from: Press et al., Numerical Recipes in C, chap 9.1 ].*
*                                                                              *
*   (C) SHEIN; Munich, December 2021                       Steffen Hein        *
*   [ Update: December 17, 2021 ]                       <contact@sfenx.de>     *
*                                                                              *
/******************************************************************************/
# ifndef FACTOR
   # define FACTOR 1.6
# endif
# ifndef NBRTRY
   # define NBRTRY 1000
# endif
/*============================================================================*/

short zbracv ( double ( *funct )( double ), \
               double *xx1, double *xx2,    \
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
