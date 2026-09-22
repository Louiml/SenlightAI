/*
Write a C++ function template `mergeSortContainer` that sorts any standard library container supporting random access (such as `std::vector`, `std::array`, or `std::deque`) using the merge sort algorithm. The function must accept a reference to the container and sort it in ascending order using the container's default `operator<` for its element type. The function should work for containers of integral, floating-point, or string types. Your implementation must be self-contained (no external sorting libraries), must not use iterators for the merge step (only the container's `at()` method for element access), and must handle empty containers and single-element containers correctly without error. The function signature should be: `template<class T> void mergeSortContainer(T& container);`
*/

#include <vector>
#include <algorithm>

// Default predicate: less-than comparison.
template <typename T>
class DefaultLessThan {
public:
    bool operator()(const T& a, const T& b) const {
        return a < b;
    }
};

// Merge two sorted subranges [p..q] and [q+1..r] of the container.
template <typename T>
void merge(T& container,
           typename T::size_type p,
           typename T::size_type q,
           typename T::size_type r) {
    using ValueType = typename T::value_type;

    // Copy left half into a temporary vector.
    std::vector<ValueType> left(q - p + 1);
    std::generate(left.begin(), left.end(),
                  [index = p, &container]() mutable -> ValueType {
                      return container.at(index++);
                  });

    // Copy right half into a temporary vector.
    std::vector<ValueType> right(r - q);
    std::generate(right.begin(), right.end(),
                  [index = q + 1, &container]() mutable -> ValueType {
                      return container.at(index++);
                  });

    auto leftIter = left.cbegin();
    auto rightIter = right.cbegin();
    auto dest = p;

    DefaultLessThan<ValueType> pred{};

    // Merge the two sorted halves.
    while (leftIter != left.cend() && rightIter != right.cend()) {
        if (pred(*leftIter, *rightIter)) {
            container.at(dest++) = *leftIter++;
        } else {
            container.at(dest++) = *rightIter++;
        }
    }

    // Copy any remaining elements from the left half.
    while (leftIter != left.cend()) {
        container.at(dest++) = *leftIter++;
    }

    // Copy any remaining elements from the right half.
    while (rightIter != right.cend()) {
        container.at(dest++) = *rightIter++;
    }
}

// Recursive merge sort on the inclusive range [left, right].
template <typename T>
void mergeSort(T& container,
               typename T::size_type left,
               typename T::size_type right) {
    if (right <= left) return;  // Base case: 0 or 1 element.

    auto middle = (left + right) / 2;
    mergeSort(container, left, middle);
    mergeSort(container, middle + 1, right);
    merge(container, left, middle, right);
}

// Public entry point: sorts an entire container using merge sort.
template <typename T>
void mergeSortContainer(T& container) {
    if (container.empty()) return;  // No elements to sort.

    auto lastIndex = container.size() - 1;
    mergeSort(container, static_cast<typename T::size_type>(0), lastIndex);
}

#include <cassert>
#include <vector>
#include <array>
#include <deque>
#include <string>

int main() {
    // Test with std::vector<int>
    std::vector<int> v1 = {5, 2, 9, 1, 5, 6};
    mergeSortContainer(v1);
    assert(v1 == std::vector<int>({1, 2, 5, 5, 6, 9}));

    // Test with empty vector
    std::vector<int> v2;
    mergeSortContainer(v2);
    assert(v2.empty());

    // Test with single-element vector
    std::vector<int> v3 = {42};
    mergeSortContainer(v3);
    assert(v3 == std::vector<int>({42}));

    // Test with std::array<int, 5>
    std::array<int, 5> a1 = {9, -3, 0, 7, 2};
    mergeSortContainer(a1);
    assert(a1 == std::array<int, 5>({-3, 0, 2, 7, 9}));

    // Test with std::deque<double>
    std::deque<double> d1 = {3.14, -1.5, 2.71, 0.0, -0.99};
    mergeSortContainer(d1);
    assert(d1 == std::deque<double>({-1.5, -0.99, 0.0, 2.71, 3.14}));

    // Test with std::vector<std::string>
    std::vector<std::string> s1 = {"banana", "apple", "cherry", "date"};
    mergeSortContainer(s1);
    assert(s1 == std::vector<std::string>({"apple", "banana", "cherry", "date"}));

    // Test already sorted
    std::vector<int> v4 = {1, 2, 3, 4, 5};
    mergeSortContainer(v4);
    assert(v4 == std::vector<int>({1, 2, 3, 4, 5}));

    // Test reverse sorted
    std::vector<int> v5 = {5, 4, 3, 2, 1};
    mergeSortContainer(v5);
    assert(v5 == std::vector<int>({1, 2, 3, 4, 5}));

    // Test duplicates
    std::vector<int> v6 = {7, 7, 7, 1, 7};
    mergeSortContainer(v6);
    assert(v6 == std::vector<int>({1, 7, 7, 7, 7}));

    // Test large container (1000 elements reverse order)
    std::vector<int> v7;
    for (int i = 1000; i > 0; --i) v7.push_back(i);
    mergeSortContainer(v7);
    for (int i = 0; i < 1000; ++i) {
        assert(v7[i] == i + 1);
    }

    return 0;
}

// The solution follows a classic recursive merge sort. The main function `mergeSortContainer` delegates to a recursive helper `mergeSort` that takes the container and zero-based indices `left` and `right` representing the inclusive range to sort. The base case is when `right <= left` (empty or one element). Otherwise, compute the middle index as `(left + right) / 2`, recursively sort the left half `[left, middle]` and right half `[middle+1, right]`, then merge the two sorted halves using a temporary `std::vector` for storage. During the merge, we copy the left half and right half into separate vectors using `generate` with a lambda that captures the container and an index (incrementing it via `mutable`). We then compare elements using a `LessThan`-style predicate (defaulting to `operator<`) and write back to the container via `at()`, which provides bounds checking. Edge cases include empty containers (size 0) and single-element containers—the base case handles these because `container.size()-1` would be `-1` for an empty vector if passed as `size_type` (unsigned), so we must guard by checking for empty before computing `right`. Time complexity is O(n log n) for all cases, and space complexity is O(n) due to the temporary vectors in each merge (though with recursion depth O(log n), total auxiliary memory is O(n) at any point). The implementation is correct for containers with `value_type`, `size_type`, and `at()` support.
