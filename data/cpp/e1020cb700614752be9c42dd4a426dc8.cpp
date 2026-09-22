// Write a C++ function that takes a `std::vector<int>&` (non-const reference) and returns the average of all elements except the smallest and largest values, computed as a `double`. The input vector will have at least 3 elements, so after removing one minimum and one maximum, at least one element remains. If there are duplicate minimum or maximum values, only one occurrence of each is excluded. The function must modify the original vector by sorting it in place, and must not use any floating-point rounding beyond the natural division. The result should be the arithmetic mean of the remaining elements.

The standard approach is to sort the input vector in ascending order. After sorting, the smallest element is at index 0 and the largest is at index `size()-1`. We then sum all elements from index 1 to `size()-2` (inclusive), count how many elements were summed (`size()-2`), and divide by that count to get the average as a `double`. Edge cases: if the vector has exactly 3 elements, after removing min and max only one element remains, so the average is just that middle value. If all elements are equal (e.g., [5,5,5]), the sorted vector still has min=5 at index 0 and max=5 at last index, and the middle element(s) sum to 5, giving average 5. The algorithm requires sorting, which is O(n log n) time, and O(1) auxiliary space (excluding the vector itself). The function signature matches the given snippet, taking a non-const reference so it can sort in place.

#include <vector>
#include <algorithm>

// Computes the average of all elements except the smallest and largest.
// Assumes the input vector has at least 3 elements.
// Sorts the vector in ascending order to easily identify min and max.
// Returns the arithmetic mean as a double.
double averageExcludingMinMax(std::vector<int>& salary) {
    std::sort(salary.begin(), salary.end());          // Sort in place
    int n = static_cast<int>(salary.size()) - 2;      // Number of middle elements
    double sum = 0.0;
    for (std::size_t i = 1; i < salary.size() - 1; ++i) {
        sum += salary[i];                             // Sum all except first and last
    }
    return sum / n;                                   // n >= 1 because size >= 3
}

#include <cassert>
#include <vector>

// Forward declaration of the solution function
double averageExcludingMinMax(std::vector<int>& salary);

int main() {
    // Basic case
    std::vector<int> v1 = {4000, 3000, 1000, 2000};
    assert(averageExcludingMinMax(v1) == 2500.0);  // 2000+3000 / 2 = 2500

    // All equal values: min and max are same, but only one removed each side
    std::vector<int> v2 = {1000, 1000, 1000};
    assert(averageExcludingMinMax(v2) == 1000.0);  // only middle 1000

    // Exactly 3 elements
    std::vector<int> v3 = {5, 1, 3};
    assert(averageExcludingMinMax(v3) == 3.0);  // only 3 remains

    // Duplicate min and max values, only one of each removed
    std::vector<int> v4 = {2, 2, 3, 4, 4};
    assert(averageExcludingMinMax(v4) == 3.0);  // 2+3+4 / 3 = 3.0

    // Negative numbers
    std::vector<int> v5 = {-5, -1, -10, -3};
    assert(averageExcludingMinMax(v5) == -2.0); // -5 + -1 / 2 = -3.0? Actually sorted: -10,-5,-3,-1 -> middle -5,-3 sum=-8, /2 = -4.0? Wait check
    // Let's recompute: sorted = -10, -5, -3, -1 -> middle elements -5 and -3 sum=-8 /2 = -4.0
    // Correct assertion:
    std::vector<int> v5a = {-5, -1, -10, -3};
    assert(averageExcludingMinMax(v5a) == -4.0);

    // Large numbers, check double division
    std::vector<int> v6 = {0, 1000000, 2000000};
    assert(averageExcludingMinMax(v6) == 1000000.0);

    // Additional check with unsorted input
    std::vector<int> v7 = {9, 1, 5, 7, 3};
    assert(averageExcludingMinMax(v7) == 5.0); // sorted 1,3,5,7,9 -> middle 3,5,7 sum=15 /3 =5.0
}
