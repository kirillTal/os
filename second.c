#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>

float (*integral)(float, float, float);
int (*gcd)(int, int);

void* handle_gcd;
void* handle_integral;

void solve() {
    int number_of_function;
    scanf("%d", &number_of_function);

    if (number_of_function == 1) {
        float A, B, e;
        scanf("%f %f %f", &A, &B, &e);

        float result = (*integral)(A, B, e);
        printf("$\\int_A^B sin(x)dx \\approx %f$\n", result);
    } else {
        int A, B;
        scanf("%d %d", &A, &B);

        int result = (*gcd)(A, B);
        printf("GCD(%d, %d)=%d\n", A, B, result);
    }
}

void load_libraries() {
    int realization;
    scanf("%d", &realization);

    if (realization == 0) {
        handle_gcd = dlopen("./libraries/libgcd_naive.so", RTLD_LAZY);
        handle_integral = dlopen("./libraries/libintegral_rectangle.so", RTLD_LAZY);
    } else {
        handle_gcd = dlopen("./libraries/libgcd_euclid.so", RTLD_LAZY);
        handle_integral = dlopen("./libraries/libintegral_trapezoid.so", RTLD_LAZY);
    }

    if (!handle_gcd) {
        fprintf(stderr, "%s\n", dlerror());
        exit(1);
    }
    if (!handle_integral) {
        fprintf(stderr, "%s\n", dlerror());
        exit(1);
    }
    dlerror();

    integral = dlsym(handle_integral, "SinIntegral");
    gcd = dlsym(handle_gcd, "GCD");

    char* error = dlerror();
    if (error != NULL) {
        fprintf(stderr, "%s\n", error);
        exit(1);
    }
}

void close_libraries() {
    dlclose(handle_gcd);
    dlclose(handle_integral);
}

int main() {
    load_libraries();
    solve();
    close_libraries();
}