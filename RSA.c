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
    //mpz_set_str( p, "103", 10 );
    //mpz_set_str( q, "107", 10 );
    mpz_set_str( p, "13", 10 );
    mpz_set_str( q, "17", 10 );

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

void cryptwork3( mpz_t ret, mpz_t m, mpz_t ed, mpz_t n ){
    unsigned int r = 3;
    unsigned int k = mpz_sizeinbase( ed, 2 );
    unsigned int s = 0;
    unsigned int mary = 0;
    unsigned int dec = 0;
    unsigned int currWord = 0;
    unsigned int totother=pow( 2, r );
    char *sr;
    mpz_t c, tmp;
    
    if( k % r != 0 )
        k += r - (k % r );

    s = k/r;

    for( int i = 0; i < r; i++ )
        mary += pow( 2, i );

    sr = malloc( k );
    if( sr == NULL ){
        printf( "No memory!\n" );
        exit(0);
    }

    for( int i = (int)k; i > 0; i--){
        if( i > mpz_sizeinbase( ed, 2 ) ){
            sr[ k-i ] = '0';
        }else{
            sr[ k-i ] = '0'+mpz_tstbit( ed, i - 1 );
        }
    }

    /*mpz_init( F1 );
    mpz_init( F2 );
    mpz_init( F3 );
    mpz_init( F4 );
    mpz_init( F5 );
    mpz_init( F6 );
    mpz_init( F7 );
*/
    //mpz_powm_ui( F1,  

/*    Mw = malloc( 1 );
    mpz_init( wn );
    for( int i = 0; i < mary; i++ ){
        mpz_pow_ui( wn, m, i );
        mpz_mod( wn, wn, ed );
        Mw = realloc( Mw, i*sizeof( wn ) );
        mpz_set( Mw[ i ], wn );
    }
*/    
    mpz_init( c );
    mpz_init( tmp );

    for( int i = 0 ; i < r+(r*currWord); i++ )
        dec = dec * 2 + (sr[ i ] == '1' ? 1 : 0);
   
    currWord++; 
    mpz_pow_ui( c, m, dec );
    mpz_mod( c, c, n );

    for( int i = 0; i < s-1; i++ ){
        dec = 0;
        for( int i = r*currWord ; i < r+(r*currWord); i++ )
            dec = dec * 2 + (sr[ i ] == '1' ? 1 : 0);
        currWord++;

        mpz_pow_ui( tmp, c, totother );
        mpz_mod( c, tmp, n );
        if( dec != 0 ){
            mpz_pow_ui( tmp, m, dec );
            mpz_mul( c, c, tmp );
            mpz_mod( c, c, n );
        } 
    }

    //for( int i = 0; i < mary; i++ ){
    //    mpz_clear( Mw[ i ] );
    //}
    mpz_set( m, c );
    mpz_clear( c );
    mpz_clear( tmp );
    free( sr );
  //  free( Mw );
  //  mpz_clear( wn );
}



    // m-ary method (m=4)
void cryptwork4( mpz_t ret, mpz_t m, mpz_t ed, mpz_t n ){
    unsigned int r = 5;
    unsigned int k = mpz_sizeinbase( ed, 2 );
    unsigned int words = 0;
    int l;
    char *sr;
    mpz_t *Fs, c, tmp, w1, w2, wn;

    for( int i = 0; i < r ; i++ )
        words += pow( 2, i );

    if( k % r != 0 )
        k += r - (k % r );

    mpz_init( tmp );
    sr = realloc( NULL, k );
    if( sr == NULL ){
        printf( "no memory\n" );
        exit(0);
    }
    Fs = realloc( NULL, ( words+2 )*sizeof( mpz_t ) );
    
    l = 0;
    for( int i=(int)k; i> 0; i-- ){
        if( i > mpz_sizeinbase( ed, 2 ) ){
            sr[ l ] = '0';
        }else{
            sr[ l ] = '0'+mpz_tstbit( ed, i-1 );
        }
        l++;
    }
    mpz_init_set_ui( w1, 1 );
    mpz_set( Fs[ 0 ],  w1);
    mpz_init_set( w2, m );
    mpz_set( Fs[ 1 ], w2);
    mpz_init( c );

    printf( "checkpnt 3\n" );
    mpz_init( wn );
    
    for( int j=2; j < words+1; j++ ){
        printf( "checkpnt 3.1\n" );
        
        mpz_set_ui( wn, 0 );
        printf( "checkpnt 3.2\n" );
        
        mpz_mul( wn, m, Fs[ j-1 ] );
        printf( "checkpnt 3.3\n" );
        
        mpz_mod( wn, wn, n );
    printf( "words: %d, j: %d Fs[ j ]: ", words, j );
        mpz_out_str( stdout, 10, Fs[ j ] );
        printf( " Fs[ j-1 ]: " );
        mpz_out_str( stdout, 10, Fs[ j-1 ] );
        printf( "\n" );
        printf( "checkpnt 3.4\n" );

        mpz_set( Fs[ j ], wn );
        printf( "checkpnt 3.5\n" );
    }

    int dec = 0;
    l = 0;
    int pow2 = pow( 2, r );
    for( ; l < r; l++ )
        dec = dec * 2 + (sr[ l ] == '1' ? 1 : 0);
    
    printf( "checkpnt 4\n" );

    mpz_set( c, Fs[ dec ] );
    mpz_mod( c, c, n );

    mpz_set_ui( tmp, 0 );

    int s = k/r;
    int bitshifter = 1;

    for( int j = s-1; j > 0; j-- ){
        dec = 0;
        for( ; l < r + ( r*bitshifter ); l++ )
            dec = dec * 2 + (sr[ l ] == '1' ? 1 : 0);
        bitshifter++;

        mpz_pow_ui( c, c, pow2 );
        mpz_mod( c, c, n );

        if( dec != 0 ){
            mpz_pow_ui( tmp, m, dec );
            mpz_mul( c, c, tmp );
            mpz_mod( c, c, n );
        }
    }


    mpz_set( m, c );

    for( int j = 0; j < words + 1; j++ ){
        mpz_clear( Fs[ j ] );
    }

    mpz_clear( c );
    mpz_clear( *Fs );
    mpz_clear( tmp );
    mpz_clear( wn );
    mpz_clear( w1 );
    mpz_clear( w2 );
    free( sr );
}

int main(){
    // Do not enable this during testing
    //find_and_print_prime();
    // Do not enable this during testing
    //key_gen();
    mpz_t m, e, d, n, iters, counter;
    mpz_init_set_str( iters, "100000", 10 );
    mpz_init_set_str( counter, "0", 10 );
    mpz_init_set_str( m, "42", 10 );
/*
    mpz_init_set_str( e, "830738740898882569", 10 );
    mpz_init_set_str( d, "2461176518949177529", 10 );
    mpz_init_set_str( n, "4241621275235948237", 10 );
*/
    //mpz_init_set_str( e, "3269", 10 );
    //mpz_init_set_str( d, "9833", 10 );
    //mpz_init_set_str( n, "11021", 10 );

    mpz_init_set_str( e, "31", 10 );
    mpz_init_set_str( d, "31", 10 );
    mpz_init_set_str( n, "221", 10 );

    for( ; mpz_cmp( iters, counter ) > 0 ; mpz_add_ui( counter, counter, 1 ) ){
//        printf( "sta msg: " );
//        mpz_out_str( stdout, 10, m );
//        printf( "\n" );
        cryptwork0( m, m, e, n );
//        printf( "mid msg: " );
//        mpz_out_str( stdout, 10, m );
//        printf( "\n" );
        cryptwork0( m, m, d, n );
//        printf( "end msg: " );
//        mpz_out_str( stdout, 10, m );
//        printf( "\n" );
    }

    mpz_clear( m );
    mpz_clear( e );
    mpz_clear( d );
    mpz_clear( n );
    mpz_clear( iters );
    mpz_clear( counter );
    return 0;
}
