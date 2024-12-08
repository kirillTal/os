#include <stdio.h>
#include "include/gcd_euclid.c"
#include "include/integral_rectangle.c"

void solve() {
    int number_of_function;
    scanf("%d", &number_of_function);

    if (number_of_function == 1) {
        float A, B, e;
        scanf("%f %f %f", &A, &B, &e);

        float result = SinIntegral(A, B, e);
        printf("$\\int_A^B sin(x)dx \\approx %f$\n", result);
    } else {
        int A, B;
        scanf("%d %d", &A, &B);

        int result = GCD(A, B);
        printf("GCD(%d, %d)=%d\n", A, B, result);
    }
}

int main() {
    solve();
}