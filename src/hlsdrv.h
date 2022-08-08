/* [ file: hlsdrv.h ] */
/*******************************************************************************
*                                                                              *
*   Body of HELIOS driver function heliosdrv(*); release v1.0r1                *
*   [ Heat and Electric Losses In Ordinary wave guiding Structures ]           *
*                                                                              *
*   This function controls the parameter input and some corrections,           *
*   the initialization function initlze(*) and the final computational         *
*   routines of hlswrk(*).                                                     *
*                                                                              *
*   (C) SHEIN; Munich, December 2021                      Steffen Hein         *
*   [ Update: June 25, 2022 ]                           <contact@sfenx.de>     *
*                                                                              *
*******************************************************************************/

/*============================================================================*/
short heliosdrv( int argn, char **args )
{
/* allusions: */

/* extern HELIOSSTATE helios; *//* [heliosdrv] menu state transfer structure */

/* declarations: */

   static FILE
     *logfle = NULL;

# if USE_NCURSES == 1
   static FILE
     *errfle;
# endif

   static HELIOSSTATE
     *state = &helios;

   static TXCNSL
     *csp = &cns; /* cf. txcnsl(*) [ text console ] */

   static char 
      fleptr[STS_SIZE] = {null},
      lnestr[STS_SIZE] = {null},
      ptr[STS_SIZE] = {null},           
    **endp = null;

   static const char 
     *logptr = "hls.log",
     *errptr = "hls.err",
     *dline = "========================================"\
              "========================================";

   static short 
      ii = null,
      job = null;

   static long 
      kk = null;
/*............................................................................*/
/* time_t types: */

   time_t nseconds = null;
   time_t   *timer = null;
   time_t time( time_t *timer );

   static char
      timeptr[STS_SIZE] = {null};

/* prototypes: */

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

   TXCNSL 
     *txcnsl( TXCNSL *csp );

   HELIOSSTATE
     *initlze( HELIOSSTATE *state );

   HELIOSSTATE
     *hlswrk( HELIOSSTATE *state );

   HELIOSSTATE
     *heliostest( HELIOSSTATE *state );

   short
      input ( char *option );

   void
      init_operts( void ),
      init_matter( void ),
      init_params( void );

   void
      deflt_operts( void ),
      deflt_matter( void ),
      deflt_params( void );

   short 
      rvise_operts( void ),
      rvise_matter( void ),
      rvise_params( void );

   short 
      rread_operts( char *filename, char mode ),
      rread_matter( char *filename, char mode ),
      rread_params( char *filename, char mode );

   short 
      store_operts( char *filename, char mode ),
      store_matter( char *filename, char mode ),
      store_params( char *filename, char mode );
/*----------------------------------------------------------------------------*/
# if USE_NCURSES == 1
/* get the terminal info: */

   term = ( char *) getenv( "TERM" ); /* get the terminal */

   kk = tgetent( null, term );

   if( kk != ONE )
   {
      errfle = fopen( errptr, "a+" );

      fprintf( errfle, \
         "\nError message from job %s, function %s :", \
         lotos ( state->job, null ), __func__ ); \
      fprintf( errfle, "\nCan't get the termcap info\n" );

      fclose ( errfle );

      fprintf( stderr, \
         "\n Error message from job %s, function %s :", \
         lotos ( state->job, null ), __func__ ); \
      fprintf( stderr, "\n Can't get the termcap info\n" );

      exit( EXIT_FAILURE );
   };
# endif
/*............................................................................*/
/* set buffer length = null: */

   kk = setvbuf( stdin, null, _IONBF, null );
   kk = setvbuf( stdout, null, _IONBF, null ); 
/*............................................................................*/
/* initialize file pointers */

   ii = null; do
   {
      tmpfle[ii] = null;
      fleptr[ii] = null;
      ++ii;
   } while( ii < STS_SIZE );
/*...........................................................................*/
/* bind structure pointers */

   state->opp = &opr;
   state->map = &mat;
   state->prp = &par;
   state->mtp = &met;
   state->dlp = &del;
   state->slp = &sld;
   state->flp = &fld;
   state->dmp = &dms;
   state->elp = &elt;
   state->thp = &trm;
   state->hcp = &hcr;
   state->cdp = &cdc;
   state->rsp = &res;
   state->csp = &cns;

   ii = null; do
   {
      state->logfle[ii] = null;
      state->errfle[ii] = null;
      ++ii;
   } while( ii < STS_SIZE );

   ( state->fleps ) = null;
   
   strcpy( state->name, "HELIOS" );
   strcpy( state->text, "waveguide_temperatures_vs_admissible_power" );
   strcpy( state->logfle, logptr );
   strcpy( state->errfle, errptr );
/*............................................................................*/
/* read command line: */

   job = null;
   if ( argn )
   {
      do
      {
         strcpy( ptr, *( args++ ));

         if ( null == strncmp( ptr, "--file", 6 )) /* paramater file */
            ( state->uif ) = 'f';                  /* input mode */

         if ( null == strncmp( ptr, "--init-job", 9 ))
         {
            if ( --argn )
            {
               strcpy ( ptr, *( args++ ));
               job = strtol( ptr, endp, DEC );
            };
         };

         if ( null == strncmp( ptr, "--init-fle", 9 ))
         {                                         /* parameter file */
            ( state->uif ) = 'f';                  /* input mode */
            if ( --argn )
            {
               strcpy ( fleptr, *( args++ ));
            };
         };
      } while ( --argn );
   };
/*............................................................................*/
   state->fstjob = job; /* the 1st job index */
   state->job = job;    /* the running job index */

   strcpy( tmpfle, "/tmp/temp.XXXXX" );
/*............................................................................*/
   init_operts( );
   init_matter( );
   init_params( );
/*............................................................................*/
/* 't'ext mode: */

   if (( state->uif != 'g' )
     &&( state->uif != 'b' )
     &&( state->uif != 'f' ))
   {
      first_console:

      state->uif = 't'; /* user interface: 't'ext console */
      csp = txcnsl( null ); /* initialize the text console */

      state->cpmrk = null;
      csp->option = 3; do
      {
         strcpy( csp->cmmnt, "Welcome to HELIOS !" );

         csp->clscr = - ONE;
         csp->items = 3;
         csp->dfopt = 2;          /* the initial default option */
         csp->dflnf = csp->dfopt; /* set line feed before default option line */

         strcpy( csp->envmt, "HELIOS" );
         strcpy( csp->tasks, "Select ...:" );

         strcpy( lnestr, "* parameter input file \"" );
         strcat( lnestr, IPT_PARINIT );
         strcat( lnestr, lotos( state->job, null ));
         strcat( lnestr, "\"");

         strcpy( csp->mline[1], lnestr ); 
         strcpy( csp->mline[2], "* text console input " );
         strcpy( csp->mline[3], "* support" );

         strcpy( csp->escpe, "End of program / escape" );

         if ( state->job == null )
            strcpy( csp->cnfrm,\
               "Nothing done! Do you really want to quit ?" );
         else
            strcpy( csp->cnfrm, 
               "Do you really want to quit ?" );

/*............................................................................*/
         csp = txcnsl( csp );   /* build the [ start ] menu                   */
/*............................*/

         switch ( csp->option )
	 {
           default:
           break;

/*............................................................................*/
	   case 0:
	   case 'y':
           case 'Y':
            return null;
	   break;

/*............................................................................*/
	   case 1:
/* clear screen: */

            fprintf( stdout, CLEAR_LINE );
            fprintf( stdout, "\n ======================================="
                                "=======================================" );
            nseconds = time( timer );
            strcpy( timeptr, ctime( &nseconds ));
            fprintf( stdout, "\n ");
            PRBLDCLR( "HELIOS started:");
            fprintf( stdout, "\n %s", timeptr );
            PRNORMAL( "" );

            state->cpmrk = TWO;
	   break;

/*............................................................................*/
           case 2:         
	   break;

/*............................................................................*/
	   case 3:
	    fprintf( stdout, " %s",\
               "======================================="\
	       "=======================================" );
            PRBLDCLR( "" );
	    fprintf( stdout, "\n Call: +49+8061.936362 or" );
            fprintf( stdout, "\n email: contact@sfenx.de" );
	    fprintf( stdout, "\n [ Don't hesitate to ask your questions.]" );
            PRNORMAL( "");

            csp->dfopt = 2; /* next default: text console input */
           break; 
         };
      } while( csp->option == 3 );
   };

   if (( state->cpmrk < TWO )
     &&( state->uif == 't' )) /* text console input */
   {
      strcpy( csp->cmmnt, "Welcome back to HELIOS !" );
      strcpy( csp->cnfrm, "Nothing done! Do you really want to quit ?" );

      csp->dfopt = 4; /* next default: complete configuration */
      csp->clscr = 1;

     second_console:

      state->cpmrk = null;
      csp->items = 6;
      csp->dflnf = csp->dfopt; /* set line feed before default option */

      nseconds = time( timer );
      strcpy( timeptr, ctime( &nseconds ));
      strcpy( csp->title, "Program HELIOS: " );
      strncat( csp->title, timeptr, 24 );

      strcpy( csp->envmt, "HELIOS" );
      strcpy( csp->tasks, "Define ...:" );

      strcpy( csp->mline[1], "* computation modes" );
      strcpy( csp->mline[2], "* conductors, dielectric materials" );
      strcpy( csp->mline[3], "* geometric and physical parameters" );
      strcpy( csp->mline[4], "* the complete configuration" );
      strcpy( csp->mline[5], "* start computation" );
      strcpy( csp->mline[6], "* support" );

      strcpy( csp->escpe, "End of program / escape" );

      if ( state->job == null )
         strcpy( csp->cnfrm,\
            "Nothing done! Do you really want to quit ?" );
      else
         strcpy( csp->cnfrm, 
            "Do you really want to quit ?" );

/*............................................................................*/
      csp = txcnsl( csp );      /* build the [ start ] menu                   */
/*............................*/

      ( state->act ) = null; /* the actual prog stage [ null = par input ] */

      switch( csp->option )
      {
        default:
         break;

        case 0:
        case 'y':
        case 'Y':
         goto terminal;
         break;

        case 1:
/*............................................................................*/
         ii = input( "operations" );     /*                                        */
/*.....................................*/
         if ( ii == -ONE ) 
	   return null;
         else if ( ii == ONE ) /*start computation */
	 {
           state->cpmrk = ONE;
	   break;
         };
/*............................................................................*/
         ( csp->dfopt ) = 2; /* the next default menu option */
         ( csp->clscr ) = 1; /* clear screen; scroll N lines */
         break;

        case 2:
/*............................................................................*/
         ii = input( "materials" );     /*                                         */
/*....................................*/
         if ( ii == -ONE ) 
	   return null;
         else if ( ii == ONE ) /*start computation */
	 {
           state->cpmrk = ONE;
	   break;
         };
/*...........................................................................*/
         ( csp->dfopt ) = 3; /* the next default menu option */
         ( csp->clscr ) = 1; /* clear screen; scroll N lines */
         break;

        case 3:
/*............................................................................*/
         ii = input( "parameters" );     /*                                        */
/*.....................................*/
         if ( ii == -ONE ) 
	   return null;
         else if ( ii == ONE ) /*start computation */
	 {
           state->cpmrk = ONE;
	   break;
         };
/*...........................................................................*/
         ( csp->dfopt ) = 5; /* the next default menu option */
         ( csp->clscr ) = 1; /* clear screen; scroll N lines */
         break;

        case 4:
/*............................................................................*/
         ii = input( "operations" );
         if ( ii == -ONE ) 
	   return null;
         else if ( ii == ONE ) /*start computation */
	 {
           state->cpmrk = ONE;
	   break;
         };

         ii = input( "materials" );
         if ( ii == -ONE ) 
	   return null;
         else if ( ii == ONE ) /*start computation */
	 {
           state->cpmrk = ONE;
	   break;
         };

         ii = input( "parameters" );
         if ( ii == -ONE ) 
	   return null;
         else if ( ii == ONE ) /*start computation */
	 {
           state->cpmrk = ONE;
	   break;
         };
         ( csp->dfopt ) = 5; /* the next default menu option */
         ( csp->clscr ) = 1; /* clear screen; scroll N lines */
	 
        break;

/*...........................................................................*/
        case 5:
         state->cpmrk = ONE;

        break;
/*...........................................................................*/
        case 6:
		
         PRBLDCLR( "" );
	 fprintf( stdout, "\n Call: +49+8061.936362 or" );
         fprintf( stdout, "\n email: contact@sfenx.de" );
         fprintf( stdout, "\n [ Don't hesitate to ask your questions.]" );
         PRNORMAL( "\n");
	 
         csp->dfopt = 4; /* the initial default menu option */
	 
         break;
      };

      if (( null < csp->option )
        &&( state->cpmrk == null ))
      {
         strcpy( csp->cmmnt, "Welcome back to HELIOS !" );
         goto second_console;
      }
      else if ( null < state->cpmrk )
      {
         rvise_params( );
/*............................................................................*/
/* clear screen: */

         fprintf( stdout, CLEAR_LINE );
         fprintf( stdout,
	    "\n ======================================="
               "=======================================" );
         nseconds = time( timer );
         strcpy( timeptr, ctime( &nseconds ));
         fprintf( stdout, "\n ");
         PRBLDCLR( "HELIOS started:");
         fprintf( stdout, "\n %s", timeptr );
         PRNORMAL( "" );

      }; /* end if state->cpmrk == ONE */
   }; /* end if ( state->uif ) != 'f','g','b' */

   if (( state->cpmrk == TWO )
     ||( state->uif != 't' )) /* parameter 'f'ile input */
   {                          /* [graphical mode, e.g.] */
/*............................................................................*/
/* enter computation modes: */

      if (( null == fleptr[null] )
        ||( state->cpmrk == TWO ))
      {
         strcpy( fleptr, IPT_PARINIT );
         strcat( fleptr, lotos( state->job, null ));
      };

      rvise_operts( ); /* revise/reconfigure ...*/
      ii = rread_operts( fleptr, 'f' );

      if (( state->cpmrk == TWO )
        &&( ii == null ))
         goto first_console; 

      rvise_operts( ); /* revise/reconfigure ...*/

      strcpy( tmpfle, "opr.log" );
      strcat( tmpfle, lotos( state->job, null ));
      store_operts( tmpfle, 'o' ); /* restore revised file as log file */
/*............................................................................*/
/* enter material parameters: */

      rvise_matter( ); /* revise/reconfigure ...*/
      ii = rread_matter( fleptr, 'f' );

      if (( state->cpmrk == TWO )
        &&( ii == null ))
         goto first_console; 

      rvise_matter( ); /* revise/reconfigure ...*/

      strcpy( tmpfle, "mat.log" );
      strcat( tmpfle, lotos( state->job, null ));
      store_matter( tmpfle, 'm' ); /* restore revised file as log file */
/*............................................................................*/
/* enter physical parameters: */

      rvise_params( );             /* revise/reconfigure */
      
      ii = rread_params( fleptr, 'f' );
      if (( state->cpmrk == TWO )
        &&( ii == null ))
         goto first_console; 

      rvise_params( );             /* revise/reconfigure */
   };
/*............................................................................*/
/* open process log file: */

   logfle = fopen( logptr , "a+" );
   setvbuf( logfle, null, _IONBF, null );

   nseconds = time( timer );
   strcpy( timeptr, ( ctime( &nseconds ) + 11 ));

   fprintf( logfle, "\nJob no " );
   fprintf( logfle, "%s ", ( lotos( state->job, null )));
   fprintf( logfle, "launched at " );
   fprintf( logfle, "%-.8s", timeptr );

   ( state->fleps ) = ftell( logfle );

   fclose( logfle );
/*............................................................................*/
/* create final parameter log: */

   ( state->act ) = ONE; /* the actual program stage [ ONE = computation ] */

   rvise_params( ); /* the final parameter revision */
   strcpy( fleptr, IPT_PARLOG );
   strcat( fleptr, lotos( state->job, null ));
   store_params( fleptr, 'p' ); /* restore revised parameter file as log file */
   
/* initialize: */
   state->ctl = null; /* <- deactivates homogeneity check initially */
/*............................................................................*/
   state = initlze( state );     /*                                           */
/*.............................*/
/* now  start the iterations: */

/*............................................................................*/
   state = hlswrk( state );     /*                                            */
/*............................*/
/* ... it's terminated [ continue ? ] */

   logfle = fopen( logptr, "a+" );

   nseconds = time( timer );
   strcpy( timeptr, ( ctime( &nseconds ) + 4 ));

   fprintf( logfle, "\nJob no %d terminated ", ( state->job ));
   fprintf( logfle, "%-.20s", timeptr );
   fprintf( logfle, "\n%s", dline );

   ( state->fleps ) = ftell( logfle );
   fclose( logfle );

   ++( state->job ); /* next job label */

   if ( state->uif == 't' ) /* text input */
   {
      strcpy( csp->cmmnt, "Welcome back to HELIOS !" );
      csp->dfopt = 0; /* the next default menu option */
      csp->clscr = 0; /* clear screen; scroll N lines */

      if ( state->cpmrk == TWO )
         goto first_console;
      else
         goto second_console;
   };
   
  terminal:
   return null;
}  
/*============================================================================*/
