// Write a C++ function named `buildFibonacciSequence` that takes a non-negative integer `n` representing the desired length of the sequence and returns a `std::vector<int>` containing the first `n` Fibonacci numbers in order (starting with 0 and 1). The function must handle the edge cases where `n` is 0 (return an empty vector) and `n` is 1 (return a vector containing only `{0}`). Additionally, write a separate recursive function `fibonacciAtIndex` that takes a non-negative integer `k` and returns the `k`-th Fibonacci number (0-indexed) using the naive recursive approach. Both functions should be declared with proper `const` correctness (e.g., parameters passed as `const` references where appropriate) and should not modify any global state. The solution must avoid using `new[]` or raw arrays; use `std::vector` for dynamic storage.

// The iterative sequence builder uses a simple dynamic programming approach: create a vector of size `n`, initialize the first two elements (if `n >= 1` and `n >= 2`) and then fill the remaining positions using the recurrence `F[i] = F[i-1] + F[i-2]`. Edge cases: `n == 0` returns an empty vector, `n == 1` returns `{0}`, and `n == 2` returns `{0, 1}`. Complexity: O(n) time and O(n) space for the vector. The recursive function follows the classic definition: base cases `k == 0` returns 0, `k == 1` returns 1, otherwise returns `fibonacciAtIndex(k-1) + fibonacciAtIndex(k-2)`. Its time complexity is exponential O(2^k) due to overlapping subproblems, and space complexity O(k) for the recursion stack. Important edge cases include handling `k == 0` and `k == 1` correctly, and ensuring the function does not accept negative indices (but the problem states non-negative input, so no negative checks needed). The iterative version avoids stack overflow for large `n` and is far more efficient.

#include <vector>

// Return the k-th Fibonacci number (0-indexed) using naive recursion.
int fibonacciAtIndex(const int k) {
    if (k == 0) return 0;
    if (k == 1) return 1;
    return fibonacciAtIndex(k - 1) + fibonacciAtIndex(k - 2);
}

// Build a vector containing the first n Fibonacci numbers (0, 1, 1, 2, ...).
std::vector<int> buildFibonacciSequence(const int n) {
    std::vector<int> sequence;
    if (n <= 0) return sequence;
    sequence.reserve(n);
    sequence.push_back(0);
    if (n == 1) return sequence;
    sequence.push_back(1);
    for (int i = 2; i < n; ++i) {
        sequence.push_back(sequence[i - 1] + sequence[i - 2]);
    }
    return sequence;
}

#include <cassert>
#include <vector>

// Declare the functions being tested (match the solution exactly).
int fibonacciAtIndex(const int k);
std::vector<int> buildFibonacciSequence(const int n);

int main() {
    // Tests for buildFibonacciSequence
    assert(buildFibonacciSequence(0).empty());
    assert((buildFibonacciSequence(1) == std::vector<int>{0}));
    assert((buildFibonacciSequence(2) == std::vector<int>{0, 1}));
    assert((buildFibonacciSequence(5) == std::vector<int>{0, 1, 1, 2, 3}));
    assert((buildFibonacciSequence(8) == std::vector<int>{0, 1, 1, 2, 3, 5, 8, 13}));

    // Tests for fibonacciAtIndex
    assert(fibonacciAtIndex(0) == 0);
    assert(fibonacciAtIndex(1) == 1);
    assert(fibonacciAtIndex(2) == 1);
    assert(fibonacciAtIndex(5) == 5);
    assert(fibonacciAtIndex(10) == 55);
}
