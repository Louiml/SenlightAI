Write a C++ function `fibonacciPair(int n)` that, given a non-negative integer `n` where `0 <= n <= 45`, returns a `std::pair<long long, long long>` containing the Fibonacci numbers `F(n-1)` and `F(n)` when `n >= 2`. For `n == 0`, the function returns `{0, 1}` (matching the snippet's output of "1 0" — note the snippet prints 1 0 for input 0, which corresponds to `F(-1)`? Actually the snippet treats 0 and 1 as special cases: for 0 it prints 1 and 0, for 1 it prints 0 and 1, and for `n >= 2` it prints `F(n-1)` and `F(n)`. The task: implement a function that returns a pair where the first element is `F(n-1)` and second is `F(n)`, with the convention `F(-1) = 1`, `F(0) = 0`, `F(1) = 1`. So for `n=0`, return `{1, 0}`; for `n=1`, return `{0, 1}`; for `n>=2`, return `{F(n-1), F(n)}` iteratively. The function must handle values up to `n=45` without overflow (Fibonacci(45) = 1,134,903,170, fits in `long long`). Test cases should verify the special cases and a few typical values.

#include <cassert>
#include <utility>

// Global main function for testing the solution.
int main() {
    // Special cases
    assert(fibonacciPair(0) == std::make_pair(1LL, 0LL));
    assert(fibonacciPair(1) == std::make_pair(0LL, 1LL));

    // Regular cases
    assert(fibonacciPair(2) == std::make_pair(1LL, 1LL));   // F(1)=1, F(2)=1
    assert(fibonacciPair(3) == std::make_pair(1LL, 2LL));   // F(2)=1, F(3)=2
    assert(fibonacciPair(4) == std::make_pair(2LL, 3LL));   // F(3)=2, F(4)=3
    assert(fibonacciPair(5) == std::make_pair(3LL, 5LL));   // F(4)=3, F(5)=5
    assert(fibonacciPair(10) == std::make_pair(34LL, 55LL)); // F(9)=34, F(10)=55
    assert(fibonacciPair(45) == std::make_pair(701408733LL, 1134903170LL)); // F(44), F(45)

    return 0;
}

#include <utility>   // for std::pair
#include <cstddef>   // for std::size_t (optional)

// Return a pair {F(n-1), F(n)} following the convention:
// F(-1)=1, F(0)=0, F(1)=1.
// For n=0, returns {1,0}; for n=1, returns {0,1}; for n>=2, returns {F(n-1), F(n)}.
std::pair<long long, long long> fibonacciPair(int n) {
    if (n == 0) {
        return {1, 0};   // special case as per the snippet's output
    }
    if (n == 1) {
        return {0, 1};   // F(0) and F(1)
    }

    long long a = 0;   // F(0)
    long long b = 1;   // F(1)

    for (int i = 2; i <= n; ++i) {
        long long next = a + b;
        a = b;
        b = next;
    }

    // After the loop, a = F(n-1), b = F(n)
    return {a, b};
}

// The problem is a direct mapping from the provided snippet, which computes Fibonacci numbers iteratively. The core algorithm: for `n == 0`, return `{1, 0}`; for `n == 1`, return `{0, 1}`; otherwise, initialize `a = 0` (representing `F(0)`) and `b = 1` (representing `F(1)`). Then loop from `i = 2` to `n`, computing `next = a + b`, setting `a = b`, `b = next`. After the loop, `a` holds `F(n-1)` and `b` holds `F(n)`. Return `{a, b}`. Edge cases: only `n >= 0` is valid; negative inputs are undefined but we can ignore or assert. The special cases for 0 and 1 are handled explicitly because the general loop would not work for `n < 2`. Time complexity is `O(n)` and space complexity is `O(1)`. Since `n` is at most 45, the loop runs at most 44 times, which is trivial. Use `long long` to avoid overflow for `n=45`.
