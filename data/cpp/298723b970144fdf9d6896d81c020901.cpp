/*
Write a C++ function that computes the first `n` terms of a Fibonacci-like sequence where each term is taken modulo `k`. The sequence starts with `0` and `1` as the first two terms (for `n >= 1` and `n >= 2` respectively). For all terms from position 3 to `n`, each term is the sum of the two previous terms, reduced modulo `k`. The function should take two non-negative integers `n` and `k` (with `k > 0`), and return a `std::vector<int>` containing the first `n` terms of this modular Fibonacci sequence. If `n` is 0, return an empty vector. If `n` is 1, return `{0}`; if `n` is 2, return `{0, 1}`. The function must handle large values of `n` (up to 10^9) and `k` (up to 10^5) efficiently, and must avoid integer overflow by applying modulo at each step. The vector should contain the sequence in order, each term as an `int` (since the modulo result fits in `int` when `k <= 10^5`).
*/
#include <vector>

// Compute the first n terms of the Fibonacci sequence modulo k.
// Returns empty vector if n == 0.
std::vector<int> modularFibonacci(int n, long long k) {
    std::vector<int> result;
    if (n <= 0) return result;

    if (n >= 1) result.push_back(0);
    if (n >= 2) result.push_back(1);

    long long f1 = 0, f2 = 1;
    for (int i = 3; i <= n; ++i) {
        long long f3 = (f1 + f2) % k;
        result.push_back(static_cast<int>(f3));
        f1 = f2;
        f2 = f3;
    }
    return result;
}
#include <cassert>
#include <vector>

// Function declaration (from solution above)
std::vector<int> modularFibonacci(int n, long long k);

int main() {
    // Test n=0
    assert(modularFibonacci(0, 100).empty());

    // Test n=1
    std::vector<int> v1 = {0};
    assert(modularFibonacci(1, 100) == v1);

    // Test n=2
    std::vector<int> v2 = {0, 1};
    assert(modularFibonacci(2, 100) == v2);

    // Test n=10, k=100 -> regular Fibonacci mod 100
    std::vector<int> expected = {0, 1, 1, 2, 3, 5, 8, 13, 21, 34};
    assert(modularFibonacci(10, 100) == expected);

    // Test n=10, k=1 -> all zeros
    std::vector<int> zeros(10, 0);
    assert(modularFibonacci(10, 1) == zeros);

    // Test n=6, k=2 -> sequence mod 2: 0,1,1,0,1,1
    std::vector<int> mod2 = {0, 1, 1, 0, 1, 1};
    assert(modularFibonacci(6, 2) == mod2);

    // Test n=5, k=3 -> sequence: 0,1,1,2,0
    std::vector<int> mod3 = {0, 1, 1, 2, 0};
    assert(modularFibonacci(5, 3) == mod3);
}
// The problem is a straightforward modular Fibonacci sequence generation. The main algorithm iterates from term 3 up to `n`, maintaining the previous two terms (`prev1` and `prev2`), and computing the next term as `(prev1 + prev2) % k`. Each computed term is pushed to the result vector. Important edge cases: `n = 0` returns an empty vector; `n = 1` returns only `[0]`; `n = 2` returns `[0, 1]`. Also, `k` is guaranteed positive, but if `k` is 1, all terms are 0 (since modulo 1 always yields 0). The time complexity is O(n) because we generate each term once; however, if `n` is very large (up to 10^9), this is not feasible in practice. But the task only asks to generate the sequence, so we assume `n` is small enough to store in a vector (e.g., up to a few million). The space complexity is O(n) for the vector that stores the sequence. Alternatively, one could write a function that prints the sequence on the fly, but here we return a vector. For very large `n`, the problem would be better solved using matrix exponentiation to find the nth term, but the task requires generating all terms, so O(n) is acceptable for typical test sizes.
