/* [ file: rvise_mat.h ] */
/*******************************************************************************
*                                                                              *
*   Function rvise_mat(*); HELIOS release v1.0r1                               *
*   [ Heat and Electric Losses In Ordinary wave guiding Structures ]           *
*                                                                              *
*   This function revises [ and corrects if necessary ] the material           *
*   parameters [ input option "materials"], thereby respecting the following   *
*   hierarchy [ from highest to lowest priority ]:                             *
*                                                                              *
*                   OPERATIONS -> MATERIALS -> PARAMETERS                      *
*                                                                              *
*   Thus, a former input in this chain always dominates any subsequent input   *
*   [ which, of course, only affects conflicting cases ].                      *
*   Also, this function settles any internal parameter dependencies for input  *
*   option "materials" [ in cases where such dependencies exist ].             *
*                                                                              *
*   (C) SHEIN; Munich, December 2021                       Steffen Hein        *
*   [ Update: December 17, 2021 ]                       <contact@sfenx.de>     *
*                                                                              *
*******************************************************************************/
# ifndef LINLEN
   # define LINLEN 58
# endif
/*============================================================================*/

short rvise_matter( void )
{ 
/*
   extern HELIOSSTATE *spt;
*/
   HELIOSSTATE *state = &helios;
/*
   static char
      ptr[STS_SIZE] = {null};
*/
   static const char
     *sstrng = "**********************";

   static short
      ll = LINLEN;

   static const char
     *shhnd = "short_hand",
     *inact = "inactive";

   METALS
     *metals ( METALS *mtp );

   DIELCT
     *dielct ( DIELCT *dlp );

   SOLIDS
     *solids ( SOLIDS *slp );

   FLUIDS
     *fluids( FLUIDS *flp );

   void
      cpylne( char lne[], const char *txt, const char *brc, short linlen );
/*----------------------------------------------------------------------------*/
/* material strings: */
   
   ( state->opp ) = &opr;
   ( state->map ) = &mat;
   ( state->mtp ) = &met;

/* default materials: */

   cpylne( mat.mti, "inner_conductor_metal", shhnd, ll );
   cpylne( mat.mto, "outer_conductor_metal", shhnd, ll );
   cpylne( mat.del, "dielectric", shhnd, ll );
   cpylne( mat.jck, "mantle", shhnd, ll );

 # ifdef _Forced
   cpylne( mat.cpe, "pipe", shhnd, ll );
   cpylne( mat.fld, "cooling_fluid", shhnd, ll );
 # else
   cpylne( mat.cpe, "pipe", inact, ll );
   cpylne( mat.fld, "cooling_fluid", inact, ll );
   strcpy( mat.shc, "VOID" );
   strcpy( mat.shf, "VOID" );
# endif
/* 
   The pertinent short hands are user defined
   [ like that, e.g.]:

   strcpy( mat.shi, "Cu" );
   strcpy( mat.sho, "Al" );
   strcpy( mat.shd, "PTFE" );
   strcpy( mat.shj, "VOID" );
   strcpy( mat.shc, "VOID" );
   strcpy( mat.shf, "H2O" );
*/  
   mat.m = 7; /* number of available [ pre-specified ] metals */
   mat.d = 7; /* number of available [ pre-specified ] specified dielectrics */
   mat.s = 4; /* number of available [ pre-specified ] mantle materials */
   mat.f = 2; /* number of available [ pre-specified ] cooling fluids */
/*............................................................................*/
/* list of available METALS: */

   strcpy( mat.avm, "Some_available_metals:" );

   strcpy( mat.mtx[0] , "METAL" );
   strcpy( mat.msx[0] , "SHORT_HAND" );
/*............................................................................*/
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

   strcpy( mat.dtx[0] , "DIELECTRIC" );
   strcpy( mat.dsx[0] , "SHORT_HAND" );
/*............................................................................*/
   strcpy( mat.dtx[1] , "AL2O3_[99.5%]" );   strcpy( mat.dsx[1] , "AL99" );
   strcpy( mat.dtx[2] , "AL2O3_[95.0%]" );   strcpy( mat.dsx[2] , "AL95" );
   strcpy( mat.dtx[3] , "teflon" );          strcpy( mat.dsx[3] , "PTFE" );
   strcpy( mat.dtx[4] , "polyethylene" );    strcpy( mat.dsx[4] , "PE" );
   strcpy( mat.dtx[5] , "polystyrene" );     strcpy( mat.dsx[5] , "PS" );
   strcpy( mat.dtx[6] , "polyuretane" );     strcpy( mat.dsx[6] , "PU" );
   strcpy( mat.dtx[7] , "quartz" );          strcpy( mat.dsx[7] , "Qu" );
/*............................................................................*/
/* list of available JACKET MATERIALS: */

   strcpy( mat.avs,
      "Some_[mantle_and_pipe]_solid_materials:" );

   strcpy( mat.stx[0] , "MATERIAL" );
   strcpy( mat.ssx[0] , "SHORT_HAND" );
/*............................................................................*/
   strcpy( mat.stx[1] , "teflon" );          strcpy( mat.ssx[1] , "PTFE" );
   strcpy( mat.stx[2] , "polyethylene" );    strcpy( mat.ssx[2] , "PE" );
   strcpy( mat.stx[3] , "polystyrene" );     strcpy( mat.ssx[3] , "PS" );
   strcpy( mat.stx[4] , "polyuretane" );     strcpy( mat.ssx[4] , "PU" );
/*............................................................................*/
/* list of available COOLING FLUIDS: */

   strcpy( mat.avf, "Some_available_cooling_fluids:" );

   strcpy( mat.ftx[0] , "COOLING_FLUID" ); strcpy( mat.fsx[0] , "SHORT_HAND" );
/*............................................................................*/
   strcpy( mat.ftx[1] , "air" );     strcpy( mat.fsx[1] , "AIR" );
   strcpy( mat.ftx[2] , "water" );   strcpy( mat.fsx[2] , "H2O" );
/*............................................................................*/
/* revise the input: */

   if (( state->opp->n[1] ) == null ) /* rectangular waveguide */
   {
      cpylne( mat.mti, "waveguide_metal", shhnd, ll );
      cpylne( mat.mto, "outer_conductor_metal", inact, ll );
      cpylne( mat.del, "dielectric", shhnd, ll );
      cpylne( mat.jck, "mantle", shhnd, ll );
      cpylne( mat.cpe, "pipe", inact, ll );
      cpylne( mat.fld, "cooling_fluid", inact, ll );
   }
   else /* if ( state->opp->n[1] == ONE ): coaxial waveguide */
   {
      cpylne( mat.mti, "inner_conductor_metal", shhnd, ll );
      cpylne( mat.mto, "outer_conductor_metal", shhnd, ll );
      cpylne( mat.del, "dielectric", shhnd, ll );
      cpylne( mat.jck, "mantle", shhnd, ll );
      cpylne( mat.cpe, "pipe", shhnd, ll );
      cpylne( mat.fld, "cooling_fluid", shhnd, ll );
   };
/*............................................................................*/
/* read inner conductor [metal] parameters: */

   if (( null != strcmp( mat.shi, "else" ))
     &&( null != strcmp( mat.shi, "Else" ))
     &&( null != strcmp( mat.shi, "ELSE" )))
   {
      strcpy(( state->mtp->shm ), mat.shi );
      strcpy(( state->mtp->met ), sstrng );
/*............................................................................*/
      ( state->mtp ) = metals ( state->mtp );     /*                          */
/*..............................................*/
      strcpy( mat.shi, ( state->mtp->shm ));
   }
   else
   {
      strcpy( mat.shi, "ELSE" );
   };
/*............................................................................*/
/* read dielectric parameters: */

   if (( null != strcmp( mat.shd, "else" ))
     &&( null != strcmp( mat.shd, "Else" ))
     &&( null != strcmp( mat.shd, "ELSE" )))
   {
      strcpy(( state->dlp->shd ), mat.shd );
      strcpy(( state->dlp->del ), sstrng ); 
/*............................................................................*/
      ( state->dlp ) = dielct ( state->dlp );     /*                          */
/*..............................................*/
      
      if ((( state->opp->n[3] ) == ONE )       /* fluid dielectric */
        &&((( state->dlp->ny ) < 1.e-277 )     /* unspecfd cinematic viscsty */
         ||( 1.e+277 < ( state->dlp->ny ))))   /* infinite cinematic viscsty */
      {
         strcpy(( state->dlp->shd ), "AIR" );  /* replace solid dielectric */
         strcpy(( state->dlp->del ), sstrng ); /* with any fluid */
/*............................................................................*/
         ( state->dlp ) = dielct ( state->dlp );     /*                       */
/*.................................................*/
      }
      else if ((( state->opp->n[3] ) == null ) /* solid dielectric */
        &&( 1.e-277 <= ( state->dlp->ny ))     /* non-zero cinematic viscsty */
        &&(( state->dlp->ny ) <= 1.e+277 ))    /* finite cinematic viscsty */
      {
         strcpy(( state->dlp->shd ), "ELSE" ); /* replace fluid dielectric */
         strcpy(( state->dlp->del ), sstrng ); /* with any solid */
/*............................................................................*/
         ( state->dlp ) = dielct ( state->dlp );     /*                       */
/*.................................................*/
      };

      strcpy( mat.shd, ( state->dlp->shd ));
   }
   else
   {
      strcpy( mat.shd, "ELSE" );
   };
/*............................................................................*/
/* read mantle parameters: */

   if (( null != strcmp( mat.shj, "else" ))
     &&( null != strcmp( mat.shj, "Else" ))
     &&( null != strcmp( mat.shj, "ELSE" ))
     &&( null != strcmp( mat.shj, "void" ))
     &&( null != strcmp( mat.shj, "Void" ))
     &&( null != strcmp( mat.shj, "VOID" )))
   {
      strcpy(( state->slp->shs ), mat.shj );
      strcpy(( state->slp->sld ), sstrng ); 
/*............................................................................*/
      ( state->slp ) = solids ( state->slp );     /*                          */
/*..............................................*/
      strcpy( mat.shj, ( state->slp->shs ));
   }
   else if (( null == strcmp( mat.shj, "else" ))
          ||( null == strcmp( mat.shj, "Else" ))
          ||( null == strcmp( mat.shj, "ELSE" )))
   {
      strcpy( mat.shj, "ELSE" );
   }
   else
      strcpy( mat.shj, "VOID" );
/*............................................................................*/
/* read pipe parameters: */
/*............................................................................*/
/* absent additional inner conductor cooling */
/* out-commented 15.07.2010 [ req. for fixed inner condctr side temperature ]*/
/*
   if (( state->opp->n[7] ) == null ) 
   {
      strcpy( mat.shc, "VOID" );
      strcpy( mat.shf, "VOID" );
   };
 */
/*............................................................................*/
   if (( null != strcmp( mat.sho, "else" ))
     &&( null != strcmp( mat.sho, "Else" ))
     &&( null != strcmp( mat.sho, "ELSE" )))
   {
      strcpy(( state->mtp->shm ), mat.sho );
      strcpy(( state->mtp->met ), sstrng );
/*............................................................................*/
      ( state->mtp ) = metals ( state->mtp );     /*                          */
/*..............................................*/
      strcpy( mat.sho, ( state->mtp->shm ));
   }
   else
   {
      strcpy( mat.sho, "ELSE" );
   };
/*............................................................................*/
/* pipe material parameters: */

   if (( null != strcmp( mat.shc, "else" ))
     &&( null != strcmp( mat.shc, "Else" ))
     &&( null != strcmp( mat.shc, "ELSE" ))
     &&( null != strcmp( mat.shc, "void" ))
     &&( null != strcmp( mat.shc, "Void" ))
     &&( null != strcmp( mat.shc, "VOID" )))
   {
      strcpy(( state->slp->shs ), mat.shc );
      strcpy(( state->slp->sld ), sstrng ); 
/*............................................................................*/
      ( state->slp ) = solids ( state->slp );     /*                          */
/*..............................................*/
      strcpy( mat.shc, ( state->slp->shs ));
   }
   else if (( null == strcmp( mat.shc, "else" ))
          ||( null == strcmp( mat.shc, "Else" ))
          ||( null == strcmp( mat.shc, "ELSE" )))
   {
      strcpy( mat.shc, "ELSE" );
   }
   else
      strcpy( mat.shc, "VOID" );
/*............................................................................*/
/* read fluid parameters: */

   if (( null != strcmp( mat.shf, "else" ))
     &&( null != strcmp( mat.shf, "Else" ))
     &&( null != strcmp( mat.shf, "ELSE" ))
     &&( null != strcmp( mat.shf, "void" ))
     &&( null != strcmp( mat.shf, "Void" ))
     &&( null != strcmp( mat.shf, "VOID" )))
   {
      strcpy(( state->flp->shf ), ( mat.shf )); 
      strcpy(( state->flp->fld ), sstrng ); 
/*............................................................................*/
      ( state->flp ) = fluids ( state->flp );     /*                          */
/*..............................................*/
      strcpy( mat.shf, ( state->flp->shf ));
   }
   else if (( null == strcmp( mat.shf, "else" ))
          ||( null == strcmp( mat.shf, "Else" ))
          ||( null == strcmp( mat.shf, "ELSE" )))
   {
      strcpy(( mat.shf ), "ELSE" );
   }
   else
   {
      strcpy(( mat.shf ), "VOID" );
   };
/*............................................................................*/

   return null;
}
/*============================================================================*/
# undef LINLEN
/********************** end of function 'rvise_mat(*)' ************************/
