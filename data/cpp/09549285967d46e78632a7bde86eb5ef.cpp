/*
Given a vector of positive integers, write a C++ function `canPartition` that determines whether the vector can be partitioned into two subsets with equal sums. The function must return `true` if such a partition exists, and `false` otherwise. The input vector may be empty (in which case the total sum is 0, and since 0 is even, an empty partition of two empties is allowed, so return `true`), and may contain duplicate values. The algorithm must use dynamic programming with memoization to avoid recomputation, and must handle the constraint that the total sum of all elements must be even for a valid partition to exist. If the total sum is odd, return `false` immediately.
*/
#include <vector>

// Determine if the vector can be partitioned into two subsets of equal sum.
bool canPartition(std::vector<int>& nums) {
    int total = 0;
    for (int num : nums) {
        total += num;
    }
    // If total is odd, cannot split equally.
    if (total % 2 != 0) return false;
    int target = total / 2;
    int n = nums.size();
    // dp[i][s] = -1 (uncomputed), 0 (false), 1 (true)
    std::vector<std::vector<int>> dp(n + 1, std::vector<int>(target + 1, -1));
    
    // Recursive lambda with memoization.
    // Index i, current sum currsum.
    std::function<bool(int, int)> solve = [&](int i, int currsum) -> bool {
        if (currsum == target) return true;
        if (currsum > target || i >= n) return false;
        if (dp[i][currsum] != -1) return dp[i][currsum] == 1;
        // Option 1: include nums[i]
        bool include = solve(i + 1, currsum + nums[i]);
        // Option 2: skip nums[i]
        bool skip = solve(i + 1, currsum);
        bool result = include || skip;
        dp[i][currsum] = result ? 1 : 0;
        return result;
    };
    
    return solve(0, 0);
}
*(Note: The `std::function` and `#include <functional>` are required; the code above already includes `<vector>` but needs `<functional>` for `std::function`. I will present the final code with all headers in the solution section.)*

I realize I missed the `<functional>` include. Let me provide the corrected full solution:

#include <vector>
#include <functional>

// Determine if the vector can be partitioned into two subsets of equal sum.
bool canPartition(std::vector<int>& nums) {
    int total = 0;
    for (int num : nums) {
        total += num;
    }
    if (total % 2 != 0) return false;
    int target = total / 2;
    int n = nums.size();
    std::vector<std::vector<int>> dp(n + 1, std::vector<int>(target + 1, -1));
    
    std::function<bool(int, int)> solve = [&](int i, int currsum) -> bool {
        if (currsum == target) return true;
        if (currsum > target || i >= n) return false;
        if (dp[i][currsum] != -1) return dp[i][currsum] == 1;
        bool include = solve(i + 1, currsum + nums[i]);
        bool skip = solve(i + 1, currsum);
        bool result = include || skip;
        dp[i][currsum] = result ? 1 : 0;
        return result;
    };
    
    return solve(0, 0);
}
#include <cassert>
#include <vector>

// The solution function is assumed to be declared above.

int main() {
    std::vector<int> test1 = {1, 5, 11, 5};
    assert(canPartition(test1) == true);

    std::vector<int> test2 = {1, 2, 3, 5};
    assert(canPartition(test2) == false);

    std::vector<int> test3 = {1, 1};
    assert(canPartition(test3) == true);

    std::vector<int> test4 = {2};
    assert(canPartition(test4) == false);

    std::vector<int> test5 = {7, 3, 4, 6, 2}; // sum=22, target=11, {7,4} works
    assert(canPartition(test5) == true);

    std::vector<int> test6 = {1, 2, 5}; // sum=8, target=4, no subset sums to 4
    assert(canPartition(test6) == false);

    std::vector<int> test7 = {}; // empty, total=0, target=0, true
    assert(canPartition(test7) == true);

    std::vector<int> test8 = {1, 1, 1, 1}; // sum=4, target=2, any two sum to 2
    assert(canPartition(test8) == true);

    std::vector<int> test9 = {0, 0, 0}; // sum=0, target=0, true
    assert(canPartition(test9) == true);

    std::vector<int> test10 = {2, 2, 2, 2}; // sum=8, target=4, {2,2} sums to 4
    assert(canPartition(test10) == true);

    return 0;
}
// The problem reduces to checking whether a subset of the given numbers sums to exactly half of the total sum. If such a subset exists, the remaining elements form the other subset, and both sums are equal. The main steps: first compute the total sum; if it is odd, return `false`. Otherwise, define a target sum `target = total/2`. Use a recursive function with memoization that processes indices from 0 to `n-1`. At each index `i`, we have two choices: either include `nums[i]` in the current subset (increasing current sum) or skip it. The recursion terminates successfully when the current sum equals the target. If the current sum exceeds the target or we run out of elements, return `false`. The memoization table is a 2D vector of size `(n+1) x (target+1)`, initialized to `-1` (uncomputed), where `dp[i][currsum]` stores the result for the subproblem starting at index `i` with a current accumulated sum. Time complexity is `O(n * target)` because each state `(i, currsum)` is computed once. Space complexity is also `O(n * target)` for the memoization table plus recursion stack depth of `O(n)`. Edge cases include an empty vector (total = 0, target = 0, recursion returns true immediately because initial currsum == target), single element (if its value equals target, true else false), and all-zero elements (any partition works, returns true).
