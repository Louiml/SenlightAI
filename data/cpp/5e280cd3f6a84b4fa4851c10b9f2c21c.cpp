// Write a C++ function `fibonacciValue(int n)` that returns the n-th Fibonacci number (0-indexed) as a `long long int`, handling n from 0 up to 49. The function should compute the sequence iteratively by precomputing an array, starting with `fib[0] = fib[1] = 1`, and for each subsequent index `i`, setting `fib[i] = fib[i-1] + fib[i-2]`. The function must be self-contained, use a statically sized accumulator for efficiency, and handle the base cases (n=0 and n=1) correctly even though they are also covered by the loop. The maximum Fibonacci value fits within a 64-bit signed integer (fib[49] = 7778742049), so no overflow occurs.
The solution precomputes all Fibonacci numbers from 0 to 49 into a `static` local array inside the function so that repeated calls do not recompute the sequence. The array is initialized once on the first call. The loop starts from index 2 and fills sequentially. Since the input `n` is guaranteed to be in `[0, 49]`, we directly return `fib[n]` after ensuring the array is populated. Edge cases: `n=0` and `n=1` both return 1 (this matches the snippet, which sets `nlist[0]=nlist[1]=1`, i.e., a shifted Fibonacci starting with two 1s). No special handling for negative or out-of-range values is required per the task spec. Time complexity is O(50) for the first call and O(1) for subsequent calls; space complexity is O(1) fixed-size array.
#include <cstdint>

// Return the n-th Fibonacci number (0-indexed) with fib[0]=fib[1]=1.
// n must be between 0 and 49 inclusive.
long long int fibonacciValue(int n) {
    static long long int fib[50]; // zero-initialized by default
    static bool initialized = false;
    if (!initialized) {
        fib[0] = 1;
        fib[1] = 1;
        for (int i = 2; i < 50; ++i) {
            fib[i] = fib[i - 1] + fib[i - 2];
        }
        initialized = true;
    }
    return fib[n];
}
#include <cassert>

int main() {
    assert(fibonacciValue(0) == 1);
    assert(fibonacciValue(1) == 1);
    assert(fibonacciValue(2) == 2);
    assert(fibonacciValue(3) == 3);
    assert(fibonacciValue(4) == 5);
    assert(fibonacciValue(10) == 89);
    assert(fibonacciValue(20) == 10946);
    assert(fibonacciValue(49) == 7778742049LL);
    assert(fibonacciValue(7) == 21);
    assert(fibonacciValue(15) == 987);
}
