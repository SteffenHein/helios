/* [ file: OPRSTRNGS.M ] */
/* Update: December 19, 2021 */
/*----------------------------------------------------------------------------*/
# define OPRSTRNGS( ) \
{ \
/* the operation parameter strings: */ \
 \
/* write only connected (!) strings in the string copy function strcpy(*) */ \
/* [ 2nd. argument       | ]  */ \
/*                       V    */ \
   strcpy( opr.ntx[0], "computation_mode" ); \
   cpylne( opr.ntx[1], "rectangular_/_coaxial_waveguide", "0/1", ll ); \
   cpylne( opr.ntx[2], "fixed_CW_power_/_inner_temperature_limit", "0/1", ll );\
   cpylne( opr.ntx[3], "solid_/_gas_[or_any_fluid]_dielectric", "0/1", ll ); \
   cpylne( opr.ntx[4], \
      "free_/_forced_convection_within_waveguide", "0/1", ll ); \
   cpylne( opr.ntx[5], "fixed_mantle_temperature_/_convection", "0/1", ll ); \
   cpylne( opr.ntx[6], "free_/_forced_convection_in_environment", "0/1", ll ); \
   cpylne( opr.ntx[7], \
      "additional_internal_inner_conductor_cooling", "0/1", ll ); \
   cpylne( opr.ntx[8], \
      "mantle/_pipe/_both/_no/_side_temperatures_fixed", "1/2/3/0", ll ); \
   cpylne( opr.ntx[9], \
      "mantle/_pipe/_both/_no/_side_heat_outflow_fixed", "1/2/3/0", ll ); \
 \
   strcpy( ptr , "0<N<=" ); \
   strcat( ptr , ( lotos( FREQPTS, null ))); \
 \
   cpylne( opr.ntx[10], "number_of_frequency_points", ptr, ll ); \
};
