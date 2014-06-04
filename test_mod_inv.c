#include <stdio.h>

int mul_inv(int a, int b)
{

    int b0 = b, t, q;
    int x0 = 0, x1 = 1;
    if (b == 1) return 1;
    while (a > 1) {
        //printf( "a: %d\n", a );
        //printf( "b: %d\n", b );
        q = a / b;
        t = b, b = a % b, a = t;
        t = x0, x0 = x1 - q * x0, x1 = t;
        //printf( "t: %d\n", t );
    }
    if (x1 < 0) x1 += b0;
    return x1;
}

int main(void) {
    printf("42 mod 2017\ninverse: %d\n", mul_inv(42, 2017));
    return 0;
}

