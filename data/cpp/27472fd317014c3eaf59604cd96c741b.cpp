Write a C++ function that determines whether any three distinct elements from a given vector of integers sum to a specified target value. The function should return `true` if at least one such triplet exists, and `false` otherwise. Inputs may include negative numbers, duplicates, and small or large vector sizes; the vector may be empty or contain fewer than three elements, in which case the answer must be `false`. The function must not modify the original vector.

#include <cassert>
#include <vector>

// Forward declaration (or include the solution header in practice).
bool hasTripletSum(const std::vector<int>& arr, long long target);

int main() {
    // Basic positive case.
    assert(hasTripletSum({1, 2, 3, 4}, 6) == true);  // 1+2+3
    // Negative numbers.
    assert(hasTripletSum({-1, 0, 1, 2}, 0) == true); // -1+0+1
    // Duplicates.
    assert(hasTripletSum({5, 5, 5}, 15) == true);   // 5+5+5
    // No triplet.
    assert(hasTripletSum({1, 2, 3}, 100) == false);
    // Fewer than 3 elements.
    assert(hasTripletSum({1, 2}, 3) == false);
    assert(hasTripletSum({}, 0) == false);
    // Large values requiring long long.
    assert(hasTripletSum({2000000000, 2000000000, 2000000000}, 6000000000LL) == true);
    // Negative target.
    assert(hasTripletSum({-10, -5, 2, 3}, -12) == true); // -10-5+3 = -12
    // Unsorted input.
    assert(hasTripletSum({7, 3, 1, 9}, 13) == true); // 3+1+9 = 13
    return 0;
}

#include <vector>
#include <algorithm>

// Returns true if any three distinct elements in arr sum to target.
bool hasTripletSum(const std::vector<int>& arr, long long target) {
    if (arr.size() < 3) return false;
    
    // Copy to allow sorting without modifying the original.
    std::vector<int> sorted = arr;
    std::sort(sorted.begin(), sorted.end());
    
    for (size_t i = 0; i + 2 < sorted.size(); ++i) {
        // Skip duplicate first elements to avoid unnecessary checks (optional).
        if (i > 0 && sorted[i] == sorted[i - 1]) continue;
        
        size_t left = i + 1;
        size_t right = sorted.size() - 1;
        long long remaining = target - static_cast<long long>(sorted[i]);
        
        while (left < right) {
            long long currentSum = static_cast<long long>(sorted[left]) + sorted[right];
            if (currentSum == remaining) {
                return true;
            } else if (currentSum < remaining) {
                ++left;
            } else {
                --right;
            }
        }
    }
    return false;
}

// The naive approach is a triple nested loop over all index combinations `(i, j, k)` with `i < j < k`, checking each sum against the target. This is simple and correct but runs in O(n³) time. For better efficiency, we can use a hash set: first, sort the array (or use a set) to enable a two-pointer technique, or use a hash set to check for the third value while iterating. A clean O(n²) approach sorts the vector first, then fixes one element and uses two pointers to find a pair summing to `target - fixed`. Edge cases: vectors with fewer than 3 elements, duplicates (which still allow distinct indices), and large values that might overflow `int`—so use `long long` for sums. Time complexity O(n²) after sorting, space O(1) auxiliary (excluding sorting overhead). Alternatively, a hash-set-based O(n²) solution also works with O(n) space.
