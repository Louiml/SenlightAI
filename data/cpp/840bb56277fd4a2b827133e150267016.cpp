Given an array of `n` integers, write a C++ function `minimumChangesToSatisfyEqualDifference` that returns the minimum number of elements that must be removed so that in the remaining array, for every pair `(i, j)` of distinct indices, the absolute difference `|a[i] - a[j]|` is the same for all such pairs. In other words, after removals, the remaining multiset must have the property that all pairwise absolute differences are equal. If `n ≤ 2`, no removals are needed (the condition is trivially satisfied). If `n > 2`, show that the optimal remaining array is either all identical elements or exactly two distinct values each occurring multiple times, and compute the minimum removals accordingly. The input array can contain any integer values, including negatives, and duplicates. The array size is at most 1e5.

For any remaining array of size `k`, the condition “all pairwise absolute differences are equal” is very restrictive. 
- If `k = 0` or `k = 1`, the condition holds vacuously.
- If `k = 2`, any two distinct numbers have exactly one pairwise difference, so the condition always holds.
- If `k ≥ 3`, let’s analyze. Suppose the remaining numbers are sorted: `b1 ≤ b2 ≤ ... ≤ bk`. All pairwise differences must equal a constant `D`. Then `b2 - b1 = D`, `b3 - b1 = D` (since difference between `b3` and `b1` must also be `D`), so `b2 = b1 + D` and `b3 = b1 + D`, implying `b2 = b3`. Continuing, we see that all elements equal `b1` or `b2`. Since `b3 - b2 = 0` must equal `D`, we get `D = 0`, so all elements are equal. Therefore for `k ≥ 3`, the only valid remaining arrays are those where all elements are identical (all pairwise differences are 0).

Thus, for `n > 2`, we have two candidate optimal remaining arrays:
1. Keep all occurrences of a single most frequent value. The number of removals is `n - max_frequency`.
2. Keep exactly two distinct values (any two), each occurring at least once. But we can also keep more than two occurrences of each of these two values. However, if we keep three or more total elements, they must all be equal (from above), so the only way to have two distinct values is to keep exactly two elements: one of each distinct value. The number of removals then is `n - 2`. This is always possible as long as `n ≥ 2`. For `n > 2`, we can always achieve `n - 2` removals by keeping any two distinct elements (if all elements are identical, then the "two distinct" case is not feasible, but the all-equal case gives 0 removals then).

So the answer for `n > 2` is `n - max(max_frequency, 2)`. Because we can either keep all occurrences of the most frequent value (if that frequency is at least 2, but even if it's 1, keeping 2 distinct elements is better, and we compute max with 2). Edge cases: when all elements are the same, `max_frequency = n`, so `max_frequency > 2`, and removals = 0. When `n = 3` and all distinct, `max_frequency = 1`, so `max(1,2)=2`, removals = 1 (remove any one element, leaving two distinct values). Time complexity: O(n) for counting frequencies using a hash map. Space complexity: O(n) in worst case for the hash map.

#include <unordered_map>
#include <algorithm>
#include <vector>

/**
 * @brief Compute the minimum number of elements to remove so that all remaining
 *        pairwise absolute differences are equal.
 * 
 * @param a Constant reference to a vector of integers (the input array).
 * @return The minimum number of elements to remove.
 */
int minimumChangesToSatisfyEqualDifference(const std::vector<int>& a) {
    const int n = static_cast<int>(a.size());
    if (n <= 2) {
        return 0;
    }
    
    std::unordered_map<int, int> frequency;
    int max_frequency = 0;
    for (const int value : a) {
        int current = ++frequency[value];
        max_frequency = std::max(max_frequency, current);
    }
    
    // Final answer: n - max(max_frequency, 2)
    return n - std::max(max_frequency, 2);
}

#include <cassert>
#include <vector>

int minimumChangesToSatisfyEqualDifference(const std::vector<int>& a);

int main() {
    // n <= 2
    assert(minimumChangesToSatisfyEqualDifference({}) == 0);
    assert(minimumChangesToSatisfyEqualDifference({5}) == 0);
    assert(minimumChangesToSatisfyEqualDifference({1, 2}) == 0);
    assert(minimumChangesToSatisfyEqualDifference({7, 7}) == 0);
    
    // All distinct, n > 2
    assert(minimumChangesToSatisfyEqualDifference({1, 2, 3}) == 1);
    assert(minimumChangesToSatisfyEqualDifference({1, 2, 3, 4, 5}) == 3);
    
    // Duplicates
    assert(minimumChangesToSatisfyEqualDifference({1, 1, 1, 1}) == 0);
    assert(minimumChangesToSatisfyEqualDifference({1, 1, 2, 2, 3}) == 2);
    assert(minimumChangesToSatisfyEqualDifference({1, 2, 2, 2, 2}) == 1);
    assert(minimumChangesToSatisfyEqualDifference({-5, -5, 3, 3, 3, 0}) == 2);
    
    // Large frequency of one value
    std::vector<int> large(100000, 42);
    assert(minimumChangesToSatisfyEqualDifference(large) == 0);
    
    // One frequent, rest scattered
    std::vector<int> mixed = {1, 1, 1, 2, 3, 4, 5};
    assert(minimumChangesToSatisfyEqualDifference(mixed) == 4);
    
    return 0;
}
