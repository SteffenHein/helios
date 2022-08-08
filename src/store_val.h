/* [ file: store_val.h ] */
/*----------------------------------------------------------------------------*/
# define DO_STORE_VAL "store_val(*)"
/*******************************************************************************
*                                                                              *
*   Function store_val(*); HELIOS release v1.0r1                               *
*   [ Heat and Electric Losses In Ordinary wave guiding Systems ]              *
*                                                                              *
*   Given an [open] stream, FILE *fleptr, this function writes updated special *
*   parameters and computed values into the stream.                            *
*                                                                              *
*   (C) SHEIN; Munich, December 2021                       Steffen Hein        *
*   [ Update: December 17, 2021 ]                       <contact@sfenx.de>     *
*                                                                              *
*******************************************************************************/
# define LINLEN 31
/*----------------------------------------------------------------------------*/
# if USE_UNICODE == 1
   # define CELSIUS ( const char * ) "\x00B0""C"
# else
   # define CELSIUS ( const char * ) "C"
# endif
# define KELVIN ( const char * ) "K"
/*----------------------------------------------------------------------------*/

/*============================================================================*/

void store_val( HELIOSSTATE *state, FILE *fleptr, char option )
{
   static char
      ptr[STS_SIZE] = {null};

   static short
      ii = null,
      jj = null,
      frc = null,
      mrank = null,
      nodes = null;

   static const char /* dsplay(*) options */
     *dline = "========================================"\
              "========================================";

   static double
      dtxx = ZERO,
      temp = ZERO,
      attn = ZERO,
      rscl = 1.;

   void
      cpylne( char lne[], const char *tet, const char * brc, short nn );
/*----------------------------------------------------------------------------*/
# ifdef HLS_RESCLE
   rscl = HLS_RESCLE/( state->dmp->lwgd );
# endif
/*............................................................................*/

   frc = ( state->elp->frc );
   
   switch( option )
   { 
     case'o':
     case'O':
/*............................................................................*/
/* materials and dimensions */

      fprintf( fleptr, "\n%s", dline );

      if (( state->opp->n[1] ) == null ) /* rectangular waveguide */
      {
         fprintf( fleptr, "\nDimensions_and_materials\n" );

         if (( state->opmrk[1] ) == ONE )
         {
/*............................................................................*/
# if USE_UNICODE == 1
            cpylne( ptr, "Waveguide_length", "\x00AB""WGL""\x00BB", LINLEN );
# else
            cpylne( ptr, "Waveguide_length", "WGL", LINLEN );
# endif
/*............................................................................*/
            fprintf( fleptr, "\n%s: % .9e %s\n",
               ptr, ( state->dmp->lwgd ), "m" );
         };

         cpylne( ptr, "Mantle", ( state->map->shj ), LINLEN );
         fprintf( fleptr, "\n%s:", ptr );
         fprintf( fleptr, "\nOuter_width = %.9e m | outer_height = %.9e m",
         ( state->dmp->wout ), ( state->dmp->hout ));

         cpylne( ptr, "Waveguide", ( state->map->shi ), LINLEN );
         fprintf( fleptr, "\n%s:", ptr );
         fprintf((fleptr), "\nInner_width = %.9e m | inner_height = %.9e m",
            ( state->dmp->winn ), ( state->dmp->hinn ));

         if (( state->opp->n[4] ) != null )
         {
            fprintf( fleptr, "\n\nLength_of_cooled_section(s)\n" );

            cpylne( ptr, "Dielectric", ( state->map->shd ), LINLEN );
            fprintf( fleptr, "\n%s: % .9e m", \
               ptr, ( state->dmp->ldel ));
         };

         fprintf( fleptr, "\n\nTE10_mode_characteristic_impedance\n"
            "%.9e Ohm", ( state->elp->limp ));
      }
      else /* if ( state->opp->n[1] == ONE ): coaxial waveguide */
      {
         fprintf( fleptr, "\nDimensions_and_materials\n" );

         if (( state->opmrk[1] ) == ONE )
         {
/*............................................................................*/
# if USE_UNICODE == 1
            cpylne( ptr, "Waveguide_length", "\x00AB""WGL""\x00BB", LINLEN );
# else
            cpylne( ptr, "Waveguide_length", "WGL", LINLEN );
# endif
/*............................................................................*/
            fprintf( fleptr, "\n%s: % .9e %s\n",
               ptr, ( state->dmp->lwgd ), "m" );
/*............................................................................*/
         };

         cpylne( ptr, "Diameter,_outer_conductor", 
            ( state->map->sho ), LINLEN );

         fprintf( fleptr, "\n%s: % .9e m", \
            ptr, ( state->dmp->dout ));

         if (( state->opp->n[4] ) == 1 )
         {
            if ( null == strcmp (( state->map->shd ), "ELSE" ))
               fprintf( fleptr, "  [ cooled ]" );
            else
               fprintf( fleptr, "  [ %s-cooled ]", ( state->map->shd ));
         };

         cpylne( ptr, "Diameter,_inner_conductor", 
            ( state->map->shi ), LINLEN );

         fprintf( fleptr, "\n%s: % .9e m", \
            ptr, ( state->dmp->dinn ));

         if (( state->opp->n[4] ) == 1 ) /* cooling */
         {
            if ( null == strcmp (( state->map->shd ), "ELSE" ))
               fprintf( fleptr, "  [ cooled ]" );
            else
               fprintf( fleptr, "  [ %s-cooled ]", ( state->map->shd ));
         };

         cpylne( ptr, "Outer_diameter,_mantle", ( state->map->shj ), LINLEN );
         fprintf( fleptr, "\n%s: % .9e m", \
            ptr, ( state->dmp->djck ));

         cpylne( ptr, "Inner_diameter,_pipe", ( state->map->shc ), LINLEN );
         fprintf( fleptr, "\n%s: % .9e m", \
            ptr, ( state->dmp->dtub ));

         if (( state->opp->n[7] ) == 1 ) /* additional inn cond cooling */
         {
            if ( null == strcmp (( state->map->shf ), "ELSE" ))
               fprintf( fleptr, "  [ cooled ]" );
            else
               fprintf( fleptr, "  [ %s-cooled ]", ( state->map->shf ));
         };

         if ((( state->opp->n[4] ) == ONE )
           ||(( state->opp->n[7] ) == ONE ))
         {
            fprintf( fleptr, "\n\nLength_of_cooled_section(s)\n" );

            if (( state->opp->n[4] ) != null )
            {
               cpylne( ptr, "Dielectric", ( state->map->shd ), LINLEN );
               fprintf( fleptr, "\n%s: % .9e m", ptr,
                  ( state->dmp->ldel ));
            };

            if (( state->opp->n[7] ) != null )
            {
               cpylne( ptr, "Cooling_pipe", ( state->map->shc ), LINLEN );
               fprintf( fleptr, "\n%s: % .9e m", ptr,
                  ( state->dmp->ltub ));

               cpylne( ptr, "Characteristic_cooling_length_L", "", LINLEN );
               fprintf( fleptr, "\n%s: % .9e m", ptr,
                  ( state->dmp->lfef ));
               fprintf( fleptr, "  [ z=L: 1/e decline of" );
               fprintf( fleptr, "\n%75s", "cooling efficiency ]" );

               fprintf ( fleptr, "\nCharacteristic_line_impedance\n"
                  "%.9e Ohm", ( state->elp->limp ));
            }
            else
               fprintf ( fleptr, "\n\nCharacteristic_line_impedance\n"
                  "%.9e Ohm", ( state->elp->limp ));
         }
	 else
            fprintf ( fleptr, "\n\nCharacteristic_line_impedance\n"
               "%.9e Ohm", ( state->elp->limp ));
      }; /* end coaxial conductor */
/*............................................................................*/
/* operating conditions: */

      fprintf( fleptr, "\n\nOperating_conditions\n" );

      cpylne( ptr, "Frequency", "", LINLEN );
      fprintf( fleptr, "\n%s: % .9e Hz", ptr,
         ( state->rsp->freq[frc] ));

      if (( state->opp->n[2] ) == null ) /* given power limit */
         cpylne( ptr, "CW_power", "limit", LINLEN );
      else
         cpylne( ptr, "CW_power", "", LINLEN );

      fprintf( fleptr, "\n%s: % .9e W", ptr,
         ( state->rsp->cwpr[frc] ));

      if (( state->opmrk[1] ) == ONE )
      { 
         if (( state->opp->n[1] ) == null ) /* rectangular WG */
         {
            cpylne( ptr, "Side_heat_outflow", "mantle", LINLEN );
            fprintf( fleptr, "\n%s: % .9e %s", \
               ptr, ( state->rsp->sdofm[frc] ), "W" );

            if ((( state->opp->n[9] ) == ONE )
              ||(( state->opp->n[9] ) == THREE ))
               fprintf( fleptr, "  [ fixed ]" );

            cpylne( ptr, "Heat_inflow,_opp_side", "mantle", LINLEN );
            fprintf( fleptr, "\n%s: % .9e %s", \
               ptr, ( state->rsp->sdifm[frc] ), "W" );

            fprintf( fleptr, "  [ fixed ]" );
         }
         else /* if (( state->opp->n[1] ) == ONE ) - coaxial line */
         {
            if (( state->cyl[6] ) == ONE )
            {
               cpylne( ptr, "Side_heat_outflow", "mantle", LINLEN );
               fprintf( fleptr, "\n%s: % .9e %s", \
	          ptr, ( state->rsp->sdofm[frc] ), "W" );

               if ((( state->opp->n[9] ) == ONE )
                 ||(( state->opp->n[9] ) == THREE ))
                  fprintf( fleptr, "  [ fixed ]" );

               cpylne( ptr, "Heat_inflow,_opp_side", "mantle", LINLEN );
               fprintf( fleptr, "\n%s: % .9e %s", \
	          ptr, ( state->rsp->sdifm[frc] ), "W" );

               fprintf( fleptr, "  [ fixed ]" );
            };

            if (( state->cyl[5] ) == ONE )
            {
               cpylne( ptr, "Side_heat_outflow", "pipe", LINLEN );
               fprintf( fleptr, "\n%s: % .9e %s", \
	          ptr, ( state->rsp->sdofp[frc] ), "W" );

               if ((( state->opp->n[9] ) == TWO )
                 ||(( state->opp->n[9] ) == THREE ))
                  fprintf( fleptr, "  [ fixed ]" );

               cpylne( ptr, "Heat_inflow,_opp_side", "pipe", LINLEN );
               fprintf( fleptr, "\n%s: % .9e %s", \
	          ptr, ( state->rsp->sdifp[frc] ), "W" );

               fprintf( fleptr, "  [ fixed ]" );
            };
         }; /* end coaxial line */
      }; /* end if (( state->opmrk[1] ) == ONE ) */
/*............................................................................*/
/* temperatures: */

      fprintf( fleptr, "\n\nTemperatures\n" );

      if (( state->opp->n[5] ) == ONE ) /* convection */
      {
         if (( state->opp->n[6] ) == null )
            cpylne( ptr, "Environment", "free_convection", LINLEN );
         else
            cpylne( ptr, "Environment", "forced_convection", LINLEN );

         temp = ( state->thp->tenv );
/*............................................................................*/
# if RES_KLVTDC == 1
         temp -= CELSIUS_TO_KELVIN;
         fprintf( fleptr, "\n%s: % .9e %s", ptr, temp, CELSIUS );
# else
         fprintf( fleptr, "\n%s % .9e %s", ptr, temp, KELVIN );
# endif
/*............................................................................*/
         if (( state->opp->n[6] ) == null )
            fprintf( fleptr, "  [ AIR ]" );
         else
            fprintf( fleptr, "  [ AIR, %.2e m/sec ]", ( state->thp->vle ));
      };

      if (( state->opp->n[1] ) == null ) /* rectangular waveguide */
      {
         if (( state->cyl[1] ) == ONE ) /* waveguide with separate mantle */
         {
            if ( state->cyl[3] == ONE )
            {
               if (( state->opp->n[5] ) == null ) /* fixed wall temperature */
                  cpylne( ptr, "Mantle", 
                     "mean_temp,_fixed", LINLEN );
               else
                  cpylne( ptr, "Mantle", "mean_temperature", LINLEN );
            }
            else
            {
               if (( state->opp->n[5] ) == null ) /* fixed wall temperature */
                  cpylne( ptr, "Mantle", "fixed", LINLEN );
               else
                  cpylne( ptr, "Mantle", "", LINLEN );
            };

            temp = ( state->rsp->tjck[frc] );
/*............................................................................*/
# if RES_KLVTDC == 1
            temp -= CELSIUS_TO_KELVIN;
            fprintf( fleptr, "\n%s: % .9e %s", ptr, temp, CELSIUS );
# else
            fprintf( fleptr, "\n%s: % .9e %s", ptr, temp, KELVIN );
# endif
/*............................................................................*/
            if (( state->opp->n[2] ) == ONE ) /* given temperature limit */
               cpylne( ptr, "Waveguide", "limit", LINLEN );
            else
               cpylne( ptr, "Waveguide", "", LINLEN );

            temp = ( state->rsp->tinn[frc] );
/*............................................................................*/
# if RES_KLVTDC == 1
            temp -= CELSIUS_TO_KELVIN;
            fprintf( fleptr, "\n%s: % .9e %s", ptr, temp, CELSIUS );
# else
            fprintf( fleptr, "\n%s: % .9e %s", ptr, temp, KELVIN );
# endif
/*............................................................................*/
         } /* end if (( state->cyl[1] == ONE ) - separated mantle */
	 else if (( state->thp->mbe ) == ONE )
         {
            if ( state->cyl[3] == ONE )
            {
               if (( state->opp->n[5] ) == null ) /* fixed wall temperature */
                  cpylne( ptr, "Mantle", "mean_temp,_fixed", LINLEN );
               else if (( state->opp->n[2] ) == ONE ) /* temperature limit */
                  cpylne( ptr, "Mantle", "mean_temp_limit", LINLEN );
               else
                  cpylne( ptr, "Mantle", "mean_temperature", LINLEN );
            }
            else
            {
               if (( state->opp->n[5] ) == null ) /* fixed wall temperature */
                  cpylne( ptr, "Mantle", "fixed", LINLEN );
               else if (( state->opp->n[2] ) == ONE ) /* temperature limit */
                  cpylne( ptr, "Mantle", "limit", LINLEN );
               else
                  cpylne( ptr, "Mantle", "", LINLEN );
            };

            temp = ( state->rsp->tjck[frc] );
/*............................................................................*/
# if RES_KLVTDC == 1
            temp -= CELSIUS_TO_KELVIN;
            fprintf( fleptr, "\n%s: % .9e %s", ptr, temp, CELSIUS );
# else
            fprintf( fleptr, "\n%s: % .9e %s", ptr, temp, KELVIN );
# endif
/*............................................................................*/
            cpylne( ptr, "Waveguide", "Ti_<=", LINLEN );

            temp = ( state->rsp->tinn[frc] );
/*............................................................................*/
# if RES_KLVTDC == 1
            temp -= CELSIUS_TO_KELVIN;
            fprintf( fleptr, "\n%s: % .9e %s", ptr, temp, CELSIUS );
# else
            fprintf( fleptr, "\n%s: % .9e %s", ptr, temp, KELVIN );
# endif
/*............................................................................*/
            fprintf( fleptr, "  [ upper bound ]" );
         }
	 else /* mantle and waveguide not separated */
         {
            temp = ( state->rsp->tjck[frc] );
         
            if (( state->opp->n[5] ) == null ) /* fixed wall temperature */
               cpylne( ptr, "Waveguide/mantle", "fixed", LINLEN );
            else if (( state->opp->n[2] ) == ONE ) /* temperature limit */
               cpylne( ptr, "Waveguide/mantle", "limit", LINLEN );
            else
               cpylne( ptr, "Waveguide/mantle", "", LINLEN );
/*............................................................................*/
# if RES_KLVTDC == 1
            temp -= CELSIUS_TO_KELVIN;
            fprintf( fleptr, "\n%s: % .9e %s", ptr, temp, CELSIUS );
# else
            fprintf( fleptr, "\n%s: % .9e %s", ptr, temp, KELVIN );
# endif
/*............................................................................*/
         };
/*............................................................................*/
/* mantle temperature or heat outflow fixed on one side */

         if ( state->cyl[3] == ONE ) /* fixed side conditions, mantle */
         {
            cpylne( ptr, "Maximum_mantle_temperature", "", LINLEN );
            temp = ( state->rsp->tmxm[frc] );
/*............................................................................*/
# if RES_KLVTDC == 1
            temp -= CELSIUS_TO_KELVIN;
            fprintf( fleptr, "\n%s: % .9e %s", ptr, temp, CELSIUS );
# else
            fprintf( fleptr, "\n%s % .9e %s", ptr, temp, KELVIN );
# endif
/*............................................................................*/

/*............................................................................*/
# if USE_UNICODE == 1
            cpylne( ptr, "Side_temperature,_mantle",
	       "\x00AB""STM""\x00BB", LINLEN );
# else
            cpylne( ptr, "Side_temperature,_mantle", "STM", LINLEN );
# endif
/*............................................................................*/
	    
            temp = ( state->rsp->trfm[frc] );

/*............................................................................*/
# if RES_KLVTDC == 1
            temp -= CELSIUS_TO_KELVIN;
            fprintf( fleptr, "\n%s: % .9e %s", ptr, temp, CELSIUS );
# else
            fprintf( fleptr, "\n%s % .9e %s", ptr, temp, KELVIN );
# endif
/*............................................................................*/
            if ((( state->opp->n[8] ) == ONE )
              ||(( state->opp->n[8] ) == THREE ))
               fprintf( fleptr, "  [ fixed ]" );

            cpylne( ptr, "dT/dL(mantle)", "L=0", LINLEN );
            dtxx = ( state->rsp->sdofm[frc] );
            dtxx /= ( state->rsp->sdhcm[frc] );
/*............................................................................*/
# if RES_KLVTDC == 1
            fprintf( fleptr, "\n%s: % .9e %s", ptr, dtxx, CELSIUS"/m" );
# else
            fprintf( fleptr, "\n%s % .9e %s", ptr, dtxx, "K/m" );
# endif
/*............................................................................*/
            cpylne( ptr, "dT/dL(mantle)_*_WGL_+_STM", "L=0", LINLEN );

            dtxx *= ( state->dmp->lwgd );
            dtxx += ( state->rsp->trfm[frc] );
/*............................................................................*/
# if RES_KLVTDC == 1
            dtxx -= CELSIUS_TO_KELVIN;
            fprintf( fleptr, "\n%s: % .9e %s", ptr, dtxx, CELSIUS );
# else
            fprintf( fleptr, "\n%s % .9e %s", ptr, dtxx, KELVIN );
# endif
/*............................................................................*/
         }; /* end if (( state->cyl[3] ) == ONE ) - fixed side conditions */

         if ( state->cyl[0] == ONE ) /* dielectric */
         {
            cpylne( ptr, "Dielectric", "", LINLEN );
            temp = ( state->rsp->tdel[frc] );
/*............................................................................*/
# if RES_KLVTDC == 1
            temp -= CELSIUS_TO_KELVIN;
            fprintf( fleptr, "\n%s: % .9e %s", ptr, temp, CELSIUS );
# else
            fprintf( fleptr, "\n%s: % .9e %s", ptr, temp, KELVIN );
# endif
/*............................................................................*/
         }
         else if ( state->opp->n[4] == ONE ) /* cooling */
         {
            cpylne( ptr, "Dielectric", "fixed", LINLEN );
            temp = ( state->rsp->tdel[frc] );
/*............................................................................*/
# if RES_KLVTDC == 1
            temp -= CELSIUS_TO_KELVIN;
            fprintf( fleptr, "\n%s: % .9e %s", ptr, temp, CELSIUS );
# else
            fprintf( fleptr, "\n%s: % .9e %s", ptr, temp, KELVIN );
# endif
/*............................................................................*/
            if ( null == strcmp (( state->map->shd ), "ELSE" ))
               fprintf( fleptr, "  [ %.2e m/sec ]", ( state->thp->vld ));
            else
               fprintf( fleptr, "  [ %s, %.2e m/sec ]",
                  ( state->map->shd ), ( state->thp->vld ));
         };
      }
      else /* if ( state->opp->n[1] == ONE ): coaxial waveguide */
      {
         if ( state->cyl[4] == ONE ) /* mantle */
         {
            if ( state->cyl[6] == ONE )
            {
               cpylne( ptr, "Mantle", "mean_temperature", LINLEN );
            }
            else
            {
               cpylne( ptr, "Mantle", "", LINLEN );
            };

            temp = ( state->rsp->tjck[frc] );
/*............................................................................*/
# if RES_KLVTDC == 1
            temp -= CELSIUS_TO_KELVIN;
            fprintf( fleptr, "\n%s: % .9e %s", ptr, temp, CELSIUS );
# else
            fprintf( fleptr, "\n%s: % .9e %s", ptr, temp, KELVIN );
# endif
/*............................................................................*/
         }
	 else if (( state->opp->n[5] == null ) /* fixed mantle temperature */
                 &&(( state->cyl[3] == ONE )   /* outer conductor */
                  ||( state->thp->mbe == ONE )))
         {
            if ( state->cyl[6] == ONE )
            {
               if (( state->opp->n[5] ) == null ) /* fixed wall temp */
                  cpylne( ptr, "Mantle", "mean_temp,_fixed", LINLEN );
               else
                  cpylne( ptr, "Mantle", "mean_temperature", LINLEN );
            }
            else
            {
               if (( state->opp->n[5] ) == null ) /* fixed mantle temperature */
                  cpylne( ptr, "Mantle", "fixed", LINLEN );
               else
                  cpylne( ptr, "Mantle", "", LINLEN );
            };

            cpylne( ptr, "Mantle", "fixed", LINLEN );
            temp = ( state->rsp->tjck[frc] );
/*............................................................................*/
# if RES_KLVTDC == 1
            temp -= CELSIUS_TO_KELVIN;
            fprintf( fleptr, "\n%s: % .9e %s", ptr, temp, CELSIUS );
# else
            fprintf( fleptr, "\n%s: % .9e %s", ptr, temp, KELVIN );
# endif
/*............................................................................*/
         };

         if (( state->cyl[3] ) == ONE ) /* outer conductor */
         {
            if ( state->cyl[6] == ONE )
            {
               cpylne( ptr, "Outer_conductor", "mean_temp", LINLEN );
            }
            else
            {
               cpylne( ptr, "Outer_conductor", "", LINLEN );
            };

            temp = ( state->rsp->tout[frc] );
/*............................................................................*/
# if RES_KLVTDC == 1
            temp -= CELSIUS_TO_KELVIN;
            fprintf( fleptr, "\n%s: % .9e %s", ptr, temp, CELSIUS );
# else
            fprintf( fleptr, "\n%s: % .9e %s", ptr, temp, KELVIN );
# endif
/*............................................................................*/
         }
	 else if ( state->thp->mbe == ONE )
         {
            if (( state->opp->n[6] == null )
              &&( state->opp->n[7] == null ))
               cpylne( ptr, "Outer_conductor", "To_<=", LINLEN );
            else
               cpylne( ptr, "Outer_conductor", "To_=+-Tm", LINLEN );

            temp = ( state->rsp->tout[frc] );
/*............................................................................*/
# if RES_KLVTDC == 1
            temp -= CELSIUS_TO_KELVIN;
            fprintf( fleptr, "\n%s: % .9e %s", ptr, temp, CELSIUS );
# else
            fprintf( fleptr, "\n%s: % .9e %s", ptr, temp, KELVIN );
# endif
/*............................................................................*/
            if ((( state->opp->n[6] ) == null )
              &&(( state->opp->n[7] ) == null ))
               fprintf( fleptr, "  [ upper bound ]" );
            else
               fprintf( fleptr, "  [ approximate ]" );
         }
	 else if (( state->opp->n[5] == null ) /* fixed wall temperature */
                &&( state->cyl[4] == null ))
         {
            if ( state->cyl[6] == ONE )
            {
               cpylne( ptr, "Outer_conductor", "mean_temp_fixed", LINLEN );
            }
            else
            {
               cpylne( ptr, "Outer_conductor", "fixed", LINLEN );
            };

            temp = ( state->rsp->tjck[frc] );
/*............................................................................*/
# if RES_KLVTDC == 1
            temp -= CELSIUS_TO_KELVIN;
            fprintf( fleptr, "\n%s: % .9e %s", ptr, temp, CELSIUS );
# else
            fprintf( fleptr, "\n%s: % .9e %s", ptr, temp, KELVIN );
# endif
/*............................................................................*/
         };

         if ( state->cyl[2] ) /* lossy dielectric */
         {
            cpylne( ptr, "Dielectric", "", LINLEN );
            temp = ( state->rsp->tdel[frc] );
/*............................................................................*/
# if RES_KLVTDC == 1
            temp -= CELSIUS_TO_KELVIN;
            fprintf( fleptr, "\n%s: % .9e %s", ptr, temp, CELSIUS );
# else
            fprintf( fleptr, "\n%s: % .9e %s", ptr, temp, KELVIN );
# endif
/*............................................................................*/
         }
         else if (( state->opp->n[4] ) == ONE ) /* forced convct [ cooling ] */
         {
            cpylne( ptr, "Dielectric", "fixed", LINLEN );
            temp = ( state->rsp->tdel[frc] );
/*............................................................................*/
# if RES_KLVTDC == 1
            temp -= CELSIUS_TO_KELVIN;
            fprintf( fleptr, "\n%s: % .9e %s", ptr, temp, CELSIUS );
# else
            fprintf( fleptr, "\n%s: % .9e %s", ptr, temp, KELVIN );
# endif
/*............................................................................*/
            if ( null == strcmp (( state->map->shd ), "ELSE" ))
               fprintf( fleptr, "  [ %.2e m/sec ]", ( state->thp->vld ));
            else
               fprintf( fleptr, "  [ %s, %.2e m/sec ]",
                  ( state->map->shd ), ( state->thp->vld ));
         };

         if ( state->cyl[5] == ONE )
         {
            if (( state->opp->n[2] ) == ONE ) /* given temperature limit */
               cpylne( ptr, "Inner_condctr", 
                  "mean_tmp_limit", LINLEN );
            else
               cpylne( ptr, "Inner_conductor", "mean_temp", LINLEN );
         }
         else
         {
            if (( state->opp->n[2] ) == ONE ) /* given temperature limit */
               cpylne( ptr, "Inner_conductor", "limit", LINLEN );
            else
               cpylne( ptr, "Inner_conductor", "", LINLEN );
         };

         temp = ( state->rsp->tinn[frc] );
/*............................................................................*/
# if RES_KLVTDC == 1
         temp -= CELSIUS_TO_KELVIN;
         fprintf( fleptr, "\n%s: % .9e %s", ptr, temp, CELSIUS );
# else
         fprintf( fleptr, "\n%s: % .9e %s", ptr, temp, KELVIN );
# endif
/*............................................................................*/
         if (( state->opp->n[7] ) == ONE ) /* additional inn cond cooling */
         {
            if (( state->cyl[null] ) == ONE )
            {
               cpylne( ptr, "Cooling_pipe", "", LINLEN );
               temp = ( state->rsp->ttub[frc] );
/*............................................................................*/
# if RES_KLVTDC == 1
               temp -= CELSIUS_TO_KELVIN;
               fprintf( fleptr, "\n%s: % .9e %s", ptr, temp, CELSIUS );
# else
               fprintf( fleptr, "\n%s: % .9e %s", ptr, temp, KELVIN );
# endif
/*............................................................................*/
            }
            else if (( state->thp->ppe ) == ONE )
            {
               if ((( state->opp->n[6] ) == null )
                  &&(( state->thp->tjck ) < ( state->thp->tinn )))
                  cpylne( ptr, "Cooling_pipe", "Tp_>=", LINLEN );
               else
                  cpylne( ptr, "Cooling_pipe", "Tp_=+-Ti", LINLEN );

               temp = ( state->rsp->ttub[frc] );
/*............................................................................*/
# if RES_KLVTDC == 1
               temp -= CELSIUS_TO_KELVIN;
               fprintf( fleptr, "\n%s: % .9e %s", ptr, temp, CELSIUS );
# else
               fprintf( fleptr, "\n%s: % .9e %s", ptr, temp, KELVIN );
# endif
/*............................................................................*/
               if ((( state->opp->n[6] ) == null )
                  &&(( state->thp->tjck ) < ( state->thp->tinn )))
                  fprintf( fleptr, "  [ lower bound ]" );
               else
                  fprintf( fleptr, "  [ approximate ]" );
            };

            cpylne( ptr, "Cooling_fluid", "fixed", LINLEN );
            temp = ( state->rsp->tfld[frc] );
/*............................................................................*/
# if RES_KLVTDC == 1
            temp -= CELSIUS_TO_KELVIN;
            fprintf( fleptr, "\n%s: % .9e %s", ptr, temp, CELSIUS );
# else
            fprintf( fleptr, "\n%s: % .9e %s", ptr, temp, KELVIN );
# endif
/*............................................................................*/
            if ( null == strcmp (( state->map->shf ), "ELSE" ))
               fprintf( fleptr, "  [ %.2e m/sec ]", ( state->thp->vlf ));
            else
               fprintf( fleptr, "  [ %s, %.2e m/sec ]",
                  ( state->map->shf ), ( state->thp->vlf ));

            cpylne( ptr, "Temperature_rise,_coolant", "", LINLEN );
            temp = ( state->rsp->tflr[frc] );
/*............................................................................*/
# if RES_KLVTDC == 1
            fprintf( fleptr, "\n%s: % .9e %s", ptr, temp, CELSIUS );
# else
            fprintf( fleptr, "\n%s: % .9e %s", ptr, temp, KELVIN );
# endif
/*............................................................................*/
            fprintf( fleptr, "  [ over %.2e m ]", ( state->dmp->ltub ));
         };
	 
         if (( state->cyl[6] ) == ONE ) /* side conditions, mantle */
         {
            cpylne( ptr, "Maximum_mantle_temperature", "", LINLEN );
            temp = ( state->rsp->tmxm[frc] );
/*............................................................................*/
# if RES_KLVTDC == 1
            temp -= CELSIUS_TO_KELVIN;
            fprintf( fleptr, "\n%s: % .9e %s", ptr, temp, CELSIUS );
# else
            fprintf( fleptr, "\n%s % .9e %s", ptr, temp, KELVIN );
# endif
/*............................................................................*/

/*............................................................................*/
# if USE_UNICODE == 1
            cpylne( ptr, "Side_temperature,_mantle",
	       "\x00AB""STM""\x00BB", LINLEN );
# else
            cpylne( ptr, "Side_temperature,_mantle", "STM", LINLEN );
# endif
/*............................................................................*/

            temp = ( state->rsp->trfm[frc] );

/*............................................................................*/
# if RES_KLVTDC == 1
            temp -= CELSIUS_TO_KELVIN;
            fprintf( fleptr, "\n%s: % .9e %s", ptr, temp, CELSIUS );
# else
            fprintf( fleptr, "\n%s % .9e %s", ptr, temp, KELVIN );
# endif
/*............................................................................*/
            if ((( state->opp->n[8] ) == ONE )
              ||(( state->opp->n[8] ) == THREE ))
               fprintf( fleptr, "  [ fixed ]" );
	    
            cpylne( ptr, "dT/dL(mantle)", "L=0", LINLEN );
            dtxx = ( state->rsp->sdofm[frc] );
            dtxx /= ( state->rsp->sdhcm[frc] );
/*............................................................................*/
# if RES_KLVTDC == 1
            fprintf( fleptr, "\n%s: % .9e %s", ptr, dtxx, CELSIUS"/m" );
# else
            fprintf( fleptr, "\n%s % .9e %s", ptr, dtxx, "K/m" );
# endif
/*............................................................................*/
            cpylne( ptr, "dT/dL(mantle)_*_WGL_+_STM", "L=0", LINLEN );

            dtxx *= ( state->dmp->lwgd );
            dtxx += ( state->rsp->tjck[frc] );
/*............................................................................*/
# if RES_KLVTDC == 1
            dtxx -= CELSIUS_TO_KELVIN;
            fprintf( fleptr, "\n%s: % .9e %s", ptr, dtxx, CELSIUS );
# else
            fprintf( fleptr, "\n%s % .9e %s", ptr, dtxx, KELVIN );
# endif
/*............................................................................*/
         };

         if (( state->cyl[5] ) == ONE ) /* Fixed side conditions, inn cnd */
         {
            cpylne( ptr, "Maximum_pipe_temperature", "", LINLEN );
            temp = ( state->rsp->tmxp[frc] );
/*............................................................................*/
# if RES_KLVTDC == 1
            temp -= CELSIUS_TO_KELVIN;
            fprintf( fleptr, "\n%s: % .9e %s", ptr, temp, CELSIUS );
# else
            fprintf( fleptr, "\n%s % .9e %s", ptr, temp, KELVIN );
# endif
/*............................................................................*/

/*............................................................................*/
# if USE_UNICODE == 1
            cpylne( ptr, "Side_temperature,_pipe",
	       "\x00AB""STP""\x00BB", LINLEN );
# else
            cpylne( ptr, "Side_temperature,_pipe", "STP", LINLEN );
# endif
/*............................................................................*/

            temp = ( state->rsp->trfp[frc] );

/*............................................................................*/
# if RES_KLVTDC == 1
            temp -= CELSIUS_TO_KELVIN;
            fprintf( fleptr, "\n%s: % .9e %s", ptr, temp, CELSIUS );
# else
            fprintf( fleptr, "\n%s % .9e %s", ptr, temp, KELVIN );
# endif
/*............................................................................*/
            if ((( state->opp->n[8] ) == TWO )
              ||(( state->opp->n[8] ) == THREE ))
               fprintf( fleptr, "  [ fixed ]" );

            cpylne( ptr, "dT/dL(pipe)", "L=0", LINLEN );
            dtxx = ( state->rsp->sdofp[frc] );
            dtxx /= ( state->rsp->sdhcp[frc] );
/*............................................................................*/
# if RES_KLVTDC == 1
            fprintf( fleptr, "\n%s: % .9e %s", ptr, dtxx, CELSIUS"/m" );
# else
            fprintf( fleptr, "\n%s % .9e %s", ptr, dtxx, "K/m" );
# endif
/*............................................................................*/
            cpylne( ptr, "dT/dL(pipe)_*_WGL_+_STP", "L=0", LINLEN );

            dtxx *= ( state->dmp->lwgd );
            dtxx += ( state->rsp->trfp[frc] );
/*............................................................................*/
# if RES_KLVTDC == 1
            dtxx -= CELSIUS_TO_KELVIN;
            fprintf( fleptr, "\n%s: % .9e %s", ptr, dtxx, CELSIUS );
# else
            fprintf( fleptr, "\n%s % .9e %s", ptr, dtxx, KELVIN );
# endif
/*............................................................................*/
         };
      }; /* end if ( opp->n[1] = ONE ): coaxial waveguide */
/*............................................................................*/
/* losses: */

      fprintf( fleptr, "\n\nDifferential_losses_"
         "[dQ/dz_at_power_insertion_point]" );

      if (( state->opp->n[1] ) == null ) /* rectangular waveguide */
      {
         if ((( state->opp->n[8] ) == ONE )
           ||(( state->opp->n[8] ) == THREE )
           ||(( state->opp->n[9] ) == ONE )
           ||(( state->opp->n[9] ) == THREE ))
         {
            cpylne( ptr, "Skin_effect", 
	       ( state->map->shi ), LINLEN );
            fprintf( fleptr, "\n%s: % .9e W/m", ptr,
               ( state->rsp->lssi[frc] ));
	 }
	 else
	 {
            cpylne( ptr, "Skin_effect", ( state->map->shi ), LINLEN );
            fprintf( fleptr, "\n%s: % .9e W/m", ptr,
               ( state->rsp->lssi[frc] ));
         };
	 
         cpylne( ptr, "Dielectric", ( state->map->shd ), LINLEN );
         fprintf( fleptr, "\n\n%s: % .9e W/m", ptr,
            ( state->rsp->lssd[frc] ));
      }
      else /* if ( state->opp->n[1] == ONE ): coaxial waveguide */
      {
         if ((( state->opp->n[8] ) == ONE )
           ||(( state->opp->n[8] ) == THREE )
           ||(( state->opp->n[9] ) == ONE )
           ||(( state->opp->n[9] ) == THREE ))
         {
            cpylne( ptr, "Outer_conductor", 
	       ( state->map->sho ), LINLEN );
            fprintf( fleptr, "\n\n%s: % .9e W/m", ptr,
               ( state->rsp->lsso[frc] ));
	 }
	 else
	 {
            cpylne( ptr, "Outer_conductor", ( state->map->sho ), LINLEN );
            fprintf( fleptr, "\n\n%s: % .9e W/m", ptr,
               ( state->rsp->lsso[frc] ));
         }; 		     
	 
         if ((( state->opp->n[8] ) == TWO )
           ||(( state->opp->n[8] ) == THREE )
           ||(( state->opp->n[9] ) == TWO )
           ||(( state->opp->n[9] ) == THREE ))
         {
            cpylne( ptr, "Inner_conductor", 
	       ( state->map->shi ), LINLEN );
            fprintf( fleptr, "\n%s: % .9e W/m", ptr,
               ( state->rsp->lssi[frc] ));
	 }
	 else
	 {
            cpylne( ptr, "Inner_conductor", ( state->map->shi ), LINLEN );
               fprintf( fleptr, "\n%s: % .9e W/m", ptr,
               ( state->rsp->lssi[frc] ));
         };

         cpylne( ptr, "Dielectric", ( state->map->shd ), LINLEN );
         fprintf( fleptr, "\n%s: % .9e W/m", ptr,
            ( state->rsp->lssd[frc] ));
      };

      cpylne( ptr, "Total", "", LINLEN );
      fprintf( fleptr, "\n%s: % .9e W/m", ptr,
         ( state->rsp->lsst[frc] ));

      if (( state->opmrk[1] ) == ONE )
      { 
         fprintf( fleptr, 
            "\n\nHeat source density equivalent to side outflow\n" );
	 
         if (( state->opp->n[1] ) == null ) /* rectangular WG */
         {
            cpylne( ptr, "Side_heat_flow_density", "mantle", LINLEN );
            fprintf( fleptr, "\n%s: % .9e %s", \
	       ptr, ( state->rsp->sdedm[frc] ), "W/m" );
         }
         else /* if (( state->opp->n[1] ) == ONE ) - coaxial line */
         {
            if (( state->cyl[6] ) == ONE )
            {
               cpylne( ptr, "Side_heat_flow_density", "mantle", LINLEN );
               fprintf( fleptr, "\n%s: % .9e %s", \
	          ptr, ( state->rsp->sdedm[frc] ), "W/m" );
            };

            if (( state->cyl[5] ) == ONE )
            {
               cpylne( ptr, "Side_heat_flow_density", "pipe", LINLEN );
               fprintf( fleptr, "\n%s: % .9e %s", \
	          ptr, ( state->rsp->sdedp[frc] ), "W/m" );
            };
         }; /* end coaxial line */
      }; /* end if (( state->opmrk[1] ) == ONE ) */
/*............................................................................*/
/* attenuation: */

      fprintf( fleptr, "\n\nAttenuation\n" );

/* CW power attenuation [ Neper/m]: */
      attn = ( state->rsp->lsst[frc] )/( state->rsp->cwpr[frc] );

      fprintf( fleptr,
         "\n%.9e dB/m", ( 10.*attn/log( 10. )));
      fprintf( fleptr,
         "\n%.9e Neper/m", attn );

      fprintf( fleptr, "\n\nResistivities\n" );
 
      if (( state->opp->n[1] ) == null ) /* rectangular waveguide */
      {
         cpylne( ptr, "Metal_resistivity", ( state->map->shi ), LINLEN );
         fprintf( fleptr, "\n%s: % .9e Ohm*m", ptr,
            ( state->rsp->rai[frc] ));

         cpylne( ptr, "Surface_resistance", "", LINLEN );
         fprintf( fleptr, "\n%s: % .9e Ohm", ptr,
            ( state->rsp->rfi[frc] ));
      }
      else /* if ( state->opp->n[1] == ONE ): coaxial waveguide */
      {
         cpylne( ptr, "Outer_conductor", ( state->map->sho ), LINLEN );
         fprintf( fleptr, "\n%s: % .9e Ohm*m", ptr,
            ( state->rsp->rao[frc] ));

         cpylne( ptr, "Inner_conductor", ( state->map->shi ), LINLEN );
         fprintf( fleptr, "\n%s: % .9e Ohm*m", ptr,
            ( state->rsp->rai[frc] ));
      };

      fprintf( fleptr, "\n" );
      break;

     case't':
     case'T':
/*............................................................................*/
/* node temperatures, etc.: */

      fprintf( fleptr, "\n%s", dline );

      nodes = ( state->cdp->nodes );

      fprintf( fleptr, "\nLoss_induced_heat_currents\n" );
      ii = null; do
      {
         fprintf( fleptr, "\nhcrr[%02d] = % .9e W/m",
            ii, (( state->hcp->hcrr[ii] )/rscl ));
      } while (( ++ii ) < nodes );

      fprintf( fleptr, "\n\nNode_temperatures\n" );
      ii = null; do
      {
         fprintf( fleptr,
            "\ntemp[%02d] = % .9e %s  [ = % .9e %s ]",
            ii, (( state->thp->temp[ii] ) - CELSIUS_TO_KELVIN ), CELSIUS,
            ( state->thp->temp[ii] ), KELVIN );
      } while(( ++ii ) < nodes );

      fprintf((fleptr), "\n\nBase_temperatures\n" );
      while( ii < ( 2*nodes ))
      {
      fprintf( fleptr,
         "\ntemp[%02d] = % .9e %s  [ = % .9e %s ]",
         ii, (( state->thp->temp[ii] ) - CELSIUS_TO_KELVIN ), CELSIUS,
         ( state->thp->temp[ii] ), KELVIN );
         ii++;
      };

      if ((( state->opp->n[1] ) == ONE ) /* coaxial waveguide */
         &&(( state->opp->n[3] ) == null )) /* solid dielectric */
      {
         fprintf( fleptr,
            "\n\nTemperature_gradients_in_dielectric\n" );

	 cpylne( ptr, "Near_outer_conductor", "", LINLEN );
         fprintf( fleptr, "\n%s: % .9e K/m", ptr,
            ( state->rsp->grdo[frc] ));

	 cpylne( ptr, "Near_inner_conductor", "", LINLEN );
         fprintf( fleptr, "\n%s: % .9e K/m", ptr,
            ( state->rsp->grdi[frc] ));
      };

      if ((( state->opp->n[4] ) != null ) /* forced convection in dielectric */
        ||(( state->opp->n[6] ) != null ) /* forced convection in environment */
        ||(( state->opp->n[7] ) != null )) /* forced inner conductor cooling */
      {
         fprintf( fleptr,
            "\n\nNusselt_and_Reynolds_numbers\n" );

         if (( state->opp->n[6] ) != null  )
         {
            cpylne( ptr, "Environment", "", LINLEN );
            fprintf( fleptr, "\n%s: Nu = %.9e | Re = %.9e", ptr,
               ( state->thp->nue ), ( state->thp->ree ));
         };

         if (( state->opp->n[4] ) != null  )
         {
            cpylne( ptr, "Dielectic", "", LINLEN );
            fprintf( fleptr, "\n%s: Nu = %.9e | Re = %.9e", ptr,
               ( state->thp->nud ), ( state->thp->red ));
         };

         if (( state->opp->n[7] ) != null  )
         {
            cpylne( ptr, "Coolant", "", LINLEN );
            fprintf( fleptr, "\n%s: Nu = %.9e | Re = %.9e", ptr,
               ( state->thp->nuf ), ( state->thp->ref ));
         };
      };

      fprintf( fleptr, "\n" );
      break;

     case'c':
     case'C':
/*............................................................................*/
/* matrices: */
      
      fprintf( fleptr, "\n%s", dline );

      mrank = ( state->cdp->mrank );

/*............................................................................*/
# if RES_STRFMT == 1

      fprintf( fleptr, 
         "\nHeat_conductances_mhc[i][j]\n[W/(m*K)]\n" );

      ii = null; do
      {
         fprintf( fleptr, "\n" );
         jj = null; do
         { 
            fprintf( fleptr, "%+.5e ",
            (( state->cdp->mhc[ii][jj] )/rscl ));
         } while(( ++jj ) < nodes );
      } while(( ++ii ) < nodes );

      fprintf( fleptr, 
         "\n\nHeat_conductances_mhc[nodes+i][nodes+j]\n[m*K/W]\n" );

      while( ii < mrank )
      {
         fprintf( fleptr, "\n" );
         jj = nodes;
         while( jj < mrank )
         { 
            fprintf( fleptr, "%+.5e ",
               (( state->cdp->mhc[ii][jj] )/rscl ));
            jj++;
         };
         ii++;
      };

      fprintf( fleptr, 
         "\n\nHeat_resistances_mhr[i][j]\n[m*K/W]\n" );

      ii = null; do   
      {
         fprintf( fleptr, "\n" );
         jj = null; do
         {
	    if ( 1.0e+77 < fabs( state->cdp->mhr[ii][jj] ))
               fprintf( fleptr, " infinite    " );
	    else
               fprintf( fleptr, "%+.5e ", 
                     ( rscl*( state->cdp->mhr[ii][jj] )));
         } while(( ++jj ) < nodes );
      } while(( ++ii ) < nodes );

      fprintf( fleptr, 
         "\n\nHeat_resistances_mhr[nodes+i][nodes+j]\n[m*K/W]\n" );

      while( ii < mrank )
      {
         fprintf( fleptr, "\n" );
         jj = nodes;
         while( jj < mrank )
         {
	    if ( 1.0e+77 < fabs( state->cdp->mhr[ii][jj] ))
               fprintf( fleptr, " infinite    " );
	    else
               fprintf( fleptr, "%+.5e ", 
                     ( rscl*( state->cdp->mhr[ii][jj] )));
            jj++;
         };
         ii++;
      };

# elif RES_STRFMT == 2

      fprintf( fleptr, "\nHeat_conductances_mhc[i][j]\n[W/(m*K)]\n" );
      ii = null; do
      {
         fprintf( fleptr, "\n" );
         jj = null; do
         {
            fprintf( fleptr, "%+.5e ",
               (( state->cdp->mhc[ii][jj] )/rscl ));
         } while(( ++jj ) < mrank );
      } while(( ++ii ) < mrank );

      fprintf( fleptr, "\n\nHeat_resistances_mhr[i][j]\n[m*K/W]\n" );
      ii = null; do   
      {
         fprintf( fleptr, "\n" );
         jj = null; do
         {
	    if ( 1.0e+77 < fabs( state->cdp->mhr[ii][jj] ))
               fprintf( fleptr, " infinite    " );
	    else
               fprintf( fleptr, "%+.5e ",
                  ( rscl*( state->cdp->mhr[ii][jj] )));
         } while(( ++jj ) < mrank );
      } while(( ++ii ) < mrank );

# endif
/*............................................................................*/

      fprintf( fleptr, 
         "\n\nExpanded_heat_conductance_matrix_chc[i][j]\n[W/(m*K)]\n" );
      ii = null; do   
      {
         fprintf( fleptr, "\n" );
         jj = null; do
         {
            fprintf( fleptr, "%+.5e ",
               (( state->cdp->chc[ii][jj] )/rscl ));
         } while(( ++jj ) < mrank );
      } while(( ++ii ) < mrank );

      fprintf( fleptr,
         "\n\nExpanded_heat_resistance_matrix_rhc[i][j]\n[m*K/W]\n" );
      ii = null; do
      {
         fprintf( fleptr, "\n" );
         jj = null; do
         {
            fprintf( fleptr, "%+.5e ",
               ( rscl*( state->cdp->rhc[ii][jj] )));
         } while(( ++jj ) < mrank );
      } while(( ++ii ) < mrank );

      fprintf( fleptr, "\n" );
      break;

   }; /* end switch */
}
/*============================================================================*/
# undef LNLNGTH
# undef CELSIUS
# undef KELVIN
/************************ end of function store_val(*) ************************/
