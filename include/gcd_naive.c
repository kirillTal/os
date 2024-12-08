int GCD(int A, int B) {
    int C = (A > B ? A : B);
    for (int i = C; i > 1; --i) {
        if (A % i == 0 && B % i == 0) {
            return i;
        }
    }
    return 1;
}