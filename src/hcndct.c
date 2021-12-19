/* [ file: hcndct.c ] */
# define DO_HCNDCT "hcndct(*)"
/*******************************************************************************
*                                                                              *
*   Function hcndct(*); HELIOS, release v1.0r1                                 *
*   [ Heat and Electric Losses In Ordinary wave guiding Structures ]           *
*                                                                              *
*   This function computes and inverts the heat conductance matrix for given   *
*   heat resistances between parallel conductors                               *
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
# include <string.h>
/*----------------------------------------------------------------------------*/
# include "../math/maths.h"
# include "../math/consts.h"
/*----------------------------------------------------------------------------*/
/* Edit and customize this general configuration header: */
# include "../CONFIG.H"
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
/* macro homogeneity check */
# include "./HMGCHK.M"
/*----------------------------------------------------------------------------*/
/* macro endothermie check */
# include "./ENDOTH.M"
/*----------------------------------------------------------------------------*/
# include "./types.h"
# include "./DISPLAY.M"
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
/*----------------------------------------------------------------------------*/
/* include the function body [ HCNDCT *hcndct( HELIOSSTATE *state ) ]: */
# include "hcndct.h"
/*============================================================================*/
# undef HCD_MHCLOG
/*----------------------------------------------------------------------------*/
# undef GIANT_VAL
# undef PRECISION
# undef SMALL_VAL
/*********************** end of function hcndct(*)' **************************/
/*
   Es ist gleichermassen toedlich fuer den Geist, ein System zu haben - und
   keines zu haben. Daher muss man sich dazu entschliessen, beides miteinander
   zu verbinden.
                                                            Friedrich Schlegel
*/
