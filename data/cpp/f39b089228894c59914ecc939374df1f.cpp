Write a C++ function `long long sumNaturalRange(long long first, long long last)` that receives two positive integers `first` and `last` (with `first <= last`) and returns the sum of all natural numbers from `first` through `last` inclusive. The function must use the arithmetic series formula to compute the result directly, without any loops. The input values can be as large as 10^12, so ensure the function uses appropriate 64-bit integer types to avoid overflow during intermediate arithmetic. The result is guaranteed to fit within a 64-bit signed integer.
The problem reduces to computing the sum of an arithmetic progression with a common difference of 1. The number of terms is `N = last - first + 1`. The sum of an arithmetic series is given by `(first + last) * N / 2`. Since multiplication may overflow even when the final result fits in a 64-bit integer, one must be careful: when `first` and `last` are both large, `(first + last)` can exceed 2^63. However, the standard trick is to compute the multiplication in a way that avoids overflow by dividing one of the factors by 2 first when either is even, or alternatively cast to `long double` for intermediate computation and then truncate, but a safer integer-only method is to conditionally divide. Since we know the product is always even (because either `(first + last)` or `N` is even), we can safely divide one of them by 2 before multiplying. Edge cases: when `first == last`, the sum is simply that number. Negative inputs are not expected per specification. Time complexity is O(1) and space complexity is O(1).
#include <cstdint>

// Return the sum of all natural numbers from `first` to `last` inclusive.
// Uses the arithmetic series formula. Precondition: first <= last.
long long sumNaturalRange(long long first, long long last) {
    long long n = last - first + 1;               // number of terms
    long long sumPair = first + last;             // sum of first and last term
    long long product;

    // To avoid overflow, divide one of the two factors by 2 before multiplying.
    if (sumPair % 2 == 0) {
        product = (sumPair / 2) * n;
    } else {
        product = sumPair * (n / 2);
    }

    return product;
}
#include <cassert>

int main() {
    // Basic cases
    assert(sumNaturalRange(1, 1) == 1);
    assert(sumNaturalRange(1, 5) == 15);
    assert(sumNaturalRange(5, 5) == 5);
    assert(sumNaturalRange(1, 10) == 55);

    // Larger ranges
    assert(sumNaturalRange(10, 20) == 165);
    assert(sumNaturalRange(100, 200) == 15050);

    // Edge case: full 64-bit range but result fits (e.g., 1 to 1e9)
    assert(sumNaturalRange(1, 1000000000LL) == 500000000500000000LL);

    // Odd number of terms and large end
    assert(sumNaturalRange(1000000000000LL, 1000000000002LL) == 3000000000003LL);

    // Very large range (1 to 1e12) – sum fits in signed 64-bit (5e23 > 9.22e18? No! choose below)
    // Use 1 to 4e9: sum = (1+4e9)*4e9/2 = 8e18 < 9.22e18, safe
    assert(sumNaturalRange(1, 4000000000LL) == 8000000002000000000LL);

    // Inverse check with a different known sum
    assert(sumNaturalRange(7, 9) == 24);
}
