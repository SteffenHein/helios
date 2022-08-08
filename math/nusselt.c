# define DO_NUSSELT "nusselt(*)"
/*******************************************************************************
*                                                                              *
*   Function nusselt(*); HELIOS release v1.0r1                                 *
*   [ Heat and Electric Losses In Ordinary wave guiding Structures ]           *
*                                                                              *
*   Returns the nusselt number as a function of Reynolds and Prandtl numbers,  *
*   and of the ratio = ( diameter / length ).                                  *
*                                                                              *
*   Formulae from Groeber, Erk, Grigull: Die Grundgesetze der Waermeueber-     *
*   tragung, ISBN 3-540-02982-6, Spinger-Verlag Berlin, Heidelberg, New York,  *
*   3.Auflage 1963. [ The indicated page numbers refer to this book.]          *
*                                                                              *
*   (C) SHEIN; Munich, December 2021                       Steffen Hein        *
*   [ Update: December 17, 2021 ]                       <contact@sfenx.de>     *
*                                                                              *
*******************************************************************************/
# include <stdio.h>
# include <stdlib.h>
# include <stddef.h>
# include <string.h>
/*----------------------------------------------------------------------------*/
# include "../math/maths.h"
# include "../math/consts.h"
/*----------------------------------------------------------------------------*/
# define NSS_ERROR  1             /* 1: do not exit on error and */
# if NSS_ERROR == 1               /* return value NSS_ERRTRN instead */
   # define NSS_ERRTRN ( ZERO )
# endif
/*----------------------------------------------------------------------------*/
/* the transition region between laminar and turbulent flow is defined as:    */
/* NSS_RELMNR < reynolds < NSS_RETURB */
# define NSS_RELMNR (2300.)
# define NSS_RETURB (10000.)
/*----------------------------------------------------------------------------*/
/* bounds for Hausen's approximation: */
# define NSS_HAUSEN1 (2800.)
# define NSS_HAUSEN2 (7000.)
/*----------------------------------------------------------------------------*/
/* Auxiliary coefficients for Hilpert's approximation of C and m [ p.248 ] */
/* polynomial coefficients for C approximation */
# define NSS_HILPRT0 (+8.9100000e-01)
# define NSS_HILPRT1 (-6.3456826e-02)
# define NSS_HILPRT2 (-1.0112674e-01)
# define NSS_HILPRT3 (+2.3206901e-02)
# define NSS_HILPRT4 (-1.5499163e-03)
/* polynomial coefficients for m approximation */
# define NSS_HILPRT5 (+3.3000000e-01)
# define NSS_HILPRT6 (-2.4920745e-03)
# define NSS_HILPRT7 (+1.4918700e-02)
/*----------------------------------------------------------------------------*/
# define NSS_CKRATIO( ) \
{ \
   if (( ratio <= 1.e-04 ) \
     ||( 1. <= ratio )) \
   { \
      fprintf( stderr, "\n Message from function %s:", DO_NUSSELT ); \
      fprintf( stderr, "\n Illegal diameter/length ratio =% .12e", ratio ); \
      fprintf( stderr, "\n The legal range is 1.e-4 < ratio < 1." ); \
 \
      if ( NSS_ERROR ) \
         return NSS_ERRTRN; \
      else \
      { \
         fprintf( stderr, "\n" ); \
         exit( EXIT_FAILURE ); \
      }; \
   }; \
}
/*============================================================================*/

double nusselt( double reynolds, double prandtl, double ratio, char *formula )
{ 
   static double
      cc = ZERO,
      qq = ZERO,
      ss = ZERO,
      tt = ZERO,
      nusselt = ZERO;

   double pow( double base, double exponent );
/*----------------------------------------------------------------------------*/

   if ( null == strncmp( formula, "kraussold", 5 )) /* turbulent */
   {                                                /* [ pp. 231, 240 ] */
      NSS_CKRATIO( );

      nusselt = 0.032*( pow( reynolds, 0.80 ));
      nusselt *= ( pow( prandtl, 0.30 ));
      nusselt *= ( pow( ratio, 0.054 ));

      return nusselt;
   }
   else if ( null == strncmp( formula, "kirschbaum", 5 )) /* turbulent in */
   {                                     /* ring tubes [ p.235, simplified ] */
      NSS_CKRATIO( );

      nusselt = 0.025*( pow( reynolds, 0.80 ));
      nusselt *= ( pow( prandtl, 0.30 ));
      nusselt *= ( pow( ratio, 0.054 ));

      return nusselt;
   }
   else if ( null == strncmp( formula, "boehm", 5 )) /* turbulent [ p.209 ] */
   {
      NSS_CKRATIO( );

      nusselt = 17.4*( pow( reynolds, 0.23 ));
      nusselt *= ( pow( prandtl, 0.12 ));
      nusselt *= ( pow( ratio, 0.5 ));

      return nusselt;
   } 
   else if ( null == strncmp( formula, "elser", 5 )) /* turbulent [ p.233 ] */
   {
      NSS_CKRATIO( );

      nusselt = 0.183*( pow( reynolds, ( 7./ 12.)));
      nusselt *= ( pow( prandtl, ( 1./ 3.)));
      nusselt *= ( pow( ratio, ( 1./ 3.)));

      return nusselt;
   }
   else if ( null == strncmp( formula, "hausen", 5 )) /* laminar to */
   {                                                  /* turbulent */
      NSS_CKRATIO( );

      if ( reynolds <= NSS_RELMNR ) /* laminar flow [ p.208 ] */
      {             
         nusselt = .0668*( reynolds*prandtl*ratio );
         nusselt /= ( 1.+.045*( pow(( reynolds*prandtl*ratio ), ( 2./ 3.))));
         nusselt += 3.65;
      }
      else if (( NSS_RELMNR < reynolds )
             &&( reynolds <= NSS_HAUSEN1 )) /* [ pp. 208, 239 ] */
      {
         nusselt = .0668*( reynolds*prandtl*ratio );
         nusselt /= ( 1.+.045*( pow(( reynolds*prandtl*ratio ), ( 2./ 3.))));
         nusselt += 3.65;

         ss = ( NSS_HAUSEN1 - reynolds )/( NSS_HAUSEN1 - NSS_RELMNR );
         tt = 1. - ss;
         ss *= nusselt;

         nusselt = 0.116*tt*( 1.+ pow( ratio, ( 2./3.)));
         nusselt *= ( pow( reynolds, ( 2./3.)) - 125. );
         nusselt *= ( pow( prandtl, ( 1./3.)));
         nusselt += ss;
      }
      else if (( NSS_HAUSEN1 < reynolds ) /* intermediate 'transition' region */
             &&( reynolds <= NSS_HAUSEN2 )) /* [ p.239 ] */
      {
         nusselt = 0.116*( 1.+ pow( ratio, ( 2./3.)));
         nusselt *= ( pow( reynolds, ( 2./3.)) - 125. );
         nusselt *= ( pow( prandtl, ( 1./3.)));
      }
      else if (( NSS_HAUSEN2 < reynolds ) /* [ pp. 239, 240 ] */
             &&( reynolds < NSS_RETURB ))
      {
         nusselt = 0.116*( 1.+ pow( ratio, ( 2./3.)));
         nusselt *= ( pow( reynolds, ( 2./3.)) - 125. );
         nusselt *= ( pow( prandtl, ( 1./3.)));

         ss = ( NSS_RETURB - reynolds )/( NSS_RETURB - NSS_HAUSEN2 );
         tt = 1. - ss;
         ss *= nusselt;

         nusselt = 0.032*tt*( pow( reynolds, 0.80 ));
         nusselt *= ( pow( prandtl, 0.30 ));
         nusselt *= ( pow( ratio, 0.054 ));
	 nusselt += ss;
      }
      else /* Kraussold's formula [ pp. 231, 240 ] */
      {
         nusselt = 0.032*( pow( reynolds, 0.80 ));
         nusselt *= ( pow( prandtl, 0.30 ));
         nusselt *= ( pow( ratio, 0.054 ));
      };

      return nusselt;
   } 
   else if ( null == strncmp( formula, "hilpert", 5 )) /* [ p.233 ] */
   {                                          /* flow transverse to tubes */
      ss = log10( reynolds);
      tt = log10( 4.e+04 );

      if ( ss <= tt )
      {
         cc = NSS_HILPRT0;
         cc += ( ss*( NSS_HILPRT1 + ss*( NSS_HILPRT2 + ss*NSS_HILPRT3 + ss*\
                 NSS_HILPRT4 )));
      }
      else /* 4.e+04 < reynolds */
      {
         cc = NSS_HILPRT0;
         cc += ( tt*( NSS_HILPRT1 + tt*( NSS_HILPRT2 + tt*NSS_HILPRT3 + tt*\
                 NSS_HILPRT4 )));

         cc *= ( pow(( tt / log10( reynolds )), 10. ));
      };

      qq = NSS_HILPRT5;
      qq += ( ss*( NSS_HILPRT6 + ss*( NSS_HILPRT7 )));

      nusselt = cc*( pow( reynolds, qq ));

      return nusselt;
   }
   else if ( null == strncmp( formula, "sieder-tate", 5 )) /* laminar */
   {                                                /* [ p.209, simplified ] */
      NSS_CKRATIO( );

      nusselt = 1.86*( pow(( reynolds*prandtl*ratio ), ( 1./ 3.)));

      return nusselt;
   } 
   else
   {
      fprintf( stderr, "\n Message from function %s:", DO_NUSSELT );
      fprintf( stderr, "\n Unknown formula '%s' !!!", formula );
      fprintf( stderr, "\n Implemented formulae are, e.g.:" );
      fprintf( stderr, "\n boehm," );
      fprintf( stderr, "\n elser," );
      fprintf( stderr, "\n hausen," );
      fprintf( stderr, "\n hilpert," );
      fprintf( stderr, "\n kirschbaum," );
      fprintf( stderr, "\n kraussold," );
      fprintf( stderr, "\n sieder-tate" );
   };

# if NSS_ERROR == 1
   return NSS_ERRTRN;
# else
   fprintf( stderr, "\n" );

   exit( EXIT_FAILURE );
# endif
}
/*============================================================================*/

/************************** end of function nusselt(*) ************************/
