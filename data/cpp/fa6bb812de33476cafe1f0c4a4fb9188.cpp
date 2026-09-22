// Write a C++ function named `findAllOccurrences` that takes a vector of integers (`std::vector<int>`) as input, along with a target integer to search for, and returns a `std::vector<int>` containing the indices (positions) where the target appears in the input vector, in ascending order. If the target is not found, the function should return an empty vector. The function must handle edge cases such as an empty input vector, duplicate occurrences of the target, and negative/zero values in the input. The function should be const-correct and should not modify the input. You may assume the input vector is not extremely large such that index values fit in `int`.

// The solution uses a simple sequential (linear) search algorithm: iterate through each element of the input vector from index 0 to size-1. For each element, check if it equals the target; if so, append the current index to a result vector. At the end, return the result vector. Key edge cases: an empty input vector (loop never runs, returns empty vector), target not present (result remains empty), multiple occurrences (each matched index appended in order, so result is naturally sorted by increasing index). No special handling is needed for duplicate values or negative numbers since we only compare equality. Time complexity is O(n) where n is the size of the input vector, because we perform a single pass. Space complexity is O(k) for the result vector, where k is the number of occurrences (at most n), and O(1) auxiliary space besides that.

#include <vector>

// Return a vector of all indices where 'target' appears in 'values'.
// If 'target' is not present, return an empty vector.
std::vector<int> findAllOccurrences(const std::vector<int>& values, int target) {
    std::vector<int> indices;
    for (int i = 0; i < static_cast<int>(values.size()); ++i) {
        if (values[i] == target) {
            indices.push_back(i);
        }
    }
    return indices;
}

#include <cassert>
#include <vector>

// Assume the solution function is declared above or included.
// (Here, we declare it for the test file.)
std::vector<int> findAllOccurrences(const std::vector<int>& values, int target);

int main() {
    // Basic case with multiple occurrences
    std::vector<int> v1 = {3, 1, 4, 1, 5, 9, 2, 6, 1};
    assert(findAllOccurrences(v1, 1) == std::vector<int>({1, 3, 8}));
    
    // Target not found
    assert(findAllOccurrences(v1, 10) == std::vector<int>());
    
    // Empty input
    std::vector<int> v2;
    assert(findAllOccurrences(v2, 5) == std::vector<int>());
    
    // Single occurrence at the beginning
    std::vector<int> v3 = {7, 2, 3};
    assert(findAllOccurrences(v3, 7) == std::vector<int>({0}));
    
    // Single occurrence at the end
    std::vector<int> v4 = {2, 3, 7};
    assert(findAllOccurrences(v4, 7) == std::vector<int>({2}));
    
    // All elements equal to target
    std::vector<int> v5 = {4, 4, 4};
    assert(findAllOccurrences(v5, 4) == std::vector<int>({0, 1, 2}));
    
    // Negative numbers and zero
    std::vector<int> v6 = {-3, 0, -3, 2, -3};
    assert(findAllOccurrences(v6, -3) == std::vector<int>({0, 2, 4}));
    assert(findAllOccurrences(v6, 0) == std::vector<int>({1}));
    
    return 0;
}
