#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <gmp.h>

/*
https://www.cs.colorado.edu/~srirams/classes/doku.php/gmp_usage_tutorial

https://gmplib.org/manual/Integer-Functions.html
*/

void gcd( mpz_t r, mpz_t a, mpz_t b ){
    mpz_t a1, b1, c1;
    mpz_init_set( a1, a );
    mpz_init_set( b1, b );
    mpz_init( c1 );
    while ( mpz_cmp_si( a1, 0 ) != 0 ){
        mpz_set( c1, a1 );
        mpz_mod( a1, b1, a1 );
        mpz_set( b1, c1 );
        /*c = a; 
        a = b%a;  
        b = c;*/
    }
    mpz_set( r, b1 );
    mpz_clear( a1 );
    mpz_clear( b1 );
    mpz_clear( c1 );
}

void mod_inv( mpz_t r, mpz_t a, mpz_t b ){
    mpz_t b0, b1, a1, t, q, x0, x1, tmp;
    mpz_init( t );
    mpz_init( q );
    mpz_init( tmp );
    mpz_init_set( b0, b );
    mpz_init_set( b1, b );
    mpz_init_set( a1, a );
    mpz_init_set_ui( x0, 0 );
    mpz_init_set_ui( x1, 1 );

    if( mpz_cmp_ui( b1, 1 ) == 0 )
        return;

    while( mpz_cmp_si( a1, 1 ) > 0 ){
        mpz_fdiv_q( q, a1, b1 );
        mpz_set( t, b1 );
        mpz_mod( b1, a1, b1 );
        mpz_set( a1, t );
        mpz_set( t, x0 );
        mpz_mul( tmp, q, x0 );
        mpz_sub( x0, x1, tmp );
        mpz_set( x1, t );
    }

    if( mpz_cmp_ui( x1, 0 ) < 0 )
        mpz_add( x1, x1, b0 );

    mpz_set( r, x1 );
    mpz_clear( b0 );
    mpz_clear( b1 );
    mpz_clear( a1 );
    mpz_clear( t );
    mpz_clear( q );
    mpz_clear( x0 );
    mpz_clear( x1 );
    mpz_clear( tmp );
}

void gen_coprime( mpz_t coprime, mpz_t upper_bound ){
    mpz_t test_number, coprime_r;
    mpz_init( test_number );
    mpz_init( coprime_r );
    gmp_randstate_t rand;
    gmp_randinit_default( rand );
    gmp_randseed_ui( rand, time( NULL ) );

    do{
        mpz_urandomm( test_number, rand, upper_bound );
        gcd( coprime_r, test_number, upper_bound );
    }while( mpz_cmp_si( coprime_r, 1 ) != 0 );

    mpz_set( coprime, test_number );

    mpz_clear( test_number );
    mpz_clear( coprime_r );
}

void key_gen( mpz_t d, mpz_t e, mpz_t n, mpz_t p, mpz_t q ){

    mpz_t b1, b2, totient;
    mpz_init( b1 );
    mpz_init( b2 );
    mpz_init( totient );

    mpz_mul( n, p, q );

    mpz_sub_ui( b1, p, 1 );
    mpz_sub_ui( b2, q, 1 );
    mpz_mul( totient, b1, b2 );
    gen_coprime( e, totient );
    mod_inv( d, e, totient );

    mpz_clear( b1 );
    mpz_clear( b2 );
    mpz_clear( totient );
}

int main(){
    
    mpz_t p, q, e, d, n, m, mc, cm ;
    mpz_init( p );
    mpz_init( q );
    mpz_init( e );
    mpz_init( d );
    mpz_init( n );
    mpz_init( mc );
    mpz_init( cm );
    mpz_init_set_ui( m, 65 );
    mpz_set_str( p, "15485863", 10 );
    mpz_set_str( q, "13849831", 10 );

    key_gen( d, e, n, p, q );

    mpz_powm( cm, m, e, n );
    mpz_powm( mc, cm, d, n );

    mpz_out_str( stdout, 10, m );
    printf( "\n" );
    mpz_out_str( stdout, 10, cm );
    printf( "\n" );
    mpz_out_str( stdout, 10, mc );
    printf( "\n" );

    mpz_clear( p );
    mpz_clear( q );
    mpz_clear( e );
    mpz_clear( d );
    mpz_clear( n );
    mpz_clear( m );
    mpz_clear( mc );
    mpz_clear( cm );
    return 0;
}
