// Write a standalone C++ function named `swapForwardBackwardBuffers` that takes two integer arrays (`src` and `diffSrc`) of equal length, along with a boolean flag `forwardMode`, and modifies them according to the following rules. If `forwardMode` is `true`, the function copies the elements of `src` into `diffSrc` in reverse order (i.e., `diffSrc[i] = src[n-1-i]`). If `forwardMode` is `false`, it copies `diffSrc` into `src` in reverse order (i.e., `src[i] = diffSrc[n-1-i]`). The function must return a `bool` indicating success (`true`) if the arrays are non-null and have non-zero length, and `false` otherwise. The function should not allocate dynamic memory, should not modify the source arrays in place during the copy (i.e., it must handle overlapping inputs correctly by using a temporary buffer or by reading appropriately), and should operate in-place on the destination array. The signature must be exactly: `bool swapForwardBackwardBuffers(int* src, int* diffSrc, size_t n, bool forwardMode);`

#include <cassert>
#include <vector>

// Forward declaration (assume the function is defined above, but for testing we include it)
bool swapForwardBackwardBuffers(int* src, int* diffSrc, size_t n, bool forwardMode);

int main() {
    // Test 1: basic forward mode
    std::vector<int> src1 = {1, 2, 3, 4};
    std::vector<int> diff1(4, 0);
    assert(swapForwardBackwardBuffers(src1.data(), diff1.data(), src1.size(), true));
    assert(diff1 == (std::vector<int>{4, 3, 2, 1}));
    // src unchanged
    assert(src1 == (std::vector<int>{1, 2, 3, 4}));

    // Test 2: basic backward mode
    std::vector<int> src2(4, 0);
    std::vector<int> diff2 = {10, 20, 30, 40};
    assert(swapForwardBackwardBuffers(src2.data(), diff2.data(), src2.size(), false));
    assert(src2 == (std::vector<int>{40, 30, 20, 10}));
    // diff unchanged
    assert(diff2 == (std::vector<int>{10, 20, 30, 40}));

    // Test 3: single element
    std::vector<int> s3 = {7};
    std::vector<int> d3 = {0};
    assert(swapForwardBackwardBuffers(s3.data(), d3.data(), 1, true));
    assert(d3[0] == 7);
    assert(s3[0] == 7);

    // Test 4: zero length returns false
    int x = 1, y = 2;
    assert(!swapForwardBackwardBuffers(&x, &y, 0, true));

    // Test 5: null pointers return false
    int a = 1;
    assert(!swapForwardBackwardBuffers(nullptr, &a, 1, true));
    assert(!swapForwardBackwardBuffers(&a, nullptr, 1, true));

    // Test 6: overlapping (same pointer) should handle correctly
    std::vector<int> ov = {1, 2, 3, 4, 5};
    assert(swapForwardBackwardBuffers(ov.data(), ov.data(), ov.size(), true));
    assert(ov == (std::vector<int>{5, 4, 3, 2, 1}));

    // Test 7: larger even size backward
    std::vector<int> s7 = {0,0,0,0,0,0};
    std::vector<int> d7 = {100, 200, 300, 400, 500, 600};
    assert(swapForwardBackwardBuffers(s7.data(), d7.data(), 6, false));
    assert(s7 == (std::vector<int>{600, 500, 400, 300, 200, 100}));

    // Test 8: ensure return true on valid
    std::vector<int> s8 = {1,2};
    std::vector<int> d8 = {3,4};
    assert(swapForwardBackwardBuffers(s8.data(), d8.data(), 2, true) == true);

    return 0;
}

#include <vector>
#include <cstddef>

// Copies one array to another in reverse order based on forwardMode.
// Returns true on success, false on invalid input.
bool swapForwardBackwardBuffers(int* src, int* diffSrc, size_t n, bool forwardMode) {
    // Validate inputs
    if (src == nullptr || diffSrc == nullptr || n == 0) {
        return false;
    }

    // If the arrays overlap (same memory), use a temporary copy to avoid corruption.
    if (src == diffSrc) {
        std::vector<int> temp(src, src + n);
        if (forwardMode) {
            for (size_t i = 0; i < n; ++i) {
                diffSrc[i] = temp[n - 1 - i];
            }
        } else {
            for (size_t i = 0; i < n; ++i) {
                src[i] = temp[n - 1 - i];
            }
        }
        return true;
    }

    // Normal case: distinct arrays, direct reversed copy.
    if (forwardMode) {
        for (size_t i = 0; i < n; ++i) {
            diffSrc[i] = src[n - 1 - i];
        }
    } else {
        for (size_t i = 0; i < n; ++i) {
            src[i] = diffSrc[n - 1 - i];
        }
    }
    return true;
}

// The task simulates passing a buffer between forward and backward pass kernels, where the order is reversed (like a resampling operation that processes data in reverse spatial order). The main challenge is to copy one array to the other in reversed order without corrupting the source if the source and destination are the same memory (though in this task they are separate arrays, we still handle general correctness). The algorithm: validate inputs; if `forwardMode` is true, iterate `i` from 0 to `n-1` and assign `diffSrc[i] = src[n-1-i]`. If false, do `src[i] = diffSrc[n-1-i]`. Since source and destination are different arrays, no temporary buffer is strictly needed, but to be safe if the caller passes the same pointer for both, we should detect that and use a temporary vector. Edge cases: null pointers, zero length → return false. For valid inputs, complexity is O(n) time and O(n) auxiliary space if overlapping (worst case), otherwise O(1) space. The solution uses a `std::vector` only in the overlapping case; otherwise direct copies.
