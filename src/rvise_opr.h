/* [ file: rvise_opr.h ] */
/*******************************************************************************
*                                                                              *
*   Function rvise_opr(*); HELIOS release v1.0r1                               *
*   [ Heat and Electric Losses In Ordinary wave guiding Structures ]           *
*                                                                              *
*   This function revises [ and corrects if necessary ] the computation mode   *
*   parameters [ input option "operation"], thereby respecting the following   *
*   hierarchy [ from highest to lowest priority ]:                             *
*                                                                              *
*                   OPERATIONS -> MATERIALS -> PARAMETERS                      *
*                                                                              *
*   Thus, a former input in this chain always dominates any subsequent input   *
*   [ which, of course, only affects conflicting cases ].                      *
*   Also, this function settles any internal parameter dependencies for input  *
*   option "operation" [ in cases where such dependencies exist ].             *
*                                                                              *
*   (C) SHEIN; Munich, December 2021                       Steffen Hein        *
*   [ Update: December 17, 2021 ]                       <contact@sfenx.de>     *
*                                                                              *
*******************************************************************************/
# ifndef OPR_SSTRNG
   # include "OPRSTRNGS.M"
# endif
/*----------------------------------------------------------------------------*/
# ifndef LINLEN
   # define LINLEN 58
# endif
/*============================================================================*/

short rvise_operts( void )
{
/* declarations: */

   HELIOSSTATE *state = &helios;

   static short
      ii = null,
      ll = LINLEN;

   static char
      ptr[STS_SIZE] = {null};

   static const char
    *inact = "inactive";

/* prototypes: */

   char
     *lotos ( long nn, char cc );

   void
      cpylne( char lne[], const char *txt, const char *brc, short linlen );
/*----------------------------------------------------------------------------*/
   ( state->opp ) = &opr;

   opr.n[null] = 10; /* the number of operation parameters */
/*............................................................................*/
/* the operation parameter strings, something like that: */

/* write only connected (!) strings in the string copy function strcpy(*) */
/* [ 2nd. argument       | ]  */
/*                       V    */
/* strcpy( opr.ntx[0] , "computation_mode" ); */
/* cpylne( opr.ntx[1], "rectangular_/_coaxial_waveguide", "0/1" ); */
/* cpylne( opr.ntx[2],
     fixed_CW_power_/_inner_temperature_limit", "0/1", ll ); */
/* cpylne( opr.ntx[3], "solid_/_gas_[or_any_fluid]_dielectric", "0/1", ll ); */
/* cpylne( opr.ntx[4],
     "free_/_forced_convection_within_waveguide", "0/1", ll ); */
/* cpylne( opr.ntx[5], "fixed_mantle_temperature_/_convection", "0/1", ll ); */
/* cpylne( opr.ntx[6], "free_/_forced_convection_in_environment", "0/1", ll );*/
/* cpylne( opr.ntx[7],
     "interior_cooling_of_inner_conductor", "0/1", ll ); */
/* cpylne( opr.ntx[8], \
      "mantle/_pipe/_both/_no/_side_temperatures_fixed", "1/2/3/0", ll ); */
/* cpylne( opr.ntx[9], \
      "mantle/_pipe/_both/_no/_side_heat_outflow_fixed", "1/2/3/0", ll ); */
/* cpylne( opr.ntx[10], "number_of_frequency_points", "0<N<=1000", ll );*/
/*............................................................................*/
   OPRSTRNGS( ); /* copy the operation options strings */
/*............................................................................*/
# ifndef _Forced
   cpylne( opr.ntx[4], "free_convection_in_waveguide", "fixed", ll );
   cpylne( opr.ntx[6], "free_convection_in_environment", "fixed", ll );
   cpylne( opr.ntx[7], "interior_cooling_of_inner_conductor", inact, ll );
   cpylne( opr.ntx[8],
     "mantle/_pipe/_both/_no/_side_temperatures_fixed", inact, ll );
   cpylne( opr.ntx[9],
     "mantle/_pipe/_both/_no/_side_heat_outflow_fixed", inact, ll );
# endif
/*............................................................................*/
/* dependencies: */
/*...........................................................................*/
   if ( opr.n[1] == null ) /* in rectangular waveguides */
      opr.n[7] = null;     /* inner conductor cooling doesn't make sense */
   else
      opr.n[1] = ONE;
/*...........................................................................*/
/* consistence: solid dielectric vs. convection within waveguide */

   if ( opr.n[3] == ONE ) /* gas [ or any fluid ] dielectric */
   {
      cpylne( opr.ntx[4],
         "free_/_forced_convection_within_waveguide", "0/1", ll );
   }
   else /* solid dielectric */
   {
      opr.n[3] = null; /* solid dielectric excludes convection in waveguide */
      opr.n[4] = null; /* in particular: forced convection excluded */

      cpylne( opr.ntx[4],
         "free_/_forced_convection_in_waveguide", inact, ll );
   };
/*...........................................................................*/
/* consistence: fixed mantle temperature vs. convection in environment */

   if ( opr.n[5] == ONE ) /* mantle exposed to free or forced convection */
   {
      cpylne( opr.ntx[6],
         "free_/_forced_convection_in_environment", "0/1", ll );

      if ( opr.n[1] == null ) /* in rectangular waveguides */
      {
         if ( opr.n[2] == null )
	 {
            if (( opr.n[8] == ONE )
               ||( opr.n[8] == THREE ))
            {
               opr.n[8] = ONE;
	    
               cpylne( opr.ntx[8],
                  "mantle/_pipe/_both/_no/_side_temperatures_fixed",
                  "1/2/3/0", ll );

               opr.n[9] = null;
            }
            else 
	    {
               opr.n[8] = null;
	    
               cpylne( opr.ntx[8],
                  "mantle/_pipe/_both/_no/_side_temperatures_fixed",
                  "1/2/3/0", ll );
            };

            if (( opr.n[9] == ONE )
               ||( opr.n[9] == THREE ))
            {
               opr.n[9] = ONE;
	    
               cpylne( opr.ntx[9],
                  "mantle/_pipe/_both/_no/_side_heat_outflow_fixed",
                  "1/2/3/0", ll );
            }
            else
	    {
               opr.n[9] = null;
	    
               cpylne( opr.ntx[9],
                  "mantle/_pipe/_both/_no/_side_heat_outflow_fixed",
                  "1/2/3/0", ll );
            };
         }
	 else /* if opr.n[2] != null [ fixed mantle temperature ] */
	 {
            opr.n[2] = ONE;
	    
            cpylne( opr.ntx[2],
               "fixed_CW_power_/_mantle_temperature_limit", "0/1", ll );
	      
            opr.n[8] = null;
	    
            cpylne( opr.ntx[8],
               "mantle/_pipe/_both/_no/_side_temperatures_fixed",
               "1/2/3/0", ll );

            opr.n[9] = null;

            cpylne( opr.ntx[9],
               "mantle/_pipe/_both/_no/_side_heat_outflow_fixed",
               "1/2/3/0", ll );
         };
      }
      else /* coaxial line */
      {
	 if ( opr.n[2] != null ) /* 1: inner conductor temperature limit */
	 {
            opr.n[2] = ONE;
	    
            cpylne( opr.ntx[2],
               "fixed_CW_power_/_inner_conductor_temperature_limit", 
               "0/1", ll );
         };

         if (( opr.n[8] == TWO )
           ||( opr.n[8] == THREE ))
         {
            cpylne( opr.ntx[8],
               "mantle/_pipe/_both/_no/_side_temperatures_fixed",
     	       "1/2/3/0", ll );
		  
            if ( opr.n[9] == THREE )
            {
               opr.n[9] = ONE;
	       
               cpylne( opr.ntx[9],
                  "mantle/_pipe/_both/_no/_side_heat_outflow_fixed",
	          "1/2/3/0", ll );
            }
	    else if ( opr.n[9] == TWO )
            {
               opr.n[9] = null;

               cpylne( opr.ntx[9],
                  "mantle/_pipe/_both/_no/_side_heat_outflow_fixed",
	          "1/2/3/0", ll );
            }
	    else
	    {
               cpylne( opr.ntx[9],
                  "mantle/_pipe/_both/_no/_side_heat_outflow_fixed",
	          "1/2/3/0", ll );
            };
         }
         else if ( opr.n[8] == ONE )
         {
            cpylne( opr.ntx[8],
               "mantle/_pipe/_both/_no/_side_temperatures_fixed",
	       "1/2/3/0", ll );

            if ( opr.n[9] == ONE )
	    {
               opr.n[9] = null;
	       
               cpylne( opr.ntx[9],
                  "mantle/_pipe/_both/_no/_side_heat_outflow_fixed",
	          "1/2/3/0", ll );
	    }
            else if ( opr.n[9] == THREE )
            {
               opr.n[9] = TWO;

               cpylne( opr.ntx[9],
                  "mantle/_pipe/_both/_no/_side_heat_outflow_fixed",
	          "1/2/3/0", ll );
            };
         }
	 else
	 {
	    opr.n[8] = null;
	    
            cpylne( opr.ntx[8],
               "mantle/_pipe/_both/_no/_side_temperatures_fixed",
               "1/2/3/0", ll );

            cpylne( opr.ntx[9],
               "mantle/_pipe/_both/_no/_side_heat_outflow_fixed",
               "1/2/3/0", ll );
	 };
      }; /* end, coaxial line case */
   }
   else /* fixed mantle temperature */
   {
      opr.n[5] = null;
      opr.n[6] = null;
      cpylne( opr.ntx[6],
         "free_/_forced_convection,_environment", inact, ll );
	 
/* fixed side temperature [or heat outflow] one one side of waveguide mantle */
/* doesn't make sense for fixed mantle temperature - hence: */

      if ( opr.n[1] == null ) /* in rectangular waveguides */
      {
         opr.n[8] = null;
         opr.n[9] = null;
      }
      else /* coaxial line */
      {
         if ( opr.n[8] == ONE ) /* fixed side temperature, mantle */
         {
            opr.n[8] = null;

            if ( opr.n[9] == ONE )
               opr.n[9] = null;
            else if ( opr.n[9] == THREE )
	    {
               opr.n[9] = TWO;
               cpylne( opr.ntx[9],
                  "mantle/_pipe/_both/_no/_side_heat_outflow_fixed",
	          "1/2/3/0", ll );
            };
         }
         else if ( opr.n[8] == TWO ) /* fixed side temperature, pipe */
         {
            cpylne( opr.ntx[8],
               "mantle/_pipe/_both/_no/_side_temperatures_fixed",
               "1/2/3/0", ll );
	       
            opr.n[9] = null;
         }
         else if ( opr.n[8] == THREE ) /* side temperature, pipe and mantle */
         {
            opr.n[8] = TWO; /* side temperature, pipe */
            cpylne( opr.ntx[8],
               "mantle/_pipe/_both/_no/_side_temperatures_fixed",
               "1/2/3/0", ll );

            opr.n[9] = null;
         }
	 else
	 {
            opr.n[8] = null;
         };

         if ( opr.n[9] == ONE )
            opr.n[9] = null;
         else if ( opr.n[9] == TWO )
         {
            cpylne( opr.ntx[9],
               "mantle/_pipe/_both/_no/_side_heat_outflow_fixed",
	       "1/2/3/0", ll );
         }
	 else
            opr.n[9] = null;
      }; /* end, coaxial line case */
   }; /* end, fixed mantle temperature */
/*...........................................................................*/
/* check number of frequency points: */

   if ( opr.n[10] < ONE )
      opr.n[10] = ONE; /* at least one frequency */
   else if ( FREQPTS < opr.n[10] )
      opr.n[10] = FREQPTS;
/*............................................................................*/
# ifndef _Forced

   if (( null != ( opr.n[3] ))
     &&( null != ( opr.n[4] )))
   {
      fprintf( stderr, "\n Computation mode %d in item 4 ", opr.n[4] );
      fprintf( stderr, "[ forced convection ]" );
      fprintf( stderr, "\n is in preparation" );
      fprintf( stderr, " [ not yet implemented ]." );
      fprintf( stderr, "\n More details from <contact@sfenx.de> ].\n\n" );

      exit( EXIT_FAILURE );
   };

   if ((( opr.n[5] == ONE ))
     &&(( opr.n[6] == ONE )))
   {
      fprintf( stderr, "\n Computation mode %d in item 6 ", opr.n[6] );
      fprintf( stderr, "[ forced convection ]" );
      fprintf( stderr, "\n is in preparation" );
      fprintf( stderr, " [ not yet implemented ]." );
      fprintf( stderr, "\n More details from <contact@sfenx.de> ].\n\n" );

      exit( EXIT_FAILURE );
   };

   if ( ONE == ( opr.n[7] ))
   {
      fprintf( stderr, "\n Computation mode %d in item 7 ", opr.n[7] );
      fprintf( stderr, "[ interior cooling ]" );
      fprintf( stderr, "\n is in preparation" );
      fprintf( stderr, " [ not yet implemented ]." );
      fprintf( stderr, "\n More details from <contact@sfenx.de> ].\n\n" );

      exit( EXIT_FAILURE );
   };
# endif
/*............................................................................*/
   ii = null; do
   {
      opr.opt[ii] = ( unsigned char ) opr.n[ii+ONE] + 48; /* 48 = ASCII null */
   } while (( ++ii ) < opr.n[null] );

   return null;
}
/*============================================================================*/
# undef LINLEN
/*********************** end of function rvise_opr(*) *************************/
