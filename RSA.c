#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <gmp.h>
#include <math.h>

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

void key_gen(){

    mpz_t p, q, e, d, n;
    mpz_t b1, b2, totient;
    mpz_init( p );
    mpz_init( q );
    mpz_init( e );
    mpz_init( d );
    mpz_init( n );
    mpz_init( b1 );
    mpz_init( b2 );
    mpz_init( totient );

    //mpz_set_str( p, "15485863", 10 );
    //mpz_set_str( q, "13849831", 10 );
    //mpz_set_str( p, "2059519673", 10 );
    //mpz_set_str( q, "2059519669", 10 );
    mpz_set_str( p, "103", 10 );
    mpz_set_str( q, "107", 10 );

    mpz_mul( n, p, q );

    mpz_sub_ui( b1, p, 1 );
    mpz_sub_ui( b2, q, 1 );
    mpz_mul( totient, b1, b2 );
    gen_coprime( e, totient );
    mod_inv( d, e, totient );

    printf( "Generated the following keys:\nN:\n" );
    mpz_out_str( stdout, 10, n );
    printf( "\nE: \n" );
    mpz_out_str( stdout, 10, e );
    printf( "\nD: \n" );
    mpz_out_str( stdout, 10, d );
    printf( "\n" );

    mpz_clear( b1 );
    mpz_clear( b2 );
    mpz_clear( p );
    mpz_clear( q );
    mpz_clear( e );
    mpz_clear( d );
    mpz_clear( n );
    mpz_clear( totient );
}
/*
void encrypt(){
    mpz_powm( cm, m, e, n );
    mpz_powm( mc, cm, d, n );

}*/

void find_and_print_prime(){
    mpz_t p, iters, counter;
    mpz_init_set_str( p, "15485863", 10 );
    mpz_init_set_str( iters, "1000000", 10 );
    mpz_init_set_ui( counter, 0 );
    for( ; mpz_cmp( iters, counter ) > 0 ; mpz_add_ui( counter, counter, 1 ) ){
        printf( "looking for prime" );
        mpz_nextprime( p, p );
    }
    printf( "found prime:\n" );
    mpz_out_str( stdout, 10, p );
    printf( "\n" );
    mpz_clear( p );
    mpz_clear( iters );
    mpz_clear( counter );
}

/**
  *
  * r is the return value, m is the message, ed is the exponent, n is the mod factor
  *
 **/
void cryptwork0( mpz_t r, mpz_t m, mpz_t ed, mpz_t n ){
    mpz_powm( r, m, ed, n );
}

void cryptwork( mpz_t r, mpz_t m, mpz_t ed, mpz_t n ){
    mpz_t c, counter;
    mpz_init_set_ui( c, 1 );
    mpz_init_set_ui( counter, 0 );
    
    for( ; mpz_cmp( ed, counter ) > 0 ; mpz_add_ui( counter, counter, 1 ) ){
        mpz_mul( c, c, m );
        mpz_mod( c, c, n );
    }
    mpz_set( m, c );
    mpz_clear( c );
    mpz_clear( counter );

}

// Binary method
void cryptwork2( mpz_t r, mpz_t m, mpz_t ed, mpz_t n ){
    mp_bitcnt_t b;
    mpz_t c;
    unsigned int k = mpz_sizeinbase( ed, 2 );

    if( mpz_tstbit( ed, k-1 ) == 1 )
        mpz_init_set( c, m );
    else
        mpz_init_set_ui( c, 1 );

    for( unsigned int i = k-1; i > 0; i-- ){
        mpz_mul( c, c, c );
        mpz_mod( c, c, n );

        if( mpz_tstbit( ed, i-1 ) == 1 ){
            mpz_mul( c, c, m );
            mpz_mod( c, c, n );
        }
    }
    mpz_set( r, c );
    mpz_clear( c );
}

    // m-ary method (m=4)
void cryptwork3( mpz_t ret, mpz_t m, mpz_t ed, mpz_t n ){
    int mary = 3;
    unsigned int r = mary;
    unsigned int k = mpz_sizeinbase( ed, 2 );
    unsigned int s = 0;
    for( int i = 0; i < mary ; i++ )
        s += pow( 2, i );

    if( k % r != 0 )
        k += r - (k % r );

    mpz_t *Fs, c, tmp;
    mpz_init( tmp );
    char *sr;
    sr = malloc( k );
    if( sr == NULL ){
        printf( "no memory\n" );
        exit(0);
    }
    Fs = malloc( (s+1)*sizeof( mpz_t ) );
    
    int j = 0;
    for( int i=(int)k; i> 0; i-- ){
        if( i > mpz_sizeinbase( ed, 2 ) ){
            sr[ j ] = '0';
        }else{
            sr[ j ] = '0'+mpz_tstbit( ed, i-1 );
        }
        j++;
    }

    mpz_t w1, w2, wn;
    mpz_init_set_ui( w1, 1 );
    mpz_set( Fs[ 0 ],  w1);
    mpz_init_set( w2, m );
    mpz_set( Fs[ 1 ], w2);
    mpz_init( c );

    for( int i=2; i < s+1; i++ ){
        mpz_init( wn );
        mpz_mul( wn, m, Fs[ i-1 ] );
        mpz_mod( wn, wn, n );
        mpz_set( Fs[ i ], wn );
    }


    int dec = 0;
    int i = 0;
    int pow2 = pow( 2, r );
    for( ; i < r; i++ )
        dec = dec * 2 + (sr[i] == '1' ? 1 : 0);
    
    mpz_set( c, Fs[ dec ] );
    mpz_mod( c, c, n );

    mpz_set_ui( tmp, 0 );
    for( int j = s-1; j > 0; j-- ){
        dec = 0;
        for( ; i < r; i++ )
            dec = dec * 2 + (sr[i] == '1' ? 1 : 0);
        mpz_pow_ui( c, c, pow2 );
        mpz_mod( c, c, n );
        if( dec != 0 ){
            mpz_powm( c, c, m, Fs[ dec ] );
        }
    }


    mpz_set( m, c );

    mpz_clear( *Fs );
    mpz_clear( w1 );
    mpz_clear( w2 );
    mpz_clear( tmp );
    free( sr );
}

int main(){
    // Do not enable this during testing
    //find_and_print_prime();
    // Do not enable this during testing
    //key_gen();
    mpz_t m, e, d, n, iters, counter;
    mpz_init_set_str( iters, "1", 10 );
    mpz_init_set_str( counter, "0", 10 );
    mpz_init_set_str( m, "42", 10 );

    mpz_init_set_str( e, "830738740898882569", 10 );
    mpz_init_set_str( d, "2461176518949177529", 10 );
    mpz_init_set_str( n, "4241621275235948237", 10 );

    //mpz_init_set_str( e, "3269", 10 );
    //mpz_init_set_str( d, "9833", 10 );
    //mpz_init_set_str( n, "11021", 10 );

    for( ; mpz_cmp( iters, counter ) > 0 ; mpz_add_ui( counter, counter, 1 ) ){
        printf( "sta msg: " );
        mpz_out_str( stdout, 10, m );
        cryptwork( m, m, e, n );
        printf( "\nmid msg: " );
        mpz_out_str( stdout, 10, m );
        cryptwork( m, m, d, n );
        printf( "\nend msg: " );
        mpz_out_str( stdout, 10, m );
        printf( "\n" );
    }

    mpz_clear( m );
    mpz_clear( e );
    mpz_clear( d );
    mpz_clear( n );
    mpz_clear( iters );
    mpz_clear( counter );
    return 0;
}
