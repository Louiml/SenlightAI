/*
Write a C++ function `int minSetSize(std::vector<int>& arr)` that, given an array of integers, returns the minimum size of a set of distinct values that must be removed from the array so that at least half of the original array's elements are removed. The set size is the number of distinct integers chosen, not the total count of removed elements. For each distinct value, you may remove all occurrences of that value. The function should work for arrays with up to 10^5 elements containing values in the range [-10^9, 10^9]. For example, if `arr = [3,3,3,3,3,4,4,4,5,5,5,1]`, removing values `{3,4}` removes 5+3=8 elements (more than half of 12), so the answer is 2, but removing only `{3}` removes 5 elements (less than half), so 2 is minimal.
*/

#include <vector>
#include <unordered_map>
#include <map>
#include <algorithm>

// Returns the minimum number of distinct values to remove so that
// at least half of the array's elements are removed.
int minSetSize(std::vector<int>& arr) {
    const int n = static_cast<int>(arr.size());
    const int required = (n + 1) / 2;  // ceiling of n/2

    std::unordered_map<int, int> freq;
    for (int val : arr) {
        ++freq[val];
    }

    // Map from frequency -> number of distinct values having that frequency,
    // sorted in descending order of frequency.
    std::map<int, int, std::greater<int>> count_of_freq;
    for (const auto& [value, f] : freq) {
        ++count_of_freq[f];
    }

    int removed = 0;
    int set_size = 0;
    for (const auto& [f, num_values] : count_of_freq) {
        if (removed >= required) break;
        int still_needed = required - removed;
        // Number of values needed at this frequency, but not exceeding available
        int take = std::min(num_values, (still_needed + f - 1) / f);
        set_size += take;
        removed += take * f;
    }
    return set_size;
}

#include <cassert>
#include <vector>

int main() {
    // Basic cases
    std::vector<int> arr1 = {3,3,3,3,3,4,4,4,5,5,5,1};
    assert(minSetSize(arr1) == 2);

    // All same
    std::vector<int> arr2 = {7,7,7,7};
    assert(minSetSize(arr2) == 1);

    // Single element
    std::vector<int> arr3 = {5};
    assert(minSetSize(arr3) == 1);

    // Already need half exactly
    std::vector<int> arr4 = {1,2,3,4};
    assert(minSetSize(arr4) == 2);

    // Odd size: need ceil(5/2)=3 removals
    std::vector<int> arr5 = {1,1,2,2,3};
    // frequencies: 1->2, 2->2, 3->1. Choose 1 and 2 -> 4 removals >=3, size 2.
    assert(minSetSize(arr5) == 2);

    // Larger test
    std::vector<int> arr6 = {1,1,1,1,1,2,2,2,2,3,3,3,4,4,5};
    // n=15, required=8. Frequencies: 1->5, 2->4, 3->3, 4->2, 5->1
    // Take 1 (5), then 2 (4) -> total 9, size 2.
    assert(minSetSize(arr6) == 2);

    // Negative values
    std::vector<int> arr7 = {-1,-1,-1,-2,-2,-3};
    // n=6, required=3. Frequencies: -1->3, -2->2, -3->1. Take -1 alone gives 3.
    assert(minSetSize(arr7) == 1);

    // All distinct, need half
    std::vector<int> arr8 = {10,20,30,40,50,60};
    assert(minSetSize(arr8) == 3);

    // Many duplicates
    std::vector<int> arr9 = {5,5,5,5,5,5,5,5,1,2,3,4};
    // n=12, required=6. freq 8 for 5, take one -> 8 >=6, size 1.
    assert(minSetSize(arr9) == 1);

    // Edge: exactly half
    std::vector<int> arr10 = {9,9,9,9,1,2};
    // n=6, required=3. freq 4 for 9, take one -> 4>=3, size 1.
    assert(minSetSize(arr10) == 1);

    return 0;
}

// The goal is to remove the fewest distinct integers such that the total count of removed elements is at least `n/2` (where n = arr.size()). The optimal strategy is to always remove the values with the highest frequencies first, since each chosen value contributes its full count to the total removed. Therefore, we count frequencies of each distinct value using an unordered_map. Then, we group distinct values by their frequency using a `map<int,int,greater<int>>` (sorted descending by frequency) so that we can process frequencies from largest to smallest. For each frequency `f`, we may need to choose some values (all having that same frequency) to reach the remaining target. The number of such values needed at that frequency is `ceil(remaining / f)`, but capped by the available count of values with that frequency. We add that count to the result set size, update the total removed, and stop when total >= half. Edge cases: if all values are the same, we need exactly 1 set element; if the array has length 1, half is 0.5, but we need at least 1 element removed (since integer division gives 0), but the requirement is "at least half" meaning `ceil(n/2)`? Actually the problem statement says "at least half" so we need `total >= (n+1)/2` (integer ceiling). The original code uses `n = arr.size()/2` which is floor, but that is a bug; we must use ceiling. However, the provided snippet uses floor, but typical LeetCode problem "Reduce Array Size to The Half" uses exactly half, so for even sizes it's fine, but for odd sizes you need to remove at least ceil(n/2). We'll define "at least half" as `total >= (n+1)/2` to be safe. Time complexity: O(n) to count frequencies, plus O(d log d) where d is number of distinct values (due to sorting in map). Space: O(d).
