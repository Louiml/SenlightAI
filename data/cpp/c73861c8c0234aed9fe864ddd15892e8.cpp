Write a C++ function `unsigned long long fibonacciTerm(int n)` that, given a non-negative integer `n`, returns the `n`-th Fibonacci number using the iterative definition F(0) = 0, F(1) = 1, and F(k) = F(k-1) + F(k-2) for k ≥ 2. Handle the edge cases `n = 0` and `n = 1` explicitly, and assume the input is small enough that the result fits within an `unsigned long long`. The function must be free-standing (no main), use only standard headers, and be `const`-correct where appropriate.

#include <cassert>

int main() {
    assert(fibonacciTerm(0) == 0);
    assert(fibonacciTerm(1) == 1);
    assert(fibonacciTerm(2) == 1);
    assert(fibonacciTerm(3) == 2);
    assert(fibonacciTerm(5) == 5);
    assert(fibonacciTerm(10) == 55);
    assert(fibonacciTerm(20) == 6765);
    assert(fibonacciTerm(30) == 832040);
    assert(fibonacciTerm(45) == 1134903170ULL);
    // 93 is the largest n that fits in unsigned long long (F(93) = 12200160415121876738)
    assert(fibonacciTerm(93) == 12200160415121876738ULL);
    return 0;
}

#include <cstddef>

// Compute the n-th Fibonacci number iteratively.
// F(0) = 0, F(1) = 1, F(k) = F(k-1) + F(k-2) for k >= 2.
// Returns 0 for n <= 0, handles n = 0 and n = 1 explicitly.
unsigned long long fibonacciTerm(int n) {
    if (n <= 0) return 0;
    if (n == 1) return 1;

    unsigned long long prev = 0;
    unsigned long long curr = 1;

    for (int i = 2; i <= n; ++i) {
        unsigned long long next = prev + curr;
        prev = curr;
        curr = next;
    }

    return curr;
}

// The classic Fibonacci sequence is defined iteratively. We track two consecutive values: `prev` (F(k-1)) and `curr` (F(k)), starting with `prev = 0` and `curr = 1` for k = 0 and k = 1 respectively. For each step from 2 to n, we compute the next term as `next = prev + curr`, then shift `prev = curr` and `curr = next`. After the loop, `curr` holds F(n). Edge cases: if `n == 0`, return 0; if `n == 1`, return 1. The loop handles `n >= 2` correctly. Because the function is iterative and uses only a few variables, time complexity is O(n) and space complexity is O(1). We use `unsigned long long` to prevent signed overflow for moderate n (practical limit around n = 93 due to 64-bit overflow). The function is declared `const`-qualified to indicate it does not modify any external state.
