// Given a collection of interval pairs representing the start and end indices of memory accesses in a loop (each with a `stride` of either +1 or -1, and a `size` in bytes), write a C++ function that determines whether a given store-to-load forwarding candidate is valid by checking that the dependence distance between the store pointer and load pointer is exactly one iteration. More precisely, given a `loadPtrStart`, `storePtrStart` (both signed 64-bit integers representing the starting addresses of the load and store), a common `stride` (either +1 or -1, meaning the address changes by `stride * size` each iteration), and a `size` (positive byte size), return `true` if and only if `storePtrStart - loadPtrStart == size * stride`. The function must also verify that the stride is exactly ±1, and that size is positive. The result should be a boolean.
// The task isolates the core arithmetic check from the LLVM loop-load-elimination pass. The key idea is that a store at address `storePtrStart` can forward to a load at `loadPtrStart` in the next iteration if, after one loop iteration, the load's address equals the store's address. Since both pointers advance by `stride * size` per iteration (with stride ±1), the required condition is that the store address is exactly one iteration ahead of the load address. In terms of starting addresses, this means `storePtrStart == loadPtrStart + size * stride`. Rearranging gives `storePtrStart - loadPtrStart == size * stride`. We must also reject invalid strides (anything other than ±1) and non-positive sizes. The solution is a direct arithmetic check with a few guard conditions. Time complexity is O(1), space O(1). Edge cases: stride = 0, |stride| > 1, size = 0, size negative, and potential overflow of the subtraction or multiplication—for a teaching task, we assume the inputs fit in a 64-bit signed integer and that the difference does not overflow, but we can add a note for safety.
#include <cstdint>
#include <cstdlib> // for std::abs

/**
 * @brief Determine if a store-to-load forwarding candidate has a dependence distance of exactly one.
 * 
 * A store at `storePtrStart` forwards to a load at `loadPtrStart` if, after one
 * loop iteration, the load's address equals the store's address. Both addresses
 * change by `stride * size` each iteration. The stride must be exactly +1 or -1,
 * and the size must be positive.
 *
 * @param loadPtrStart  Starting address of the load (signed 64-bit).
 * @param storePtrStart Starting address of the store (signed 64-bit).
 * @param stride        The loop stride, must be +1 or -1.
 * @param size          The byte size of each access, must be positive.
 * @return true if storePtrStart - loadPtrStart == size * stride, false otherwise.
 */
bool isDependenceDistanceOfOne(std::int64_t loadPtrStart,
                               std::int64_t storePtrStart,
                               std::int64_t stride,
                               std::int64_t size) {
    // Validate the stride: must be exactly ±1.
    if (stride != 1 && stride != -1)
        return false;
    // Validate the size: must be positive.
    if (size <= 0)
        return false;

    // The difference must equal size * stride.
    // Use multiplication to avoid division and potential rounding issues.
    return (storePtrStart - loadPtrStart) == (size * stride);
}
#include <cassert>
#include <cstdint>

// Declaration of the function under test
bool isDependenceDistanceOfOne(std::int64_t loadPtrStart,
                               std::int64_t storePtrStart,
                               std::int64_t stride,
                               std::int64_t size);

int main() {
    // Basic case: stride +1, size 4, store is one element ahead.
    assert(isDependenceDistanceOfOne(1000, 1004, 1, 4) == true);
    // Basic case: stride -1, size 4, store is one element behind (i.e., address less).
    assert(isDependenceDistanceOfOne(1000, 996, -1, 4) == true);
    // Not exactly one distance: distance is two elements.
    assert(isDependenceDistanceOfOne(1000, 1008, 1, 4) == false);
    // Opposite direction: with stride +1, store behind does not forward.
    assert(isDependenceDistanceOfOne(1000, 996, 1, 4) == false);
    // Stride of 0 is invalid.
    assert(isDependenceDistanceOfOne(1000, 1000, 0, 4) == false);
    // Stride of 2 is invalid.
    assert(isDependenceDistanceOfOne(1000, 1008, 2, 4) == false);
    // Zero size is invalid.
    assert(isDependenceDistanceOfOne(1000, 1000, 1, 0) == false);
    // Negative size is invalid.
    assert(isDependenceDistanceOfOne(1000, 1004, 1, -4) == false);
    // Larger size with stride -1.
    assert(isDependenceDistanceOfOne(2000, 1992, -1, 8) == true);
    // Edge: zero-size already tested, but also check a large size.
    assert(isDependenceDistanceOfOne(1'000'000, 1'000'032, 1, 32) == true);
    // Non-matching size and stride combination.
    assert(isDependenceDistanceOfOne(100, 108, 1, 4) == false); // distance 8 vs 4
    return 0;
}
