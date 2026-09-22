// Write a C++ function that accepts a vector of integers and returns `true` if any value appears at least twice in the vector, and `false` if every element is distinct. The function must handle empty vectors (returning `false`), vectors with negative values, and large vectors efficiently. The input vector should not be modified, and the solution should not rely on external libraries beyond the C++ standard library.

// The simplest robust approach is to sort a copy of the input vector and then scan adjacent pairs. After sorting, all duplicate values become adjacent, so a single pass comparing each element to its predecessor detects any repetition. If no adjacent equal pairs are found, all elements are distinct. Sorting handles all edge cases uniformly: an empty vector contains no duplicates; a single-element vector trivially has no duplicates; negative values sort normally; and large vectors are handled in \(O(n \log n)\) time. An alternative using a hash set would give \(O(n)\) average time but requires extra allocation and is less cache-friendly. The sorting approach uses \(O(n)\) auxiliary space for the copy and \(O(1)\) extra space during the scan. Time complexity is dominated by sorting: \(O(n \log n)\). The function is marked `const` on the vector reference to guarantee the caller's data is unchanged.

#include <vector>
#include <algorithm>

// Returns true if any value in the input vector appears more than once.
bool containsDuplicate(const std::vector<int>& nums) {
    std::vector<int> sorted = nums;
    std::sort(sorted.begin(), sorted.end());
    for (std::size_t i = 1; i < sorted.size(); ++i) {
        if (sorted[i] == sorted[i - 1]) {
            return true;
        }
    }
    return false;
}

#include <cassert>
#include <vector>

// Assume containsDuplicate is defined above.

int main() {
    // Empty vector
    assert(containsDuplicate({}) == false);
    
    // Single element
    assert(containsDuplicate({42}) == false);
    
    // All distinct
    assert(containsDuplicate({1, 2, 3, 4, 5}) == false);
    
    // Simple duplicate
    assert(containsDuplicate({1, 2, 3, 1}) == true);
    
    // Negative and duplicate
    assert(containsDuplicate({-1, -2, -3, -2}) == true);
    
    // All same value
    assert(containsDuplicate({7, 7, 7, 7}) == true);
    
    // Large vector with duplicates at ends
    std::vector<int> big;
    for (int i = 0; i < 10000; ++i) big.push_back(i);
    big.push_back(0); // duplicate
    assert(containsDuplicate(big) == true);
    
    // Large vector all distinct
    std::vector<int> big2;
    for (int i = 0; i < 10000; ++i) big2.push_back(i);
    assert(containsDuplicate(big2) == false);
    
    // Duplicate negative values
    assert(containsDuplicate({-5, -5, 0, 10}) == true);
    
    // Values that are negative-zero edge case
    assert(containsDuplicate({0, -0}) == true);
    
    return 0;
}
