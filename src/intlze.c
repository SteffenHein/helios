/* [ file: intlze.c ] */
# define DO_INTLZE "intlze(*)"
/*******************************************************************************
*                                                                              *
*   Function initlze(*); HELIOS, release v1.0r1                                *
*   [ Heat and Electric Losses In Ordinary wave guiding Structures ]           *
*                                                                              *
*   This function Initializes waveguide temperatures for transferred           *
*   parameters, such as given materials, radio frequency or CW power.          *
*   It also determines the cylinder structure of the waveguide system,         *
*   viz. essentially the structure of its heat conductance matrix,             *
*   which is iterativeliy updated in function hlswrk(*) ].                     *
*                                                                              *
*   (C) SHEIN; Munich, December 2021                      Steffen Hein         *
*   [ Update: December 17, 2021 ]                       <contact@sfenx.de>     *
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
# include "../math/consts.h"
# include "../math/maths.h"
/*----------------------------------------------------------------------------*/
/* Edit and customize this general configuration header: */
# include "../CONFIG.H" 
/*----------------------------------------------------------------------------*/
# include "../src/types.h"
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
/*
   Es ist gleichermassen toedlich fuer den Geist, ein System zu haben - und
   keines zu haben. Daher muss man sich dazu entschliessen, beides miteinander
   zu verbinden.
                                                          Friedrich Schlegel
*/
