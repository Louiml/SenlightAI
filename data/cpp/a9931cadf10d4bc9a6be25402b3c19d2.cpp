/*
Write a standalone C++ function that measures and returns how many times a `std::vector<int>` reallocates its internal storage (i.e., changes its starting memory address) when pushing a given number of integers, and then compare the behavior with and without calling `reserve`. The function should accept a vector size `n` (positive integer) and a boolean flag `useReserve`. It must push integers `0` through `n-1` into an initially empty `vector<int>`, track the number of times the address of `v[0]` changes during the pushes, and return that count. If `useReserve` is true, call `v.reserve(n)` before the loop. This directly demonstrates the effect of `reserve` on reducing dynamic reallocation. Handle the edge case where `n` is 1 (no reallocation should occur because the first push allocates once). For very large `n` (e.g., 100,000), the function should still be efficient and not use excessive memory beyond the vector itself.
*/
#include <vector>

// Count how many times the vector's internal buffer is reallocated when pushing n integers.
// If useReserve is true, reserve space for n elements before pushing.
// Returns the number of times the address of v[0] changes during the push sequence.
int countReallocations(int n, bool useReserve) {
    std::vector<int> v;
    if (useReserve) {
        v.reserve(n);
    }
    if (n <= 0) {
        return 0;
    }

    // Push first element and set baseline address
    v.push_back(0);
    const int* baseline = &v[0];
    int reallocationCount = 0;

    for (int i = 1; i < n; ++i) {
        v.push_back(i);
        if (&v[0] != baseline) {
            ++reallocationCount;
            baseline = &v[0];
        }
    }
    return reallocationCount;
}
#include <cassert>
#include <vector>

int countReallocations(int n, bool useReserve);

int main() {
    // For small n without reserve, the vector typically grows by doubling, so reallocations are few.
    assert(countReallocations(1, false) == 0);
    assert(countReallocations(10, false) >= 1);
    // With reserve, exactly zero reallocations (initial reserve allocates once, pushes reuse it).
    assert(countReallocations(0, true) == 0);
    assert(countReallocations(1, true) == 0);
    assert(countReallocations(10, true) == 0);
    assert(countReallocations(100000, true) == 0);
    // Large n without reserve should have far more reallocations than with reserve.
    assert(countReallocations(100000, false) > countReallocations(100000, true));
    // Consistency: repeated calls produce same result (deterministic).
    assert(countReallocations(64, false) == countReallocations(64, false));
    // Sanity: for n=2 without reserve, first push allocates, second push may cause one reallocation (typical).
    // But the standard library may preallocate differently, so only check it's non-negative.
    assert(countReallocations(2, false) >= 0);
    // Additional check: n=3 without reserve should have at most 1 reallocation typically.
    assert(countReallocations(3, false) <= 2);
    return 0;
}
// The core idea is to simulate the common pattern of repeatedly calling `push_back` on an empty vector while monitoring the address of the first element. Each time the vector needs to grow its capacity, it typically allocates a new larger block of memory and copies/moves existing elements, causing the starting address to change. By storing a pointer to `v[0]` (or using `&v[0]`) before each push and comparing it after the push, we can detect a reallocation: if the address differs, we increment a counter and update the stored pointer. For `n` pushes, the loop runs exactly `n` times, so time complexity is O(n) regardless of `useReserve`, but without reserve the number of reallocations is O(log n) for typical growth factors (e.g., doubling), while with reserve it is exactly 0 reallocations after the initial allocation (since reserve pre-allocates enough capacity). The space complexity is O(n) for the vector storage, plus O(1) auxiliary. Edge cases: `n=0` (if allowed, loop does nothing, returns 0), `n=1` (first push triggers initial allocation, but address stays same after that, so count remains 0 because we only count changes after the push; careful: we must initialize the pointer after the first push or start by pushing once before monitoring). The approach: push the first element (index 0) first, store its address as the baseline, then loop from 1 to n-1, each time comparing before and after push. This avoids subtle off-by-one errors.
