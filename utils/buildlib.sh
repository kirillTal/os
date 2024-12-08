gcc -shared -fPIC include/gcd_euclid.c -o libraries/libgcd_euclid.so -lm
gcc -shared -fPIC include/gcd_naive.c -o libraries/libgcd_naive.so -lm
gcc -shared -fPIC include/integral_trapezoid.c -o libraries/libintegral_trapezoid.so -lm
gcc -shared -fPIC include/integral_rectangle.c -o libraries/libintegral_rectangle.so -lm