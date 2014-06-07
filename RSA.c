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
    //mpz_set_str( p, "13", 10 );
    //mpz_set_str( q, "17", 10 );
    mpz_set_str( p, "7666172106961010188607710952610064715673142380525941804482087895724963026643399329833714724398474437462001234767364783426177872612952693520323367645787914454104091663430279483312486353933611458403472823346013808552729555271107930953546428959560589120785484857003352866980809284265958629308686216673463", 10 );
    mpz_set_str( q, "9227304258488643571457905727956872530022057832870021065038620292479116815433625757787708410630607032577001806875329135827517309877908822634795599019285392338160662795921635744095284093297975347686807674793820805619539325307228550555082168125293765850058617264667195854606399942290488010751026658243227", 10 );

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

    mpz_set( m, c );
    mpz_clear( c );
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
/*
    mpz_init_set_str( e, "830738740898882569", 10 );
    mpz_init_set_str( d, "2461176518949177529", 10 );
    mpz_init_set_str( n, "4241621275235948237", 10 );
*/
    //mpz_init_set_str( e, "3269", 10 );
    //mpz_init_set_str( d, "9833", 10 );
    //mpz_init_set_str( n, "11021", 10 );
/*
    mpz_init_set_str( e, "31", 10 );
    mpz_init_set_str( d, "31", 10 );
    mpz_init_set_str( n, "221", 10 );
*/
    mpz_init_set_str( e, "8741500521860928100462572966473463672441659338550227337791296443213109592114661033966960724136761810841434280815516683363658238182831355421639071958827617404164512416852950952591910362394529524772233291899658466016028551314357424893292400611897359961667561416168953712286626503536220531557372634449295999566280735496206073657995048559726791621706180126274587091547321886041129992261901118323710660127761570247708506786564896032898111373466395659260190837722016930496773893906384290872953858569569470930049106196059409032050271604694925169906698742374244244702651749347566044929920731265748922520438557", 10 );
    mpz_init_set_str( d, "60168529103217807457215739625313932459090284786427810715336228479238509896223033874391098363275751505399343770847939534385474634439233863790619424938210721808384463364528630008582097232617354876703150400867802883031780027222207051754481051114153874020172515304729886859552806529843974624349768412200514711035286208638490538556677504286855960942623882164556772033467467414481465500026601660865686672095514539368997705649551214699290529370492888982887322616204848495794626937850988076659532540225466298097011993239742592891128762038477015021854588252882262927236431853364262629394562129101256029791332729", 10 );
    mpz_init_set_str( n, "70738102528868186471077720460261394393246672392452678056007650479344300726348942698000587969531163078569497997607991868260711388840777586682289541117601777237999377487258658861824790826177878222028835045909883709706794196706296215964342884343045054229136096688991238599450607684923819690064080931120591125020412273715110238578228163621081501866503529231010252237313979351853892795839167121955267176666704119397719063529148722626864322912262553141859931616266426771709856206037226795783616256482968126275716075843040211871019386785926783785918898799853858777511894954373523255168865888428616599490385101", 10 );
    for( ; mpz_cmp( iters, counter ) > 0 ; mpz_add_ui( counter, counter, 1 ) ){
        //printf( "sta msg: " );
        //mpz_out_str( stdout, 10, m );
        //printf( "\n" );
        cryptwork3( m, m, e, n );
        //printf( "mid msg: " );
        //mpz_out_str( stdout, 10, m );
        //printf( "\n" );
        cryptwork3( m, m, d, n );
        //printf( "end msg: " );
        //mpz_out_str( stdout, 10, m );
        //printf( "\n" );
    }

    mpz_clear( m );
    mpz_clear( e );
    mpz_clear( d );
    mpz_clear( n );
    mpz_clear( iters );
    mpz_clear( counter );
    return 0;
}
