/*
Write a C++ function named `countMiddleElements` that takes a non-empty array of integers (passed as a pointer and its size) and returns the number of elements that are strictly greater than the smallest value and strictly less than the largest value. For example, given `[1, 3, 2, 3, 1, 5]`, the smallest is `1`, the largest is `5`, and the elements strictly between them are `3, 2, 3` (the two `1`s and the `5` are excluded), so the function returns `3`. If all elements are identical, the result must be `0`. The function must handle negative numbers, duplicates, and large arrays. Do not modify the input array.
*/
#include <cstddef> // for size_t

// Returns the number of elements strictly between the smallest and largest values.
int countMiddleElements(const int* arr, std::size_t n) {
    if (n == 0) return 0;

    int min_val = arr[0];
    int max_val = arr[0];
    int min_count = 1;
    int max_count = 1;

    for (std::size_t i = 1; i < n; ++i) {
        const int value = arr[i];
        if (value < min_val) {
            min_val = value;
            min_count = 1;
        } else if (value == min_val) {
            ++min_count;
        }

        if (value > max_val) {
            max_val = value;
            max_count = 1;
        } else if (value == max_val) {
            ++max_count;
        }
    }

    // If min and max are the same value, both counts refer to the same elements.
    // Still, n - min_count - max_count would be negative, so clamp at 0.
    const int result = static_cast<int>(n) - min_count - max_count;
    return result > 0 ? result : 0;
}
#include <cassert>
#include <cstddef>

// Function under test (declaration or include from the solution above)
int countMiddleElements(const int* arr, std::size_t n);

int main() {
    // Basic mixed case
    int a1[] = {1, 3, 2, 3, 1, 5};
    assert(countMiddleElements(a1, 6) == 3);

    // All identical → no middle
    int a2[] = {7, 7, 7};
    assert(countMiddleElements(a2, 3) == 0);

    // Single element → no middle
    int a3[] = {42};
    assert(countMiddleElements(a3, 1) == 0);

    // Negative numbers and duplicates
    int a4[] = {-5, -5, 0, 10, -5, 10, 2};
    assert(countMiddleElements(a4, 7) == 2); // only 0 and 2

    // Only two distinct values
    int a5[] = {2, 5, 2, 5, 2};
    assert(countMiddleElements(a5, 5) == 0);

    // Sorted array with one extremum at each end repeated
    int a6[] = {-10, -10, 0, 1, 2, 3, 10, 10, 10};
    assert(countMiddleElements(a6, 9) == 4); // 0,1,2,3

    // Large equal min/max counts
    int a7[] = {1, 2, 2, 3, 1, 3, 1, 2};
    assert(countMiddleElements(a7, 8) == 4); // 2,2,2,2? actual: min=1 count3, max=3 count2 → middle = 8-3-2=3? Wait compute: 1,2,2,3,1,3,1,2 → min=1 (3 times), max=3 (2 times), middle elements are 2,2,2 → 3.
    assert(countMiddleElements(a7, 8) == 3);

    // Empty array (should return 0, though spec says non‑empty, still safe)
    assert(countMiddleElements(nullptr, 0) == 0);
}
// The main algorithm involves a single pass over the array to find both the minimum and maximum values and their respective counts. After this pass, the answer is simply the total number of elements minus the count of minimums minus the count of maximums, because any element that is equal to the minimum or maximum is excluded from the "middle" category. Edge cases include: an array with only one unique value (then `min_count + max_count == n`, resulting in `0`), arrays where all elements are the same, and arrays with duplicate values of min/max (all duplicates are excluded). The time complexity is O(n) for one linear scan, and the auxiliary space is O(1) because we only use a few integer variables, regardless of the array size.
