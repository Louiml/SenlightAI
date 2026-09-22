/*
Write a C++ function named `countPairsInSortedDuplicates` that takes a vector of integers (by const reference) and returns the number of pairs of equal values that can be formed from the elements. A pair consists of two identical numbers, and each element can be used in at most one pair. The function must internally sort the vector (a copy) to group duplicates, then count how many complete pairs exist for each value by dividing its frequency by 2 and summing the results. The input vector may contain negative numbers, zeros, duplicates, and any order. The function must not modify the original vector and must handle an empty vector correctly (returning 0). The function signature is: `int countPairsInSortedDuplicates(const std::vector<int>& numbers);`. The implementation must be efficient and avoid using the original unsorted array repeatedly.
*/
#include <vector>
#include <algorithm>

// Count how many pairs of equal numbers can be formed from the input vector.
// Reads the vector, creates a sorted copy, then sums the floor of each frequency / 2.
int countPairsInSortedDuplicates(const std::vector<int>& numbers) {
    // Create a copy to avoid modifying the caller's data.
    std::vector<int> sorted = numbers;
    std::sort(sorted.begin(), sorted.end());
    
    int totalPairs = 0;
    int index = 0;
    int size = static_cast<int>(sorted.size());
    
    while (index < size) {
        int currentValue = sorted[index];
        int frequency = 0;
        // Count how many times currentValue appears consecutively.
        while (index < size && sorted[index] == currentValue) {
            ++frequency;
            ++index;
        }
        // Each pair consumes two identical elements.
        totalPairs += frequency / 2;
    }
    
    return totalPairs;
}
#include <cassert>
#include <vector>

// The solution function is declared above; include it directly in the same file.

int main() {
    // Empty vector: no pairs.
    assert(countPairsInSortedDuplicates(std::vector<int>{}) == 0);

    // Single element: no pairs.
    assert(countPairsInSortedDuplicates(std::vector<int>{5}) == 0);

    // All distinct values.
    assert(countPairsInSortedDuplicates(std::vector<int>{1, 2, 3, 4}) == 0);

    // Simple duplicates.
    assert(countPairsInSortedDuplicates(std::vector<int>{3, 3, 3}) == 1);

    // Multiple values with different frequencies.
    assert(countPairsInSortedDuplicates(std::vector<int>{1, 1, 2, 2, 2, 3}) == 2);

    // Negative numbers and zeros.
    assert(countPairsInSortedDuplicates(std::vector<int>{-1, -1, 0, 0, 0, -1}) == 2);

    // Odd frequency rounds down.
    assert(countPairsInSortedDuplicates(std::vector<int>{7, 7, 7, 7, 7}) == 2);

    // Unsorted input with duplicates.
    assert(countPairsInSortedDuplicates(std::vector<int>{4, 2, 4, 2, 4, 2}) == 3);

    // Mixed large values.
    assert(countPairsInSortedDuplicates(std::vector<int>{100, 100, 100, 100, 50, 50, 1}) == 3);

    // All same values, even count.
    assert(countPairsInSortedDuplicates(std::vector<int>{9, 9, 9, 9}) == 2);

    return 0;
}
// The solution creates a copy of the input vector and sorts it (using `std::sort`). Sorting groups equal values together, making it easy to process each group. After sorting, iterate through the array once, tracking the current value and its frequency. When the value changes (or at the end of the array), add `frequency / 2` to the total pairs count, then reset the frequency for the new value. Edge cases: an empty vector returns 0; a vector with all distinct values returns 0; values with odd frequencies contribute only the floor division (e.g., 5 occurrences give 2 pairs). The algorithm works for negative numbers and duplicates. The time complexity is O(n log n) due to sorting, and O(n) auxiliary space for the copy (or O(1) if we sort in place of a passed-by-value parameter, but we use const reference and copy). The iteration after sorting is O(n), so overall O(n log n). Space complexity is O(n) for the copy.
