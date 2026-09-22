/*
Given an array of `n` positive integers where exactly one integer appears exactly once and all other integers appear at least twice (possibly more), write a C++ function `int find_unique_position(const std::vector<int>& a)` that returns the 1-based index of the unique element. If no such unique element exists (i.e., every element appears at least twice), return `-1`. The input vector may contain duplicate values, and the unique element may appear anywhere. For example, given `[5, 3, 5, 5, 5]`, the unique element is `3` at position `2`, so return `2`. Given `[1,1,2,2]`, return `-1`. The function must work efficiently for arrays up to length 10^5.
*/

#include <vector>
#include <unordered_map>

// Returns the 1-based index of the unique element in the array,
// or -1 if there is no unique element (every element repeats).
int find_unique_position(const std::vector<int>& a) {
    if (a.empty()) return -1;
    
    std::unordered_map<int, int> freq;
    for (int value : a) {
        ++freq[value];
    }
    
    int unique_value = -1;
    bool found = false;
    for (const auto& pair : freq) {
        if (pair.second == 1) {
            unique_value = pair.first;
            found = true;
            break;
        }
    }
    
    if (!found) return -1;
    
    for (int i = 0; i < static_cast<int>(a.size()); ++i) {
        if (a[i] == unique_value) {
            return i + 1; // 1-based index
        }
    }
    
    return -1; // Should never reach here if unique_value exists
}

#include <cassert>
#include <vector>

int find_unique_position(const std::vector<int>& a); // declaration for testing

int main() {
    // Basic case with unique element in the middle
    assert(find_unique_position({5, 3, 5, 5, 5}) == 2);
    // Unique element at the beginning
    assert(find_unique_position({7, 2, 2, 2}) == 1);
    // Unique element at the end
    assert(find_unique_position({4, 4, 4, 9}) == 4);
    // Single element
    assert(find_unique_position({42}) == 1);
    // No unique element (all duplicates)
    assert(find_unique_position({1, 1, 2, 2}) == -1);
    // Three same values, no unique
    assert(find_unique_position({8, 8, 8}) == -1);
    // Larger array with multiple duplicates and one unique
    assert(find_unique_position({2, 3, 2, 4, 3, 2, 4}) == 1);
    // Empty array
    assert(find_unique_position({}) == -1);
    return 0;
}

// The straightforward approach is to count frequencies of each value using a hash map (`std::unordered_map`), then find the value whose frequency is exactly 1. After identifying that value, scan the original array to find its first occurrence and return its 1-based index. If no value has frequency 1, return `-1`. Key edge cases: (1) empty array — return `-1` (though constraints say positive integers, still handle it); (2) single element — that element is unique, index 1; (3) all elements duplicated — return `-1`. The approach uses two linear passes: one to build the frequency map (`O(n)`), and one to find the first occurrence of the unique value (`O(n)`). Space complexity is `O(k)` where `k` is the number of distinct values, up to `O(n)` in the worst case. Sorting-based alternative would be `O(n log n)` but is less efficient; hash map is preferred.
