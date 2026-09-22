/*
Write a C++ function named `twoSumIndexes` that takes a vector of integers (`numbers`) and an integer (`target`) as input, and returns a vector of two integers representing the 1-based indices of the two numbers that sum to the target. The function must return the indices in ascending order (smaller index first, larger index second). You may assume that there is exactly one valid solution, and you cannot use the same element twice. The function should handle vectors with duplicate values and positive/negative numbers, and it should be efficient for large inputs. If no solution exists, return an empty vector. The function should be `const`-correct with respect to the input vector (i.e., it takes `const std::vector<int>&` and does not modify it).
*/
#include <vector>
#include <unordered_map>

// Returns 1-based indices of two numbers in `numbers` that sum to `target`.
// Assumes exactly one solution exists; returns empty vector if none found.
// The returned indices are in ascending order.
std::vector<int> twoSumIndexes(const std::vector<int>& numbers, int target) {
    std::unordered_map<int, int> value_to_index;
    for (int i = 0; i < static_cast<int>(numbers.size()); ++i) {
        int complement = target - numbers[i];
        auto it = value_to_index.find(complement);
        if (it != value_to_index.end()) {
            // it->second is the earlier index (0-based), i is current (0-based)
            return {it->second + 1, i + 1}; // convert to 1-based, ascending order
        }
        // Store current number's index (update if duplicate, but only used for future)
        value_to_index[numbers[i]] = i;
    }
    return {};
}
#include <cassert>
#include <vector>

int main() {
    // Basic case
    std::vector<int> v1 = {2, 7, 11, 15};
    assert(twoSumIndexes(v1, 9) == std::vector<int>({1, 2}));
    
    // Duplicate values but different indices (target uses the two distinct occurrences)
    std::vector<int> v2 = {3, 3, 4};
    assert(twoSumIndexes(v2, 6) == std::vector<int>({1, 2}));
    
    // Negative numbers and zero
    std::vector<int> v3 = {-3, 4, 3, 90};
    assert(twoSumIndexes(v3, 0) == std::vector<int>({1, 3}));
    
    // Pair not at the beginning
    std::vector<int> v4 = {1, 2, 3, 4, 5};
    assert(twoSumIndexes(v4, 7) == std::vector<int>({2, 5}));
    
    // Single-element vector with no solution
    std::vector<int> v5 = {5};
    assert(twoSumIndexes(v5, 10) == std::vector<int>());
    
    // Pair with one element used twice is not allowed (target = 2*element with single occurrence)
    std::vector<int> v6 = {4, 1, 2};
    assert(twoSumIndexes(v6, 8) == std::vector<int>());
    
    // Large target and values
    std::vector<int> v7 = {1000000, 3, 7, 2000000};
    assert(twoSumIndexes(v7, 1000003) == std::vector<int>({1, 2}));
    
    // Duplicate values where a later duplicate is used correctly
    std::vector<int> v8 = {2, 5, 5, 1};
    assert(twoSumIndexes(v8, 10) == std::vector<int>({2, 3}));
    
    // Empty vector
    std::vector<int> v9;
    assert(twoSumIndexes(v9, 1) == std::vector<int>());
    
    // Negative target
    std::vector<int> v10 = {-1, -2, -3, -4};
    assert(twoSumIndexes(v10, -5) == std::vector<int>({1, 4}));
    
    return 0;
}
// The classic two-pointer approach requires sorting, which destroys index information. Instead, use a hash map (e.g., `std::unordered_map`) to store each number's most recent index while iterating once. As we traverse the array, for each element `numbers[i]`, compute the complement `target - numbers[i]`. If the complement exists in the map, we have found a pair. Since we only store indices of previously visited elements, the stored index is always less than the current index, so the indices are naturally in ascending order. This approach handles duplicates correctly: if a duplicate appears later, the map's value is updated to the latest index, but we only check the map before inserting the current element, ensuring we never pair an element with itself. Edge cases: an empty vector or no valid pair returns an empty vector; negative numbers and zero are handled naturally by arithmetic. Time complexity is O(n) for a single pass with average O(1) hash map operations, and space complexity is O(n) for the map. The map stores up to n entries in the worst case (when no pair is found until the end).
