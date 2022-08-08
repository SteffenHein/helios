/* [ file: deflt_mat.h ] */
/* Update: December 17, 2021 */
/*----------------------------------------------------------------------------*/
# ifndef LINLEN
   # define LINLEN 58
# endif
/*============================================================================*/

void deflt_matter( void ) /* default HELIOS material parameters */
{
   static short
      ll = LINLEN;

/* allusions: */

   short
      rvise_matter( void );

   void
      cpylne( char lne[], const char *txt, const char *brc, short linlen );
/*----------------------------------------------------------------------------*/
/* write only connected (!) strings into the string copy function strcpy(*)   */
/* [ 2nd. argument       | ]                                                  */
/*                       V                                                    */
/* default metals: */

   cpylne( mat.mti, "inner_conductor_metal", "short_hand", ll );
   strcpy( mat.shi, "Cu" );
                                                                               
   cpylne( mat.mto, "outer_conductor_metal", "short_hand", ll );
   strcpy( mat.sho, "Al" );
   
   cpylne( mat.del, "dielectric", "short_hand", ll );
   strcpy( mat.shd, "AIR" );
   
   cpylne( mat.jck, "mantle", "short_hand", ll );
   strcpy( mat.shj, "Al" );

   cpylne( mat.cpe, "pipe", "short_hand", ll );
   strcpy( mat.shc, "Al" );

   cpylne( mat.fld, "cooling_fluid", "short_hand", ll );
   strcpy( mat.shf, "VOID" );
/*............................................................................*/
   mat.m = 7; /* number of metals */
   mat.d = 7; /* number of dielectrics */
   mat.s = 4; /* number of mantle materials */
   mat.f = 2; /* number of cooling fluids */
/*............................................................................*/
/* list of available METALS: */
/*
   rvise_matter( );

   return;
*/
   strcpy( mat.avm, "Some_available_metals:" );
   strcpy( mat.mtx[0] , "METAL" ); strcpy( mat.msx[0] , "SHORT_HAND" );
/*----------------------------------------------------------------------------*/
   strcpy( mat.mtx[1] , "copper" );    strcpy( mat.msx[1] , "Cu" );
   strcpy( mat.mtx[2] , "aluminium" ); strcpy( mat.msx[2] , "Al" );
   strcpy( mat.mtx[3] , "brass" );     strcpy( mat.msx[3] , "Ms" );
   strcpy( mat.mtx[4] , "gold" );      strcpy( mat.msx[4] , "Au" );
   strcpy( mat.mtx[5] , "invar" );     strcpy( mat.msx[5] , "Inv" );
   strcpy( mat.mtx[6] , "nickel" );    strcpy( mat.msx[6] , "Ni" );
   strcpy( mat.mtx[7] , "steel" );     strcpy( mat.msx[7] , "St" );
/*............................................................................*/
/* list of available DIELECTRICS: */

   strcpy( mat.avd, "Some_available_dielectrics:" );
   strcpy( mat.dtx[0] , "DIELECTRIC" ); strcpy( mat.dsx[0] , "SHORT_HAND" );
/*----------------------------------------------------------------------------*/
   strcpy( mat.dtx[1] , "AL2O3_[99.5%]" ); strcpy( mat.dsx[1] , "AL99" );
   strcpy( mat.dtx[2] , "AL2O3_[95.0%]" ); strcpy( mat.dsx[2] , "AL95" );
   strcpy( mat.dtx[3] , "teflon" );        strcpy( mat.dsx[3] , "PTFE" );
   strcpy( mat.dtx[4] , "polyethylene" );  strcpy( mat.dsx[4] , "PE" );
   strcpy( mat.dtx[5] , "polystyrene" );   strcpy( mat.dsx[5] , "PS" );
   strcpy( mat.dtx[6] , "polyuretane" );   strcpy( mat.dsx[6] , "PU" );
   strcpy( mat.dtx[7] , "quartz" );        strcpy( mat.dsx[7] , "Qu" );
/*............................................................................*/
/* list of available JACKET MATERIALS */

   strcpy( mat.avs, "Some_[mantle_or_cooling_pipe]_solid_materials:" );
   strcpy( mat.stx[0] , "MATERIAL" ); strcpy( mat.ssx[0] , "SHORT_HAND" );
/*----------------------------------------------------------------------------*/
   strcpy( mat.stx[1] , "teflon" );        strcpy( mat.ssx[1] , "PTFE" );
   strcpy( mat.stx[2] , "polyethylene" );  strcpy( mat.ssx[2] , "PE" );
   strcpy( mat.stx[3] , "polystyrene" );   strcpy( mat.ssx[3] , "PS" );
   strcpy( mat.stx[4] , "polyuretane" );   strcpy( mat.ssx[4] , "PU" );
/*............................................................................*/
/* list of available COOLING_FLUIDS: */

   strcpy( mat.avf, "Some_available_cooling_fluids:" );
   strcpy( mat.ftx[0] , "COOLING_FLUID" ); strcpy( mat.fsx[0] , "SHORT_HAND" );
/*----------------------------------------------------------------------------*/
   strcpy( mat.ftx[1] , "air" );   strcpy( mat.fsx[1] , "AIR" );
   strcpy( mat.ftx[2] , "water" ); strcpy( mat.fsx[2] , "H2O" );

   return;
}
/*============================================================================*/
# undef LINLEN
