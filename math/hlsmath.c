/* [ file: hlsmath.c ] */
/*******************************************************************************
*                                                                              *
*   Mathematical utilities [ nrutil.c ]; HELIOS, release v1.0r1                *
*   [ Heat and Electric Losses In Ordinary wave guiding Structures ]           *
*                                                                              *
*   Some of the routines collected in this file are [ modified ] versions of   *
*   functions and programs of the nrutil.c listing published in the book of    *
*   William H. Press, Saul A. Teukolski, William T. Vetterling, and Brian P.   *
*   Flannery: NUMERICAL RECIPES IN C, The Art of Scientific Computing, 2nd Ed, *
*   Cambridge University Press, 1999.                                           *
*   Of course, all modifications and adaptations [ to our needs or personal    *
*   taste ] are in our responsibility - and no warrenty is given for these or  *
*   other applications.                                                        *
*                                                                              *
*   (C) SHEIN; Munich, December 2021                       Steffen Hein        *
*   [ Update: December 17, 2021 ]                       <contact@sfenx.de>     *
*                                                                              *
*******************************************************************************/
# define _POSIX_SOURCE 0 /* set to 1: if POSIX.1 standard headers will be used*/
/*----------------------------------------------------------------------------*/
# include <stdio.h>
# include <stddef.h>
# include <stdlib.h>
# include <string.h>
/*----------------------------------------------------------------------------*/
# include "../math/maths.h"
# include "../math/consts.h"
/*----------------------------------------------------------------------------*/
# include "../CONFIG.H"
# include "../src/types.h"
/*----------------------------------------------------------------------------*/
# define DO_DVECTOR "dvector(*)"
/*******************************************************************************
*                                                                              *
*   Function dvector(*) in CODIS-2.0r1                                         *
*                                                                              *
*   Allocates a double vector with subscript range dv[nl...nh]                 *
*                                                                              *
*   [ Modified version of dvector(*), Numerical Recipes in C, W.H.Press        *
*     et al., Cambridge University Press 1999. ]                               *
*                                                                              *
*   SHEIN; Bad Aibling, July 2002                              Steffen Hein    *
*   [ Update: 30 January 2006 ]                               <hein@bnro.de>   *
*                                                                              *
*******************************************************************************/
# ifndef NR_END
   # define NR_END 1
# endif
# ifndef FREE_ARG
   # define FREE_ARG char*
# endif
/*============================================================================*/

double *dvector( long nl, long nh )
{
   double *dv;
/*----------------------------------------------------------------------------*/
/* allocate pointers to rows: */

   if ( NR_END < nl )
   {
      fprintf( stderr, "\n Warning from function %s:", DO_DVECTOR );
      fprintf( stderr, "\n Non ANSI compliant vector allocation !" );
      fprintf( stderr, "\n [ NR_END = %ld < %ld = nl "
         "( first allocated index ),", ( long ) NR_END, nl );
      fprintf( stderr, "\n   ANSI requires nl <= NR_END. - This can be" );
      fprintf( stderr, "\n   remedied setting macro NR_END in %s to %ld.]",
         DO_DVECTOR, nl );
   };
  
   dv = ( double *) calloc (( size_t )( nh-nl+1+NR_END ), sizeof( double ));

   if ( !dv )
   {
      fprintf( stderr, "\n Allocation failure dvector(*) !\n" );
      exit( EXIT_FAILURE );
   };

   dv += NR_END;
   dv -= nl;

   return dv ;
}
/*============================================================================*/
# undef DO_DVECTOR

/* free double vector allocated by dvector(*): */
/*============================================================================*/

void free_dvector( double *dv, long nl, long nh )
{
   free (( FREE_ARG ) ( dv+nl-NR_END ));
}
/*============================================================================*/
/*************** end of functions dvector(*), free_dvector(*) *****************/




# define DO_DMATRIX "dmatrix(*)"
/*******************************************************************************
*                                                                              *
*   Function dmatrix(*) in CODIS-2.0r1                                         *
*                                                                              *
*   Allocates a ( double ) matrix with subscript range m[nrl...nrh][ncl...nch] *
*                                                                              *
*   [ Modified version of dvector(*), Numerical Recipes in C, W.H.Press        *
*     et al., Cambridge University Press 1999. ]                               *
*                                                                              *
*   SHEIN; Bad Aibling, July 2002                              Steffen Hein    *
*   [ Update: 30 January 2006 ]                               <hein@bnro.de>   *
*                                                                              *
*******************************************************************************/

/*============================================================================*/

double **dmatrix( long nrl, long nrh, long ncl, long nch )
{
   long 
      ii,
      nrow = nrh-nrl+1,
      ncol = nch-ncl+1;

   double **dm;
/*----------------------------------------------------------------------------*/
/* allocate pointers to rows: */

   if ( NR_END < nrl )
   {
      fprintf( stderr, "\n Warning from function %s:", DO_DMATRIX );
      fprintf( stderr, "\n Non ANSI compliant matrix allocation !" );
      fprintf( stderr, "\n [ NR_END = %ld < %ld = nrl ( first allocated row "
         "index ),", ( long ) NR_END, nrl );
      fprintf( stderr, "\n   ANSI requires nrl <= NR_END. - This can be" );
      fprintf( stderr, "\n   remedied setting macro NR_END in %s to %ld.]",
         DO_DMATRIX, nrl );
   };

   if ( NR_END < ncl )
   {
      fprintf( stderr, "\n Warning from function %s:", DO_DMATRIX );
      fprintf( stderr, "\n Non ANSI compliant matrix allocation !" );
      fprintf( stderr, "\n [ NR_END = %ld < %ld = ncl ( first allocated column "
         "index ),", ( long ) NR_END, ncl );
      fprintf( stderr, "\n   ANSI requires ncl <= NR_END. - This can be" );
      fprintf( stderr, "\n   remedied setting macro NR_END in %s to %ld.]",
         DO_DMATRIX, ncl );
   };

   dm = ( double **) calloc (( size_t )( nrow+NR_END ), sizeof( double *));

   if ( !dm )
   {
      fprintf( stderr, "\n Allocation failure 1 in matrix(*) !\n" );
      exit( EXIT_FAILURE );
   };

   dm += NR_END;
   dm -= nrl;

/* allocate rows and set pointers to them: */

   dm[nrl] = ( double *) calloc(( size_t )(nrow*ncol+NR_END), sizeof( double));

   if ( !dm[nrl] )
   {
      fprintf( stderr, "\n Allocation failure 2 in matrix(*) !\n" );
      exit( EXIT_FAILURE );
   };

   dm[nrl] += NR_END;
   dm[nrl] -= ncl;

   for ( ii=nrl+1; ii <= nrh; ii++ )
      dm[ii] = dm[ii-1]+ncol;

   return dm ;
}
/*============================================================================*/
# undef DO_DMATRIX

/* free double matrix allocated by dmatrix(*): */
/*============================================================================*/

void free_dmatrix( double **dm, long nrl, long nrh, long ncl, long nch )
{
   free (( FREE_ARG ) ( dm[nrl]+ncl-NR_END ));
   free (( FREE_ARG ) ( dm+nrl-NR_END ));
}
/*============================================================================*/
/*************** end of functions dmatrix(*), free_dmatrix(*) *****************/




# define DO_RTBIS "rtbis(*)"
/*******************************************************************************
*                                                                              *
*   Function rtbis in CODIS-2.0r1                                              *
*                                                                              *
*   Using bisection, find the root of a function func known to lie between     *
*   xx1 and xx2.                                                               *
*   The root, returned as rtbis, will be defined until acurracy is +i xacc     *
*   [ Largely following: Press et al., Numerical Recipes in C, chap 9.1 ]      *
*                                                                              *
*   SHEIN, Bad Aibling; June 2002                              Steffen Hein    *
*   [ Update: 30 January 2006 ]                               <hein@bnro.de>   *
*                                                                              *
*******************************************************************************/
# ifndef RTB_MAXITR
   # define RTB_MAXITR 1000
# endif
# ifndef GIANT_VAL
   # define GIANT_VAL ( 1.e+301 )
# endif
# define RTB_ERROR  1             /* 1: do not exit on error and */
# if RTB_ERROR == 1               /* return value RTB_ERRTRN instead */
   # define RTB_ERRTRN (HUGE_VALF)
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
*   Function zbracv in CODIS-2.0r1                                             *
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
*   SHEIN, Bad Aibling; June 2002                              Steffen Hein    *
*   [ Update: 30 January 2006 ]                               <hein@bnro.de>   *
*                                                                              *
*******************************************************************************/
# ifndef FACTOR
   # define FACTOR 1.6
# endif
# ifndef NBRTRY
   # define NBRTRY 1000
# endif
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
*   Function zbrakl in CODIS-2.0r1                                             *
*                                                                              *
*   Given a function funct defined on the interval [xx1, xx2], subdivide the   *
*   interval into n equally spaced segments, and search for zero crossings of  *
*   the function. lb is iput as the maximum number of roots sought, and is     *
*   reset to the number of bracketing pairs xb1[1..lb], xb2[1..lb] that are    *
*   found.                                                                     *
*   [ Modified zbrak(*) from: Press et al., Numerical Recipes in C, chap 9.1,  *
*     version using long integer type ll ]                                     *
*                                                                              *
*   SHEIN, Bad Aibling; June 2002                              Steffen Hein    *
*   [ Update: 30 January 2006 ]                               <hein@bnro.de>   *
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
/*********************** end of function rtbis(*) ****************************/




# define DO_FDJAC "fdjac(*)"
/*******************************************************************************
*                                                                              *
*   Function fdjac in CODIS-2.0r1                                              *
*                                                                              *
*   Computes forward-difference approximation to Jacobian                      *
*   On input, xx[0,...,dimension-1] is the point at which the Jacobian is to   *
*   be evaluated, ff[0,...,dimension-1] is the vector of function values at    *
*   the point, vfunct( dimension, xx, ff) is a user-supplied routine that      *
*   returns the vector of functions at xx.                                     *
*   On output, df[][] is the Jacobian array.                                   *
*                                                                              *
*   [ Modified version of fdjac(*) in Numerical Recipes in C, W.H.Press        *
*     at al., Cambridge University Press, 1999 ]                               *
*                                                                              *
*   SHEIN, Bad Aibling; June 2002                              Steffen Hein    *
*   [ Update: 30 January 2006 ]                               <hein@bnro.de>   *
*                                                                              *
*******************************************************************************/
/*
# include <stdio.h>
# include <stddef.h>
# include <stdlib.h>
# include <math.h>
*/
/*----------------------------------------------------------------------------*/
# ifndef FDJAC_EPS
   # define FDJAC_EPS ( 1.0e-07 )
# endif
/*
# include "consts.h"
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




# define DO_RDUNIT "rdunit(*)"
/*******************************************************************************
*                                                                              *
*   Function rdunit in CODIS-2.0r1                                             *
*                                                                              *
*   SHEIN, Bad Aibling; June 2002                              Steffen Hein    *
*   [ Update: 30 January 2006 ]                               <hein@bnro.de>   *
*                                                                              *
*******************************************************************************/
/*
# include <stdio.h>
# include <string.h>
# include <stdlib.h>
*/
/*===========================================================================*/

char *rdunit( char *filename, char *unit )
{
   static FILE
     *fleptr;

   static char
     ptr0[80] = {0},
     ptr1[80] = {0};

   static short
      ii = 0,
      jj = 0;
/*............................................................................*/
   fleptr = fopen( filename, "r" );

   if ( NULL == fleptr )
   {
      fprintf( stderr, "\n can't open file %s\n", filename );
      exit( EXIT_FAILURE );
   }
   else
   {
      jj = strlen( unit );

      ii = 0; do
      {
         strcpy( ptr1, ptr0 );
         fscanf( fleptr, "%s", ptr0 );
      } while ((( ++ii ) < 20 )&&( strspn( unit, ptr0 ) < jj ));

      if ( ii >= 20 )
      {
         fprintf( stderr, "\n can't stat unit\n" );
         exit( EXIT_FAILURE );
      }
      else if ( 4 == strspn( "unit", ptr1 ))
      {
         return unit;
      }
      else
      {
         strcpy ( ptr0, ptr1 );
         strcat ( ptr0, " " );
         strcat ( ptr0, unit );
         return ptr0;
      };

      fclose( fleptr );
   };
   return NULL;
}
/*============================================================================*/
/************************ end of function rdunit(*) ***************************/




# define DO_LOTOS "lotos(*)"
/*******************************************************************************
*                                                                              *
*   Function lotos(*) in CODIS-2.0r1                                           *
*                                                                              *
*   This function converts given integer lngint into                           *
*   an  ASCII character string  pointed to by lngstr                           *
*                                                                              *
*   (C) SHEIN; Bad Aibling, 1999                               Steffen Hein    *
*   [ Update: 30 January 2006 ]                               <hein@bnro.de>   *
*                                                                              *
*******************************************************************************/
# include <stdarg.h>
# include <string.h>
# include <float.h>
# include <math.h>
# include <time.h>
/*----------------------------------------------------------------------------*/
/*
# include "../math/consts.h"
*/
/*============================================================================*/

char *lotos ( long lngint, char length )
{
   static long
      nn = null,
      dd = null;

   static signed char
      ii    = null,
      jj    = null,
      ssize = null;

   static char 
      *lngstr;
      
   lngstr = ( char *) calloc ( SHS_SIZE, ONE );
/*----------------------------------------------------------------------------*/

   nn    = ONE;
   ii    = null;
   ssize = null;
   if ( lngint < null )
   {
      lngstr[ssize] = '-';
      lngint = - lngint;
      ssize = ONE;
      ii = ONE;
   };
   do
   {
      lngstr[ssize] = 48; /* 48:  ASCII sign '0' */
      nn *= 10;
      ssize++;
   }  while (( ssize < SHS_SIZE )&&( nn <= lngint ));
   do
   {
      nn /=10;
      dd = lngint / nn ;
      lngstr[ii] = ( char ) dd + 48;        /* (+ 48) converts  digit  dd     */
      lngint -= dd*nn;                      /*        into ASCII sign 'dd'    */
      ii++;
   }  while ( ii < ssize );
   lngstr[ssize] = null;

/* write trailing ZEROs if length > ssize: */

   if ( length > ssize ) 
   {
      jj = length;

      while ( ssize >= null )
      { 
         lngstr[jj] = lngstr[ssize]; 
         jj--;
         ssize--;
      };
      while ( jj >= null )
      {
         lngstr[jj] = 48;
         jj--; 
      };
   };

   return lngstr; 
}
/*============================================================================*/
/************* end of long-to-string conversion function 'lotos(*)' ***********/




# define DO_DOTOS "dotos(*)"
/*******************************************************************************
*                                                                              *
*   Function dotos(*) in CODIS-2.0r1                                           *
*                                                                              *
*   This function converts given  double lngdbl into a fixed point [ *format   *
*   = 'f' ] or floating point [ *format = 'e', 'E' ] ASCII character string,   *
*   pointed to by char *dblstr, which is returned to the calling program.      *
*                                                                              *
*   (C) SHEIN; Bad Aibling, January 2008                   Steffen Hein        *
*   [ Update: 04 August 2010 ]                      <contact@steffen-hein.org> *
*                                                                              *
*******************************************************************************/
# ifndef DBL_SGN
   # define DBL_SGN 0
# endif
# ifndef DBL_DIG
   # define DBL_DIG 15
# endif
/*============================================================================*/

char *dotos ( double lngdbl, char precision, char *format )
{
   static double
      dd = ZERO,
      qq = ZERO,
      rr = ZERO,
      ss = ZERO,
      pw10 = ZERO,
      ln10 = ZERO,
      one_ =.9999999999999999;

   static signed char
      ff = null,
      ii = null,
      jj = null,
      ssize  = null,
      mxprcs = null,
      mxsize = null;

   static short
      ld = null,
      expt = null;

   static char
      dblstr[STS_SIZE] = {null};
/*      
   static char
      ptr[STS_SIZE] = {null};
*/
/* prototypes: */
   
   double exp( double x );
   double log( double x );
   double log10( double x );

   mxprcs = DBL_DIG;
   mxsize = DBL_DIG + EIGHT; /* must be greater than FIVE */
/*----------------------------------------------------------------------------*/

   ff = *format;

   if (( ff != 'e' )&&( ff != 'E' ))
      ff = 'f'; /* fixed point */

   if ( mxprcs < precision )
      precision = mxprcs;

   dd = lngdbl;
   ssize = null;
   if ( dd == ZERO )
   {
     dd_zero:

      dblstr[ssize] = 48;  /* ASCII char '0' */
      ssize++;
      dblstr[ssize] = 46;  /* ASCII char '.' */
      ssize++;

      if ( ff == 'f' )
         jj = mxsize - TWO;
      else if (( ff == 'e' )||( ff == 'E' ))
         jj = mxsize - FIVE;

      ii = ONE;
      while (( ssize < jj )&&( ii < precision ))
      {
         dblstr[ssize] = 48; /* ASCII char '0' */
         ssize++;
         ii++;
      };
      
      if (( ff == 'e' )||( ff == 'E' ))
      {
         dblstr[ssize] = ff; /* ASCII char format */
         ssize++;
         dblstr[ssize] = 43;  /* ASCII char '+' */
         ssize++;
         dblstr[ssize] = 48;  /* ASCII char '0' */
         ssize++;
         dblstr[ssize] = 48; 
         ssize++;
      };

      return dblstr;
   }
   else if ( ZERO < dd )
   { 
# if DBL_SGN == 1
      dblstr[ssize] = 40;  /* ASCII char ' ' [ SPACE ] */
      ssize++;
# elif DBL_SGN == 2
      dblstr[ssize] = 43;  /* ASCII char '+' */
      ssize++;
# endif
   }
   else if ( dd < ZERO )
   {
      dblstr[ssize] = 45;  /* ASCII char '-' */
      ssize++;
      dd = - dd;
   };

   ln10 = log( 10.0000000000000 );

   expt = ( signed int ) log10( dd );

   if (( ff == 'f' )&&( precision <= expt ))
      ff = 'e';
/*............................................................................*/
/* round off [ upto precision = number of decimal digits ] */
   ld = expt;

   if (( ff == 'f' )&&( ld < -ONE ))
      ld = -ONE;
      
   rr = dd;
   ss = ZERO;

   ii = null;
   while ( ii <= precision )
   {
      pw10 = exp( ln10*ld );

      if ( ZERO < pw10 )
      {
         qq = rr/pw10;
         jj = ( char ) qq;
      }
      else
      {
         jj = NINE;
         goto brk1;
      };
      ss += ( double ) jj * pw10;

      rr -= ( double ) jj * pw10;
      ld -= ONE;

      ii++;
   };

   pw10 = exp( ld * ln10 );

   if ( ZERO < pw10 )
   {
      qq = rr/( one_*pw10 );      /* ! */
      jj = ( char ) qq;
   };

   ss += ( double ) jj * pw10;

  brk1:

   if ( FOUR < jj ) 
      dd = ss + ( double ) ( 10.01 - jj ) * pw10;

   if ( dd == ZERO )
      goto dd_zero;

   expt = ( signed int ) log10( dd );
/*............................................................................*/
/* log10(dd): find leading digit != null */

   rr = dd;
   ld = expt + ONE; do
   {
      ld--;
      pw10 = exp( ld * ln10 );
      qq = rr/( one_*pw10 );
      jj = ( char ) qq;
   } while ( jj <= null );
   expt = ld;                          /* expt is the greatest interger n     */
                                       /* such that 10^n <= dd < 10^(n+1)     */
/*............................................................................*/
/* write floating point ASCII string: */

   if (( ff == 'e' )||( ff == 'E' ))
   {
      if( NINE < jj )  
         jj = NINE;  

      dblstr[ssize] = jj + 48;  /* ASCII char of integer jj */
      ssize++;
      dblstr[ssize] = 46;       /* ASCII char '.' */
      ssize++;

      ii = ONE;
      while (( ssize < mxsize - SIX )&&( ii < precision ))
      {
         rr -= ( double ) jj * pw10;
         ld -= ONE;
         pw10 = exp( ld * ln10 );

         if ( ZERO < pw10 )
         { 
            qq = rr/pw10;
            jj = ( char ) qq;
         }
         else
         {
            jj = NINE;
         };

         if ( jj < null )
            jj = null;
         if ( NINE < jj )
            jj = NINE; 

         dblstr[ssize] = jj + 48;  /* ASCII char of integer jj */
         ssize++;

         ii++;
      };

      dblstr[ssize] = ff; /* ASCII char format[0], e.g. 'e' = 101      */
      ssize++;

      if ( null <= expt )
      {
         dblstr[ssize] = 43;  /* ASCII char '+' */
         ssize++;
      }
      else
      {
         dblstr[ssize] = 45;  /* ASCII char '-' */
         ssize++;
         expt = - expt;
      };
      
      if ( 10000 <= expt )
      {
         printf( "\n\n Error in function '%s' : ", DO_DOTOS ); 
         printf( "\n Floating point overflow ! " );
         printf( "\n [ overrides and returns a null pointer ] \n" );
         return null;
      };

      ld = ( int ) expt/1000;

      if ( ld != null )
      {
         dblstr[ssize] = ld + 48;  /* ASCII char of integer ld */
         ssize++;

         expt -= (( int ) ld )*1000;
         ld = ( int ) expt/100; 

         dblstr[ssize] = ld + 48;  /* ASCII char of integer ld */
         ssize++;
      }
      else
      {
         ld = ( int ) expt/100;

         if ( ld != null )
         {
            dblstr[ssize] = ld + 48;  /* ASCII char of integer ld */
            ssize++;
         };
      };

      expt -= (( int ) ld )*100;
      ld = ( int ) expt/10;

      dblstr[ssize] = ld + 48;  /* ASCII char of integer ld */
      ssize++;

      expt -= (( int ) ld )*10;
      ld = ( int ) expt;

      dblstr[ssize] = ld + 48;  /* ASCII char of integer ld */
      ssize++;
   }
   else /* ff == 'f': fixed point ASCII string */ 
   {
      rr = dd;
      ld = expt;

      if ( ld < -ONE )
      {
         ld = -ONE;
         pw10 = exp( ln10*ld );

         if ( ZERO < pw10 )
	 {
            qq = rr / pw10;
            jj = ( char ) qq;
         };
      };

      ii = null;
      while (( ssize < mxsize )&&( ii < precision ))
      {
         if ( jj < null )
            jj = null;
         if ( NINE < jj )
            jj = NINE;

         if ( ld == -ONE )
         {
            dblstr[ssize] = 46; /* ASCII char '.' */
            ssize++;
         };

         if (( null < jj )||( null < ii )||( ld < null))
         {
            dblstr[ssize] = jj + 48;  /* ASCII char of integer jj */
            ssize++;
            ii++;
         };

         rr -= ( double ) jj * pw10;
         ld -= ONE;
         pw10 = exp( ln10*ld );

         if ( ZERO < pw10 )
         {
            qq = rr / pw10;
            jj = ( char ) qq;
         }
         else
         {
            jj = NINE;
         }; 
      };

      if ( ld == ( -ONE ))
      {
         dblstr[ssize] = 46; /* ASCII char '.' */
         ssize++;
      };
   };

   dblstr[ssize] = null;     /* trailing null */
   return dblstr;
}
/*============================================================================*/
# undef DBL_SGN
# undef DBL_DIG
/************ end of double-to-string conversion function dotos(*) ************/




/*******************************************************************************
*                                                                              *
*   Function rndoff(*) in CODIS-2.0r1                                          *
*                                                                              *
*   This function returns ( double ) xx off-rounded up to ( short ) nn digits  *
*                                                                              *
*   (C) SHEIN; Bad Aibling, 1999                               Steffen Hein    *
*   [ Update: 30 January 2006 ]                               <hein@bnro.de>   *
*                                                                              *
*******************************************************************************/
/*
# include <stdio.h>
# include <stdlib.h>
# include <stddef.h>
# include <math.h>
# include "consts.h"
*/
/*----------------------------------------------------------------------------*/
# ifndef SMALL_VAL
   # define SMALL_VAL ( 1.e-277 )
# endif
/*----------------------------------------------------------------------------*/
# define USE_ROUND 0
/*============================================================================*/

double rndoff( double xx, short nn )
{
   static double
      ss = ZERO;

   static short
      ii = null;

   double pow( double xx, double yy );

# if USE_ROUND == 1
   double round( double xx );
# endif
/*----------------------------------------------------------------------------*/
   ss = fabs( xx );

   if ( ss < SMALL_VAL )
      return ZERO;

   ii = log10( ss );

   ss *= pow( 10., -ii );
   ss *= pow( 10., nn );

# if USE_ROUND == 1
   ss = round( ss );
# else
   ss += ( double ) HALF;
   ss = ( double )( long )( ss );
# endif

   ss *= pow( 10., -nn );
   ss *= pow( 10., ii );

   if ( xx < ZERO )
      ss = -ss;

   return ss;
}
/*============================================================================*/
/************************ end of function rndoff(*) ***************************/




# define DO_TXCNSL "txcnsl(*)"
/*******************************************************************************
*                                                                              *
*   Function txcnsl in CODIS-2.0r1                                           *
*                                                                              *
*   Provides standardized command line and parameter input format on a text    *
*   console.                                                                   *
*                                                                              *
*   (C) SHEIN; Bad Aibling, November 1999                      Steffen Hein    *
*   [ Update: 30 January 2006 ]                               <hein@bnro.de>   *
*                                                                              *
*******************************************************************************/

/*----------------------------------------------------------------------------*/
# ifndef USE_NCURSES
   # define USE_NCURSES 0
# endif
/*----------------------------------------------------------------------------*/
# if USE_NCURSES == 1
   # include <ncurses.h>
# endif 
# include <limits.h>
# include <ctype.h>
# include <time.h>           /* cf. time( ),ctime( ),asctime( ),localtime( )  */
/*----------------------------------------------------------------------------*/
/* some user defined constants  
[ e.g.:
   # define SSTRNG "***.......
   # define null   0
   # define ONE    1
     ***
   # define DEC   10
   # define ZERO   0.
   # define PI     3.14.......
] */
/*
# include "../math/consts.h"
*/
# include "../math/txctyp.h"
/*----------------------------------------------------------------------------*/
# define CNS_DEFLT  "active"   
                     
# ifndef CNS_ITEMS
   # define CNS_ITEMS 10 /* maximum number of menu items                     */
# endif
# ifndef CNS_LNLEN
   # define CNS_LNLEN 79 /* number of characters in menu line                */
# endif
# ifndef CNS_LNINT
   # define CNS_LNINT  2 /* first non-space character in line                */
# endif
# ifndef CNS_POSIT
   # define CNS_POSIT 67 /* position of menu labels [indices] in line         */
# endif
/*----------------------------------------------------------------------------*/
/* structure typedefs: */

/* Transfer structure of function 'txcnsl(*)' [ which provides standardized   */
/* command line / menu options on a text console ] */
/*
typedef struct
{
   signed char 
      clscr,
      rtn; 

   int
      items, dfopt, dflnf,
      lnlen, posit, lnint,
      lglen, dblen, stlen,
      option;

   char
      title[LGS_SIZE],
      rqfrm[SHS_SIZE],
      tasks[CNS_LNLEN],
      flags[CNS_LNLEN],
      cmmnt[CNS_LNLEN],
      envmt[CNS_LNLEN],
      escpe[CNS_LNLEN],
      cnfrm[CNS_LNLEN],
      instr[CNS_LNLEN],
      dfstr[CNS_LNLEN],
      rqdbl[CNS_LNLEN],
      rqlng[CNS_LNLEN],
      rqstr[CNS_LNLEN],
      mflag[CNS_ITEMS],
      mflin[CNS_ITEMS],
      mline[CNS_ITEMS][CNS_LNLEN];

   long
      dflng, inlng;

   double
      dfdbl, indbl;
   
} TXCNSL;
*/
static TXCNSL
   cns = {null};
/*
static FILE 
  *keyboard = stdin,
  *display = stdout;
  [ This didn't work with Redhat's version of egcc C compiler ]
*/
/*----------------------------------------------------------------------------*/
# if USE_NCURSES == 1
/* 'my_terminal' configuration: */

   # include <termcap.h> /* terminal type header */
   static char *term;    /* terminal type string */ 

   # define CLSCREEN { /* clear screen */\
      fprintf( stdout, "%s", tgetstr( "cl", null ));\
   }

   # define PRBLDCLR(a) { /* bold clear output */\
      fprintf( stdout, "%s%s", tgetstr( "md", null ), (a));\
   }

   # define PRINVERS(a) { /* inverse: black <-> white */\
      fprintf( stdout, "%s%s", tgetstr( "mr", null ), (a));\
   }

   # define PRNORMAL(a) { /* back to normal output */\
      fprintf( stdout, "%s%s", tgetstr( "me", null ), (a));\
   }
# else /* if USE_NCURSES != 1 */
# ifdef _Unix
   # define CLSCREEN { /* clear screen */\
      system( "clear" ); \
      fprintf( stdout, "\f" );\
   }
# endif
# ifdef _BSD
   # define CLSCREEN { /* clear screen */\
      system( "clear" ); \
      fprintf( stdout, "\f" );\
   }
# endif
# ifdef _Linux
   # define CLSCREEN { /* clear screen */\
      system( "clear" ); \
      fprintf( stdout, "\f" );\
   }
# endif
# ifdef _GNU_Linux
   # define CLSCREEN { /* clear screen */\
      system( "clear" ); \
      fprintf( stdout, "\f" );\
   }
# endif
# ifdef _SuSE
   # define CLSCREEN { /* clear screen */\
      system( "clear" ); \
      fprintf( stdout, "\f" );\
   }
# endif
# ifndef CLSCREEN
   # define CLSCREEN { /* clear screen */\
      fprintf( stdout, "\f" );\
   }
# endif

   # define PRBLDCLR(a) { \
      fprintf( stdout, "%s", (a));\
   }

   # define PRINVERS(a) { \
      fprintf( stdout, "%s", (a));\
   }

   # define PRNORMAL(a) { \
      fprintf( stdout, "%s", (a));\
   }
# endif
/*============================================================================*/

TXCNSL *clcnsl( TXCNSL *cxp )
{
/* allusions: */
/*
   extern TXCNSL 
      cns;
*/
/* declarations: */

   static TXCNSL 
     *cpt;
/*
     *cpt = &cns;
*/
   static short
      ii = null,
      jj = null;

/*----------------------------------------------------------------------------*/
   cpt = cxp;

   ii = null;
   while( ii < SHS_SIZE )
   {
      ( cpt->rqfrm[ii] ) = null;
      ii++ ;
   };

   ii = null;
   while( ii < LGS_SIZE )
   {
      ( cpt->title[ii] ) = null;
      ii++ ;
   };

   ii = null;
   while( ii < CNS_ITEMS )
   {
      jj = null;
      while( jj < CNS_LNLEN )
      {
         ( cpt->mline[ii][jj] ) = null;
         jj++ ;
      };
      ( cpt->mflag[ii] ) = null;
      ii++ ;
   };

   ii = null;
   while( ii < CNS_LNLEN )
   {
      ( cpt->envmt[ii] ) = null; /* clear any environment indicator */
      ( cpt->cmmnt[ii] ) = null; /* clear any comment line */
      ( cpt->tasks[ii] ) = null; /* clear any tasks line */
      ( cpt->flags[ii] ) = null; /* clear any flags line */
      ( cpt->escpe[ii] ) = null; /* clear any escape path */
      ( cpt->cnfrm[ii] ) = null; /* clear any confirmation command */
      ( cpt->dfstr[ii] ) = null; /* clear any default of the former */
      ( cpt->rqlng[ii] ) = null; /* clear any long integer request */
      ( cpt->rqdbl[ii] ) = null; /* clear any floating point / double request */
      ( cpt->rqstr[ii] ) = null; /* clear any char string request */
      ii++ ;
   };

/* reset: */

   ( cpt->lnint ) = CNS_LNINT;
   ( cpt->posit ) = CNS_POSIT;
   ( cpt->lnlen ) = CNS_LNLEN;
   
   ( cpt->clscr ) = null;
   ( cpt->lglen ) = null;
   ( cpt->dblen ) = null;
   ( cpt->stlen ) = null;

   ( cpt->items ) = null;
   ( cpt->dflng ) = LONG_MIN;
   ( cpt->dfopt ) = -ONE;
   ( cpt->dflnf ) = -ONE;

   ( cpt->dfdbl ) = -HUGE_VALF;
   ( cpt->rtn ) = null;

   return cpt;
}
/*============================================================================*/
/************************* end of function 'clcnsl' ***************************/




# define FGETC(n) \
{ \
   ii = null; \
/* \
   ptr[ii] = fgetc( keyboard ); \
   [ This didn't work with Redhat's version of egcc C compiler ] \
*/ \
   ptr[ii] = fgetc( stdin ); \
   while(( ptr[ii] != '\n' )&&( ii < (n) )) \
   { \
      ii++ ; \
/* \
      ptr[ii] = fgetc( keyboard ); \
      [ This didn't work with Redhat's version of egcc C compiler ] \
*/ \
      ptr[ii] = fgetc( stdin ); \
   }; \
}

# define CNS_CLSTR(m,n) \
{ \
   ii = (m); \
   while ( ii < (n) ) \
   { \
      ptr[ii] = 0; \
      ii++ ; \
   }; \
}
# define CNS_BLANCS(m,n) \
{ \
   ii = (m); \
   while( ii < (n) ) \
   { \
      ptr[ii] = ' '; \
      ii++ ; \
   }; \
}
# define CNS_POINTS(m,n) \
{ \
   ii = (m); \
   while( ii < (n) ) \
   { \
      ptr[ii] = '.'; \
      ii++ ; \
   }; \
}
# define CNS_ARROW(m,n) \
{ \
   ii = (m); \
   ptr[ii] = '>'; \
   ii++ ; \
   while( ii < (n) ) \
   { \
      ptr[ii] = '-'; \
      ii++ ; \
   }; \
   ptr[ii] = '>'; \
   ii++ ; \
}
/*============================================================================*/

TXCNSL *txcnsl( TXCNSL *csp )
{
/* allusions: */
/*
   extern TXCNSL 
      cns;
*/
/* declarations: */

   static TXCNSL 
     *cpt = &cns;

   static short
      ii = null,
      jj = null,
      kk = null,
      ll = null;

   static char
      ptr[STS_SIZE] = {null},
    **endp = null;

/* prototypes: */

   TXCNSL 
      *clcnsl( TXCNSL *csp );
   
   char 
      *lotos ( long lngint, char length ),
      *dotos ( double lngdbl, char precision, char *format );
/*----------------------------------------------------------------------------*/
# if USE_NCURSES == 1
/* get the terminal info: */

   term = ( char *) getenv( "TERM" ); /* get the terminal */

   ii = tgetent( null, term );

   if( ONE != ii )
   {
      fprintf( stderr, "Error on getting the termcap info\n" ); 
      exit( EXIT_FAILURE );
   };
# endif
/*............................................................................*/
/* set buffer length = null: */
/*
   ii = setvbuf( keyboard, null, _IONBF, null );
   ii = setvbuf( display, null, _IONBF, null );
*/
   ii = setvbuf( stdin, null, _IONBF, null );
   ii = setvbuf( stdout, null, _IONBF, null );
/*............................................................................*/
/* initialize: */

   if( csp == null )
   {
      ii = null;
      while( ii < CNS_LNLEN )
      {
         ( cpt->instr[ii] ) = null;
         ii++ ;
      };

      ii = null;
      while( ii < CNS_ITEMS )
      {
         ( cpt->mflin[ii] ) = null;
         ii++ ;
      };

      ( cpt->inlng ) = null;
      ( cpt->inlng ) = null;
      ( cpt->indbl ) = null;
/*............................................................................*/
      cpt = clcnsl( cpt );     /*                                             */
/*...........................*/
      ( cpt->rtn ) = null;
      return cpt;
   }
   else
   {
      if( CNS_LNLEN < ( csp->lnlen ))
         ( csp->lnlen ) = CNS_LNLEN;

      if(( csp->lnint ) < null )
         ( csp->lnint ) = CNS_LNINT;

      if(( csp->lnlen ) < ( csp->lnint ))
         ( csp->lnint ) = ( csp->lnlen );

      if(( csp->posit ) < null )
         ( csp->posit ) = CNS_POSIT;

      if(( csp->lnlen ) < ( cpt->posit ))
         ( csp->posit ) = ( csp->lnlen );

      ii = null;
      while( ii < CNS_ITEMS )
      {
         ( cpt->mflin[ii] ) = ( cpt->mflag[ii] );

         if(( null == ( cpt->mflag[null] ))&&( null != ( cpt->mflag[ii] )))
            ( csp->mflag[null] ) = ii;

         ii++ ;
      };
   };
/*...........................................................................*/
/* clear screen, scroll ( csp->clscr ) number of lines: */

  start:

   if( null < ( csp->clscr ))
   {
      CLSCREEN;

      ii = ONE;
      while( ii < ( csp->clscr ))
      {
         printf( "\n" );
         ii++ ;
      };
   };

/*...........................................................................*/
/* write title [ if any ]: */

   if( null < strlen( csp->title ))
   {
      PRNORMAL( "\n" );
      ii = null;
      ll = ( csp->lnint );
      while( ii < ll )
      {
         printf( "%c", ' ' );
         ii++;
      };
      printf( "%s", csp->title );
   };
/*............................................................................*/
/* underline and display environment label [ if any ]: */

   if( null < strlen( csp->envmt ))
   {
      printf( "\n" );
      ii = null;
      ll = ( csp->lnint );
      while ( ii < ll )
      {
         printf( "%c", ' ' );
         ii++ ;
      };
      ll = ( csp->lnlen );
      while ( ii < ll )
      {
         printf( "%c", '=' );
         ii++ ;
      };

      CNS_CLSTR( null, STS_SIZE ); /* clear string ptr[] */
      ll = ( csp->lnint );
      CNS_BLANCS( null, ll ); /* indent */

      strcat( ptr, csp->cmmnt );

      ii += strlen( csp->cmmnt );
      jj = strlen( csp->envmt );
      kk = ii;
      ll = ( csp->lnlen ) - jj;

      CNS_BLANCS( kk, ll ); /* fill with blancs */

      strcat( ptr, csp->envmt );

      PRBLDCLR( "" );
      printf( "\n%s", ptr );
      PRNORMAL( "\n" );
   };

/*............................................................................*/
/* build the menu: */

   if( null < csp->items )
   {
/*............................................................................*/
/* display the tasks description [ if any ] and point to the items: */

      CNS_CLSTR( null, STS_SIZE ); /* clear string ptr[] */
      ll = ( csp->lnint );
      CNS_BLANCS( null, ll ); /* indent */

      strcat( ptr, csp->tasks );
      jj = strlen( ptr );
      kk = strlen( "Select |" );
      ll = CNS_POSIT - kk;

      CNS_BLANCS( jj, ll ); /* fill with blancs */

      strcat( ptr, "Select |" );
      printf( "\n%s", ptr );

      CNS_CLSTR( ll, STS_SIZE );   /* clear string ptr[] */
      ll = ( csp->posit ) - ONE;
      CNS_BLANCS( null, ll ); /* fill with blancs */

      ptr[ii] = 'V';
      printf( "\n%s", ptr );

/*............................................................................*/
/* display the menu items [ options ]: */

      jj = ONE;
      while( jj <= ( csp->items ))
      {
/*............................................................................*/
/* display the flag comment [ if any ]: */

         if(( jj == ( csp->mflag[null] ))&&( null != strlen( csp->flags )))
         {
            CNS_CLSTR( null, STS_SIZE ); /* clear string ptr[] */
            ll = ( csp->lnint );
            CNS_BLANCS( null, ll ); /* indent */

            strcat( ptr, csp->flags );
            kk = strlen( ptr );

            if( ONE < jj )
            {
               ll = ( short ) ( - strlen( "Change |" ));
               ll += CNS_POSIT;
               CNS_BLANCS( kk, ll ); /* fill with blancs */

               strcat( ptr, "Change |" );
            };
            printf( "\n\n%s", ptr );

            if( ONE < jj )
            {
               CNS_CLSTR( ll, STS_SIZE ); /* clear string ptr[] */
               ll = ( csp->posit ) - ONE;
               CNS_BLANCS( null, ll ); /* fill with blancs */

               ptr[ii] = 'V';
               printf( "\n%s", ptr );
            };
         };

         CNS_CLSTR( null, STS_SIZE ); /* clear string ptr[] */
         ll = ( csp->lnint );
         CNS_BLANCS( null, ll ); /* indent */

         ll = strlen( csp->mline[jj] );
         kk = null;
         while( kk < ll )
         {
            ptr[ii] = csp->mline[jj][kk];
            ii++ ;
            kk++ ;
         };
         ptr[ii] = ' ';
         ii++ ;
         ll = CNS_POSIT - FOUR;

         CNS_ARROW( ii, ll ); /* draw arrow >---...---> */

         ptr[ii] = ' ';
         ii++ ;
         ptr[ii] = '[';
         ii++ ;

         if(( csp->dflnf ) == jj )
            printf( "\n" );

         if(( csp->dfopt ) == jj )
         {
            printf( "\n%s", ptr );

            PRBLDCLR( lotos( jj, null ));
            PRNORMAL( "] " );
            PRBLDCLR( "<- default" );
            PRNORMAL( "" );

            if ((( csp->dflnf ) == ONE )&&( jj < csp->items ))
               printf( "\n" );
         }
         else
         {
            printf( "\n%s", ptr );
            if (( csp->mflin[jj] ) == -ONE )
            {
               PRNORMAL( lotos( jj, null ));
               PRNORMAL( "] " );
               PRNORMAL( "<- not set" );
               PRNORMAL( "" );
            }
            else if (( csp->mflin[jj] ) == ONE )
            {
               PRBLDCLR( lotos( jj, null ));
               PRNORMAL( "] " );
               PRBLDCLR( "<- set" );
               PRNORMAL( "" );
            }
            else
            {
               PRNORMAL( lotos( jj, null ));
               PRNORMAL( "] " );
            };
         };

         jj++ ;
      }; /* end while ( jj <= csp->items ) */

/*............................................................................*/
/* display the escape option: */

      if( null < strlen( csp->escpe ))
      {
         CNS_CLSTR( null, STS_SIZE ); /* clear string ptr[] */
         ll = ( csp->lnint );
         CNS_BLANCS( null, ll ); /* indent */

         ll = strlen( csp->escpe );
         kk = null;
         while( kk < ll )
         {
            ptr[ii] = csp->escpe[kk];
            ii++ ;
            kk++ ;
         };
         ptr[ii] = ' ';
         ii++ ;
         ll = ( csp->posit ) - FOUR;

         CNS_ARROW( ii, ll ); /* draw arrow >---...---> */

         ptr[ii] = ' ';
         ii++ ;
         ptr[ii] = '[';
         ii++ ;

         printf( "\n\n%s", ptr );

         if( null == ( csp->dfopt ))
         {
            PRBLDCLR( lotos( null, null ));
            PRNORMAL( "] " );
            PRBLDCLR( "<- default" );
            PRNORMAL( "" );
         }
         else
            printf( "%c%s", 48, "]" ); /* 48: ASCII char <null> */
         ii++ ;
      }; /* end if ( null != strlen( csp->escpe )) */
/*............................................................................*/
/* position the cursor: */

      CNS_CLSTR( null, STS_SIZE ); /* clear string ptr[] */
      ll = ( csp->posit );
      CNS_BLANCS( null, ll ); /* fill with blancs */

      ptr[ii] = ']';
      ii++ ;
      strcat( ptr, " <- ?" );
      printf( "\n\n%s", ptr );

      CNS_CLSTR( null, STS_SIZE ); /* clear string ptr[] */
      ll = ( csp->lnint );
      CNS_BLANCS( null, ll ); /* indent */

      strcat( ptr, "Please select item " );
      ii = strlen( ptr );
      ll = ( csp->posit ) - strlen( " [ enter 1,2,... or 0 ] >-> (" ) - TWO ;

      CNS_ARROW( ii, ll ); /* draw arrow >---...---> */

      strcat( ptr, " [ enter 1,2,... or 0 ] >-> [" );

      printf( "\r%s" , ptr );

      if( null <= ( csp->dfopt ))
      {
         PRBLDCLR( lotos( csp->dfopt, null ));
         PRNORMAL( "\b" );
      };
      
/*............................................................................*/
/* enter menu option on keyboard: */

      FGETC(CNS_LNLEN );

      if( ii == null )
         ( cpt->option ) = ( csp->dfopt );
      else
         ( cpt->option ) = strtol( ptr, endp, DEC );

      if( null != ( csp->mflin[( csp->option )] ))
      {
         ( csp->mflin[( csp->option )] ) *= ( -ONE );
         goto start;
      };

      if(( null == ( csp->option ))&&( null < strlen( csp->cnfrm )))
      {
         CNS_CLSTR( null, STS_SIZE ); /* clear string ptr[] */
         ll = ( csp->posit );
         CNS_BLANCS( null, ll ); /* fill with blancs */

         ptr[ii] = ']';
         ii++ ;
         strcat( ptr, " <- ?" );
         printf( "\r%s", ptr );

         CNS_CLSTR( null, STS_SIZE ); /* clear string ptr[] */
         ll = ( csp->lnint );
         CNS_BLANCS( null, ll ); /* indent */
/*
         e.g.: csp->cnfrm: "Do you really want to quit ? " 
*/
         strcat( ptr, csp->cnfrm );
         ii = strlen( ptr );
         ptr[ii] = ' ';
         ii++;

         ll = ( csp->posit ) - strlen( " [ enter y/n ] >-> [" ) - TWO;
         CNS_ARROW( ii, ll ); /* draw arrow >---...---> */

         strcat( ptr, " [ enter y/n ] >-> [" );
         printf( "\r%s", ptr );
         PRBLDCLR( "y");
         PRNORMAL( "\b");

         CNS_CLSTR( null, STS_SIZE ); /* clear string ptr[] */

         FGETC(CNS_LNLEN);

         if( null != strchr( ptr, 'n' ))
         {
            CNS_CLSTR( null, STS_SIZE ); /* clear string ptr[] */
            goto start;
         };
      };
   }; /* end if ( null < csp->items ) */

/*............................................................................*/
/* long integer request: */

   if( null < strlen( csp->rqlng ))
   {
      CNS_CLSTR( null, STS_SIZE ); /* clear string ptr[] */
      ll = ( csp->lnint );
      CNS_BLANCS( null, ll ); /* indent */

      if( LONG_MIN == ( csp->dflng ))
      {
         if( null == strncmp( csp->rqfrm, "brackets", TWO ))
         {
            if( null < ( csp->lglen ))
               ll = ( csp->posit ) + ( csp->lglen ) - ONE;
            else
               ll = ( csp->posit );

            CNS_BLANCS( ii, ll ); /* fill with blancs */
            strcat( ptr, "] <- ?" );
         }
         else if( null == strncmp( csp->rqfrm, "points", TWO )) 
         {
            if( null < ( csp->lglen ))
               ll = ( csp->posit ) + ( csp->lglen ) - ONE;
            else
               ll = ( csp->lnlen ) - ONE;

            CNS_POINTS( ii, ll ); /* fill with blancs */
            ptr[ii] = '?';
         }
         else if( null == strncmp( csp->rqfrm, "blancs", TWO )) 
         {
            if( null < ( csp->lglen ))
               ll = ( csp->posit ) + ( csp->lglen ) - ONE;
            else
               ll = ( csp->lnlen ) - ONE;

            CNS_BLANCS( ii, ll ); /* fill with blancs */
            ptr[ii] = '?';
         }
         else
         {
            if( null < ( csp->lglen ))
               ll = ( csp->posit ) + ( csp->lglen ) - ONE;
            else
               ll = ( csp->posit );

            CNS_BLANCS( ii, ll ); /* fill with blancs */
            strcat( ptr, "] <- ?" );
         };
         printf( "\r%s", ptr );
      }
      else
      { 
         kk = ( csp->lnlen ) - ( csp->posit );
         kk -= ( strlen( lotos( csp->dflng, null )) + FIVE );

         if( kk < null )
            ( csp->posit ) += kk;

         ll = ( csp->posit ) - TWO;
         CNS_BLANCS( null, ll ); /* fill with blancs */

         if( null == strncmp( csp->rqfrm, "brackets", TWO ))
         {
            printf( "\r%s[", ptr );
            strcpy( ptr, lotos( csp->dflng, null));
            PRBLDCLR( ptr );
            PRNORMAL( "] <- ?" );
         }
         else
         {
            printf( "\r%s[", ptr );
            strcpy( ptr, lotos( csp->dflng, null ));
            PRBLDCLR( ptr );
            PRNORMAL( "] <- ?" );
         };
      };

      CNS_CLSTR( null, STS_SIZE ); /* clear string ptr[] */
      ll = ( csp->lnint );
      CNS_BLANCS( null, ll ); /* indent */

      strcat( ptr, csp->rqlng );
      ii = strlen( ptr );
      ptr[ii] = ' ';
      ii++;

      if( LONG_MIN == ( csp->dflng ))
      {
         ll = ( csp->posit ) - FOUR;
         CNS_ARROW( ii, ll ); /* draw arrow >---...---> */

         if( null == strncmp( csp->rqfrm, "brackets", TWO )) 
            strcat( ptr, " [" );
         else if( null == strncmp( csp->rqfrm, "points", TWO )) 
            strcat( ptr, " :" );
         else if( null == strncmp( csp->rqfrm, "blancs", TWO ))
            strcat( ptr, " :" );
         else
            strcat( ptr, " [" );
      }
      else
      {
         ll = ( csp->posit ) - FOUR;
         CNS_ARROW( ii, ll ); /* draw arrow >---...---> */

         if( null == strncmp( csp->rqfrm, "brackets", TWO )) 
            strcat( ptr, " [" );
         else
            strcat( ptr, " [" );
      };
      printf( "\r%s", ptr );

      FGETC(CNS_LNLEN);

      if( ii == null )
         ( cpt->inlng ) = ( csp->dflng );
      else
         ( cpt->inlng ) = strtol( ptr, endp, DEC );
   };

/*............................................................................*/
/* floating point / double request: */

   if( null < strlen( csp->rqdbl ))
   {
      CNS_CLSTR( null, STS_SIZE ); /* clear string ptr[] */
      ll = ( csp->lnint );
      CNS_BLANCS( null, ll ); /* indent */

      if( -HUGE_VALF == ( csp->dfdbl ))
      {
         if( null == strncmp( csp->rqfrm, "brackets", TWO ))
         {
            if( null < ( csp->dblen ))
               ll = ( csp->posit ) + ( csp->lglen ) - ONE;
            else
               ll = ( csp->posit );

            CNS_BLANCS( ii, ll ); /* fill with blancs */
            strcat( ptr, "] <- ?" );
         }
         else if( null == strncmp( csp->rqfrm, "points", TWO )) 
         {
            if( null < ( csp->dblen ))
               ll = ( csp->posit ) + ( csp->dblen ) - ONE;
            else
               ll = ( csp->lnlen ) - ONE;

            CNS_POINTS( ii, ll ); /* fill with blancs */
            ptr[ii] = '?';
         }
         else if( null == strncmp( csp->rqfrm, "blancs", TWO )) 
         {
            if( null < ( csp->dblen ))
               ll = ( csp->posit ) + ( csp->dblen ) - ONE;
            else
               ll = ( csp->lnlen ) - ONE;

            CNS_BLANCS( ii, ll ); /* fill with blancs */
            ptr[ii] = '?';
         }
         else
         {
            if( null < ( csp->dblen ))
               ll = ( csp->posit ) + ( csp->dblen ) - ONE;
            else
               ll = ( csp->posit );

            CNS_BLANCS( ii, ll ); /* fill with blancs */
            strcat( ptr, "] <- ?" );
         };
         printf( "\r%s", ptr );
      }
      else
      { 
         kk = ( csp->lnlen ) - ( csp->posit );
         kk -= ( strlen( dotos( csp->dfdbl, 7, "e" )) + FIVE );

         if( kk < null )
            ( csp->posit ) += kk;

         ll = ( csp->posit ) - TWO;
         CNS_BLANCS( null, ll ); /* fill with blancs */

         if( null == strncmp( csp->rqfrm, "brackets", TWO ))
         {
            printf( "\r%s[", ptr );
            strcpy( ptr, dotos( csp->dfdbl, 7, "e" ));
            PRBLDCLR( ptr );
            PRNORMAL( "] <- ?" );
         }
         else
         {
            printf( "\r%s[", ptr );
            strcpy( ptr, dotos( csp->dfdbl, 7, "e" ));
            PRBLDCLR( ptr );
            PRNORMAL( "] <- ?" );
         };
      };

      CNS_CLSTR( null, STS_SIZE ); /* clear string ptr[] */
      ll = ( csp->lnint );
      CNS_BLANCS( null, ll ); /* indent */

      strcat( ptr, csp->rqdbl );
      ii = strlen( ptr );
      ptr[ii] = ' ';
      ii++;

      if( -HUGE_VALF == ( csp->dfdbl ))
      {
         ll = ( csp->posit ) - FOUR;
         CNS_ARROW( ii, ll ); /* draw arrow >---...---> */

         if( null == strncmp( csp->rqfrm, "brackets", TWO )) 
            strcat( ptr, " [" );
         else if( null == strncmp( csp->rqfrm, "points", TWO )) 
            strcat( ptr, " :" );
         else if( null == strncmp( csp->rqfrm, "blancs", TWO ))
            strcat( ptr, " :" );
         else
            strcat( ptr, " [" );

         ( csp->dfdbl ) = null;
      }
      else
      {
         ll = ( csp->posit ) - FOUR;
         CNS_ARROW( ii, ll ); /* draw arrow >---...---> */
         if( null == strncmp( csp->rqfrm, "brackets", TWO )) 
            strcat( ptr, " [" );
         else
            strcat( ptr, " [" );
      };
      printf( "\r%s", ptr );

      FGETC(CNS_LNLEN);

      if( ii == null )
         ( cpt->indbl ) = ( csp->dfdbl );
      else
         ( cpt->indbl ) = strtod( ptr, endp );
   };

/*............................................................................*/
/* character string request: */

   if( null < strlen( csp->rqstr ))
   {
      CNS_CLSTR( null, STS_SIZE ); /* clear string ptr[] */
      ll = ( csp->lnint );
      CNS_BLANCS( null, ll ); /* indent */

      if( null == strlen( csp->dfstr ))
      {
         if( null == strncmp( csp->rqfrm, "brackets", TWO ))
         {
            if( null < ( csp->stlen ))
               ll = ( csp->posit ) + ( csp->stlen ) - ONE;
            else
               ll = ( csp->posit );

            CNS_BLANCS( ii, ll ); /* fill with blancs */
            strcat( ptr, "] <- ?" );
         }
         else if( null == strncmp( csp->rqfrm, "points", TWO )) 
         {
            if( null < ( csp->stlen ))
               ll = ( csp->posit ) + ( csp->stlen ) - ONE;
            else
               ll = ( csp->lnlen ) - ONE;

            CNS_POINTS( ii, ll ); /* fill with blancs */
            ptr[ii] = '?';
         }
         else if( null == strncmp( csp->rqfrm, "blancs", TWO )) 
         {
            if( null < ( csp->stlen ))
               ll = ( csp->posit ) + ( csp->stlen ) - ONE;
            else
               ll = ( csp->lnlen ) - ONE;

            CNS_BLANCS( ii, ll ); /* fill with blancs */
            ptr[ii] = '?';
         }
         else
         {
            if( null < ( csp->stlen ))
               ll = ( csp->posit ) + ( csp->stlen ) - ONE;
            else
               ll = ( csp->posit );

            CNS_BLANCS( ii, ll ); /* fill with blancs */
            strcat( ptr, "] <- ?" );
         };
         printf( "\r%s", ptr );
      }
      else
      { 
         kk = ( csp->lnlen ) - ( csp->posit );
         kk -= ( strlen( csp->dfstr ) + FIVE );

         if( kk < null )
            ( csp->posit ) += kk;

         ll = ( csp->posit ) - TWO;
         CNS_BLANCS( null, ll ); /* fill with blancs */

         if( null == strncmp( csp->rqfrm, "brackets", TWO ))
         {
            printf( "\r%s[", ptr );
            strcpy( ptr, csp->dfstr );
            PRBLDCLR( ptr );
            PRNORMAL( "] <- ?" );
         }
         else
         {
            printf( "\r%s[", ptr );
            strcpy( ptr, csp->dfstr );
            PRBLDCLR( ptr );
            PRNORMAL( "] <- ?" );
         };
      };

      CNS_CLSTR( null, STS_SIZE ); /* clear string ptr[] */
      ll = ( csp->lnint );
      CNS_BLANCS( null, ll ); /* indent */

      strcat( ptr, csp->rqstr );
      ii = strlen( ptr );
      ptr[ii] = ' ';
      ii++;

      if( null == strlen ( csp->dfstr ))
      {
         ll = ( csp->posit ) - FOUR;
         CNS_ARROW( ii, ll ); /* draw arrow >---...---> */

         if( null == strncmp( csp->rqfrm, "brackets", TWO )) 
            strcat( ptr, " [" );
         else if( null == strncmp( csp->rqfrm, "points", TWO )) 
            strcat( ptr, " :" );
         else if( null == strncmp( csp->rqfrm, "blancs", TWO ))
            strcat( ptr, " :" );
         else
            strcat( ptr, " [" );
      }
      else
      {
         ll = ( csp->posit ) - FOUR;
         CNS_ARROW( ii, ll ); /* draw arrow >---...---> */
         if( null == strncmp( csp->rqfrm, "brackets", TWO )) 
            strcat( ptr, " [" );
         else
            strcat( ptr, " [" );
      };
      printf( "\r%s", ptr );

      ii = null;
      CNS_CLSTR( null, STS_SIZE ); /* clear string ptr[] */

      FGETC(CNS_LNLEN);

      if( ii == null )
         strcpy( cpt->instr, csp->dfstr );
      else
         strncpy( cpt->instr, ptr, ii );
   };

/*............................................................................*/
   cpt = clcnsl( csp );  /* clear console input                               */
/*.....................*/
   ( cpt->rtn ) = null;
   return cpt;
}
/*============================================================================*/
# undef CNS_CLSTR
# undef CNS_BLANCS
# undef CNS_POINTS
# undef CNS_ARROW
/************************ end of function txcnsl(*) ***************************/




# define DO_DSPLAY "dsplay(*)"
/*******************************************************************************
*                                                                              *
*   Function dsplay in CODIS-2.0r1                                             *
*                                                                              *
*   Displays process states [ running cursor function ] and runtime messages   *
*   on screen.                                                                 *
*                                                                              *
*   (C) SHEIN; Bad Aibling, September 1999                    Steffen Hein     *
*   [ Update: 30 January 2006 ]                               <hein@bnro.de>   *
*                                                                              *
*******************************************************************************/
# include "../math/scropr.h"
/*----------------------------------------------------------------------------*/
# define DSP_RMRGN_   1 /* right margin */
# define DSP_LMRGN_   1 /* left margin */
# define DSP_SIGNS_  78 /* monitored number of charcters */
# define DSP_LINE__ ( DSP_SIGNS_ + DSP_RMRGN_ + DSP_LMRGN_ )
# define DSP_FXPREC   3 /* fixed point precision of percent indication */

# define DSP_FLECHE  45 /* ASCII character '-' */
# define DSP_POINTE  62 /* ASCII character '>' */
# define DSP_MEET__ 124 /* ASCII character '|' */
# define DSP_TRACE_  32 /* ASCII characters 32 = ' ' or 46 = '.' */
# define DSP_EQUAL_  61 /* ASCII character '=' */
/*----------------------------------------------------------------------------*/
static char *term = {null};     /* terminal type string */
/*----------------------------------------------------------------------------*/
/*
*//* # include "dsptyp.h" 
*/
static DSPLAY disp = {null};
/*----------------------------------------------------------------------------*/
static int unbuff     = _IONBF;
static size_t bufsize =  null;
/*============================================================================*/

DSPLAY *\
dsplay( DSPLAY *dsp )
{
  static DSPLAY
    *rpt = &disp;

  static char
      cr = null,
      arrow = null,
      dsplay[LGS_SIZE] = {null};

  static short
      lc = null,
      cc = null,
      sln = null, /* length of new string under running cursor */ 
      slp = null, /* length of previous string */ 
      crs = null; /* the actual cursor position */

  static double
      pc = ZERO;

  static char
     pcs[STS_SIZE] = {null};

  char *dotos ( double xx, char precision, char *format );
/*---------------------------------------------------------------------------*/
   ( rpt->rtn ) = null;

   if( dsp == null ) /* initialize terminal buffer and struct DSPLAY */
   {
/* set buffer length to null: */

      setvbuf( stdout, null, unbuff, bufsize );
      term = ( char *) getenv( "TERM" );

# if USE_NCURSES == 1
/* get the terminal type: */

      if ( term != null )
      {
         cc = tgetent( null, term );

         if( ONE != cc )
         {
            fprintf( stderr, "Error on getting the termcap info\n" );
            exit( EXIT_FAILURE );
         };
      };
# endif

      ( rpt->display ) = stdout;
      ( rpt->option ) = null;
      ( rpt->state ) = null;
      ( rpt->range ) = null;
      ( rpt->fleps ) = null;
      ( rpt->rcsps ) = null;

      cc = null;
      while ( cc < LGS_SIZE )
      {
	 dsplay[cc] = null;
         ( rpt->messge[cc] ) = null;
         cc++ ;
      };

      cc = null;
      cr = null;
      lc = null;
      sln = null;
      slp = null;
      crs = null;
      pc = ZERO;
      
      return rpt;
/*............................................................................*/
   }
   else
      rpt = dsp;
/*............................................................................*/

   switch( dsp->option )
   {
/*............................................................................*/
# if (( DSP_CMODE_ == 0 ) \
    ||( DSP_CMODE_ == 1 ))

     default: /* write running cursor [ evtlly into  any message ] */

      if (( dsp->range ) != null )
      {
	 lc = DSP_SIGNS_ - ONE;
         crs = ( int )(( lc*( dsp->state )) / ( dsp->range ));

         if( crs <= ( dsp->rcsps ))
            return rpt;
         else if ( lc < crs )
            return rpt;
/*............................................................................*/

         arrow = DSP_FLECHE;

/* rotate helix: */

	 if ( null < DSP_LMRGN_ )
	 {
            switch ( cr )
            {
              case 0:
               cr = 124; /* 124: ASCII character '|' */
               break;

              case 124:
               cr = 47; /* 47: ASCII character '/' */
               break;

              case 47:
               cr = 45; /* 45: ASCII character '-' */
               break;

              case 45:
               cr = 92; /* 97: ASCII character '\' */
               break;

              case 92:
               cr = 124;
               break;
            };
         }
	 else
            cr = 32; /* 32: ASCII character space */

/* write message on display: */

         fprintf(( dsp->display ), "\r" );

         cc = null;
         while ( cc < ( DSP_LMRGN_-ONE ))
         {
           fprintf(( dsp->display ), "%c", 32 );
            cc++;
         };

         PRBLDCLR( "" );
         fprintf( dsp->display, "%c", cr );
         PRNORMAL( "" );

         cc = null;
         while ( cc < crs )
         {
            fprintf(( dsp->display ), "%c", dsplay[cc] );
            cc++;
         };

         PRBLDCLR( "" );
         fprintf( dsp->display, "%c\b", DSP_POINTE );
         PRNORMAL( "" );

         while ( cc < sln )
         {
            fprintf(( dsp->display ), "%c", dsplay[cc] );
            cc++;
         };
         while ( cc < lc )
         {
            dsplay[cc] = arrow;
            cc++;
         };

         ( rpt->rcsps ) = crs; /* the new cursor position */

         return rpt;
      }
      else
         return rpt;

      break;
/*............................................................................*/
# else /* if DSP_CMODE != 0,1 */

     default: /* write running cursor [ evtlly into  any message ] */

      if (( dsp->range ) != null )
      {
	 lc = DSP_SIGNS_ - ONE;
         crs = ( int )(( lc*( dsp->state )) / ( dsp->range ));

         if( crs <= ( dsp->rcsps ))
            return rpt;
         else if ( lc < crs )
            return rpt;
/*............................................................................*/

         arrow = DSP_FLECHE;

/* rotate helix: */

         fprintf( dsp->display, "\r" );

	 if ( null < DSP_LMRGN_ )
	 {
            switch ( cr )
            {
              case 0:
               cr = 124; /* 124: ASCII character '|' */
               break;

              case 124:
               cr = 47; /* 47: ASCII character '/' */
               break;

              case 47:
               cr = 45; /* 45: ASCII character '-' */
               break;

              case 45:
               cr = 92; /* 97: ASCII character '\' */
               break;

              case 92:
               cr = 124;
               break;
            };
         }
	 else
            cr = 32; /* 32: ASCII character space */

/* write message on display: */

         fprintf(( dsp->display ), "\r%*c", DSP_LMRGN_, 124 );

         cc = null;
         while ( cc < ( dsp->rcsps ))
         {
            fprintf(( dsp->display ), "%c", dsplay[cc] );
            cc++;
         };
         fprintf(( dsp->display ), "%c", 32 );
         while ( cc < sln )
         {
            fprintf(( dsp->display ), "%c", dsplay[cc] );
            cc++;
         };

/* write cursor on new position [ eventually into message ]: */

         fprintf( dsp->display, "\r" );

         cc = ONE;
         while ( cc < DSP_LMRGN_ )
         {
            fprintf(( dsp->display ), "%c", 32 );
	    cc++;
         };

         PRBLDCLR( "" );
         fprintf( dsp->display, "\r%c", cr );
         PRNORMAL( "" );

         cc = null;
         while ( cc < crs )
         {
            fprintf(( dsp->display ), "%c", dsplay[cc] );
	    cc++;
         };

         PRBLDCLR( "" );
         fprintf( dsp->display, "%c\b", DSP_POINTE );
         PRNORMAL( "" );

	 cc = sln;
         while ( cc < lc )
         {
            dsplay[cc] = arrow;
            cc++;
         };
	 
	 ( dsp->rcsps ) = crs; /* the new cursor position */
         return rpt;
      }
      else
         return rpt;

      break;
/*............................................................................*/
# endif /* if DSP_CMODE != 0,1 */
/*............................................................................*/
# if DSP_CMODE_ == 0

     case 'c': /* move cursor and display percent state [ mode 2 ] */
     case 'C':

      if (( dsp->range ) != null )
      {
         lc = ( DSP_SIGNS_ - ( DSP_FXPREC+FIVE ));
         crs = ( int )(( lc*( dsp->state )) / ( dsp->range ));

         if( crs <= ( dsp->rcsps ))
            return rpt;
         else if ( lc < crs )
            return rpt;
/*............................................................................*/

         arrow = DSP_EQUAL_;

/* rotate helix: */

         switch ( cr )
         {
           case 0:
            cr = 124; /* 124: ASCII character '|' */
            break;

           case 124:
            cr = 47; /* 47: ASCII character '/' */
            break;

           case 47:
            cr = 45; /* 45: ASCII character '-' */
            break;

           case 45:
            cr = 92; /* 97: ASCII character '\' */
            break;

           case 92:
            cr = 124;
            break;
         };

/* compute and write percent value: */      

         pc = 100.* ( double )( dsp->state ) / ( dsp->range );

         pcs[0] = cr;
         pcs[1] = null;
         strcat( pcs, "[" );
         strcat( pcs, ( dotos( pc, DSP_FXPREC, "fixed" )));
         strcat( pcs, "%]" );

         fprintf(( dsp->display ), "\r" );

         PRBLDCLR( "" );
         fprintf(( dsp->display ), "\r%*s", ( DSP_SIGNS_ + DSP_LMRGN_ ), pcs );
         PRNORMAL( "" );

/* write complete message on display: */

         fprintf(( dsp->display ), "\r" );

         cc = null;
         while ( cc < DSP_LMRGN_ )
         {
            fprintf(( dsp->display ), "%c", 32 );
            cc++;
         };

         cc = null;
         while ( cc < crs )
         {
            fprintf(( dsp->display ), "%c", dsplay[cc] );
            cc++;
         };

         PRBLDCLR( "" );
         fprintf( dsp->display, "%c", DSP_POINTE );
         PRNORMAL( "" );

         while ( cc < sln )
         {
            fprintf(( dsp->display ), "%c", dsplay[cc] );
            cc++;
         };
         while ( cc < lc )
         {
            dsplay[cc] = arrow;
            cc++;
         };

         fprintf( dsp->display, "\b" );

         ( rpt->rcsps ) = crs; /* the new cursor position */

         return rpt;
      }
      else
         return rpt;

      break;
/*............................................................................*/
# elif DSP_CMODE_ == 1

     case 'c': /* move cursor and display percent state [ mode 2 ] */
     case 'C':

      if (( dsp->range ) != null )
      {
         lc = ( DSP_SIGNS_ - ( DSP_FXPREC+FIVE ));
         crs = ( int )(( lc*( dsp->state )) / ( dsp->range ));

         if( crs <= ( dsp->rcsps ))
            return rpt;
         else if ( lc < crs )
            return rpt;
/*............................................................................*/

         arrow = DSP_EQUAL_;

/* rotate helix: */

         switch ( cr )
         {
           case 0:
            cr = 124; /* 124: ASCII character '|' */
            break;

           case 124:
            cr = 47; /* 47: ASCII character '/' */
            break;

           case 47:
            cr = 45; /* 45: ASCII character '-' */
            break;

           case 45:
            cr = 92; /* 97: ASCII character '\' */
            break;

           case 92:
            cr = 124;
            break;
         };

/* compute percent value: */      

         pc = 100.* ( double )( dsp->state ) / ( dsp->range );

         pcs[0] = cr;
         pcs[1] = null;
         strcat( pcs, "[" );
         strcat( pcs, ( dotos( pc, DSP_FXPREC, "fixed" )));
         strcat( pcs, "%]" );

/* write complete message on display: */

         fprintf(( dsp->display ), "\r" );

         cc = null;
         while ( cc < DSP_LMRGN_ )
         {
            fprintf(( dsp->display ), "%c", 32 );
            cc++;
         };

         cc = null;
         while ( cc < crs )
         {
            fprintf(( dsp->display ), "%c", dsplay[cc] );
            cc++;
         };

         PRBLDCLR( "" );
         fprintf( dsp->display, "%c", DSP_POINTE );
         PRNORMAL( "" );

         while ( cc < sln )
         {
            fprintf(( dsp->display ), "%c", dsplay[cc] );
            cc++;
         };
         while ( cc < lc )
         {
            fprintf(( dsp->display ), "%c", DSP_TRACE_ );
            dsplay[cc] = arrow;
            cc++;
         };

         PRBLDCLR( "\b" );
         if ( lc <= crs )
            fprintf(( dsp->display ), "%c", DSP_POINTE );
         else
            fprintf(( dsp->display ), "%c", pcs[null] );

         cc = ONE;
         while ( cc < 8 )
         {
            fprintf(( dsp->display ), "%c", pcs[cc] );
            cc++;
         };
         PRNORMAL( "" );

         ( rpt->rcsps ) = crs; /* the new cursor position */

         return rpt;
      }
      else
         return rpt;

      break;
/*............................................................................*/
# else /* if DSP_CMODE_ != 1,2 */

     case 'c': /* move cursor and display percent state */
     case 'C':

      if (( dsp->range ) != null )
      {
         lc = ( DSP_SIGNS_ - ( DSP_FXPREC+FIVE ));
         crs = ( int )(( lc*( dsp->state )) / ( dsp->range ));

         if( crs <= ( dsp->rcsps ))
            return rpt;
         else if ( lc < crs ) 
            return rpt;
/*............................................................................*/

         arrow = DSP_EQUAL_;

/* rotate helix: */

         switch ( cr )
         {
           case 0:
            cr = 124; /* 124: ASCII character '|' */
            break;

           case 124:
            cr = 47; /* 47: ASCII character '/' */
            break;

           case 47:
            cr = 45; /* 45: ASCII character '-' */
            break;

           case 45:
            cr = 92; /* 97: ASCII character '\' */
            break;

           case 92:
            cr = 124;
            break;
         };

/* compute and write percent value: */      

         pc = 100.* ( double )( dsp->state ) / ( dsp->range );

         pcs[0] = cr;
         pcs[1] = null;
         strcat( pcs, "[" );
         strcat( pcs, ( dotos( pc, DSP_FXPREC, "fixed" )));
         strcat( pcs, "%]" );
	 
         PRBLDCLR( "" );
         fprintf(( dsp->display ), "\r%*s", ( DSP_SIGNS_ + DSP_LMRGN_ ), pcs );
         PRNORMAL( "" );

/* write complete message on display: */

         fprintf(( dsp->display ), "\r%*c", DSP_LMRGN_, 124 );
         cc = null;
         while ( cc < ( dsp->rcsps ))
         {
            fprintf(( dsp->display ), "%c", dsplay[cc] );
            cc++;
         };
         fprintf(( dsp->display ), "%c", 32 );
         while ( cc < sln )
         {
            fprintf(( dsp->display ), "%c", dsplay[cc] );
            cc++;
         };

/* write cursor on new position [ eventually into message ]: */

         fprintf(( dsp->display ), "\r" );
         cc = null;
         while ( cc < DSP_LMRGN_ )
         {
            fprintf(( dsp->display ), "%c", 32 );
	    cc++;
         };
         cc = null;
         while ( cc < crs )
         {
            fprintf(( dsp->display ), "%c", dsplay[cc] );
	    cc++;
         };

         PRBLDCLR( "" );
         fprintf( dsp->display, "%c\b", DSP_POINTE );
         PRNORMAL( "" );

         cc = sln;
         while ( cc < lc )
         {
            dsplay[cc] = arrow;
            cc++;
         };

         ( rpt->rcsps ) = crs; /* the new cursor position */

         return rpt;
      }
      else
         return rpt;

      break;
/*............................................................................*/

# endif /* DSP_CMODE_ == 1 */
/*............................................................................*/

     case 'p': /* only display percent state */
     case 'P':

      if (( dsp->range ) != null )
      {
         crs = ( int ) DSP_SIGNS_ * ( dsp->state ) / ( dsp->range );

         if( crs == ( dsp->rcsps ))
            return rpt;
         else
            ( rpt->rcsps ) = crs;
      }
      else
         return rpt;
/*............................................................................*/

      pc = 100.* ( double )( dsp->state ) / ( dsp->range );
      fseek( dsp->display, ( dsp->fleps ), SEEK_SET );
      fprintf( dsp->display,
         " [>>->%s%%]", ( dotos( pc, DSP_FXPREC, "fixed" )));

      break;
/*............................................................................*/

     case null: /* clear display line */

      fprintf( dsp->display, "\r" );

      cc = null;
      while( cc < DSP_LINE__ )
      {
         fprintf( dsp->display, " " );
         cc++ ;
      };

      fprintf( dsp->display, "\r" );

      cc = null;
      while( cc < DSP_LMRGN_ )
      {
         fprintf( dsp->display, " " );
         cc++ ;
      };

      if(( dsp->messge ) != null )
         fprintf( dsp->display, "\r %s", ( dsp->messge ));

      lc = null;
      crs = null;
      crs = null;
      sln = null;
      arrow = null;

      break;
/*............................................................................*/

     case 's': /* dsp->display start message, initialize struct dsp */
     case 'S':

      crs = null; /* reset cursor position */
      sln = null; /* reset length of cursor message */
      
      fprintf( dsp->display, "\r" );

      cc = null;
      while( cc < DSP_LMRGN_ )
      {
         fprintf( dsp->display, "%c", 32 );
         cc++ ;
      };
      fprintf( dsp->display, "%s\n", ( dsp->messge ));

      arrow = DSP_TRACE_;

      break;
/*............................................................................*/

     case 'm': /* dsp->display restart message, left to cursor  */
     case 'M': /* [ right from former text, which is retained ] */

      ( rpt->rcsps ) = null;
      sln = strlen( dsp->messge );

      cc = null;
      while( cc < sln )
      {
         dsplay[cc] = ( dsp->messge[cc] );
         cc++ ;
      };
      while( cc < DSP_SIGNS_ )
      {
         dsplay[cc] = arrow;
         cc++ ;
      };

      if ( sln != slp )
      {
         slp = sln;

         fprintf( dsp->display, "\r" );
	 cc = null;
         while( cc < DSP_LMRGN_ )
         {
            fprintf(( dsp->display ), "%c", 32 );
	    cc++ ;
         };
	 cc = null;
         while( cc < sln )
         {
            fprintf( dsp->display, "%c", dsplay[cc] );
	    cc++ ;
         };
         lc = DSP_SIGNS_ - ONE;
         while( cc < lc )
         {
            cc++ ;
            fprintf( dsp->display, "%c", DSP_TRACE_ );
         };
         fprintf( dsp->display, "%c\r", DSP_MEET__ );
      };

      break;
/*............................................................................*/

     case 'd': /* dsp->display the message */
     case 'D':

      lc = strlen( dsp->messge );
      crs = DSP_SIGNS_ - DSP_LMRGN_;

      fprintf( dsp->display, "\r" );
      cc = null;
      while( cc < DSP_LMRGN_ )
      {
         fprintf( dsp->display, "%c", 32 );
         cc++ ;
      };
      cc = null;
      while (( cc <= crs )&&( cc <= lc ))
      {
         fprintf( dsp->display, "%c", ( dsp->messge[cc] ));
         cc++ ;
      };

      break;
   }; /* end switch( dsp->option ) */

   return rpt;
}
/*============================================================================*/
# undef DSP_LMRGN_
# undef DSP_SIGNS_
# undef DSP_FXPREC
# undef DSP_LINE__
# undef DSP_FLECHE
# undef DSP_EQUAL_
# undef DSP_POINTE
# undef DSP_MEET__
# undef DSP_TRACE_
/************************* end of function dsplay(*) **************************/




# define DO_NUSSELT "nusselt(*)"
/*******************************************************************************
*                                                                              *
*   Function nusselt(*) in CODIS-2.0r1                                         *
*                                                                              *
*   Returns the nusselt number as function of Reynolds amd Prandtl numbers,    *
*   and of ratio = ( diameter / length ).                                      *
*                                                                              *
*   Formulae from Groeber, Erk, Grigull: Die Grundgesetze der Waermeueber-     *
*   tragung, ISBN 3-540-02982-6, Spinger-Verlag Berlin, Heidelberg, New York,  *
*   3.Auflage 1963. [ The indicated page numbers refer to this book.]          *
*                                                                              *
*   (C) SHEIN; Bad Aibling, March 2003                         Steffen Hein    *
*   [ Update: 30 January 2006 ]                               <hein@bnro.de>   *
*                                                                              *
*******************************************************************************/
/*
# include <stdio.h>
# include <stdlib.h>
# include <stddef.h>
# include <string.h>
# include <float.h>
# include <math.h>
*/
/*----------------------------------------------------------------------------*/
/*
# include "consts.h"
*/
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




# define DO_GRAZET "grazet(*)"
/*******************************************************************************
*                                                                              *
*   Function grazet(*) in CODIS-2.0r1                                          *
*                                                                              *
*   This function returns the heat conduction coefficient zeta( gr )           *
*   for a system of horizontal coaxial cylinders.                              *
*   The range of the diameter ratio is 1.2 < Da/Di= ratio < 8.1                *
*   The values are due to BECKMANN: Forschung Bd.2, Heft 5 (1931); pp.170-178  *
*                                                                              *
*   (C) SHEIN; Bad Aibling, March 2003                         Steffen Hein    *
*   [ Update: 30 January 2006 ]                               <hein@bnro.de>   *
*                                                                              *
*******************************************************************************/
/*
# include <stdio.h>
# include <stdlib.h>
# include <stddef.h>
# include <float.h>
# include <math.h>
*/
/*----------------------------------------------------------------------------*/
/*
# include "../math/consts.h"
*/
/*----------------------------------------------------------------------------*/

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

   aa = ( apr - ratio )/( apr - bpr ); /* absence of convection for gr < xhor */
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
         anm = cnm / ep1 / pow (( xlm-xhr ), ( ep1 - 1. ));
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




# define DO_GRANUS "granus(*)"
/*******************************************************************************
*                                                                              *
*   Function granus(*) in CODIS-2.0r1                                          *
*                                                                              *
*   This function returns the nusselt number Nu for a horizontal cylinder as   *
*   function of the Grashof number gr, in the domain                           *
*                                                                              *
*                     1.e-04  <=  gr  <= 1.e+08                                *
*                                                                              *
*   The values are due to Prandtl, Fuehrer durch die Stroemungslehre,          *
*   chap. 5.16.                                                                *
*                                                                              *
*   (C) SHEIN; Bad Aibling, March 2003                         Steffen Hein    *
*   [ Update: 30 January 2006 ]                               <hein@bnro.de>   *
*                                                                              *
*******************************************************************************/
/*
# include <stdio.h>
# include <stdlib.h>
# include <stddef.h>
# include <float.h>
# include <math.h>
*/
/*----------------------------------------------------------------------------*/
/*
# include "consts.h"
*/
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




# define DO_TRNORM "trnorm(*)"
/*******************************************************************************
*                                                                              *
*   Function trnorm(*) in CODIS-2.0r1                                          *
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
*   (C) SHEIN, Bad Aibling; June 2002                          Steffen Hein    *
*   [ Update: 30 January 2006 ]                               <hein@bnro.de>   *
*                                                                              *
*******************************************************************************/
/*
# include <stdio.h>
# include <stdlib.h>
# include <stddef.h>
# include <math.h>
*/
/*----------------------------------------------------------------------------*/
/*
# include "consts.h"
*/
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
# undef GIANT_VAL
/************************** end of file hlsmath.c *****************************/
