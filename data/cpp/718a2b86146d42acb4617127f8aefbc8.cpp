Write a C++ function named `normalizeAndSortVector` that takes a `std::vector<double>` by non-const reference. The function should first remove any negative values from the vector (i.e., keep only non-negative numbers), then sort the remaining elements in ascending order, and finally replace each element with its square root. The function must return nothing (void) and modify the vector in place. If the input vector contains only negative numbers or is empty, the function should leave the vector empty (after removing negatives, sorting, and applying square root—which results in an empty vector). The function should handle duplicate values, `0.0`, and floating-point precision naturally. The function signature must be `void normalizeAndSortVector(std::vector<double>& numbers);`. You are not allowed to use any global variables; all logic must be inside the function, using only standard library facilities.

// The solution involves three main steps: (1) Filter out negative elements from the vector. This can be done with the erase-remove idiom: `numbers.erase(std::remove_if(numbers.begin(), numbers.end(), [](double x) { return x < 0; }), numbers.end());`. This keeps only values `>= 0`. (2) Sort the remaining vector in ascending order using `std::sort`. (3) Apply the square root function to each element using `std::transform` with `std::sqrt`. Edge cases: an empty input vector remains empty after all steps; a vector with only negatives becomes empty after filtering; a vector with only `0.0` results in a single element `0.0` after sqrt; duplicate values are preserved and transformed individually. Time complexity: filtering is O(n), sorting is O(n log n), transformation is O(n), so overall O(n log n). Space complexity: O(1) auxiliary space aside from the vector itself, because the operations are in-place.

#include <vector>
#include <algorithm>
#include <cmath>

// Removes negative values, sorts ascending, and replaces each element with its square root.
// The input vector is modified in place.
void normalizeAndSortVector(std::vector<double>& numbers) {
    // Remove negative elements (keep only non-negative)
    numbers.erase(
        std::remove_if(numbers.begin(), numbers.end(),
                       [](double value) { return value < 0; }),
        numbers.end());
    
    // Sort remaining elements in ascending order
    std::sort(numbers.begin(), numbers.end());
    
    // Replace each element with its square root
    std::transform(numbers.begin(), numbers.end(), numbers.begin(),
                   [](double value) { return std::sqrt(value); });
}

#include <cassert>
#include <vector>
#include <cmath>

// Declaration of the solution function (in real code, include the header)
void normalizeAndSortVector(std::vector<double>& numbers);

int main() {
    // Test 1: Basic mixed vector
    std::vector<double> v1 = {16.0, -4.0, 9.0, 0.0, 25.0, -1.0};
    normalizeAndSortVector(v1);
    std::vector<double> expected1 = {0.0, 3.0, 4.0, 5.0};
    assert(v1.size() == expected1.size());
    for (size_t i = 0; i < v1.size(); ++i) {
        assert(std::abs(v1[i] - expected1[i]) < 1e-9);
    }

    // Test 2: All negative -> empty
    std::vector<double> v2 = {-2.0, -3.0, -1.0};
    normalizeAndSortVector(v2);
    assert(v2.empty());

    // Test 3: Empty vector
    std::vector<double> v3;
    normalizeAndSortVector(v3);
    assert(v3.empty());

    // Test 4: Only zeros
    std::vector<double> v4 = {0.0, 0.0, 0.0};
    normalizeAndSortVector(v4);
    assert(v4.size() == 3);
    for (double val : v4) {
        assert(std::abs(val - 0.0) < 1e-9);
    }

    // Test 5: Already sorted with duplicates
    std::vector<double> v5 = {4.0, 4.0, 9.0, 9.0};
    normalizeAndSortVector(v5);
    std::vector<double> expected5 = {2.0, 2.0, 3.0, 3.0};
    assert(v5 == expected5); // exact equality works for these sqrt results

    // Test 6: Unsorted with non-perfect squares
    std::vector<double> v6 = {2.0, 1.0, 3.0, -5.0};
    normalizeAndSortVector(v6);
    std::vector<double> expected6 = {1.0, std::sqrt(2.0), std::sqrt(3.0)};
    assert(v6.size() == expected6.size());
    for (size_t i = 0; i < v6.size(); ++i) {
        assert(std::abs(v6[i] - expected6[i]) < 1e-9);
    }

    // Test 7: Single non-negative element
    std::vector<double> v7 = {100.0};
    normalizeAndSortVector(v7);
    assert(std::abs(v7[0] - 10.0) < 1e-9);

    return 0;
}
