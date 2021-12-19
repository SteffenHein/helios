/* [ file: values.c ] */
# define DO_VALUES "values(*)"
/*******************************************************************************
*                                                                              *
*   Function values(*); HELIOS, release v1.0r1                                 *
*   [ Heat and Electric Losses In Ordinary wave guiding Systems ]              *
*                                                                              *
*   This function generates text and graphics files for results computed with  *
*   program HELIOS                                                             *
*                                                                              *
*   (C) SHEIN; Munich, December 2021                       Steffen Hein        *
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
# include "../math/maths.h"
# include "../math/consts.h"
/*----------------------------------------------------------------------------*/
/* Edit and customize this general configuration header: */
# include "../CONFIG.H" 
/*----------------------------------------------------------------------------*/
# define RES_KLVTDC 1 /* convert temperatures from KELVIN to DEG_CELSIUS  */
# define RES_MAXTMP 1
# define RES_CW_PWR 1
# define RES_TEMPRS 1
# define RES_LOSSES 1
# define RES_OPRLOG 1
# define RES_TMPLOG 1
# define RES_CHCLOG 1
# define RES_PEAKTP 1
# define RES_STRFMT 2 /* format of mhc and mrc matrices stored in store_val(*)*/
/*----------------------------------------------------------------------------*/
# include "./DISPLAY.M"
/*----------------------------------------------------------------------------*/
# ifndef CELSIUS_TO_KELVIN 
   # define CELSIUS_TO_KELVIN ( 273.15 )
# endif
/*----------------------------------------------------------------------------*/
# ifndef GIANT_VAL 
   # define GIANT_VAL ( 1.e+277 )
# endif
/*----------------------------------------------------------------------------*/
# ifndef SMALL_VAL 
   # define SMALL_VAL ( 1.e-277 )
# endif
/*----------------------------------------------------------------------------*/
/* structures typedefs, etc.: */
# include "../src/types.h"
/*----------------------------------------------------------------------------*/
# if (( RES_OPRLOG == 1 ) \
    ||( RES_TMPLOG == 1 ) \
    ||( RES_CHCLOG == 1 ))

    # include "store_val.h"
# endif
/*============================================================================*/
/* the function body [ RESULTS *values( HELIOSSTATE *state ) ]: */
# include "values.h"
/*============================================================================*/
# undef RES_CW_PWR
# undef RES_TEMPRS
# undef RES_LOSSES
# undef RES_OPRLOG
# undef RES_TMPLOG
# undef RES_CHCLOG
/*----------------------------------------------------------------------------*/
# undef CELSIUS_TO_KELVIN 
/************************ end of function 'values(*)' *************************/
/*
   Es ist gleichermassen toedlich fuer den Geist, ein System zu haben - und
   keines zu haben. Daher muss man sich dazu entschliessen, beides miteinander
   zu verbinden.
                                                          Friedrich Schlegel
*/
