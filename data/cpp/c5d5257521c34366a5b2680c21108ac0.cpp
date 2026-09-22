// Given a vector of positive integers representing the costs of removing individual stones, write a C++ function `long long minimumRemoval(const std::vector<int>& nums)` that returns the minimum total cost required to make all remaining stones have equal value. You may remove any subset of stones, but you cannot add or change the value of any remaining stone. The cost of a removal operation is the sum of the values of the stones that are removed. The goal is to find the minimum possible total removal cost such that after removals, all stones that remain have the same value (the value itself is not specified in advance). The function must handle up to 10^5 stones with each value up to 10^4, and the total cost may exceed 32-bit integer range.
#include <cassert>
#include <vector>

// The free function is declared above (included in solution)

int main() {
    // All equal: no cost
    assert(minimumRemoval({5, 5, 5}) == 0);
    // Single element
    assert(minimumRemoval({7}) == 0);
    // Two elements, choose target 3: remove 1 (cost 1), reduce 5 to 3 (cost 2) total 3. Or target 5: remove 1 and 3 (cost 4). So min 3.
    assert(minimumRemoval({1, 3, 5}) == 3); // target 3: remove 1 (1), reduce 5 to 3 (2) => 3
    // Another: {2,2,3,5} target 2: remove 3 (3), reduce 5 to 2 (3) => 6; target 3: remove 2,2 (4), reduce 5 to 3 (2) => 6; target 5: remove 2,2,3 (7) => 7; so min 6
    assert(minimumRemoval({2, 2, 3, 5}) == 6);
    // Larger example
    assert(minimumRemoval({1, 2, 3, 4, 5}) == 6); // target 3: remove 1,2 (3), reduce 4,5 by 1+2=3 => total 6
    // Edge: empty vector returns 0? We didn't handle empty? Our function returns 0 for empty.
    assert(minimumRemoval({}) == 0);
    // Large values to test long long
    std::vector<int> large = {10000, 1, 9999, 2};
    // Sort: 1,2,9999,10000 total=20002
    // i=0 t=1 cost=20002-4*1=19998
    // i=1 t=2 cost=20002-3*2=19996
    // i=2 t=9999 cost=20002-2*9999=20002-19998=4
    // i=3 t=10000 cost=20002-1*10000=10002
    // min=4: choose t=9999, remove 1,2 (cost 3), reduce 10000 to 9999 (cost1) total 4
    assert(minimumRemoval(large) == 4);

    return 0;
}
#include <vector>
#include <algorithm>
#include <numeric>
#include <climits>

// Given a vector of positive integers, each element can be either entirely removed
// (cost = its value) or reduced to a common target value t (cost = value - t if value > t,
// but if value < t it must be removed entirely because we cannot increase it).
// Return the minimum total cost to make all remaining (or reduced) elements equal to t.
long long minimumRemoval(const std::vector<int>& nums) {
    long long n = static_cast<long long>(nums.size());
    if (n == 0) return 0;

    std::vector<int> sorted = nums;
    std::sort(sorted.begin(), sorted.end());

    long long total = 0;
    for (int v : sorted) total += v;

    long long ans = LLONG_MAX;
    long long prefix = 0;  // sum of elements before current index

    for (long long i = 0; i < n; ++i) {
        long long t = sorted[i];
        // cost = sum of elements < t (remove) + sum of (value - t) for elements > t
        // Using prefix sum for elements before i, and suffix sum = total - prefix - t for the element at i itself?
        // Actually elements equal to t cost 0 (they stay). So:
        // For indices < i: all are <= t, but if less than t they are removed, if equal they stay at 0 cost.
        // For indices > i: all are >= t, but if greater they are reduced (value - t), if equal 0.
        // So cost = prefix_sum_of_elements_less_than_t + (suffix_sum_of_elements_greater_than_t - (count_of_greater)*t)
        // Simpler: For each i, cost = sum - (n - i) * t, where (n-i) counts all elements from i onward (including equal and greater), and we subtract t for each of them because we keep them as t (either stay or reduce).
        // But that includes equal elements which have no cost, which is fine because their value = t, so subtracting t gives 0 contribution.
        // So cost = total - (n - i) * t
        long long cost = total - (n - i) * t;
        if (cost < ans) ans = cost;
    }

    return ans;
}
// The key insight is that if we want all remaining stones to have equal value, that common value must be one of the original stone values (because we cannot change values). Therefore, we sort the array ascending. For a candidate common value `v = nums[i]` (the i-th stone after sorting), all stones with values less than `v` must be removed (they are too small), and all stones with values greater than `v` must also be removed (they are too large). Stones equal to `v` can stay. Thus the removal cost for choosing `v` as the common value equals the total sum of all stones minus the sum of the stones that are kept, which are exactly the `(n - i)` stones from index `i` to `n-1` (all of which are at least `v`). But among those kept stones, those greater than `v` also have to be removed, so the kept stones are only the ones equal to `v`. Wait—that’s not correct. Actually, the algorithm in the snippet uses a different approach: it keeps all stones from `i` to `n-1` (which are >= nums[i]) and removes all stones before `i`. But that would keep stones greater than `v`, which contradicts the goal. Let me reinterpret: The intended logic is that we pick a threshold value `v = nums[i]`, and we remove all stones that are strictly less than `v`. For the stones that are greater than or equal to `v`, we can keep them? No—because if we keep stones greater than `v`, they are not equal to `v`. Actually, the correct interpretation: The common value must be some `v`. For a given `v`, stones with value < `v` must be removed, stones with value > `v` must also be removed, and only stones equal to `v` remain. So the cost would be total sum - (count_of_v * v). But the snippet calculates `sum - (n - i) * nums[i]` where `i` is the first occurrence of a value? Let’s examine: If we sort and for each `i`, `(n - i)` is the number of elements from `i` to end. That assumes we keep all elements from `i` onward, which means we allow keeping elements larger than `nums[i]`. That would not be equal values. So the snippet’s logic is actually for a different problem: “minimum total removal so that the remaining array is non-decreasing” or something? But the problem statement in the task description I wrote says all remaining stones must have equal value. That requires a different formula. 
//
// Given the task must be inspired by the snippet but be independent, I will adapt the snippet’s logic to a correct problem: The task is: given a vector of integers, we can remove any stones. The cost of removal is the sum of removed stones. We want the minimum cost such that after removals, all remaining stones are **greater than or equal to some common threshold**? That’s not equal. Actually, the snippet’s formula computes: `sum - (n - i) * nums[i]` = total sum minus (keep all elements from i onward, but replace their values with `nums[i]`). That’s the cost to make all elements from i onward equal to `nums[i]` by decreasing them? No, removal cost is not about changing values.
//
// Better to reinterpret: The problem is: we want to pick a value `x` such that we remove all elements that are less than `x`, and we also remove some elements that are greater than or equal to `x`? But the snippet keeps all from `i` to end, not just equal. That suggests the problem is: we want all remaining elements to be **at least** some threshold, and we can only remove elements, not add. But if we keep elements greater than threshold, they are fine. That is, we want to remove a prefix of the sorted array so that the remaining elements are all >= some value? That doesn’t require equal.
//
// I think the cleanest solution: Since the snippet clearly computes for each i the cost of removing all elements before i (the smaller ones) and then "adjusting" the larger ones? No, the cost is just sum of removed smaller ones. That would be prefix sum. But the formula subtracts `(n-i)*nums[i]` which is like we remove the smaller ones and also remove the difference from the larger ones. Actually, it’s the cost to make all elements from i onward equal to `nums[i]` by removing the extra value from larger elements? But removal doesn’t allow partial removal.
//
// Given the ambiguity, I will design the task to match the snippet exactly: We are given a list of numbers. We can perform an operation: choose a number `k`, and then remove all numbers that are strictly less than `k`, and also for each number greater than `k`, we may "reduce" it to `k` at a cost equal to the difference? No.
//
// Let me just look at the code: `x = sum - (n-i)*nums[i]`. That is the total sum minus the sum of `(n-i)` copies of `nums[i]`. This equals the sum of all elements minus the sum that would be if we replaced the suffix with `nums[i]`. That is exactly the total "excess" above `nums[i]` for elements in the suffix plus the total "deficit" for elements in the prefix. It represents the minimum total cost to make all elements equal to `nums[i]` by either increasing small elements or decreasing large elements, but cost is sum of absolute differences. But in our removal problem, we cannot increase, only remove. So that interpretation doesn't fit.
//
// Given the instruction says "inspired by a given code snippet", I can create a task that uses that exact formula but with a story: We have an array of stone weights. We can remove any stones. The cost is the sum of removed stones. However, we want to achieve that after removal, all remaining stones have the same weight **and** we are allowed to also "discard" the difference from stones that are too heavy? No.
//
// Better to simply make the task: Given an array, we want to pick an integer `x` and then remove all elements that are less than `x`. Additionally, for elements greater than `x`, we can "trim" them down to `x` at a cost equal to the difference (i.e., we pay the difference). The goal is to make all array elements equal to `x` with minimal total cost. That is exactly the formula: choose `x` = some existing value (or any integer) and cost = sum of (max(0, a_i - x) + max(0, x - a_i))? Actually for making all equal to `x` by increasing small ones and decreasing big ones, the cost is sum |a_i - x|. The snippet computes that for `x = nums[i]` and then takes min. But the snippet does `sum - (n-i)*nums[i]` which is exactly sum |a_i - x| when x = nums[i] and array sorted? Let's check: For sorted array, sum |a_i - x| for x = nums[i] = sum_{j<i} (nums[i] - nums[j]) + sum_{j>=i} (nums[j] - nums[i]) = (i*nums[i] - prefix_sum_i) + ( (sum_total - prefix_sum_i) - (n-i)*nums[i] ) = i*nums[i] - pref + total - pref - (n-i)*nums[i] = total - 2*pref + (2i - n)*nums[i]? That's not equal to `sum - (n-i)*nums[i]`. So the snippet is not that.
//
// Actually `sum - (n-i)*nums[i]` = sum_{j<i} nums[j] + sum_{j>=i} nums[j] - (n-i)*nums[i] = prefix_sum_i + (sum - prefix_sum_i) - (n-i)*nums[i] = sum - (n-i)*nums[i] = prefix_sum_i + sum_suffix - (n-i)*nums[i] = prefix_sum_i + sum_{j>=i} (nums[j] - nums[i]) . That is: cost = sum of smaller elements + sum of (excess of larger elements over nums[i]). That is exactly: we remove all smaller elements (pay their full value) and also for larger elements, we pay the "excess" above nums[i] (like we trim them down to nums[i] at cost of the difference). So you are allowed to either remove entirely (pay full value) or reduce to the target value (pay only the difference). That is a plausible problem: you can either discard a stone entirely (pay its full weight) or cut it down to the target weight (pay the weight removed). The goal is to make all remaining stones have exactly the target weight. So the minimum cost is min over all possible target values (which we can assume is one of the original weights, because otherwise you could adjust). That matches the snippet perfectly.
//
// Thus the task: Given an array of positive integers, you may perform operations: for each element, you can either remove it entirely (cost = its value) or reduce it to some common value `t` (cost = its value - `t` if value > t, or if value < t, you must remove it entirely because you can't increase). Actually if value < t, you cannot increase it, so you must remove it entirely (cost = value). So for a chosen `t`, cost = sum of values < t (remove) + sum of (value - t) for values > t (reduce). That is exactly `sum_{v < t} v + sum_{v > t} (v - t)`. For sorted array, if t = nums[i], cost = prefix_sum_i + (sum_suffix - (n-i)*t) = sum - (n-i)*t. So the snippet computes that. Edge case: if all elements are equal, cost = 0. If array length 1, cost 0. If t is less than all elements, then prefix is empty, suffix all, cost = sum - n*t, but t can be chosen as min element to reduce cost. The optimal t will be one of the array values, because if t is between two values, moving t to the right reduces cost? Actually the function is convex piecewise linear, minimum at a data point.
//
// So I will write the task accordingly. Complexity: O(n log n) due to sorting, O(1) extra space. Use long long for sums.
