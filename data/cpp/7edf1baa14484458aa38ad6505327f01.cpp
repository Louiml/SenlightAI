Write a C++ function that takes two integer parameters: a size `n` (non-negative) and a pointer to an integer array `arr` (or `nullptr` if `n` is 0). The function must dynamically allocate a new integer array of size `n` on the heap, copy the contents of the input array into the newly allocated array, and then return a pointer to this new array. The function must not modify the input array, and the caller is responsible for deallocating the returned array using `delete[]`. If `n` is 0, the function should return `nullptr` without allocating. Your solution must demonstrate correct use of dynamic memory, `const` correctness, and handle the edge case where the input pointer is `nullptr` with `n > 0` by returning `nullptr` as well (invalid input).

// The solution involves allocating a new array of size `n` with `new int[n]`, then copying each element from the source array to the destination using a simple loop, and finally returning the destination pointer. The main edge cases are: (1) `n == 0` — return `nullptr` without any allocation; (2) `arr` is `nullptr` but `n > 0` — this is invalid input, so return `nullptr` to indicate failure; (3) normal case — allocate, copy, return. The copying loop runs in \(O(n)\) time, and the space complexity is \(O(n)\) for the new array. Important: no bounds checking is needed because sizes match, and the input array must have at least `n` valid elements (this is the caller’s responsibility). The function should be `const`-correct by taking `const int* arr` to guarantee the input is not modified.

#include <cstddef>

// Return a heap-allocated copy of the given array, or nullptr if size is 0 or input is invalid.
int* copyArray(std::size_t n, const int* arr) {
    if (n == 0) {
        return nullptr;
    }
    if (arr == nullptr) {
        return nullptr;
    }
    int* copy = new int[n];
    for (std::size_t i = 0; i < n; ++i) {
        copy[i] = arr[i];
    }
    return copy;
}

#include <cassert>
#include <cstddef>

// Declaration of the solution function (assume it's provided in the solution section).
int* copyArray(std::size_t n, const int* arr);

int main() {
    // Test 1: Normal copy
    int src1[] = {1, 2, 3};
    int* dst1 = copyArray(3, src1);
    assert(dst1 != nullptr);
    assert(dst1[0] == 1 && dst1[1] == 2 && dst1[2] == 3);
    delete[] dst1;

    // Test 2: Empty array returns nullptr
    assert(copyArray(0, nullptr) == nullptr);

    // Test 3: Null pointer with non-zero size returns nullptr
    assert(copyArray(5, nullptr) == nullptr);

    // Test 4: Copy does not modify source
    int src2[] = {9, -4, 7, 0};
    int* dst2 = copyArray(4, src2);
    assert(dst2 != nullptr);
    assert(src2[0] == 9 && src2[1] == -4 && src2[2] == 7 && src2[3] == 0);
    assert(dst2[0] == 9 && dst2[1] == -4 && dst2[2] == 7 && dst2[3] == 0);
    delete[] dst2;

    // Test 5: Single element
    int src3[] = {42};
    int* dst3 = copyArray(1, src3);
    assert(dst3 != nullptr);
    assert(dst3[0] == 42);
    delete[] dst3;

    // Test 6: Large array with negative values
    int src4[] = {-10, -20, -30, -40, -50};
    int* dst4 = copyArray(5, src4);
    assert(dst4 != nullptr);
    for (int i = 0; i < 5; ++i) {
        assert(dst4[i] == src4[i]);
    }
    delete[] dst4;

    return 0;
}
