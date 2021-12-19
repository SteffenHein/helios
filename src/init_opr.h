void init_operts( void )
{
   static short 
      ii = null,
      jj = null;
      
   void deflt_operts( void );
   short rvise_operts( void );
/*............................................................................*/
   if ( opr.n[null] == null )
   {
/* initialize; enter default divisions */

      for ( ii=null; ii<=OPERATS; ii++ )
         for ( jj=null; jj<STS_SIZE; jj++ )
            opr.ntx[ii][jj] = ( char ) null;
/*............................................................................*/
      deflt_operts( );        /* enter default operations                     */
      rvise_operts( );       /*  revise default parameters                    */
/*.........................*/
   };
   return;
}
