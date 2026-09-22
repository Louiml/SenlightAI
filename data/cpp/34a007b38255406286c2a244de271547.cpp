/*
Write a C++ function `bubbleSortArray` that takes a `std::vector<int>` by reference and sorts it in ascending order using the bubble sort algorithm (the same logic as the provided snippet, but adapted to vectors). The function must be `void` and modify the vector in‑place. Additionally, output the sorted array to the standard output in the format `Array after bubble sort:` followed by space-separated values, exactly as in the snippet. The function should handle any size vector (including empty and single-element cases) without runtime errors. In a separate test harness, verify the function using `assert` on several cases, including already-sorted, reverse-sorted, unsorted with duplicates, and empty/one-element vectors.
*/

#include <iostream>
#include <vector>

// Sorts the given vector in ascending order using bubble sort and prints the result.
void bubbleSortArray(std::vector<int>& arr) {
    int n = static_cast<int>(arr.size());
    for (int i = 0; i < n - 1; ++i) {
        for (int j = 0; j < n - i - 1; ++j) {
            if (arr[j] > arr[j + 1]) {
                std::swap(arr[j], arr[j + 1]);
            }
        }
    }

    std::cout << "Array after bubble sort:";
    for (int value : arr) {
        std::cout << " " << value;
    }
    std::cout << std::endl;
}

#include <cassert>
#include <vector>

// The solution function is declared above; include its definition before this main.

int main() {
    // Test 1: unsorted array
    std::vector<int> v1 = {5, 2, 8, 1, 9};
    bubbleSortArray(v1);
    assert((v1 == std::vector<int>{1, 2, 5, 8, 9}));

    // Test 2: already sorted
    std::vector<int> v2 = {1, 2, 3, 4, 5};
    bubbleSortArray(v2);
    assert((v2 == std::vector<int>{1, 2, 3, 4, 5}));

    // Test 3: reverse sorted
    std::vector<int> v3 = {9, 7, 5, 3, 1};
    bubbleSortArray(v3);
    assert((v3 == std::vector<int>{1, 3, 5, 7, 9}));

    // Test 4: duplicates
    std::vector<int> v4 = {4, 2, 4, 2, 4};
    bubbleSortArray(v4);
    assert((v4 == std::vector<int>{2, 2, 4, 4, 4}));

    // Test 5: single element
    std::vector<int> v5 = {42};
    bubbleSortArray(v5);
    assert((v5 == std::vector<int>{42}));

    // Test 6: empty vector
    std::vector<int> v6;
    bubbleSortArray(v6);
    assert(v6.empty());

    return 0;
}

// The solution uses the classic bubble sort: repeatedly iterate through the array, comparing adjacent elements and swapping them if they are out of order. After each full pass, the largest unsorted element “bubbles up” to its correct position at the end, so the number of passes equals `n-1` (where `n` is the vector size). The inner loop runs from `0` to `n-i-1` (since the last `i` elements are already sorted after `i` passes). If no swaps occur during a pass, the array is already sorted, but the snippet does not include that optimization; we can keep the straightforward double loop for fidelity. Edge cases: an empty vector requires no work; a single-element vector requires no comparisons. The time complexity is \(O(n^2)\) (worst and average) and \(O(n)\) best case if we add a swap flag, but the classic version is always \(O(n^2)\); space complexity is \(O(1)\) extra. Output formatting: print `"Array after bubble sort:"` once, then each element separated by a space, with no trailing extra output (the snippet prints the first element with a leading space after the colon, which we replicate exactly).
