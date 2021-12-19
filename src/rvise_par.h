/* [ file: rvise_par.h ] */
/*******************************************************************************
*                                                                              *
*   Function rvise_par(*); HELIOS release v1.0r1                               *
*   [ Heat and Electric Losses In Ordinary wave guiding Structures ]           *
*                                                                              *
*   This function revises [ and corrects if necessary ] the physical           *
*   parameters [ input option "parameters"], thereby respecting the following  *
*   hierarchy [ from highest to lowest priority ]:                             *
*                                                                              *
*                   OPERATIONS -> MATERIALS -> PARAMETERS                      *
*                                                                              *
*   Thus, a former input in this chain always dominates any subsequent input   *
*   in conflicting cases.                                                      *
*   Also, this function settles any internal parameter dependencies where such *
*   exist [ only in input option "parameters" ].                               *
*                                                                              *
*   (C) SHEIN; Munich, December 2021                       Steffen Hein        *
*   [ Update: December 17, 2021 ]                       <contact@sfenx.de>     *
*                                                                              *
*******************************************************************************/
# ifndef HLS_NUSSELT
   # define HLS_NUSSELT 1 /* 1: use Nusselt number approximations */
# endif                   /* [ function nusselt(*) ] */
/*----------------------------------------------------------------------------*/
# ifndef INIT_NONZERO     /* avoid illegal ZERO for some parameters: initia-  */
   # define INIT_NONZERO (1.) /* lize to that non-zero value [if not yet done]*/
# endif 
/*----------------------------------------------------------------------------*/
/* Reynolds number determining transition from Hausen's to Kirschbaums formula*/
/* for Nusselt number in ring tubes: */
# ifndef HLS_KRSCHB
   # define HLS_KRSCHB ( 8000.)
# endif
/* Transition regime between laminar to turbulent flow: */
# ifndef HLS_RELMNR
   # define HLS_RELMNR ( 10000.) /* Re <= HLS_RELMNR: laminar flow */
# endif
# ifndef HLS_RETURB
   # define HLS_RETURB ( 10000.) /* HLS_RETURB <= Re: turbulent flow */
# endif
/* Neglected mantle [ inner-outer ] temperature difference */
# ifndef WRK_DTMNGL
   # define WRK_DTMNGL ( 6.7e-03 ) /* neglected mantle temperature difference */
# endif
/*----------------------------------------------------------------------------*/

/*============================================================================*/

short rvise_params( void )
{
   HELIOSSTATE *state = &helios;

   static const char
     *sstrng = "**********************";   

   static OPERATIONS
     *opp = NULL;

   static MATERIALS
     *map = NULL;

   static PARAMETERS
     *prp = NULL;

   static DIMENSIONS
     *dmp = NULL;

   static ELECTRIC
     *elp = NULL;

   static THERMDYN
     *thp = NULL;

   static METALS
     *mtp = NULL;

   static DIELCT
     *dlp = NULL;

   static SOLIDS
     *slp = NULL;

   static FLUIDS
     *flp = NULL;

   METALS
     *metals ( METALS *mtp );

   DIELCT
     *dielct ( DIELCT *dlp );

   SOLIDS
     *solids ( SOLIDS *slp );

   FLUIDS
     *fluids( FLUIDS *flp );

   static short
      ii = null,
      nn = null;
/*
   static char
      ptr[STS_SIZE] = {null};
*/
   static double
      temp = ZERO, /* mean temperature */
      dhyd = ZERO, /* hydraulic diameter */
      lsec = ZERO, /* effective length of cooled section */
      pn = ZERO,
      ny = ZERO, /* cinematic viscosity [m^2/s] */
      re = ZERO,
      nu = ZERO,
      ve = ZERO,
      ss = ZERO;

 static const char
   *depdt = "dependent",
   *inact = "inactive",
   *dmlss = "";

   void
      cpypar( short index, const char *ltext, const char *bracket );

   double
      nusselt( double reynolds, double prandtl, double ratio, char *formula );
/*----------------------------------------------------------------------------*/
/* parameter strings:                                                         */

   ( state->opp ) = &opr;
   ( state->map ) = &mat;
   ( state->prp ) = &par;
   ( state->dmp ) = &dms;
   ( state->elp ) = &elt;
   ( state->thp ) = &trm;

   ( state->mtp ) = &met;
   ( state->dlp ) = &del;
   ( state->slp ) = &sld;
   ( state->flp ) = &fld;

   opp = ( state->opp );
   map = ( state->map );
   prp = ( state->prp );
   dmp = ( state->dmp );
   elp = ( state->elp );
   thp = ( state->thp );
   mtp = ( state->mtp );
   slp = ( state->slp );
   flp = ( state->flp );
   dlp = ( state->dlp );
/*............................................................................*/
/* number of parameters */

   ( prp->s[null] )  = 71;
   nn = ( short )( prp->s[null] );
/*............................................................................*/
   strcpy( par.stx[0] , "parameters\n" );
   strcat( par.stx[0] , "[INTERNATIONAL_UNITS;"\
      "_material_constants_at_0_Celsius_and_1_bar_abs_pressure]" );

   cpypar( 1, "Starting_[lower_or_upper]_frequency", "Hz" );
   cpypar( 2, "Stopping_[upper_or_lower]_frequency", "Hz" );
   cpypar( 3, "Transmitted_CW_power", "Watts" );
   cpypar( 4, "Temperature,_environment", "deg_Celsius" );
   cpypar( 5, "Temperature,_waveguide_mantle", "deg_Celsius" );
   cpypar( 6, "Temperature,_inner_conductor", "deg_Celsius" );
   cpypar( 7, "Pressure,_environment", "bar_absolute" );
   cpypar( 8, "Pressure_in_waveguide", "bar_absolute" );
   cpypar( 9, "Heat_radiation_emissivity,_mantle", dmlss );
   cpypar( 10, "Heat_radiation_emissivity,_outer_conductor", dmlss );
   cpypar( 11, "Heat_radiation_emissivity,_inner_conductor", dmlss );
   cpypar( 12, "Diameter,_outer_conductor", "m" );
   cpypar( 13, "Diameter,_inner_conductor", "m" );
   cpypar( 14, "Diameter,_mantle_[outer_diameter]", "m" );
   cpypar( 15, "Diameter,_pipe_[inner_diameter]", "m" );
   cpypar( 16, "Form_factor,_inner_conductor", dmlss );
   cpypar( 17, "Form_factor,_outer_conductor", dmlss );
   cpypar( 18, "Electric_resistivity,_inner_conductor", "Ohm*m" );
   cpypar( 19, "Temperature_coefficient_of_the_latter", "1/K" );
   cpypar( 20, "Relative_permeability,_inner_conductor", dmlss );
   cpypar( 21, "Heat_conductivity,_inner_conductor", "W/(m*K)" );
   cpypar( 22, "Temperature_coefficient_of_the_latter", "1/K" );
   cpypar( 23, "Density,_inner_conductor", "Kg/m^3" );
   cpypar( 24, "Heat_capacity_[cp],_inner_conductor", "J/(Kg*K)" );
   cpypar( 25, "Electric_resistivity,_outer_conductor", "Ohm*m" );
   cpypar( 26, "Temperature_coefficient_of_the_latter", "1/K" );
   cpypar( 27, "Relative_permeability,_outer_conductor", dmlss );
   cpypar( 28, "Heat_conductivity,_outer_conductor", "W/(m*K)" );
   cpypar( 29, "Temperature_coefficient_of_the_latter", "1/K" );
   cpypar( 30, "Density,_outer_conductor", "Kg/m^3" );
   cpypar( 31, "Heat_capacity,_outer_conductor", "J/(Kg*K)" );
   cpypar( 32, "Relative_permittivity,_dielectric", "'epsilon-r'" );
   cpypar( 33, "Dielectric_loss_factor", "'tangent_delta'" );
   cpypar( 34, "Relative_permeability,_dielectric", "'my-r'" );
   cpypar( 35, "Magnetic_loss_factor", "'tangent_delta'" );
   cpypar( 36, "Heat_conductivity,_dielectric", "W/(K*m)" );
   cpypar( 37, "Temperature_coefficient_of_the_latter", "1/K" );
   cpypar( 38, "Density,_dielectric", "Kg/m^3" );
   cpypar( 39, "Heat_capacity_[cp],_dielectric", "J/(Kg*K)" );
   cpypar( 40, "Cinematic_viscosity,_dielectric", "m^2/s" );
   cpypar( 41, "Temperature_coefficient_of_the_latter", "1/K" );
   cpypar( 42, "Prandtl_number,_dielectric", dmlss );
   cpypar( 43, "Heat_conductivity,_mantle_[outer_tube]", "W/(K*m)" );
   cpypar( 44, "Temperature_coefficient_of_the_latter", "1/K" );
   cpypar( 45, "Heat_conductivity,_pipe_[inner_tube]", "W/(K*m)" );
   cpypar( 46, "Temperature_coefficient_of_the_latter", "1/K" );
   cpypar( 47, "Heat_conductivity,_cooling_fluid", "W/(K*m)" );
   cpypar( 48, "Temperature_coefficient_of_the_latter", "1/K" );
   cpypar( 49, "Density,_cooling_fluid", "Kg/m^3" );
   cpypar( 50, "Heat_capacity_[cp],_cooling_fluid", "J/(Kg*K)" );
   cpypar( 51, "Cinematic_viscosity,_cooling_fluid", "m^2/s" );
   cpypar( 52, "Temperature_coefficient_of_the_latter", "1/K" );
   cpypar( 53, "Prandtl_number,_cooling_fluid", dmlss );
   cpypar( 54, "Temperature,_gas_flowing_in_waveguide", "deg_Celsius" );
   cpypar( 55, "Flow,_that_gas", "liter/min" );
   cpypar( 56, "Nusselt_number,_that_gas_flow", dmlss );
   cpypar( 57, "Temperature,_cooling_fluid", "deg_Celsius" );
   cpypar( 58, "Pressure,_cooling_fluid", "bar_absolute" );
   cpypar( 59, "Flow,_cooling_fluid", "liter/min" );
   cpypar( 60, "Nusselt_number,_that_fluid_flow", dmlss );
   cpypar( 61, "Flow_velocity,_air_in_environment", "m/s" );
   cpypar( 62, "Nusselt_number,_that_air_flow", dmlss );
   cpypar( 63, "Length_of_cooled_section(s),_dielectric", "m" );
   cpypar( 64, "Length_of_cooled_section(s),_pipe", "m" );
   cpypar( 65, "Waveguide_length", "m" );
   cpypar( 66, "Fixed_side_temperature,_mantle", "deg_Celsius" );
   cpypar( 67, "Fixed_heat_outflow_on_this_side,_mantle", "W" );
   cpypar( 68, "Fixed_heat_inflow_[opposite_side],_mantle", "W" );
   cpypar( 69, "Fixed_side_temperature,_pipe", "deg_Celsius" );
   cpypar( 70, "Fixed_heat_outflow_on_this_side,_pipe", "W" );
   cpypar( 71, "Fixed_heat_inflow_[opposite_side],_pipe", "W" );
/*............................................................................*/
/* revisions: */

/* initial ZERO not allowed in certain cases: */

  ii = ONE; do
  {
     switch( ii )
     {
/*............................................................................*/
/* in general avoid ZERO: */

        default:
         if (( fabs( prp->s[ii] )) < 1.e-277 )
            ( prp->s[ii] ) = INIT_NONZERO;
         break;
/*............................................................................*/
/* however, ZERO is allowed in the following cases: */
        case 4:   /* temperatures */
        case 5:
        case 6:
        case 54: 
	case 57:
         break;

        case 9:  /* heat radiation emissivities */
        case 10:
        case 11:
         break;

        case 19: /* temperature coefficients */
        case 22:
	case 26:
	case 29:
	case 37:
	case 41:
	case 44:
	case 46:
	case 48:
	case 52:
         break;

        case 33: /* dielectric loss factor */ 
        case 35: /* magnetic loss factpr */
         break;
/*............................................................................*/
/* ZERO not allowed in the following cases: */

        case 7: /* pressure, environment */
         if ((( opp->n[5] ) != null )
           &&(( fabs( prp->s[ii] )) < 1.e-277 ))
            ( prp->s[ii] ) = INIT_NONZERO;
        break;

        case 8: /* pressure, dielectric */
         if ((( opp->n[3] ) != null )
           &&(( fabs( prp->s[ii] )) < 1.e-277 ))
            ( prp->s[ii] ) = INIT_NONZERO;
        break;

	case 58: /* pressure, inner pipe: */
         if ((( opp->n[7] ) != null )
           &&(( fabs( prp->s[ii] )) < 1.e-277 ))
            ( prp->s[ii] ) = INIT_NONZERO;
        break;
	 
        case 40: /* cinematic viscosity, dielectric */
         if ((( opp->n[5] ) != null )
           &&(( fabs( prp->s[ii] )) < 1.e-277 ))
            ( prp->s[ii] ) = INIT_NONZERO;
        break;

        case 51: /* cinematic viscosity, cooling fluid */
         if ((( opp->n[7] ) != null )
           &&(( fabs( prp->s[ii] )) < 1.e-277 ))
            ( prp->s[ii] ) = INIT_NONZERO;
        break;

	case 55: /* forced cooling flow, dielectric */
         if ((( opp->n[4] ) != null )
           &&(( fabs( prp->s[ii] )) < 1.e-277 ))
            ( prp->s[ii] ) = INIT_NONZERO;
        break;

        case 59: /* forced cooling flow, pipe */
         if ((( opp->n[7] ) != null )
           &&(( fabs( prp->s[ii] )) < 1.e-277 ))
            ( prp->s[ii] ) = INIT_NONZERO;
        break;

        case 61: /* forced flow velocity, environment */
         if ((( opp->n[6] ) != null )
           &&(( fabs( prp->s[ii] )) < 1.e-277 ))
            ( prp->s[ii] ) = INIT_NONZERO;
        break;

        case 42: /* Prandtl number */
         if ((( opp->n[4] ) != null )
           &&(( fabs( prp->s[ii] )) < 1.e-277 ))
            ( prp->s[ii] ) = INIT_NONZERO;
         break;

        case 53: /* Prandtl number */
         if ((( opp->n[7] ) != null )
           &&(( fabs( prp->s[ii] )) < 1.e-277 ))
            ( prp->s[ii] ) = INIT_NONZERO;
         break;

        case 56: /* Nusselt number */
         if ((( opp->n[4] ) != null )
           &&(( fabs( prp->s[ii] )) < 1.e-277 ))
            ( prp->s[ii] ) = 3.65; /* apprx Nusselt number of laminar flow */
         break;

        case 60: /* Nusselt number, cooling pipe */
         if ((( opp->n[7] ) != null )
           &&(( fabs( prp->s[ii] )) < 1.e-277 ))
            ( prp->s[ii] ) = 3.65; /* apprx Nusselt number of laminar flow */
         break;

        case 62: /* Nusselt number environment */
         if ((( opp->n[6] ) != null )
           &&(( fabs( prp->s[ii] )) < 1.e-277 ))
            ( prp->s[ii] ) = 3.65; /* apprx Nusselt number of laminar flow */
         break;

        case 63: /* length of cooled section(s), dielectric */
         if ((( opp->n[4] ) != null )
           &&(( fabs( prp->s[ii] )) < 1.e-277 ))
            ( prp->s[ii] ) = 1.;
         break;

        case 64: /* length of cooled section(s), cooling pipe */
         if ((( opp->n[7] ) != null )
           &&(( fabs( prp->s[ii] )) < 1.e-277 ))
            ( prp->s[ii] ) = 1.;
         break;

        case 65: /* waveguide length */
         if (((( opp->n[8] ) != null )
           ||(( opp->n[9] ) != null ))
           &&( fabs( prp->s[ii] ) < 1.e-277 ))
            ( prp->s[ii] ) = 1.;
         break;

        case 66: /* fixed side temperature, mantle */
         if ((( opp->n[8] ) != null )
           &&(( fabs( prp->s[ii] )) < 1.e-277 ))
            ( prp->s[ii] ) = 0.;
         break;

        case 67: /* fixed heat outflow, mantle */
         if ((( opp->n[9] ) != null )
           &&(( fabs( prp->s[ii] )) < 1.e-277 ))
            ( prp->s[ii] ) = 0.;
         break;

        case 68: /* fixed heat inflow, opposite side, mantle */
         if (((( opp->n[8] ) != null )\
           ||(( opp->n[9] ) != null ))
           &&(( fabs( prp->s[ii] )) < 1.e-277 ))
            ( prp->s[ii] ) = 0.;
         break;

        case 69: /* fixed side temperature, pipe */
         if ((( opp->n[8] ) != null )\
           &&(( fabs( prp->s[ii] )) < 1.e-277 ))
            ( prp->s[ii] ) = 0.;
         break;

        case 70: /* fixed heat outflow, pipe */
         if ((( opp->n[9] ) != null )
           &&(( fabs( prp->s[ii] )) < 1.e-277 ))
            ( prp->s[ii] ) = 0.;
         break;

        case 71: /* fixed heat inflow, opposite side, pipe */
         if (((( opp->n[8] ) != null )\
           ||(( opp->n[9] ) != null ))
           &&(( fabs( prp->s[ii] )) < 1.e-277 ))
            ( prp->s[ii] ) = 0.;
         break;
      };
   } while(( ii++ ) < nn );
/*............................................................................*/
/* normalize waveguide length: */
   ( dmp->lwgd ) = 1. ;
/*............................................................................*/
   if (( opp->n[10] ) <= ONE )
   {
      cpypar( 1, "Frequency_F1", "Hz" );
      cpypar( 2, "F2=F1", "only_one_frequency_defined;_inactive" );

      par.s[2] = par.s[1];
   }
   else
   {
      cpypar( 1, "Starting_[lower_or_upper]_frequency", "Hz" );
      cpypar( 2, "Stopping_[upper_or_lower]_frequency", "Hz" );
   };
/*............................................................................*/
/* cooled section length(s), dielectric: */

   if (( opp->n[4] ) == ONE )
      cpypar( 63, "Length_of_cooled_section(s),_dielectric", "m" );
   else
      cpypar( 63, "Length_of_cooled_section(s),_dielectric", inact );
/*............................................................................*/
/* cooled section length(s), in inner cooling pipe: */

   if (( opp->n[7] ) == ONE )
      cpypar( 64, "Length_of_cooled_section(s),_cooling_pipe", "m" );
   else
      cpypar( 64, "Length_of_cooled_section(s),_pipe", inact );
/*............................................................................*/
   if (( opp->n[1] ) == null ) /* rectangular waveguide */
   {
      cpypar( 10, "Heat_radiation_emissivity,_outer_conductor", inact );
      cpypar( 11, "Heat_radiation_emissivity,_inner_conductor", inact );
      cpypar( 12, "Waveguide_width_[a]", "m" );
      cpypar( 13, "Waveguide_height_[b]", "m" );
      cpypar( 14, "Waveguide_mantle_width", "m" );
      cpypar( 15, "Waveguide_mantle_height", "m" );
      cpypar( 16, "Form_factor,_waveguide", dmlss );
      cpypar( 17, "Form_factor,_outer_coaxial_conductor", inact );
/*...........................................................................*/
/* absent convection in conductors */

      if (( opp->n[3] ) == null )
      {
         cpypar( 8, "Pressure_in_waveguide", inact );
         cpypar( 10, "Heat_radiation_emissivity,_outer_conductor", inact );
         cpypar( 11, "Heat_radiation_emissivity,_inner_conductor", inact );
         cpypar( 40, "Cinematic_viscosity,_dielectric", inact );
         cpypar( 41, "Temperature_coefficient_of_the_latter", inact );
         cpypar( 42, "Prandtl_number,_dielectric", inact );
         cpypar( 54, "Temperature,_gas_flowing_in_waveguide", inact );
         cpypar( 55, "Flow_[liter/min],_that_gas", inact );
         cpypar( 56, "Nusselt_number,_that_gas_flow", inact );
      }
      else if (( opp->n[4] ) == null )
      {
         cpypar( 54, "Temperature,_gas_flowing_in_waveguide", inact );
         cpypar( 55, "Flow_[liter/min],_that_gas", inact );
         cpypar( 56, "Nusselt_number,_that_gas_flow", inact );
      }
      else if (( opp->n[4] ) == ONE )
      {
/*............................................................................*/
# if HLS_NUSSELT == 1
         cpypar( 8, "Pressure_in_waveguide", "bar_absolute" );
         cpypar( 55, "Flow,_that_gas", "liter/min" );
         cpypar( 56, "Nusselt_number,_that_gas_flow", depdt );
# else
         cpypar( 8, "Pressure_in_waveguide", inact );
         cpypar( 55, "Flow_[liter/min],_that_gas", inact );
         cpypar( 56, "Nusselt_number,_that_gas_flow", dmlss );
# endif
/*............................................................................*/
      }
      else if (( opp->n[4] ) == TWO )
      {
         cpypar( 8, "Pressure_in_waveguide", inact );
         cpypar( 55, "Flow_[liter/min],_that_gas", inact );
         cpypar( 56, "Nusselt_number,_that_gas_flow", dmlss );
      };

      cpypar( 25, "Electric_resistivity,_outer_conductor", inact );
      cpypar( 26, "Temperature_coefficient_of_the_latter", inact );
      cpypar( 27, "Relative_permeability,_outer_conductor", inact );
      cpypar( 28, "Heat_conductivity,_outer_conductor", inact );
      cpypar( 29, "Temperature_coefficient_of_the_latter", inact );
      cpypar( 30, "Density,_outer_conductor", inact );
      cpypar( 31, "Heat_capacity_[cp],_outer_conductor", inact );

      cpypar( 45, "Heat_conductivity,_pipe_[inner_tube]", inact );
      cpypar( 46, "Temperature_coefficient_of_the_latter", inact );
      cpypar( 47, "Heat_conductivity,_cooling_fluid", inact );
      cpypar( 48, "Temperature_coefficient_of_the_latter", inact );
      cpypar( 49, "Density,_cooling_fluid", inact );
      cpypar( 50, "Heat_capacity_[cp],_cooling_fluid", inact );
      cpypar( 51, "Cinematic_viscosity,_cooling_fluid", inact );
      cpypar( 52, "Temperature_coefficient_of_the_latter", inact );
      cpypar( 53, "Prandtl_number,_cooling_fluid", inact );
      cpypar( 57, "Temperature,_cooling_fluid", inact );
      cpypar( 58, "Presssure,_cooling_fluid", inact );
      cpypar( 59, "Flow_[liter/min],_cooling_fluid", inact );
      cpypar( 60, "Nusselt_number,_that_fluid_flow", inact );
/*............................................................................*/
/* fixed temperatures on one side */

      cpypar( 65, "Waveguide_length", inact );
      cpypar( 66, "Fixed_side_temperature,_mantle", inact );
      cpypar( 67, "Fixed_heat_outflow_on_this_side,_mantle", inact );
      cpypar( 68, "Fixed_heat_inflow_[opposite_side],_mantle", inact );
      cpypar( 69, "Fixed_side_temperature,_pipe", inact );
      cpypar( 70, "Fixed_heat_outflow_on_this_side,_pipe", inact );
      cpypar( 71, "Fixed_heat_inflow_[opposite_side],_pipe", inact );

      ii = ( opp->n[8] );
      switch( ii )
      {
        default:
         break;

        case 1:
         ( dmp->lwgd ) = ( prp->s[65] ); /* waveguide length */
         cpypar( 65, "Waveguide_length", "m" );
         cpypar( 66, "Fixed_side_temperature,_mantle", "deg_Celsius" );
         cpypar( 67, "Heat_outflow_on_this_side,_mantle", depdt );
         cpypar( 68, "Fixed_heat_inflow_[opposite_side],_mantle", "W" );
         break;

        case 3:
         ( dmp->lwgd ) = ( prp->s[65] ); /* waveguide length */
         cpypar( 65, "Waveguide_length", "m" );
         cpypar( 66, "Fixed_side_temperature,_mantle", "deg_Celsius" );
         cpypar( 67, "Heat_outflow_on_this_side,_mantle", depdt );
         cpypar( 68, "Fixed_heat_inflow_[opposite_side],_mantle", "W" );
         break;
      };
/*............................................................................*/
/* fixed heat outflow on one side */

      ii = ( opp->n[9] );
      switch( ii )
      {
        default:
         break;

        case 1:
         ( dmp->lwgd ) = ( prp->s[65] ); /* waveguide length */
         cpypar( 65, "Waveguide_length", "m" );
         cpypar( 66, "Side_temperature,_mantle", depdt );
         cpypar( 67, "Fixed_heat_outflow_on_this_side,_mantle", "W" );
         cpypar( 68, "Fixed_heat_inflow_[opposite_side],_mantle", "W" );
         break;

        case 3:
         ( dmp->lwgd ) = ( prp->s[65] ); /* waveguide length */
         cpypar( 65, "Waveguide_length", "m" );
         cpypar( 66, "Side_temperature,_mantle", depdt );
         cpypar( 67, "Fixed_heat_outflow_on_this_side,_mantle", "W" );
         cpypar( 68, "Fixed_heat_inflow_[opposite_side],_mantle", "W" );
         break;
      }; /* end, switch */
/*............................................................................*/
/* fixing any mantle temperature disconnects from environment: */

      if ( opp->n[5] == null )
      {
	 cpypar( 4, "Temperature,_environment", inact );
	 cpypar( 5, "Temperature,_waveguide_mantle", "deg_Celsius" );
	 cpypar( 9, "Heat_radiation_emissivity,_mantle", inact );
	 cpypar( 61, "Flow_velocity_[m/s],_air_in_environment", inact );
	 cpypar( 62, "Nusselt_number,_that_air_flow", inact );

         cpypar( 65, "Waveguide_length", inact );
         cpypar( 66, "Side_temperature,_mantle", depdt );
         cpypar( 67, "Heat_outflow_on_this_side,_mantle", inact );
         cpypar( 68, "Fixed_heat_inflow_[opposite_side],_mantle", inact );
      }
      else /* if ( opp->n[5] != null ) - convective heat exchange with */
      {                                                    /* environment */
	 cpypar( 5, "Temperature,_waveguide_mantle", depdt );

         if (( opp->n[6] ) == null ) /* no forced convection */
         {
	    cpypar( 61, "Flow_velocity_[m/s],_air_in_environment", inact );
            cpypar( 62, "Nusselt_number,_that_air_flow", inact );
         }
         else if ( opp->n[6] == ONE )
         {
/*............................................................................*/
# if HLS_NUSSELT == 1
	    cpypar( 4, "Temperature,_environment", "deg_Celsius" );
	    cpypar( 61, "Flow_velocity,_air_in_environment", "m/s" );
            cpypar( 62, "Nusselt_number,_that_air_flow", depdt );
# else
	    cpypar( 4, "Temperature,_environment", "deg_Celsius" );
	    cpypar( 61, "Flow_velocity_[m/s],_air_in_environment", inact );
            cpypar( 62, "Nusselt_number,_that_air_flow", dmlss );
# endif
/*............................................................................*/
         }
         else if (( opp->n[6] ) == TWO )
         {
	    cpypar( 4, "Temperature,_environment", "deg_Celsius" );
	    cpypar( 61, "Flow_velocity_[m/s],_air_in_environment", inact );
            cpypar( 62, "Nusselt_number,_that_air_flow", dmlss );
         }
         else
         {
	    cpypar( 4, "Temperature,_environment" , "deg_Celsius" );
            cpypar( 62, "Nusselt_number,_that_air_flow", inact );
         }; 
      }; /* end if (( opp->n[5] ) == ONE ): convection in environment */
/*............................................................................*/
      if (( opp->n[2] ) == ONE ) /* given [ fixed ] inner condctr temperature */
      {
         cpypar( 3, "Transmitted_CW_power", "initial_value;_Watts" );
         cpypar( 6, "Temperature,_waveguide", "deg_Celsius" );
      }
      else /* given [ fixed ] CW power */
      {
         cpypar( 3, "Transmitted_CW_power", "Watts" );
         cpypar( 6, "Temperature,_waveguide", "initial_value;_C" );
      };
/*............................................................................*/
/* physical properties, waveguide metal: */

      if (( null != strcmp(( map->shi ), "else" ))
	&&( null != strcmp(( map->shi ), "Else" ))
	&&( null != strcmp(( map->shi ), "ELSE" )))
      {
	 strcpy(( mtp->shm ), ( map->shi ));
	 strcpy(( mtp->met ), sstrng );
/*............................................................................*/
	 ( mtp ) = metals ( mtp );     /*                       */
/*.................................................*/
	 strcpy(( map->shi ), ( mtp->shm ));

	 ( prp->s[18] ) = ( mtp->er );
	 ( prp->s[19] ) = ( mtp->der );
	 ( prp->s[20] ) = ( mtp->myr );
	 ( prp->s[21] ) = ( mtp->hc );
	 ( prp->s[22] ) = ( mtp->dhc );
	 ( prp->s[23] ) = ( mtp->md );
	 ( prp->s[24] ) = ( mtp->cv );

	 cpypar( 18, "Electric_resistivity,_waveguide_metal", depdt );
	 cpypar( 19, "Temperature_coefficient_of_the_latter", depdt );
         cpypar( 20, "Relative_permeability,_waveguide_metal", depdt );
	 cpypar( 21, "Heat_conductivity,_waveguide_metal", depdt );
	 cpypar( 22, "Temperature_coefficient_of_the_latter", depdt );
	 cpypar( 23, "Density,_waveguide_metal", depdt );
	 cpypar( 24, "Heat_capacity_[cp],_waveguide_metal", depdt );
      }
      else
      {
	 strcpy(( map->shi ), "ELSE" );

	 cpypar( 18, "Electric_resistivity,_waveguide_metal", "Ohm*m" );
	 cpypar( 19, "Temperature_coefficient_of_the_latter", "1/K" );
         cpypar( 20, "Relative_permeability,_inner_conductor", dmlss );
	 cpypar( 21, "Heat_conductivity,_waveguide_metal", "W/(m*K)" );
	 cpypar( 22, "Temperature_coefficient_of_the_latter", "1/K" );
	 cpypar( 23, "Density,_waveguide_metal", "Kg/m^3" );
	 cpypar( 24, "Heat_capacity_[cp],_waveguide_metal", "J/(Kg*K)" );
      };
/*............................................................................*/
/* physical properties, dielectric: */

      if (( null != strcmp(( map->shd ), "else" ))
        &&( null != strcmp(( map->shd ), "Else" ))
        &&( null != strcmp(( map->shd ), "ELSE" )))
      {
         strcpy(( dlp->shd ), ( map->shd ));
         strcpy(( dlp->del ), sstrng );
/*............................................................................*/
         ( dlp ) = dielct ( dlp );                   /*                       */
/*.................................................*/
         if ((( opp->n[3] ) == ONE )     /* fluid dielectric */
           &&((( dlp->ny ) < 1.e-217 )   /* unspecified cinematic viscosity */
            ||( 1.e+217 < ( dlp->ny )))) /* infinite cinematic viscosity */
         {
            strcpy(( dlp->shd ), "AIR" );  /* replace solid dielectric */
            strcpy(( dlp->del ), sstrng ); /* by air parameters */
/*............................................................................*/
            ( dlp ) = dielct ( dlp );                 /*                      */
/*..................................................*/
         };
         strcpy(( map->shd ), ( dlp->shd ));

         ( prp->s[32] ) = ( dlp->epr );
         ( prp->s[33] ) = ( dlp->tge );
         ( prp->s[34] ) = ( dlp->myr );
         ( prp->s[35] ) = ( dlp->tgm );
         ( prp->s[36] ) = ( dlp->hc );
         ( prp->s[37] ) = ( dlp->dhc );
         ( prp->s[38] ) = ( dlp->md );
         ( prp->s[39] ) = ( dlp->cv );
         ( prp->s[40] ) = ( dlp->ny );
         ( prp->s[41] ) = ( dlp->dny );
         ( prp->s[42] ) = ( dlp->pr );

         ( thp->nyd ) = ( dlp->ny );
         ( thp->dnd ) = ( dlp->dny );
         ( thp->pnd ) = ( dlp->pr );
         ( thp->dpd ) = ( dlp->dpr );

         cpypar( 32, "Relative_permittivity,_dielectric", depdt );
         cpypar( 33, "Dielectric_loss_factor", depdt );
         cpypar( 34, "Relative_permeability,_dielectric", depdt );
         cpypar( 35, "Magnetic_loss_factor", depdt );
         cpypar( 36, "Heat_conductivity,_dielectric", depdt );
         cpypar( 37, "Temperature_coefficient_of_the_latter", depdt );
         cpypar( 38, "Density,_dielectric", depdt );
         cpypar( 39, "Heat_capacity_[cp],_dielectric", depdt );
         cpypar( 40, "Cinematic_viscosity,_dielectric", depdt );
         cpypar( 41, "Temperature_coefficient_of_the_latter", depdt );
         cpypar( 42, "Prandtl_number,_dielectric", depdt );
      }
      else /* ELSE */
      {
         strcpy(( map->shd ), "ELSE" );

         if ((( opp->n[3] ) == ONE )        /* fluid dielectric */
           &&((( prp->s[40] ) < 1.e-217 )   /* unspecfd cinematic viscsty */
            ||( 1.e+217 < ( prp->s[40] )))) /* infinite cinematic viscsty */
         {
            strcpy(( dlp->shd ), "AIR" );  /* replace solid dielectric */
            strcpy(( dlp->del ), sstrng ); /* by air parameters */
/*............................................................................*/
            ( dlp ) = dielct ( dlp );     /*                    */
/*....................................................*/
            strcpy(( map->shd ), ( dlp->shd ));

            ( prp->s[32] ) = ( dlp->epr );
            ( prp->s[33] ) = ( dlp->tge );
            ( prp->s[34] ) = ( dlp->myr );
            ( prp->s[35] ) = ( dlp->tgm );
            ( prp->s[36] ) = ( dlp->hc );
            ( prp->s[37] ) = ( dlp->dhc );
            ( prp->s[38] ) = ( dlp->md );
            ( prp->s[39] ) = ( dlp->cv );
            ( prp->s[40] ) = ( dlp->ny );
	    ( prp->s[41] ) = ( dlp->dny );
            ( prp->s[42] ) = ( dlp->pr );

            ( thp->nyd ) = ( dlp->ny );
            ( thp->dnd ) = ( dlp->dny );
            ( thp->pnd ) = ( dlp->pr );
            ( thp->dpd ) = ( dlp->dpr );

	    cpypar( 32, "Relative_permittivity,_dielectric", depdt );
	    cpypar( 33, "Dielectric_loss_factor", depdt );
            cpypar( 34, "Relative_permeability,_dielectric", depdt );
	    cpypar( 35, "Magnetic_loss_factor", depdt );
	    cpypar( 36, "Heat_conductivity,_dielectric", depdt );
	    cpypar( 37, "Temperature_coefficient_of_the_latter", depdt );
	    cpypar( 38, "Density,_dielectric", depdt );
	    cpypar( 39, "Heat_capacity_[cp],_dielectric", depdt );
	    cpypar( 40, "Cinematic_viscosity,_dielectric", depdt );
	    cpypar( 41, "Temperature_coefficient_of_the_latter", depdt );
	    cpypar( 42, "Prandtl_number,_dielectric", depdt );
         }
         else
         {
            cpypar( 32, "Relative_permittivity,_dielectric", "epsilon-r" );
            cpypar( 33, "Dielectric_loss_factor", "'tangent_delta'" );
            cpypar( 34, "Relative_permeability,_dielectric", "my-r" );
            cpypar( 35, "Magnetic_loss_factor", "'tangent_delta'" );
            cpypar( 36, "Heat_conductivity,_dielectric", "W/(K*m)" );
            cpypar( 37, "Temperature_coefficient_of_the_latter", "1/K" );
            cpypar( 38, "Density,_dielectric", "Kg/m^3" );
            cpypar( 39, "Heat_capacity_[cp],_dielectric", "J/(Kg*K)" );
            cpypar( 40, "Cinematic_viscosity,_dielectric", "m^2/s" );
            cpypar( 41, "Temperature_coefficient_of_the_latter", "1/K" );
            cpypar( 42, "Prandtl_number,_dielectric", dmlss );

            ( thp->nyd ) = ( prp->s[40] );
            ( thp->dnd ) = ( prp->s[41] );
            ( thp->pnd ) = ( prp->s[42] );
            ( thp->dpd ) = ZERO;
         };
      };
/*............................................................................*/
/* forced convection in waveguide: */

      if (( opp->n[3] ) == ONE ) /* gas [ or any fluid ] dielectric */
      {
         ( thp->prd ) = ( prp->s[8] ); /* wave_guide internal pressure */

         if (( opp->n[4] ) != null )
         {
/* mean temperature: */
            temp = ( prp->s[54] );
/* dhyd = hydraulic diameter 4*F/S = characteristic length: */
            dhyd = 2.*( prp->s[12] )*( prp->s[13] );
            dhyd /= (( prp->s[12] )+( prp->s[13] ));
/* effective cooling length: */
            lsec = ( prp->s[64] );
/* cinematic viscosity: */
            ny = ( thp->nyd ) + temp*( thp->dnd );
            ny /= ( prp->s[8] ); /* divided through pressure */
/* Prandtl number: */
            pn = ( thp->pnd ) + temp*( thp->dpd );
/* cross section: */
            ss = ( prp->s[12] )*( prp->s[13] );
/* flow velocity: */
            ve = 1.0e-03*( prp->s[55] )/( 60.*ss );
/* Reynolds number: */
            re = ve*dhyd/ny;

/*............................................................................*/
# if HLS_NUSSELT == 1
            if (( opp->n[4] ) == ONE )
            {
/*............................................................................*/
               nu = nusselt( re, pn, ( dhyd/lsec ), "hausen" );
/*............................................................................*/
            }
	    else if (( opp->n[4] ) == TWO )
               nu = ( prp->s[56] );
            else
               nu = ( prp->s[56] );
# else
            nu = ( prp->s[56] );
# endif
/*............................................................................*/
            ( thp->nud ) = nu;
            ( thp->red ) = re;
            ( thp->vld ) = ve;
            ( thp->tdel ) = temp + ( CELSIUS_TO_KELVIN );

            ( prp->s[56] ) = nu;
         };
      }; /* end if ( opp->n[3] ) == ONE ): gas or any fluid dielectric */
/*............................................................................*/
/* convection in environment: */

      if (( opp->n[5] ) == ONE )
      {
/*............................................................................*/
/* physical properties, environment: */

         ( thp->pre ) = ( prp->s[7] ); /* environmental pressure */
         ( thp->eme ) = ( prp->s[9] ); /* heat radiation emissivity, mantle */

         strcpy(( flp->shf ), "air" );
         strcpy(( flp->fld ), sstrng );
/*............................................................................*/
         ( flp ) = fluids ( flp );        /*                    */
/*....................................................*/
	 ( thp->hce ) = ( flp->hc );
	 ( thp->dhe ) = ( flp->dhc );
	 ( thp->mde ) = ( flp->md );
	 ( thp->nye ) = ( flp->ny );
	 ( thp->dne ) = ( flp->dny );
	 ( thp->pne ) = ( flp->pr );
	 ( thp->dpe ) = ( flp->dpr );
/*............................................................................*/
/* applying Kraussold's formula for Nusselt number */

         if (( opp->n[6] ) != null )
         {
/* mean temperature: */
            temp = ( prp->s[4] );
/* dhyd = hydraulic diameter 4*F/S = characteristic length: */
            dhyd = 2.*( prp->s[14] )*( prp->s[15] );
            dhyd /= (( prp->s[14] )+( prp->s[15] ));
/* cinematic viscosity: */
            ny = ( thp->nye ) + temp*( thp->dne );
            ny /= ( prp->s[7] ); /* divided through pressure */
/* Prandtl number: */
            pn = ( thp->pne ) + temp*( thp->dpe );
/* flow velocity: */
            ve = ( prp->s[61] ); /* flow velocity */
/* Reynolds number: */
            re = ve*dhyd/ny;
/*............................................................................*/
# if HLS_NUSSELT == 1
            if (( opp->n[6] ) == ONE )
            {
/*............................................................................*/
               nu = nusselt( re, pn, ZERO, "hilpert" );
/*............................................................................*/
            }
	    else if (( opp->n[6] ) == TWO )
               nu = ( prp->s[62] );
            else
               nu = ( prp->s[62] );
# else
            nu = ( prp->s[62] );
# endif
/*............................................................................*/
            ( thp->nue ) = nu;
            ( thp->ree ) = re;
            ( thp->vle ) = ve;
            ( thp->tenv ) = temp + ( CELSIUS_TO_KELVIN );

            ( prp->s[62] ) = nu;
         };
      }; /* end if ( opp->n[5] != null ) */
/*............................................................................*/
   } /* end if ( opp->n[1] == null ): rectangular waveguide */
   else /* if (( opp->n[1] ) == ONE ): coaxial waveguide */
   {
/*...........................................................................*/
/* absent convection in conductors */

      if (( opp->n[3] ) == null )
      {
         cpypar( 8, "Pressure_in_waveguide", inact );
         cpypar( 10, "Heat_radiation_emissivity,_outer_conductor", inact );
         cpypar( 11, "Heat_radiation_emissivity,_inner_conductor", inact );
         cpypar( 40, "Cinematic_viscosity,_dielectric", inact );
         cpypar( 41, "Temperature_coefficient_of_the_latter", inact );
         cpypar( 42, "Prandtl_number,_dielectric", inact );
         cpypar( 54, "Temperature,_gas_flowing_in_waveguide", inact );
         cpypar( 55, "Flow_[liter/min],_that_gas", inact );
         cpypar( 56, "Nusselt_number,_that_gas_flow", inact );
      }
      else if (( opp->n[4] ) == null )
      {
         cpypar( 54, "Temperature,_gas_flowing_in_waveguide", inact );
         cpypar( 55, "Flow_[liter/min],_that_gas", inact );
         cpypar( 56, "Nusselt_number,_that_gas_flow", inact );
      }
      else if (( opp->n[4] ) == ONE )
      {
/*...........................................................................*/
# if HLS_NUSSELT == 1
         cpypar( 42, "Prandtl_number,_dielectric", depdt );
         cpypar( 54, "Temperature,_gas_flowing_in_w-guide", "deg_Celsius" );
         cpypar( 55, "Flow,_that_gas", "liter/min" );
         cpypar( 56, "Nusselt_number,_that_gas_flow", depdt );
# else
         cpypar( 42, "Prandtl_number,_dielectric", inact );
         cpypar( 54, "Temperature,_gas_flowing_in_w-guide", "deg_Celsius" );
         cpypar( 55, "Flow_[liter/min],_that_gas", inact );
         cpypar( 56, "Nusselt_number,_that_gas_flow", dmlss );
# endif
/*...........................................................................*/
      }
      else if (( opp->n[4] ) == TWO )
      {
         cpypar( 42, "Prandtl_number,_dielectric", inact );
         cpypar( 54, "Temperature,_gas_flowing_in_w-guide", "deg_Celsius" );
         cpypar( 55, "Flow_[liter/min],_that_gas", inact );
         cpypar( 56, "Nusselt_number,_that_gas_flow", dmlss );
      };
/*............................................................................*/
/* cooling pipe: */

      if (( opp->n[7] ) == null )
      {
         cpypar( 47, "Heat_conductivity,_cooling_fluid", inact );
         cpypar( 48, "Temperature_coefficient_of_the_latter", inact );
         cpypar( 49, "Density,_cooling_fluid", inact );
         cpypar( 50, "Heat_capacity_[cp],_cooling_fluid", inact );
         cpypar( 51, "Cinematic_viscosity,_cooling_fluid", inact );
         cpypar( 52, "Temperature_coefficient_of_the_latter", inact );
         cpypar( 53, "Prandtl_number,_cooling_fluid", inact );
         cpypar( 57, "Temperature,_cooling_fluid", inact );
         cpypar( 58, "Presssure,_cooling_fluid", inact );
         cpypar( 59, "Flow_[liter/min],_cooling_fluid", inact );
         cpypar( 60, "Nusselt_number,_that_fluid_flow", inact );
      }
      else if (( opp->n[7] ) == ONE )
      {
/*...........................................................................*/
# if HLS_NUSSELT == 1
         cpypar( 53, "Prandtl_number,_cooling_fluid", depdt );
         cpypar( 57, "Temperature,_cooling_fluid", "deg_Celsius" );
         cpypar( 58, "Presssure,_cooling_fluid", "bar_absolute" );
         cpypar( 59, "Flow,_cooling_fluid", "liter/min" );
         cpypar( 60, "Nusselt_number,_that_fluid_flow", depdt );
# else
         cpypar( 53, "Prandtl_number,_cooling_fluid", inact );
         cpypar( 57, "Temperature,_cooling_fluid", "deg_Celsius" );
         cpypar( 58, "Presssure,_cooling_fluid", inact );
         cpypar( 59, "Flow_[liter/min],_cooling_fluid", inact );
         cpypar( 60, "Nusselt_number,_that_fluid_flow", dmlss );
# endif
/*...........................................................................*/
      }
      else if (( opp->n[7] ) == TWO )
      {
         cpypar( 53, "Prandtl_number,_cooling_fluid", inact );
         cpypar( 57, "Temperature,_cooling_fluid", "deg_Celsius" );
         cpypar( 58, "Presssure,_cooling_fluid", inact );
         cpypar( 59, "Flow_[liter/min],_cooling_fluid", inact );
         cpypar( 60, "Nusselt_number,_that_fluid_flow", dmlss );
      };
/*...........................................................................*/
/* each of the following options implies some others: */

      if (( opp->n[2] ) == ONE ) /* given [ fixed ] inner condctr temperature */
      {
         cpypar( 3, "Transmitted_CW_power", "initial_value;_Watts" );
         cpypar( 6, "Temperature,_inner_conductor", "deg_Celsius" );
      }
      else /* given [ fixed ] CW power */
      {
         cpypar( 3, "Transmitted_CW_power", "Watts" );
         cpypar( 6, "Temperature,_inner_conductor", "initial_value;_C" );
      };
/*...........................................................................*/
/* fixed temperatures on one side */

      cpypar( 65, "Waveguide_length", inact );
      cpypar( 66, "Fixed_side_temperature,_mantle", inact );
      cpypar( 67, "Fixed_heat_outflow_on_this_side,_mantle", inact );
      cpypar( 68, "Fixed_heat_inflow_[opposite_side],_mantle", inact );
      cpypar( 69, "Fixed_side_temperature,_pipe", inact );
      cpypar( 70, "Fixed_heat_outflow_on_this_side,_pipe", inact );
      cpypar( 71, "Fixed_heat_inflow_[opposite_side],_pipe", inact );
      
      ii = ( opp->n[8] );
      switch( ii )
      {
        default:
         break;

        case 1:
         ( dmp->lwgd ) = ( prp->s[65] ); /* waveguide length */
         cpypar( 65, "Waveguide_length", "m" );
         cpypar( 66, "Fixed_side_temperature,_mantle", "deg_Celsius" );
         cpypar( 67, "Heat_outflow_on_this_side,_mantle", depdt );
         cpypar( 68, "Fixed_heat_inflow_[opposite_side],_mantle", "W" );
         break;

        case 2:
         ( dmp->lwgd ) = ( prp->s[65] ); /* waveguide length */
         cpypar( 65, "Waveguide_length", "m" );
         cpypar( 69, "Fixed_side_temperature,_pipe", "deg_Celsius" );
         cpypar( 70, "Heat_outflow_on_this_side,_pipe", depdt );
         cpypar( 71, "Fixed_heat_inflow_[opposite_side],_pipe", "W" );
         break;

        case 3:
         ( dmp->lwgd ) = ( prp->s[65] ); /* waveguide length */
         cpypar( 65, "Waveguide_length", "m" );
         cpypar( 66, "Fixed_side_temperature,_mantle", "deg_Celsius" );
         cpypar( 67, "Heat_outflow_on_this_side,_mantle", depdt );
         cpypar( 68, "Fixed_heat_inflow_[opposite_side],_mantle", "W" );
         cpypar( 69, "Fixed_side_temperature,_pipe", "deg_Celsius" );
         cpypar( 70, "Heat_outflow_on_this_side,_pipe", depdt );
         cpypar( 71, "Fixed_heat_inflow_[opposite_side],_pipe", "W" );
         break;
      };
/*............................................................................*/
/* fixed heat outflow on one side */

      ii = ( opp->n[9] );
      switch( ii )
      {
        default:
         break;

        case 1:
         ( dmp->lwgd ) = ( prp->s[65] ); /* waveguide length */
         cpypar( 65, "Waveguide_length", "m" );
         cpypar( 66, "Side_temperature,_mantle", depdt );
         cpypar( 67, "Fixed_heat_outflow_on_this_side,_mantle", "W" );
         cpypar( 68, "Fixed_heat_inflow_[opposite_side],_mantle", "W" );
         break;

        case 2:
         ( dmp->lwgd ) = ( prp->s[65] ); /* waveguide length */
         cpypar( 65, "Waveguide_length", "m" );
         cpypar( 69, "Side_temperature,_pipe", depdt );
         cpypar( 70, "Fixed_heat_outflow_on_this_side,_pipe", "W" );
         cpypar( 71, "Fixed_heat_inflow_[opposite_side],_pipe", "W" );
         break;

        case 3:
         ( dmp->lwgd ) = ( prp->s[65] ); /* waveguide length */
         cpypar( 65, "Waveguide_length", "m" );
         cpypar( 66, "Side_temperature,_mantle", depdt );
         cpypar( 67, "Fixed_heat_outflow_on_this_side,_mantle", "W" );
         cpypar( 68, "Fixed_heat_inflow_[opposite_side],_mantle", "W" );
         cpypar( 69, "Side_temperature,_pipe", depdt );
         cpypar( 70, "Fixed_heat_outflow_on_this_side,_pipe", "W" );
         cpypar( 71, "Fixed_heat_inflow_[opposite_side],_pipe", "W" );
         break;
      }; /* end, switch */
/*............................................................................*/
/* fixing any mantle temperature disconnects from environment: */

      if (( opp->n[5] ) == null )
      {
	 cpypar( 4, "Temperature,_environment", inact );
	 cpypar( 5, "Temperature,_waveguide_mantle", "deg_Celsius" );
	 cpypar( 9, "Heat_radiation_emissivity,_mantle", inact );
	 cpypar( 61, "Flow_velocity_[m/s],_air_in_environment", inact );
	 cpypar( 62, "Nusselt_number,_that_air_flow", inact );
	 
         cpypar( 66, "Side_temperature,_mantle", depdt );
      }
      else /* if (( opp->n[5] ) == ONE ) - heat exchange with environment */
      {
         cpypar( 5, "Temperature,_waveguide_mantle", depdt );

         if (( opp->n[6] ) == null ) /* free convection */
         {
	    cpypar( 61, "Flow_velocity_[m/s],_air_in_environment", inact );
	    cpypar( 62, "Nusselt_number,_that_air_flow", inact );
         }
         else if (( opp->n[6] ) == ONE )
         {
/*...........................................................................*/
# if HLS_NUSSELT == 1
            cpypar( 4, "Temperature,_environment", "deg_Celsius" );
            cpypar( 61, "Flow_velocity,_air_in_environment", "m/s" );
            cpypar( 62, "Nusselt_number,_that_air_flow", depdt );
# else
            cpypar( 4, "Temperature,_environment", "deg_Celsius" );
	    cpypar( 61, "Flow_velocity_[m/s],_air_in_environment", inact );
            cpypar( 62, "Nusselt_number,_that_air_flow", dmlss );
# endif
/*...........................................................................*/
         }
         else if (( opp->n[6] ) == TWO )
         {
            cpypar( 4, "Temperature,_environment", "deg_Celsius" );
	    cpypar( 61, "Flow_velocity_[m/s],_air_in_environment", inact );
            cpypar( 62, "Nusselt_number,_that_air_flow", dmlss );
         }
         else
         {
            cpypar( 4, "Temperature,_environment", "deg_Celsius" );
         };
      }; /* end if (( opp->n[5] ) != null ) */
/*...........................................................................*/
/* physical properties, outer conductor metal: */

      if (( null != strcmp(( map->sho ), "else" ))
	&&( null != strcmp(( map->sho ), "Else" ))
	&&( null != strcmp(( map->sho ), "ELSE" )))
      {
	 strcpy(( mtp->shm ), ( map->sho ));
	 strcpy(( mtp->met ), sstrng );
/*............................................................................*/
	 ( mtp ) = metals ( mtp );     /*                       */
/*.................................................*/
	 strcpy(( map->sho ), ( mtp->shm ));

	 ( prp->s[25] ) = ( mtp->er );
	 ( prp->s[26] ) = ( mtp->der );
	 ( prp->s[27] ) = ( mtp->myr );
	 ( prp->s[28] ) = ( mtp->hc );
	 ( prp->s[29] ) = ( mtp->dhc );
	 ( prp->s[30] ) = ( mtp->md );
	 ( prp->s[31] ) = ( mtp->cv );

	 cpypar( 25, "Electric_resistivity,_outer_conductor", depdt );
	 cpypar( 26, "Temperature_coefficient_of_the_latter", depdt );
	 cpypar( 27, "Relative_permeability,_outer_conductor", depdt );
	 cpypar( 28, "Heat_conductivity,_outer_conductor", depdt );
	 cpypar( 29, "Temperature_coefficient_of_the_latter", depdt );
	 cpypar( 30, "Density,_outer_conductor", depdt );
	 cpypar( 31, "Heat_capacity_[cp],_outer_conductor", depdt );
      }
      else
      {
	 strcpy(( map->sho ), "ELSE" );

	 cpypar( 25, "Electric_resistivity,_outer_conductor", "Ohm*m" );
	 cpypar( 26, "Temperature_coefficient_of_the_latter", "1/K" );
	 cpypar( 27, "Relative_permeability,_outer_conductor", dmlss );
	 cpypar( 28, "Heat_conductivity,_outer_conductor", "W/(m*K)" );
	 cpypar( 29, "Temperature_coefficient_of_the_latter", "1/K" );
	 cpypar( 30, "Density,_outer_conductor", "Kg/m^3" );
	 cpypar( 31, "Heat_capacity_[cp],_outer_conductor", "J/(Kg*K)" );
      };
/*............................................................................*/
/* physical properties, inner conductor metal: */

      if (( null != strcmp(( map->shi ), "else" ))
	&&( null != strcmp(( map->shi ), "Else" ))
	&&( null != strcmp(( map->shi ), "ELSE" )))
      {
	 strcpy(( mtp->shm ), ( map->shi ));
	 strcpy(( mtp->met ), sstrng );
/*............................................................................*/
	 ( mtp ) = metals ( mtp );                   /*                       */
/*.................................................*/
	 strcpy(( map->shi ), ( mtp->shm ));

	 ( prp->s[18] ) = ( mtp->er );
	 ( prp->s[19] ) = ( mtp->der );
	 ( prp->s[20] ) = ( mtp->myr );
	 ( prp->s[21] ) = ( mtp->hc );
	 ( prp->s[22] ) = ( mtp->dhc );
	 ( prp->s[23] ) = ( mtp->md );
	 ( prp->s[24] ) = ( mtp->cv );

	 cpypar( 18, "Electric_resistivity,_inner_conductor", depdt );
	 cpypar( 19, "Temperature_coefficient_of_the_latter", depdt );
	 cpypar( 20, "Relative_permeability,_inner_conductor", depdt );
	 cpypar( 21, "Heat_conductivity,_inner_conductor", depdt );
	 cpypar( 22, "Temperature_coefficient_of_the_latter", depdt );
	 cpypar( 23, "Density,_inner_conductor", depdt );
	 cpypar( 24, "Heat_capacity_[cp],_inner_conductor", depdt );
      }
      else
      {
	 strcpy(( map->shi ), "ELSE" );

	 cpypar( 18, "Electric_resistivity,_inner_conductor", "Ohm*m" );
	 cpypar( 19, "Temperature_coefficient_of_the_latter", "1/K" );
	 cpypar( 20, "Relative_permeability,_inner_conductor", dmlss );
	 cpypar( 21, "Heat_conductivity,_inner_conductor", "W/(m*K)" );
	 cpypar( 22, "Temperature_coefficient_of_the_latter", "1/K" );
	 cpypar( 23, "Density,_inner_conductor", "Kg/m^3" );
	 cpypar( 24, "Heat_capacity_[cp],_inner_conductor", "J/(Kg*K)" );
      };
/*............................................................................*/
/* physical properties, dielectric: */

      if (( null != strcmp(( map->shd ), "else" ))
        &&( null != strcmp(( map->shd ), "Else" ))
        &&( null != strcmp(( map->shd ), "ELSE" )))
      {
         strcpy(( dlp->shd ), ( map->shd )); 
         strcpy(( dlp->del ), sstrng ); 
/*............................................................................*/
         ( dlp ) = dielct ( dlp );     /*                       */
/*.................................................*/
         if ((( opp->n[3] ) == ONE )     /* fluid dielectric */
           &&((( dlp->ny ) < 1.e-217 )   /* unspecfd cinematic viscsty */
            ||( 1.e+217 < ( dlp->ny )))) /* infinite cinematic viscsty */
         {
            strcpy(( dlp->shd ), "AIR" );  /* replace solid dielectric */
            strcpy(( dlp->del ), sstrng ); /* by air parameters */
/*............................................................................*/
            ( dlp ) = dielct ( dlp );     /*                    */
/*....................................................*/
         };
         strcpy(( map->shd ), ( dlp->shd ));

         ( prp->s[32] ) = ( dlp->epr );
         ( prp->s[33] ) = ( dlp->tge );
         ( prp->s[34] ) = ( dlp->myr );
         ( prp->s[35] ) = ( dlp->tgm );
         ( prp->s[36] ) = ( dlp->hc );
         ( prp->s[37] ) = ( dlp->dhc );
         ( prp->s[38] ) = ( dlp->md );
         ( prp->s[39] ) = ( dlp->cv );
         ( prp->s[40] ) = ( dlp->ny );
         ( prp->s[41] ) = ( dlp->dny );
         ( prp->s[42] ) = ( dlp->pr );

         ( thp->nyd ) = ( dlp->ny );
         ( thp->dnd ) = ( dlp->dny );
         ( thp->pnd ) = ( dlp->pr );
         ( thp->dpd ) = ( dlp->dpr );

         cpypar( 32, "Relative_permittivity,_dielectric", depdt );
         cpypar( 33, "Dielectric_loss_factor", depdt );
         cpypar( 34, "Relative_permeability,_dielectric", depdt );
         cpypar( 35, "Magnetic_loss_factor", depdt );
         cpypar( 36, "Heat_conductivity,_dielectric", depdt );
         cpypar( 37, "Temperature_coefficient_of_the_latteri", depdt );
         cpypar( 38, "Density,_dielectric", depdt );
         cpypar( 39, "Heat_capacity_[cp],_dielectric", depdt );
         cpypar( 40, "Cinematic_viscosity,_dielectric", depdt );
         cpypar( 41, "Temperature_coefficient_of_the_latter", depdt );
         cpypar( 42, "Prandtl_number,_dielectric", depdt );
      }
      else /* ELSE */
      {
         strcpy(( map->shd ), "ELSE" );

         if ((( opp->n[3] ) == ONE )        /* fluid dielectric */
           &&((( prp->s[40] ) < 1.e-217 )   /* unspecfd cinematic viscsty */
            ||( 1.e+217 < ( prp->s[40] )))) /* infinite cinematic viscsty */
         {
            strcpy(( dlp->shd ), "AIR" );  /* replace solid dielectric */
            strcpy(( dlp->del ), sstrng ); /* by air parameters */
/*............................................................................*/
            ( dlp ) = dielct ( dlp );                   /*                    */
/*....................................................*/
            strcpy(( map->shd ), ( dlp->shd ));

            ( prp->s[32] ) = ( dlp->epr );
            ( prp->s[33] ) = ( dlp->tge );
            ( prp->s[34] ) = ( dlp->myr );
            ( prp->s[35] ) = ( dlp->tgm );
            ( prp->s[36] ) = ( dlp->hc );
            ( prp->s[37] ) = ( dlp->dhc );
            ( prp->s[38] ) = ( dlp->md );
            ( prp->s[39] ) = ( dlp->cv );
            ( prp->s[40] ) = ( dlp->ny );
	    ( prp->s[41] ) = ( dlp->dny );
            ( prp->s[42] ) = ( dlp->pr );

            ( thp->nyd ) = ( dlp->ny );
            ( thp->dnd ) = ( dlp->dny );
            ( thp->pnd ) = ( dlp->pr );
            ( thp->dpd ) = ( dlp->dpr );

            cpypar( 32, "Relative_permittivity,_dielectric", depdt );
            cpypar( 33, "Dielectric_loss_factor", depdt );
            cpypar( 34, "Relative_permeability,_dielectric", depdt );
            cpypar( 35, "Magnetic_loss_factor", depdt );
            cpypar( 36, "Heat_conductivity,_dielectric", depdt );
            cpypar( 37, "Temperature_coefficient_of_the_latter", depdt );
            cpypar( 38, "Density,_dielectric", depdt );
            cpypar( 39, "Heat_capacity_[cp],_dielectric", depdt );
            cpypar( 40, "Cinematic_viscosity,_dielectric", depdt );
            cpypar( 41, "Temperature_coefficient_of_the_latter", depdt );
            cpypar( 42, "Prandtl_number,_dielectric", depdt );
         }
         else
         {
            cpypar( 32, "Relative_permittivity,_dielectric", "epsilon-r" );
            cpypar( 33, "Dielectric_loss_factor", "'tangent_delta'" );
            cpypar( 34, "Relative_permeability,_dielectric", "my-r" );
            cpypar( 35, "Magnetic_loss_factor", "'tangent_delta'" );
            cpypar( 36, "Heat_conductivity,_dielectric", "W/(K*m)" );
            cpypar( 37, "Temperature_coefficient_of_the_latter", "1/K" );
            cpypar( 38, "Density,_dielectric", depdt );
            cpypar( 39, "Heat_capacity_[cp],_dielectric", "J/(Kg*K)" );
            cpypar( 40, "Cinematic_viscosity,_dielectric", "m^2/s" );
            cpypar( 41, "Temperature_coefficient_of_the_latter", "1/K" );
            cpypar( 42, "Prandtl_number,_dielectric", dmlss );

            ( thp->nyd ) = ( prp->s[40] );
            ( thp->dnd ) = ( prp->s[41] );
            ( thp->pnd ) = ( prp->s[42] );
            ( thp->dpd ) = ZERO;
         };
      };
/*............................................................................*/
/* forced convection between conductors: */

      if (( opp->n[3] ) == ONE ) /* gas [ or any fluid ] dielectric */
      {
         ( thp->prd ) = ( prp->s[8] );  /* wave guide internal pressure */
         ( thp->emo ) = ( prp->s[10] ); /* heat rad emissivity, outer cd */
         ( thp->emi ) = ( prp->s[11] ); /* heat rad emissivity, inner cd */

         if (( opp->n[4] ) != null )
         {
/* mean temperature: */
            temp = ( prp->s[54] );
/* dhyd = hydraulic diameter 4*F/S = characteristic length: */
	    dhyd = ( prp->s[12] ) - ( prp->s[13] );
/* effective cooling length */
	    lsec = ( prp->s[63] );
/* cinematic viscosity: */
            ny = ( thp->nyd ) + temp*( thp->dnd );
            ny /= ( prp->s[8] ); /* divided through pressure */
/* Prandtl number: */
            pn = ( thp->pnd ) + temp*( thp->dpd );
/* cross section: */
	    ss = .25*PI*( pow(( prp->s[12] ), 2. ) - pow(( prp->s[13] ), 2. ));
/* flow velocity: */
            ve = 1.0e-03*( prp->s[55] )/( 60.*ss );
/* Reynolds number: */
            re = ve*dhyd/ny;
/*............................................................................*/
# if HLS_NUSSELT == 1

            if (( opp->n[4] ) == ONE )
            {
/*............................................................................*/
               if ( re < HLS_KRSCHB )
                  nu = nusselt( re, pn, ( dhyd/lsec ), "hausen" );
               else if (( HLS_KRSCHB <= re )
                      &&( re < HLS_RETURB ))
               {
                  ss = ( HLS_RETURB - re )/( HLS_RETURB - HLS_KRSCHB );
		  nu = ss*nusselt( re, pn, ( dhyd/lsec ), "hausen" );
		  nu += (( 1.- ss )* \
			nusselt( re, pn, ( dhyd/lsec ), "kirschbaum" ));
               }
               else /* turbulent */
                  nu = nusselt( re, pn, ( dhyd/lsec ), "kirschbaum" );
            }
	    else if (( opp->n[4] ) == TWO )
               nu = ( prp->s[56] );
            else
               nu = ( prp->s[56] );
# else
            nu = ( prp->s[56] );
# endif
/*............................................................................*/
            ( thp->nud ) = nu;
            ( thp->red ) = re;
            ( thp->vld ) = ve;
            ( thp->tdel ) = temp + ( CELSIUS_TO_KELVIN );

            ( prp->s[56] ) = nu;
         };
      }; /* end if ( opp->n[3] ) == ONE ): gas or any fluid dielectric */
/*............................................................................*/
/* convection in environmant: */

      if (( opp->n[5] ) == ONE )
      {
/*............................................................................*/
/* physical properties, environment: */

         ( thp->pre ) = ( prp->s[7] ); /* environmental pressure */
         ( thp->eme ) = ( prp->s[9] ); /* heat radiation emissivity, mantle */

         strcpy(( flp->shf ), "air" );
         strcpy(( flp->fld ), sstrng );
/*............................................................................*/
         ( flp ) = fluids ( flp );        /*                    */
/*....................................................*/
	 ( thp->hce ) = ( flp->hc );
	 ( thp->dhe ) = ( flp->dhc );
	 ( thp->mde ) = ( flp->md );
	 ( thp->nye ) = ( flp->ny );
	 ( thp->dne ) = ( flp->dny );
	 ( thp->pne ) = ( flp->pr );
	 ( thp->dpe ) = ( flp->dpr );
/*............................................................................*/
/* applying Kraussold's formula for Nusselt number */

         if (( opp->n[6] ) != null )
         {
/* mean temperature */
            temp = ( prp->s[4] );
/* dhyd = hydraulic diameter 4*F/S = diameter = characteristic length: */
            dhyd = ( prp->s[14] );
/* cinematic viscosity: */
            ny = ( thp->nye ) + temp*( thp->dne );
            ny /= ( prp->s[7] ); /* divided through pressure */
/* Prandtl number: */
            pn = ( thp->pne ) + temp*( thp->dpe );
/* flow velocity: */
            ve = ( prp->s[61] ); /* flow velocity */
/* Reynolds number: */
            re = ve*dhyd/ny;
/*............................................................................*/
# if HLS_NUSSELT == 1
            if (( opp->n[6] ) == ONE )
            {
/*............................................................................*/
               nu = nusselt( re, pn, ZERO, "hilpert" );
/*...........................................................................*/
            }
	    else if (( opp->n[6] ) == TWO )
               nu = ( prp->s[62] );
            else
               nu = ( prp->s[62] );
# else
            nu = ( prp->s[62] );
# endif
/*............................................................................*/
            ( thp->nue ) = nu;
            ( thp->ree ) = re;
            ( thp->vle ) = ve;
            ( thp->tenv ) = temp + ( CELSIUS_TO_KELVIN );
	    
            ( prp->s[62] ) = nu;
         };
      } /* end if ( opp->n[5] != null ) */
/*............................................................................*/
/* physical properties, inner conductor pipe material: */

      if (( null != strcmp(( map->shc ), "else" ))
        &&( null != strcmp(( map->shc ), "Else" ))
        &&( null != strcmp(( map->shc ), "ELSE" ))
        &&( null != strcmp(( map->shc ), "void" ))
        &&( null != strcmp(( map->shc ), "Void" ))
        &&( null != strcmp(( map->shc ), "VOID" )))
      {
         strcpy(( slp->shs ), ( map->shc ));
         strcpy(( slp->sld ), sstrng );
/*............................................................................*/
         ( slp ) = solids ( slp );        /*                    */
/*....................................................*/
         strcpy(( map->shc ), ( slp->shs ));

         ( prp->s[45] ) = ( slp->hc );
         ( prp->s[46] ) = ( slp->dhc );

         cpypar( 45, "Heat_conductivity,_pipe", depdt );
         cpypar( 46, "Temperature_coefficient_of_the_latter", depdt );
      }
      else if (( null == strcmp(( map->shc ), "else" ))
             ||( null == strcmp(( map->shc ), "Else" ))
             ||( null == strcmp(( map->shc ), "ELSE" )))
      {
         strcpy(( map->shc ), "ELSE" );

         cpypar( 45, "Heat_conductivity,_pipe", "W/(K*m)" );
         cpypar( 46, "Temperature_coefficient_of_the_latter", "1/K" );
      }
      else
      {
         strcpy(( map->shc ), "VOID" );

         cpypar( 45, "Heat_conductivity,_cooling_pipe", inact );
         cpypar( 46, "Temperature_coefficient_of_the_latter", inact );
         
         if (( opp->n[7] ) == ONE ) /* additional inn con cooling */
            par.s[15] = par.s[13]; /* void cooling tube */
      };
/*............................................................................*/
/* additional inner conductor cooling; physical properties, cooling fluid: */

      if (( opp->n[7] ) == ONE )
      {
         if (( null != strcmp(( map->shf ), "else" ))
	   &&( null != strcmp(( map->shf ), "Else" ))
           &&( null != strcmp(( map->shf ), "ELSE" ))
           &&( null != strcmp(( map->shf ), "void" ))
           &&( null != strcmp(( map->shf ), "Void" ))
           &&( null != strcmp(( map->shf ), "VOID" )))
         {
            strcpy(( flp->shf ), ( map->shf )); 
            strcpy(( flp->fld ), sstrng ); 
/*............................................................................*/
            ( flp ) = fluids ( flp );     /*                    */
/*....................................................*/
            strcpy(( map->shf ), ( flp->shf ));

            ( prp->s[47] ) = ( flp->hc );
	    ( prp->s[48] ) = ( flp->dhc );
            ( prp->s[49] ) = ( flp->md );
            ( prp->s[50] ) = ( flp->cv );
            ( prp->s[51] ) = ( flp->ny );
            ( prp->s[52] ) = ( flp->dny );
            ( prp->s[53] ) = ( flp->pr );

	    ( thp->nyf ) = ( flp->ny );
	    ( thp->dnf ) = ( flp->dny );
	    ( thp->pnf ) = ( flp->pr );
	    ( thp->dpf ) = ( flp->dpr );

            cpypar( 47, "Heat_conductivity,_cooling_fluid", depdt );
            cpypar( 48, "Temperature_coefficient_of_the_latter", depdt );
            cpypar( 49, "Density,_cooling_fluid", depdt );
            cpypar( 50, "Heat_capacity_[cp],_cooling_fluid", depdt );
            cpypar( 51, "Cinematic_viscosity,_cooling_fluid", depdt );
            cpypar( 52, "Temperature_coefficient_of_the_latter", depdt );
            cpypar( 53, "Prandtl_number,_cooling_fluid", depdt );
         }
         else if (( null == strcmp(( map->shf ), "else" ))
                ||( null == strcmp(( map->shf ), "Else" ))
                ||( null == strcmp(( map->shf ), "ELSE" )))
         {
            strcpy(( map->shf ), "ELSE" );

            cpypar( 47, "Heat_conductivity,_cooling_fluid", "W/(K*m)" );
            cpypar( 48, "Temperature_coefficient_of_the_latter", "1/K" );
            cpypar( 49, "Density,_cooling_fluid", "Kg/m^3" );
            cpypar( 50, "Heat_capacity_[cp],_cooling_fluid", "J/(Kg*K)" );
            cpypar( 51, "Cinematic_viscosity,_cooling_fluid", "m^2/s" );
            cpypar( 52, "Temperature_coefficient_of_the_latter", "1/K" );
            cpypar( 53, "Prandtl_number,_cooling_fluid", dmlss );

	    ( thp->nyf ) = ( prp->s[51] );
	    ( thp->dnf ) = ( prp->s[52] );
	    ( thp->pnf ) = ( prp->s[53] );
            ( thp->dpf ) = ZERO;
         }
         else
         {
            strcpy(( map->shf ), "VOID" );

            cpypar( 47, "Heat_conductivity,_cooling_fluid", inact );
            cpypar( 48, "Temperature_coefficient_of_the_latter", inact );
            cpypar( 49, "Density,_cooling_fluid", inact );
            cpypar( 50, "Heat_capacity_[cp],_cooling_fluid", inact );
            cpypar( 51, "Cinematic_viscosity,_cooling_fluid", inact );
            cpypar( 52, "Temperature_coefficient_of_the_latter", inact );
            cpypar( 53, "Prandtl_number,_cooling_fluid", inact );
            cpypar( 57, "Temperature,_cooling_fluid", inact );
            cpypar( 58, "Pressure,_cooling_fluid", inact );
            cpypar( 59, "Flow_[liter/min],_cooling_fluid", inact );
            cpypar( 60, "Nusselt_number,_cooling_fluid", inact );
         };
/*............................................................................*/
/* applying Kraussold's formula for Nusselt number */
         if (( opp->n[7] ) != null )
         {
/* mean temperature */
            temp = ( prp->s[57] );
/* dhyd = hydraulic diameter 4*F/S = diameter = characteristic length: */
	    dhyd = ( prp->s[15] );
/* effective cooling length */
            lsec = ( prp->s[64] );
/* cinematic viscosity: */
            ny = ( thp->nyf ) + temp*( thp->dnf );
            ny /= ( prp->s[58] ); /* divided through pressure */
/* Prandtl number: */
            pn = ( thp->pnf ) + temp*( thp->dpf );
/* cross section: */
            ss = .25*PI*dhyd*dhyd;
/* flow velocity: */
            ve = 1.e-03*( prp->s[59] )/( 60.*ss );
/* Reynolds number: */
            re = ve*dhyd/ny;
/*............................................................................*/
# if HLS_NUSSELT == 1
            if (( opp->n[7] ) == ONE )
            {
/*............................................................................*/
               nu = nusselt( re, pn, ( dhyd/lsec ), "hausen" );
/*............................................................................*/
            }
	    else if (( opp->n[7] ) == TWO )
               nu = ( prp->s[60] );
            else
               nu = ( prp->s[60] );
# else
            nu = ( prp->s[60] );
# endif
/*............................................................................*/
            ( thp->nuf ) = nu;
            ( thp->ref ) = re;
            ( thp->vlf ) = ve;
            ( thp->tfld ) = temp + ( CELSIUS_TO_KELVIN );

            ( prp->s[60] ) = nu;
         };
      } /* end if ( opp->n[7] == ONE ) - additional inner conductor cooling */
      else
      {
         strcpy(( map->shf ), "VOID" );

         cpypar( 47, "Heat_conductivity,_cooling_fluid", inact );
         cpypar( 48, "Temperature_coefficient_of_the_latter", inact );
         cpypar( 49, "Density,_cooling_fluid", inact );
         cpypar( 50, "Heat_capacity_[cp],_cooling_fluid", inact );
         cpypar( 51, "Cinematic_viscosity,_cooling_fluid", inact );
         cpypar( 52, "Temperature_coefficient_of_the_latter", inact );
         cpypar( 53, "Prandtl_number,_cooling_fluid", inact );
         cpypar( 57, "Temperature,_cooling_fluid", inact );
         cpypar( 58, "Pressure,_cooling_fluid", inact );
         cpypar( 59, "Flow_[liter/min],_cooling_fluid", inact );
         cpypar( 60, "Nusselt_number,_cooling_fluid", inact );
      }; /* end if ( opp->n[7] == null ) */
   }; /* end if (( opp->n[1] ) == ONE ): coaxial waveguide */
/*............................................................................*/
/* physical properties, dielectric: */

   if (( null != strcmp(( map->shd ), "else" ))
     &&( null != strcmp(( map->shd ), "Else" ))
     &&( null != strcmp(( map->shd ), "ELSE" )))
   {
      strcpy(( dlp->shd ), ( map->shd )); 
      strcpy(( dlp->del ), sstrng ); 
/*............................................................................*/
      ( dlp ) = dielct ( dlp );     /*                          */
/*..............................................*/
      if ((( opp->n[3] ) == ONE )     /* fluid dielectric */
        &&((( dlp->ny ) < 1.e-217 )   /* unspecfd cinematic viscsty */
         ||( 1.e+217 < ( dlp->ny )))) /* infinite cinematic viscsty */
      {
         strcpy(( dlp->shd ), "AIR" );  /* replace solid dielectric */
         strcpy(( dlp->del ), sstrng ); /* by air parameters */
/*............................................................................*/
         ( dlp ) = dielct ( dlp );     /*                       */
/*.................................................*/
      };
      strcpy(( map->shd ), ( dlp->shd ));

      ( prp->s[32] ) = ( dlp->epr );
      ( prp->s[33] ) = ( dlp->tge );
      ( prp->s[34] ) = ( dlp->myr );
      ( prp->s[35] ) = ( dlp->tgm );
      ( prp->s[36] ) = ( dlp->hc );
      ( prp->s[37] ) = ( dlp->dhc );
      ( prp->s[38] ) = ( dlp->md );
      ( prp->s[39] ) = ( dlp->cv );
      ( prp->s[40] ) = ( dlp->ny );
      ( prp->s[41] ) = ( dlp->dny );
      ( prp->s[42] ) = ( dlp->pr );

      ( thp->nyd ) = ( dlp->ny );
      ( thp->dnd ) = ( dlp->dny );
      ( thp->pnd ) = ( dlp->pr );
      ( thp->dpd ) = ( dlp->dpr );

      cpypar( 32, "Relative_permittivity,_dielectric", depdt );
      cpypar( 33, "Dielectric_loss_factor", depdt );
      cpypar( 34, "Relative_permeability,_dielectric", depdt );
      cpypar( 35, "Magnetic_loss_factor", depdt );
      cpypar( 36, "Heat_conductivity,_dielectric", depdt );
      cpypar( 37, "Temperature_coefficient_of_the_latter", depdt );
      cpypar( 38, "Density,_dielectric", depdt );
      cpypar( 39, "Heat_capacity_[cp],_dielectric", depdt );
      cpypar( 40, "Cinematic_viscosity,_dielectric", depdt );
      cpypar( 41, "Temperature_coefficient_of_the_latter", depdt );
      cpypar( 42, "Prandtl_number,_dielectric", depdt );
   }
   else /* ELSE */
   {
      strcpy(( map->shd ), "ELSE" );

      if ((( opp->n[3] ) == ONE )        /* fluid dielectric */
        &&((( prp->s[40] ) < 1.e-217 )   /* unspecfd cinematic viscsty */
         ||( 1.e+217 < ( prp->s[40] )))) /* infinite cinematic viscsty */
      {
         strcpy(( dlp->shd ), "AIR" );  /* replace solid dielectric */
         strcpy(( dlp->del ), sstrng ); /* by air parameters */
/*............................................................................*/
         ( dlp ) = dielct ( dlp );     /*                       */
/*.................................................*/
         strcpy(( map->shd ), ( dlp->shd ));

         ( prp->s[32] ) = ( dlp->epr );
         ( prp->s[33] ) = ( dlp->tge );
         ( prp->s[34] ) = ( dlp->myr );
         ( prp->s[35] ) = ( dlp->tgm );
         ( prp->s[36] ) = ( dlp->hc );
         ( prp->s[37] ) = ( dlp->dhc );
         ( prp->s[38] ) = ( dlp->md );
         ( prp->s[39] ) = ( dlp->cv );
         ( prp->s[40] ) = ( dlp->ny );
	 ( prp->s[41] ) = ( dlp->dny );
         ( prp->s[42] ) = ( dlp->pr );
/*............................................................................*/
# if HLS_NUSSELT == 1
         ( thp->nyd ) = ( dlp->ny );
         ( thp->dnd ) = ( dlp->dny );
         ( thp->pnd ) = ( dlp->pr );
         ( thp->dpd ) = ( dlp->dpr );
# endif
/*............................................................................*/
         cpypar( 32, "Relative_permittivity,_dielectric", depdt );
         cpypar( 33, "Dielectric_loss_factor", depdt );
         cpypar( 34, "Relative_permeability,_dielectric", depdt );
         cpypar( 35, "Magnetic_loss_factor", depdt );
         cpypar( 36, "Heat_conductivity,_dielectric", depdt );
         cpypar( 37, "Temperature_coefficient_of_the_latter", depdt );
         cpypar( 38, "Density,_dielectric", depdt );
         cpypar( 39, "Heat_capacity_[cp],_dielectric", depdt );
         cpypar( 40, "Cinematic_viscosity,_dielectric", depdt );
         cpypar( 41, "Temperature_coefficient_of_the_latter", depdt );
         cpypar( 42, "Prandtl_number,_dielectric", depdt );
      }
      else
      {
         cpypar( 32, "Relative_permittivity,_dielectric", "epsilon-r" );
         cpypar( 33, "Dielectric_loss_factor", "'tangent_delta'" );
         cpypar( 34, "Relative_permeability,_dielectric", "my-r" );
         cpypar( 35, "Magnetic_loss_factor", "'tangent_delta'" );
         cpypar( 36, "Heat_conductivity,_dielectric", "W/(K*m)" );
         cpypar( 37, "Temperature_coefficient_of_the_latter", "1/K" );
         cpypar( 38, "Density,_dielectric", "Kg/m^3" );
         cpypar( 39, "Heat_capacity_[cp],_dielectric", "J/(Kg*K)" );
         cpypar( 40, "Cinematic_viscosity,_dielectric", "m^2/s" );
         cpypar( 41, "Temperature_coefficient_of_the_latter", "1/K" );
         cpypar( 42, "Prandtl_number,_dielectric", dmlss );
/*............................................................................*/
# if HLS_NUSSELT == 1
         ( thp->nyd ) = ( prp->s[40] );
         ( thp->dnd ) = ( prp->s[41] );
         ( thp->pnd ) = ( prp->s[42] );
         ( thp->dpd ) = ZERO;
# endif
/*............................................................................*/
      };
   }; /* end, material properties 'ELSE' */
/*............................................................................*/
   /* physical properties, mantle material: */

   if (( null != strcmp(( map->shj ), "else" ))
     &&( null != strcmp(( map->shj ), "Else" ))
     &&( null != strcmp(( map->shj ), "ELSE" ))
     &&( null != strcmp(( map->shj ), "void" ))
     &&( null != strcmp(( map->shj ), "Void" ))
     &&( null != strcmp(( map->shj ), "VOID" )))
   {
      strcpy(( slp->shs ), ( map->shj ));
      strcpy(( slp->sld ), sstrng );
/*............................................................................*/
      ( slp ) = solids ( slp );     /*                          */
/*..............................................*/
      strcpy(( map->shj ), ( slp->shs ));

      ( prp->s[43] ) = ( slp->hc );
      ( prp->s[44] ) = ( slp->dhc );

      cpypar( 43, "Heat_conductivity,_mantle", depdt );
      cpypar( 44, "Temperature_coefficient_of_the_latter", depdt );
   }
   else if (( null == strcmp(( map->shj ), "else" ))
          ||( null == strcmp(( map->shj ), "Else" ))
	  ||( null == strcmp(( map->shj ), "ELSE" )))
   {
      strcpy(( map->shj ), "ELSE" );

      cpypar( 43, "Heat_conductivity,_mantle", "W/(K*m)" );
      cpypar( 44, "Temperature_coefficient_of_the_latter", "1/K" );
   }
   else
   {
      strcpy(( map->shj ), "VOID" );

      cpypar( 43, "Heat_conductivity,_mantle", inact );
      cpypar( 44, "Temperature_coefficient_of_the_latter", inact );

      par.s[14] = par.s[12];
   };
/*...........................................................................*/
/* inner dimensions can't exceed outer ones: */

   if (( opp->n[1] ) == null ) /* rectangular waveguide */
   {
      if ( par.s[14] < par.s[12] )
      {
         cpypar( 14, "Waveguide_mantle_width", "wgd_width=s12;_corrected" );
         par.s[14] = par.s[12];
      };

      if ( par.s[15] < par.s[13] )
      {
         cpypar( 15, "Waveguide_mantle_height", "wgd_height=s13;_corrected" );
         par.s[15] = par.s[13];
      };

      if (( par.s[14] <= par.s[12] )
         ||( par.s[15] <= par.s[13] )
         ||( par.s[43] < SMALL_VAL ))
      {
         if (( opp->n[2] ) == null ) /* fixed input power */
         {
            cpypar( 5, "Temperature,_waveguide_mantle", "initial_value;_C" );
	    cpypar( 6, "Temperature,_waveguide", depdt );
/*
            par.s[6] = par.s[5];      WHY THAT ???
*/
         }
         else /* fixed mantle temperature */
         {
            cpypar( 5, "Temperature,_waveguide_mantle", "deg_Celsius" );
	    cpypar( 6, "Temperature,_waveguide", depdt );
/*
            par.s[6] = par.s[5];      WHY THAT ???
*/
         };
      };
   }
   else /* coaxial waveguide */
   {
      if ( par.s[14] < par.s[12] )
      {
         cpypar( 14, "Diameter,_mantle", "outer_tube=s12;_corrected" );
         par.s[14] = par.s[12];
      };

      if ( par.s[13] < par.s[15] )
      {
         cpypar( 15, "Diameter,_cooling_tube", "inn_diameter=s13;_corrected" );
         par.s[15] = par.s[13];
      };
   }; /* end coaxial waveguide */
/*...........................................................................*/
/* special requirements for cooled waveguides. */

   if ((( state->act ) == ONE )
     &&(( opp->n[1] ) == null )) /* rectangular waveguide */
   {
      if ((( opp->n[4] ) != null )
        &&(( par.s[14] <= par.s[12] )
         ||( par.s[15] <= par.s[13] )
         ||( par.s[43] < SMALL_VAL )))
      {
         fprintf( stderr, "\n Forced convection [ cooling ] in waveguide" );
         fprintf( stderr, "\n requires specifying a waveguide mantle of" );
         fprintf( stderr, "\n finite wall thickness and finite heat" );
         fprintf( stderr, "\n conductivity.\n\n" );

         exit( EXIT_FAILURE );
      };
   }
   else if (( state->act ) == ONE )
   {
      if ((( opp->n[4] ) != null )
        &&(( opp->n[7] ) != null ))
      {
         if (( par.s[13] <= par.s[15] )
           ||( par.s[45] < SMALL_VAL ))
         {
            fprintf( stderr, "\n Forced convection between conductors" );
            fprintf( stderr, "\n combined with additional inner conductor" );
            fprintf( stderr, "\n cooling requires specifying a cooling" );
            fprintf( stderr, "\n pipe of finite wall thickness and" );
            fprintf( stderr, "\n finite heat conductivity.\n\n" );

            exit( EXIT_FAILURE );
         };
      }
      else if (( opp->n[4] ) != null )
      {
         if (( par.s[14] <= par.s[12] )
	   ||( par.s[43] < SMALL_VAL ))
         {
            fprintf( stderr, "\n Forced convection between conductors" );
            fprintf( stderr, "\n requires specifying an outer conductor" );
            fprintf( stderr, "\n mantle of finite wall thickness and" );
            fprintf( stderr, "\n finite heat conductivity.\n\n" );
 
            exit( EXIT_FAILURE );
         };
      };
   }; /* end if ( opp->n[1] == ONE ): coaxial waveguide */
/*............................................................................*/
/*............................................................................*/
/* PARAMETER TRANSFER SECTION - do no longer modify any *->prp parameters !!! */
/* parameters are transferred into structures ( state )->dmp, *->elp, *->thp  */
/*............................................................................*/
/*............................................................................*/
/* read physical parameters into structures: */
/* waveguide or inner conductor [ metal ]: */
/* [ electric ] */

   ( elp->rsi ) = ( prp->s[18] ); /* el. resistivity [Ohm*m] */
   ( elp->dri ) = ( prp->s[19] ); /* its temperature coefficient*/
   ( elp->myi ) = ( prp->s[20] ); /* relative premeability */
   
   if ( 1.0e-277 < ( elp->rsi ))
      ( elp->cdi ) = 1./( elp->rsi ); /* el. conductivity [S/m] */
   else
      ( elp->cdi ) = HUGE_VALF;

/* [ thermal ] */

   ( thp->hci ) = ( prp->s[21] ); /* heat conductivity [W/(m*K)] */
   ( thp->dhi ) = ( prp->s[22] ); /* its temp coefficient [1/K] */
   ( thp->mdi ) = ( prp->s[23] ); /* mass density [Kg/m^3 ]*/
   ( thp->cvi ) = ( prp->s[24] ); /* heat capacity [J/(Kg*K)] */
   ( thp->hcp ) = ( thp->hci );   /* used with side conditions, pipe */
   ( thp->dhp ) = ( thp->dhi );   /* "" */
/*............................................................................*/
/* dielectric: */
/* [ electric ] */

   ( elp->epd ) = ( prp->s[32] ); /* relative permittivity */
   ( elp->tge ) = ( prp->s[33] ); /* electric loss tangent */
   ( elp->myd ) = ( prp->s[34] ); /* relative permeability */
   ( elp->tgm ) = ( prp->s[35] ); /* magnetic loss tangent */

/* [ thermal ] */

   ( thp->hcd ) = ( prp->s[36] ); /* heat conductivity [W/(m*K)] */
   ( thp->dhd ) = ( prp->s[37] ); /* its temp coeff [1/K] */
   ( thp->mdd ) = ( prp->s[38] ); /* mass density [Kg/m^3] */
   ( thp->cvd ) = ( prp->s[39] ); /* heat capacity [J/(Kg*K)] */
   ( thp->nyd ) = ( prp->s[40] ); /* cinematic visc [m^2/s] */
   ( thp->dnd ) = ( prp->s[41] ); /* its temp coeff [1/k] */
   ( thp->pnd ) = ( prp->s[42] ); /* Prandtl number [dimsless] */
/*............................................................................*/
/* mantle: */

   ( thp->hcj ) = ( prp->s[43] ); /* heat condctvty [W/(m*K)] */
   ( thp->dhj ) = ( prp->s[44] ); /* its temperature coeff [1/K] */
   ( thp->hcm ) = ( thp->hcj );   /* circumstantially used: */
   ( thp->dhm ) = ( thp->dhj );   /* "" */
/*............................................................................*/
/* operational [ frequencies, cw power ] */
/* fixed [ or initializing ] temperatures, etc.: */

   ( elp->frn ) = ( opp->n[10] ); /* the number of frequency points */

   ( elp->frq0 ) = ( prp->s[1] ); /* initial frequency [Hz] */
   ( elp->frq1 ) = ( prp->s[2] ); /* final frequency [Hz] */
   ( elp->cwpr ) = ( prp->s[3] ); /* cw power [ fixed or initial value ] */

   if (( elp->frq0 ) <= ( elp->frq1 ))
      ( elp->freq ) = ( elp->frq0 );
   else
      ( elp->freq ) = ( elp->frq1 );

   ( thp->tenv ) = ( prp->s[4] ) + \
      CELSIUS_TO_KELVIN; /* environment temperature */
   ( thp->tjck ) = ( prp->s[5] ) + \
      CELSIUS_TO_KELVIN; /* mantle temperature */
   ( thp->tout ) = ( prp->s[5] ) + \
      CELSIUS_TO_KELVIN; /* outer conductor temperature */
   ( thp->tinn ) = ( prp->s[6] ) + \
      CELSIUS_TO_KELVIN; /* inner conductor temperature */
   ( thp->tdel ) = ( prp->s[54] ) + \
         CELSIUS_TO_KELVIN; /* mean gas temperature in waveguide */
   ( thp->tfld ) = ( prp->s[57] ) + \
         CELSIUS_TO_KELVIN; /* mean temperature, coolant */
   ( thp->tsdm ) = ( prp->s[66] ) + \
         CELSIUS_TO_KELVIN; /* fixed side temperature, mantle */
   ( thp->tsdp ) = ( prp->s[69] ) + \
         CELSIUS_TO_KELVIN; /* fixed side temperature, pipe */
	 
   ( thp->sdofm ) = ( prp->s[67] );
   ( thp->sdifm ) = ( prp->s[68] );
   ( thp->sdofp ) = ( prp->s[70] );
   ( thp->sdifp ) = ( prp->s[71] );

/* reset mantle temperature difference indicator */
   ( thp->mbe ) = null;
/* reset pipe temperature difference indicator */
   ( thp->ppe ) = null;
/*............................................................................*/
   if (( opp->n[1] ) == null ) /* rectangular waveguide */
   {
/*............................................................................*/
/* geometric dimensions: */

      ( dmp->winn ) = ( prp->s[12] ); /* outer waveguide width */
      ( dmp->hinn ) = ( prp->s[13] ); /* inner waveguide height */
      ( dmp->wout ) = ( prp->s[14] ); /* outer waveguide width */
      ( dmp->hout ) = ( prp->s[15] ); /* inner waveguide height */
      ( dmp->ffi ) = ( prp->s[16] );  /* form factor, waveguide */
      ( dmp->ldel ) = ( prp->s[63] ); /* length of cooled section, dielectric */
      ( dmp->ltub ) = ( prp->s[64] ); /* length of cooled section, coolg tube */
   } /* end if (( opp->n[1] ) == null ); waveguide */
   else if (( opp->n[1] ) == ONE ) /* coaxial waveguide */
   {
      ( dmp->dout ) = ( prp->s[12] ); /* diameter, outer conductor */
      ( dmp->dinn ) = ( prp->s[13] ); /* diameter, inner conductor */
      ( dmp->djck ) = ( prp->s[14] ); /* diameter, mantle [ outer tube dmtr ] */
      ( dmp->dtub ) = ( prp->s[15] ); /* diameter, cooling pipe [ tube dmtr ] */
      ( dmp->ffi ) = ( prp->s[16] );  /* form factor, inner conductor */
      ( dmp->ffo ) = ( prp->s[17] );  /* form factor, outer conductor */
      ( dmp->ldel ) = ( prp->s[63] ); /* length, cooled section */
      ( dmp->ltub ) = ( prp->s[64] ); /* length, cooled section */
/*............................................................................*/
/* characteristic line impedance [Ohm]: */

      ( elp->limp ) = ( sqrt( MY_VAC_ / EPS_VAC )) / ( 2.* PI );
      ( elp->limp ) *= sqrt (( elp->myd ) / ( elp->epd ));
      ( elp->limp ) *= log(( dmp->dout ) / ( dmp->dinn ));
/*............................................................................*/
/* outer conductor [ metal ]: */
/* [ electric ] */

      ( elp->rso ) = ( prp->s[25] ); /* el. resistivity [Ohm*m] */
      ( elp->dro ) = ( prp->s[26] ); /* its temperature coefficient*/
      ( elp->myo ) = ( prp->s[27] ); /* relative premeability */

      if ( 1.0e-277 < ( elp->rso ))
         ( elp->cdo ) = 1./( elp->rso ); /* el. conductivity [S/m] */
      else
         ( elp->cdo ) = HUGE_VALF;

/* [ thermal ] */

      ( thp->hco ) = ( prp->s[28] ); /* heat conductivity [W/(m*K)] */
      ( thp->dho ) = ( prp->s[29] ); /* its temp coefficient [1/K] */
      ( thp->mdo ) = ( prp->s[30] ); /* mass density [Kg/m^3 ]*/
      ( thp->cvo ) = ( prp->s[31] ); /* heat capacity [J/(Kg*K)] */
/*............................................................................*/
/* pipe [ thermal ]: */

      ( thp->hct ) = ( prp->s[45] ); /* heat conductivity [W/(m*K)] */
      ( thp->dht ) = ( prp->s[46] ); /* temperature coeff of the latter [1/K] */
/*............................................................................*/
/* cooling fluid parameters: */

      if (( opp->n[7] ) != null )
      {
         ( thp->tfld ) = ( prp->s[57] ); /* coolant temperature */
	 ( thp->tfld ) += ( CELSIUS_TO_KELVIN );

         ( thp->hcf ) = ( prp->s[47] ); /* heat conductivity [W/(m*K)] */
         ( thp->dhf ) = ( prp->s[48] ); /* its temperature coeff [1/K] */
         ( thp->mdf ) = ( prp->s[49] ); /* mass density [Kg/m^3] */
         ( thp->cvf ) = ( prp->s[50] ); /* heat capacity [J/(Kg*K)] */
         ( thp->nyf ) = ( prp->s[51] ); /* cinematic visc [m^2/s] */
         ( thp->dnf ) = ( prp->s[52] ); /* its temp coeff [1/K] */
         ( thp->pnf ) = ( prp->s[53] ); /* Prandtl number [dimensionless] */
         ( thp->nuf ) = ( prp->s[60] ); /* Nusselt number */
      };

      ( thp->tdel ) = ( prp->s[54] ); /* temperature dielectric */
      ( thp->tdel ) += ( CELSIUS_TO_KELVIN );

      ( thp->hcd ) = ( prp->s[36] ); /* heat conductivity [W/(m*K)] */
      ( thp->dhd ) = ( prp->s[37] ); /* its temperature coeff [1/K] */
      ( thp->mdd ) = ( prp->s[38] ); /* mass density [Kg/m^3] */
      ( thp->cvd ) = ( prp->s[39] ); /* heat capacity [J/(Kg*K)] */
      ( thp->nyd ) = ( prp->s[40] ); /* cinematic visc [m^2/s] */
      ( thp->dnd ) = ( prp->s[41] ); /* its temp coeff [1/K] */
      ( thp->pnd ) = ( prp->s[42] ); /* Prandtl number [dimensionless] */
      ( thp->nud ) = ( prp->s[56] ); /* Nusselt number */

      if (( opp->n[5] ) != null )
      {
         ( thp->tenv ) = ( prp->s[4] ); /* temperature environment */
         ( thp->tenv ) += ( CELSIUS_TO_KELVIN );
         ( thp->nue ) = ( prp->s[62] ); /* Nusselt number */
      };
   }; /* end if ( opp->n[1] == ONE ): coaxial waveguide */
/*............................................................................*/
   return null;
}
/*============================================================================*/
# undef LINLEN
/*********************** end of function rvise_par(*) *************************/
