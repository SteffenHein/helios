void init_params( void )
{
   static short
      ii = null,
      jj = null;

   void deflt_params( void );
   short rvise_params( void );
/*----------------------------------------------------------------------------*/
   if ( ( short ) par.s[null] == null )
   {
/* initialize; enter default parameters: */

      for ( ii=null; ii<=PARMTRS; ii++ )
         par.stx[ii] = ( char *) calloc( STS_SIZE, ONE );

      for ( ii=null; ii<=PARMTRS; ii++ )
         for ( jj=null; jj<STS_SIZE; jj++ )
            par.stx[ii][jj] = ( char ) null;
/*............................................................................*/
      deflt_params( );     /*                                                 */
      rvise_params( );    /*                                                  */
/*.......................*/
   };
   return;
}
