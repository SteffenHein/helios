/* [ file: hlswrk.c ] */
# define DO_HLSWRK "hlswrk(*)"
/*******************************************************************************
*                                                                              *
*   Function hlswrk(*); HELIOS, release v1.0r1                                 *
*   [ Heat and Electric Losses In Ordinary wave guiding Structures ]           *
*                                                                              *
*   This function iterates the temperature function kelvin(*), the loss        *
*   function losses(*), and the heat conductances function hcndct(*).          *
*   It returns the final results and plot data by function values(*).          *
*                                                                              *
*   (C) SHEIN; Munich, December 2021                      Steffen Hein         *
*   [ Update: January 31, 2022 ]                        <contact@sfenx.de>     *
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
# include "../src/types.h"
/*----------------------------------------------------------------------------*/
# define WRK_USENRM 2 /* n: deviation measured in l_n norm [ n=1,2 ] */
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
/*----------------------------------------------------------------------------*/
/* macro homogeneity check: */
/* tolerated upper bound of side heat current density to loss ratio: */ 
# include "./HMGCHK.M"
/*----------------------------------------------------------------------------*/
/* macro endothermie check: */
/* tolerated lower bound of side heat current density to loss ratio: */ 
# include "./ENDOTH.M"
/*----------------------------------------------------------------------------*/
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
