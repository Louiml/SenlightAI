Given a sorted array of integers in non-decreasing order and a target integer, write a C++ function `findRange` that returns a `std::vector<int>` containing the starting and ending positions (0-based indices) of all occurrences of the target in the array. If the target is not present, return `{-1, -1}`. The function must run in O(log n) time and handle edge cases such as an empty array, a single-element array, all elements equal to the target, and the target appearing at the very beginning or end of the array. You may use binary search to locate one occurrence of the target, then expand outward linearly, but the linear expansion could degrade to O(n) in the worst case (e.g., all elements equal). Instead, design the solution using two binary searches: one to find the first occurrence and one to find the last occurrence. The function must be `const`-correct, take the vector by `const std::vector<int>&`, and return a `std::vector<int>`. Include necessary headers.

The solution uses two binary searches. The first binary search finds the leftmost index of the target. We maintain `low` and `high` pointers (inclusive range). While `low <= high`, compute `mid = low + (high - low) / 2`. If `nums[mid] >= target`, we move `high = mid - 1`; otherwise, `low = mid + 1`. After the loop, `low` is the first index where `nums[low] == target` if it exists. If `low` is out of range or `nums[low] != target`, return `{-1, -1}`. The second binary search finds the rightmost index: use the same pattern but move `low = mid + 1` when `nums[mid] <= target`. After the loop, `high` is the last index where `nums[high] == target`. Return `{left, right}`. This runs in O(log n) time and O(1) auxiliary space. Edge cases: empty array (return `{-1, -1}` immediately), target not present (check after first binary search), and when target appears once (left == right). The function avoids integer overflow by using `low + (high - low) / 2`.

#include <vector>

// Find the first and last occurrence of target in a sorted array.
// Returns a vector {left, right} or {-1, -1} if target not found.
std::vector<int> findRange(const std::vector<int>& nums, int target) {
    int n = nums.size();
    int left = 0, right = n - 1;
    int first = -1, last = -1;

    // Binary search for first occurrence.
    left = 0;
    right = n - 1;
    while (left <= right) {
        int mid = left + (right - left) / 2;
        if (nums[mid] >= target) {
            right = mid - 1;
        } else {
            left = mid + 1;
        }
    }
    if (left >= n || nums[left] != target) {
        return {-1, -1};
    }
    first = left;

    // Binary search for last occurrence.
    left = 0;
    right = n - 1;
    while (left <= right) {
        int mid = left + (right - left) / 2;
        if (nums[mid] <= target) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }
    last = right;

    return {first, last};
}

#include <cassert>
#include <vector>

int main() {
    std::vector<int> empty;
    assert(findRange(empty, 5) == std::vector<int>({-1, -1}));

    std::vector<int> single{7};
    assert(findRange(single, 7) == std::vector<int>({0, 0}));
    assert(findRange(single, 6) == std::vector<int>({-1, -1}));

    std::vector<int> allSame{3, 3, 3, 3};
    assert(findRange(allSame, 3) == std::vector<int>({0, 3}));

    std::vector<int> normal{1, 2, 2, 2, 3, 4, 5};
    assert(findRange(normal, 2) == std::vector<int>({1, 3}));
    assert(findRange(normal, 4) == std::vector<int>({5, 5}));
    assert(findRange(normal, 9) == std::vector<int>({-1, -1}));

    std::vector<int> atEdges{2, 3, 4, 4};
    assert(findRange(atEdges, 2) == std::vector<int>({0, 0}));
    assert(findRange(atEdges, 4) == std::vector<int>({2, 3}));

    return 0;
}
