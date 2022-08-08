/* [ file: hlsdrv.c ] */
# define DO_HELIOSDRV "heliosdrv(*)"
/*******************************************************************************
*                                                                              *
*   Driver function heliosdrv(*); HELIOS release v1.0r1                        *
*   [ Heat and Electric Losses In Ordinary wave guiding Structures ]           *
*                                                                              *
*   This function controls the parameter input and some corrections,           *
*   the initialization function initlze(*) and the final computational         *
*   routines of hlswrk(*).                                                     *
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
# include <float.h>
# include <limits.h>
# include <math.h>
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
# ifndef IPT_PARINIT
   # define IPT_PARINIT "par.init"
# endif
/*----------------------------------------------------------------------------*/
# ifndef IPT_PARLOG
   # define IPT_PARLOG "par.log"
# endif
/*----------------------------------------------------------------------------*/
# if USE_NCURSES == 1
   # include <ncurses.h>
# endif 
/*----------------------------------------------------------------------------*/
/* precision in heliosdrv: */
# ifndef PRECISION
   # define PRECISION ( 1.0e-15 )
# endif
/*----------------------------------------------------------------------------*/
/* very small double in heliosdrv: */
# ifndef SMALL_VAL
   # define SMALL_VAL ( 1.0e-301 )
# endif
/*----------------------------------------------------------------------------*/
/* giant double in heliosdrv: */
# ifndef GIANT_VAL
   # define GIANT_VAL ( 1.0e+277 )
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

# include "../src/types.h"
/*----------------------------------------------------------------------------*/
static OPERATIONS opr = {null};
static MATERIALS mat = {null};
static PARAMETERS par = {null};
static METALS met = {null};
static DIELCT del = {null};
static SOLIDS sld = {null};
static FLUIDS fld = {null};
/*
static GAUSS_JRD gsj = {null};
*/
static DIMENSIONS dms = {null};
static ELECTRIC elt = {null};
static THERMDYN trm = {null};
static HCURRS hcr = {null};
static HCNDCT cdc = {null};
static RESULTS res = {null};
static TXCNSL cns = {null};
/*----------------------------------------------------------------------------*/
static HELIOSSTATE helios = {null};
/*----------------------------------------------------------------------------*/
static char /* temporary file name */
   tmpfle[STS_SIZE] = {null};
/*----------------------------------------------------------------------------*/
# include "../src/inputs.h"
/*----------------------------------------------------------------------------*/
# if USE_NCURSES == 1
/* 'my_terminal' configuration: */

   # include <termcap.h>     /* terminal type header */
   static char *term;        /* terminal type string */ 

# ifndef CLSCREEN
   # define CLSCREEN { \
     fprintf( stdout, "%s", tgetstr( "cl", null )); \
   }
# endif
# ifndef PRBLDCLR
   # define PRBLDCLR(a) {\
     fprintf( stdout, \
        "%s%s", tgetstr( "md", null ), (a)); /* bold clear output */ \
   }
# endif
# ifndef PRINVERS
   # define PRINVERS(a) {\
     fprintf( stdout, \
        "%s%s", tgetstr( "mr", null ), (a)); /* inverse */ \
   }
# endif
# ifndef PRNORMAL
   # define PRNORMAL(a) {\
     fprintf( stdout, \
        "%s%s", tgetstr( "me", null ), (a)); /* back to normal output */ \
   }
# endif
# else /* if USE_NCURSES != 1 */
# ifndef CLSCREEN
   # define CLSCREEN { \
     fprintf( stdout, "\f" ); \
   }
# endif
# ifndef PRBLDCLR
   # define PRBLDCLR(a) {\
     fprintf( stdout, "%s", (a));\
   }
# endif
# ifndef PRINVERS
   # define PRINVERS(a) {\
     fprintf( stdout, "%s", (a));\
   }
# endif
# ifndef PRNORMAL
   # define PRNORMAL(a) {\
     fprintf( stdout, "%s", (a));\
   }
# endif
# endif /* USE_NCURSES != 1 */
/*============================================================================*/
/* include the function body [ short heliosdrv( int argn, char **args ) ] */
# include "hlsdrv.h" 
/*============================================================================*/
# undef DO_HELIOSDRV
/********************* end of function 'heliosdrv(*)' *************************/
/*
   Es ist gleichermassen toedlich fuer den Geist, ein System zu haben - und
   keines zu haben. Daher muss man sich dazu entschliessen, beides miteinander
   zu verbinden.
                                                          Friedrich Schlegel
*/
