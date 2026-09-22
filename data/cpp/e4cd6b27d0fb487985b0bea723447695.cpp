Write a C++ function that takes a vector of non-negative integers and returns the number of subsets whose bitwise OR operation yields the maximum possible OR value among all subsets of the given vector. The vector will contain at least one element and at most 16 elements, with each element in the range [0, 10^5]. A subset can be any selection of elements (including the empty set, whose OR is 0). The function must handle duplicate values correctly and return the count as an integer. Note: The maximum OR value is the bitwise OR of all elements in the vector, and the answer is guaranteed to fit within a 32-bit signed integer.

// The key insight is that the maximum possible OR is simply the bitwise OR of all elements, since OR is monotonic (adding more elements can only set more bits). Once we know this target maximum, we need to count subsets whose OR equals it. We can solve this using dynamic programming over the OR values. Let `dp[i]` represent the number of subsets (from the elements processed so far) that have OR exactly equal to `i`. Initialize `dp[0] = 1` (the empty subset). For each element `num`, we update the DP by considering adding `num` to every existing subset: for each possible OR value `i` that we have already reached, the subset with OR `i` combined with `num` produces OR `i | num`. To avoid using already-updated values, iterate over `i` in descending order from the current maximum OR achieved so far. Track the current maximum OR (`maxOr`) as we process elements (it becomes the OR of all processed elements). After processing all elements, the answer is `dp[maxOr]`. Edge cases: an empty subset contributes OR 0, so if all elements are 0, the maximum OR is 0 and the answer is 2^n (all subsets). Duplicate elements are handled naturally because DP counts each distinct subset, not each combination. Time complexity: O(n * M) where M is the maximum possible OR value (bounded by 2^17 since elements ≤ 10^5 < 2^17). Space complexity: O(M) for the DP array.

#include <vector>
#include <cstdint>

// Count subsets whose bitwise OR equals the maximum possible OR of the entire vector.
int countMaxOrSubsets(const std::vector<int>& nums) {
    // Maximum possible OR value: elements are <= 1e5 < 2^17, so max OR fits in 17 bits.
    const int MAX_OR_VALUE = 1 << 17;
    std::vector<int> dp(MAX_OR_VALUE, 0);
    dp[0] = 1;  // empty subset has OR = 0

    int currentMaxOr = 0;
    for (int num : nums) {
        // Iterate backwards to avoid using updated values from the same element.
        for (int i = currentMaxOr; i >= 0; --i) {
            dp[i | num] += dp[i];
        }
        currentMaxOr |= num;
    }

    return dp[currentMaxOr];
}

#include <cassert>
#include <vector>

// The function declaration (assumed to be in scope).
int countMaxOrSubsets(const std::vector<int>& nums);

int main() {
    // Basic case: all subsets have OR = 1, so 2^n subsets.
    assert(countMaxOrSubsets({1}) == 2);       // subsets: {}, {1} both OR = 1
    assert(countMaxOrSubsets({1, 1}) == 4);     // {}, {1}, {1}, {1,1} -> OR = 1 for all
    assert(countMaxOrSubsets({2, 2}) == 4);     // OR = 2 for all subsets

    // Mixed case: max OR = 3, subsets achieving it: {1,2}, {3}, {1,3}, {2,3}, {1,2,3} -> 5
    assert(countMaxOrSubsets({1, 2, 3}) == 5);

    // Duplicate with larger numbers: max OR = 7 (binary 111)
    // Subsets: {3,4}, {3,5}, {4,5}, {3,4,5}, plus {3,4,5}? Let's enumerate: 
    // Elements: 3 (011), 4 (100), 5 (101). Subsets with OR 7: {3,4} -> 7, {4,5} -> 7, {3,4,5} -> 7. Also {3,5}? 011|101=111->7. So 4 subsets.
    assert(countMaxOrSubsets({3, 4, 5}) == 4);

    // All zeros: max OR = 0, every subset qualifies (2^n)
    assert(countMaxOrSubsets({0, 0, 0}) == 8);

    // Single element zero: empty subset OR=0, {0} OR=0 -> 2 subsets
    assert(countMaxOrSubsets({0}) == 2);

    // Mixed with zero: [0, 1] -> max OR = 1, subsets: {}, {0} (OR=0), {1} (OR=1), {0,1} (OR=1) -> 2 subsets with OR=1
    assert(countMaxOrSubsets({0, 1}) == 2);

    // Larger test: [1, 2, 4] -> max OR = 7, subsets achieving 7: {1,2,4} only -> 1
    assert(countMaxOrSubsets({1, 2, 4}) == 1);

    // Multiple ways: [1, 2, 2] -> max OR = 3, subsets: {1,2a}, {1,2b}, {1,2a,2b} -> 3
    assert(countMaxOrSubsets({1, 2, 2}) == 3);

    // [1, 1, 2] -> max OR = 3, subsets: {1a,2}, {1b,2}, {1a,1b,2} -> 3
    assert(countMaxOrSubsets({1, 1, 2}) == 3);

    // Edge: large values up to 1e5 but below 2^17, e.g., 65536 (2^16) and 32768 (2^15) -> OR = 98304
    assert(countMaxOrSubsets({65536, 32768}) == 1); // only the full set gives the max OR

    return 0;
}
