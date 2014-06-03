#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <gmp.h>

/*

https://www.cs.colorado.edu/~srirams/classes/doku.php/gmp_usage_tutorial

https://gmplib.org/manual/Integer-Functions.html

void mpz_powm_ui (mpz_t rop, const mpz_t base, unsigned long int exp, const mpz_t mod)
    Set rop to (base raised to exp) modulo mod.
    Negative exp is supported if an inverse base^-1 mod mod exists (see mpz_invert in 
    Number Theoretic Functions). If an inverse doesn't exist then a divide by zero is raised. 

void mpz_ui_pow_ui (mpz_t rop, unsigned long int base, unsigned long int exp)
void mpz_add (mpz_t rop, const mpz_t op1, const mpz_t op2)
void mpz_sub (mpz_t rop, const mpz_t op1, const mpz_t op2)
void mpz_mul (mpz_t rop, const mpz_t op1, const mpz_t op2)
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

    while (a > 1) {
        q = a / b;
        t = b, b = a % b, a = t;
        t = x0, x0 = x1 - q * x0, x1 = t;

    if( mpz_cmp_ui( b1, 1 ) == 0 )
        return;

    while( mpz_cmp_ui( a1, 1 ) > 1 ){
        mpz_fdiv_q( q, a1, b1 );
        mpz_set( t, a1 );
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
}

/*void key_gen( mpz_t n, mpz_t p, mpz_t q, mpz_t b ){
    mpz_t b1, b2;
    mpz_init( b1 );
    mpz_init( b2 );
    mpz_sub( b1, p, 1 );
    mpz_sub( b2, q, 1 );

    mpz_mul( n, p, q );

    mpz_mul( b, b1, b2 );
    gen_coprime( n, p );
}
*/
int main(){
    
/*    mpz_t i;
*/
    mpz_t p, q, n;//, b1, b2;
    mpz_init( p );
    mpz_init( q );
    mpz_init( n );
    mpz_set_str( p, "42", 10 );
    mpz_set_str( q, "2017", 10 );
    //mpz_set_str( p, "3120", 10 );
    //mpz_set_str( q, "54", 10 );
    //mpz_set_str( p, "15485863", 10 );
    //mpz_set_str( q, "13849831", 10 );
    //gen_coprime( n, p );

    mod_inv( n, p, q );

    mpz_out_str( stdout, 10, p );
    printf( " mod " );
    mpz_out_str( stdout, 10, q );
    printf( "\n" );
    printf( "inverse: " );
    mpz_out_str( stdout, 10, n );
    printf( "\n" );

    return 0;
}
