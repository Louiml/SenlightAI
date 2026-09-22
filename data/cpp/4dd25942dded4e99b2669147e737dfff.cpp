/*
Write a C++ function `beeCounts(int n)` that, given a non-negative integer `n` (with `-1` used only as a sentinel in the original problem and not passed to this function), returns a `std::pair<long long, long long>` containing two numbers: the number of male bees and female bees in a bee colony after `n` generations, following the exact recurrence used in the original problem. Specifically, define a sequence `B` where `B[0] = 1` and `B[1] = 1`, and for `i >= 2`, `B[i] = B[i-1] + B[i-2]` (this is the Fibonacci-like sequence `1, 1, 2, 3, 5, 8, ...`). For a given `n`, the number of male bees is `B[n] + B[n-1] - 1`, and the number of female bees is `(B[n] + B[n-1] - 1) + B[n]`. For `n = 0`, the output must be `(0, 1)` (this is a special case handled separately). The function must compute values efficiently even for large `n` (up to, say, 10^6) using iterative precomputation with `long long` to avoid overflow. You may assume `n >= 0` when called; the function should not handle `-1` as input (that is only for the original I/O loop). Provide a self-contained implementation with a descriptively named free function and appropriate `const` correctness.
*/

#include <utility>
#include <vector>

// Returns a pair (males, females) for generation n.
// n must be non-negative. For n == 0, returns (0, 1) as a special case.
std::pair<long long, long long> beeCounts(int n) {
    if (n == 0) {
        return {0, 1};
    }
    std::vector<long long> bee(n + 1);
    bee[0] = 1;
    bee[1] = 1;
    for (int i = 2; i <= n; ++i) {
        bee[i] = bee[i - 1] + bee[i - 2];
    }
    long long males = bee[n] + bee[n - 1] - 1;
    long long females = males + bee[n];
    return {males, females};
}

#include <cassert>
#include <utility>

// Declare the function from the solution
std::pair<long long, long long> beeCounts(int n);

int main() {
    // n = 0 special case
    assert(beeCounts(0) == std::make_pair(0LL, 1LL));
    // n = 1
    assert(beeCounts(1) == std::make_pair(1LL, 2LL));
    // n = 2: B[2]=2, B[1]=1 -> males=2+1-1=2, females=2+2=4
    assert(beeCounts(2) == std::make_pair(2LL, 4LL));
    // n = 3: B[3]=3, B[2]=2 -> males=3+2-1=4, females=4+3=7
    assert(beeCounts(3) == std::make_pair(4LL, 7LL));
    // n = 4: B[4]=5, B[3]=3 -> males=5+3-1=7, females=7+5=12
    assert(beeCounts(4) == std::make_pair(7LL, 12LL));
    // n = 5: B[5]=8, B[4]=5 -> males=8+5-1=12, females=12+8=20
    assert(beeCounts(5) == std::make_pair(12LL, 20LL));
    // n = 10: B[10]=89, B[9]=55 -> males=89+55-1=143, females=143+89=232
    assert(beeCounts(10) == std::make_pair(143LL, 232LL));
    // Simple large value to ensure no overflow (n=50)
    auto result = beeCounts(50);
    assert(result.first > 0 && result.second > 0);
    return 0;
}

// The core idea is to precompute the sequence `B` up to the largest needed index. Since `B[n]` grows like the Fibonacci sequence, we must use `long long` and precompute iteratively to avoid recursion overhead. The recurrence is simple: start with a vector `bee = {1, 1}`, then for each next index `i` from 2 to `n`, push `bee[i-1] + bee[i-2]`. This gives `B[n]` in `bee[n]`. Then for `n >= 1`, the answer is: `males = bee[n] + bee[n-1] - 1`, `females = males + bee[n]`. For `n == 0`, we directly return `{0, 1}`. Edge cases: `n=0` returns `(0,1)`; `n=1` gives `B[1]=1, B[0]=1` → males = 1+1-1=1, females = 1+1=2, which matches the recurrence. The algorithm runs in `O(n)` time and `O(n)` space (to store the sequence up to `n`). If called multiple times with increasing `n`, we can reuse a static precomputed vector to avoid recomputation, but the task only requires a single call per function invocation, so we'll compute fresh each call for simplicity. However, to be safe for large `n`, we allocate the vector of size `n+1` and fill iteratively, which is efficient.
