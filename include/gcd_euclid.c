int GCD(int A, int B) {
    while(A && B) {
        if (A > B) {
            A %= B;
        } else {
            B %= A;
        }
    }

    return (A > B ? A : B);
}