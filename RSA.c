#include <stdio.h>
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

void key_gen( mpz_t p, mpz_t q ){
   mpz_t n;
   mpz_init( n ); 
}

int main(){
/*    mpz_t i;
    mpz_init( i );
    mpz_set_ui( i, 0 );
    mpz_set_str( i, "1918273981273918273918273981273918723981723981723981723981273981723981729381729387129387129387719283719827379128739182773981277391827391827391827391827398172323123123123123123123123123123123123123123123123123123123123", 10 );

    printf( "tallet er: " );
    mpz_out_str( stdout, 10, i );
    printf( "\n" );

    mpz_add_ui( i, i, 1 );
    
    printf( "tallet er: " );
    mpz_out_str( stdout, 10, i );
    printf( "\n" );

    mpz_clear( i );
*/
    mpz_t p, q;
    mpz_init( p );
    mpz_init( q );
    return 0;
}
