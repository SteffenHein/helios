/* [ file: hlsutls.c ] */
/*******************************************************************************
*                                                                              *
*   Gauss-Jordan elimination with full pivoting; HELIOS release v1.0r1         *
*   [ Heat and Electric Losses In Ordinary wave guiding Structures ]           *
*                                                                              *
*   Given any non-singular complex square matrix M of rank n > 0 and           *
*   an array of n complex numbers a[i] ( i=0,.., n-1 ), this function          *
*   returns the solution z[k] of the linear equation                           *
*                                                                              *
*                sum( k=0,...,n-1)  M[i][k] * z[k]  =  a[i] .                  *
*                                                                              *
*   To this end, M and a[i] ( i=0,...,n-1 ) are transferred to the function    *
*   as the columns k=0,...,n-1 and k=n, respectively, of the structure         *
*   matrix                                                                     *
*                                                                              *
*          ( gjp->mr[i][k] + j*( gjp->mi[i][k] ))  ( i=0,...,n-1 ).            *
*                                                                              *
*   The function the writes the solution z[k] into                             *
*                                                                              *
*          ( gjp->zr[k][0] + j*( gjp->zi[k][0] ))  ( k=0,...,n-1 )             *
*                                                                              *
*   ( option opt = 'e' ).                                                      *
*                                                                              *
*   Also, in option opt = 'i', the inverse matrix of M,  M^(-1),               *
*   is computed and written into                                               *
*                                                                              *
*         ( gjp->zr[i][k] + j*( gjp->zi[i][k] ))  ( i,k=0,...,n-1 )            *
*                                                                              *
*   (C) SHEIN; Munich, December 2021                       Steffen Hein        *
*   [ Update: December 27, 2021 ]                       <contact@sfenx.de>     *
*                                                                              *
*******************************************************************************/
# define _POSIX_SOURCE 1 /* some headers of the POSIX.1 standard will be used */
/*----------------------------------------------------------------------------*/
# include <stdio.h>
# include <string.h>
# include <stdlib.h>
# include <stddef.h>
# include <stdarg.h>
# include <ctype.h>
# include <time.h>            /* cf. time( ),ctime( ),asctime( ),localtime( ) */
/*----------------------------------------------------------------------------*/
# if defined ( OPTIMIZE )
   # pragma OPTIMIZE ON
   # pragma OPT_LEVEL 2
# endif
/*----------------------------------------------------------------------------*/
# include "../CONFIG.H"
/*----------------------------------------------------------------------------*/
# include "../math/maths.h"
# include "../math/consts.h"
# include "../math/txctyp.h"
# include "../math/CPRODUCT.M"
# include "./types.h"
/*----------------------------------------------------------------------------*/
/* operational constants:                                                     */

# ifndef GSS_STRUCT
   # define GSS_STRUCT 0   /* 1: define struct GAUSS_JRD [may be done elsw.] */
# endif

# if GSS_STRUCT == 0
/*----------------------------------------------------------------------------*/
/* The structure type definition of Gauss-Jordan algorithm functions          */
/* gssjrd(*) [line pivoting] and gssjpv(*) [full pivoting ]                   */
/*----------------------------------------------------------------------------*/
# ifndef GSS_DISP
   # define GSS_DISP  1     /* 1: display if coeff. mtx.gss.m[][] is singular */
# endif
# ifndef GSS_IMPLCT
   # define GSS_IMPLCT 1    /* 1: implicit pivoting  [ normalizing equations ]*/
# endif
# ifndef GSS_STRUCT
   # define GSS_STRUCT 1    /* 1: define struct gaussc gss [may be done elsw.]*/
# endif
# ifndef GSS_LONG_DBL
   # define GSS_LONG_DBL 0  /* 1: long double prexcision */
# endif
# ifndef GSS_MAXRNK
   # define GSS_MAXRNK 10   /* maximum rank of matrix M */
# endif
# ifndef GSS_SINGLR 
   # define GSS_SINGLR (1.e-277) /* bound for singularity check */
# endif
/*----------------------------------------------------------------------------*/
typedef struct
{
   signed char
      rtn;

   char 
      opt;

   short
      rank, /* rank of matrix M = ( mr[][]+j*mi[][] ) */
      neqs; /* number of linear equations to be solved [ simultaneously ] */
            /* - only used in function gssjpv(*) */

   # if GSS_LONG_DBL  == 1
      long double
   # else
      double
   # endif

         dtr,                         /* determinant  ( real  part )          */
         dti,                         /* "            ( imag. part )          */
         mr[GSS_MAXRNK][2*GSS_MAXRNK],/* r = real part of matrix              */
         mi[GSS_MAXRNK][2*GSS_MAXRNK],/* i = imag."    "  "                   */
         zr[GSS_MAXRNK][GSS_MAXRNK],
         zi[GSS_MAXRNK][GSS_MAXRNK];

}  GAUSS_JRD;
# endif /* GSS_STRUCT != 0 */
/*----------------------------------------------------------------------------*/
static GAUSS_JRD gss = {null};
/*============================================================================*/

GAUSS_JRD *\
gssjpv( GAUSS_JRD *gjp )
{
/* allusions: - */

/* declarations: */

   static GAUSS_JRD 
     *rpt = NULL;

# if GSS_LONG_DBL == 1
   static long double
# else
   static double  
# endif
      uu = ZERO,
      vv = ZERO,
      rr = ZERO,
      ss = ZERO,
      sq = ZERO,
      mx = ZERO,
      pr = ZERO,
      pi = ZERO,
      dtr = ZERO,
      dti = ZERO;

   static short 
      ii = null,
      jj = null,
      kk = null,
      mm = null,
      nn = null,
      lin = null,
      col = null,
      rank = null;

   static short
      ipv[GSS_MAXRNK],
      ixl[GSS_MAXRNK],
      ixc[GSS_MAXRNK];
/*----------------------------------------------------------------------------*/
/* options:                                                                   */
/*                     compute
   opt = 'i'[inversion]   -->   matrix inversion
   opt = 'e'[equation]    -->   linear equation  
   opt = 'd'[determinant] -->   determinant
*/
/*...........................................................................*/
/* initialize: */

   if ( gjp == NULL )
   {
      rpt = &gss;
      ( rpt->rtn ) = ONE;
      return rpt;
   };
/*...........................................................................*/
   rpt = gjp;
   rank = ( gjp->rank );
/*...........................................................................*/
   if ( GSS_MAXRNK < rank )
   {
      printf( "\n\n Error message from funcion %s:", __func__ );
      printf( "\n\n Rank of matrix ( gjp->m[][] ) too high !!!" );
      printf( "\n [ The maximum rank is %d = macro GSS_MAXRNK", GSS_MAXRNK );
      printf( "\n   - change macro in compliance "
         "with memory resources. ]\n " );

      ( rpt->rtn ) = null;
      return rpt;
   };
/*............................................................................*/
/* the options: */

   switch( gjp->opt )
   {
     case 'i': /* option: matrix inversion */
     case 'I':
                                     
      mm = ( gjp->rank );

      for ( ii=null; ii<rank; ii++ )
      {
         for ( jj=null; jj<rank ; jj++ ) /* write unit matrix */
         {   
            kk = rank + jj;

# if GSS_LONG_DBL == 1
            ( rpt->mr[ii][kk] ) = ( long double ) ( ii == jj );
# else
            ( rpt->mr[ii][kk] ) = ( double ) ( ii == jj );
# endif
            ( rpt->mi[ii][kk] ) = ZERO;
         };
      };
      break;

     case 'e': /* option: linear equation */
     case 'E':

      mm = ( gjp->neqs );

      if ( mm <= null )
         mm = ONE;

      if ( GSS_MAXRNK < mm )
      {
         printf( "\n\n Error message from funcion %s:", __func__ );
         printf( "\n\n Too many equations !!!" );
         printf( "\n [ The maximum number is %d = macro GSS_MAXRNK", 
            GSS_MAXRNK );
         printf( "\n   - change macro in compliance "
            "with memory resources. ]\n " );
         return NULL;
      };
      break;                                    

     case 'd':                                     /* option: determinant     */
     case 'D': 

      mm = null;

      break; 

     default:
      printf( "\n\n Error message from function %s:", __func__ );
      printf( "\n\n Unknown or unspecified option on calling function !" );
      printf( "\n The legal options are [ case insensitive ]:"
         "\n opt = 'e' - linear equation"
         "\n opt = 'i' - matrix inversion"
         "\n opt = 'd' - determinant."
         "\n [ Please specify option on function call.]\n " );

      return NULL; 
   };
/*............................................................................*/
   nn = rank + mm;

   dtr = 1.;
   dti = ZERO;
/*............................................................................*/
/* store coefficient matrix [for recovery in the case of multiple equations]: */

   ii = null;
   while( ii < rank )
   {
      jj = null;
      while( jj < nn )
      {
         ( rpt->zr[ii][jj] ) = ( rpt->mr[ii][jj] );
         ( rpt->zi[ii][jj] ) = ( rpt->mi[ii][jj] );
         jj++ ;
      };
      ii++ ;
   };
/*............................................................................*/
# if GSS_IMPLCT == 1
/* normalize equations:                                                       */

   while( ii < rank )
   {
      col = - ONE;
      mx = ZERO;
      jj = null;
      while( jj < rank )
      {
         uu = ( rpt->mr[ii][jj] );
         vv = ( rpt->mi[ii][jj] );
         sq = uu*uu + vv*vv; 
         if( mx < sq )
         { 
            col = jj;
            mx = sq;
         };
         jj++ ;
      };
      if( mx < GSS_SINGLR )
      {
         goto restore;
      }
      else
      {
         uu = ( rpt->mr[ii][col] );
         vv = ( rpt->mi[ii][col] );

         CPRODUCT( uu, vv, dtr, dti, rr, ss );
         dtr = rr;
         dti = ss;

         ( rpt->mr[ii][col] ) = 1.;
         ( rpt->mi[ii][col] ) = 0.; 

         jj = null;
         while( jj < nn )
         {
            if( jj != col )
            {
               CQUOTIENT(( rpt->mr[ii][jj] ), ( rpt->mi[ii][jj] ), \
                  uu, vv, rr, ss );
               ( rpt->mr[ii][jj] ) = rr;
               ( rpt->mi[ii][jj] ) = ss;
            };
            jj++ ;
         };
      };/* end if */
      ii++ ;
   };
# endif 
/*............................................................................*/
/* pivote: */

   jj = null;
   while( jj < rank )
   {
      ipv[jj] = - ONE;
      jj++ ;
   };

   ii = null;
   while( ii < rank )
   {
      mx = ZERO;
      jj = null;
      while( jj < rank )
      {
         if ( ipv[jj] != null ) 
         {
            kk = null;
            while( kk < rank )
            {
               if( ipv[kk] == - ONE )
               {
                  uu = ( rpt->mr[jj][kk] );
                  vv = ( rpt->mi[jj][kk] );

                  sq = uu*uu + vv*vv; 
                  if ( mx < sq )
                  {
                     mx = sq;
                     lin = jj;
                     col = kk; 
                  };
               }
               else if ( null < ipv[kk] ) /* singular matrix */
               {
                  mx = ZERO;
                  goto restore;
               }; 

               kk++ ;
            }; /* end while( kk < rank ) */  
         }; /* end if ipv[jj] != - ONE */    
         jj++ ;
      }; /* end while( jj < rank ) */

      ipv[col]++ ;
/*............................................................................*/
/* Having got the pivot element, lines may now be interchanged, if necessary, */
/* to put the pivot element on the diagonal. The columns are not physically   */
/* interchanged, only relabeled: ixc[I], the column of the Ith pivot element, */
/* is the Ith column that is reduced, while ixl[I] is the line in which that  */
/* pivot element was originally located. If ixl[I] != ixc[I] than there is an */
/* implied column interchange. With this form of bookkeeping, the solution    */
/* ( rpt->zr[][], rpt->zi[][] ) will end up in correct order, and the inverse */
/* matrix will be scrambled by columns.                                       */

      if ( lin != col )
      {
         dtr = - dtr;
         dti = - dti;

         jj = null;  
         while( jj < nn )
         {
            uu = ( rpt->mr[lin][jj] );
            vv = ( rpt->mi[lin][jj] );
            ( rpt->mr[lin][jj] ) = ( rpt->mr[col][jj] );
            ( rpt->mi[lin][jj] ) = ( rpt->mi[col][jj] );
            ( rpt->mr[col][jj] ) = uu;
            ( rpt->mi[col][jj] ) = vv;
            jj++ ;
         }; /* end while ... */
      };/* end if lin != col */

      ixl[ii] = lin; /* now ready to divide the pivot element, */
      ixc[ii] = col; /* located at [lin][col]                  */

      uu = ( rpt->mr[col][col] );
      vv = ( rpt->mi[col][col] );
      sq = uu*uu + vv*vv; 

      if ( sq < GSS_SINGLR ) /* singular matrix */ 
      {
         mx = ZERO;
         goto restore;
      };

      CPRODUCT( uu, vv, dtr, dti, rr, ss );

      dtr = rr;
      dti = ss;

      pr =   uu / sq;
      pi = - vv / sq; 

      ( rpt->mr[col][col] ) = 1.;
      ( rpt->mi[col][col] ) = ZERO;

      jj = null; 
      while( jj < nn )
      {
         CPRODUCT(( rpt->mr[col][jj] ), ( rpt->mi[col][jj] ), \
            pr, pi, uu, vv );
         ( rpt->mr[col][jj] ) = uu;
         ( rpt->mi[col][jj] ) = vv;
         jj++ ; 
      }; /* end while ... */

      jj = null;
      while( jj < rank )
      {
         if( jj != col )
         {
            uu = ( rpt->mr[jj][col] );
            vv = ( rpt->mi[jj][col] );

            kk = null;  
            while( kk < nn )
            {
               CPRODUCT( uu, vv, ( rpt->mr[col][kk] ), ( rpt->mi[col][kk] ), \
                  rr, ss );
               ( rpt->mr[jj][kk] ) -= rr;
               ( rpt->mi[jj][kk] ) -= ss;
               kk++ ;
            };
         }; /* end if jj != col */
         jj++ ;
      }; /* end while( jj < rank ) */
      ii++ ;
   };/* end while( ii < rank ) [ end of main loop ] */

   ii = rank - ONE;
   while( null <= ii )
   {
      if( ixl[ii] != ixc[ii] )
      {
         jj = null;
         while( jj < rank )
         {
            uu = ( rpt->mr[jj][ixl[ii]] );
            vv = ( rpt->mi[jj][ixl[ii]] );
            ( rpt->mr[jj][ixl[ii]] ) = ( rpt->mr[jj][ixc[ii]] ); 
            ( rpt->mi[jj][ixl[ii]] ) = ( rpt->mi[jj][ixc[ii]] );
            ( rpt->mr[jj][ixc[ii]] ) = uu;
            ( rpt->mi[jj][ixc[ii]] ) = vv;
            jj++ ;
         };
      };/* end if ... */
      ii-- ;
   };  
/*............................................................................*/
/* restore coeficcient matrix m[i][j]  ( i,j = 0,..., rank+mm )               */
/* and write mm solutions into  z[][k]  ( k = 0, ..., mm-1 )                  */

  restore:

   ii = null;
   while( ii < rank )
   {
      jj = null;
      while( jj < rank )
      {
         kk = jj + gjp->rank;
         ( rpt->mr[ii][jj] ) = ( rpt->zr[ii][jj] );
         ( rpt->mi[ii][jj] ) = ( rpt->zi[ii][jj] );
         ( rpt->zr[ii][jj] ) = ( rpt->mr[ii][kk] );
         ( rpt->zi[ii][jj] ) = ( rpt->mi[ii][kk] );
         ( rpt->mr[ii][kk] ) = ( rpt->zr[ii][kk] );
         ( rpt->mi[ii][kk] ) = ( rpt->zi[ii][kk] );
         jj++ ;
      };
      ii++ ;
   };
/*............................................................................*/
   if ( GSS_SINGLR <= mx )
   {
      ( rpt->dtr ) = dtr;
      ( rpt->dti ) = dti;
      ( rpt->rtn ) = ONE;

      return rpt;
   };
/*............................................................................*/
/* singular coeficient matrix:                                                */

   ( rpt->dtr ) = ZERO;
   ( rpt->dti ) = ZERO;

   if ((( gjp->opt ) == 'd' )||(( gjp->opt ) == 'D' ))
   {
      ( rpt->rtn ) = ONE;
      return rpt;
   };

# if GSS_DISP == 1
   fprintf( stderr, "\n\n Message from function %s:\n", __func__ );
   ii = null; do
   {
       fprintf( stderr, "\n Singular coefficient matrix "
          "M = ( gjp->mr[][] + i*gjp->mi[][] ) !!!" );
      ii++ ;
   } while ( ii < THREE );
   fprintf( stderr, "\n\n [ Abnormal end of function %s.]\n", __func__ );
# endif

   ( rpt->rtn ) = null;
   return rpt;
} 
/*============================================================================*/
# if defined ( OPTIMIZE )
   # pragma OPTIMIZE OFF
# endif
/************************ end of function gssjpv(*) ***************************/




# define DO_GRAPHP "graphp(*)"
/*******************************************************************************
*                                                                              *
*   Graphics files creation function graphp(*), HELIOS release 1.0.r4          *
*   [ Heat and Electric Losses In Ordinary wave guiding Systems ]              *
*                                                                              *
*   (C) SHEIN; Bad Aibling, January 2008                   Steffen Hein        *
*                                                                              *
*******************************************************************************/

/*----------------------------------------------------------------------------*/
# ifndef PLOT_FORMAT
   # define PLOT_FORMAT "gnuplot_2D" /*plot file format: "SPLINE" or "gnuplot"*/
# endif
# ifndef GPH_USCALE
   # define GPH_USCALE 0    /* 1: take uniform scales on xy axes */
# endif
# ifndef GPH_CURVES
   # define GPH_CURVES 20   /* maximum number of simultaneously plotted */
# endif                     /* graphics */
# ifndef GPH_POINTS
   # define GPH_POINTS 5000 /* maximum number of graphics points */
# endif
# ifndef GPH_PNTSZE
   # define GPH_PTSIZE 2    /* default point size */
# endif
# ifndef GPH_DATFLE
   # define GPH_DATFLE "dat"
# endif
# ifndef GPH_RNDOFF         /* roundoff to that number of floating pnt digits */
   # define GPH_RNDOFF 5
# endif
# ifndef GPH_MXRNGE
   # define GPH_MXRNGE (9.e+00) /* The max. range of abs.values in gnuplot is */
# endif                         /* interval [1.e-GPH_MXRNGE, 1.e+GPH_MXRNGE]. */
                                /* Beyond this range all values are rescaled. */
/* macros: */

# define GPH_FOPEN( pfx, fle ) \
{ \
   strcpy( pptr, (pfx) ); \
   strcat( pptr, (fle) ); \
 \
   pltfle = fopen( pptr, "w" ); \
 \
   if (( pltfle == null ) \
     &&( null != ( gpt->dsp ))) \
   { \
      fprintf( stderr, "\n\n Message from function %s:", DO_GRAPHP ); \
      fprintf( stderr, "\n\n Error on opening file %s !!!", pptr ); \
      fprintf( stderr, "\n [ overrides and returns to calling program ].\n "); \
      return null; \
   }; \
}
/*--------------------------------------------------------------------------*/
# ifndef HLSTP_GRAPHICS
# define HLSTP_GRAPHICS 1
/* graphics data transfer structure [ used in function graphp(*), e.g.]: */
typedef struct
{
   signed char
      rtn, /* return operation mark: 0: returm with error */
      dsp; /* display operation mark: 1 display some file saving messages */

   char
      name[STS_SIZE],
      text[STS_SIZE];

   char
      file[STS_SIZE],
      curve[GPH_CURVES][STS_SIZE],
      format[SHS_SIZE];

   char
      xunit[SHS_SIZE],
      yunit[SHS_SIZE];

   short
      ptsize,
      nc;

   long
      nn,
      np[GPH_CURVES];

   double
      xmin,
      xmax,
      ymin,
      ymax,
      vct[GPH_CURVES+ONE][GPH_POINTS];

} GRAPHICS;
# endif
/*============================================================================*/

int graphp( GRAPHICS *gpt )
{
/* allusions: */

/* declarations; */

   time_t nseconds = null;
   time_t   *timer = null;

   static short 
      ii = null;

   static long 
      kk = null;

   static double
      x_min   = ZERO,
      y_min   = ZERO,
      x_max   = ZERO,
      y_max   = ZERO,
      x_mean  = ZERO, 
      y_mean  = ZERO,
      x_shift = ZERO,
      y_shift = ZERO,
      xx      = ZERO,
      yy      = ZERO,
      uu      = ZERO,
      vv      = ZERO,
      lx      = ZERO,
      ly      = ZERO,
      rscale  = 1.05;  /* range scaling factor */

   static char
     *timefrm = " created: %.24s ",
     *timeptr,
     *pptr,
     *dptr,
     *prefix;
/*............................................................................*/
# if GPH_USCALE == 1
   static char 
      **endp = null; 
# endif
/*............................................................................*/
/* prototypes: */

   double fabs( double x );
   double pow( double x, double y );
   double log( double x );
   double log10( double x );
   double floor( double x );

   time_t time( time_t *timer );

/* streams: */
       
   FILE 
     *pltfle,
     *datfle;

   char *lotos ( long ll, char digits );

# ifdef GPH_RNDOFF
   double rndoff( double xx, short nn );
# endif
/*----------------------------------------------------------------------------*/
/* memory allocations: */

   pptr    = ( char *) calloc( STS_SIZE, ONE );
   dptr    = ( char *) calloc( STS_SIZE, ONE );
   prefix  = ( char *) calloc( VSS_SIZE, ONE );
   timeptr = ( char *) calloc( STS_SIZE, ONE );

   x_shift = ZERO;
   y_shift = ZERO;

   rscale  = 1.05;
/*............................................................................*/
   if (( gpt->nc ) == null ) /* no curve number transferred */
      ( gpt->nc ) = ONE;
   else if ( GPH_CURVES < ( gpt->nc ))
   {
      fprintf( stderr, "\n\n Error in function %s:", DO_GRAPHP );
      fprintf( stderr, "\n\n Too many [=%d] curves transferred to function !!!",
         ( gpt->nc ));
      fprintf( stderr, "\n [ The maximum number is %ld = macro GPH_CURVES "
         "in function %s.", ( long ) GPH_CURVES, DO_GRAPHP );
      fprintf( stderr, "\n - Change macro only in compliance "
         "with memory resources.]\n" );

      exit( EXIT_FAILURE );
   };
/*............................................................................*/
   if ( null == gpt->ptsize )
      gpt->ptsize = GPH_PTSIZE;
      
   ii = null; do
   {
      if (( gpt->nn ) < ( gpt->np[ii] )) 
         ( gpt->nn ) = ( gpt->np[ii] );
   } while( ++ii < ( gpt->nc ));
   
   ii = null; do
   {
      if (( gpt->np[ii] ) == null ) /* no individual point number transferred */
         ( gpt->np[ii] ) = ( gpt->nn );
   } while( ++ii < ( gpt->nc ));
/*............................................................................*/
   ii = null;
   while ( ii < ( gpt->nc ))
   {
      if ( GPH_POINTS < ( gpt->np[ii] ))
      {
         fprintf( stderr, "\n\n Error in function %s:", DO_GRAPHP );
         fprintf( stderr,
            "\n\n Too many [=%ld] points transferred in array vct[%d][*] !!!",
            ( long )( gpt->np[ii] ), ( ii + ONE ));
         fprintf( stderr, "\n [ The maximum number is %ld = macro GPH_POINTS "
            "in function %s.", (long) GPH_POINTS, DO_GRAPHP );
         fprintf( stderr, "\n - Change macro only in compliance "
            "with memory resources.]\n" );

         exit( EXIT_FAILURE );
      };
      ii++;
   };
/*............................................................................*/
# if GPH_USCALE == 1

   fprintf( stdout, "\n\t\t\t\t\t\t\t\t       ) <- ?" );
   fprintf( stdout, "\r Rescale coordinates ? >----------"
      "-------------------> [ y/n ] >---> (" );
   scanf( "%s", pptr );

   if ( *pptr == 'y' )
   {
     scaling:
      fprintf( stdout, " Please enter scaling factor [ ZERO "
         "< factor ] ....................: " );
      scanf( "%s", pptr );
      rscale = strtod( pptr, endp );

      if ( rscale < SMALL_VAL )
         goto scaling;

      fprintf( stdout, " Enter x-shift [ -1 <= shift <= 1 ] "
         "...............................: ");
      scanf( "%s", pptr );
      x_shift = strtod( pptr, endp );

      fprintf( stdout, " Enter y-shift [ -1 <= shift <= 1 ] "
         "...............................: ");
      scanf( "%s", pptr );
      y_shift = strtod( pptr, endp );
   }; 
# endif
/*............................................................................*/
   ( gpt->xmin ) = 1.e+277;
   ( gpt->ymin ) = 1.e+277;

   ( gpt->xmax ) = -( gpt->xmin );
   ( gpt->ymax ) = -( gpt->ymin );

   kk = null;
   while( kk < ( gpt->nn ))
   {
      xx = ( gpt->vct[null][kk] );

      if (( gpt->xmax ) < xx )
          ( gpt->xmax ) = xx;

      if( xx < ( gpt->xmin ))
        ( gpt->xmin ) = xx;
      kk++;
   };

   ii = null;
   while( ii < ( gpt->nc ))
   {
      kk = null;
      while( kk < ( gpt->np[ii] ))
      {
         yy = ( gpt->vct[ii+ONE][kk] );

         if(( gpt->ymax ) < yy )
            ( gpt->ymax ) = yy;

         if( yy < ( gpt->ymin )) 
           ( gpt->ymin ) = yy;
         kk++;
      };
      ii++;
   };

   xx = (( gpt->xmax ) - ( gpt->xmin ))/2.;
   yy = (( gpt->ymax ) - ( gpt->ymin ))/2.;
/*............................................................................*/
# if GPH_USCALE == 1
   if( xx < yy ) 
      xx = yy;

   yy = xx;
# endif
/*............................................................................*/
   x_mean = (( gpt->xmin ) + ( gpt->xmax ))/2.;
   x_min = x_mean + ( x_shift - rscale )*xx;
   x_max = x_mean + ( x_shift + rscale )*xx;

   y_mean = (( gpt->ymin ) + ( gpt->ymax ))/2.;
   y_min = y_mean + ( y_shift - rscale )*yy;
   y_max = y_mean + ( y_shift + rscale )*yy;
/*............................................................................*/
/* plot system file */

   if ( null == strncmp(( gpt->format ), "gnuplot_2D", THREE ))
   {
/* renormalize absolute x and y values to match interval 
                                           [ 1.e-GPH_MXRNGE, 1.e+GPH_MXRNGE ] */
      xx = fabs( gpt->xmax );

      if ( xx < fabs( gpt->xmin ))
         xx = fabs( gpt->xmin );
	 
      if ( SMALL_VAL < fabs( xx ))
         lx = floor( log10( xx ));
      else
         lx = ZERO;

      if ( fabs( lx ) < GPH_MXRNGE )
      {
         xx = 1.;
         lx = ZERO;
      }
      else
         xx = pow( 10., -lx ); /* = 10^(-lx ) = exp( -lx*log( 10.)) */

      x_min *= xx;
      x_max *= xx;

      yy = fabs( gpt->ymax );

      if ( yy < fabs( gpt->ymin ))
         yy = fabs( gpt->ymin );

      if ( SMALL_VAL < fabs( yy ))
      {
          y_min /= rscale;
          y_max *= rscale;
      };

      if( SMALL_VAL < fabs( yy ))
         ly = floor( log10( yy ));
      else
         ly = ZERO;

      if ( fabs( ly ) < GPH_MXRNGE )
      {
         yy = 1.;
         ly = ZERO;
      }
      else
         yy = pow( 10., -ly );     /* = 10^(-ly ) = exp( -ly*log( 10.)) */

      y_min *= yy;
      y_max *= yy;

      if ( fabs( y_max - y_min ) < SMALL_VAL )
      {
          y_min /= rscale;
          y_max *= rscale;
      };

      strcpy( prefix , "gpl." );

      GPH_FOPEN( prefix, ( gpt->file ));

      if ( null != ( gpt->dsp ))
         fprintf( stdout, "\n opened: gnuplot system file %s", pptr );

      fprintf( pltfle, "set title '%s %s'\n", ( gpt->name ), ( gpt->text ));
      fprintf( pltfle, "set xrange [%.15e:%.15e]\n", x_min, x_max );
      fprintf( pltfle, "set yrange [%.15e:%.15e]\n", y_min, y_max );

      if ( lx == ZERO )
         fprintf( pltfle, "set xlabel '%s'\n", ( gpt->xunit ));
      else
         fprintf( pltfle, "set xlabel '1.E%+d %s'\n", 
         ( short ) lx, ( gpt->xunit ));
      if ( ly == ZERO )
         fprintf( pltfle, "set ylabel '%s'\n", ( gpt->yunit ));
      else
         fprintf( pltfle, "set ylabel '1.E%+d %s'\n",
         ( short ) ly, ( gpt->yunit ));

      fprintf( pltfle, "set grid\n" );
      fprintf( pltfle, "set border\n" );
      fprintf( pltfle, "set pointsize %d\n", gpt->ptsize );
      fprintf( pltfle, "%s %c\n", "plot", 92 );

# ifdef USER_PATH
      fprintf( pltfle, "%s/", USER_PATH );
# endif

      ii = null;
      while ( ii < (( gpt->nc )-ONE ))
      {
         if ( null < strlen( gpt->curve[ii] ))
            strcpy( dptr, ( gpt->curve[ii] ));
         else
         {
            strcpy( dptr, prefix );
            strcat( dptr, ( gpt->file ));
            strcat( dptr, GPH_DATFLE );
            strcat( dptr, lotos( ii, null ));
         };

         fprintf( pltfle, "%c%s%c with linespoints, %c\n", 39, dptr, 39, 92 );
	 ii++;            /* 39 = ASCII semicolon */ 
      };                  /* 92 = ASCII backslash */

      if ( ii < ( gpt->nc ))
      {
         if ( null < strlen( gpt->curve[ii] ))
            strcpy( dptr, ( gpt->curve[ii] ));
         else
         {
            strcpy( dptr, prefix );
            strcat( dptr, ( gpt->file ));
            strcat( dptr, GPH_DATFLE );
            strcat( dptr, lotos( ii, null ));
         };

         fprintf( pltfle, "%c%s%c with linespoints\n", 39, dptr, 39 );
      };

      fprintf( pltfle, "pause -1 '[ hit return to continue ]'\n" );

      nseconds = time( timer );
      timeptr = ctime( &nseconds );

      fprintf( pltfle, "\n# 2D-gnuplot system file '%s'\n", pptr );
      fprintf( pltfle, "# created:%.24s", timeptr );
      fprintf( pltfle, "\n# EOF" );

      fclose( pltfle );

      if ( null != ( gpt->dsp ))
      {
         fprintf( stdout, CLEAR_LINE );
         fprintf( stdout, "\r gnuplot system file %s", pptr );
         fprintf( stdout, timefrm, timeptr );
      };
/*............................................................................*/
/* 2D-plot points file: */ 

      ii = null;
      while ( ii < ( gpt->nc ))
      {
         if ( null < strlen( gpt->curve[ii] ))
            strcpy( dptr, ( gpt->curve[ii] ));
         else
         {
            strcpy( dptr, prefix );
            strcat( dptr, ( gpt->file ));
            strcat( dptr, GPH_DATFLE );
            strcat( dptr, lotos( ii, null ));
         };

         datfle = fopen( dptr, "w" );

         if (( datfle == null )
           &&( null != ( gpt->dsp )))
         {
            fprintf( stderr,
               "\n\n Message from function %s:", DO_GRAPHP );
            fprintf( stderr,
               "\n\n Unknown error on opening plot data file %s !!!", dptr );
            fprintf( stderr,
               "\n [ overrides and returns to calling program ].\n" );
            return null;
         };

         if ( null != ( gpt->dsp ))
            fprintf( stdout,
               "\n opened: gnuplot data file %s ", dptr );

         fprintf( datfle, "# %s", ( gpt->name ));

         if ( lx == ZERO )
            fprintf( datfle, " [x-unit: %s, ", ( gpt->xunit ));
         else
            fprintf( datfle, " [x-unit: 1.E%+d %s, ",
            ( short ) lx, ( gpt->xunit ));

         if ( ly == ZERO )
            fprintf( datfle, "y-unit: %s]\n", ( gpt->yunit ));
         else
            fprintf( datfle, "y-unit: 1.E%+d %s]\n",
            ( short ) ly, ( gpt->yunit ));

         kk=null;
	 while ( kk < ( gpt->np[ii] ))
         {
            uu = xx*( gpt->vct[null][kk] );
            vv = yy*( gpt->vct[ii+ONE][kk] );

# ifdef GPH_RNDOFF
            vv = rndoff( vv, ( GPH_RNDOFF+ONE ));
            fprintf( datfle, "%+.*e %+.*e\n",
               ( int ) GPH_RNDOFF, uu, ( int ) GPH_RNDOFF, vv );
# else
            fprintf( datfle, "%+.15e %+.15e\n", uu, vv );
# endif
            kk++;
         };

         fclose( datfle );

         if ( null != ( gpt->dsp ))
         {
            fprintf( stdout, CLEAR_LINE );
            fprintf( stdout, "\r gnuplot data file %s", dptr );
            fprintf( stdout, timefrm, timeptr );
         };
     
         ( gpt->np[(ii++)] ) = null; /* clear */
      };
   } /* end if ( gpt->format ) == "gnuplot_2D" */
   else if ( null == strncmp(( gpt->format ), "SPLINE", THREE ))
   {
      strcpy( prefix , "SPL." );

      GPH_FOPEN( prefix, ( gpt->file ));

      if ( null != ( gpt->dsp ))
         fprintf( stdout, "\n opened: SPLINE file '%s' ", pptr );

      fprintf( pltfle, "%s\n" , ( gpt->name ));
      fprintf( pltfle, "%s\n" , ( gpt->text ));
      fprintf( pltfle, "%s\n" , ( gpt->xunit ));
      fprintf( pltfle, "%s\n" , ( gpt->yunit ));
      fprintf( pltfle, "%ld\n", ( gpt->nn ));

      ii = null;
      while( ii< ( gpt->nc ))
      {
         kk = null;
         while( kk< ( gpt->np[ii] ))
         {
            uu = ( gpt->vct[kk][null] );
            vv = ( gpt->vct[kk][ii+ONE] );

# ifdef GPH_RNDOFF
            vv = rndoff( vv, ( GPH_RNDOFF+ONE ));
            fprintf( pltfle, "%+.*E   %+.*E\n",
               ( int ) GPH_RNDOFF, uu, ( int ) GPH_RNDOFF, vv );
# else
            fprintf( pltfle, "%+.15E   %+.15E\n", uu, vv );
# endif
            kk++;
         };
         fprintf( pltfle, "\n" );

         ( gpt->np[(ii++)] ) = null; /* clear */
      };

      nseconds = time( timer );
      timeptr = ctime( &nseconds );

      fprintf( pltfle, "\n# SPLINE file '%s'\n", pptr );
      fprintf( pltfle, "# created:%.24s", timeptr );
      fprintf( pltfle, "\n# EOF" );

      fclose( pltfle );

      if ( null != ( gpt->dsp ))
      {
         fprintf( stdout, CLEAR_LINE );
         fprintf( stdout, "\r SPLINE file '%s'", pptr );
         fprintf( stdout, timefrm, timeptr );
      };
   }; /* end if ( gpt->format ) == "SPLINE" */

   return ONE;
}
/*===========================================================================*/
# undef GPH_USCALE
# undef GPH_MXRNGE
# undef GPH_FOPEN
# undef GPH_DATFLE
/*************** end of plot file generation function graphp(*) ***************/




# define DO_METALS "metals(*)"
/*******************************************************************************
*                                                                              *
*   Function metals(*); HELIOS release 1.0.r4                                  *
*   [ Heat and Electric Losses In Ordinary wave guiding Systems ]              *
*                                                                              *
*   This function returns physical parameters for some metals commonly used in *
*   electrical waveguides                                                      *
*                                                                              *
*   (C) SHEIN; Bad Aibling, January 2008                   Steffen Hein        *
*                                                                              *
*******************************************************************************/

/*----------------------------------------------------------------------------*/
# ifndef MET_DEFLT
   # define MET_DEFLT 1
# endif
/*----------------------------------------------------------------------------*/
# ifndef HLS_PARSET
   # define HLS_PARSET 1 /* 1: use secure parameters for some materials */
# endif
/*----------------------------------------------------------------------------*/
# ifndef HLSTP_METALS
/*----------------------------------------------------------------------------*/
/* the metals [parameter] structure type */
# define HLSTP_METALS 1
typedef struct
{
   signed char 
      rtn;

   char
      met[30], /* metal name: "Aluminium", "Copper", ..., e.g. */
      shm[30]; /* sign: "Al", "Cu", ..., e.g.  */
   
   double  /* parameters at 0 DEG Celsius: */
      md,  /* mass density [ Kg/m^3 ] */
      myr, /* relative permeability [ dimensionless ( enters skin effect )] */
      er,  /* electric resistivity [ Ohm*m ] */
      der, /* temperature coefficient of the latter [ 1/K ] */
      hc,  /* heat conductivity [ W/(K*m) ] */
      dhc, /* temperature coefficient of the latter [ 1/K ] */
      cv;  /* heat capacity [ J/(Kg*K) ] */

} METALS;
/*----------------------------------------------------------------------------*/
# endif /* end ifndef HLSTP_METALS */
/*----------------------------------------------------------------------------*/
/*
static METALS met = \
{ 0, 
  { 'S', 'i', 'l', 'v', 'e', 'r' },
  { 'A', 'g' },
  0., 0., 0., 0., 0., 0. \
};
*/
/*============================================================================*/
/* include function body [ METALS *metals( METALS *mpt ) ] */
# include "./metals.h"
/*============================================================================*/
/*************************** end of function metals(*) ************************/





# define DO_DIELCT "dielct(*)"
/*******************************************************************************
*                                                                              *
*   Function dielct(*); HELIOS release 1.0r5                                  *
*   [ Heat and Electric Losses In Ordinary wave guiding Systems ]              *
*                                                                              *
*   Returns physical parameters for some dielectric material commonly used in  *
*   electric waveguides                                                        *
*                                                                              *
*   (C) SHEIN; Bad Aibling, January 2008                   Steffen Hein        *
*                                                                              *
*******************************************************************************/

/*----------------------------------------------------------------------------*/
# ifndef DEL_DEFLT
   # define DEL_DEFLT 1
# endif
/*----------------------------------------------------------------------------*/
# ifndef HLSTP_DIELCT
/*----------------------------------------------------------------------------*/
/* the dielectric materials structure type */
# define HLSTP_DIELCT 1
typedef struct
{
   signed char
      rtn;

   char
      del[30], /* name of dielectric: "Teflon", "Polyurethane", ..., e.g. */
      shd[30]; /* sign: "PTFE", "PU", ..., e.g.  */
   
   double  /* parameters at 0 DEG Celsius: */
      md,  /* mass density [ Kg/m^3 ] */
      epr, /* relative permittivity */
      tge, /* electric loss factor, tangent delta_E */
      myr, /* relative permeability */
      tgm, /* magnetic loss factor; tangent delta_M */
      hc,  /* heat conductivity [ W/(K*m) ] */
      dhc, /* temperature coefficient heat conductivity [ 1/K ] */
      cv,  /* specific heat capacity [ J/(Kg*K) ] */
      ny,  /* cinematic viscosity [ m^2/s - only defined for gases & fluids ]*/
      dny, /* temperature coefficient of the latter [ 1/K ] */
      pr,  /* Prandtl number [ dimensionless - only gases & fluids ] */
      dpr; /* temperature coefficient of the latter [ 1/K ] */

} DIELCT;
/*----------------------------------------------------------------------------*/
# endif
/*----------------------------------------------------------------------------*/
/*
static DIELCT del = \
{ 0, 
  { 'T', 'e', 'f', 'l', 'o', 'n' },
  { 'P', 'T', 'F', 'E' },
  0., 0., 0., 0., 0., 0., 0., 0. \
};
*/
/*============================================================================*/
/* include the function body [ DIELCT *dielct( DIELCT *dpt ) ] */
# include "./dielct.h"
/*============================================================================*/
/*************************** end of function dielct(*) ************************/




# define DO_SOLIDS "solids(*)"
/*******************************************************************************
*                                                                              *
*   Function solids(*); HELIOS release 1.0r5                                  *
*   [ Heat and Electric Losses In Ordinary wave guiding Systems ]              *
*                                                                              *
*   Returns physical parameters for solid materials commonly used in electric  *
*   waveguides                                                                 *
*                                                                              *
*   (C) SHEIN; Bad Aibling, January 2008                   Steffen Hein        *
*                                                                              *
*******************************************************************************/

/*----------------------------------------------------------------------------*/
# ifndef SLD_DEFLT
   # define SLD_DEFLT 1
# endif
/*----------------------------------------------------------------------------*/
# ifndef null
   # define null 0
# endif
# ifndef ZERO
   # define ZERO ( 0.00000 ) 
# endif
/*----------------------------------------------------------------------------*/
# ifndef HLSTP_SOLIDS
/*----------------------------------------------------------------------------*/
/* the solid materials structure type */
# define HLSTP_SOLIDS 1
typedef struct
{
   signed char
      rtn;

   char
      sld[30], /* name of dielectric: "Teflon", "Polyurethane", ..., e.g. */
      shs[30]; /* sign: "PTFE", "PU", ..., e.g.  */
   
   double   /* parameters at 0 DEG Celsius: */
      md,   /* mass density [ Kg/m^3 ] */
      er,   /* electric resistivity [ Ohm*m ] */
      der,  /* temperature coefficient of the latter [ 1/K ] */
      epr,  /* relative permittivity */
      tge,  /* electric loss factor, tangent delta_E */
      myr,  /* relative permeability */
      tgm,  /* magnetic loss factor; tangent delta_M */
      hc,   /* heat conductivity [ W/(K*m) ] */
      dhc,  /* temperature coefficient heat conductivity [ percent/K ] */
      cv;   /* specific heat capacity [ J/(Kg*K) ] */

} SOLIDS;
/*----------------------------------------------------------------------------*/
# endif /* end ifndef HLSTP_SOLIDS */
/*----------------------------------------------------------------------------*/
/*
static SOLIDS sld = \
{ 0, 
  { 'T', 'e', 'f', 'l', 'o', 'n' },
  { 'P', 'T', 'F', 'E' },
  0., 0., 0., 0., 0., 0., 0., 0. \
};
*/
/*============================================================================*/
/* include the function body [ SOLIDS *solids( SOLIDS *spt ) ] */
# include "./solids.h"
/*============================================================================*/
/*************************** end of function solids(*) ************************/




# define DO_FLUIDS "fluids(*)"
/*******************************************************************************
*                                                                              *
*   Function fluids(*); HELIOS release 1.0r5                                  *
*   [ Heat and Electric Losses In Ordinary wave guiding Systems ]              *
*                                                                              *
*   Returns physical parameters of gases and cooling fluids used in electric   *
*   waveguides                                                                 *
*                                                                              *
*   (C) SHEIN; Bad Aibling, January 2008                   Steffen Hein        *
*                                                                              *
*******************************************************************************/

/*----------------------------------------------------------------------------*/
# ifndef null
   # define null 0
# endif
# ifndef ZERO
   # define ZERO ( 0.00000 ) 
# endif
/*----------------------------------------------------------------------------*/
# ifndef HLSTP_FLUIDS
/*----------------------------------------------------------------------------*/
/* the gases and coolant fluids structure type: */
# define HLSTP_FLUIDS 1
typedef struct
{
   signed char
      rtn;

   char
      fld[30], /* name of dielectric: "air", "nitrogen", e.g. */
      shf[30]; /* shorthand [sign]: "AIR", "N2", "SF6",..., e.g.  */
   
   double  /* parameters at 0 DEG Celsius: */
      md,  /* mass density [ Kg/m^3 ] */
      epr, /* relative permittivity [ dimensionless ] */
      myr, /* relative permeability [ dimensionless ] */
      hc,  /* heat conductivity [ W/(K*m) ] */
      dhc, /* temperature coefficient of the latter [ 1/K ] */
      cv,  /* cp, heat capacity [ J/(Kg*K) ] */
      ny,  /* cinematic viscosity [ m^2 / s ] */
      dny, /* temperature coefficient of the latter [ 1/K ] */
      pr,  /* Prandtl number [ dimensionless ] */
      dpr; /* temperature coefficient of the latter [ 1/K ] */

} FLUIDS;
/*----------------------------------------------------------------------------*/
# endif /* end ifndef HLSTP_FLUIDS */
/*----------------------------------------------------------------------------*/
/*
static FLUIDS fld = \
{ 0, 
  { 'a', 'i', 'r' },
  { 'A', 'I', 'R' },
  0., 0., 0., 0., 0., 0., 0., 0. \
};
*/
/*============================================================================*/
/* include the function body [ FLUIDS *fluids( FLUIDS *flp ) ] */
# include "./fluids.h"
/*============================================================================*/
/*************************** end of function fluids(*) ************************/




# define DO_LOSSES "losses(*)"
/*******************************************************************************
*                                                                              *
*   Function losses(*); HELIOS release 1.0r5                                   *
*   [ Heat and Electric Losses In Ordinary wave guiding Systems ]              *
*                                                                              *
*   Returns the dielectric and skin effect losses in a coaxial line, for given *
*   frequency, [ reflectionless ] transmitted CW power, and temperatures of    *
*   the inner and outer conductors.                                            *
*                                                                              *
*   (C) SHEIN; Bad Aibling, January 2008                   Steffen Hein        *
*                                                                              *
*******************************************************************************/

/*----------------------------------------------------------------------------*/
# define PRINTF(SS1,SS2,XX) \
{ \
   printf( "\n %s   % .12e", (SS2), ( double )(XX)); \
   printf( "\n please acknowledge (%s) [ enter any character ]:", (SS1)); \
   scanf( "%s", ptr ); \
}
/*----------------------------------------------------------------------------*/
# include "./DISPLAY.M"
/*----------------------------------------------------------------------------*/
# ifndef EPS_VAC
   # define EPS_VAC ( 8.8541878170e-12 ) /* vac. permittivity [A*sec/(V*m)]   */
# endif
/*............................................................................*/
# ifndef MY_VAC_
   # define MY_VAC_ ( 1.2566370614e-06 ) /* "    permeability [V*sec/(A*m)]   */
# endif
/*----------------------------------------------------------------------------*/
# ifndef LSS_UPDWGT /* [ may be defined in CONFIG.H ] */
   # define LSS_UPDWGT ( 7.70e-01 )
# endif
/*----------------------------------------------------------------------------*/
# ifndef CELSIUS_TO_KELVIN 
   # define CELSIUS_TO_KELVIN ( 273.15 )
# endif
/*----------------------------------------------------------------------------*/
/* Gavitation acceleration [m/s^2] */
# ifndef GRAVIT_ACC
   # define GRAVIT_ACC ( 9.81000e+00 )
# endif
/*============================================================================*/
/* include the function body [ ELECTRIC *losses ( HELIOSSTATE *state ) ]: */
# include "./losses.h"
/*============================================================================*/
/********************** end of function 'losses(*)' **************************/




# define DO_HCNDCT "hcndct(*)"
/*******************************************************************************
*                                                                              *
*   Function hcndct(*); HELIOS, release 1.0r5                                  *
*   [ Heat and Electric Losses In Ordinary wave guiding Structures ]           *
*                                                                              *
*   This function computes and inverts the heat conductance matrix for given   *
*   heat resistances between parallel conductors                               *
*                                                                              *
*   (C) SHEIN; Bad Aibling, January 2008                   Steffen Hein        *
*                                                                              *
*******************************************************************************/

/*----------------------------------------------------------------------------*/
# ifndef HCD_MHCLOG
   # define HCD_MHCLOG 0 /* [0] 1: [don't] print heat current */
# endif                  /* mutual conductances */
# ifndef HCD_MCDMDE
   # define HCD_MCDMDE 0 /* dielectric loss current insertion mode */
# endif	                 /* [ the most stable seems to be 0 ] */
# ifndef HCD_MDDHYD      /* affects only rectangular waveguide inner cooling: */
   # define HCD_MDDHYD 0 /* 0: dhyd = 2*sqrt( F/PI ) [ 1: dhyd = 4*F/S ] */
# endif	                 /* 0 yields slightly higher temperatures [secure] */
/*----------------------------------------------------------------------------*/
# ifndef HCD_STRCHP      /* affects only fixed side temperature, pipe */
   # define HCD_STRCHP ( 5.00e+00 ) /* [ usually defined in CONFIG.H ] */
# endif	                 /* [ impact on alogithm satability for 'short' WG ] */
/*----------------------------------------------------------------------------*/
# ifndef HCD_STRCHM      /* affects only fixed side temperature, mantle */
   # define HCD_STRCHM ( 5.00e+00 ) /* [ usually defined in CONFIG.H ] */
# endif	                 /* [ impact on alogithm satability for 'short' WG ] */
/*----------------------------------------------------------------------------*/
/* macro homogeneity check: */
/* tolerated upper bound of side heat current density to loss ratio: */ 
# include "./HMGCHK.M"
/*----------------------------------------------------------------------------*/
/* macro endothermie check: */
/* tolerated lower bound of side heat current density to loss ratio: */ 
# include "./ENDOTH.M"
/*----------------------------------------------------------------------------*/
/* equivalent waveguide diameter gauge factor */
# ifndef HCD_DGAUGE
/*  # define HCD_DGAUGE ( 0.800 ) *//* Max Pitschi's proposal */
    # define HCD_DGAUGE ( 0.975 )   /* Hein's approximation */
# endif
/*----------------------------------------------------------------------------*/
/* precision in function hcndct(*) */
# ifndef PRECISION
   # define PRECISION ( 1.00e-15 )
# endif
/*----------------------------------------------------------------------------*/
/* very small double in function hcdndct(*): */
# ifndef SMALL_VAL
   # define SMALL_VAL ( 1.0e-277 )
# endif
/*----------------------------------------------------------------------------*/
/* giant double in function hcdndct(*): */
# ifndef GIANT_VAL
   # define GIANT_VAL ( 1.0e+277 )
# endif
/*----------------------------------------------------------------------------*/
/* Vacuum permittivity [A*sec/(V*m)] */
# ifndef EPS_VAC
   # define EPS_VAC ( 8.8541878170e-12 )
# endif
/*----------------------------------------------------------------------------*/
/* Vacuum permeability [V*sec/(A*m)] */
# ifndef MY_VAC_
   # define MY_VAC_ ( 1.2566370614e-06 )
# endif
/*----------------------------------------------------------------------------*/
/* Stefan-Boltzmann constant [W/(K^4*m^2)] */
# ifndef STEFAN_BOLTZ
   # define STEFAN_BOLTZ ( 5.67051e-08 )
# endif
/*----------------------------------------------------------------------------*/
/* Gavitation acceleration [m/s^2] */
# ifndef GRAVIT_ACC
   # define GRAVIT_ACC ( 9.81000e+00 )
# endif
/*----------------------------------------------------------------------------*/
# ifndef CELSIUS_TO_KELVIN
   # define CELSIUS_TO_KELVIN ( 273.15 )
# endif
/*============================================================================*/
/* include the function body [ HCNDCT *hcndct( HELIOSSTATE *state ) ]: */
# include "hcndct.h"
/*============================================================================*/
# undef HCD_MHCLOG
/*********************** end of function hcndct(*)' **************************/




# define DO_KELVIN "kelvin(*)"
/*******************************************************************************
*                                                                              *
*   ANSI C function kelvin(*); HELIOS release 1.0r5                           *
*   [ Heat and Electric Losses In Ordinary wave guiding Systems ]              *
*                                                                              *
*   This function computes and inverts a heat conductance matrix for given     *
*   heat resistances between parallel conductors                               *
*                                                                              *
*   (C) SHEIN; Bad Aibling, January 2008                   Steffen Hein        *
*                                                                              *
*******************************************************************************/

/*----------------------------------------------------------------------------*/
# ifndef KLV_REORDR       /* 1 [-1]: reorder essential heat base nodes with */
   # define KLV_REORDR  0 /* increasing [decreasing] base temperatures */
# endif                   /* 0: follow parameter input order [less secure] */
# ifndef KLV_REFINE       /* 1: refine initial heat base conductances using */
   # define KLV_REFINE  3 /* that number of additional bracketing steps [try */
# endif                   /* at least 1], 0: take coarsely initialized values */
# if KLV_REFINE != 0
# ifndef KLV_ZBRDIV
   # define KLV_ZBRDIV 10 /* subdivisions nn in bracketing funct zbrakl(*) */
# endif
# ifndef KLV_ZBRITR
   # define KLV_ZBRITR  7 /* number of zbrakl(*) iterations */
# endif
# endif
/*----------------------------------------------------------------------------*/
/* debugging options: */
# define KLV_HTCLOG 0 /* [0] 1: [don't] print heat currents */
# define KLV_TMPLOG 0 /* [0] 1: [don't] print temperatures */
# define KLV_MHCLOG 0 /* [0] 1: [don't] print heat current mutual conductances*/
# define KLV_CHCLOG 0 /* [0] 1: [don't] print heat current conductance matrix */
# define KLV_RHCLOG 0 /* [0] 1: [don't] heat current resistanc matrix */
# define KLV_PRTSPC 0 /* [0] 1: [don't] display intermediate special values */

# define PRINTF(SS1,SS2,XX) \
{ \
   printf( "\n %s   % .12e", (SS2), ( double )(XX)); \
   printf( "\n please acknowledge (%s) [ enter any character ]:", (SS1)); \
   scanf( "%s", ptr ); \
}
# define SCANF(SS,XX) \
{ \
   printf( "\n please enter %s: ", (SS)); \
   scanf( "%s", ptr ); \
   (XX) = strtod( ptr, endp ); \
}
/*----------------------------------------------------------------------------*/
# define FREERETURN \
{ \
   free_dvector( xx_, 0, NODES ); \
   free_dvector( yy_, 0, NODES ); \
 \
   free_dmatrix( jcb, 0, NODES, 0, NODES ); \
   goto store_matrices; \
}
/*----------------------------------------------------------------------------*/
/* delta x bound in bisection function rtbis(*): */
# ifndef DXRTBIS
   # define DXRTBIS ( 1.0e-11 )
# endif
/* ground temperature tolerance: */
# ifndef GRTOLRC
   # define GRTOLRC ( 1.0e-05 )
# endif
/* delta x bound in Newton Raphson(*): */
# ifndef DXXNEWR
   # define DXXNEWR ( 1.0e-10 )
# endif
/* delta f bound in Newton Raphson(*): */
# ifndef DFFNEWR
   # define DFFNEWR ( 1.0e-09 )
# endif
/* delta x in Jacobi matrix approximation: */
# ifndef DXFDJAC
   # define DXFDJAC ( 1.0e-11 )
# endif
/* maximum number of iterations in function kelvin(*): */
# ifndef KLV_MAXITR
   # define KLV_MAXITR 1024
# endif
/* precision in function kelvin(*) */
# ifndef PRECISION
   # define PRECISION ( 1.0e-15 )
# endif
# ifndef MHC_LWBOUND
   # define MHC_LWBOUND ( 1.0e-14 )
# endif
# ifndef MHC_UPBOUND
   # define MHC_UPBOUND ( 1.0e+14 )
# endif
/*----------------------------------------------------------------------------*/
# ifndef GIANT_VAL
   # define GIANT_VAL ( 1.0e+277 )
# endif
# ifndef SMALL_VAL
   # define SMALL_VAL ( 1.0e-277 )
# endif
/*----------------------------------------------------------------------------*/
# ifndef CELSIUS_TO_KELVIN 
   # define CELSIUS_TO_KELVIN ( 273.15 )
# endif
/*----------------------------------------------------------------------------*/
# ifndef EPS_VAC
   # define EPS_VAC ( 8.8541878170e-12 ) /* vac. permittivity [A*sec/(V*m)]   */
# endif
/*............................................................................*/
# ifndef MY_VAC_
   # define MY_VAC_ ( 1.2566370614e-06 ) /* "    permeability [V*sec/(A*m)]   */
# endif
/*============================================================================*/
/* include the function body [ THERMDYN *kelvin ( HELIOSSTATE *state ) ]: */
# include "./kelvin.h"
/*============================================================================*/
# undef KLV_ZBRDIV
# undef KLV_REORDR
# undef KLV_MAXITR 

# undef KLV_HTCLOG
# undef KLV_TMPLOG
# undef KLV_MHCLOG
# undef KLV_CHCLOG
# undef KLV_RHCLOG
/********************** end of function 'kelvin(*)' **************************/




# define DO_VALUES "values(*)"
/*******************************************************************************
*                                                                              *
*   Function values(*); HELIOS, release 1.0r5                                  *
*   [ Heat and Electric Losses In Ordinary wave guiding Systems ]              *
*                                                                              *
*   This function generates text and graphics files for results computed with  *
*   program HELIOS                                                             *
*                                                                              *
*   (C) SHEIN; Bad Aibling, January 2008                  Steffen Hein         *
*                                                                              *
*******************************************************************************/

/*----------------------------------------------------------------------------*/
# define RES_KLVTDC 1 /* convert temperatures from KELVIN to DEG_CELSIUS  */
# define RES_CW_PWR 1
# define RES_TEMPRS 1
# define RES_LOSSES 1
# define RES_OPRLOG 1
# define RES_TMPLOG 1
# define RES_CHCLOG 1
# define RES_PEAKTP 1
/*----------------------------------------------------------------------------*/
# ifndef CELSIUS_TO_KELVIN 
   # define CELSIUS_TO_KELVIN ( 273.15 )
# endif
/*----------------------------------------------------------------------------*/
# if (( RES_OPRLOG == 1 ) \
    ||( RES_TMPLOG == 1 ) \
    ||( RES_CHCLOG == 1 ))

    # include "store_val.h"
# endif
/*----------------------------------------------------------------------------*/
# include "values.h"
/*============================================================================*/
# undef RES_CW_PWR
# undef RES_TEMPRS
# undef RES_LOSSES
# undef RES_OPRLOG
# undef RES_TMPLOG
# undef RES_CHCLOG
/************************ end of function 'values(*)' *************************/




# define DO_INTLZE "intlze(*)"
/*******************************************************************************
*                                                                              *
*   Function initlze(*); HELIOS, release 1.0r5                                 *
*   [ Heat and Electric Losses In Ordinary wave guiding Structures ]           *
*                                                                              *
*   Initializes waveguide temperatures for the transferred ( given ) radio     *
*   frequency and CW power.                                                    *
*   Also, determines the cylinder structure of the waveguide system [ which    *
*   is essentially the structure of the heat conductance matrix of the system, *
*   iterativeliy updated in function hlswrk(*) ].                              * 
*                                                                              *
*   (C) SHEIN; Bad Aibling, January 2008                   Steffen Hein        *
*                                                                              *
*******************************************************************************/
# define INI_ADDLSS 0 /* 1: add dielectric contributions to lssfm, lssfp */
/*----------------------------------------------------------------------------*/
# include "./WRKMCS.M"
/*----------------------------------------------------------------------------*/
# ifndef CELSIUS_TO_KELVIN 
   # define CELSIUS_TO_KELVIN ( 273.15 )
# endif
/*----------------------------------------------------------------------------*/
/* number of initial trials: on error, first scale [ power or temperature ] */
/* up by 2^INI_INITSC, then scale down by 2^(-INI_INITSC) */
# ifndef INI_INITRS
   # define INI_INITRS 128
# endif
/*----------------------------------------------------------------------------*/
# ifndef INI_REPEAT
   # define INI_REPEAT 32
# endif
/*----------------------------------------------------------------------------*/
# ifndef INI_INITSC     
   # define INI_INITSC 1
# endif
/*----------------------------------------------------------------------------*/
/* maximum number of iterations in fixed side temperature mode */
# ifndef INI_MXITSD 
   # define INI_MXITSD 64
# endif
/*----------------------------------------------------------------------------*/
/* temperature deviation bound, outer loop: */
# ifndef INI_DTEXLP
   # define INI_DTEXLP ( 1.0e-09 )
# endif
/*----------------------------------------------------------------------------*/
/* temperature deviation bound, inner loop: */
# ifndef INI_DTINLP
   # define INI_DTINLP ( 1.0e-10 )
# endif
/*----------------------------------------------------------------------------*/
/* neglected mantle temperature difference: */
# ifndef INI_DTMNGL
   # define INI_DTMNGL ( 1.00e-03 )
# endif
/*----------------------------------------------------------------------------*/
/* smooth temperature updates: */
# ifndef INI_UPDWGT
   # define INI_UPDWGT ( 5.25e-01 )
# endif
/*----------------------------------------------------------------------------*/
/* order of normalized temperature maximum in waveguide: */
# ifndef INI_TRNORD
   # define INI_TRNORD  100
# endif
/*----------------------------------------------------------------------------*/
/* cw power updating stability exponents: */
# ifndef INI_UDEXP0
   # define INI_UDEXP0 ( 1.0e+00 )
# endif
# ifndef INI_UDEXP1
   # define INI_UDEXP1 ( 8.3e-01 )
# endif
# ifndef INI_UDEXP2
   # define INI_UDEXP2 ( 8.3e-01 )
# endif
/*----------------------------------------------------------------------------*/
/* ground temperature tolerance: */
# ifndef GRTOLRC
   # define GRTOLRC ( 1.0e-08 )
# endif
/*============================================================================*/
/* the function body [ CODISSTATE *initlze( CODISSTATE *state ) ]: */
# include "./intlze.h" 
/*============================================================================*/
# undef INI_MAXITR
# undef INI_DTINLP
# undef INI_DTMNGL
# undef INI_DTEXLP
# undef INI_INITRS
# undef INI_INITSC
# undef INI_TRNORD
# undef INI_UDEXP0
# undef INI_UDEXP1
# undef INI_UDEXP2
/************************* end of function intlze(*) **************************/




# define DO_HLSWRK "hlswrk(*)"
/*******************************************************************************
*                                                                              *
*   Function hlswrk(*); HELIOS, release 1.0r5                                  *
*   [ Heat and Electric Losses In Ordinary wave guiding Structures ]           *
*                                                                              *
*   Returns waveguide temperatures for given transferred radio frequeny and    *
*   CW power.                                                                  *
*                                                                              *
*   (C) SHEIN; Bad Aibling, January 2008                   Steffen Hein        *
*                                                                              *
*******************************************************************************/

/*----------------------------------------------------------------------------*/
# define WRK_USENRM 2 /* n: tolerances measured in l_n norm [ n=1,2 ] */
/*----------------------------------------------------------------------------*/
# define WRK_TMPRSE 1 /* n: linear/exponential temperature rise [ n=1,2] */
/*----------------------------------------------------------------------------*/
# define WRK_OPRLOG 1
# define WRK_TMPLOG 1
# define WRK_CHCLOG 1
/*----------------------------------------------------------------------------*/
# define WRK_CWPPLT 1
# define WRK_TMPPLT 1
# define WRK_LSSPLT 1
/*----------------------------------------------------------------------------*/
# ifndef CELSIUS_TO_KELVIN 
   # define CELSIUS_TO_KELVIN ( 273.15 )
# endif
/*----------------------------------------------------------------------------*/
/* maximum number of iterations per loop: */
# ifndef WRK_MAXITR 
   # define WRK_MAXITR 4096
# endif
/*----------------------------------------------------------------------------*/
/* maximum number of iterations in fixed side temperature mode */
# ifndef WRK_MXITSD 
   # define WRK_MXITSD 64
# endif
/*----------------------------------------------------------------------------*/
/* temperature deviation bound, outer loop: */
# ifndef WRK_DTEXLP
   # define WRK_DTEXLP ( 1.0e-09 )
# endif
/*----------------------------------------------------------------------------*/
/* temperature deviation bound, inner loop: */
# ifndef WRK_DTINLP
   # define WRK_DTINLP ( 1.0e-10 )
# endif
/*============================================================================*/
/* the function body [ CODISSTATE *hlswrk( CODISSTATE *state ) ]: */
# include "./hlswrk.h" 
/*============================================================================*/
# undef WRK_OPRLOG
# undef WRK_TMPLOG
# undef WRK_CHCLOG
/*----------------------------------------------------------------------------*/
# undef WRK_CWPPLT
# undef WRK_TMPPLT
# undef WRK_LSSPLT
/*----------------------------------------------------------------------------*/
# undef WRK_MAXITR
# undef WRK_DTINLP
# undef WRK_DTEXLP
# undef WRK_INITMD
# undef WRK_USENRM
# undef WRK_DTEXLP
# undef WRK_DTINLP
/************************* end of function hlswrk(*) **************************/
/*
   Es ist gleichermassen toedlich fuer den Geist, ein System zu haben - und
   keines zu haben. Daher muss man sich dazu entschliessen, beides miteinander
   zu verbinden.
                                                          Friedrich Schlegel
*/
