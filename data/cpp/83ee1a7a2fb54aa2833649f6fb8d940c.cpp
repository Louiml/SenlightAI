/*
Write a standalone C++ function that, given a sorted vector of integers and a target value, returns the total number of occurrences of that target in the vector. The vector is sorted in non-decreasing order, and may contain duplicates. The function must use binary search to achieve efficient lookup; specifically, it should locate the first position where the target appears (using lower bound semantics) and then count consecutive equal elements until a different value or the end of the vector is reached. Your function must be `const`‑correct (i.e., it should not modify the input vector), and should handle edge cases such as an empty vector, a target not present, or a target that appears at the very beginning or end of the vector.
*/
#include <vector>
#include <algorithm>

// Returns the total number of occurrences of 'target' in the sorted vector 'nums'.
// The vector is assumed to be non-decreasing and is not modified.
int countOccurrences(const std::vector<int>& nums, int target) {
    // Find the first position where target could be inserted while maintaining sorted order.
    auto first = std::lower_bound(nums.begin(), nums.end(), target);
    
    int count = 0;
    // Iterate from that position while the value equals target.
    for (auto it = first; it != nums.end(); ++it) {
        if (*it != target) {
            break;
        }
        ++count;
    }
    return count;
}
#include <cassert>
#include <vector>

// Solution function declaration (already defined above).
int countOccurrences(const std::vector<int>& nums, int target);

int main() {
    // Basic cases
    std::vector<int> v1 = {1, 2, 2, 3, 3, 3, 4};
    assert(countOccurrences(v1, 3) == 3);
    assert(countOccurrences(v1, 2) == 2);
    assert(countOccurrences(v1, 1) == 1);
    assert(countOccurrences(v1, 4) == 1);
    assert(countOccurrences(v1, 5) == 0);

    // Target not present and smaller than all elements
    std::vector<int> v2 = {10, 20, 30};
    assert(countOccurrences(v2, 5) == 0);
    assert(countOccurrences(v2, 15) == 0);
    assert(countOccurrences(v2, 35) == 0);

    // Empty vector
    std::vector<int> v3;
    assert(countOccurrences(v3, 0) == 0);

    // All elements equal to target
    std::vector<int> v4 = {7, 7, 7, 7};
    assert(countOccurrences(v4, 7) == 4);

    // Single element vector
    std::vector<int> v5 = {100};
    assert(countOccurrences(v5, 100) == 1);
    assert(countOccurrences(v5, 99) == 0);

    // Negative numbers and duplicates
    std::vector<int> v6 = {-5, -5, -1, 0, 2, 2, 2, 9};
    assert(countOccurrences(v6, -5) == 2);
    assert(countOccurrences(v6, 2) == 3);
    assert(countOccurrences(v6, 0) == 1);

    return 0;
}
// The core idea is to exploit the sorted property of the input vector. We first use `std::lower_bound` to find an iterator to the first element that is not less than `target`. This iterator either points to the first occurrence of `target` (if it exists) or to the first element greater than `target` (if it does not exist). After obtaining that iterator, we iterate forward as long as the value equals `target`, incrementing a counter. We stop when we reach the end of the vector or encounter a value different from `target`. Because the input is sorted, all duplicates are contiguous, so this counting loop visits exactly the number of occurrences of `target`. Edge cases: an empty vector immediately yields a count of zero; a target not present yields a lower bound pointing either to a larger value (and the loop does not run) or to `end()`; a target that is the smallest element works normally; a target that appears at the end is handled because the loop terminates at `end()`. Time complexity is \(O(\log n + k)\), where \(n\) is the size of the vector and \(k\) is the number of occurrences of the target. In the worst case (all elements equal to target), this is \(O(n)\), but for typical inputs with few duplicates it is logarithmic plus a small constant. Space complexity is \(O(1)\) auxiliary, ignoring the input storage.
