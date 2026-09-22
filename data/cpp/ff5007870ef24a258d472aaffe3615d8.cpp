// Write a C++ function named `fibonacciSeries` that takes a positive integer `n` (where 1 ≤ n ≤ 46) and returns a `std::vector<int>` containing the first `n` Fibonacci numbers in order, starting with 0 and 1. The function must compute the Fibonacci numbers using an iterative array-based approach (similar to the given snippet) but must be reusable and not print anything. Handle the case where `n == 1` correctly (return just `{0}`) and `n == 2` (return `{0, 1}`). The function should be `const`-correct and validate that `n` is within the allowed range; if `n` is invalid, return an empty vector.

The solution uses a fixed-size array (here `std::vector<int>` for dynamic sizing) to store intermediate Fibonacci values. The first two elements are initialized to 0 and 1. For each index `i` from 2 to `n-1`, we compute `fib[i] = fib[i-1] + fib[i-2]`, ensuring no overflow because the 46th Fibonacci number is 1836311903, which fits in a signed 32-bit int (max 2147483647). Edge cases: `n=1` must return `{0}` without accessing `fib[1]`; `n=2` returns `{0,1}`. Invalid `n` (0 or >46) returns an empty vector. Time complexity is O(n) because we do a single loop to fill the array. Space complexity is O(n) because of the storage for the result (plus constant extra). The function does not depend on console I/O or `conio.h`, making it portable and testable.

#include <vector>
#include <cstddef>

// Return the first n Fibonacci numbers (0-indexed start: F0=0, F1=1).
// n must be in [1, 46]; returns empty vector otherwise.
std::vector<int> fibonacciSeries(int n) {
    if (n < 1 || n > 46) {
        return {};
    }
    
    std::vector<int> fib(static_cast<std::size_t>(n));
    if (n >= 1) {
        fib[0] = 0;
    }
    if (n >= 2) {
        fib[1] = 1;
    }
    for (int i = 2; i < n; ++i) {
        fib[static_cast<std::size_t>(i)] = fib[static_cast<std::size_t>(i - 1)] + 
                                           fib[static_cast<std::size_t>(i - 2)];
    }
    return fib;
}

#include <cassert>
#include <vector>
#include <cstddef>

int main() {
    // Test n = 1, 2, and typical values.
    assert(fibonacciSeries(1) == std::vector<int>{0});
    assert(fibonacciSeries(2) == std::vector<int>{0, 1});
    assert(fibonacciSeries(5) == std::vector<int>{0, 1, 1, 2, 3});
    assert(fibonacciSeries(10) == std::vector<int>{0, 1, 1, 2, 3, 5, 8, 13, 21, 34});
    assert(fibonacciSeries(46).back() == 1836311903);
    
    // Invalid inputs return empty.
    assert(fibonacciSeries(0).empty());
    assert(fibonacciSeries(47).empty());
    assert(fibonacciSeries(-5).empty());
    
    return 0;
}
