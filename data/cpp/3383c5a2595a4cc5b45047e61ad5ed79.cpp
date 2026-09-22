Write a C++ function `quickSort` that sorts a `std::vector` of any numeric type (e.g., `int`, `float`, `double`, `char`) in ascending order using the quicksort algorithm with the first element as the pivot. The function must accept two iterators (begin and end-of-range, where `end` points one past the last element) and must be templated on the vector's element type. The function must modify the vector in place and must handle vectors with duplicate values, negative numbers, and a single element correctly. The partition step must always place the pivot in its final sorted position, ensuring that the recursion terminates even when all elements are equal (e.g., `{3,3,3}`). The function must not use any standard sorting library functions. Provide a separate helper function `printVector` to display the vector before and after sorting. The solution must compile without warnings and be robust for any vector size ≥ 1.

#include <cassert>
#include <vector>
#include <algorithm>  // for std::is_sorted

int main() {
    // Test 1: typical integer vector with duplicates and negatives
    std::vector<int> v1 = {5, -3, 2, 5, 0, -1, 4};
    quickSort(v1.begin(), v1.end());
    assert(std::is_sorted(v1.begin(), v1.end()));

    // Test 2: all equal elements (edge case for infinite recursion)
    std::vector<int> v2 = {7, 7, 7, 7};
    quickSort(v2.begin(), v2.end());
    assert(std::is_sorted(v2.begin(), v2.end()));

    // Test 3: single element
    std::vector<double> v3 = {3.14};
    quickSort(v3.begin(), v3.end());
    assert(std::is_sorted(v3.begin(), v3.end()));

    // Test 4: vector of chars
    std::vector<char> v4 = {'b', 'a', 'c', 'b'};
    quickSort(v4.begin(), v4.end());
    assert(std::is_sorted(v4.begin(), v4.end()));

    // Test 5: already sorted vector (checks worst-case pivot behavior)
    std::vector<int> v5 = {1, 2, 3, 4, 5};
    quickSort(v5.begin(), v5.end());
    assert(std::is_sorted(v5.begin(), v5.end()));

    // Test 6: reverse sorted vector
    std::vector<int> v6 = {9, 8, 7, 6};
    quickSort(v6.begin(), v6.end());
    assert(std::is_sorted(v6.begin(), v6.end()));

    // Test 7: vector with two elements
    std::vector<float> v7 = {2.5f, 1.2f};
    quickSort(v7.begin(), v7.end());
    assert(std::is_sorted(v7.begin(), v7.end()));

    // Test 8: large vector with random values (using simple pattern)
    std::vector<int> v8 = {10, 1, 9, 2, 8, 3, 7, 4, 6, 5};
    quickSort(v8.begin(), v8.end());
    assert(std::is_sorted(v8.begin(), v8.end()));

    // Test 9: vector with negative and positive doubles
    std::vector<double> v9 = {-1.5, 0.0, 2.2, -3.3};
    quickSort(v9.begin(), v9.end());
    assert(std::is_sorted(v9.begin(), v9.end()));

    // Test 10: empty vector (should not crash; base case handles it)
    std::vector<int> v10;
    quickSort(v10.begin(), v10.end());
    assert(v10.empty());

    return 0;
}

#include <vector>
#include <iostream>
#include <iterator>  // for std::iter_swap, std::distance
#include <cstddef>   // for std::ptrdiff_t (optional)

// Partition the range [first, last) using the first element as pivot.
// Returns an iterator to the pivot's final sorted position.
template<typename T>
typename std::vector<T>::iterator partition(
    typename std::vector<T>::iterator first,
    typename std::vector<T>::iterator last) 
{
    // Pivot value is the first element.
    T pivot = *first;
    auto left = first + 1;
    auto right = last;

    while (true) {
        // Move left to the right while element <= pivot and left < right.
        while (left < right && *left <= pivot) {
            ++left;
        }
        // Move right to the left while element > pivot and left < right.
        while (left < right && *right > pivot) {
            --right;
        }

        if (left == right) {
            break;
        }
        std::iter_swap(left, right);
    }

    // Place pivot in its final position: swap with right if pivot is larger.
    if (*first > *right) {
        std::iter_swap(first, right);
    }
    return right;
}

// Quicksort the range [first, last) where last points one past the last element.
template<typename T>
void quickSort(typename std::vector<T>::iterator first,
               typename std::vector<T>::iterator last) 
{
    // Base case: 0 or 1 element.
    if (std::distance(first, last) <= 1) {
        return;
    }

    // Partition the range and get the pivot's final position.
    auto pivot_iter = partition<T>(first, last - 1);  // pass last element as right

    // Recursively sort left and right subranges.
    quickSort<T>(first, pivot_iter);
    quickSort<T>(pivot_iter + 1, last);
}

// Helper to print a vector to std::cout.
template<typename T>
void printVector(const std::vector<T>& vec) {
    for (const auto& elem : vec) {
        std::cout << elem << " ";
    }
    std::cout << std::endl;
}

// The solution implements the classic quicksort algorithm using a Hoare-style partition with the first element as the pivot. The core idea is to choose a pivot value, then iterate from the left (after the pivot) and from the right (end of range) inward, swapping elements that are out of order relative to the pivot, until the two iterators cross. After crossing, the pivot is swapped with the last element on the right that is ≤ pivot, thereby placing the pivot in its final sorted position. The partition returns an iterator to the pivot's final position, and then quicksort recursively sorts the left subrange (from begin to partition-1) and the right subrange (from partition+1 to end). A critical edge case is handling vectors with all identical values: the standard naive partition can cause infinite recursion, but this implementation ensures that when the pivot equals the right value, the right iterator moves left and the loop terminates correctly because the `right_iter` condition `*right_iter > pivot_val` fails, and `left_iter` eventually meets `right_iter`. The recursion base case is when `std::distance(begin, end) < 1` (i.e., empty or single-element subrange). Time complexity is average \(O(n \log n)\) and worst-case \(O(n^2)\) when the pivot is always the smallest or largest (e.g., already sorted input), but for random input it performs well. Space complexity is \(O(\log n)\) for the recursion stack in the average case, but \(O(n)\) in the worst case. The algorithm is in-place and does not use extra containers except a few iterators and the pivot value.
