/*
Write a C++ function `gf2n_multiply` that performs multiplication of two polynomials over GF(2) with coefficients stored as 64-bit unsigned integers, where each bit position \(i\) represents the coefficient of \(x^i\). The reduction is performed modulo a trinomial \(x^n + x^{t1} + 1\), where \(n\), \(t1\), and the field mask are provided as parameters. The function must return the reduced 64-bit product. The inputs are guaranteed to be already reduced (i.e., the highest bit set is at most \(n-1\)). Assume \(n \le 64\), and that \(2 \cdot (n-1) - 64 + t1 < 64\) so that no overflow occurs during intermediate multiplication before reduction. The multiplication should split the 64-bit inputs into two 32-bit halves, multiply them using a simple shift-and-XOR method (no Karatsuba needed), then perform reduction using the given trinomial parameters by first folding the high 32 bits into the low part, then iteratively reducing the top overflow bits.
*/
#include <cstdint>

// Multiply two GF(2) polynomials modulo x^n + x^t1 + 1.
// Inputs a and b are already reduced (bits < n).
// n, t1 must satisfy: 2*(n-1) - 64 + t1 < 64.
uint64_t gf2n_multiply(uint64_t a, uint64_t b, int n, int t1) {
    // Helper to multiply two 32-bit polynomials, returning a 64-bit result.
    auto mul32 = [](uint32_t x, uint32_t y) -> uint64_t {
        uint64_t result = 0;
        for (int i = 0; i < 32; ++i) {
            if ((x >> i) & 1) {
                result ^= (static_cast<uint64_t>(y) << i);
            }
        }
        return result;
    };

    // Split a and b into 32-bit halves.
    uint64_t a_lo = a & 0xFFFFFFFFULL;
    uint64_t a_hi = a >> 32;
    uint64_t b_lo = b & 0xFFFFFFFFULL;
    uint64_t b_hi = b >> 32;

    // Compute the 128-bit product as two 64-bit words.
    uint64_t p00 = mul32(static_cast<uint32_t>(a_lo), static_cast<uint32_t>(b_lo));
    uint64_t p01 = mul32(static_cast<uint32_t>(a_lo), static_cast<uint32_t>(b_hi));
    uint64_t p10 = mul32(static_cast<uint32_t>(a_hi), static_cast<uint32_t>(b_lo));
    uint64_t p11 = mul32(static_cast<uint32_t>(a_hi), static_cast<uint32_t>(b_hi));

    // Combine: lo = p00 ^ (p01 << 32) ^ (p10 << 32)
    //         hi = p11 ^ (p01 >> 32) ^ (p10 >> 32)
    uint64_t lo = p00;
    uint64_t hi = p11;

    lo ^= (p01 << 32);
    hi ^= (p01 >> 32);
    lo ^= (p10 << 32);
    hi ^= (p10 >> 32);

    // Now reduce the 128-bit product (hi:lo) modulo x^n + x^t1 + 1.
    // Since x^n = x^t1 + 1, we have x^(n+i) = x^(t1+i) + x^i.
    // First fold the high 64 bits into the low 64 bits.
    for (int i = 0; i < 64; ++i) {
        if ((hi >> i) & 1) {
            // XOR at position i (i < 64 always, but only significant if i < n)
            if (i < 64) lo ^= (1ULL << i);
            // XOR at position t1 + i if less than 64
            int pos2 = t1 + i;
            if (pos2 < 64) lo ^= (1ULL << pos2);
        }
    }

    // Now reduce any remaining bits in lo above n-1.
    while (true) {
        // Find the highest set bit.
        if (lo == 0 || (n >= 64)) break;
        int top = 63 - __builtin_clzll(lo);
        if (top < n) break;

        // Reduce bit at position top: x^top = x^(top-n) * (x^t1 + 1)
        // = x^(top-n+t1) + x^(top-n)
        lo ^= (1ULL << top);
        int pos1 = top - n;
        if (pos1 < 64) lo ^= (1ULL << pos1);
        int pos2 = top - n + t1;
        if (pos2 < 64) lo ^= (1ULL << pos2);
    }

    return lo;
}
#include <cassert>
#include <cstdint>
#include <iostream>

// Declaration of the function to test (assume it's defined in the same file).
uint64_t gf2n_multiply(uint64_t a, uint64_t b, int n, int t1);

int main() {
    // Field GF(2^4) with polynomial x^4 + x + 1 (n=4, t1=1)
    // Manual checks: (x^3 + 1) * (x^2 + x) = x^5 + x^4 + x^3 + x
    // Reduce: x^4 = x+1, x^5 = x^2 + x, so result = x^2+x + x+1 + x^3+x = x^3 + x^2 + x + 1
    uint64_t a1 = 0b1001; // x^3 + 1
    uint64_t b1 = 0b0110; // x^2 + x
    assert(gf2n_multiply(a1, b1, 4, 1) == 0b1111);

    // Test identity: 1 * any = any
    assert(gf2n_multiply(1, 0b1011, 4, 1) == 0b1011);

    // Test zero: 0 * any = 0
    assert(gf2n_multiply(0, 0b1011, 4, 1) == 0);

    // Test with larger values: Field GF(2^8) with polynomial x^8 + x^4 + x^3 + x + 1 is not trinomial,
    // so use a trinomial: x^8 + x^4 + 1 (n=8, t1=4) actually that's a binomial, but okay.
    // Use x^8 + x^3 + 1? Actually that's trinomial. Test: x^7 * x = x^8 = x^3 + 1 (mod x^8+x^3+1)
    uint64_t a2 = (1ULL << 7); // x^7
    uint64_t b2 = (1ULL << 1); // x^1
    // x^7 * x = x^8 = x^3 + 1
    assert(gf2n_multiply(a2, b2, 8, 3) == ((1ULL << 3) | 1ULL));

    // Test square: (x^4 + 1)^2 = x^8 + 1 = x^3 + 1 + 1 = x^3 (mod x^8+x^3+1)
    uint64_t a3 = (1ULL << 4) | 1; // x^4 + 1
    assert(gf2n_multiply(a3, a3, 8, 3) == (1ULL << 3)); // x^3

    // Test a larger product: x^6 * x^6 = x^12 = x^4 * x^8 = x^4 * (x^3+1) = x^7 + x^4
    uint64_t a4 = (1ULL << 6);
    assert(gf2n_multiply(a4, a4, 8, 3) == ((1ULL << 7) | (1ULL << 4)));

    // Test that result is bounded by mask (all bits < n)
    uint64_t result = gf2n_multiply(0xDEADBEEF, 0xCAFEBABE, 16, 1); // n=16, t1=1 (x^16+x+1)
    assert((result >> 16) == 0);

    std::cout << "All tests passed." << std::endl;
    return 0;
}
// The core challenge is performing carry-less multiplication (GF(2) polynomial multiplication) and then reducing modulo a trinomial. Since the inputs are already reduced to \(n\) bits, their product is at most \(2n-2\) bits, which fits in a 128-bit result when using two 64-bit words. A straightforward multiplication approach is to use a 64-bit accumulator loop: for each set bit in one operand, shift the other operand left by that bit position and XOR into the result. However, to keep the implementation clean and efficient, split each 64-bit input into two 32-bit halves, multiply the halves using a nested loop over bits (since 32 bits is small), and combine the four partial products. Specifically, for a 32-bit multiplication, iterate over each bit of one 32-bit number, and if set, XOR the other shifted by that bit into a 64-bit accumulator. This yields a 64-bit result for a 32×32 multiplication. Combining the halves: let `a = a_hi << 32 ^ a_lo` and `b = b_hi << 32 ^ b_lo`. The full 128-bit product is `(a_lo * b_lo) + ((a_lo * b_hi) << 32) + ((a_hi * b_lo) << 32) + ((a_hi * b_hi) << 64)`, where all operations are XOR and shifts. Since the original inputs are at most \(n\) bits, and \(n \le 64\), the product may require up to 128 bits; we store it as two 64-bit words `hi` and `lo`. After computing the raw 128-bit product, we reduce modulo \(x^n + x^{t1} + 1\). First, fold the high 64 bits into the low 64 bits using the relations: for each bit position \(i\) in the high word (from \(i=0\) to \(i=63\)), the bit corresponds to \(x^{n+i}\) in the product. Since \(x^n = x^{t1} + 1\), we have \(x^{n+i} = x^{t1+i} + x^i\). Thus XOR the high word into the low word at positions \(i\) and \(t1+i\), but only when those positions are less than 64. After folding the high word, there may still be overflow beyond bit \(n-1\) in the low word (since the folded value could have bits up to \(n-1 + \text{shift}\) up to close to 64). Then iteratively, while the value exceeds \(n\) bits (i.e., has any bit set at position \(\ge n\)), repeatedly reduce: for each bit position \(j\) (from \(n\) to the top), replace \(x^j\) with \(x^{j-n} (x^{t1} + 1) = x^{t1 + j - n} + x^{j-n}\). So clear the bit at \(j\), and XOR at positions \(j-n\) and \(t1+j-n\), as long as those positions are within 0..63. After this loop, the result is reduced modulo the trinomial. The time complexity is \(O(32^2)\) for each 32-bit multiplication, so constant time (about 1024 bit operations per multiplication), plus a small reduction loop (at most a few iterations). Space complexity is O(1).
