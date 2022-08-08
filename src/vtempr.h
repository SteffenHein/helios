/* [ file: vtempr.h] */
# define DO_VTEMPR "vtempr(*)"
/*******************************************************************************
*                                                                              *
*   Function vtempr(*); HELIOS release v1.0r1                                  *
*   [ Heat and Electric Losses In Ordinary wave guiding Structures ]           *
*                                                                              *
*   Given a number <nodes> of heat currents hcrr[ii] ( 0<=ii<nodes ),          *
*   and also given mutual heat conductances between these nodes                *
*                                                                              *
*           mhc[ii][jj]     ( 0 <= ii < nodes, 0 <= jj < nodes )               *
*                                                                              *
*   and non-zero conductances from some - or all of the - nodes to associated  *
*   basis nodes bn, each of which is paired with at most one node ii[bp]       *
*                                                                              *
*           mhc[ii[bn]][bn] ( nodes <= bn < mrank <= 2*nodes )                 *
*                                                                              *
*   and is in thermal contact to a common heat reservoir at ground temperature *
*   tgrnd [ usually tgrnd = ZERO ] via finite heat conductances                *
*                                                                              *
*           mhc[[bn][bn]    ( nodes <= bp < rank <= 2*nodes ),                 *
*                                                                              *
*   this program computes the node temperatures                                *
*                                                                              *
*           temp[ii]       ( 0 <= ii < rank )                                  *
*                                                                              *
*   obtained by imposing those heat currents hcrr[ii] to the nodes ii.         *
*                                                                              *
*   The heat conductances are transferred to the function as                   *
*                                                                              *
*          mhc[bn][bn] = xx[bn-nodes]  ( nodes <= bp < rank <= 2*nodes )       *
*                                                                              *
*   and the deviations of the node temperatures temp[bn] from the goal         *
*   temperatures tgoal[bn-nodes] are returned as                               *
*                                                                              *
*          ff[bn-nodes] = temp[bn] - tgoal[bn-nodes]                           *
*                                                                              *
*   (C) SHEIN; Munich, December 2021                       Steffen Hein        *
*   [ Update: December 17, 2021 ]                       <contact@sfenx.de>     *
*                                                                              *
*******************************************************************************/
# ifndef MXFRNK
   # define MXFRNK NODES /* the maximum number of function arguments */
# endif
# ifndef GIANT_VAL
   # define GIANT_VAL ( 1.0e+301 )
# endif
/*----------------------------------------------------------------------------*/
static char
   errptr[STS_SIZE] = {null};

static short
   actbn = null,
   nodes = null,
   mrank = null;

static double
   tgrnd= ZERO;

static double
   hcrr[NODES] = {ZERO},
   tgoal[NODES] = {ZERO},
   temp[(2*NODES)] = {ZERO};

static double
   mhc[(2*NODES)][(2*NODES)] = {{ZERO}},
   chc[(2*NODES)][(2*NODES)] = {{ZERO}},
   rhc[(2*NODES)][(2*NODES)] = {{ZERO}};

static double
   xb1[KLV_ZBRDIV+ONE] = {ZERO},
   xb2[KLV_ZBRDIV+ONE] = {ZERO};
/*============================================================================*/

void vtempr ( double *xx, double *ff, short rk )
{
/* 
   extern double
      tfix,
      hcrr[],
      temp[],
      mhc[][],
      chc[][];

   extern short
      mrank,
      actbn,
      nodes;
*/
   static FILE
     *errfle;

   static short
      ii = null,
     dii = null,
      jj = null,
      kk = null;

   static GAUSS_JRD
     *gsp;

   GAUSS_JRD
     *gssjpv( GAUSS_JRD *gjp );
/*----------------------------------------------------------------------------*/
/* initialize GAUSS_JRD *gsp: */
/*............................................................................*/
   gsp = gssjpv( NULL );    /* Gauss-Jordan initialization */
/*........................*/

   if ( rk < null )
   {
      errfle = fopen( errptr, "a+" );

      fprintf( errfle,
         "\nError message from function %s:", __func__ );
      fprintf( errfle,
         "\nIllegal negative dimension !\n" );

      fclose( errfle );

      fprintf( stderr,
         "\n Error message from function %s:", __func__ );
      fprintf( stderr,
         "\n Illegal negative dimension !\n" );

      exit( EXIT_FAILURE );
   }
   else if ( MXFRNK <= rk )
   {
      errfle = fopen( errptr, "a+" );

      fprintf( errfle,
         "\nError message from function %s:", __func__ );
      fprintf( errfle,
         "\nToo many arguments transferred !" );
      fprintf( errfle,
         "\n[ The number exceeds maximum %d = macro MXFRNK.\n ", MXFRNK );
      fprintf( errfle,
         "\n- Eventually change macro in compliance with" );
      fprintf( errfle,
         "\n  memory ressources.]\n" );

      fclose( errfle );

      fprintf( stderr,
         "\n Error message from function %s:", __func__ );
      fprintf( stderr,
         "\n Too many arguments transferred !" );
      fprintf( stderr,
         "\n [ The number exceeds maximum %d = macro MXFRNK.\n ", MXFRNK );
      fprintf( stderr,
         "\n - Eventually change macro in compliance with" );
      fprintf( stderr,
         "\n   memory ressources.]\n" );

      exit( EXIT_FAILURE );
   }
   else if ( rk != ( mrank - nodes ))
   {
      errfle = fopen( errptr, "a+" );

      fprintf( errfle,
         "\nError message from function %s:", __func__ );
      fprintf( errfle,
         "\nInconsistent number N of variables transferred !" );
      fprintf( errfle,
         "\n[ required: N = mrank - nodes" );
      fprintf( errfle,
         "\n- Please verify and correct.]\n" );

      fclose( errfle );

      fprintf( stderr,
         "\n Error message from function %s:", __func__ );
      fprintf( stderr,
         "\n Inconsistent number N of variables transferred !" );
      fprintf( stderr,
         "\n [ required: N = mrank - nodes" );
      fprintf( stderr,
         "\n - Please verify and correct.]\n" );

      exit( EXIT_FAILURE );
   };

   kk = -ONE;
   while(( ++kk ) < rk )
   {
      if ( fabs( xx[kk] ) < ZERO )
      {
         errfle = fopen( errptr, "a+" );

         fprintf( errfle,
            "\nError message from function %s:", __func__ );
         fprintf( errfle,
            "\nIllegal parameter input !" );
         fprintf( errfle,
            "\n[ Non-positive heat conductances transferred" );
         fprintf( errfle,
            "\n- function requires positive arguments.]\n" );

         fclose( errfle );

         fprintf( stderr,
            "\n Error message from function %s:", __func__ );
         fprintf( stderr,
            "\n Illegal parameter input !" );
         fprintf( stderr,
            "\n [ Non-positive heat conductances transferred" );
         fprintf( stderr,
            "\n - function requires positive arguments.]\n" );

         exit( EXIT_FAILURE );
      };
   };
/*............................................................................*/
/* build heat conductance matrices: */

   ii = null; do
   {
      if ( nodes <= ii )
      {
         dii = ii-nodes;
         mhc[ii][ii] = xx[dii];
      };

      jj = null; do
      {
         chc[ii][jj] = ZERO;
      } while (( ++jj ) < mrank );
   } while (( ++ii ) < mrank );

   ii = null; do
   {
      jj = -ONE;
      while (( ++jj ) < ii )
      {
         chc[ii][ii] += mhc[ii][jj];
         chc[ii][jj] -= mhc[ii][jj];
      };
      chc[ii][ii] += mhc[ii][jj]; /* ii == jj */
      while (( ++jj ) < mrank )
      {
         chc[ii][ii] += mhc[ii][jj];
         chc[ii][jj] -= mhc[ii][jj];
      };
   } while (( ++ii ) < mrank );
/*............................................................................*/
/* copy updated heat conductance matrix into structure of type GAUSS_JRD: */

   ii = null; do
   {
      jj = null; do
      {
         ( gsp->mr[ii][jj] ) = chc[ii][jj];
         ( gsp->mi[ii][jj] ) = ZERO;
      } while (( ++jj ) < mrank );
   } while (( ++ii ) < mrank );

   ( gsp->rank ) = mrank;
   ( gsp->opt ) = 'i'; /* option: matrix 'i'nversion */

/* compute updated resistance matrix by inverting conductance matrix: */
/*............................................................................*/
   gsp = gssjpv( gsp );    /* Gauss-Jordan elimination */
/*.......................*/
/* read updated heat resistance matrix from structure of type GAUSS_JRD */
/* and compute updated temperatures: */

/* singular matrix: */

   if (( gsp->rtn ) == null )
   {
      *ff = HUGE_VALF;
      return;
   };

   ii = null; do
   {
      temp[ii] = tgrnd;
      jj = null; do
      {
         rhc[ii][jj] = ( gsp->zr[ii][jj] );
         temp[ii] += ( rhc[ii][jj] * hcrr[jj] );
      } while(( ++jj ) < nodes );
      while( jj < mrank )
      {
         rhc[ii][jj] = ( gsp->zr[ii][jj] );
         jj++;
      };
   } while(( ++ii ) < mrank );
/*............................................................................*/
   ii = nodes;
   while ( ii < mrank )
   {
      dii = ii-nodes;
      ff[dii] = ( temp[( ii++ )] - tgoal[dii] );
   };
/*............................................................................*/
   return;
}
/*============================================================================*/
# undef DO_VTEMPR
/************************* end of function vtempr(*) **************************/





# define DO_STEMPR "stempr(*)"
/*******************************************************************************
*                                                                              *
*  ANSI C function stempr(*)                                                   *
*                                                                              *
*  Given a number <nodes> of heat currents hcrr[ii] ( 0<=ii<nodes ), and also  *
*  given mutual heat conductances between these nodes                          *
*                                                                              *
*           mhc[ii][jj]     ( 0 <= ii < nodes, 0 <= jj < nodes )               *
*                                                                              *
*  and non-zero conductances from some - or all of the - nodes to associated   *
*  basis nodes bn, each of which is paired with at most one node ii[bp]        *
*                                                                              *
*           mhc[ii[bn]][bn] ( nodes <= bn < mrank <= 2*nodes )                 *
*                                                                              *
*  and is in thermal contact to a common heat reservoir at ground temperature  *
*  tgrnd [ usually tgrnd = ZERO ] via finite heat conductances                 *
*                                                                              *
*           mhc[[bn][bn]    ( nodes <= bp < rank <= 2*nodes ),                 *
*                                                                              *
*  this program computes the node temperatures                                 *
*                                                                              *
*           temp[ii]       ( 0 <= ii < rank )                                  *
*                                                                              *
*  obtained by imposing those heat currents hcrr[ii] to the nodes ii.          *
*                                                                              *
*  One heat conductance, pertinent to an actual index act_bn, is transferred   *
*  to the function as mhc[act_bn][act_bn] = xx, and the deviation of the       *
*  pertinent node temperature temp[act_bn] from a goal temperature tgoal[dnn]  *
*  is retured as the function value                                            *
*                                                                              *
*            yy = stempr( xx ) = temp[actbn] - tgoal[dnn]                      *
*                                                                              *
*  (C) SHEIN, Bad Aibling, July 2002                       Steffen Hein        *
*  [ Update: 21 November 2016 ]                     <contact@steffen-hein.org> *
*                                                                              *
*******************************************************************************/

/*============================================================================*/

double stempr( double xx )
{
   static short
      ii = null,
      dii = null,
      drank = null;

   static double
      yy = ZERO,
     *xx_,
     *yy_;

   void 
      vtempr( double *xx, double *ff, short dimension );

   double
     *dvector( long nl, long nh );

   void
      free_dvector( double *dv, long nl, long nh );
/*----------------------------------------------------------------------------*/
   drank = mrank - nodes;

   xx_ = dvector( 0, drank );
   yy_ = dvector( 0, drank );

   ii = nodes-ONE;
   while (( ++ii ) < mrank )
   {
      dii = ii - nodes;

      if ( ii == actbn )
         xx_[dii] = xx;
      else
         xx_[dii] = mhc[ii][ii];
   };

/* now call the vector valued function: */
/*............................................................................*/
   vtempr ( xx_, yy_, drank );    /*                                     */
/*..............................*/

   if ( GIANT_VAL < *yy_  )
      yy = HUGE_VALF;
   else
      yy = yy_[actbn-nodes];

   free_dvector( xx_, 0, drank );
   free_dvector( yy_, 0, drank );
/*............................................................................*/
   return yy;
}
/*============================================================================*/
# undef DO_STEMPR
/************************* end of function stempr(*) **************************/
