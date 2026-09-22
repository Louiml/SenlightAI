Write a C++ function that, given a non-negative integer `n` (where `0 <= n <= 60`), returns the `n`-th Fibonacci number as a `long long int`. The function must compute Fibonacci numbers iteratively using an array precomputed up to index 60, and handle the base cases `F(0)=0` and `F(1)=1` correctly. The function should be reusable and not depend on any global mutable state; it must return the Fibonacci value directly.
// The Fibonacci sequence is defined as `F(0)=0`, `F(1)=1`, and for `i >= 2`, `F(i) = F(i-1) + F(i-2)`. Since the maximum `n` is 60, the maximum Fibonacci number fits within a 64-bit signed integer (`F(60) ≈ 1.548e12`), so `long long int` is safe. The simplest and most efficient approach is to precompute all Fibonacci numbers from 0 to 60 into a local static array (or a `std::array`) once, then return the element at index `n`. Because the array is computed only once per program execution (if declared `static`), subsequent calls are O(1). The precomputation itself runs in O(60) time, which is effectively constant, and uses O(1) extra space relative to the input size. Edge cases: `n=0` returns 0, `n=1` returns 1, and any `n` in range is handled without overflow. If `n` were outside 0..60, the function could throw or assert, but since the task specifies constraints, we can simply assume valid input.
#include <array>
#include <cstddef>

// Return the n-th Fibonacci number for 0 <= n <= 60.
// Uses a static precomputed table to achieve O(1) lookup after first call.
long long int fibonacci(int n) {
    static const std::array<long long int, 61> fib = [] {
        std::array<long long int, 61> arr{};
        arr[0] = 0;
        arr[1] = 1;
        for (std::size_t i = 2; i < arr.size(); ++i) {
            arr[i] = arr[i - 1] + arr[i - 2];
        }
        return arr;
    }();
    return fib[n];
}
#include <cassert>

int main() {
    assert(fibonacci(0) == 0);
    assert(fibonacci(1) == 1);
    assert(fibonacci(2) == 1);
    assert(fibonacci(10) == 55);
    assert(fibonacci(20) == 6765);
    assert(fibonacci(30) == 832040);
    assert(fibonacci(40) == 102334155);
    assert(fibonacci(50) == 12586269025LL);
    assert(fibonacci(60) == 1548008755920LL);
    return 0;
}
