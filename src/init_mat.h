void init_matter( void )
{
   static short
      ii = null,
      jj = null;

   void deflt_matter( void );
   short rvise_matter( void );
/*----------------------------------------------------------------------------*/
   if ( ( short ) mat.m == null )
   {
/* initialize; enter default materials: */

      for ( jj=null; jj<STS_SIZE; jj++ )
         mat.mto[jj] = ( char ) null;

      for ( jj=null; jj<STS_SIZE; jj++ )
         mat.sho[jj] = ( char ) null;

      for ( jj=null; jj<STS_SIZE; jj++ )
         mat.mti[jj] = ( char ) null;

      for ( jj=null; jj<STS_SIZE; jj++ )
         mat.shi[jj] = ( char ) null;

      for ( jj=null; jj<STS_SIZE; jj++ )
         mat.avm[jj] = ( char ) null;

      for ( ii=null; ii<=CMETALS; ii++ )
         mat.mtx[ii] = ( char *) calloc( STS_SIZE, ONE );

      for ( ii=null; ii<=CMETALS; ii++ )
         mat.msx[ii] = ( char *) calloc( STS_SIZE, ONE );

      for ( ii=null; ii<=CMETALS; ii++ )
         for ( jj=null; jj<STS_SIZE; jj++ )
            mat.mtx[ii][jj] = ( char ) null;

      for ( ii=null; ii<=CMETALS; ii++ )
         for ( jj=null; jj<STS_SIZE; jj++ )
            mat.msx[ii][jj] = ( char ) null;

      for ( jj=null; jj<STS_SIZE; jj++ )
         mat.del[jj] = ( char ) null;

      for ( jj=null; jj<STS_SIZE; jj++ )
         mat.shd[jj] = ( char ) null;

      for ( jj=null; jj<STS_SIZE; jj++ )
         mat.avd[jj] = ( char ) null;

      for ( ii=null; ii<=DIELCTS; ii++ )
         mat.dtx[ii] = ( char *) calloc( STS_SIZE, ONE );

      for ( ii=null; ii<=DIELCTS; ii++ )
         mat.dsx[ii] = ( char *) calloc( STS_SIZE, ONE );

      for ( ii=null; ii<=DIELCTS; ii++ )
         for ( jj=null; jj<STS_SIZE; jj++ )
            mat.dtx[ii][jj] = ( char ) null;

      for ( ii=null; ii<=DIELCTS; ii++ )
         for ( jj=null; jj<STS_SIZE; jj++ )
            mat.dsx[ii][jj] = ( char ) null;

      for ( jj=null; jj<STS_SIZE; jj++ )
         mat.jck[jj] = ( char ) null;

      for ( jj=null; jj<STS_SIZE; jj++ )
         mat.shj[jj] = ( char ) null;

      for ( jj=null; jj<STS_SIZE; jj++ )
         mat.cpe[jj] = ( char ) null;

      for ( jj=null; jj<STS_SIZE; jj++ )
         mat.shc[jj] = ( char ) null;

      for ( jj=null; jj<STS_SIZE; jj++ )
         mat.avs[jj] = ( char ) null;

      for ( ii=null; ii<=SLDMTRS; ii++ )
         mat.stx[ii] = ( char *) calloc( STS_SIZE, ONE );

      for ( ii=null; ii<=SLDMTRS; ii++ )
         mat.ssx[ii] = ( char *) calloc( STS_SIZE, ONE );

      for ( ii=null; ii<=SLDMTRS; ii++ )
         for ( jj=null; jj<STS_SIZE; jj++ )
            mat.stx[ii][jj] = ( char ) null;

      for ( ii=null; ii<=SLDMTRS; ii++ )
         for ( jj=null; jj<STS_SIZE; jj++ )
            mat.ssx[ii][jj] = ( char ) null;

      for ( jj=null; jj<STS_SIZE; jj++ )
         mat.fld[jj] = ( char ) null;

      for ( jj=null; jj<STS_SIZE; jj++ )
         mat.shf[jj] = ( char ) null;

      for ( jj=null; jj<STS_SIZE; jj++ )
         mat.avf[jj] = ( char ) null;

      for ( ii=null; ii<=COOLFLS; ii++ )
         mat.ftx[ii] = ( char *) calloc( STS_SIZE, ONE );

      for ( ii=null; ii<=COOLFLS; ii++ )
         mat.fsx[ii] = ( char *) calloc( STS_SIZE, ONE );

      for ( ii=null; ii<=COOLFLS; ii++ )
         for ( jj=null; jj<STS_SIZE; jj++ )
            mat.ftx[ii][jj] = ( char ) null;

      for ( ii=null; ii<=COOLFLS; ii++ )
         for ( jj=null; jj<STS_SIZE; jj++ )
            mat.fsx[ii][jj] = ( char ) null;
/*............................................................................*/
      deflt_matter( );      /*                                                */
      rvise_matter( );     /*                                                 */
/*.......................*/
   };
   return;
}
