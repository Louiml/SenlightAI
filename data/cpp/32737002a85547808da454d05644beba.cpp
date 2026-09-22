Write a C++ function that, given a positive integer `n` representing a Fibonacci index (with `F(1)=1`, `F(2)=1`, and `F(k)=F(k-1)+F(k-2)` for `k≥3`), returns the `n`-th Fibonacci number. The function must handle indices up to 92 inclusive, where the value is small enough to fit in a `long long`. The function should be efficient and not recompute the sequence for every call; instead, it should precompute the Fibonacci numbers once and then return the requested value in constant time. The function should be robust to invalid indices by returning `0` for `n <= 0` and also `0` for `n > 92` (since beyond that, values overflow a 64-bit signed integer). You may assume the calling code will pass only integers.

// The Fibonacci sequence grows exponentially, and the 93rd Fibonacci number exceeds the maximum value of a signed 64-bit integer (`9,223,372,036,854,775,807`). Therefore, valid indices are `1` through `92`. The simplest approach is to precompute the first 92 Fibonacci numbers into a static array once, using a `std::once_flag` or a static local array initialized via a lambda, so that multiple calls do not recompute the array. The function then checks the input index: if it is outside `[1, 92]`, return `0`; otherwise, return the precomputed value at that index. Time complexity: O(1) per call after the first precomputation (which is O(92) ≈ O(1)). Space complexity: O(92) constant. Edge cases include `n=1`, `n=2`, `n=92`, and invalid inputs like `n=0`, negative values, or `n=93`.

#include <array>
#include <cstddef>

// Precomputes Fibonacci numbers up to index 92 and returns the n-th one.
// F(1)=1, F(2)=1, F(k)=F(k-1)+F(k-2). Returns 0 for invalid n outside [1,92].
long long fibonacciAtIndex(int n) {
    // Static array initialized once with the first 92 Fibonacci numbers.
    static const std::array<long long, 93> fib = []() {
        std::array<long long, 93> arr{};
        arr[1] = 1;
        arr[2] = 1;
        for (int i = 3; i <= 92; ++i) {
            arr[i] = arr[i - 1] + arr[i - 2];
        }
        return arr;
    }(); // Immediately-invoked lambda ensures single initialization.

    // Check valid range.
    if (n < 1 || n > 92) {
        return 0;
    }
    return fib[n];
}

#include <cassert>

int main() {
    // Basic known values.
    assert(fibonacciAtIndex(1) == 1);
    assert(fibonacciAtIndex(2) == 1);
    assert(fibonacciAtIndex(3) == 2);
    assert(fibonacciAtIndex(10) == 55);
    assert(fibonacciAtIndex(20) == 6765);
    // Maximum valid index.
    assert(fibonacciAtIndex(92) == 7540113804746346429LL);
    // Invalid indices return 0.
    assert(fibonacciAtIndex(0) == 0);
    assert(fibonacciAtIndex(-5) == 0);
    assert(fibonacciAtIndex(93) == 0);
    // Check that repeated calls are consistent.
    assert(fibonacciAtIndex(50) == fibonacciAtIndex(50));
    return 0;
}
