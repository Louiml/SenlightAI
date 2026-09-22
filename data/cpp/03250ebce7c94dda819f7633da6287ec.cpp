/*
Write a C++ function `resolvePens(const std::vector<int>& a, const std::vector<int>& b, const std::vector<int>& c, const std::vector<int>& d, const std::vector<int>& k)` that, for each test case, determines the minimum number of pen packages and pencil packages needed to cover exactly `a` pens and `b` pencils, where each pen package contains `c` pens and each pencil package contains `d` pencils. The function should return a vector of pairs: for each test case, the pair `(x, y)` where `x` is the number of pen packages and `y` is the number of pencil packages, or `(-1, -1)` if it is impossible to buy enough packages without exceeding `k` total packages. Note that you can buy more pens/pencils than needed (i.e., you do not require exact equality on counts, only that the total number of packages does not exceed `k`). All parameters are positive integers, and the arrays have equal length. The function must be efficient for up to `10^5` test cases with values up to `10^9`.
*/

#include <vector>
#include <utility>

// For each test case, compute minimal pen and pencil packages.
// Returns vector of pairs: (pens_packages, pencils_packages) or (-1, -1) if exceeds k.
std::vector<std::pair<long long, long long>> resolvePens(
    const std::vector<long long>& a,
    const std::vector<long long>& b,
    const std::vector<long long>& c,
    const std::vector<long long>& d,
    const std::vector<long long>& k) {
    
    std::vector<std::pair<long long, long long>> result;
    result.reserve(a.size());
    
    for (size_t i = 0; i < a.size(); ++i) {
        // Ceiling division for positive integers
        long long x = (a[i] + c[i] - 1) / c[i];
        long long y = (b[i] + d[i] - 1) / d[i];
        if (x + y > k[i]) {
            result.emplace_back(-1, -1);
        } else {
            result.emplace_back(x, y);
        }
    }
    return result;
}

#include <cassert>
#include <vector>
#include <utility>

// The solution function is assumed to be defined above.
// Including it here for completeness in a real environment.

int main() {
    // Sample from the original problem statement
    std::vector<long long> a = {1, 2, 3, 4, 5};
    std::vector<long long> b = {1, 2, 3, 4, 5};
    std::vector<long long> c = {1, 1, 1, 1, 1};
    std::vector<long long> d = {1, 1, 1, 1, 1};
    std::vector<long long> k = {2, 4, 6, 8, 10};
    auto res = resolvePens(a, b, c, d, k);
    assert(res[0] == std::make_pair(1LL, 1LL));
    assert(res[1] == std::make_pair(2LL, 2LL));
    assert(res[2] == std::make_pair(3LL, 3LL));
    assert(res[3] == std::make_pair(4LL, 4LL));
    assert(res[4] == std::make_pair(5LL, 5LL));

    // Test case with insufficient total packages
    std::vector<long long> a2 = {10}, b2 = {10}, c2 = {6}, d2 = {6}, k2 = {3};
    auto res2 = resolvePens(a2, b2, c2, d2, k2);
    assert(res2[0] == std::make_pair(-1LL, -1LL));

    // Test exact divisibility
    std::vector<long long> a3 = {12}, b3 = {15}, c3 = {4}, d3 = {5}, k3 = {7};
    auto res3 = resolvePens(a3, b3, c3, d3, k3);
    assert(res3[0] == std::make_pair(3LL, 3LL));

    // Test large values
    std::vector<long long> a4 = {1000000000LL}, b4 = {1000000000LL}, c4 = {1LL}, d4 = {1LL}, k4 = {2000000000LL};
    auto res4 = resolvePens(a4, b4, c4, d4, k4);
    assert(res4[0] == std::make_pair(1000000000LL, 1000000000LL));

    // Test k exactly equal to sum
    std::vector<long long> a5 = {5}, b5 = {7}, c5 = {2}, d5 = {3}, k5 = {5};
    auto res5 = resolvePens(a5, b5, c5, d5, k5);
    assert(res5[0] == std::make_pair(3LL, 3LL));

    return 0;
}

// The problem is a straightforward greedy/arithmetic optimization. For each test case, we need the smallest integer `x` such that `x * c >= a`, which is `ceil(a / c)`, and similarly the smallest integer `y` such that `y * d >= b`, which is `ceil(b / d)`. The minimum total packages is `x + y`; if this sum exceeds `k`, it is impossible, so answer `(-1, -1)`, otherwise answer `(x, y)`. This works because using more packages than the minimum for either type is never beneficial: any extra package increases the total count and cannot help satisfy the condition of staying within `k`. Edge cases include when `a` or `b` is exactly divisible by `c` or `d`, where `ceil` equals the exact division, and when `k` is very small (e.g., `k < 2`). Computation uses integer arithmetic: `(a + c - 1) / c` for ceiling division without overflow since all values are at most `10^9` and the product in the addition could be up to `2*10^9`, which fits in a 64-bit signed integer. Time complexity is `O(n)` for `n` test cases, space complexity `O(n)` for the result.
