/* [ file: shutil.c ] */
/*******************************************************************************
*                                                                              *
*   Mathematical utilities shutil.c; HELIOS, release v1.0r1                    *
*   [ Heat and Electric Losses In Ordinary wave guiding Structures ]           *
*                                                                              *
*   Almost all routines collected in this file are [ modified ] versions of    *
*   functions and programs of the nrutil.c listing published in the book of    *
*   William H. Press, Saul A. Teukolski, William T. Vetterling, and Brian P.   *
*   Flannery: NUMERICAL RECIPES IN C, The Art of Scientific Computing, 2nd Ed, *
*   Cambridge University Press, 1999.                                          *
*   Of course, all modifications and adaptations [ to our personal needs or    *
*   taste ] are in our own responsibility an no warrenty is given for any      *
*   further application.                                                       *
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
/*----------------------------------------------------------------------------*/
# include "../math/maths.h"
# include "../math/consts.h"
/*----------------------------------------------------------------------------*/
# define DO_DVECTOR "dvector(*)"
/*******************************************************************************
*                                                                              *
*   ANSI C function dvector(*)                                                 *
*   Allocates a double vector with subscript range dv[nl...nh]                 *
*                                                                              *
*   Modified version of dvector(*), Numerical Recipes in C, W.H.Press et al.,  *
*   Cambridge University Press 1999.                                           *
*                                                              Steffen Hein    *
*   [ Update: 19 October 2005 ]                            <s.hein@spinner.de> *
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
      fprintf( stderr,
         "\n Warning from function %s:", DO_DVECTOR );
      fprintf( stderr,
         "\n Non ANSI compliant vector allocation !" );
      fprintf( stderr,
         "\n [ NR_END = %ld < %ld = nl ( first allocated index ),",
         ( long ) NR_END, nl );
      fprintf( stderr,
         "\n   ANSI requires nl <= NR_END. - This can be" );
      fprintf( stderr,
         "\n   remedied setting macro NR_END in %s to %ld.]",
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
/*************** end of functions dvector(*), free_dvector(*) *****************/

# define DO_DMATRIX "dmatrix(*)"
/*******************************************************************************
*                                                                              *
*  ANSI C function dmatrix(*)                                                  *
*  Allocates a double matrix with subscript range m[nrl...nrh][ncl...nch]      *
*                                                                              *
*  Modified version of matrix(*), Numerical Recipes in C, W.H.Press et al.,    *
*  Cambridge University Press 1999.                                            *
*                                                             Steffen Hein     *
*  [ Update: 17 July 2002 ]                                <s.hein@spinner.de> *
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
/*************** end of functions dmatrix(*), free_dmatrix(*) *****************/
/************************** end of file shutil.c ******************************/
