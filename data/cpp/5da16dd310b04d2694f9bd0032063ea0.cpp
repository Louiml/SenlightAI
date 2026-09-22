// Write a C++ function that takes a non-empty vector of integers where every element appears exactly twice except for one element that appears exactly once, and returns that unique element. The input vector may contain negative numbers, zeros, large positive values, and the unique element can appear anywhere in the vector. The function should work for vectors of any size greater than zero.
// The straightforward approach is to count the frequency of each number using a hash map (e.g., `std::unordered_map<int, int>`). First, iterate through the entire vector and increment the count for each element. Then, iterate through the hash map and return the first key whose associated value is exactly 1. Edge cases include a vector of size 1 (the single element is the answer immediately) and the possibility of negative numbers or zeros, which are handled naturally by the hash map since it uses integer keys. The time complexity is O(n) for the counting pass plus O(n) for the map scan, so overall O(n). The space complexity is O(n) because in the worst case all elements are distinct except for the duplicates, but since each element except one appears twice, the number of unique keys is approximately n/2, so space is O(n). No special handling for large negative or positive values is needed because the hash map supports arbitrary integers. The algorithm is correct because the problem guarantees exactly one element with an odd frequency of 1.
#include <vector>
#include <unordered_map>

// Return the element that appears exactly once in a vector where all other elements appear twice.
int findUniqueElement(const std::vector<int>& nums) {
    std::unordered_map<int, int> frequency;

    // Count occurrences of each number
    for (int num : nums) {
        ++frequency[num];
    }

    // Find the number with count 1
    for (const auto& pair : frequency) {
        if (pair.second == 1) {
            return pair.first;
        }
    }

    // Should never reach here given problem constraints
    return 0;
}
#include <cassert>
#include <vector>

int main() {
    // Basic case with positives
    std::vector<int> v1 = {2, 2, 1};
    assert(findUniqueElement(v1) == 1);

    // Case with negatives and zero
    std::vector<int> v2 = {0, -5, -5, 0, 7};
    assert(findUniqueElement(v2) == 7);

    // Single element vector
    std::vector<int> v3 = {42};
    assert(findUniqueElement(v3) == 42);

    // Unique element at the beginning
    std::vector<int> v4 = {9, 3, 3, 8, 8};
    assert(findUniqueElement(v4) == 9);

    // Unique element at the end and large values
    std::vector<int> v5 = {1000000, -1000000, 1000000, -1000000, 12345};
    assert(findUniqueElement(v5) == 12345);

    // All duplicates except one, mixed order
    std::vector<int> v6 = {4, 1, 2, 1, 2};
    assert(findUniqueElement(v6) == 4);

    // Duplicate values that are not adjacent
    std::vector<int> v7 = {-1, -1, -2, -2, -3, -3, -4, -4, -5};
    assert(findUniqueElement(v7) == -5);

    // Zero as the unique element
    std::vector<int> v8 = {1, 1, 2, 2, 0};
    assert(findUniqueElement(v8) == 0);

    return 0;
}
