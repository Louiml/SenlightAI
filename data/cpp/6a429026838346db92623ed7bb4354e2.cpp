// Write a C++ function `bool containsDuplicateWithinDistance(const std::vector<int>& nums, int k)` that returns `true` if there exist two distinct indices `i` and `j` in the vector such that `nums[i] == nums[j]` and `abs(i - j) <= k`, and `false` otherwise. The distance `k` is a non-negative integer. The function must handle an empty vector (return `false`), a single element (return `false`), duplicate values that are farther than `k` apart (return `false`), and duplicates exactly `k` apart (return `true`). Assume `k >= 0` and the vector size can be up to 10^5, so the algorithm must run in O(n) time on average.
// The most efficient approach uses a hash map (unordered_map) to store the most recent index of each value encountered as we iterate through the vector from left to right. For each element `nums[i]`, we check if the map already contains that value. If it does, we compare the difference between the current index `i` and the stored index (which is the last seen position) against `k`. If the difference is less than or equal to `k`, we immediately return `true`. After the check (whether the value existed or not), we update the map entry for that value to the current index `i`. This ensures that for any value, we always compare with its most recent occurrence, because any earlier occurrence would be even farther away and thus less likely to satisfy the distance constraint.
//
// Edge cases include an empty vector (loop never runs, return `false`), a vector with only one element (no pair, return `false`), and duplicate values that appear at indices differing by more than `k` (we update the index each time, so the latest occurrence is always used). Since the map stores the most recent index, even if there are multiple duplicates, we correctly detect whether any pair is within distance `k`. Time complexity is O(n) on average because each lookup and insertion in an unordered_map is O(1) on average. Space complexity is O(n) in the worst case where all elements are distinct, as the map stores up to n entries.
#include <vector>
#include <unordered_map>
#include <cstdlib> // for std::abs

// Returns true if there exist two distinct indices i and j such that
// nums[i] == nums[j] and abs(i - j) <= k.
bool containsDuplicateWithinDistance(const std::vector<int>& nums, int k) {
    std::unordered_map<int, int> lastSeenIndex;
    for (int i = 0; i < static_cast<int>(nums.size()); ++i) {
        auto it = lastSeenIndex.find(nums[i]);
        if (it != lastSeenIndex.end()) {
            if (i - it->second <= k) {
                return true;
            }
        }
        lastSeenIndex[nums[i]] = i;
    }
    return false;
}
#include <cassert>
#include <vector>

int main() {
    // Empty vector
    assert(containsDuplicateWithinDistance({}, 3) == false);

    // Single element
    assert(containsDuplicateWithinDistance({42}, 0) == false);

    // Duplicates within distance
    assert(containsDuplicateWithinDistance({1, 2, 3, 1}, 3) == true);
    assert(containsDuplicateWithinDistance({1, 2, 3, 1}, 2) == false); // distance is 3 > 2
    assert(containsDuplicateWithinDistance({1, 0, 1, 1}, 1) == true); // indices 0 and 2 are 2 apart, 2 and 3 are 1 apart

    // Duplicates exactly at distance k
    assert(containsDuplicateWithinDistance({1, 2, 1}, 2) == true);
    assert(containsDuplicateWithinDistance({1, 2, 3, 1}, 3) == true);

    // k = 0: only possible if same index, but we require distinct indices, so false
    assert(containsDuplicateWithinDistance({5, 5}, 0) == false);

    // Large gap
    assert(containsDuplicateWithinDistance({1, 2, 3, 4, 1}, 3) == false); // distance 4 > 3
    assert(containsDuplicateWithinDistance({1, 2, 3, 4, 1}, 4) == true);

    // All distinct
    assert(containsDuplicateWithinDistance({1, 2, 3, 4, 5}, 10) == false);

    // Negative numbers and zeros
    assert(containsDuplicateWithinDistance({-1, 0, -1}, 2) == true);
    assert(containsDuplicateWithinDistance({0, 0, 0}, 1) == true); // first two are 0 apart? no, indices 0 and 1 distance 1

    return 0;
}
