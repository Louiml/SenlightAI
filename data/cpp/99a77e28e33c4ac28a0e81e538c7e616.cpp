// Write a standalone C++ function `countIntegerPartitions(int n, int k)` that returns the number of ways to express the positive integer `n` as a sum of positive integers, where each summand does not exceed `k`, and the order of summands does not matter (i.e., partitions are unordered). The function must handle the base cases where `n` or `k` is 1, where `n < k`, where `n == k`, and the general recursive case, using dynamic programming with memoization to avoid recomputation. The result may be large, so use a `long long` return type. The function should be `const`-correct and not depend on global state; you may allocate a 2D memo table inside the function or use a static local cache if you prefer.

#include <cassert>

int main() {
    // n=5, k=5 -> 7 (as given in the problem snippet)
    assert(countIntegerPartitions(5, 5) == 7);
    // n=5, k=4 -> partitions with max part <=4: 5=4+1, 3+2, 3+1+1, 2+2+1, 2+1+1+1, 1+1+1+1+1 = 6 (excluding 5 alone)
    assert(countIntegerPartitions(5, 4) == 6);
    // n=1, any k>=1 -> 1
    assert(countIntegerPartitions(1, 1) == 1);
    assert(countIntegerPartitions(1, 10) == 1);
    // n=3, k=3 -> partitions: 3, 2+1, 1+1+1 => 3
    assert(countIntegerPartitions(3, 3) == 3);
    // n=4, k=2 -> partitions: 2+2, 2+1+1, 1+1+1+1 => 3
    assert(countIntegerPartitions(4, 2) == 3);
    // n=6, k=3 -> partitions: 3+3, 3+2+1, 3+1+1+1, 2+2+2, 2+2+1+1, 2+1+1+1+1, 1+1+1+1+1+1 => 7 (verify manually)
    assert(countIntegerPartitions(6, 3) == 7);
    // n=2, k=5 -> partitions: 2, 1+1 => 2
    assert(countIntegerPartitions(2, 5) == 2);
    // n=0, k=1 -> should be 1 (empty partition) but not required; let's test for n=0? The task says positive n, so skip.
    // Large check: n=10, k=10 -> p(10)=42 (number of partitions of 10)
    assert(countIntegerPartitions(10, 10) == 42);
    return 0;
}
Note: The test includes a manual check for (6,3) which equals 7? Let's verify: partitions of 6 with parts <=3:
- 3+3
- 3+2+1
- 3+1+1+1
- 2+2+2
- 2+2+1+1
- 2+1+1+1+1
- 1+1+1+1+1+1
That's 7, correct. Good.

Thus the final answer.

#include <vector>
#include <cstddef>

// Count the number of unordered partitions of n where each part <= k.
// Uses memoization with a local 2D table.
long long countIntegerPartitions(int n, int k) {
    // Table dimensions: rows 0..n, columns 0..k.
    // Initialize with -1 to indicate "not computed".
    std::vector<std::vector<long long>> memo(n + 1, std::vector<long long>(k + 1, -1));
    
    // Recursive lambda with memoization.
    // Uses std::function or a helper function.
    // For simplicity, we define a recursive function using a lambda with capture.
    // But since lambdas can't call themselves easily without std::function, we use a helper.
    // We can define a nested function using a struct or just a static function outside.
    // Since the task requires a free function, we can use a helper lambda with std::function.
    
    // Alternatively, use a recursive function defined inside via std::function.
    std::function<long long(int, int)> solve = [&](int a, int b) -> long long {
        // Base cases
        if (a == 0) return 1; // empty sum? But typically n>=1. Still safe.
        if (b == 0) return 0; // no parts allowed
        if (a == 1 || b == 1) return 1;
        
        if (memo[a][b] != -1) return memo[a][b];
        
        long long result;
        if (a < b) {
            result = solve(a, a);
        } else if (a == b) {
            result = solve(a, b - 1) + 1; // +1 for the partition consisting solely of b
        } else { // a > b
            result = solve(a - b, b) + solve(a, b - 1);
        }
        memo[a][b] = result;
        return result;
    };
    
    return solve(n, k);
}
But wait, we need to include `<functional>` for std::function. The solution above is free function, but it captures a lambda. However, that's fine. Alternatively, we can write a helper function with static memoization, but the task says "free function". The above is a free function. It's self-contained. However, the lambda uses recursion and captures the vector by reference. That's fine.

But to keep it more standard, we could define a struct with a static method, but the task says "descriptively named free function". So I'll keep as above but need to include `<functional>`. Also note that the lambda is defined inside the function, so it's fine.

Here is the corrected code with necessary headers:
#include <vector>
#include <functional>

// Count the number of unordered partitions of n where each part <= k.
// Uses memoization with a local 2D table.
long long countIntegerPartitions(int n, int k) {
    // Memo table
    std::vector<std::vector<long long>> memo(n + 1, std::vector<long long>(k + 1, -1));
    
    std::function<long long(int, int)> solve = [&](int a, int b) -> long long {
        if (a == 0) return 1; // only empty partition (not used for n>=1)
        if (b == 0) return 0; // no parts allowed
        if (a == 1 || b == 1) return 1;
        if (memo[a][b] != -1) return memo[a][b];
        
        long long result;
        if (a < b) {
            result = solve(a, a);
        } else if (a == b) {
            result = solve(a, b - 1) + 1;
        } else {
            result = solve(a - b, b) + solve(a, b - 1);
        }
        memo[a][b] = result;
        return result;
    };
    
    return solve(n, k);
}
This is correct. Now test code.

// The problem is a classic integer partition with an upper bound on the largest part. The recurrence is:
// - If `n == 1` or `k == 1`, there is exactly 1 way (all ones or a single part).
// - If `n < k`, then the largest part cannot exceed `n`, so `f(n,k) = f(n,n)`.
// - If `n == k`, then we either include a part of size `k` (which means `n-k=0` but we handle as +1 for the partition consisting of just `k`), or we exclude it and use `f(n,k-1)`. So `f(n,k) = f(n,k-1) + 1`.
// - If `n > k`, we either include at least one part of size `k` (which reduces to `f(n-k,k)` because we still can use parts up to `k`), or we exclude `k` entirely (`f(n,k-1)`). So `f(n,k) = f(n-k,k) + f(n,k-1)`.
//
// We can implement this using a 2D memoization table `memo[n+1][k+1]` initialized to -1 (meaning not computed). The recursion has overlapping subproblems. The base cases are straightforward. Edge cases: if `n` or `k` is 0, return 0 because there are no ways to partition a positive number with a zero upper bound, but the problem states positive integers so we assume `n>=1, k>=1`. For `n=0` (if it ever occurs) we could return 1 (empty partition) but not needed.
//
// Time complexity: The memo table has size `(n+1)*(k+1)`, and each state is computed in O(1) after base cases, so O(n*k). Space: O(n*k) for the memo table. If we use a 2D DP iterative approach, same complexity.
