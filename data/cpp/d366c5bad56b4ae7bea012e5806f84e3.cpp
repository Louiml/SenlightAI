/*
Write a C++ function `isSortedAfterRadixSort` that takes a vector of non-negative integers and returns a boolean indicating whether the vector would be sorted in non-decreasing order after applying a radix sort (LSD, base 10) to the entire vector. The function should not modify the original vector; it should work on a copy. For empty or single-element vectors, the function should return `true` since they are trivially sorted. The input is guaranteed to contain only non-negative integers (including 0). The radix sort must use a stable counting sort per digit (from least significant to most significant) and handle numbers of varying digit lengths correctly. Edge cases include zeros and numbers with leading zeros (e.g., 5 represented as 005 after enough passes, though the original integers are normal).
*/
#include <vector>
#include <algorithm>
#include <cmath>

// Returns true if the vector is sorted in non-decreasing order after applying
// an LSD radix sort (base 10) to a copy of the input. Original vector is unchanged.
bool isSortedAfterRadixSort(const std::vector<int>& input) {
    if (input.size() <= 1) return true;

    // Make a copy to sort
    std::vector<int> arr = input;
    int n = arr.size();

    // Find the maximum element to determine number of digit passes
    auto maxIt = std::max_element(arr.begin(), arr.end());
    int maxVal = *maxIt;

    // If max is 0, all are zeros, already sorted
    if (maxVal == 0) return true;

    // Number of digits in maxVal
    int maxDigits = 0;
    while (maxVal > 0) {
        ++maxDigits;
        maxVal /= 10;
    }

    // Radix sort LSD with counting sort per digit
    int exp = 1;
    for (int digit = 0; digit < maxDigits; ++digit) {
        std::vector<int> output(n);
        std::vector<int> count(10, 0);

        // Count occurrences of each digit
        for (int num : arr) {
            int d = (num / exp) % 10;
            ++count[d];
        }

        // Prefix sums
        for (int i = 1; i < 10; ++i) {
            count[i] += count[i - 1];
        }

        // Build output array (stable): iterate from right to left
        for (int i = n - 1; i >= 0; --i) {
            int d = (arr[i] / exp) % 10;
            --count[d];
            output[count[d]] = arr[i];
        }

        // Copy back
        arr = std::move(output);
        exp *= 10;
    }

    // Check if sorted
    for (int i = 1; i < n; ++i) {
        if (arr[i - 1] > arr[i]) return false;
    }
    return true;
}
#include <cassert>
#include <vector>

// Function declared elsewhere
bool isSortedAfterRadixSort(const std::vector<int>& input);

int main() {
    // Test empty and single-element
    assert(isSortedAfterRadixSort({}) == true);
    assert(isSortedAfterRadixSort({42}) == true);

    // Already sorted
    assert(isSortedAfterRadixSort({1, 2, 3, 4}) == true);
    assert(isSortedAfterRadixSort({0, 0, 0}) == true);

    // Unsorted that radix sort will fix
    assert(isSortedAfterRadixSort({4, 3, 2, 1}) == true);
    assert(isSortedAfterRadixSort({5, 1, 4, 2, 8}) == true);
    assert(isSortedAfterRadixSort({170, 45, 75, 90, 802, 24, 2, 66}) == true);

    // Numbers with different digit lengths including zeros
    assert(isSortedAfterRadixSort({0, 5, 10, 2, 100, 1}) == true);

    // Duplicates
    assert(isSortedAfterRadixSort({7, 7, 7, 2, 7}) == true);

    // Original not modified
    std::vector<int> original = {3, 1, 2};
    std::vector<int> copy = original;
    assert(isSortedAfterRadixSort(original) == true);
    assert(original == copy);

    return 0;
}
// The solution must implement an LSD radix sort on a copy of the input vector, then check if the sorted copy is in non-decreasing order. The radix sort processes digits from the least significant digit (units) to the most significant. For each digit position, use a stable counting sort: first count occurrences of each digit (0-9) in the current position, convert counts to prefix sums, then iterate the original copy from right to left to place elements into an output array at positions determined by the digit's count. After each pass, copy the output back to the working array. Determine the maximum number of digits in the largest element to set the number of passes. Handle the zero case correctly: if all elements are zero or the maximum is zero, the array is already sorted, so return `true` without extra passes. Time complexity is O(d * (n + 10)) where d is the number of digits in the maximum value and n is the vector size; space complexity is O(n) for the output array and the copy.
