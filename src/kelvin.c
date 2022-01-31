/* [ file: kelvin.c ] */
# define DO_KELVIN "kelvin(*)"
/*******************************************************************************
*                                                                              *
*   Function kelvin(*); HELIOS release v1.0r1                                  *
*   [ Heat and Electric Losses In Ordinary wave guiding Structures ]           *
*                                                                              *
*   This function computes and inverts a heat conductance matrix for given     *
*   heat resistances between parallel conductors                               *
*                                                                              *
*   (C) SHEIN; Munich, December 2021                      Steffen Hein         *
*   [ Update: Janzary 31, 2022 ]                        <contact@sfenx.de>     *
*                                                                              *
*******************************************************************************/
# define _POSIX_SOURCE 1 /* some headers of the POSIX.1 standard will be used */
/*----------------------------------------------------------------------------*/
# include <stdio.h>
# include <stdlib.h>
# include <stddef.h>
# include <stdarg.h>
# include <string.h>
# include <ctype.h>
# include <time.h>            /* cf. time( ),ctime( ),asctime( ),localtime( ) */
/*----------------------------------------------------------------------------*/
# include "../math/maths.h"
# include "../math/consts.h"
/*----------------------------------------------------------------------------*/
/* Edit and customize this general configuration header: */
# include "../CONFIG.H"
/*----------------------------------------------------------------------------*/
# ifndef KLV_REORDR       /* 1 [-1]: reorder essential heat base nodes with */
   # define KLV_REORDR  0 /* increasing [decreasing] base temperatures */
# endif                   /* 0: follow parameter input order [less secure] */
# ifndef KLV_REFINE       /* 1: refine initial heat base conductances using */
   # define KLV_REFINE  3 /* that number of additional bracketing steps [try */
# endif                   /* at least 1], 0: take coarsely initialized values */
/*----------------------------------------------------------------------------*/
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
# define KLV_MHCLOG 0 /* [0] 1: [don't] print mutual heat conductances */
# define KLV_CHCLOG 0 /* [0] 1: [don't] print heat conductance matrix */
# define KLV_RHCLOG 0 /* [0] 1: [don't] heat resistance matrix */
# define KLV_PRTSPC 0 /* [0] 1: [don't] display intermediate special values */
# define KLV_TSTRTB 0 /* [0] 1: [don't] test rtbis(*) */

# define PRINTF( SS1, SS2, XX ) \
{ \
   if (( state->klv_skp[0] != 's' )&&( state->klv_skp[0] != 'S' )) \
   { \
      printf( "\n %s   % .12e", (SS2), ( double )(XX)); \
      printf( "\n" ); \
      printf( "\n please acknowledge (%s) [ enter any character,", (SS1)); \
      printf( "\n enter 's' to skip further messages ]: " ); \
 \
      scanf( "%s", ptr ); \
      if (( ptr[0] == 's' )||( ptr[0] == 'S' )) \
      { \
         ( state->klv_skp[0] ) = 's'; \
      }; \
   }; \
}
# define SCANF( SS, XX ) \
{ \
   printf( " please enter %s: ", (SS)); \
   scanf( "%s", ptr ); \
   (XX) = strtod( ptr, endp ); \
}
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
   # define GRTOLRC ( 1.0e-08 )
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
/*----------------------------------------------------------------------------*/
/* structures typedefs, etc.: */

# include "types.h"
/*============================================================================*/
/* include the function body [ THERMDYN *kelvin( HELIOSSTATE *state ) ]: */
# include "kelvin.h"
/*============================================================================*/
# undef KLV_TEST_RTBIS
# undef KLV_PRTSPC

# undef KLV_ZBRDIV
# undef KLV_REORDR
# undef KLV_MAXITR 

# undef KLV_HTCLOG
# undef KLV_TMPLOG
# undef KLV_MHCLOG
# undef KLV_CHCLOG
# undef KLV_RHCLOG
/*----------------------------------------------------------------------------*/
# undef CELSIUS_TO_KELVIN 
# undef GRTOLRC 
# undef DXRTBIS 
# undef DXXNEWR
# undef DFFNEWR
# undef GIANT_VAL
# undef MHC_LWBOUND 
# undef MHC_UPBOUND 
# undef PRECISION
# undef SMALL_VAL
/*********************** end of function kelvin(*) ***************************/
/*
   Es ist gleichermassen toedlich fuer den Geist, ein System zu haben - und
   keines zu haben. Daher muss man sich dazu entschliessen, beides miteinander
   zu verbinden.
                                                            Friedrich Schlegel
*/
