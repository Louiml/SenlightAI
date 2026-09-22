// Write a C++ function `isValidMountain` that takes a vector of integers and returns `true` if the sequence forms a valid "mountain" array, and `false` otherwise. A valid mountain array must have at least 3 elements, strictly increase from the start to a single peak, then strictly decrease to the end. The peak cannot be the first or last element. For example, `[0,3,2,1]` is valid, but `[1,2,2]`, `[2,1]`, and `[0,1,2,3]` are not. Your function should be efficient and handle empty or small arrays gracefully.
#include <cassert>
#include <vector>

int main() {
    // Valid mountains
    assert(isValidMountain({0, 3, 2, 1}) == true);
    assert(isValidMountain({1, 2, 3, 2, 1}) == true);
    assert(isValidMountain({5, 9, 7, 3, 1}) == true);
    
    // Invalid: too short
    assert(isValidMountain({}) == false);
    assert(isValidMountain({1}) == false);
    assert(isValidMountain({1, 2}) == false);
    
    // Invalid: no peak (strictly increasing or decreasing)
    assert(isValidMountain({1, 2, 3, 4}) == false);
    assert(isValidMountain({4, 3, 2, 1}) == false);
    
    // Invalid: plateau or equal adjacent values
    assert(isValidMountain({1, 2, 2, 1}) == false);
    assert(isValidMountain({1, 1, 1}) == false);
    
    // Invalid: peak at first or last element
    assert(isValidMountain({5, 4, 3, 2}) == false);
    assert(isValidMountain({2, 3, 4, 5}) == false);
    
    // Invalid: multiple peaks or non-strict slopes
    assert(isValidMountain({1, 3, 2, 4, 3}) == false);
    
    return 0;
}
#include <vector>

// Checks whether the given vector forms a valid mountain array.
// A mountain array has length >= 3, strictly increases to a peak,
// then strictly decreases. The peak cannot be the first or last element.
bool isValidMountain(const std::vector<int>& arr) {
    int n = static_cast<int>(arr.size());
    if (n < 3) {
        return false;
    }

    int left = 0;
    int right = n - 1;

    // Move left while strictly increasing (but stop before the last element).
    while (left + 1 < n - 1 && arr[left] < arr[left + 1]) {
        ++left;
    }

    // Move right while strictly decreasing (but stop before the first element).
    while (right - 1 > 0 && arr[right] < arr[right - 1]) {
        --right;
    }

    // Valid if both pointers meet at the same peak index.
    return left == right;
}
// The approach uses two pointers, one starting from the left and moving right while the array is strictly increasing, and one starting from the right and moving left while the array is strictly decreasing. If both pointers meet at the same index, that index is the unique peak, and the array is a valid mountain. Key edge cases:  
// - Arrays with fewer than 3 elements cannot be mountains.  
// - The peak must not be at either end — the left pointer stops before the last element, and the right pointer stops before the first element, ensuring strict slopes on both sides.  
// - If the array is strictly increasing or strictly decreasing, the pointers will not meet, returning `false`.  
// - Duplicate adjacent values break the strictness, causing the pointers to stop early and not meet (unless they coincide, but that would imply a plateau, which is invalid).  
// Time complexity is O(n) in the worst case (each pointer moves at most n steps total), and space complexity is O(1).
