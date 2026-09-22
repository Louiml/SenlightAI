// Given two non-negative integers `n` and `b`, write a C++ function `bool isPossible(long long n, long long b)` that determines whether there exists a positive integer `k` such that the number of ways to choose `k` items from a set of size `n` (i.e., the binomial coefficient `C(n, k)`) is divisible by `2^b`. More precisely, `C(n, k)` must be divisible by `2^b` for **some** integer `k` with `0 ≤ k ≤ n`. The function should return `true` if such a `k` exists, otherwise `false`. The inputs satisfy `0 ≤ n ≤ 2^60` and `0 ≤ b ≤ 60`. Note: the divisibility condition requires that `2^b` divides `C(n, k)` exactly (i.e., the prime factorization of `C(n, k)` contains at least `b` factors of 2). If `b = 0`, then `2^0 = 1` divides every integer, so the answer is always `true` for any `n` (including `n = 0`, where the only binomial coefficient is `C(0,0)=1`). For `n = 0`, the only possible binomial coefficient is `1`, which is not divisible by any power of 2 greater than 1, so the answer is `false` for `b > 0`. The challenge is to derive an efficient condition using known properties of binomial coefficients and the largest power of 2 dividing them (Kummer’s theorem or Lucas’ theorem).

#include <cassert>

int main() {
    // b = 0 is always possible
    assert(canChoose(0, 0) == true);
    assert(canChoose(1, 0) == true);
    assert(canChoose(12345, 0) == true);

    // n = 0, b > 0 not possible
    assert(canChoose(0, 1) == false);
    assert(canChoose(0, 5) == false);

    // n all ones (like 3,7) only powers of 2 divide? Actually none for b>0
    assert(canChoose(3, 1) == false);
    assert(canChoose(7, 1) == false);
    assert(canChoose(7, 2) == false);

    // n=2: max exponent 1
    assert(canChoose(2, 1) == true);
    assert(canChoose(2, 2) == false);

    // n=4: max exponent 2
    assert(canChoose(4, 2) == true);
    assert(canChoose(4, 3) == false);

    // n=6: max exponent 2
    assert(canChoose(6, 2) == true);
    assert(canChoose(6, 3) == false);

    // n=10: max exponent 3 (C(10,3)=120)
    assert(canChoose(10, 3) == true);
    assert(canChoose(10, 4) == false);

    // n=12: max exponent 3 (C(12,5)=792)
    assert(canChoose(12, 3) == true);
    assert(canChoose(12, 4) == false);

    // n=14: max exponent 3 (C(14,7)=3432)
    assert(canChoose(14, 3) == true);
    assert(canChoose(14, 4) == false);

    return 0;
}

// Determine if there exists k such that C(n,k) is divisible by 2^b.
bool canChoose(long long n, int b) {
    if (b == 0) return true;
    if (n == 0) return false;

    // Compute the number of bits in n (bit length).
    int m = 0;
    long long tmp = n;
    while (tmp > 0) {
        ++m;
        tmp >>= 1;
    }

    // If n is of the form 2^m - 1 (all bits 1), no carries are possible.
    if (n == (1LL << m) - 1) return false;

    // Find the position of the least significant 0-bit.
    int z = 0;
    while ((n >> z) & 1LL) {
        ++z;
    }

    // Maximum number of carries = m - 1 - z.
    long long max_carries = m - 1 - z;
    return max_carries >= b;
}

// The key is Kummer’s theorem: the exponent of the largest power of 2 dividing `C(n,k)` equals the number of carries when `k` and `n-k` are added in binary. To maximize the number of carries for a fixed `n`, we should start a carry at the least significant 0-bit of `n` (if any) and then propagate that carry through all higher bits, stopping before the most significant 1-bit (because a carry out of the most significant position would make the sum exceed `n`). Let `m` be the number of bits in `n` (`m = floor(log2(n)) + 1`). Let `z` be the index of the least significant 0-bit (0-indexed from the least significant bit). If `n` has no 0-bit (i.e., `n = 2^m - 1`), then no carry can be generated, so the maximum exponent is 0. Otherwise, the maximum possible number of carries is `m - 1 - z`. Thus, an answer of `true` is obtained exactly when `b == 0`, or when `n > 0` and `m - 1 - z >= b`. Special case: if `n == 0` and `b > 0`, the only binomial coefficient is `C(0,0)=1`, which has no factor of 2, so the answer is `false`. The algorithm runs in O(log n) time because we only need to inspect the binary representation of `n`, and uses O(1) auxiliary space.
