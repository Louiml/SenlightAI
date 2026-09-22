Write a C++ function `std::vector<int> findRange(const std::vector<int>& nums, int target)` that takes a sorted (non-decreasing) integer vector and a target value, and returns a two‑element vector containing the first (leftmost) and last (rightmost) index where `target` appears in the array. If the target is not present, return `{-1, -1}`. The algorithm must run in \(O(\log n)\) time, where \(n\) is the number of elements. Handle edge cases such as an empty vector, a target smaller than all elements, larger than all elements, or appearing exactly once (in which case both indices are the same).

// The problem reduces to performing two binary searches: one to find the leftmost occurrence, and one to find the rightmost occurrence.  
// - For the leftmost index, we use a lower‑bound search that finds the smallest index `l` such that `nums[l] >= target`. Standard binary search returns such an index even if `target` is absent; after the search, we check if `nums[l] == target` to confirm presence.  
// - For the rightmost index, we perform an upper‑bound variant that finds the largest index `r` such that `nums[r] <= target`. After the search, verify `nums[r] == target`.  
// If the leftmost search returns `-1` (target absent), we can immediately return `{-1, -1}` without running the second search.  
// Edge cases:  
// - Empty vector: return `{-1, -1}`.  
// - If the target is smaller than all elements, the leftmost search will point to index 0 but `nums[0] != target`, so return `{-1, -1}`.  
// - If the target is larger than all elements, both searches will eventually point to the last index but the value won’t match, again returning `{-1, -1}`.  
// - Duplicates: both searches work correctly because the lower‑bound search moves `high` down when `nums[mid] >= target`, and the upper‑bound search moves `low` up when `nums[mid] <= target`.  
// Time complexity: two binary searches, each \(O(\log n)\), so total \(O(\log n)\). Space complexity: \(O(1)\) auxiliary space (excluding the output vector).

#include <vector>

// Find the first and last index of a target in a sorted vector.
// Returns {-1, -1} if the target is absent.
std::vector<int> findRange(const std::vector<int>& nums, int target) {
    int n = static_cast<int>(nums.size());
    if (n == 0) {
        return {-1, -1};
    }

    // Binary search for leftmost occurrence (lower bound).
    int low = 0, high = n - 1;
    while (low < high) {
        int mid = low + (high - low) / 2;
        if (nums[mid] >= target) {
            high = mid;
        } else {
            low = mid + 1;
        }
    }
    int left = -1;
    if (nums[low] == target) {
        left = low;
    } else {
        return {-1, -1};
    }

    // Binary search for rightmost occurrence (upper bound).
    low = 0;
    high = n - 1;
    while (low < high) {
        int mid = low + (high - low + 1) / 2;
        if (nums[mid] <= target) {
            low = mid;
        } else {
            high = mid - 1;
        }
    }
    // At this point, nums[high] is guaranteed equal to target because left != -1.
    return {left, high};
}

#include <cassert>
#include <vector>

// Solution function declaration (implementation above).
std::vector<int> findRange(const std::vector<int>& nums, int target);

int main() {
    // Basic examples from the problem.
    assert(findRange({5, 7, 7, 8, 8, 10}, 8) == std::vector<int>({3, 4}));
    assert(findRange({5, 7, 7, 8, 8, 10}, 6) == std::vector<int>({-1, -1}));

    // Single element present.
    assert(findRange({1}, 1) == std::vector<int>({0, 0}));

    // Single element absent.
    assert(findRange({1}, 0) == std::vector<int>({-1, -1}));

    // All elements identical.
    assert(findRange({4, 4, 4, 4}, 4) == std::vector<int>({0, 3}));

    // Target at the very beginning and very end.
    assert(findRange({1, 2, 3, 4, 5}, 1) == std::vector<int>({0, 0}));
    assert(findRange({1, 2, 3, 4, 5}, 5) == std::vector<int>({4, 4}));

    // Empty vector.
    assert(findRange({}, 1) == std::vector<int>({-1, -1}));

    // Target smaller than all elements.
    assert(findRange({2, 3, 4}, 1) == std::vector<int>({-1, -1}));

    // Target larger than all elements.
    assert(findRange({2, 3, 4}, 5) == std::vector<int>({-1, -1}));

    // Duplicates with target at both ends of a larger array.
    assert(findRange({1, 1, 2, 2, 2, 3, 3}, 2) == std::vector<int>({2, 4}));

    return 0;
}
