/*
Write a C++ function named `bubbleSortBothDirections` that takes a `std::vector<int>` by reference and sorts it first in ascending order, then in descending order, and returns a `std::pair<std::vector<int>, std::vector<int>>` where the first element is the ascending-sorted vector and the second is the descending-sorted vector. The function must not modify the input vector during processing but instead work on a copy. The input vector may contain duplicate values, negative numbers, and can be empty or of size 1. The sorting must be implemented using the bubble sort algorithm (with early exit optimization if no swaps occur in a pass) and must not use any standard library sorting functions.
*/

#include <vector>
#include <utility>
#include <algorithm>

// Helper function to bubble sort a vector in the specified direction.
// direction = true for ascending, false for descending.
void bubbleSort(std::vector<int>& vec, bool ascending) {
    int n = static_cast<int>(vec.size());
    for (int i = 0; i < n - 1; ++i) {
        bool swapped = false;
        for (int j = 0; j < n - i - 1; ++j) {
            bool needSwap = ascending ? (vec[j] > vec[j + 1]) : (vec[j] < vec[j + 1]);
            if (needSwap) {
                std::swap(vec[j], vec[j + 1]);
                swapped = true;
            }
        }
        if (!swapped) break;
    }
}

// Returns {ascendingSortedVector, descendingSortedVector}.
std::pair<std::vector<int>, std::vector<int>> bubbleSortBothDirections(const std::vector<int>& input) {
    std::vector<int> asc = input; // copy
    std::vector<int> desc = input; // copy

    bubbleSort(asc, true);
    bubbleSort(desc, false);

    return {asc, desc};
}

#include <cassert>
#include <vector>
#include <utility>

// (Assume the solution function is declared above, or include it here.)

int main() {
    // Empty input
    std::vector<int> empty;
    auto res1 = bubbleSortBothDirections(empty);
    assert(res1.first.empty() && res1.second.empty());

    // Single element
    std::vector<int> single = {42};
    auto res2 = bubbleSortBothDirections(single);
    assert(res2.first.size() == 1 && res2.first[0] == 42);
    assert(res2.second.size() == 1 && res2.second[0] == 42);

    // Mixed input with duplicates and negatives
    std::vector<int> data = {12, 5, 8, 1, 9, 3, 7, 6, 2, 4, 11, 10, -3, 5};
    auto res3 = bubbleSortBothDirections(data);
    std::vector<int> expectedAsc = {-3, 1, 2, 3, 4, 5, 5, 6, 7, 8, 9, 10, 11, 12};
    std::vector<int> expectedDesc = {12, 11, 10, 9, 8, 7, 6, 5, 5, 4, 3, 2, 1, -3};
    assert(res3.first == expectedAsc);
    assert(res3.second == expectedDesc);

    // Already sorted ascending
    std::vector<int> sortedAsc = {1, 2, 3, 4};
    auto res4 = bubbleSortBothDirections(sortedAsc);
    std::vector<int> ascOut4 = {1, 2, 3, 4};
    std::vector<int> descOut4 = {4, 3, 2, 1};
    assert(res4.first == ascOut4 && res4.second == descOut4);

    // Already sorted descending
    std::vector<int> sortedDesc = {5, 4, 3, 2};
    auto res5 = bubbleSortBothDirections(sortedDesc);
    std::vector<int> ascOut5 = {2, 3, 4, 5};
    std::vector<int> descOut5 = {5, 4, 3, 2};
    assert(res5.first == ascOut5 && res5.second == descOut5);

    // All identical
    std::vector<int> allSame = {7, 7, 7};
    auto res6 = bubbleSortBothDirections(allSame);
    std::vector<int> sameOut = {7, 7, 7};
    assert(res6.first == sameOut && res6.second == sameOut);

    return 0;
}

// The solution approach involves creating a copy of the input vector to avoid modifying the caller's data. Then, implement a helper bubble sort routine that sorts a vector in a specified direction (ascending or descending) using nested loops. The outer loop runs from 0 to size-2, and the inner loop runs from 0 to size-i-1, comparing adjacent elements and swapping them if they are out of order (e.g., for ascending, if current > next; for descending, if current < next). An early exit can be added: if a full pass completes without any swaps, the array is already sorted, so break early. Edge cases include empty vectors (return two empty vectors) and single-element vectors (return identical vectors). The time complexity is O(n^2) in the worst case (reverse-sorted input) and O(n) in the best case (already sorted input with early exit). The space complexity is O(n) due to the copy of the input vector.
