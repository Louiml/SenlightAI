Write a C++ function named `bubbleSort` that takes a reference to a `std::vector<int>` and sorts it in ascending order using the bubble sort algorithm exactly as shown in the provided snippet (nested loops with adjacent swaps, outer loop runs `n` times, inner loop runs `n-1` times per outer iteration). The function must modify the vector in place and return `void`. You may not use `std::sort` or any other sorting utility. Ensure the function is correct for empty vectors, vectors with a single element, vectors with all duplicate elements, and large inputs (e.g., 1000 elements in reverse order). The function must be `const`-correct concerning the input parameter (though a vector passed by reference is naturally non-const to allow modification). Provide the solution as a standalone function with necessary headers; no `main` in the solution section.
The solution implements bubble sort exactly as specified: the outer loop runs `n` times (where `n` is the current size of the vector), and the inner loop runs `n-1` times per outer iteration, comparing adjacent elements and swapping them if they are out of order. This guarantees that after the first pass, the largest element bubbles to the end, after the second pass the second-largest is in place, and so on. For an input of size `n`, the algorithm always performs `n*(n-1)` comparisons and at most that many swaps, regardless of input order (no early termination optimization). Edge cases: empty vector → loops do not execute, no change; single element → outer loop runs once, inner loop runs 0 times, no change; all equal elements → no swaps occur, but comparisons still happen. The time complexity is \(O(n^2)\) in all cases, and space complexity is \(O(1)\) auxiliary (only the loop counters and a temporary for swap). The function is straightforward: it directly applies the snippet's logic to a vector, using `std::swap` from `<utility>` or manual swap.
#include <vector>
#include <utility>  // for std::swap

// Sort a vector of integers in ascending order using bubble sort.
// The outer loop runs n times; inner loop runs n-1 times per outer iteration.
// The vector is modified in place.
void bubbleSort(std::vector<int>& array) {
    int n = static_cast<int>(array.size());
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n - 1; ++j) {
            if (array[j] > array[j + 1]) {
                std::swap(array[j], array[j + 1]);
            }
        }
    }
}
#include <cassert>
#include <vector>

// The solution function is declared above; tests below.

int main() {
    // Empty vector
    std::vector<int> empty;
    bubbleSort(empty);
    assert(empty.empty());

    // Single element
    std::vector<int> single = {5};
    bubbleSort(single);
    assert(single.size() == 1 && single[0] == 5);

    // Already sorted
    std::vector<int> sorted = {1, 2, 3, 4, 5};
    bubbleSort(sorted);
    assert(sorted == std::vector<int>({1, 2, 3, 4, 5}));

    // Reverse sorted
    std::vector<int> reverse = {5, 4, 3, 2, 1};
    bubbleSort(reverse);
    assert(reverse == std::vector<int>({1, 2, 3, 4, 5}));

    // Unsorted with duplicates
    std::vector<int> duplicates = {3, 1, 4, 1, 5, 9, 2, 6, 5, 3, 5};
    bubbleSort(duplicates);
    assert(duplicates == std::vector<int>({1, 1, 2, 3, 3, 4, 5, 5, 5, 6, 9}));

    // All equal
    std::vector<int> allEqual = {7, 7, 7, 7};
    bubbleSort(allEqual);
    assert(allEqual == std::vector<int>({7, 7, 7, 7}));

    // Large input (e.g., 1000 descending)
    std::vector<int> large(1000);
    for (int i = 0; i < 1000; ++i) large[i] = 999 - i; // descending
    bubbleSort(large);
    for (int i = 0; i < 1000; ++i) {
        assert(large[i] == i);
    }

    return 0;
}
