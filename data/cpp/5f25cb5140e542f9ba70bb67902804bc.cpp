Write a C++ function named `selectionSortAndPrint` that takes a `std::vector<T>` by reference and performs selection sort in ascending order. The function should be templated so it works for any comparable type (e.g., `int`, `double`). After sorting, it must print the sorted elements to `std::cout` in a single line, separated by no spaces (just concatenated digits/values), followed by a newline. The function must handle vectors with duplicate values and the empty vector (printing just a newline). Do not use `std::sort`; implement the selection sort algorithm from scratch using explicit loops.

#include <cassert>
#include <sstream>
#include <string>

// We need to redirect std::cout for testing, so we'll use a helper that captures output.
std::string captureSortOutput(std::vector<int> input) {
    std::ostringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());
    selectionSortAndPrint(input);
    std::cout.rdbuf(old);
    return buffer.str();
}

int main() {
    // Test with typical vector
    assert(captureSortOutput({5, 2, 9, 1, 5, 6}) == "125569\n");

    // Test with negative numbers
    assert(captureSortOutput({-3, -10, 0, 7}) == "-10-307\n");

    // Test with duplicates only
    assert(captureSortOutput({4, 4, 4}) == "444\n");

    // Test with single element
    assert(captureSortOutput({42}) == "42\n");

    // Test with empty vector
    assert(captureSortOutput({}) == "\n");

    // Test with already sorted (descending)
    assert(captureSortOutput({3, 2, 1}) == "123\n");

    // Test with double values (using a separate check because capture function is for int)
    std::vector<double> doubles = {2.5, 0.1, 3.3, 1.2};
    std::ostringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());
    selectionSortAndPrint(doubles);
    std::cout.rdbuf(old);
    assert(buffer.str() == "0.11.22.53.3\n");

    // Test with large numbers
    assert(captureSortOutput({1000000, 2, 999999}) == "21000000999999\n");

    // Test with two elements already sorted
    assert(captureSortOutput({1, 2}) == "12\n");

    // Test with two elements reversed
    assert(captureSortOutput({2, 1}) == "12\n");
}

#include <vector>
#include <iostream>
#include <algorithm> // for std::swap

/**
 * Sorts a std::vector in ascending order using selection sort,
 * then prints the sorted elements concatenated with no separators
 * followed by a newline. Handles empty and single-element vectors.
 *
 * @tparam T The element type, must support operator< and operator<<.
 * @param data The vector to be sorted in-place.
 */
template <typename T>
void selectionSortAndPrint(std::vector<T>& data) {
    const std::size_t n = data.size();

    // Selection sort outer loop: grow the sorted prefix
    for (std::size_t i = 0; i < n; ++i) {
        // Find the index of the minimum element in data[i..n-1]
        std::size_t minIndex = i;
        for (std::size_t j = i + 1; j < n; ++j) {
            if (data[j] < data[minIndex]) {
                minIndex = j;
            }
        }
        // Swap the found minimum with the current position if needed
        if (minIndex != i) {
            std::swap(data[i], data[minIndex]);
        }
    }

    // Print the sorted elements, no separators, then newline
    for (const T& value : data) {
        std::cout << value;
    }
    std::cout << '\n';
}

// The selection sort algorithm repeatedly finds the minimum element from the unsorted portion of the vector and swaps it with the first unsorted element. The sorted portion grows from the left. For each index `i` from 0 to `n-2`, we scan from `i+1` to `n-1` to find the index of the minimum, then swap if needed. Edge cases: an empty vector — the outer loop runs zero times, and we print just a newline. A vector with one element — outer loop runs zero times, and we print that single element. Duplicates — the algorithm naturally handles them because it only swaps when finding a strictly smaller element, but if we compare with `>` or `<` correctly it remains stable? Selection sort is not stable, but duplicates are still correctly sorted (they end up in some relative order). Time complexity is O(n²) for both average and worst cases because of the nested loops. Space complexity is O(1) auxiliary (in-place) aside from the printing overhead, which is O(n) for the output stream but that's not auxiliary storage in the algorithm sense — we can consider it O(1) extra memory for the algorithm itself.
