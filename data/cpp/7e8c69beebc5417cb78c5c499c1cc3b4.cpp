// You are given an array of `n` integers. Write a C++ function named `isFirstElementOne` that takes a `const std::vector<int>&` as input and returns a `bool` indicating whether the first element of the array equals `1`. The function must properly handle an empty array: if the array is empty, it should return `false`. Multiple test cases are not needed inside the function; the function should simply evaluate a single array. This task is meant to practice simple array indexing, const correctness, and edge-case handling.
#include <cassert>
#include <vector>

// Function declaration matches the solution definition.
bool isFirstElementOne(const std::vector<int>& a);

int main() {
    // Non-empty array with first element 1 -> true
    assert(isFirstElementOne({1, 5, 3}) == true);
    
    // Non-empty array with first element not 1 -> false
    assert(isFirstElementOne({2, 1, 1}) == false);
    
    // Single element equal to 1 -> true
    assert(isFirstElementOne({1}) == true);
    
    // Single element not equal to 1 -> false
    assert(isFirstElementOne({0}) == false);
    
    // Empty array -> false
    assert(isFirstElementOne({}) == false);
    
    // Larger array with first element 1 and repeated values -> true
    assert(isFirstElementOne({1, 1, 1, 1}) == true);
    
    // Array with negative values and first element not 1 -> false
    assert(isFirstElementOne({-3, 2, 1}) == false);
    
    return 0;
}
#include <vector>

// Returns true if the vector is non-empty and its first element equals 1.
// Returns false for an empty vector.
bool isFirstElementOne(const std::vector<int>& a) {
    if (a.empty()) {
        return false;
    }
    return a[0] == 1;
}
// The problem is straightforward: we need to check if the first element of the vector is equal to `1`. The main algorithm is to verify that the vector is not empty, and if it is not, compare `a[0]` with `1`. Edge cases include an empty array (where we return `false` since there is no first element) and an array with only one element (where we compare that element). Time complexity is O(1) because we only access the first element and check emptiness (which is constant-time for `std::vector`). Space complexity is O(1) as no extra storage is used. The solution uses `const` reference to avoid copying the vector and `const` correctness is applied to the function parameter.
