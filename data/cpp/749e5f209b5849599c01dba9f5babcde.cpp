Write a C++ function that takes a vector of integers and a target difference value, and determines whether there exist two distinct elements in the vector whose absolute difference equals the target. The function should return a boolean indicating whether such a pair exists. The input vector may be unsorted, may contain duplicate values, and the target difference is guaranteed to be non-negative. The function must handle edge cases such as an empty vector, a vector with fewer than two elements, and cases where multiple pairs satisfy the condition. The solution should be efficient for large inputs (up to 10^5 elements) and should not modify the original vector.
// The most efficient approach is to sort a copy of the input vector, then use the two-pointer technique with indices `i` and `j` starting at 0 and 1 respectively. Since the target is non-negative, we can compare `a[j] - a[i]` to the target: if the difference is less than the target, increment `j`; if it is greater, increment `i`; if it equals the target, we have found a valid pair. This works because after sorting, the difference between elements is non-decreasing as `j` increases and `i` stays fixed (or as `i` increases and `j` stays fixed), allowing us to scan all possible differences in linear time. Important edge cases: if the vector has fewer than two elements, return false immediately. If `i` ever equals `j`, we must increment `j` to avoid comparing an element with itself (the problem requires two distinct elements). Also, if `j` reaches the end and the difference is still less than the target, no further progress is possible from that `i`, so the loop should terminate when `j` exceeds the last index. Time complexity is O(n log n) due to sorting, and O(n) for the two-pointer scan, so overall O(n log n). Space complexity is O(n) for the sorted copy (or O(1) if we are allowed to modify the input, but the specification says not to modify it, so we use a copy).
#include <vector>
#include <algorithm>

// Returns true if there exist two distinct elements in arr whose absolute difference equals target.
bool hasPairWithDifference(const std::vector<int>& arr, int target) {
    // Need at least two distinct elements to compare.
    if (arr.size() < 2) {
        return false;
    }

    // Work on a sorted copy to keep the original vector unchanged.
    std::vector<int> sortedArr = arr;
    std::sort(sortedArr.begin(), sortedArr.end());

    int i = 0;
    int j = 1;
    const int n = static_cast<int>(sortedArr.size());

    while (j < n) {
        // Ensure we compare distinct indices.
        if (i == j) {
            ++j;
            continue;
        }

        int diff = sortedArr[j] - sortedArr[i];

        if (diff < target) {
            // Difference too small, move the right pointer forward.
            ++j;
        } else if (diff == target) {
            return true;
        } else {
            // Difference too large, move the left pointer forward.
            ++i;
        }
    }

    return false;
}
#include <cassert>
#include <vector>

int main() {
    // Basic positive case.
    std::vector<int> v1 = {5, 1, 2, 3, 6, 9};
    assert(hasPairWithDifference(v1, 2) == true);  // e.g., (1,3) or (3,5)

    // Negative case.
    assert(hasPairWithDifference(v1, 4) == false);

    // Duplicate values and target zero.
    std::vector<int> v2 = {4, 4, 4};
    assert(hasPairWithDifference(v2, 0) == true);  // two 4's diff 0

    // Single element.
    std::vector<int> v3 = {7};
    assert(hasPairWithDifference(v3, 0) == false);

    // Empty vector.
    std::vector<int> v4;
    assert(hasPairWithDifference(v4, 5) == false);

    // Unsorted with negative numbers.
    std::vector<int> v5 = {-3, 10, -1, 0, 5};
    assert(hasPairWithDifference(v5, 5) == true);  // -3 and 2? no, but -3 and -1? diff 2; 0 and5 diff5
    assert(hasPairWithDifference(v5, 13) == true); // -3 and 10

    // Large target that no pair can reach (but target is non-negative per spec).
    assert(hasPairWithDifference(v5, 100) == false);

    // Two elements that differ by target.
    std::vector<int> v6 = {2, 8};
    assert(hasPairWithDifference(v6, 6) == true);
    assert(hasPairWithDifference(v6, 7) == false);

    // Target zero with no duplicates.
    std::vector<int> v7 = {1, 2, 3};
    assert(hasPairWithDifference(v7, 0) == false);

    // Many elements, ensure the algorithm doesn't fall into an infinite loop.
    std::vector<int> v8(100000, 1);
    v8[50000] = 100000; // one big value
    assert(hasPairWithDifference(v8, 99999) == true);

    return 0;
}
