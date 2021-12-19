/* [ file: hcndct.h ] */
/*******************************************************************************
*                                                                              *
*   Function body hcndct(*); HELIOS release v1.0r1                             *
*   [ Heat and Electric Losses In Ordinary wave guiding Structures ]           *
*                                                                              *
*   This function computes and inverts the heat conductance matrix for given   *
*   heat resistances between parallel conductors                               *
*                                                                              *
*   (C) SHEIN; Munich, December 2021                      Steffen Hein         *
*   [ Update: December 17, 2021 ]                       <contact@sfenx.de>     *
*                                                                              *
*******************************************************************************/
# define HCD_DEBUG 0
/*----------------------------------------------------------------------------*/
# ifndef HCD_ADJEXT   
   # define HCD_ADJEXT 1 /* [0]/1: [don't] re-adjust temperature maxima under */
# endif	                 /* side conditions at asymptotic behaviour lwgd->oo */
/*----------------------------------------------------------------------------*/
# ifndef HCD_MDDHYD      /* affects only rectangular waveguide inner cooling: */
   # define HCD_MDDHYD 0 /* 0: dhyd = 2*sqrt( F/PI ) [ 1: dhyd = 4*F/S ] */
# endif	                 /* 0 yields slightly higher temperatures [ secure ] */
/*----------------------------------------------------------------------------*/
# ifndef HLS_SDNODE
   # define HLS_SDNODE 1  /* [0]/1: [don't] use side nodes */
# endif
/*============================================================================*/

HCNDCT *hcndct( HELIOSSTATE *state )
{
/* allusions: */

/* declarations: */

   static HELIOSSTATE
     *stp;

   static OPERATIONS
     *opp;

   static DIMENSIONS
     *dmp;

   static HCNDCT
     *cdp;

   static ELECTRIC
     *elp;

   static THERMDYN
     *thp;

   static FILE
     *errfle;

   static short
      ii = null,
      jj = null,
      nds_ = null,
      rank = null,
      nodes = null;

   static const double
      eps = EPS_VAC,
      my = MY_VAC_,
      gamma = GRAVIT_ACC, /* gravitation acceleration [m/s^2] */
      css = STEFAN_BOLTZ; /* Stefan-Boltzman constant [W/(K^4*m^2)] */

   static double
      ss0 = ZERO,
      ss1 = ZERO,
      kk0 = ZERO,
      kk1 = ZERO,
      qq = ZERO,
      ss = ZERO,
      rscl = 1.;

   static double
      freq = ZERO, /* frequency [Hz] */
      cwpr = ZERO, /* CW power [W] */
      zeta = ZERO; /* value returned by grazet(*); essentlly a Nusselt number */

   static double
      lwgd = ZERO, /* waveguide length [m] */
      lwgp = ZERO, /* pipe length, rescaled [m] */
      lwgm = ZERO, /* mantle lengthi, rescaled [m] */
      djck = ZERO, /* outer [jacket] diameter [m] */
      dout = ZERO, /* outer conductor diameter [m] */
      dinn = ZERO, /* inner conductor diameter [m] */
      dtub = ZERO, /* inner [ cooling tube ] diameter [m] */
      dhyd = ZERO, /* hydraulic diameter [m] */
      winn = ZERO, /* inner waveguide width */
      hinn = ZERO, /* inner waveguide height */
      wout = ZERO, /* outer waveguide [ mantle ] width */
      hout = ZERO; /* outer waveguide [ mantle ] height */

   static double
      epd = ZERO, /* relative permittivity, dielectric */
      myd = ZERO, /* relative permeability, dielectric */
      tge = ZERO, /* electric loss tangent, dielectric */
      gre = ZERO, /* exterior Grashof number [ cylinder in gas ] */
      grd = ZERO, /* inner Grashof number [ between coaxial cylinders ] */
      emo = ZERO, /* emissivity, outer conductor */
      emi = ZERO, /* emissivity, inner conductor */
      eme = ZERO, /* emissivity, cable jacket */
      mcf = ZERO, /* Mutual Heat Conductance [W/(m^2*K)], fluid <-> clng tube */
      mcc = ZERO, /* MHC, cooling tube <-> inner conductor */
      mci = ZERO, /* MHC, inner conductor <-> dielectric [ or cooling gas ] */
      mco = ZERO, /* MHC, dielectric [ or cooling gas ] <-> outer conductor */
      mcr = ZERO, /* MHC, radiative inner conductor <-> outer conductor */
      mcj = ZERO, /* MHC, outer cond <-> mantel */
      mce = ZERO, /* MHC, mantel <-> environment */
      mcm = ZERO, /* MHC, effective mantle <-> front end */
      mcp = ZERO, /* MHC, effective pipe <-> front end */
      ny = ZERO,  /* cinematic viscosity [m^2/s] */
      nu = ZERO,  /* Nusselt number */
      rd = ZERO,  /* any radiative contribution to heat conductance [W/(m*K)] */
      hc = ZERO;  /* any heat conductance [W/(m*K)] */

   static double
      lssd = ZERO, /* dielectric losses [W/m] */
      tfld = ZERO, /* cooling fluid temperature [K] */
      ttub = ZERO, /* temperature, inner cooling tube [K] */
      tinn = ZERO, /* temperature, inner conductor [K] */
      tdel = ZERO, /* temperature, dielectric or cooling gas betw condcrs [K] */
      tout = ZERO, /* temperature, outer conductor [K] */
      tjck = ZERO, /* temperature, mantle [K] */
      tenv = ZERO, /* temperature, environment [K] */
      trfm = ZERO, /* transferred mantle temperatur on one side [K] */
      trfp = ZERO, /* transferred inner conductor temperatur on one side [K] */
      topm = ZERO, /* transferred mantle temperatur on one side [K] */
      topp = ZERO, /* transferred inner conductor temperatur on one side [K] */
      tmxm = ZERO, /* average mantle temperatur [K] */
      tmxp = ZERO, /* average pipe temperature [K] */
      tnrm = ZERO, /* normalized waveguide [dielectric] temperature */

      tmean = ZERO, /* any average temperature [K] */
      sdedm = ZERO, /* heat source density equivalent to side outflow, mantle */
      sdedp = ZERO, /* heat source density equivalent to side outflow, pipe */
      sdmcm = ZERO, /* efficient heat conductance to side, mantle [W/(m*K)]*/
      sdmcp = ZERO, /* efficient heat conductance to side, pipe [W/(m*K)] */
      
      sdofm = ZERO, /* heat outflow mantle, one side [W] */
      sdifm = ZERO, /* heat inflow mantle, opposite side [W] */
      sdofp = ZERO, /* heat outflow pipe, one side [W] */
      sdifp = ZERO; /* heat inflow pipe, opposite side [W] */
/*............................................................................*/
# if ( HCD_MCDMDE == 1 )\
   ||( HCD_MCDMDE == 2 ) /* the gradient method [ not always stable ] */
   static double
      grdi = ZERO, /* temperature gradient, inner conductor */
      grdo = ZERO; /* temperature gradient, in outer conductor */
# endif
/*............................................................................*/
# if HCD_MHCLOG == 1
   static char
      ptr[STS_SIZE] = {null};

# elif HCD_MHCLOG == 2
/*............................................................................*/
   static FILE
     *results = NULL;

   static char
      ptr[STS_SIZE] = {null},
      fleptr[STS_SIZE] = {null};

/* time_t types etc.: */

   time_t
      nseconds = null,
     *timer = null;
     
   static char
      timeptr[STS_SIZE] = {null};
/*............................................................................*/
/* prototyping: */

   time_t 
      time( time_t *timer );
# endif /* create log files HCD_... */
/*............................................................................*/
   double
      pow ( double base, double expnt );
   
   double
      sqrt ( double xx );

   char
     *lotos( long mm, char cc );

   char
      *dotos( double x, char precision, char *format );

   double
      sqrt( double xx );

   double
      log( double xx );

   double
      granus ( double gr );

   double
      grazet ( double gr, double ratio );
/*----------------------------------------------------------------------------*/
/* initialize structure pointers and parameters: */

   stp = state;

   opp = ( stp->opp );
   dmp = ( stp->dmp );
   elp = ( stp->elp );
   thp = ( stp->thp );
   cdp = ( stp->cdp );

   mcf = ZERO;
   mcc = ZERO;
   mcj = ZERO;
   mce = ZERO;
/*...........................................................................*/
/* compute mutual heat conductances [ inverse heat resistances ] between */
/* nodes [ the number of which, nodes, is simultaneously counted ] */

   freq = ( elp->freq );
   cwpr = ( elp->cwpr );

   epd = ( elp->epd );
   tge = ( elp->tge );
   myd = ( elp->myd );

   tenv = ( thp->tenv );
   tjck = ( thp->tjck );
   tinn = ( thp->tinn );
   tdel = ( thp->tdel );

   sdifm = ( thp->sdifm );
   sdofm = ( thp->sdofm );
   sdifp = ( thp->sdifp );
   sdofp = ( thp->sdofp );

   nds_ = null; /* reset node counter */
   lwgd = ( dmp->lwgd ); /* waveguide length */
/*............................................................................*/
# ifdef HLS_RESCLE
   rscl = HLS_RESCLE/( dmp->lwgd );
# endif
/*............................................................................*/
   if ( stp->opmrk[1] == ONE )
   {
      lwgp = lwgd; /* waveguide length [ in options opt.n[8/9] != null */
      lwgm = lwgd; /* waveguide length [ in options opt.n[8/9] != null */
/*............................................................................*/
/* [ smoothly approach waveguide length ] */
/* far from convergence - stretch the lengths: */ 

      lwgp *= ( 1. + ( HCD_STRCHP*( 1. - ( cdp->scl ))));
      lwgm *= ( 1. + ( HCD_STRCHM*( 1. - ( cdp->scl ))));
   };
/*............................................................................*/
/* RECTANGULAR WAVEGUIDE: */
/*............................................................................*/
   if ( opp->n[1] == null )
   { 
      winn = ( dmp->winn );
      hinn = ( dmp->hinn );
      wout = ( dmp->wout );
      hout = ( dmp->hout );
      tnrm = ( thp->tnrm );
/*............................................................................*/
/* fixed side conditions, waveguide mantle */

      if ( stp->cyl[3] == ONE )
      {
/*............................................................................*/
# if HLS_SDNODE == 1 /* count one side node */

         if (( stp->opp->n[8] == ONE )
           ||( stp->opp->n[8] == THREE )) 
            nds_++;
         else if (( stp->opp->n[9] == ONE )
                ||( stp->opp->n[9] == THREE )) 
            nds_++;
# endif
/*............................................................................*/
/* fixed side temperature, mantle: */
/*............................................................................*/
            if (( stp->opp->n[8] == ONE )
              ||( stp->opp->n[8] == THREE )) 
            {
               tmxm = ( thp->tmxm );
	       trfm = ( thp->tsdm );
	       sdifm = ( thp->sdifm );
/*............................................................................*/
/* heat conductance over mantle length: */
/* [ replace in the following tjck by tmxm to be on the safe side ]: */

               hc = ( thp->hcm ) + \
                  ( tjck - CELSIUS_TO_KELVIN )*( thp->dhm );
               hc *= ( wout*hout - winn*hinn );

	       mcm = hc/lwgm;
/*............................................................................*/
/* effective heat current density equivalent to side heat in- and outflow */

               sdedm = 3.*( mcm*( tjck - trfm ) - sdifm/2. )/lwgm;
/*............................................................................*/
/* homogeneity check: */
/* option 0/1/2: skip[no]/warning/error message */

               HMGCHK( "mantle", ( - sdedm ), ZERO, null );
/*............................................................................*/
/* heat outflow at mantle side: */

               sdofm = lwgm*sdedm + sdifm;
               ( thp->sdofm ) = sdofm;
/*............................................................................*/
/* endothermie check [ exclude tjck < trfm ]: */
/* option 0/1/2: skip[no]/warning/error message */

               ENDOTH( "mantle", sdifm, sdofm, null );

            } /* end [ fixed side temperature, mantle ] */
/*............................................................................*/
/* fixed heat outflow, mantle: */
/*............................................................................*/
            else if (( stp->opp->n[9] == ONE ) \
                   ||( stp->opp->n[9] == THREE ))
            {
               tjck = ( thp->tjck );
               tmxm = ( thp->tmxm );
	       sdifm = ( thp->sdifm );
	       sdofm = ( thp->sdofm );
/*............................................................................*/
/* endothermie check: */
/* option 0/1/2: skip[no]/warning/error message */

               ENDOTH( "mantle", sdifm, sdofm, null );
/*............................................................................*/
/* heat conductance over mantle length: */
/* [ replace in the following tjck by tmxm to be on the safe side ]: */

               hc = ( thp->hcm ) + \
                  ( tjck - CELSIUS_TO_KELVIN )*( thp->dhm );
               hc *= ( wout*hout - winn*hinn );

	       mcm = hc/lwgm;
/*............................................................................*/
/* effective heat current density equivalent to side heat in- and outflow */

               sdedm = ( sdofm - sdifm )/lwgm;
/*............................................................................*/
/* homogeneity check: */
/* option 0/1/2: skip[no]/warning/error message */

               HMGCHK( "mantle", ( - sdedm ), ZERO, null ); 
/*............................................................................*/
/* side temperature, mantle: */

               trfm = tjck - ( sdofm/3. + sdifm/6. )/mcm;
               ( thp->trfm ) = trfm;

            }; /* end [ fixed side heat inflow, mantle ] */
/*............................................................................*/
/* opposite side temperature, mantle: */

            topm = tjck + ( sdofm/6. + sdifm/3. )/mcm;
/*............................................................................*/
/* maximum temperature, mantle */

            if ( SMALL_VAL < fabs( sdofm - sdifm ))
               tmxm = trfm + ( sdofm*sdofm )/( 2.*mcm*( sdofm - sdifm ));
            else
               tmxm = topm;
/*............................................................................*/
/* effective heat conductance to mantle side: */

            if ( SMALL_VAL < fabs( tjck - trfm ))
               sdmcm = fabs( sdedm/( tjck - trfm ));
            else
               sdmcm = 3.*mcm/lwgm;
/*............................................................................*/
# if HCD_ADJEXT == 1
/* re-adjust tmxm to asymptotic behaviour for lwgm->oo: */

            ss0 = fabs( sdedm );
            ss1 = ss0 + fabs( elp->lssi );
            ss0 /= ss1;
            ss1 = 1. - ss0;
            ss0 = pow( ss1, 2. );
            ss1 = 1. - ss0;
            tmxm = ss0*tjck + ss1*tmxm;
# endif
/*............................................................................*/
/* return updated quantities: */

            ( thp->sdedm ) = sdedm;
	    ( thp->sdhcm ) = hc;
            ( thp->sdmcm ) = sdmcm;
            ( thp->topm ) = topm;
	    ( thp->tmxm ) = tmxm;
/*............................................................................*/
      } /* end [ cyl[3] == ONE: side conditions, mantle ] */
      else
      { 
         ( thp->sdedm ) = ZERO;
      };
/*............................................................................*/
/* lossy dielectric */

      if ( SMALL_VAL < elp->tge ) 
      {
         if ( opp->n[3] == null ) /* solid */
         {
            tmean = .5*( tdel + tinn );
            hc = ( thp->hcd ) + \
                 ( tmean - CELSIUS_TO_KELVIN )*( thp->dhd );
            qq = winn / hinn;
            mcc = hc / ( qq*tnrm );
         }
         else if ( opp->n[4] == ONE )
         {
/*............................................................................*/
/* forced convection in waveguide [ inner cooling ] */

            hc = ( thp->hcd ) + \
                 ( tdel - CELSIUS_TO_KELVIN )*( thp->dhd );

            nu = ( thp->nud ); /* Nusselt number, dielectric fluid flow */
/*............................................................................*/
# if HCD_MDDHYD == 0
/* defining the characteristic length as */

            dhyd = 2.*sqrt(( winn*hinn )/PI );

# elif HCD_MDDHYD == 1
/* yields a more secure approximation than taking the hydaulic diameter */
/* dhyd = 4*F/S [ which provides slightly lower temperatures ] */

            dhyd = 2.*( winn*hinn );
            dhyd /= ( winn+hinn );
# endif
/*............................................................................*/
            mcf = 2.*nu*hc*( winn + hinn )/dhyd;
            mcc = mcf;
         };
         nds_++;
      }
      else if ( opp->n[4] == ONE )
      {
/* inner cooling [ forced convection within waveguide ] */

         hc = ( thp->hcd ) + \
              ( tdel - CELSIUS_TO_KELVIN )*( thp->dhd );

         nu = ( thp->nud ); /* Nusselt number, dielectric fluid flow */
/*............................................................................*/
# if HCD_MDDHYD == 0
/* defining the characteristic length as */
/* [ the so-called hydraulic diameter ] */

         dhyd = 2.*sqrt(( winn * hinn )/PI );

# elif HCD_MDDHYD == 1
/* yields a more secure approximation than taking the hydaulic diameter */
/* dhyd = 4*F/S [ which provides slightly lower temperatures ] */

         dhyd = 2.*( winn * hinn );
         dhyd /= ( winn + hinn );
# endif
/*............................................................................*/
         mcf = 2.*nu*hc*( winn+hinn )/dhyd; 
      };

      if ((( winn*hinn ) < ( wout*hout ))
        &&(( thp->hcj ) < GIANT_VAL ))
      {
         tmean = .5*( tinn + tjck );
         hc = ( thp->hcj ) + \
              ( tmean - CELSIUS_TO_KELVIN )*( thp->dhj );

         mcj = ( wout + winn )/( hout - hinn );
         mcj += ( hout + hinn )/( wout - winn );
         mcj *= ( 2.* hc );
         nds_++;
      }
      else /* waveguide and mantle not separated */
         ( thp->tinn ) = ( thp->tjck );
/*............................................................................*/
/* floating wall temperature: */
      
      if ( opp->n[5] == ONE ) 
      {
         hc = ( thp->hce ) + \
              ( tenv - CELSIUS_TO_KELVIN )*( thp->dhe );

/* [ equivalent thermal ] outer waveguide diameter: */

         djck = 2.*HCD_DGAUGE*( wout + hout )/PI;

         if (( opp->n[6] ) == null ) /* free convection in environment */
         { 
            ny = ( thp->nye ) + \
                 ( tenv - CELSIUS_TO_KELVIN )*( thp->dne );
            ny /= ( thp->pre ); /* divided through pressure */

/* Grashof number for horizontal cylinder of diameter djck: */

            gre = gamma*pow( djck, 3. ) / pow( ny, 2.);
            gre *= (( tjck - tenv ) / tenv );
/*............................................................................*/
            nu = granus( gre );     /* returns exterior Nusselt number */
/*................................*//* for free convection */
         }
         else /* if (( opp->n[6] ) == ONE ) - frcd cnvct [ exterior cooling ] */
            nu = ( thp->nue ); /* Nusselt number, environmental air flow */

         mce = PI*nu*hc;
/*............................................................................*/
/* radiative contributions: */

         ss = fabs( tjck - tenv );
         if ( 1.e-277 < ss )
         {
            eme = ( thp->eme );

            rd = PI*djck;
/* same as: rd = 2.*HCD_DGAUGE*( wout + hout ) [ cf. djck, above ] */

	    rd *= ( css*eme*fabs(( pow( tjck, 4. ) - pow( tenv, 4. ))));
            mce += fabs( rd/ss );
         };
         nds_++;
      }; /* end floating mantle temperature */
   } /* end if (( opp->n[1] ) == null ) - rectangular waveguide */
/*............................................................................*/
/* COAXIAL WAVEGUIDE: */
/*............................................................................*/
   else /* if ( opp->n[1] == ONE ) */
   {
      mci = ZERO;
      mco = ZERO;
      mcr = ZERO;

      dtub = ( dmp->dtub );
      dinn = ( dmp->dinn );
      dout = ( dmp->dout );
      djck = ( dmp->djck );

      tout = ( thp->tout );
      tfld = ( thp->tfld );
      ttub = ( thp->ttub );
      
      tmxm = ( thp->tjck );
      tmxp = ( thp->tinn );
/*............................................................................*/
/* fixed side conditions, inner conductor [ pipe ] */

      if ( stp->cyl[5] == ONE )
      {
/*............................................................................*/
# if HLS_SDNODE == 1 /* count one side node */

         if (( stp->opp->n[8] == TWO )
           ||( stp->opp->n[8] == THREE )) 
            nds_++;
         else if (( stp->opp->n[9] == TWO )
                ||( stp->opp->n[9] == THREE )) 
            nds_++;
# endif
/*............................................................................*/
/* fixed side temperature, pipe: */
/*............................................................................*/
            if (( stp->opp->n[8] == TWO ) \
              ||( stp->opp->n[8] == THREE )) 
            {
               tinn = ( thp->tinn );
	       tmxp = ( thp->tmxp);
	       trfp = ( thp->tsdp );
	       sdifp = ( thp->sdifp );
/*............................................................................*/
/* heat conductance over pipe length: */
/* [ replace in the following tinn by tmxp to be on the safe side ]: */

               hc = ( thp->hcp ) + \
                  ( tinn - CELSIUS_TO_KELVIN )*( thp->dhp );
               hc *= ( PI*( dinn*dinn - dtub*dtub )/4.);

	       mcp = hc/lwgp;
/*............................................................................*/
/* effective heat current density equivalent to side heat in- and outflow */

               sdedp = 3.*( mcp*( tinn - trfp ) - sdifp/2. )/lwgp;
/*............................................................................*/
/* homogeneity check: */
/* option 0/1/2: skip[no]/warning/error message */

               HMGCHK( "pipe", ( - sdedp ), ZERO, null );
/*............................................................................*/
/* heat outflow at pipe side: */

               sdofp = lwgp*sdedp + sdifp;
               ( thp->sdofp ) = sdofp;
/*............................................................................*/
/* endothermie check: */
/* option 0/1/2: skip[no]/warning/error message */

               ENDOTH( "pipe", sdifp, sdofp, null );

            } /* end [ fixed side temperature, pipe ] */
/*............................................................................*/
/* fixed heat outflow, pipe: */
/*............................................................................*/
            else if (( stp->opp->n[9] == TWO ) \
                   ||( stp->opp->n[9] == THREE ))
            {
               tinn = ( thp->tinn );
	       tmxp = ( thp->tmxp);
	       sdifp = ( thp->sdifp );
	       sdofp = ( thp->sdofp );
/*............................................................................*/
/* endothermie check: */
/* option 0/1/2: skip[no]/warning/error message */

               ENDOTH( "pipe", sdifp, sdofp, null );
/*............................................................................*/
/* heat conductance over pipe length: */
/* [ replace in the following tinn by tmxp to be on the safe side ]: */

               hc = ( thp->hcp ) + \
                  ( tinn - CELSIUS_TO_KELVIN )*( thp->dhp );
               hc *= ( PI*( dinn*dinn - dtub*dtub )/4.);

	       mcp = hc/lwgp;
/*............................................................................*/
/* effective heat current density equivalent to side heat in- and outflow */

               sdedp = ( sdofp - sdifp )/lwgp;
/*............................................................................*/
/* homogeneity check: */
/* option 0/1/2: skip[no]/warning/error message */

               HMGCHK( "pipe", ( - sdedp ), ZERO, null ); 
/*............................................................................*/
/* update side temperature, pipe: */

               trfp = tinn - ( sdofp/3. + sdifp/6. )/mcp;
               ( thp->trfp ) = trfp;

            }; /* end [ fixed heat outflow, pipe ] */
/*............................................................................*/
/* opposite side temperature, pipe: */

            topp = tinn + ( sdofp/6. + sdifp/3. )/mcp;
/*............................................................................*/
/* maximum temperature, pipe */

            if ( SMALL_VAL < fabs( sdofp - sdifp ))
               tmxp = trfp + ( sdofp*sdofp )/( 2.*mcp*( sdofp - sdifp ));
            else
               tmxp = topp;
/*............................................................................*/
/* effective heat conductance to pipe side: */

            if ( SMALL_VAL < fabs( tinn - trfp ))
               sdmcp = fabs( sdedp/( tinn - trfp ));
            else
               sdmcp = 3.*mcp/lwgp;
/*............................................................................*/
# if HCD_ADJEXT == 1
/* re-adjust tmxp to asymptotic behaviour for lwgp->oo: */

            ss0 = fabs( sdedp );
            ss1 = ss0 + fabs( elp->lssi );
            ss0 /= ss1;
            ss1 = 1. - ss0;
            ss0 = pow( ss1, 2. );
            ss1 = 1. - ss0;
            tmxp = ss0*tinn + ss1*tmxp;
# endif
/*............................................................................*/
/* return updated quantities: */

            ( thp->sdedp ) = sdedp;
	    ( thp->sdhcp ) = hc;
            ( thp->sdmcp ) = sdmcp;
	    ( thp->tmxp ) = tmxp;
            ( thp->topp ) = topp;
      } /* end [ cyl[5] == ONE: side conditions, pipe ] */
      else
      { 
         ( thp->sdedp ) = ZERO;
      };
/*............................................................................*/
/* fixed side conditions, outer conductor [ mantle ] */

      if ( stp->cyl[6] == ONE ) 
      {
/*............................................................................*/
# if HLS_SDNODE == 1 /* count one side node */

         if (( stp->opp->n[8] == ONE )
           ||( stp->opp->n[8] == THREE )) 
            nds_++;
         else if (( stp->opp->n[9] == ONE )
                ||( stp->opp->n[9] == THREE )) 
            nds_++;
# endif
/*............................................................................*/
/* fixed side temperature, mantle: */
/*............................................................................*/
            if ((( stp->opp->n[8] ) == ONE )
              ||(( stp->opp->n[8] ) == THREE )) 
            {
               tjck = ( thp->tjck );
	       tmxm = ( thp->tmxm );
	       trfm = ( thp->tsdm );
	       sdifm = ( thp->sdifm );
/*............................................................................*/
/* heat conductance over mantle length: */
/* [ replace in the following tjck by tmxm to be on the safe side ]: */

               hc = ( thp->hcm ) + \
                  ( tjck - CELSIUS_TO_KELVIN )*( thp->dhm );
               hc *= ( PI*( djck*djck - dout*dout )/4. );
	    
	       mcm = hc/lwgm;
/*............................................................................*/
/* effective heat current density equivalent to side heat in- and outflow */

               sdedm = 3.*( mcm*( tjck - trfm ) - sdifm/2. )/lwgm;
/*............................................................................*/
/* homogeneity check: */
/* option 0/1/2: skip[no]/warning/error message */

               HMGCHK( "mantle", ( - sdedm ), ZERO, null );
/*............................................................................*/
/* heat outflow at mantle side: */

               sdofm = lwgm*sdedm + sdifm;
               ( thp->sdofm ) = sdofm;
/*............................................................................*/
/* endothermie check [ exclude tjck < trfm ]: */
/* option 0/1/2: skip[no]/warning/error message */

               ENDOTH( "mantle", sdifm, sdofm, null );

            } /* end [ fixed side temperature, mantle ] */
/*............................................................................*/
/* fixed heat outflow, mantle: */
/*............................................................................*/
            else if ((( stp->opp->n[9] ) == ONE ) \
                   ||(( stp->opp->n[9] ) == THREE ))
            {
               tjck = ( thp->tjck );
               tmxm = ( thp->tmxm );
	       sdifm = ( thp->sdifm );
	       sdofm = ( thp->sdofm );
/*............................................................................*/
/* endothermie check: */
/* option 0/1/2: skip[no]/warning/error message */

               ENDOTH( "mantle", sdifm, sdofm, null );
/*............................................................................*/
/* heat conductance over mantle length: */
/* [ replace in the following tjck by tmxm to be on the safe side ]: */

               hc = ( thp->hcm ) + \
                  ( tjck - CELSIUS_TO_KELVIN )*( thp->dhm );
               hc *= ( PI*( djck*djck - dout*dout )/4. );
	    
	       mcm = hc/lwgm;
/*............................................................................*/
/* effective heat current density equivalent to side heat in- and outflow */

               sdedm = ( sdofm - sdifm )/lwgm;
/*............................................................................*/
/* homogeneity check: */
/* option 0/1/2: skip[no]/warning/error message */

               HMGCHK( "mantle", ( - sdedm ), ZERO, null ); 
/*............................................................................*/
/* update side temperature, mantle: */

               trfm = tjck - ( sdofm/3. + sdifm/6. )/mcm;
               ( thp->trfm ) = trfm;

            }; /* end [ fixed heat outflow, mantle ] */
/*............................................................................*/
/* opposite side temperature, mantle: */

            topm = tjck + ( sdofm/6. + sdifm/3. )/mcm;
/*............................................................................*/
/* maximum temperature, mantle */

            if ( SMALL_VAL < fabs( sdofm - sdifm ))
               tmxm = trfm + ( sdofm*sdofm )/( 2.*mcm*( sdofm - sdifm ));
            else
               tmxm = topm;
/*............................................................................*/
/* effective heat conductance to mantle side: */

            if ( SMALL_VAL < fabs( tjck - trfm ))
               sdmcm = fabs( sdedm/( tjck - trfm ));
            else
               sdmcm = 3.*mcm/lwgm;
/*............................................................................*/
# if HCD_ADJEXT == 1
/* re-adjust tmxm to asymptotic behaviour for lwgm->oo: */

            ss0 = fabs( sdedm );
            ss1 = ss0 + fabs( elp->lsso );
            ss0 /= ss1;
            ss1 = 1. - ss0;
            ss0 = pow( ss1, 2. );
            ss1 = 1. - ss0;
            tmxm = ss0*tjck + ss1*tmxm;
# endif
/*............................................................................*/
/* return updated quantities: */

            ( thp->sdedm ) = sdedm;
	    ( thp->sdhcm ) = hc;
            ( thp->sdmcm ) = sdmcm;
	    ( thp->tmxm ) = tmxm;
            ( thp->topm ) = topm;
      } /* end [ cyl[6] == ONE: side conditions, mantle ] */
      else
      { 
         ( thp->sdedm ) = ZERO;
      };
/*............................................................................*/
/* additional inner conductor cooling [ forced convection ] */

      if (( opp->n[7] ) == ONE )
      {
         hc = ( thp->hcf ) + \
              ( tfld - CELSIUS_TO_KELVIN )*( thp->dhf );

         nu = ( thp->nuf ); /* Nusselt number, coolant flow */
         mcf = PI*nu*hc;

         if (( dtub < dinn )
           &&(( thp->hct ) < GIANT_VAL ))
         { 
            tmean = .5*( ttub + tinn );
            hc = ( thp->hct ) + \
                 ( tmean - CELSIUS_TO_KELVIN )*( thp->dht );

            mcc = 2.*PI*hc / log( dinn / dtub );
            nds_++;
         };
      }; /* end if (( opp->n[7] ) == ONE ) - additional inner condct cooling */
/*............................................................................*/
/* solid dielectric: */

      if (( opp->n[3] ) == null ) /* solid dielectric */
      {
         tmean = .5*( tinn + tout );
	 
         hc = ( thp->hcd ) + \
              ( tmean - CELSIUS_TO_KELVIN )*( thp->dhd );

         ss0 = log( .5*dout );
         ss1 = log( .5*dinn );

         if ( SMALL_VAL < ( elp->tge )) /* lossy dielectric */
         {
            lssd = 2.*PI*freq*cwpr*tge*sqrt( eps*epd*my*myd );

            kk0 = lssd / ( 2.*PI*(  ss0 - ss1 ));
            kk1 = ( tout - tinn )/( ss0 - ss1 );
            kk1 += ( kk0*( ss0 + ss1 )/2./hc );
/*............................................................................*/
# if ( HCD_MCDMDE == 1 )||( HCD_MCDMDE == 2 ) /* the gradient method [ not always stable ] */
/* the temperature gradients, inner and outer conductor: */

            grdo = 2.*( kk1 - kk0*ss0/hc ) / dout;
            grdi = 2.*( kk1 - kk0*ss1/hc ) / dinn;
# endif
/*............................................................................*/
# if HCD_MCDMDE == 0 /* stable approxmation which yields the same [ net ] */
	             /* temperature differences as the distributed sources */ 

            mci = 4.*PI*hc / ( ss0 - ss1 );
            mco = 4.*PI*hc / ( ss0 - ss1 );

# elif HCD_MCDMDE == 1 /* the gradient method [ not always stable ] */

            if ( SMALL_VAL < fabs( tinn - tdel ))
               mci = fabs( PI*dinn*hc*grdi / ( tinn - tdel ));
            else
               mci = GIANT_VAL;

            if ( SMALL_VAL < fabs( tdel - tout ))
               mco = fabs( PI*dout*hc*grdo / ( tdel - tout ));
            else
               mco = GIANT_VAL;
	    
# elif HCD_MCDMDE == 2

	    if ( fabs( tout - tdel ) < fabs( tinn - tdel ))
            {
               ss = fabs( PI*dinn*hc*grdi );
               mci = fabs( ss/( tinn - tdel ));

               if ( SMALL_VAL < fabs( tout - tdel ))
                  mco =  fabs(( lssd - ss )/( tout - tdel ));
               else 
                  mco =  ZERO;
            }
	    else if ( fabs( tinn - tdel ) < fabs( tout - tdel ))
            {
               ss = fabs( PI*dout*hc*grdo );
               mco = fabs( ss / ( tout - tdel ));
         
               if ( SMALL_VAL < fabs ( tinn - tdel ))
                  mci =  fabs(( lssd - ss ) / ( tinn - tdel ));
               else
                  mci =  ZERO;
            }
            else
            {
               mci = ZERO;
               mco = ZERO;
            };
/*............................................................................*/
# else /* cf. HCD_MCDMDE == 0 */
            mci = 4.*PI*hc / ( ss0 - ss1 );
            mco = 4.*PI*hc / ( ss0 - ss1 );
# endif
/*............................................................................*/
            nds_++;
         }
         else
         {
            mco = 2./( ss0 - ss1 );
/*............................................................................*/
# if ( HCD_MCDMDE == 1 )\
   ||( HCD_MCDMDE == 2 ) /* the gradient method [ not always stable ] */
            grdi = mco*( tinn - tout )/dinn;
            grdo = mco*( tinn - tout )/dout;
# endif
/*............................................................................*/
            mco *= ( PI*hc );
         };
         nds_++;
      } /* end, solid dielectric */
/*............................................................................*/
/* fluid dielectric: */
      else /* if ( opp->n[3] == ONE ) - convection within waveguide */
      {
         if (( opp->n[4] ) == null ) /* free convection */
         {
/* mean temperature: */

            tmean = .5*( tinn + tout );
	    
            hc = ( thp->hcd ) + \
                 ( tmean - CELSIUS_TO_KELVIN )*( thp->dhd );

/* cinematic viscosity: */

            ny = ( thp->nyd ) + \
                 ( tmean - CELSIUS_TO_KELVIN )*( thp->dnd );
            ny /= ( thp->prd ); /* divided through pressure */

/* Grashof number between coaxial cylinders: */

            qq = dout / dinn;

            grd = gamma*pow( dinn, 3. ) / pow( ny, 2.);
            grd *= (( tinn - tout ) / tmean );
            grd = fabs( grd ); /* [ only to ensure that ... ] */
/*............................................................................*/
            zeta = grazet( grd, qq );     /* essentially, returns the Nusselt */
/*......................................*//* number for free convection       */
                                          /* between coaxial cylinders        */
            mco = 2.*PI*hc*zeta/log( qq );
         }
         else /* if ( opp->n[4] ) == ONE ) - forced convection */
         {
            tmean = tdel;
	    
            hc = ( thp->hcd ) + \
                 ( tmean - CELSIUS_TO_KELVIN )*( thp->dhd );

/* characteristic length [ the hydraulic diameter 4*F/S ]: */

            dhyd = dout - dinn;

 /* Nusselt number, dielectric fluid flow: */

            nu = ( thp->nud );

            mci = PI*nu*hc*dinn/dhyd;
            mco = PI*nu*hc*dout/dhyd;
         };
/*............................................................................*/
/* radiative contributions: */

         ss = fabs( tinn - tout );
         if ( 1.e-277 < ss )
         {
            emo = ( thp->emo );
            emi = ( thp->emi );

            qq = dout / dinn;

            rd = css*PI*dinn*fabs(( pow( tinn, 4. ) - pow( tout, 4. )));
            rd /= (( 1./ emi ) + (( 1./ emo ) - 1.) / qq ); 
                                                
            mcr = fabs( rd/ss );
         }
         else
            mcr = ZERO;

         nds_++;
      }; /* end if (( opp->n[3] ) == ONE ) - convection between waveguides */
/*............................................................................*/
/* heat conductivity, outer conductor to [ exterior ] mantle: */

      if (( dout < djck )
        &&(( thp->hcj ) < GIANT_VAL ))
      {
         tmean = .5*( tout + tjck );
         hc = ( thp->hcj ) + \
              ( tjck - CELSIUS_TO_KELVIN )*( thp->dhj );

         mcj = 2.*PI*hc/log( djck/dout );
         nds_++;
      }
      else /* waveguide and mantle not separated */
         ( thp->tout ) = ( thp->tjck );
/*............................................................................*/
/* floating wall temperature: */

      if (( opp->n[5] ) == ONE ) 
      {
         hc = ( thp->hce ) + \
              ( tenv - CELSIUS_TO_KELVIN )*( thp->dhe );

         if (( opp->n[6] ) == null ) /* free convection in environment */
         { 
            ny = ( thp->nye ) + \
                 ( tenv - CELSIUS_TO_KELVIN )*( thp->dne );

            ny /= ( thp->pre ); /* divided through pressure */

/* Grashof number for horizontal cylinder: */

            gre = gamma*pow( djck, 3. ) / pow( ny, 2.);
            gre *= (( tjck - tenv ) / tenv );
/*............................................................................*/
            nu = granus( gre );     /* returns exterior Nusselt number */
/*................................*//* for free convection */
         }
         else if (( opp->n[6] ) == ONE ) /* forced convct [exterior cooling] */
            nu = ( thp->nue ); /* Nusselt number, environmental air flow */

         mce = PI*nu*hc;
/*............................................................................*/
/* radiative contributions: */

         ss = fabs( tjck - tenv );

         if ( 1.e-277 < ss )
         {
            eme = ( thp->eme );
            rd = PI*djck*css*eme*fabs(( pow( tjck, 4. ) - pow( tenv, 4. )));
            mce += fabs( rd/ss );
         };
         nds_++;
      }; /* end if (( opp->n[5] ) == ONE ) - floating mantle temperature */
   }; /* end if (( opp->n[1] ) == ONE ): coaxial waveguide */
/*...........................................................................*/
   nodes = nds_;
   rank = 2*nodes;
/*...........................................................................*/
/* reset mutual heat conductance matrix: */

   ii = null; do
   {
      jj = null; do
      {
         ( cdp->mhc[ii][jj] ) = ZERO;
      } while (( ++jj ) < rank );
   } while (( ++ii ) < rank );

   ( cdp->ndm ) = -ONE;
   ( cdp->ndp ) = -ONE;

   nds_ = null;
/*...........................................................................*/
/* RECTANGULAR WAVEGUIDE [ re-build mhc-matrix ] */
/*............................................................................*/
   if (( opp->n[1] ) == null ) /* rectangular waveguide */
   {
      if ( opp->n[4] == ONE ) /* inner waveguide cooling [forced convection] */
         ( cdp->mhc[nds_][nds_] ) = rscl*mcf;

      if ( state->cyl[0] == ONE ) /* lossy dielectric */
      {
         ( cdp->mhc[nds_][nds_+1] ) = rscl*mcc;
         ( cdp->mhc[nds_+1][nds_] ) = rscl*mcc;
         nds_++;
      };

      if ( state->cyl[1] == ONE ) /* mantle of finite wall thickness */
      {
         if ( opp->n[5] == null ) /* fixed mantle temperature */
            ( cdp->mhc[nds_][nds_] ) = rscl*mcj;
         else /* if( opp->n[5] == ONE ) - convection */
         {
            ( cdp->mhc[nds_][nds_+1] ) = rscl*mcj;
            ( cdp->mhc[nds_+1][nds_] ) = rscl*mcj;
         };
         nds_++;
      };

      if ( stp->cyl[2] == ONE ) /* floating mantle temperature */
      {
         ( cdp->mhc[nds_][nds_] ) = rscl*mce;
         nds_++;
      }; /* [ end, floating mantle temperature ] */
/*...........................................................................*/
# if HLS_SDNODE == 1 /* use side nodes */

      if ( stp->cyl[3] == ONE )
      {
         cdp->ndm = cdp->nde[3];

         if ( sdmcm < SMALL_VAL ) 
            sdmcm = 1.00000e-09;

         if (( opp->n[8] == ONE )
           ||( opp->n[8] == THREE ))
         {
            ( cdp->mhc[cdp->ndm][nds_] ) = rscl*sdmcm*2.;
            ( cdp->mhc[nds_][cdp->ndm] ) = rscl*sdmcm*2.;
            ( cdp->mhc[nds_][nds_] ) = rscl*sdmcm*2.;
            ( cdp->nde[4] ) = nds_;
	     nds_++;
         }
	 else if (( opp->n[9] == ONE )
	        ||( opp->n[9] == THREE ))
         {
            ( cdp->mhc[cdp->ndm][nds_] ) = rscl*sdmcm;
            ( cdp->mhc[nds_][cdp->ndm] ) = rscl*sdmcm;
            ( cdp->nde[5] ) = nds_;
	    nds_++;
         };
      }; /* end [ cyl[3]  == ONE, side conditions, waveguide mantle ] */
# endif /* HLS_SDNODE == 1 */
/*...........................................................................*/
   } /* [ end, rectangular waveguide ] */
/*............................................................................*/
/* COAXIAL WAVEGUIDE [ re-build mhc-matrix ] */
/*............................................................................*/
   else /* if ( opp->n[1] == ONE ) - coaxial waveguide */
   {
      if ( state->cyl[0] == ONE )
      {
         ( cdp->mhc[nds_][nds_+1] ) = rscl*mcc;
         ( cdp->mhc[nds_+1][nds_] ) = rscl*mcc;
         nds_++; /* oder nachgestellt ???????? */
         ( cdp->mhc[nds_][nds_] ) = rscl*mcf;
      };

      if ( state->cyl[2] == ONE ) /* lossy dielectric */
      {
         ( cdp->mhc[nds_][nds_+1] ) = rscl*mci;
         ( cdp->mhc[nds_+1][nds_] ) = rscl*mci;
         nds_++;
      }
      else if ( opp->n[3] == ONE ) /* non-lossy gas or fluid dielectric */
      {
         if (( opp->n[4] ) == ONE ) /* forced convection within waveguide */
            ( cdp->mhc[nds_][nds_] ) = rscl*mci;

         ( cdp->mhc[nds_][nds_+1] ) = rscl*mcr;
         ( cdp->mhc[nds_+1][nds_] ) = rscl*mcr;
      };

      if ( state->cyl[3] == ONE ) /* mantle with finite wall thickness */
      {
         if (( opp->n[3] == null )  /* solid dielectric */
           ||( opp->n[4] == null )) /* or free convection between cndctrs */
         {
            ( cdp->mhc[nds_][nds_+1] ) += ( rscl*mco );
            ( cdp->mhc[nds_+1][nds_] ) += ( rscl*mco );
         };
         nds_++;
      
         if (( opp->n[3] == ONE  ) /* non-lossy gas or fluid dielectric */
           &&( opp->n[4] == ONE )) /* forced convection within waveguide */
         {
            ( cdp->mhc[nds_][nds_] ) = rscl*mco;
         };
      }
      else if (( opp->n[3] == null )  /* solid dielectric */
             ||( opp->n[4] == null )) /* or free convection btw. cndctrs. */
      {
          mcj = mco;
      }; 

      if ( state->cyl[4] == ONE ) /* floating mantle temperature */
      { 
         ( cdp->mhc[nds_][nds_+1] ) += ( rscl*mcj );
         ( cdp->mhc[nds_+1][nds_] ) += ( rscl*mcj );
         nds_++;
         ( cdp->mhc[nds_][nds_] ) = rscl*mce;
      }
      else /* fixed mantle temperature */
      {
         ( cdp->mhc[nds_][nds_] ) = rscl*mcj;

         if ( stp->cyl[3] == ONE ) /* [ thermally ] finite mtl.wall thickness */
            ( cdp->mhc[nds_][nds_] ) = rscl*mcj;
      };
      nds_++;
/*............................................................................*/
# if HLS_SDNODE == 1 /* use side nodes */
/* inner conductor [ pipe ] in thermal contact to one side: */

      if ( stp->cyl[5] == ONE ) /* side conditions, pipe */
      {
         cdp->ndp = cdp->nde[2];

         if ( sdmcp < SMALL_VAL ) 
            sdmcp = 1.00000e-09;

         if (( opp->n[8] == TWO )
           ||( opp->n[8] == THREE ))
         {
            ( cdp->mhc[cdp->ndp][nds_] ) = rscl*sdmcp*2.;
            ( cdp->mhc[nds_][cdp->ndp] ) = rscl*sdmcp*2.;
            ( cdp->mhc[nds_][nds_] ) = rscl*sdmcp*2.;
            ( cdp->nde[6] ) = nds_; 
            nds_++; 
         }
         else if (( opp->n[9] == TWO )
	        ||( opp->n[9] == THREE ))
         {
            ( cdp->mhc[cdp->ndp][nds_] ) = rscl*sdmcp;
            ( cdp->mhc[nds_][cdp->ndp] ) = rscl*sdmcp;
            ( cdp->nde[7] ) = nds_; 
            nds_++;
         };
      }; /* end [ side conditions, pipe ] */

      if ( stp->cyl[6] == ONE ) /* side conditions, mantle */
      {
         cdp->ndm = cdp->nde[5];

         if ( sdmcm < SMALL_VAL ) 
            sdmcm = 1.00000e-09;

         if (( opp->n[8] == ONE )
           ||( opp->n[8] == THREE ))
         {
            ( cdp->mhc[cdp->ndm][nds_] ) = rscl*sdmcm*2.;
            ( cdp->mhc[nds_][cdp->ndm] ) = rscl*sdmcm*2.;
            ( cdp->mhc[nds_][nds_] ) = rscl*sdmcm*2.;
            ( cdp->nde[8] ) = nds_; 
            nds_++;
         }
         else if (( opp->n[9] == ONE )
	        ||( opp->n[9] == THREE ))
         {
            ( cdp->mhc[cdp->ndm][nds_] ) = rscl*sdmcm;
            ( cdp->mhc[nds_][cdp->ndm] ) = rscl*sdmcm;
            ( cdp->nde[9] ) = nds_; 
            nds_++;
         };
      }; /* end [ side conditions, mantle ] */
# endif /* HLS_SDNODE == 1 */
/*............................................................................*/
   }; /* end if ( opp->n[1] == ONE ) - coaxial waveguide */
/*...........................................................................*/
# if HCD_DEBUG == 1

   if (( state->hcd_skp[1] != 's' )&&( state->hcd_skp[1] != 'S' ))
   {
      fprintf ( stdout, "\n Message from job %s, function %s :",
         lotos (( state->job ), null ), __func__ );
      fprintf ( stdout, "\n The number of nodes counted is %d .", nds_+1 );
      fprintf( stdout, "\n\n Please acknowledge:" );
      fprintf( stdout, "\n Enter any character"
         " [ enter 's' to skip this message ]: " );

      scanf( "%s", &state->hcd_skp[1] );
   };
# endif 
/*............................................................................*/
   if ( nodes != ( nds_ ))
   {
      errfle = fopen(( state->errfle ), "a+" );
      
      fprintf( errfle, \
         "\nError message from job %s, function %s :", \
         lotos (( state->job ), null ), __func__ ); \
      fprintf( errfle,
         "\nInconsistent node number !" );
      fprintf( errfle,
         "\n[ This is probably program failure." );
      fprintf( errfle,
         "\n- Please send bug report to <contact@sfenx.de>" );
      fprintf( errfle,
         "\nannexing your parameter log file par.log%s.]\n",
         ( lotos (( state->job ), null )));
      fclose( errfle );
/*...........................................................................*/
# if HCD_DEBUG == 1

      if (( state->hcd_skp[2] != 's' )&&( state->hcd_skp[1] != 'S' ))
      {
         fprintf ( stdout, "\n Error message from job %s, function %s :", \
            lotos (( state->job ), null ), __func__ ); 
         fprintf ( stdout, "\n The number %d of counted nodes differs from\n"
            " the number %d of formerly defined nodes !", nds_+1, nodes+1 ); 
         fprintf( stdout, "\n\n Please acknowledge:" );
         fprintf( stdout, "\n Enter any character"
            " [ enter 's' to skip this message ]: " );

         scanf( "%s", &state->hcd_skp[2] );
      };
/*............................................................................*/
# else

      fprintf( stderr, \
         "\n Error message from job %s, function %s :", \
         lotos (( state->job ), null ), __func__ ); \
      fprintf( stderr,
         "\n Inconsistent node number !" );
      fprintf( stderr,
         "\n [ This is probably program failure." );
      fprintf( stderr,
         "\n - Please send bug report to <contact@sfenx.de>" );
      fprintf( stderr,
         "\n annexing your parameter log file par.log%s.]\n",
         ( lotos (( state->job ), null )));

      exit( EXIT_FAILURE );

# endif
/*............................................................................*/
   }
   else
      ( cdp->nodes ) = nodes;
/*............................................................................*/
# if HCD_MHCLOG == 1

   fprintf( stdout, "\nMHCLOG\n[program helios(*)]\n" );
   fprintf( stdout, "\nmutual heat conductances [W/(m*K)]\n" );

   ii = null; do
   {
      fprintf( stdout, "\n" );
      jj = null; do
      {
         fprintf( stdout, "% .5e ",
            ( cdp->mhc[ii][jj] ));
      } while(( ++jj ) < nodes );
   } while(( ++ii ) < nodes );
   fprintf( stdout, "\n" );

   fprintf( stdout, "\nplease acknowledge [enter any character]: " );
   scanf( "%s", ptr );

# elif HCD_MHCLOG == 2

   strcpy( fleptr, "hcd.log" );
   strcat( fleptr, lotos(( stp->job ), null ));

   results = fopen( fleptr, "w+" );

   fprintf( results, "MHCLOG\n[program codis(*)]\n" );
   fprintf( results, "\nmutual heat conductances [W/(m*K)]\n" );

   ii = null; do
   {
      fprintf( results, "\n" );
      jj = null; do
      {
         fprintf( results, "% .5e ",
            ( cdp->mhc[ii][jj] ));
      } while(( ++jj ) < nodes );
   } while(( ++ii ) < nodes );
   fprintf( results, "\n" );

   fclose( results );

   fprintf( stdout, "\nMHCLOG written into file %s", fleptr );
   fprintf( stdout, "\nplease acknowledge [enter any character]: " );
   scanf( "%s", ptr );

# endif
/*...........................................................................*/
# if HCD_DEBUG == 2

   if (( opp->n[1] ) == null ) /* rectangular waveguide */
   { 
       if (( state->hcd_skp[3] != 's' )
         &&( state->hcd_skp[3] != 'S' ))
       {
          fprintf ( stdout, "\n Message from job %s, function %s :",
             lotos (( state->job ), null ), __func__ );
          fprintf ( stdout, "\n The conductor [ mantle ] temperature is "
             "%.7e C.", ( thp->tjck ) - CELSIUS_TO_KELVIN );
          fprintf ( stdout, "\n The side temperature is "
             "%.7e C.", ( thp->trfm ) - CELSIUS_TO_KELVIN );
          fprintf( stdout, "\n\n Please acknowledge:" );
          fprintf( stdout, "\n Enter any character"
             " [ enter 's' to skip this message ]: " );

          scanf( "%s", &state->hcd_skp[3] );
       };
   }
   else if ( opp->n[1] == ONE ) /* coaxial waveguide */
   {
       if (( state->hcd_skp[3] != 's' )
         &&( state->hcd_skp[3] != 'S' ))
       {
          fprintf ( stdout, "\n Message from job %s, function %s :",
             lotos (( state->job ), null ), __func__ );
          fprintf ( stdout, "\n The inner conductor [ pipe ] temperature is "
             "%.7e C.", ( thp->tinn ) - CELSIUS_TO_KELVIN );
          fprintf ( stdout, "\n The side temperature is "
             "%.7e C.", ( thp->trfp ) - CELSIUS_TO_KELVIN );
          fprintf( stdout, "\n\n Please acknowledge:" );
          fprintf( stdout, "\n Enter any character"
             " [ enter 's' to skip this message ]: " );

          scanf( "%s", &state->hcd_skp[3] );
       };

       if (( state->hcd_skp[4] != 's' )
         &&( state->hcd_skp[4] != 'S' ))
       {
          fprintf ( stdout, "\n Message from job %s, function %s :",
             lotos (( state->job ), null ), __func__ );
          fprintf ( stdout, "\n The conductor [ mantle ] temperature is "
             "%.7e C.", ( thp->tjck ) - CELSIUS_TO_KELVIN );
          fprintf ( stdout, "\n The side temperature is "
             "%.7e C.", ( thp->trfm ) - CELSIUS_TO_KELVIN );
          fprintf( stdout, "\n\n Please acknowledge:" );
          fprintf( stdout, "\n Enter any character"
             " [ enter 's' to skip this message ]: " );

          scanf( "%s", &state->hcd_skp[4] );
       };
    }; /* end [ coaxial waveguide ] */
# endif 
/*............................................................................*/
  
   ( cdp->rtn ) = null;
   return cdp;
}  
/*============================================================================*/
/*********************** end of function hcndct(*)' **************************/
/*
   Es ist gleichermassen toedlich fuer den Geist, ein System zu haben - und
   keines zu haben. Daher muss man sich dazu entschliessen, beides miteinander
   zu verbinden.
                                                            Friedrich Schlegel
*/
