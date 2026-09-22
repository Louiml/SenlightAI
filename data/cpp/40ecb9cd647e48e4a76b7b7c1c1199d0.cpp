// Write a C++ function `long long nthFibonacci(int n)` that computes the n-th Fibonacci number (F₀ = 0, F₁ = 1) using **bottom-up dynamic programming**, i.e., iteratively building a table of results from base cases up to `n`. The function must handle `n = 0` and `n = 1` correctly without special-case early returns (only loop-based logic), must avoid recursion (which could overflow the stack for large `n`), and must use `long long` to accommodate results for `n` up to 90 (where F₉₀ ≈ 2.88e18, close to the `long long` limit). The function should be self-contained, include necessary headers, and be efficient for any `n` from 0 to 90 inclusive.
int main() {
    // Base cases
    assert(nthFibonacci(0) == 0);
    assert(nthFibonacci(1) == 1);
    // Small numbers
    assert(nthFibonacci(2) == 1);
    assert(nthFibonacci(3) == 2);
    assert(nthFibonacci(4) == 3);
    assert(nthFibonacci(5) == 5);
    assert(nthFibonacci(6) == 8);
    assert(nthFibonacci(10) == 55);
    // Known larger values
    assert(nthFibonacci(20) == 6765);
    assert(nthFibonacci(30) == 832040);
    assert(nthFibonacci(50) == 12586269025LL);
    assert(nthFibonacci(90) == 2880067194370816120LL); // fits in long long
}
#include <vector>
#include <algorithm>

// Compute the n-th Fibonacci number using bottom-up dynamic programming.
// F(0)=0, F(1)=1, F(n)=F(n-1)+F(n-2) for n>=2.
// Works correctly for n up to 90 (fits in long long).
long long nthFibonacci(int n) {
    // Ensure at least 2 entries so n=0 and n=1 are safely handled.
    const std::size_t size = std::max(n, 1) + 1;
    std::vector<long long> values(size);

    values[0] = 0;
    values[1] = 1;

    for (int i = 2; i <= n; ++i) {
        values[i] = values[i - 1] + values[i - 2];
    }

    return values[n];
}
// The solution uses a bottom-up DP table: allocate a `std::vector<long long>` of size `n+1` (or `n+2` for safety when `n=0`, to avoid out-of-bounds write), set `values[0] = 0` and `values[1] = 1`, then iterate `i` from 2 to `n`, computing each `values[i] = values[i-1] + values[i-2]`. Finally return `values[n]`. This avoids recursion overhead and stack overflow. Edge cases: for `n = 0`, we should still allocate a vector of size at least 2 (e.g., `size_t size = max(n, 1) + 1`) to safely assign `values[1] = 1`; for `n = 1`, the loop does not run and we return `values[1]`. The time complexity is O(n) and space complexity is O(n) due to the table; we could optimize to O(1) space with two variables, but the task explicitly says "bottom-up DP" with a table, so O(n) space is acceptable. The range is small (≤90) so `long long` is safe. No need for modulo. The function should be `const`‑correct (though it has no const parameters, it can be marked `noexcept` optionally). We must include `<vector>` and maybe `<algorithm>` for `std::max` to handle the size adjustment.
