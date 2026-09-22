Write a C++ function that, given a positive integer `n` and a non-negative integer `k` where `0 ≤ k ≤ n`, returns the number of permutations of `k` items selected from a set of `n` distinct items, calculated as the falling factorial \( n! / (n-k)! = n \cdot (n-1) \cdot \ldots \cdot (n-k+1) \). The function must handle large values of `n` (up to 20) and `k` (up to `n`) without overflow by returning a `long long int`, and should return `1` when `k = 0` (since there is exactly one way to select zero items). The result is the product of exactly `k` terms, starting from `n` and decreasing by 1 each step.

The main algorithm is a direct iterative computation of the falling factorial. Start with a result variable initialized to `1`. Loop `k` times, multiplying the result by the current value starting from `n` and decreasing by 1 each iteration. The loop runs exactly `k` times, so for `k = 0` the loop body never executes and the function correctly returns `1`. Edge cases to consider: `k = 0` (returns 1), `k = 1` (returns `n`), and `k = n` (returns `n!`). Since `n` is at most 20, `n!` fits within the range of a 64-bit signed integer (max ~2×10^18 for 20! ≈ 2.43×10^18, which is just under 9.22×10^18 for `long long` in C++). Using `long long int` for the result and loop variable avoids overflow for all valid inputs. Time complexity is O(k), and space complexity is O(1) since only a single accumulator variable is used.

#include <cstdint>

// Compute the number of permutations of k items selected from n distinct items.
// Returns n * (n-1) * ... * (n-k+1), 1 if k == 0, and 0 is not valid because k <= n.
long long int permutations(long long int n, long long int k) {
    long long int product = 1;
    for (long long int i = n; i > n - k; --i) {
        product *= i;
    }
    return product;
}

#include <cassert>

long long int permutations(long long int n, long long int k);

int main() {
    assert(permutations(52, 2) == 2652);
    assert(permutations(5, 0) == 1);
    assert(permutations(5, 1) == 5);
    assert(permutations(5, 5) == 120);
    assert(permutations(20, 3) == 6840);
    assert(permutations(20, 20) == 2432902008176640000LL);
    assert(permutations(10, 10) == 3628800);
    assert(permutations(1, 1) == 1);
    assert(permutations(7, 4) == 840);
    assert(permutations(0, 0) == 1);
}
