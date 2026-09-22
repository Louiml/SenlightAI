/*
Write a C++ function `optimizedBubbleSort` that sorts a `std::vector<int>` in ascending order using an optimized version of bubble sort. The function must detect when the array becomes sorted early (i.e., no swaps occur in a complete pass) and stop immediately, improving best-case performance to O(n). Your implementation must handle empty vectors, vectors with a single element, already-sorted vectors, reverse-sorted vectors, and vectors with duplicate values. The function should modify the vector in-place and return `void`. Ensure the code is self-contained, uses `const` correctly for read-only parameters (though here the parameter is non-const as it is modified), and is ready to be used in a larger program without a `main` function.
*/

#include <vector>
#include <utility> // for std::swap

// Sorts a vector of integers in ascending order using optimized bubble sort.
// Early termination occurs if no swaps are performed in a full pass.
void optimizedBubbleSort(std::vector<int>& arr) {
    int n = arr.size();
    for (int i = 0; i < n - 1; ++i) {
        bool swapped = false;
        for (int j = 0; j < n - 1 - i; ++j) {
            if (arr[j] > arr[j + 1]) {
                std::swap(arr[j], arr[j + 1]);
                swapped = true;
            }
        }
        if (!swapped) {
            break; // Array is already sorted
        }
    }
}

#include <cassert>
#include <vector>

int main() {
    // Empty vector
    std::vector<int> empty;
    optimizedBubbleSort(empty);
    assert(empty.empty());

    // Single element vector
    std::vector<int> single = {5};
    optimizedBubbleSort(single);
    assert(single.size() == 1 && single[0] == 5);

    // Already sorted vector (best case, O(n))
    std::vector<int> sorted = {1, 2, 3, 4, 5};
    optimizedBubbleSort(sorted);
    assert(sorted == std::vector<int>({1, 2, 3, 4, 5}));

    // Reverse sorted vector (worst case)
    std::vector<int> reversed = {5, 4, 3, 2, 1};
    optimizedBubbleSort(reversed);
    assert(reversed == std::vector<int>({1, 2, 3, 4, 5}));

    // Unsorted vector with duplicates
    std::vector<int> duplicates = {3, 1, 3, 2, 1, 3};
    optimizedBubbleSort(duplicates);
    assert(duplicates == std::vector<int>({1, 1, 2, 3, 3, 3}));

    // Larger unsorted vector
    std::vector<int> large = {9, -3, 7, 0, 5, -1, 4};
    optimizedBubbleSort(large);
    assert(large == std::vector<int>({-3, -1, 0, 4, 5, 7, 9}));

    // Vector with all identical values
    std::vector<int> allSame = {2, 2, 2, 2};
    optimizedBubbleSort(allSame);
    assert(allSame == std::vector<int>({2, 2, 2, 2}));

    // Vector with negative and positive numbers mixed
    std::vector<int> mixed = {-10, 10, -20, 20, 0, 5};
    optimizedBubbleSort(mixed);
    assert(mixed == std::vector<int>({-20, -10, 0, 5, 10, 20}));

    // Two-element vector already sorted
    std::vector<int> twoSorted = {1, 2};
    optimizedBubbleSort(twoSorted);
    assert(twoSorted == std::vector<int>({1, 2}));

    // Two-element vector unsorted
    std::vector<int> twoUnsorted = {2, 1};
    optimizedBubbleSort(twoUnsorted);
    assert(twoUnsorted == std::vector<int>({1, 2}));

    return 0;
}

// The optimized bubble sort works by performing multiple passes over the vector. In each pass, it compares adjacent elements and swaps them if they are out of order. The key optimization is tracking a boolean flag `swapped` that is set to `true` whenever a swap occurs during a pass. After each pass, if no swaps were made, the array is already sorted, and the algorithm terminates early, avoiding unnecessary passes. The algorithm ensures that after the i-th pass, the i largest elements are correctly positioned at the end of the vector, so the inner loop only needs to iterate up to `n-1-i`. Edge cases: an empty vector or a vector of size 1 requires no sorting and the loop conditions naturally handle them (the outer loop runs `n-1` times, which is 0 for n=1 or negative for n=0, so the function exits immediately). Duplicate values are handled correctly because swaps only occur when `arr[j] > arr[j+1]`, not when they are equal. Time complexity: O(n^2) in the worst and average cases (reverse-sorted or random data), but O(n) in the best case (already sorted) due to the early exit. Space complexity: O(1) auxiliary space, as sorting is in-place using only a temporary variable for swapping and a boolean flag.
