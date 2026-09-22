Write a C++ function that, given a vector of integers, returns the median value. The median is defined as the middle element after sorting when the vector has an odd number of elements, and as the arithmetic average of the two middle elements (as a `double`) when the vector has an even number of elements. The input vector may contain duplicate values, negative numbers, and can be of any non-zero size. The function must not modify the original vector.

The solution sorts a copy of the input vector in non-decreasing order. For a vector of size `n`, the median index is `n/2` (integer division). If `n` is odd, the median is the element at that index. If `n` is even, the two middle elements are at positions `n/2 - 1` and `n/2`, and the median is their sum divided by 2.0 (as a `double`). Edge cases include a single-element vector (median is that element, returns a `double`), duplicates (handled naturally by sorting), and negative numbers (sorting handles ordering). Time complexity is \(O(n \log n)\) due to sorting, and space complexity is \(O(n)\) for the copied vector.

#include <vector>
#include <algorithm>

// Returns the median of the integers in the input vector.
// For odd-sized vectors, returns the middle element.
// For even-sized vectors, returns the average of the two middle elements.
double median_of_vector(const std::vector<int>& input) {
    std::vector<int> sorted = input;  // Copy to avoid modifying original
    std::sort(sorted.begin(), sorted.end());
    size_t n = sorted.size();
    if (n % 2 == 1) {
        return static_cast<double>(sorted[n / 2]);
    } else {
        return (static_cast<double>(sorted[n / 2 - 1]) + static_cast<double>(sorted[n / 2])) / 2.0;
    }
}

#include <cassert>
#include <vector>

int main() {
    // Odd size
    assert(median_of_vector({1, 2, 3}) == 2.0);
    // Even size
    assert(median_of_vector({1, 2, 3, 4}) == 2.5);
    // Single element
    assert(median_of_vector({7}) == 7.0);
    // Duplicates
    assert(median_of_vector({5, 5, 5, 5}) == 5.0);
    // Negative numbers
    assert(median_of_vector({-3, -1, -2}) == -2.0);
    // Unsorted input
    assert(median_of_vector({4, 1, 3, 2}) == 2.5);
    // Large values
    assert(median_of_vector({1000, 2000, 1500}) == 1500.0);
    // Mixed signs and even size
    assert(median_of_vector({-10, 10, -5, 5}) == 0.0);
    // Odd size with duplicates and negatives
    assert(median_of_vector({-1, -1, 0, 2, 2}) == 0.0);
    // Already sorted even size
    assert(median_of_vector({10, 20, 30, 40}) == 25.0);
}
