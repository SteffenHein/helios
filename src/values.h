/* [ file: values.h ] */
/*******************************************************************************
*                                                                              *
*   Function body values(*); HELIOS release v1.0r1                             *
*   [ Heat and Electric Losses In Ordinary wave guiding Systems ]              *
*                                                                              *
*   This function generates text and graphics files for results computed with  *
*   program HELIOS                                                             *
*                                                                              *
*   (C) SHEIN; Munich, December 2021                       Steffen Hein        *
*   [ Update: December 17, 2021 ]                       <contact@sfenx.de>     *
*                                                                              *
*******************************************************************************/
# ifndef RES_MAXTMP
   # define RES_MAXTMP 1 /* [0] 1: [ don't] plot maximum temperatures */
# endif
/*============================================================================*/

RESULTS *\
values( HELIOSSTATE *state )
{
/* allusions: */

/* extern HELIOSSTATE helios; *//* [hlswrk] menu state transfer structure */

/* declarations: */

   static RESULTS
     *rsp;

# if (( RES_CW_PWR == 1 ) \
    ||( RES_TEMPRS == 1 ) \
    ||( RES_LOSSES == 1 ))
    
   static GRAPHICS
      gph = {null},
     *grp = &gph;
# endif
/*
   static char
      ptr[STS_SIZE] = {null};
    **endp = null;
*/
   static short
      ii = null,
      jj = null,
      frn = null;

   static long 
      ll = null;

# if (( RES_OPRLOG == 1 ) \
    ||( RES_CW_PWR == 1 ))
   static double
      attn = ZERO;
# endif

# if (( RES_CW_PWR == 1 ) \
    ||( RES_TEMPRS == 1 ) \
    ||( RES_LOSSES == 1 ) \
    ||( RES_OPRLOG == 1 ) \
    ||( RES_TMPLOG == 1 ) \
    ||( RES_CHCLOG == 1 ))

   static FILE
     *grplog = NULL,
     *valfle = NULL;

   static char
      valptr[STS_SIZE] = {null},
      strptr[STS_SIZE] = {null};
/*............................................................................*/
/* time_t types etc.: */

   static char
     timeptr[STS_SIZE] = {null};

   time_t
      nseconds = null,
     *timer = null;
/*............................................................................*/
/* prototyping: */

   time_t 
      time( time_t *timer );

   void
      store_val( HELIOSSTATE *state, FILE *stream, char opt );
# endif
/*............................................................................*/
# ifndef _CCBUG
   char
     *strcpy( char *ptr1, const char *ptr2 ),
     *strcat( char *ptr1, const char *ptr2 ),
     *strncat( char *ptr1, const char *ptr2, size_t n );
# endif

   char
     *lotos( long mm, char cc );

   char
     *dotos( double x, char precision, char *format );

   int
      graphp( GRAPHICS *grp );
/*----------------------------------------------------------------------------*/
/* initialize: */

   rsp = ( state->rsp );

   ii = ll; /* to use it once ...*/
   jj = ll;
/*............................................................................*/
   frn = ( state->elp->frn );
/*............................................................................*/
# if (( RES_CW_PWR == 1 ) \
    ||( RES_TEMPRS == 1 ) \
    ||( RES_LOSSES == 1 ))

   strcpy( strptr, "hls.pfs" );
   strcat( strptr, lotos(( state->job ), null ));

   grplog = fopen( strptr, "w+" );
   fprintf( grplog, "PLOT_FILES\nprogram_helios_[job_no_%s]\n",
     ( lotos(( state->job ), null )));
# endif

# if (( RES_OPRLOG == 1 ) \
    ||( RES_TMPLOG == 1 ) \
    ||( RES_CHCLOG == 1 ))

   strcpy( valptr, "hls.val" );
   strcat( valptr, lotos(( state->job ), null ));

   valfle = fopen( valptr, "w+" );
   fprintf( valfle,
      "HELIOS RESULTS FILE %c%s%c\n[program helios, job no %s]",
      39, valptr, 39, ( lotos(( state->job ), null )));
# endif
/*............................................................................*/
/* log: operation parameters [ CW power, losses, attenuation ] */
# if RES_OPRLOG == 1
   store_val( state, valfle, 'o' );
# endif
/*............................................................................*/
   grp->ptsize = 1; /* point size of plotted data points */
   
# if RES_CW_PWR == 1
/* cw power: */

   strcpy( strptr, lotos(( state->job ), null ));

   strcpy( gph.file, "cwp" );
   strcat( gph.file, strptr );
   strcpy( gph.format, "gnuplot_2D" );
   strcpy( gph.name, "program HELIOS" );
   strcpy( gph.text, "- cw_power" );
   strcpy( gph.xunit, "Hz" );
   strcpy( gph.yunit, "kW" );

   ii = null;
   while ( ii < frn )
   {
      gph.vct[0][ii] = ( rsp->freq[ii] );
      gph.vct[1][ii]  = ( rsp->cwpr[ii] )/1000.;
      ii++ ;
   };

   strcpy( gph.curve[0], "cwp" );
/*............................................................................*/
   if ((( state->opmrk[1] ) == ONE )   /* fixed side conditions */
     &&(( state->opp->n[2] ) == ONE )) /* temperature limited */
   {
/*............................................................................*/
# if HLS_LBLWGL == 0
      strcat( gph.curve[0], strptr );
# elif HLS_LBLWGL == 1
      strcat( gph.curve[0], "[WGL=" );
      strcat( gph.curve[0], dotos(( state->dmp->lwgd ), 3, "e" ));
      strcat( gph.curve[0], "m]" );
# endif
/*............................................................................*/
   }
   else
      strcat( gph.curve[0], strptr );

   fprintf( grplog, "\n%-7s [cw power]", gph.curve[0] );

   if (( state->uif ) == 'f' )
      ( grp->dsp ) = null; /* silent mode */
   else
      ( grp->dsp ) = null; /* message mode */

/* number of curves [ nc ]: */
   ( grp->nc ) = ONE;

/* number of frequencies [ abscisse ]  */
   ( grp->nn ) = frn;

/*............................................................................*/
   ii = graphp( grp );    /* create CW power graphics file */
/*......................*/

/*............................................................................*/
/* attenuation: */

   strcpy( strptr, lotos(( state->job ), null ));

   strcpy( gph.file, "att" );
   strcat( gph.file, strptr );
   strcpy( gph.format, "gnuplot_2D" );
   strcpy( gph.name, "program HELIOS" );
   strcpy( gph.text, "- attenuation" );
   strcpy( gph.xunit, "Hz" );
   strcpy( gph.yunit, "dB/m" );

   ii = null;
   while ( ii < frn )
   {
      gph.vct[0][ii] = ( rsp->freq[ii] );
      attn = ( rsp->lsst[ii] )/( rsp->cwpr[ii] ); /* attenuation [Neper/m] */
/*
*//* [ 10./log( 10. ) = 4.342944819 ] 
*/
      gph.vct[1][ii] = ( 10.*attn/log( 10. ));
      ii++ ;
   };

   strcpy( gph.curve[0], "att" );
   strcat( gph.curve[0], strptr );
   fprintf( grplog, "\n%-7s [attenuation]", gph.curve[0] );

   if (( state->uif ) == 'f' )
      ( grp->dsp ) = null; /* silent mode */
   else
      ( grp->dsp ) = null; /* message mode */

/* number of curves [ nc ]: */
   ( grp->nc ) = ONE;

/* number of frequencies [ abscisse ]  */
   ( grp->nn ) = frn;

/*............................................................................*/
   ii = graphp( grp );    /* create attenuation graphics file */
/*......................*/

/*............................................................................*/
/* side heat currents: */

   if (( state->opmrk[1] ) == ONE )   /* fixed side conditions */
   {
      strcpy( strptr, lotos(( state->job ), null ));

      strcpy( gph.file, "shc" );
      strcat( gph.file, strptr );
      strcpy( gph.format, "gnuplot_2D" );
      strcpy( gph.name, "program HELIOS" );
/*............................................................................*/
# if HLS_LBLWGL == 0
      strcpy( gph.text, "- side heat current" );
# elif HLS_LBLWGL == 1
      strcpy( gph.text, "- side heat current" );
      strcat( gph.text, " [WGL=" );
      strcat( gph.text, dotos(( state->dmp->lwgd ), 3, "e" ));
      strcat( gph.text, "m]" );
# endif
/*............................................................................*/
      strcpy( gph.xunit, "Hz" );
      strcpy( gph.yunit, "W" );

      ii = null;
      while ( ii < frn )
      {
         jj = null;
         gph.vct[jj][ii] = ( rsp->freq[ii] );
	 jj++;
	 
         if (( state->opp->n[1] ) == null ) /* rectangular waveguide */
         {
            gph.vct[jj][ii] = ( rsp->sdofm[ii] );
	    jj++;
         }
         else /* if (( state->opp->n[1] ) == ONE ) - coaxial waveguide */
         {
            if (( state->cyl[5] ) == ONE )
            {
               gph.vct[jj][ii] = ( rsp->sdofp[ii] );
	       jj++;
            };

            if (( state->cyl[6] ) == ONE )
            {
               gph.vct[jj][ii] = ( rsp->sdofm[ii] );
	       jj++;
            };
         }; /* end if (( state->opp->n[1] ) == ONE ) - coaxial waveguide */
         ii++ ;
      }; /* end while ii ... */
      
      jj = null;
      if (( state->opp->n[1] ) == null ) /* rectangular waveguide */
      {
         strcpy( gph.curve[jj], "shm" );
/*............................................................................*/
# if HLS_LBLWGL == 0
         strcat( gph.curve[jj], strptr );
# elif HLS_LBLWGL == 1
         strcat( gph.curve[jj], "[WGL=" );
         strcat( gph.curve[jj], dotos(( state->dmp->lwgd ), 3, "e" ));
         strcat( gph.curve[jj], "m]" );
# endif
/*............................................................................*/
         fprintf( grplog,
            "\n%-7s [side_heat_current, mantle]", gph.curve[jj] );
         jj++;
      }
      else /* if (( state->opp->n[1] ) == ONE ) - coaxial waveguide */
      {
         if (( state->cyl[5] ) == ONE )
         {
            strcpy( gph.curve[jj], "shp" );
/*............................................................................*/
# if HLS_LBLWGL == 0
            strcat( gph.curve[jj], strptr );
# elif HLS_LBLWGL == 1
            strcat( gph.curve[jj], "[WGL=" );
            strcat( gph.curve[jj], dotos(( state->dmp->lwgd ), 3, "e" ));
            strcat( gph.curve[jj], "m]" );
# endif
/*............................................................................*/
            fprintf( grplog,
               "\n%-7s [side_heat_current, pipe]", gph.curve[jj] );
            jj++;
         }; /* end if (( state->cyl[5] ) == ONE ) */

         if (( state->cyl[6] ) == ONE )
         {
            strcpy( gph.curve[jj], "shm" );
/*............................................................................*/
# if HLS_LBLWGL == 0
            strcat( gph.curve[jj], strptr );
# elif HLS_LBLWGL == 1
            strcat( gph.curve[jj], "[WGL=" );
            strcat( gph.curve[jj], dotos(( state->dmp->lwgd ), 3, "e" ));
            strcat( gph.curve[jj], "m]" );
# endif
/*............................................................................*/
            fprintf( grplog,
               "\n%-7s [side_heat_current, mantle]", gph.curve[jj] );
            jj++;
         }; /* end if (( state->cyl[6] ) == ONE ) */
      }; /* end if (( state->opp->n[1] ) == ONE ) - coaxial waveguide */
/*............................................................................*/
      if (( state->uif ) == 'f' )
         ( grp->dsp ) = null; /* silent mode */
      else
         ( grp->dsp ) = null; /* message mode */

/* number of curves [ nc ]: */
      ( grp->nc ) = jj;

/* number of frequencies [ abscisse ]  */
      ( grp->nn ) = frn;

/*............................................................................*/
      ii = graphp( grp );    /* create side heat current graphics file        */
/*.........................*/

   }; /* end if ((( state->opmrk[1] ) == ONE ) - fixed side temperature */
# endif /* RES_CW_PWR == 1 */
/*............................................................................*/
/* log: losses, attenuation */ 
# if RES_TMPLOG == 1
   store_val( state, valfle, 't' );
# endif
/*............................................................................*/
# if RES_TEMPRS == 1
/* temperatures: */

   strcpy( strptr, lotos(( state->job ), null ));

   strcpy( gph.file, "tmp");
   strcat( gph.file, strptr );
   strcpy( gph.format, "gnuplot_2D" );
   strcpy( gph.name, "program HELIOS" );
   strcpy( gph.text, "- temperatures" );
   strcpy( gph.xunit, "Hz" );
/*............................................................................*/
# if RES_KLVTDC == 0
   strcpy( gph.yunit, "Kelvin" );
# else
   strcpy( gph.yunit, "°Celsius" );
# endif
/*............................................................................*/
   if (( state->opp->n[1] ) == null ) /* rectangular waveguide */
   {
/*----------------------------------------------------------------------------*/
/* GRAPHICS, RECT. WG */
/*----------------------------------------------------------------------------*/
      ii = null;
      while ( ii < frn )
      {
	 jj = null;
         gph.vct[jj][ii] = ( rsp->freq[ii] );
	 jj++;
	 
         if ((( state->cyl[1] ) == ONE ) /* waveguide with separate mantle */
           ||(( state->thp->mbe )  == ONE ))
         {
            gph.vct[jj][ii] = ( rsp->tinn[ii] );
/*............................................................................*/
# if RES_KLVTDC == 1
            gph.vct[jj][ii] -= CELSIUS_TO_KELVIN;
# endif
/*............................................................................*/
            jj++;

            gph.vct[jj][ii] = ( rsp->tjck[ii] );
/*............................................................................*/
# if RES_KLVTDC == 1
            gph.vct[jj][ii] -= CELSIUS_TO_KELVIN;
# endif
/*............................................................................*/
            jj++;
         }
         else /* waveguide and mantle not separated */
         {
            gph.vct[jj][ii] = ( rsp->tjck[ii] );
/*............................................................................*/
# if RES_KLVTDC == 1
            gph.vct[jj][ii] -= CELSIUS_TO_KELVIN;
# endif
/*............................................................................*/
            jj++;
         };

         if (( state->cyl[0] ) == ONE ) /* dielectric */
         {
            gph.vct[jj][ii] = ( rsp->tdel[ii] );
/*............................................................................*/
# if RES_KLVTDC == 1
            gph.vct[jj][ii] -= CELSIUS_TO_KELVIN;
# endif
/*............................................................................*/
            jj++;
         };

         if (( state->cyl[3] ) == ONE )
         {
/* side temperature, mantle: */
            gph.vct[jj][ii] = ( rsp->trfm[ii] );
/*............................................................................*/
# if RES_KLVTDC == 1
            gph.vct[jj][ii] -= CELSIUS_TO_KELVIN;
# endif
/*............................................................................*/
            jj++;
/*............................................................................*/
# if RES_MAXTMP == 1 /* [ maximum temperature, mantle ] */
            gph.vct[jj][ii] = ( rsp->tmxm[ii] );
/*............................................................................*/
# if RES_KLVTDC == 1
            gph.vct[jj][ii] -= CELSIUS_TO_KELVIN;
# endif
/*............................................................................*/
            jj++;
# endif
/*............................................................................*/
         };
         ii++ ;
      }; /* end while ( ii < frn ) */
/*----------------------------------------------------------------------------*/
/* LEGENDE, RECT. WG */
/*----------------------------------------------------------------------------*/
      jj = null;
      if ((( state->cyl[1] ) == ONE ) /* waveguide with separate mantle */
        ||(( state->thp->mbe ) == ONE ))
      {
         strcpy( gph.curve[jj], "ti" ); /* inner temperature, waveguide */
         strcat( gph.curve[jj], strptr );
         jj++;
	 
         strcpy( gph.curve[jj], "tm" ); /* '', mantle */
	 if (( state->cyl[3] ) == ONE )
         {
/*............................................................................*/
# if HLS_LBLWGL == 0
            strcat( gph.curve[jj], strptr );
# elif HLS_LBLWGL == 1
            strcat( gph.curve[jj], "[WGL=" );
            strcat( gph.curve[jj], dotos(( state->dmp->lwgd ), 3, "e" ));
            strcat( gph.curve[jj], "m]" );
# endif
/*............................................................................*/
         }
	 else
            strcat( gph.curve[jj], strptr );

         fprintf( grplog,
           "\n%-7s [temperature, mantle]", gph.curve[jj] );
         jj++;
      }
      else /* waveguide and mantle not separated */
      {
         strcpy( gph.curve[jj], "tm" ); /* mantle temperature, waveguide */
         strcat( gph.curve[jj], strptr );
         fprintf( grplog,
           "\n%-7s [temperature, mantle]", gph.curve[jj] );
         jj++;
       };

      if (( state->cyl[0] ) == ONE )
      {
         strcpy( gph.curve[jj], "td" ); /* '', dielectric */
         strcat( gph.curve[jj], strptr );
         fprintf( grplog,
	    "\n%-7s [temperature, dielectric]",
            gph.curve[jj] );
         jj++;
      };

      if (( state->cyl[3] ) == ONE )
      {
         strcpy( gph.curve[jj], "tsdm" ); /* side temperature, mantle */
         strcat( gph.curve[jj], strptr );
         fprintf( grplog,
            "\n%-7s [ side temperature, mantle ]",
            gph.curve[jj] );
         jj++;
/*............................................................................*/
# if RES_MAXTMP == 1 /* [ maximum temperature, mantle ] */
         strcpy( gph.curve[jj], "tmxm" ); 
         strcat( gph.curve[jj], strptr );
         fprintf( grplog,
            "\n%-7s [ maximum temperature, mantle]",
            gph.curve[jj] );
         jj++;
# endif
/*............................................................................*/
      };
   }
   else /* if (( opp->n[1] ) == ONE ) - coaxial waveguide */
   {
/*----------------------------------------------------------------------------*/
/* GRAPHICS, COAXIAL WG */
/*----------------------------------------------------------------------------*/
      ii = null;
      while ( ii < frn )
      {
         jj = null;
         gph.vct[jj][ii] = ( rsp->freq[ii] );
         jj++;
	 
         gph.vct[jj][ii] = ( rsp->tinn[ii] );
/*............................................................................*/
# if RES_KLVTDC == 1
         gph.vct[jj][ii] -= CELSIUS_TO_KELVIN;
# endif
/*............................................................................*/
         jj++;
	 
         if ((( state->cyl[3] ) == ONE ) /* outer conductor */
           ||(( state->thp->mbe ) == ONE ))
         {
            gph.vct[jj][ii] = ( rsp->tjck[ii] );
/*............................................................................*/
# if RES_KLVTDC == 1
            gph.vct[jj][ii] -= CELSIUS_TO_KELVIN;
# endif
/*............................................................................*/
            jj++;
         };

         if (( state->cyl[2] ) == ONE ) /* lossy dielectric */
         {
            gph.vct[jj][ii] = ( rsp->tdel[ii] );
/*............................................................................*/
# if RES_KLVTDC == 1
            gph.vct[jj][ii] -= CELSIUS_TO_KELVIN;
# endif
/*............................................................................*/
	    jj++;
         };

         if (( state->cyl[4] ) == ONE ) /* mantle */
         {
            gph.vct[jj][ii] = ( rsp->tjck[ii] );
/*............................................................................*/
# if RES_KLVTDC == 1
            gph.vct[jj][ii] -= CELSIUS_TO_KELVIN;
# endif
/*............................................................................*/
            jj++;
         };

         if ((( state->cyl[0] ) == ONE ) /* inner conductor cooling */
           ||(( state->thp->ppe ) == ONE ))
         {
            gph.vct[jj][ii] = ( rsp->ttub[ii] );
/*............................................................................*/
# if RES_KLVTDC == 1
            gph.vct[jj][ii] -= CELSIUS_TO_KELVIN;
# endif
/*............................................................................*/
            jj++;

            gph.vct[jj][ii] = ( rsp->tflr[ii] );
/*............................................................................*/
# if RES_KLVTDC == 1
            gph.vct[jj][ii] -= CELSIUS_TO_KELVIN;
# endif
/*............................................................................*/
            jj++;
         };

         if (( state->cyl[5] ) == ONE )
         {
/* side temperature, pipe: */
            gph.vct[jj][ii] = ( rsp->trfp[ii] );
/*............................................................................*/
# if RES_KLVTDC == 1
            gph.vct[jj][ii] -= CELSIUS_TO_KELVIN;
# endif
/*............................................................................*/
            jj++;
/*............................................................................*/
# if RES_MAXTMP == 1 /* [ maximum temperature, pipe ] */
            gph.vct[jj][ii] = ( rsp->tmxp[ii] );
/*............................................................................*/
# if RES_KLVTDC == 1
            gph.vct[jj][ii] -= CELSIUS_TO_KELVIN;
# endif
/*............................................................................*/
            jj++;
# endif
/*............................................................................*/
         };

         if (( state->cyl[6] ) == ONE )
         {
/* side temperature, mantle: */
            gph.vct[jj][ii] = ( rsp->trfm[ii] );
/*............................................................................*/
# if RES_KLVTDC == 1
            gph.vct[jj][ii] -= CELSIUS_TO_KELVIN;
# endif
/*............................................................................*/
            jj++;
/*............................................................................*/
# if RES_MAXTMP == 1 /* [ maximum temperature, mantle ] */
            gph.vct[jj][ii] = ( rsp->tmxm[ii] );
/*............................................................................*/
# if RES_KLVTDC == 1
            gph.vct[jj][ii] -= CELSIUS_TO_KELVIN;
# endif
/*............................................................................*/
            jj++;
# endif
/*............................................................................*/
         };
         ii++ ;
      }; /* end while ( ii < frn ) */
/*----------------------------------------------------------------------------*/
/* LEGENDE, COAXIAL WG */
/*----------------------------------------------------------------------------*/
      jj = null;
      strcpy( gph.curve[jj], "ti" ); /* temperature, inner conductor */
      if (( state->cyl[5] ) == ONE )
      {
/*............................................................................*/
# if HLS_LBLWGL == 0
         strcat( gph.curve[jj], strptr );
# elif HLS_LBLWGL == 1
         strcat( gph.curve[jj], "[WGL=" );
         strcat( gph.curve[jj], dotos(( state->dmp->lwgd ), 3, "e" ));
         strcat( gph.curve[jj], "m]" );
# endif
/*............................................................................*/
      }
      else
         strcat( gph.curve[jj], strptr );

      fprintf( grplog,
         "\n%-7s [temperature, inner conductor]",
         gph.curve[jj] );
      jj++;

      if ((( state->cyl[3] ) == ONE ) /* outer conductor */
        ||(( state->thp->mbe ) == ONE ))
      {
         strcpy( gph.curve[jj], "to" ); /* '', outer conductor */
         strcat( gph.curve[jj], strptr );
         fprintf( grplog,
            "\n%-7s [temperature, outer conductor]",
	    gph.curve[jj] );
         jj++;
      };

      if (( state->cyl[2] ) == ONE )
      {
         strcpy( gph.curve[jj], "td" ); /* '', dielectric */
         strcat( gph.curve[jj], strptr );
         fprintf( grplog, "\n%-7s [temperature, dielectric]",
	    gph.curve[jj] );
         jj++;
      };

      if (( state->cyl[4] ) == ONE )
      {
         strcpy( gph.curve[jj], "tm" ); /* '', mantle */
	 if (( state->cyl[6] ) == ONE )
         {
/*............................................................................*/
# if HLS_LBLWGL == 0
            strcat( gph.curve[jj], strptr );
# elif HLS_LBLWGL == 1
            strcat( gph.curve[jj], "[WGL=" );
            strcat( gph.curve[jj], dotos(( state->dmp->lwgd ), 3, "e" ));
            strcat( gph.curve[jj], "m]" );
# endif
/*............................................................................*/
         }
	 else
            strcat( gph.curve[jj], strptr );
	    
         fprintf( grplog,
            "\n%-7s [temperature, mantle]",
            gph.curve[jj] );
         jj++;
      };

      if ((( state->cyl[0] ) == ONE )
        ||(( state->thp->ppe ) == ONE ))
      {
         strcpy( gph.curve[jj], "tp" ); /* '', cooling pipe */
         strcat( gph.curve[jj], strptr );
         fprintf( grplog,
            "\n%-7s [temperature, cooling pipe]",
            gph.curve[jj] );
         jj++;

         strcpy( gph.curve[jj], "tr" ); /* '', temperature rise, coolant */
         strcat( gph.curve[jj], strptr );
         fprintf( grplog,
            "\n%-7s [temperature rise, coolant]",
            gph.curve[jj] );
         jj++;
      };

      if (( state->cyl[5] ) == ONE )
      {
         strcpy( gph.curve[jj], "tsdp" ); /* side temperature, pipe */
         strcat( gph.curve[jj], strptr );
         fprintf( grplog,
            "\n%-7s [side temperature, pipe]",
            gph.curve[jj] );
         jj++;
/*............................................................................*/
# if RES_MAXTMP == 1 /* [ maximum temperature, pipe ] */
         strcpy( gph.curve[jj], "tmxp" );
         strcat( gph.curve[jj], strptr );
         fprintf( grplog,
            "\n%-7s [ maximum temperature, pipe ]",
            gph.curve[jj] );
         jj++;
# endif
/*............................................................................*/
      };

      if (( state->cyl[6] ) == ONE )
      {
         strcpy( gph.curve[jj], "tsdm" ); /* side temperature, mantle */
         strcat( gph.curve[jj], strptr );
         fprintf( grplog,
            "\n%-7s [ side temperature, mantle ]",
            gph.curve[jj] );
         jj++;
/*............................................................................*/
# if RES_MAXTMP == 1 /* [ maximum temperature, mantle ] */
         strcpy( gph.curve[jj], "tmxm" );
         strcat( gph.curve[jj], strptr );
         fprintf( grplog,
            "\n%-7s [maximum temperature, mantle]",
            gph.curve[jj] );
         jj++;
# endif
/*............................................................................*/
      };
   }; /* end if (( opp->n[1] == ONE )) - coaxial waveguide */
/*............................................................................*/
/* number of curves [ nc ]: */
   ( grp->nc ) = jj;
   
/* number of frequencies [ abscisse ]  */
   ( grp->nn ) = frn;

/*............................................................................*/
   ii = graphp( grp );     /* create graphics file for inner cond. temperature*/
/*.......................*/

# endif /* RES_TEMPRS == 1 */
/*............................................................................*/
# if RES_LOSSES == 1
/* losses: */

   strcpy( strptr, lotos(( state->job ), null ));
   
   strcpy( gph.file, "lss" );
   strcat( gph.file, strptr );
   strcpy( gph.format, "gnuplot_2D" );
   strcpy( gph.name, "program HELIOS" );
   strcpy( gph.text, "- losses" );
   strcpy( gph.xunit, "Hz" );
   strcpy( gph.yunit, "W/m" );

   ii = null;
   while ( ii < frn )
   {
      jj = null;
      gph.vct[jj][ii] = ( rsp->freq[ii] );
      jj++;
      
      gph.vct[jj][ii] = ( rsp->lsst[ii] );
      jj++;
      
      gph.vct[jj][ii] = ( rsp->lssi[ii] );
      jj++;
      
      if (( state->opp->n[1] ) == ONE ) /* coaxial waveguide */
      {
         gph.vct[jj][ii] = ( rsp->lsso[ii] );
         jj++;
      };

      if (( state->opp->n[3] ) == null ) /* solid [lossy] dielectric */
      {
         gph.vct[jj][ii] = ( rsp->lssd[ii] );
         jj++;
      };
/*............................................................................*/
/* fixed side conditions */

      if (( state->opp->n[1] ) == null ) /* rectangular waveguide */
      {
/*............................................................................*/
/* waveguide mantle */

         if (( state->cyl[3] ) == ONE )
         {
            gph.vct[jj][ii] = ( rsp->sdedm[ii] );
            jj++;

         };
      } /* end, rectangular waveguide case */ 
/*............................................................................*/
      else /* coaxial line */
      {
/*............................................................................*/
/* outer conductor ['mantle']: */

         if (( state->cyl[5] ) == ONE )
         {
            gph.vct[jj][ii] = ( rsp->sdedp[ii] );
            jj++;
         };
/*............................................................................*/
/* inner conductor ['pipe']: */

         if (( state->cyl[6] ) == ONE )
         {
            gph.vct[jj][ii] = ( rsp->sdedm[ii] );
            jj++;
         };
      }; /* end, coaxial line */
      ii++ ;
   };
/*............................................................................*/
/* electric losses: */

   strcpy( gph.curve[0], "lt" );
   strcat( gph.curve[0], strptr );

   fprintf( grplog, "\n%-7s [total losses]", gph.curve[0] );

   strcpy( gph.curve[1], "li" );
   strcat( gph.curve[1], strptr );

   if (( state->opp->n[1] ) == null ) /* rectangular waveguide */
      fprintf( grplog, "\n%-7s [losses, skin effect]", gph.curve[1] );
   else /* coaxial waveguide */
      fprintf( grplog, "\n%-7s [losses, inner conductor]", gph.curve[1] );
/*............................................................................*/
/* number of curves [ nc ]: */

   jj = TWO;
   if (( state->opp->n[1] ) == ONE ) /* coaxial waveguide */
   {
      strcpy( gph.curve[jj], "lo" );
      strcat( gph.curve[jj], strptr );
      fprintf( grplog, "\n%-7s [losses, outer conductor]", gph.curve[jj] );
      jj++;
   };
/*............................................................................*/
/* lossy dielectric: */

   if (( state->opp->n[3] ) == null )
   {
      strcpy( gph.curve[jj], "ld" );
      strcat( gph.curve[jj], strptr );
      fprintf( grplog, "\n%-7s [losses, dielectric]", gph.curve[jj] );
      jj++;
   };
/*............................................................................*/
/* fixed side temperatures: */

   if (( state->opp->n[1] ) == null ) /* rectangular waveguide */
   {
      if (( state->cyl[3] ) == ONE )
      {
         strcpy( gph.curve[jj], "sdm" );
         strcat( gph.curve[jj], strptr );
         fprintf( grplog, "\n%-7s [side_heat_outflow_density,_mantle]", 
            gph.curve[jj] );
         jj++;
      };
   } /* end, rectangular waveguide case */
/*............................................................................*/
   else /* if (( state->opp->n[1] ) == ONE ) - coaxial line */
   {
/*............................................................................*/
/* 'outer conductor' [ precisely: mantle ]: */

      if (( state->cyl[5] ) == ONE )
      {
         strcpy( gph.curve[jj], "sdp" );
         strcat( gph.curve[jj], strptr );
         fprintf( grplog, "\n%-7s [side_heat_outflow_density,_pipe]", 
            gph.curve[jj] );
         jj++;
      }; /* end, mantle */
/*............................................................................*/
/* 'inner conductor' [ precisely: pipe ]: */

      if (( state->cyl[6] ) == ONE )
      {
         strcpy( gph.curve[jj], "sdm" );
         strcat( gph.curve[jj], strptr );
         fprintf( grplog, "\n%-7s [side_heat_outflow_density,_mantle]", 
            gph.curve[jj] );
         jj++;
      }; /* end, pipe */
   }; /* end, coaxial line */
/*............................................................................*/
/* number of curves [ nc ]: */
   ( grp->nc ) = jj;

/* number of frequencies [ abscisse ]  */
   ( grp->nn ) = frn;

/*............................................................................*/
   ii = graphp( grp );       /* create graphics files */
/*.........................*/

# endif /* RES_LOSSES == 1 */
/*............................................................................*/
/* log files */
# if RES_CHCLOG == 1
   store_val( state, valfle, 'c' );
# endif
/*............................................................................*/
# if (( RES_CW_PWR == 1 ) \
    ||( RES_TEMPRS == 1 ) \
    ||( RES_LOSSES == 1 ))

   nseconds = time( timer );
   strcpy( timeptr, ctime( &nseconds ));

   fprintf( grplog, "\n\ngraphics log created\n%s", timeptr );
   fclose( grplog );
# endif
/*............................................................................*/
# if (( RES_OPRLOG == 1 ) \
    ||( RES_TMPLOG == 1 ) \
    ||( RES_CHCLOG == 1 ))

   nseconds = time( timer );
   strcpy( timeptr, ctime( &nseconds ));

   fprintf( valfle,
      "\nRESULTS file %s created\n%s", valptr, timeptr );
   fclose( valfle );
# endif
/*............................................................................*/
/*
   printf( "\n hlswrk terminated " );
*/ 
   return rsp;
}  
/*============================================================================*/
/************************ end of function values(*) ***************************/
/*
   Es ist gleichermassen toedlich fuer den Geist, ein System zu haben - und
   keines zu haben. Daher muss man sich dazu entschliessen, beides miteinander
   zu verbinden.
                                                          Friedrich Schlegel
*/
