Given a positive integer `n` (1 ≤ n ≤ 1,000,000), write a C++ function `minOperationsAndPath` that returns a `std::vector<int>` containing the minimum number of operations to reduce `n` to 1, followed by the sequence of numbers from `n` down to 1. In one operation, you may replace the current number `x` by one of: `x-1`, `x/2` (only if `x` is even), or `x/3` (only if `x` is divisible by 3). The goal is to minimize the total number of operations. If multiple optimal paths exist, any one is acceptable. The first element of the returned vector must be the minimum operation count, and the remaining elements are the path including `n` and ending with `1`. The function must handle the base case `n = 1` by returning `{0, 1}`.
#include <cassert>
#include <vector>

// Function prototype (matches the solution above)
std::vector<int> minOperationsAndPath(int n);

int main() {
    // n = 1 : zero operations, path is just {1}
    assert(minOperationsAndPath(1) == std::vector<int>({0, 1}));

    // n = 2 : 2 -> 1 (one operation)
    {
        std::vector<int> res = minOperationsAndPath(2);
        assert(res[0] == 1);
        assert(res.size() == 3); // count + 2 numbers
        assert(res[1] == 2 && res[2] == 1);
    }

    // n = 3 : 3 -> 1 (one operation, since divisible by 3)
    {
        std::vector<int> res = minOperationsAndPath(3);
        assert(res[0] == 1);
        assert(res.size() == 3);
        assert(res[1] == 3 && res[2] == 1);
    }

    // n = 5 : optimal 5 -> 4 -> 2 -> 1 (3 operations)
    {
        std::vector<int> res = minOperationsAndPath(5);
        assert(res[0] == 3);
        assert(res.size() == 5); // count + 4 numbers: 5,4,2,1
        assert(res[1] == 5 && res[2] == 4 && res[3] == 2 && res[4] == 1);
    }

    // n = 10 : optimal 10 -> 9 -> 3 -> 1 (3 ops) or 10->5->4->2->1 (4 ops) so min=3
    {
        std::vector<int> res = minOperationsAndPath(10);
        assert(res[0] == 3);
        // verify that path is valid and ends at 1
        assert(res.size() == 1 + res[0] + 1); // count + number of nodes
        assert(res.back() == 1);
        // verify each step is legal
        for (size_t idx = 1; idx + 1 < res.size(); ++idx) {
            int a = res[idx];
            int b = res[idx + 1];
            bool valid = (b == a - 1) || (a % 2 == 0 && b == a / 2) || (a % 3 == 0 && b == a / 3);
            assert(valid);
        }
    }

    // n = 100 : check minimal count equals known value and path ends at 1
    {
        std::vector<int> res = minOperationsAndPath(100);
        // Fastest: 100 -> 99 -> 33 -> 11 -> 10 -> 9 -> 3 -> 1 = 7 ops
        assert(res[0] == 7);
        assert(res.back() == 1);
    }

    // n = 1000000 : just check it runs and path is valid, count is finite
    {
        std::vector<int> res = minOperationsAndPath(1000000);
        assert(res[0] > 0);
        assert(res.back() == 1);
        assert(res.size() == static_cast<size_t>(res[0] + 2));
    }

    return 0;
}
#include <vector>
#include <algorithm>

// Returns [minimum number of operations, path from n to 1 inclusive].
// Path is in order: n, ..., 1.
std::vector<int> minOperationsAndPath(int n) {
    if (n == 1) {
        return {0, 1};
    }

    std::vector<int> dp(n + 1);
    std::vector<int> pre(n + 1);

    dp[1] = 0;
    pre[1] = 1; // not used, but set for completeness

    for (int i = 2; i <= n; ++i) {
        dp[i] = dp[i - 1] + 1;
        pre[i] = i - 1;

        if (i % 3 == 0 && dp[i] > dp[i / 3] + 1) {
            dp[i] = dp[i / 3] + 1;
            pre[i] = i / 3;
        }

        if (i % 2 == 0 && dp[i] > dp[i / 2] + 1) {
            dp[i] = dp[i / 2] + 1;
            pre[i] = i / 2;
        }
    }

    std::vector<int> result;
    result.push_back(dp[n]);
    int cur = n;
    while (true) {
        result.push_back(cur);
        if (cur == 1) break;
        cur = pre[cur];
    }
    return result;
}
// This is a classic dynamic programming problem on the natural numbers from 1 to `n`. Define `dp[i]` as the minimum operations needed to reduce `i` to 1. The recurrence is `dp[1] = 0`, and for `i > 1`, `dp[i] = 1 + min(dp[i-1], dp[i/2] if i even, dp[i/3] if i divisible by 3)`. To reconstruct an optimal path, store for each `i` the predecessor `pre[i]` that gave the minimal value. A bottom-up loop from 2 to `n` fills these arrays; because `i-1`, `i/2`, and `i/3` are all strictly less than `i`, each dp value is already computed. Edge cases: `n = 1` must be handled without any loop; also ensure that division checks are made before using the result (i.e., check divisibility first). Time complexity is `O(n)` and auxiliary space is `O(n)` for the two arrays. The path reconstruction walks backward from `n` to `1`, producing the numbers in decreasing order, and the final vector prepends the count `dp[n]`.
