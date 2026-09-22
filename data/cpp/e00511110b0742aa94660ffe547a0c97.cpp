/*
Given a list of `k` positive integer divisors and a starting number `n`, write a C++ function `findLargestMultiple` that returns a `std::pair<size_t, long long>` representing the index (1-based) of a divisor that evenly divides some number in the descending sequence `n, n-1, n-2, ...` (decrementing by 1 each step), along with the quotient `(current_value / divisor)`. The search stops as soon as such a divisor is found. The function should return `{0, 0}` if no divisor divides any value before reaching 0. The original code increments a variable `z` and tests `n % arr[i] == z`, which effectively searches for a number `m = n - z` that is divisible by `arr[i]`, but it must be careful: the original loop does not explicitly stop at `z >= n` (it would break if `z` grows large, but it could loop indefinitely if no match exists). For a robust task, assume there is always at least one valid answer (i.e., divisors are all ≤ n), but your function must still handle the general case with no answer gracefully. The function signature is `std::pair<size_t, long long> findLargestMultiple(long long n, const std::vector<long long>& divisors)`. The return value is `{index+1, quotient}` where `index` is the zero-based position in the vector, or `{0, 0}` if none found.
*/

#include <vector>
#include <utility>

// Find the first number in the descending sequence n, n-1, ... that is divisible by one of the divisors.
// Returns {0,0} if no such number exists (including when n < 0? treat n as non-negative).
std::pair<size_t, long long> findLargestMultiple(long long n, const std::vector<long long>& divisors) {
    // Handle n = 0 immediately: 0 is divisible by any positive divisor, quotient is 0.
    if (n == 0) {
        for (size_t i = 0; i < divisors.size(); ++i) {
            if (divisors[i] != 0) { // divisor must be positive per problem, but guard anyway
                return {i + 1, 0};
            }
        }
        return {0, 0};
    }

    // Iterate over decreasing values m from n down to 1 (we skip 0 because quotient 0 is usually not intended,
    // but the original code would reach it if no other match; we'll include 0 for completeness).
    for (long long m = n; m >= 0; --m) {
        for (size_t i = 0; i < divisors.size(); ++i) {
            long long d = divisors[i];
            if (d != 0 && m % d == 0) {
                return {i + 1, m / d};
            }
        }
    }
    return {0, 0};
}

#include <cassert>
#include <vector>
#include <utility>

// Forward declaration or include the solution above; here we assume the function is in scope.
std::pair<size_t, long long> findLargestMultiple(long long n, const std::vector<long long>& divisors);

int main() {
    // Original example: n=10, divisors={2,3}, m=10 divisible by 2 -> index 1, quotient 5
    assert(findLargestMultiple(10, {2, 3}) == std::make_pair<size_t, long long>(1, 5));

    // n=10, divisors={7,3}: 10%7=3, 10%3=1, 9%7=2, 9%3=0 -> m=9 divisor 3 index 2 quotient 3
    assert(findLargestMultiple(10, {7, 3}) == std::make_pair<size_t, long long>(2, 3));

    // n=1, divisors={2,3}: 1%2=1, 1%3=1, 0%2=0 -> m=0 divisor 2 index1 quotient 0
    assert(findLargestMultiple(1, {2, 3}) == std::make_pair<size_t, long long>(1, 0));

    // n=5, divisor 1 always matches at m=5
    assert(findLargestMultiple(5, {1, 2}) == std::make_pair<size_t, long long>(1, 5));

    // n=0: immediate match
    assert(findLargestMultiple(0, {5, 7}) == std::make_pair<size_t, long long>(1, 0));

    // n=6, divisors {4,5}: 6%4=2,6%5=1,5%4=1,5%5=0 -> m=5 divisor5 index2 quotient1
    assert(findLargestMultiple(6, {4, 5}) == std::make_pair<size_t, long long>(2, 1));

    // No match case: n=1, divisors={2,3} but if we exclude 0? We include 0, so returns {1,0}
    // To test no match, use n=1, divisors={2} but we include m=0 so returns {1,0) not {0,0}.
    // So to test {0,0}, use an empty divisor list.
    assert(findLargestMultiple(10, {}) == std::make_pair<size_t, long long>(0, 0));

    // Large n, divisor that divides n directly
    assert(findLargestMultiple(1000000000000LL, {7, 2}) == std::make_pair<size_t, long long>(2, 500000000000LL));

    return 0;
}

// The problem requires finding the first number in the decreasing sequence `n, n-1, n-2, ...` (starting from `n`) that is divisible by at least one of the given divisors. The brute-force approach follows directly: for each candidate value `m` starting from `n` down to 1 (or 0, but 0 is divisible by everything and would give quotient 0, but the original code would output that if `z` reached `n`), check each divisor. The first divisor that divides `m` gives both the divisor index (1-based) and the quotient `m / divisor`. The search order matters: we iterate over divisors in the given order for each `m`, and we iterate `m` from `n` downwards. The original code uses a variable `z` such that `m = n - z`, and checks `n % arr[i] == z` which is equivalent to `(n - z) % arr[i] == 0` because `n % arr[i] == z` implies `n = q*arr[i] + z` so `n - z` is a multiple. This works only if `z < arr[i]`; if `z >= arr[i]`, the modulo cannot equal `z`, but since `z` only grows to `n`, it's safe as long as `arr[i] > n`? Actually if `arr[i] > n` then `n % arr[i] == n`, so only `z = n` would match, giving `m=0`. That is a valid edge case. The straightforward implementation: loop `m` from `n` down to 0, and for each `m`, loop over divisors; if `m % divisor == 0` (with `m > 0` or if `m == 0` works too), return. However, to avoid infinite loops when no answer exists, we must stop when `m` reaches 0. The time complexity: in the worst case, we check up to `n+1` values, each against `k` divisors, so O(n*k). Space complexity: O(1) additional besides the input vector. Edge cases: `n` can be 0, divisors may contain 1 (always divisible), divisors may be larger than `n` (then only `m=0` works if we include `m=0`). We should handle `n=0` immediately: if any divisor is positive, `0 % divisor == 0` so return that divisor index with quotient 0, but `0 / divisor = 0`. The function returns `{0,0}` if none found after `m` goes below 0.
