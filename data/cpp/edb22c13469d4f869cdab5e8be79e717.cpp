Write a C++ function named `bubbleSortAscending` that takes a `std::vector<int>` by value and returns a new `std::vector<int>` with all elements sorted in ascending order using the classic bubble sort algorithm (as shown in the snippet, but adapted to vectors for flexibility). The function must not modify the original input vector, must handle any size (including empty and single-element vectors), and must preserve duplicate values. The sorting must be stable, meaning equal elements retain their original relative order; however, since the original code swaps only when `arr[j] > arr[j+1]`, this stability is naturally maintained. The function should be `const`-correct and efficient enough for small to medium inputs.
// The solution uses the standard bubble sort technique: repeatedly iterate through the vector, comparing adjacent elements and swapping them if the left element is strictly greater than the right. Each complete pass ensures that the largest unsorted element “bubbles up” to its final position at the end, so the next pass can ignore one fewer element. For a vector of size `n`, we perform at most `n-1` passes, each comparing up to `n-1` pairs. However, we can optimize by tracking whether any swap occurred in a pass; if none occurred, the vector is already sorted and we can break early. Edge cases: for empty or single-element vectors, the function returns immediately (the outer loop naturally does nothing). Duplicate values remain in original order because we only swap on strict greater-than. Time complexity is O(n²) worst-case and average, O(n) best-case when already sorted (due to early break). Space complexity is O(n) for the returned copy of the input, plus O(1) auxiliary for loops and a temporary swap variable. The function takes the vector by value to naturally create a copy, allowing it to sort the copy without modifying the original.
#include <vector>

// Sorts a copy of the input vector in ascending order using bubble sort.
// Returns the sorted vector; the original input remains unchanged.
std::vector<int> bubbleSortAscending(std::vector<int> values) {
    size_t n = values.size();
    if (n <= 1) {
        return values; // Already sorted or empty
    }

    for (size_t i = 0; i < n - 1; ++i) {
        bool swapped = false;
        // Last i elements are already in place
        for (size_t j = 0; j < n - i - 1; ++j) {
            if (values[j] > values[j + 1]) {
                int temp = values[j];
                values[j] = values[j + 1];
                values[j + 1] = temp;
                swapped = true;
            }
        }
        // If no swaps, the vector is already sorted
        if (!swapped) {
            break;
        }
    }
    return values;
}
#include <cassert>
#include <vector>

// Declaration of the function (normally in a header)
std::vector<int> bubbleSortAscending(std::vector<int> values);

int main() {
    // Empty vector
    assert(bubbleSortAscending({}) == std::vector<int>({}));
    // Single element
    assert(bubbleSortAscending({42}) == std::vector<int>({42}));
    // Already sorted
    assert(bubbleSortAscending({1, 2, 3, 4}) == std::vector<int>({1, 2, 3, 4}));
    // Reverse sorted
    assert(bubbleSortAscending({9, 5, 3, 1}) == std::vector<int>({1, 3, 5, 9}));
    // Duplicates
    assert(bubbleSortAscending({4, 2, 4, 1}) == std::vector<int>({1, 2, 4, 4}));
    // Negative numbers
    assert(bubbleSortAscending({-3, -1, -7, 2}) == std::vector<int>({-7, -3, -1, 2}));
    // Mixed large
    assert(bubbleSortAscending({100, -50, 0, 23, 7}) == std::vector<int>({-50, 0, 7, 23, 100}));
    // Check original vector is not modified
    std::vector<int> original = {3, 1, 2};
    auto result = bubbleSortAscending(original);
    assert(original == std::vector<int>({3, 1, 2}));
    assert(result == std::vector<int>({1, 2, 3}));
    return 0;
}
