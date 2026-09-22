Write a C++ function template `sortDescending` that accepts a raw pointer to an array of elements, the number of elements, and a comparator object (derived from a pure virtual `Comparator<T>` interface). The function must sort the array in descending order according to the comparator's `operator()` semantics (where a positive return means the first argument should come after the second). The sort must be performed in-place and must not use any STL sorting algorithms. The comparator defines the ordering logic; for example, `IntComparator` returns `rha - lha`, which yields a descending numeric sort. Your implementation must handle arrays of any size (including 0 and 1), must not leak memory, and must work with any trivially copyable type `T` (e.g., `int`, `double`, `char`). The function should have signature `template<typename T> void sortDescending(T* array, size_t size, Comparator<T>& comp)`. Provide a complete standalone implementation without a `main` function. The solution should be robust, efficient, and clearly commented.

#include <cassert>
#include <iostream>

// Concrete comparator for integers: returns rhs - lhs (descending order).
struct IntComparator final : Comparator<int> {
    int operator()(int const& lhs, int const& rhs) const override {
        return rhs - lhs;
    }
};

// Concrete comparator for doubles (descending).
struct DoubleComparator final : Comparator<double> {
    int operator()(double const& lhs, double const& rhs) const override {
        if (lhs < rhs) return 1;
        if (lhs > rhs) return -1;
        return 0;
    }
};

int main() {
    // Test 1: Normal unsorted array
    int a[] = {1, 9, 5, 6, 3, 4, 10, 12, 0, 7};
    IntComparator intComp;
    sortDescending(a, 10, intComp);
    int expected1[] = {12, 10, 9, 7, 6, 5, 4, 3, 1, 0};
    for (size_t i = 0; i < 10; ++i) {
        assert(a[i] == expected1[i]);
    }

    // Test 2: Already sorted descending
    int b[] = {5, 4, 3, 2, 1};
    sortDescending(b, 5, intComp);
    int expected2[] = {5, 4, 3, 2, 1};
    for (size_t i = 0; i < 5; ++i) {
        assert(b[i] == expected2[i]);
    }

    // Test 3: Reverse order (ascending input → descending output)
    int c[] = {1, 2, 3, 4, 5};
    sortDescending(c, 5, intComp);
    int expected3[] = {5, 4, 3, 2, 1};
    for (size_t i = 0; i < 5; ++i) {
        assert(c[i] == expected3[i]);
    }

    // Test 4: Single element
    int d[] = {42};
    sortDescending(d, 1, intComp);
    assert(d[0] == 42);

    // Test 5: Empty array (size 0) – should not crash
    int e[] = {};
    sortDescending(e, 0, intComp);

    // Test 6: Duplicate values
    int f[] = {3, 1, 3, 2, 3, 1};
    sortDescending(f, 6, intComp);
    int expected6[] = {3, 3, 3, 2, 1, 1};
    for (size_t i = 0; i < 6; ++i) {
        assert(f[i] == expected6[i]);
    }

    // Test 7: Negative numbers
    int g[] = {-5, -1, -10, 0};
    sortDescending(g, 4, intComp);
    int expected7[] = {0, -1, -5, -10};
    for (size_t i = 0; i < 4; ++i) {
        assert(g[i] == expected7[i]);
    }

    // Test 8: Doubles
    double h[] = {2.5, 0.1, 3.7, -1.2};
    DoubleComparator doubleComp;
    sortDescending(h, 4, doubleComp);
    double expected8[] = {3.7, 2.5, 0.1, -1.2};
    for (size_t i = 0; i < 4; ++i) {
        assert(h[i] == expected8[i]);
    }

    // Test 9: All equal values
    int i_arr[] = {7, 7, 7, 7};
    sortDescending(i_arr, 4, intComp);
    for (size_t i = 0; i < 4; ++i) {
        assert(i_arr[i] == 7);
    }

    // Test 10: Large array (1000 elements) for basic correctness
    int large[1000];
    for (size_t i = 0; i < 1000; ++i) {
        large[i] = (i * 37) % 100;  // pseudo-random
    }
    sortDescending(large, 1000, intComp);
    for (size_t i = 1; i < 1000; ++i) {
        assert(large[i-1] >= large[i]);
    }

    std::cout << "All tests passed!" << std::endl;
    return 0;
}

#include <utility>  // for std::swap
#include <cstddef>  // for size_t

// Abstract comparator interface: returns >0 if lhs should come after rhs.
template <typename T>
struct Comparator {
    virtual int operator()(T const& lhs, T const& rhs) const = 0;
    virtual ~Comparator() = default;
};

// Helper function: partitions array[low..high] around pivot (last element).
// Returns the index of the pivot after partition.
template <typename T>
size_t partition(T* array, size_t low, size_t high, Comparator<T>& comp) {
    T pivot = array[high];
    size_t i = low;  // boundary for elements that should be before pivot

    for (size_t j = low; j < high; ++j) {
        // If array[j] should come before pivot (descending order), swap.
        // comp(pivot, array[j]) <= 0 means pivot is "not greater than" array[j],
        // so array[j] should be on the left side.
        if (comp(pivot, array[j]) <= 0) {
            std::swap(array[i], array[j]);
            ++i;
        }
    }
    std::swap(array[i], array[high]);  // place pivot in correct position
    return i;
}

// Recursive quicksort using the comparator for descending order.
template <typename T>
void quickSortHelper(T* array, size_t low, size_t high, Comparator<T>& comp) {
    if (low < high) {
        size_t pivotIndex = partition(array, low, high, comp);
        // Recurse on left segment (before pivot) if it has at least 2 elements.
        if (pivotIndex > low) {
            quickSortHelper(array, low, pivotIndex - 1, comp);
        }
        // Recurse on right segment (after pivot) if it has at least 2 elements.
        if (pivotIndex + 1 < high) {
            quickSortHelper(array, pivotIndex + 1, high, comp);
        }
    }
}

// Main sorting function: sorts array of given size in descending order.
// Handles size 0 and 1 gracefully.
template <typename T>
void sortDescending(T* array, size_t size, Comparator<T>& comp) {
    if (size <= 1) {
        return;
    }
    quickSortHelper(array, 0, size - 1, comp);
}

// The core challenge is implementing a correct in‑place sorting algorithm that respects a virtual comparator. The provided code sketch contains a buggy attempt at a quicksort-like partition: it uses a fixed pivot (last element) and attempts a single‑pass partition, but the logic is flawed (e.g., it swaps with `memcpy` which is fine for trivially copyable types, but the pointer arithmetic and flag handling produce incorrect orderings for many inputs). A safer approach is to implement a standard quicksort with Hoare or Lomuto partition scheme, adapted to use the comparator interface. The comparator returns a positive value when the first argument should come after the second (i.e., for descending sort, we need to swap when `comp(a, b) > 0`). We can implement Lomuto partition: choose the last element as pivot, maintain an index `i` for the boundary of elements that should be placed before the pivot, and iterate `j` from start to `end-1`. If `comp(pivot, array[j]) <= 0` (meaning `array[j]` should be before the pivot in descending order), swap `array[i]` and `array[j]` and increment `i`. After the loop, swap `array[i]` with the pivot. Then recurse on the left segment `[start, i-1]` and right segment `[i+1, end]`. Base case: if `size <= 1`, return. Edge cases: size 0 (do nothing), size 1 (already sorted), duplicate values (comparator returns 0, handled by `<= 0` condition to avoid infinite recursion), all elements equal (pivot ends up at one end, recursion depth linear, but still correct). Time complexity is average \(O(n \log n)\), worst-case \(O(n^2)\) if pivot is poor (e.g., already sorted descending order — but since we sort descending, if input is already ascending, pivot is smallest, leading to worst case; to mitigate, we could choose a random pivot or median‑of‑three, but for simplicity we can accept worst case as the problem is about correctness). Space complexity is \(O(\log n)\) average recursion stack, worst \(O(n)\). We use `std::swap` from `<utility>` instead of custom `memcpy`, which is safe for trivially copyable types and requires no manual buffer. All parameters are `const` where appropriate (e.g., `size_t size`), and the function itself is not `const` because it modifies the array.
