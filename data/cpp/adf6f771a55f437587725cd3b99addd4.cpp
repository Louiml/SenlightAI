// Write a C++ function named `fibonacciNeighbors` that takes a single integer `n` (where `n >= 2`) and returns a `std::pair<unsigned long long, unsigned long long>` containing the `(n-2)`-th and `(n-1)`-th terms of the Fibonacci sequence (0-indexed: F(0)=1, F(1)=1, F(i)=F(i-1)+F(i-2)). The function must compute the sequence iteratively without recursion and return the two requested terms. Handle large `n` by using an unsigned 64-bit integer type, and ensure the function is `const`-correct (pass `n` by value). The function should not print anything; it must only return the pair. Assume `n >= 2` and that the result fits within `unsigned long long` for the given `n`.
// The Fibonacci sequence is defined with F(0)=1, F(1)=1, and for i>=2, F(i)=F(i-1)+F(i-2). To compute the n-th term iteratively, we initialize an array (or just two variables) with the base cases. Since the task asks for the (n-2)-th and (n-1)-th terms, we can compute the sequence up to index n-1. A simple approach is to use a fixed-size array of length `n` (or better, use dynamic allocation or just two rolling variables to save space). However, since `n` can be large, a vector or two-variable approach is safer. We'll use two rolling variables: `prev2` (F(i-2)), `prev1` (F(i-1)), and `current` (F(i)). Start with `prev2=1` (F0), `prev1=1` (F1). If n==2, then F(0) and F(1) are requested. For n>2, iterate from i=2 to n-1, updating `current = prev1 + prev2`, then shift `prev2 = prev1`, `prev1 = current`. After the loop, `prev2` holds F(n-2) and `prev1` holds F(n-1). Edge case: n=2 gives pair (1,1). Time complexity is O(n), space is O(1) if using rolling variables. Overflow is avoided by using `unsigned long long`. The function returns a `std::pair` where first is F(n-2) and second is F(n-1).
#include <cstdint>
#include <utility>

// Returns the (n-2)-th and (n-1)-th Fibonacci numbers (0-indexed, F(0)=1, F(1)=1).
// Requires n >= 2. Uses iterative computation with O(1) space.
std::pair<unsigned long long, unsigned long long> fibonacciNeighbors(const int n) {
    // Base cases: F(0) and F(1)
    unsigned long long prev2 = 1; // F(0)
    unsigned long long prev1 = 1; // F(1)

    // If n == 2, we return F(0) and F(1) directly.
    // Otherwise, compute up to F(n-1).
    for (int i = 2; i <= n - 1; ++i) {
        unsigned long long current = prev1 + prev2;
        prev2 = prev1;
        prev1 = current;
    }

    // prev2 now holds F(n-2), prev1 holds F(n-1)
    return {prev2, prev1};
}
#include <cassert>
#include <utility>

// Assume the solution function is defined above this main.
int main() {
    // n=2 -> F(0)=1, F(1)=1
    auto r1 = fibonacciNeighbors(2);
    assert(r1.first == 1 && r1.second == 1);

    // n=3 -> F(1)=1, F(2)=2
    auto r2 = fibonacciNeighbors(3);
    assert(r2.first == 1 && r2.second == 2);

    // n=4 -> F(2)=2, F(3)=3
    auto r3 = fibonacciNeighbors(4);
    assert(r3.first == 2 && r3.second == 3);

    // n=5 -> F(3)=3, F(4)=5
    auto r4 = fibonacciNeighbors(5);
    assert(r4.first == 3 && r4.second == 5);

    // n=6 -> F(4)=5, F(5)=8
    auto r5 = fibonacciNeighbors(6);
    assert(r5.first == 5 && r5.second == 8);

    // n=7 -> F(5)=8, F(6)=13
    auto r6 = fibonacciNeighbors(7);
    assert(r6.first == 8 && r6.second == 13);

    // n=10 -> F(8)=34, F(9)=55
    auto r7 = fibonacciNeighbors(10);
    assert(r7.first == 34 && r7.second == 55);

    // n=20 -> F(18)=4181, F(19)=6765
    auto r8 = fibonacciNeighbors(20);
    assert(r8.first == 4181 && r8.second == 6765);

    return 0;
}
