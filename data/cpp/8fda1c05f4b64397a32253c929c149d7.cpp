/*
Write a C++ function named `computeAverageWithPointers` that takes a fixed-size C-style array of 10 `double` values and returns their arithmetic mean as a `double`. The function must traverse the array using a pointer (not array indexing), accumulate the sum of all elements through dereferencing the pointer at each step, and return the average. You may assume the array always contains exactly 10 valid `double` values; however, the function should be written robustly so that it works for any array size by accepting the array and its size as parameters instead of hardcoding 10. The function must not modify the original array and must use `const` correctness appropriately. Include only the function definition, with necessary headers, and no `main` function.
*/

#include <cstddef>

// Compute the average of a double array using pointer traversal.
// Returns 0.0 for an empty array (size 0).
double computeAverageWithPointers(const double* arr, std::size_t size) {
    if (size == 0) {
        return 0.0;
    }

    const double* p = arr;      // pointer to traverse the array
    double sum = 0.0;

    for (std::size_t i = 0; i < size; ++i) {
        sum += *p;              // dereference to get current value
        ++p;                    // move to next element
    }

    return sum / static_cast<double>(size);
}

#include <cassert>
#include <cmath>

int main() {
    // Test case 1: Example from the original snippet
    double arr1[10] = { 0.1, 2.0, 3.4, 5.2, 4.5, 7.8, 9.7, 1.4, 6.6, 7.2 };
    double expected1 = (0.1 + 2.0 + 3.4 + 5.2 + 4.5 + 7.8 + 9.7 + 1.4 + 6.6 + 7.2) / 10.0;
    assert(std::abs(computeAverageWithPointers(arr1, 10) - expected1) < 1e-9);

    // Test case 2: All zeros
    double arr2[5] = { 0.0, 0.0, 0.0, 0.0, 0.0 };
    assert(computeAverageWithPointers(arr2, 5) == 0.0);

    // Test case 3: Single element
    double arr3[1] = { 42.5 };
    assert(computeAverageWithPointers(arr3, 1) == 42.5);

    // Test case 4: Negative values
    double arr4[3] = { -1.0, -2.0, -3.0 };
    assert(computeAverageWithPointers(arr4, 3) == -2.0);

    // Test case 5: Empty array (size 0) returns 0.0
    assert(computeAverageWithPointers(nullptr, 0) == 0.0);

    return 0;
}

// The solution uses a pointer to iterate through the array from the first element to the last. Initialize a pointer `p` to the first element of the array, and a `double sum` to 0.0. In a loop that runs `size` times, dereference the pointer to get the current value, add it to `sum`, then increment the pointer to point to the next element. After the loop, return `sum / size`. Edge cases: if `size` is 0, the result is undefined—it is recommended to return 0.0 or assert that `size > 0`; since the task specifies a fixed array of 10, but the function accepts a size parameter, the implementation should handle `size == 0` gracefully by returning 0.0 (or by documenting that the caller must ensure non‑zero size). Time complexity is O(n), where n is the number of elements, because each element is visited exactly once. Space complexity is O(1) additional space, as only a pointer and a sum variable are used, independent of input size.
