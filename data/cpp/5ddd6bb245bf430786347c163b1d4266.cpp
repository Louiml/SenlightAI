Write a C++ function that accepts a pointer to a constant integer array and its size, and returns a dynamically allocated array where each element at index `i` contains the sum of all elements from index `0` through index `i` of the original array (i.e., a prefix-sum / running-total array). The function must handle a valid non-empty array (size > 0) and return a `nullptr` for a size of 0 or less. The caller is responsible for managing the returned memory. The function should not modify the original array and should use `const` correctness where appropriate. Additionally, provide a separate helper that prints the elements of an array space-separated without a trailing space, but that helper is only for demonstration and can be omitted from the solution function. The core task is to implement the prefix-sum array generation with proper dynamic allocation and edge-case handling.
#include <cassert>
#include <cstddef>

// Solution function (must be copied/pasted here or included via header)
int* buildPrefixSum(const int* data, int num) {
    if (num <= 0 || data == nullptr) {
        return nullptr;
    }
    int* result = new int[num];
    int runningSum = 0;
    for (int i = 0; i < num; ++i) {
        runningSum += data[i];
        result[i] = runningSum;
    }
    return result;
}

int main() {
    // Test 1: Simple positive numbers
    int arr1[] = {1, 2, 3, 4};
    int* res1 = buildPrefixSum(arr1, 4);
    assert(res1[0] == 1);
    assert(res1[1] == 3);
    assert(res1[2] == 6);
    assert(res1[3] == 10);
    delete[] res1;

    // Test 2: Includes negative and zero
    int arr2[] = {5, -3, 0, 7};
    int* res2 = buildPrefixSum(arr2, 4);
    assert(res2[0] == 5);
    assert(res2[1] == 2);
    assert(res2[2] == 2);
    assert(res2[3] == 9);
    delete[] res2;

    // Test 3: Single element
    int arr3[] = {42};
    int* res3 = buildPrefixSum(arr3, 1);
    assert(res3[0] == 42);
    delete[] res3;

    // Test 4: All negative numbers
    int arr4[] = {-4, -1, -5};
    int* res4 = buildPrefixSum(arr4, 3);
    assert(res4[0] == -4);
    assert(res4[1] == -5);
    assert(res4[2] == -10);
    delete[] res4;

    // Test 5: Edge case - zero or negative size returns nullptr
    int arr5[] = {10, 20};
    assert(buildPrefixSum(arr5, 0) == nullptr);
    assert(buildPrefixSum(arr5, -1) == nullptr);
    assert(buildPrefixSum(nullptr, 5) == nullptr);

    return 0;
}
#include <cstddef>

// Returns a dynamically allocated array where result[i] is the sum of data[0]..data[i].
// Returns nullptr if size is zero or negative. Caller must delete[] the returned pointer.
int* buildPrefixSum(const int* data, int num) {
    if (num <= 0 || data == nullptr) {
        return nullptr;
    }

    int* result = new int[num];
    int runningSum = 0;

    for (int i = 0; i < num; ++i) {
        runningSum += data[i];
        result[i] = runningSum;
    }

    return result;
}
// The solution approach is straightforward: allocate a new integer array of the same size as the input using `new int[num]`. Then iterate over the input array from left to right, maintaining a running total. For each index `i`, add the current input element `data[i]` to the running total and store that total into the result array at position `i`. This produces the prefix sums. Edge cases: if `num <= 0`, return `nullptr` because no valid array can be created; this matches the requirement for invalid sizes. Another edge case is when the array contains negative numbers or zeros—no special handling is needed since the prefix sum simply accumulates arithmetically. Time complexity is \(O(n)\) because each element is visited exactly once. Space complexity is \(O(n)\) for the newly allocated result array, plus \(O(1)\) auxiliary space for the running total and loop variable. Memory management is critical: the returned pointer must be deleted by the caller to avoid leaks. Const correctness: the input pointer is `const int *` so the original data cannot be modified. The function itself does not throw exceptions, but if `new` fails it would throw `std::bad_alloc` (not handled here).
