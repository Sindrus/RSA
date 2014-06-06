#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <stdio.h>
#include <gmp.h>

void first_num( mpz_t ro ){

    char *bitstr;
    int r = 1;
    bitstr = malloc( 1001*sizeof( char ) );
    bitstr[ 0 ] = '0'+r;
    srand( time( NULL ) );
    
    for( int i = 1; i < 1000; i++ ){
        r = ( rand() )%100;
        bitstr[ i ] = '0'+r%2;
    }
    
    bitstr[ 1001 ] = '\0';
    mpz_set_str( ro, bitstr, 2 );
}

int main(){
    mpz_t p;
    mpz_init( p );
    first_num( p );
    mpz_nextprime( p, p );
    mpz_out_str( stdout, 10, p ); 
    printf( "\n" );

    return 0;
}
