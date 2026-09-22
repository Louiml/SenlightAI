/*
Write a C++ function `arithmeticSeriesInfo` that takes three integers: `n` (the number of terms, guaranteed positive), `a` (the first term), and `d` (the common difference), and returns a `std::pair<int, int>` where the first element is the `n`-th term of the arithmetic sequence and the second element is the sum of the first `n` terms. The function must not print anything; it must only compute and return the pair. Handle all integer values (including negatives, zero, and large values that might overflow `int` — you may use `long long` internally for safety, but the return type should be `std::pair<long long, long long>` for correctness). The task does not involve user input; the caller will supply the parameters.
*/
#include <utility> // for std::pair

// Returns {n-th term, sum of first n terms} of an arithmetic sequence.
// n: number of terms (>=1), a: first term, d: common difference.
std::pair<long long, long long> arithmeticSeriesInfo(int n, int a, int d) {
    // Cast to long long to avoid overflow during intermediate calculations.
    long long first = a;
    long long diff = d;
    long long terms = n;

    long long nthTerm = first + (terms - 1) * diff;                    // term at index n-1
    long long sum = terms * (2 * first + (terms - 1) * diff) / 2;      // closed-form sum

    return {nthTerm, sum};
}
#include <cassert>
#include <utility>

// The solution function is declared above, but for testing we include it directly or via header.
// Here we assume it's declared; we'll insert the actual function before main in a real test.

int main() {
    // Basic positive difference
    auto r1 = arithmeticSeriesInfo(5, 2, 3);
    assert(r1.first == 14);   // 2,5,8,11,14 -> 5th term=14
    assert(r1.second == 40);  // sum = 2+5+8+11+14 = 40

    // Negative difference
    auto r2 = arithmeticSeriesInfo(4, 10, -2);
    assert(r2.first == 4);    // 10,8,6,4 -> 4th term=4
    assert(r2.second == 28);  // 10+8+6+4 = 28

    // Single term
    auto r3 = arithmeticSeriesInfo(1, 7, 100);
    assert(r3.first == 7);
    assert(r3.second == 7);

    // Zero difference
    auto r4 = arithmeticSeriesInfo(3, 5, 0);
    assert(r4.first == 5);
    assert(r4.second == 15);

    // Large values to test overflow safety
    auto r5 = arithmeticSeriesInfo(1000000, 1000000, 1000000);
    // first term a = 1,000,000; d=1,000,000; n=1,000,000
    // n-th term = 1,000,000 + (999,999)*1,000,000 = 1,000,000,000,000? Actually 999,999*1,000,000 = 999,999,000,000 + 1,000,000 = 1,000,000,000,000 (1e12? No, 1e12 is 1,000,000,000,000). So first = 1000000 + (999999)*1000000 = 1,000,000,000,000? 999,999*1,000,000 = 999,999,000,000, plus 1,000,000 = 1,000,000,000,000? That's 1e12, fits in long long (max ~9e18).
    assert(r5.first == 1000000000000LL);
    // Sum = n * (first + last)/2 = 1,000,000 * (1,000,000 + 1,000,000,000,000)/2 = 1,000,000 * 1,000,001,000,000 /2 = 500,000,500,000,000,000? Compute: 1,000,001,000,000 * 1,000,000 = 1,000,001,000,000,000,000; divided by 2 = 500,000,500,000,000,000. Fits in long long.
    assert(r5.second == 500000500000000000LL);

    // Negative n-th term (n=3, a=-5, d=2) -> -5,-3,-1 -> 3rd term=-1, sum=-9
    auto r6 = arithmeticSeriesInfo(3, -5, 2);
    assert(r6.first == -1);
    assert(r6.second == -9);

    return 0;
}
// The arithmetic sequence is defined as: term i (0-indexed) = a + i*d. The n-th term (1-indexed, i.e., the term at position n, meaning index n-1) is computed as `a + (n-1)*d`. The sum of the first n terms is the arithmetic series sum, given by `n * (first + last) / 2` or equivalently the iterative accumulation — but iterating is O(n) and could be slow for large n, so we should use the closed-form formula. However, to avoid overflow and integer division issues, it is best to compute the sum as `(long long)n * (2*a + (long long)(n-1)*d) / 2` — this works even when the product is odd? Actually, `n * (2a + (n-1)d)` is always even because either n is even or the second factor is even? Let's verify: if n is odd, then (n-1) is even, so (n-1)*d is even, and 2a is even, so the sum of those two is even, so the product is even*odd = even. If n is even, n is even, product even. So integer division is exact. For the n-th term, compute as `(long long)a + (long long)(n-1)*d`. Edge cases: n=1 gives first term and sum = first term. Negative d works fine. Large values may exceed 32-bit int, so use `long long` for all arithmetic internally. Time complexity O(1), space O(1). The main potential pitfall is integer overflow in intermediate multiplication if using int; that’s why we cast to `long long`. Also, ensure the formula for sum uses parentheses to avoid operator precedence mistakes.
