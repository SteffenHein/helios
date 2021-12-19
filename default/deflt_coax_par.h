/* [ file: deflt_coax_par.h ] */
/* Update: December 17, 2021 */
/*============================================================================*/

void deflt_params( void ) /* default HELIOS physical operation parameters */
{
   void
      cpypar( short index, const char *ltext, const char *bracket );
/*----------------------------------------------------------------------------*/
/* parameter strings: */
/* number of parameters */
   
   par.s[0]  = 71;

/* write only connected (!) strings into the string copy function strcpy(*) */
/* [ 2nd. argument       | ] */
/*                       V   */
/*............................................................................*/
   strcpy( par.stx[0] , "parameters\n" );
   strcat( par.stx[0] , "[INTERNATIONAL_UNITS;"\
		        "_material_constants_at_0_deg_Celsius]" );

   cpypar( 1, "Starting_[lower_or_upper]_frequency", "Hz" );
   cpypar( 2, "Stopping_[upper_or_lower]_frequency", "Hz" );
   cpypar( 3, "Transmitted_CW_power", "Watts" );
   cpypar( 4, "Temperature,_environment", "deg_Celsius" );
   cpypar( 5, "Temperature,_waveguide_mantle", "deg_Celsius" );
   cpypar( 6, "Temperature,_inner_conductor", "deg_Celsius" );
   cpypar( 7, "Pressure,_environment", "bar_absolute" );
   cpypar( 8, "Pressure_in_waveguide", "bar_absolute" );
   cpypar( 9, "Heat_radiation_emissivity,_mantle", "" );
   cpypar( 10, "Heat_radiation_emissivity,_outer_conductor", "" );
   cpypar( 11, "Heat_radiation_emissivity,_inner_conductor", "" );
   cpypar( 12, "Diameter,_outer_conductor", "m" );
   cpypar( 13, "Diameter,_inner_conductor", "m" );
   cpypar( 14, "Diameter,_mantle_[outer_tube_diameter]", "m" );
   cpypar( 15, "Diameter,_pipe_[inner_tube_width]", "m" );
   cpypar( 16, "Form_factor,_inner_conductor", "" );
   cpypar( 17, "Form_factor,_outer_conductor", "" );
   cpypar( 18, "Electric_resistivity,_inner_conductor", "Ohm*m" );
   cpypar( 19, "Temperature_coefficient_of_the_latter", "1/K" );
   cpypar( 20, "Relative_permeability,_inner_conductor", "" );
   cpypar( 21, "Heat_conductivity,_inner_conductor", "W/(m*K)" );
   cpypar( 22, "Temperature_coefficient_of_the_latter", "1/K" );
   cpypar( 23, "Density,_inner_conductor", "Kg/m^3" );
   cpypar( 24, "Heat_capacity_[cp],_inner_conductor", "J/(Kg*K)" );
   cpypar( 25, "Electric_resistivity,_outer_conductor", "Ohm*m" );
   cpypar( 26, "Temperature_coefficient_of_the_latter", "1/K" );
   cpypar( 27, "Relative_permeability,_outer_conductor", "" );
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
   cpypar( 42, "Prandtl_number,_dielectric", "" );
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
   cpypar( 53, "Prandtl_number,_cooling_fluid", "" );
   cpypar( 54, "Temperature,_gas_flowing_in_waveguide", "deg_Celsius" );
   cpypar( 55, "Flow,_that_gas", "liter/min" );
   cpypar( 56, "Nusselt_number,_that_gas_flow", "" );
   cpypar( 57, "Temperature,_cooling_fluid", "deg_Celsius" );
   cpypar( 58, "Pressure,_cooling_fluid", "bar_absolute" );
   cpypar( 59, "Flow,_cooling_fluid", "liter/min" );
   cpypar( 60, "Nusselt_number,_that_fluid_flow", "" );
   cpypar( 61, "Flow_velocity,_air_in_environment", "m/s" );
   cpypar( 62, "Nusselt_number,_that_air_flow", "" );
   cpypar( 65, "Waveguide_length", "m" );
   cpypar( 63, "Length_of_cooled_section(s),_dielectric", "m" );
   cpypar( 64, "Length_of_cooled_section(s),_pipe", "m" );
   cpypar( 66, "Fixed_temperature_on_one_side,_mantle", "deg_Celsius" );
   cpypar( 67, "Fixed_heat_outflow_on_this_side,_mantle", "W" );
   cpypar( 68, "Fixed_heat_inflow_[opposite_side],_mantle", "W" );
   cpypar( 69, "Fixed_temperature_on_one_side,_pipe", "deg_Celsius" );
   cpypar( 70, "Fixed_heat_outflow_on_this_side,_pipe", "W" );
   cpypar( 71, "Fixed_heat_inflow_[opposite_side],_pipe", "W" );
/*............................................................................*/
/* read outer conductor [metal] parameters: */
                                                                               
   par.s[1]  = 1.00000e+08; /* lower frequency                           [Hz] */
   par.s[2]  = 5.00000e+08; /* upper frequency                           [Hz] */
   par.s[3]  = 2.00000e+05; /* transmitted CW power                       [W] */
   par.s[4]  = 3.00000e+01; /* environmental temperature        [deg Celsius] */
   par.s[5]  = 5.00000e+01; /* temperature, waveguide_mantle    [deg Celsius] */
   par.s[6]  = 1.50000e+02; /* temperature, inner conductor     [deg Celsius] */
   par.s[7]  = 1.00000e+00; /* abs pressure, environment                [Bar] */
   par.s[8]  = 1.00000e+00; /* abs pressure in conductor                [Bar] */
   par.s[9]  = 2.00000e-02; /* outer rel heat emissivity, outer conductor     */
   par.s[10] = 2.00000e-02; /* inner rel heat emissivity, outer conductor     */
   par.s[11] = 2.00000e-02; /* rel heat emissivity, inner       conductor     */

   par.s[12] = 2.30000e-01; /* diameter, outer conductor                  [m] */
   par.s[13] = 9.98800e-02; /* diameter, inner conductor                  [m] */
   par.s[14] = 2.40000e-01; /* diameter, mantle [or tube jacket]          [m] */
   par.s[15] = 9.70000e-02; /* diameter, cooling pipe                     [m] */
   par.s[16] = 1.00000e+00; /* form factor, inner conductor                   */
   par.s[17] = 1.00000e+00; /* form factor, outer conductor                   */

   par.s[18] = 4.35000e-08; /* electric resistivity, inner conductor  [Ohm*m] */
   par.s[19] = 4.08000e-03; /* its temperature coefficient              [1/K] */
   par.s[20] = 1.00000e+00; /* relative permeability, inner conductor         */
   par.s[21] = 5.40000e+05; /* heat conductivity, inner conductor   [W/(m*K)] */
   par.s[22] = 5.00000e-01; /* its temperature coefficient              [1/K] */
   par.s[23] = 7.00000e+03; /* density, inner conductor              [Kg/m^3] */
   par.s[24] = 5.00000e+03; /* heat capacity, inner conductor      [J/(Kg*K)] */

   par.s[25] = 4.35000e-08; /* electric resistivity, outer conductor  [Ohm*m] */
   par.s[26] = 4.08000e-03; /* its temperature coefficient              [1/K] */
   par.s[28] = 5.40000e+05; /* heat conductivity, outer conductor   [W/(m*K)] */
   par.s[27] = 1.00000e+00; /* relative permeability, outer conductor         */
   par.s[29] = 5.00000e-03; /* its temperature coefficient              [1/K] */
   par.s[30] = 7.00000e+03; /* density, outer conductor              [Kg/m^3] */
   par.s[31] = 5.00000e+03; /* heat capacity, inner conductor      [J/(Kg*K)] */

   par.s[32] = 1.00000e+00; /* rel permittivity, dielectric                   */
   par.s[33] = 1.00000e-04; /* electric loss factor                           */
   par.s[34] = 1.00000e+00; /* rel permeability, dielectric                   */
   par.s[35] = 0.00000e+00; /* magnetic loss factor */
   par.s[36] = 2.00000e+01; /* heat conductivity, dielectric        [W/(m*K)] */
   par.s[37] = 0.00000e+00; /* its temperature coefficient              [1/K] */
   par.s[38] = 0.00000e+00; /* density, dielectric                   [Kg/m^3] */
   par.s[39] = 0.00000e+00; /* heat capacity, dielectric           [J/(Kg*K)] */
   par.s[40] = 1.33000e-05; /* cinematic viscosity, dielectric        [m^2/s] */
   par.s[41] = 9.76000e-08; /* its temperature coefficient              [1/K] */
   par.s[42] = 7.11000e-01; /* Prandtl number, dielectric                     */

   par.s[43] = 2.30000e+02; /* heat conductivity, mantle             [W/(m*K)]*/
   par.s[44] = 0.00000e+00; /* its temperature coefficient              [1/K] */

   par.s[45] = 2.30000e+02; /* heat conductivity, inner cooling pipe [W/(m*K)]*/
   par.s[46] = 0.00000e+00; /* its temperature coefficient              [1/K] */
   par.s[47] = 2.00000e+00; /* heat conductivity, cooling fluid     [W/(m*K)] */
   par.s[48] = 0.00000e+00; /* its temperature coefficient              [1/K] */
   par.s[49] = 0.00000e+00; /* density, cooling fluid                [Kg/m^3] */
   par.s[50] = 1.00000e+00; /* heat capacity, cooling fluid        [J/(Kg*K)] */
   par.s[51] = 1.33000e-05; /* cinematic viscosity, cooling fluid     [m^2/s] */
   par.s[52] = 9.76000e-08; /* its temperature coefficient              [1/K] */
   par.s[53] = 7.11000e-01; /* Prandtl number, cooling fluid                  */

   par.s[54] = 2.00000e+01; /* temperature, gas flowing in conductors [deg C] */
   par.s[55] = 0.00000e+00; /* flow volume per time, that gas     [liter/min] */
   par.s[56] = 3.60000e+00; /* Nusselt number, that flow                      */

   par.s[57] = 2.00000e+01; /* fluid temperature in cooling tube [deg C]      */
   par.s[58] = 1.00000e+00; /* pressure, that fluid [Bar]                     */
   par.s[59] = 0.00000e+00; /* flow volume per time, that fluid   [liter/min] */
   par.s[60] = 3.60000e+00; /* Nusselt number, that flow                      */

   par.s[61] = 0.00000e+00; /* flow velocity, air in environment      [m/sec] */
   par.s[62] = 3.60000e+00; /* Nusselt number, that flow                      */
   par.s[63] = 1.00000e+00; /* length of cooled section, dielectric       [m] */
   par.s[64] = 1.00000e+00; /* length of cooled section, pipe             [m] */
   par.s[65] = 5.00000e-01; /* waveguide length                           [m] */
   par.s[66] = 3.00000e+01; /* fixed temperature on 1 side, mantle  [Celsius] */
   par.s[67] = 0.00000e+00; /* fixed heat outflow on one this side, mantle [W]*/
   par.s[68] = 0.00000e+00; /* fixed side heat inflow [ops. side], mantle [W] */
   par.s[69] = 3.00000e+01; /* fixed temperature on 1 side, pipe    [Celsius] */
   par.s[70] = 0.00000e+00; /* fixed heat outflow on one this side,   pipe [W]*/
   par.s[71] = 0.00000e+00; /* fixed side heat inflow [ops. side],   pipe [W] */

   return;
}
/*============================================================================*/
