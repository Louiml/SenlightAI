Write a C++ function `int min_removals_no_adjacent_equal(const std::vector<int>& nums)` that returns the minimum number of elements that must be removed from `nums` so that the remaining elements can be permuted into a sequence where no two equal values are adjacent. Reordering is allowed. For example, from `{1,1,2,2,3}` you can keep all five (e.g., as `1,2,1,2,3`), so return 0. From `{1,1,1,2}`, you can keep at most three (e.g., `1,2,1`), so return 1. The input vector may contain any integers, including negatives and duplicates, and may be empty (return 0 for empty input).
// The key observation is that since we can arbitrarily reorder the kept elements, the only constraint against keeping all elements is the frequency of the most common value. Let `n` be the total count and `maxf` be the highest frequency of any single value. If `maxf <= (n+1)/2`, then it is always possible to arrange all `n` elements so that no two equal are adjacent: place the most frequent values at positions 1, 3, 5, ... and fill the even positions with the remaining values. If `maxf > (n+1)/2`, then even the most efficient interleaving cannot separate all copies of that value; the maximum keepable length is `2*(n - maxf) + 1`. To see why, imagine placing the frequent value `v` (count `maxf`) as "separators" requiring at least `maxf-1` other elements between them; you have only `n - maxf` other elements, so you can create at most `(n - maxf) + 1` gaps for the frequent value, yielding at most `2*(n - maxf) + 1` elements in total. Removing the excess gives `removals = n - (2*(n - maxf) + 1) = 2*maxf - n - 1`. If this value is negative, the answer is 0. Therefore, the answer is `max(0, 2*maxf - n - 1)`. The algorithm counts frequencies with a hash map, finds `maxf`, computes `n`, and returns the formula. Time complexity is O(n) and extra space O(number of distinct values), which is at most O(n).
#include <vector>
#include <unordered_map>
#include <algorithm>

// Return the minimum number of elements to remove so that the remaining
// multiset can be arranged with no two equal adjacent values.
int min_removals_no_adjacent_equal(const std::vector<int>& nums) {
    if (nums.empty()) return 0;

    std::unordered_map<int, int> freq;
    int maxf = 0;
    for (int v : nums) {
        maxf = std::max(maxf, ++freq[v]);
    }

    int n = static_cast<int>(nums.size());
    int removals = 2 * maxf - n - 1;
    return removals > 0 ? removals : 0;
}
#include <cassert>
#include <vector>

int min_removals_no_adjacent_equal(const std::vector<int>& nums); // declared from solution

int main() {
    assert(min_removals_no_adjacent_equal({}) == 0);
    assert(min_removals_no_adjacent_equal({5}) == 0);
    assert(min_removals_no_adjacent_equal({1,2,3,4}) == 0);
    assert(min_removals_no_adjacent_equal({1,1,2,2,3}) == 0);
    assert(min_removals_no_adjacent_equal({1,1,1,2}) == 1);
    assert(min_removals_no_adjacent_equal({1,1,1,1}) == 3);
    assert(min_removals_no_adjacent_equal({1,1,1,1,2}) == 2);
    assert(min_removals_no_adjacent_equal({7,7,8,8,9,9,9}) == 1);
    return 0;
}
