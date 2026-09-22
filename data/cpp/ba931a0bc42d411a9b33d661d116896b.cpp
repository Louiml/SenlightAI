/*
Write a C++ function `find_subarray` that determines whether a given sequence `B` of integers appears as a contiguous subarray within another sequence `A`. The function should take four parameters: a pointer to the first element of `A`, the size of `A`, a pointer to the first element of `B`, and the size of `B`. It should return a `const int*` pointing to the first element of the matching subarray in `A` if `B` is found, or `nullptr` if not. An empty `B` (size 0) should be considered a match, returning a pointer to the beginning of `A`. The function must not modify either input array and must handle edge cases such as `B` being larger than `A`, negative numbers, and duplicate values gracefully.
*/

#include <cstddef>

// Find the first occurrence of subarray B in array A.
// Returns a pointer to the first element of the match in A, or nullptr if not found.
// An empty B is considered a match, returning a pointer to the beginning of A.
const int* find_subarray(const int* A, std::size_t sizeA, const int* B, std::size_t sizeB) {
    if (sizeB == 0) {
        return A;
    }
    if (sizeB > sizeA) {
        return nullptr;
    }
    for (std::size_t i = 0; i <= sizeA - sizeB; ++i) {
        bool match = true;
        for (std::size_t j = 0; j < sizeB; ++j) {
            if (A[i + j] != B[j]) {
                match = false;
                break;
            }
        }
        if (match) {
            return &A[i];
        }
    }
    return nullptr;
}

#include <cassert>

// function declaration
const int* find_subarray(const int* A, std::size_t sizeA, const int* B, std::size_t sizeB);

int main() {
    // Basic match in middle
    int A1[] = {1, 2, 3, 4, 5};
    int B1[] = {3, 4};
    assert(find_subarray(A1, 5, B1, 2) == &A1[2]);

    // Match at beginning
    int B2[] = {1, 2};
    assert(find_subarray(A1, 5, B2, 2) == &A1[0]);

    // Match at end
    int B3[] = {4, 5};
    assert(find_subarray(A1, 5, B3, 2) == &A1[3]);

    // No match
    int B4[] = {6, 7};
    assert(find_subarray(A1, 5, B4, 2) == nullptr);

    // Empty B matches
    assert(find_subarray(A1, 5, nullptr, 0) == &A1[0]);

    // B larger than A
    int B5[] = {1, 2, 3, 4, 5, 6};
    assert(find_subarray(A1, 5, B5, 6) == nullptr);

    // Overlapping pattern, should find first occurrence
    int A2[] = {1, 2, 1, 2, 1};
    int B6[] = {1, 2, 1};
    assert(find_subarray(A2, 5, B6, 3) == &A2[0]);

    // Negative numbers
    int A3[] = {-1, -2, -3, -4};
    int B7[] = {-2, -3};
    assert(find_subarray(A3, 4, B7, 2) == &A3[1]);

    // Single element arrays
    int A4[] = {42};
    int B8[] = {42};
    assert(find_subarray(A4, 1, B8, 1) == &A4[0]);

    int B9[] = {43};
    assert(find_subarray(A4, 1, B9, 1) == nullptr);

    return 0;
}

// The straightforward approach is a nested loop: for each possible starting index `i` in `A` (from `0` to `sizeA - sizeB` inclusive), check whether every element of `B` matches the corresponding element of `A[i + j]` for `j = 0` to `sizeB - 1`. If a mismatch occurs, break early and try the next starting index. If all elements match, return `&A[i]`. If no starting index works, return `nullptr`. For the empty `B` case, the loop condition `i <= sizeA - sizeB` becomes `i <= sizeA` because `sizeB = 0`, so the first iteration checks `i = 0` and the inner loop never runs, automatically returning `&A[0]`. This is correct, but to be explicit, we can handle `sizeB == 0` at the top by returning `A`. The time complexity is `O(sizeA * sizeB)` in the worst case (e.g., when `B` does not appear until the end or does not appear at all). The space complexity is `O(1)` beyond the input arrays, as only a few integer variables are used.
