Write a C++ function `int nonInterleavedIncrements(std::vector<int>& a, int l1, int l2, int i, int j)` that simulates the given code snippet under a strict two-lock discipline: the function must perform `a[i]++` while holding `l1`, then release `l1`, then perform `a[j]--` while holding `l2`, then release `l2`. The input is a vector of integers `a`, two integers `i` and `j` (valid indices), and two mutexes passed by reference (`std::mutex& l1, std::mutex& l2`). The function should return the final value of `a[i]` after the increment. The key constraint: the operation on `a[j]` must happen only after `l1` is fully released, so no deadlock or interleaving with the increment. For simplicity, assume `i` and `j` are valid indices. Your function must use `<mutex>` and `<vector>` only, and must not lock both mutexes at the same time. Provide the solution as a free function with appropriate `const` correctness where possible.

// The solution is straightforward: first lock `l1` using a `std::lock_guard` or manual `lock()`/`unlock()`, then perform `a[i]++`. Immediately after incrementing, release `l1` (either by scope exit or explicit `unlock()`). After `l1` is released, lock `l2`, perform `a[j]--`, then release `l2`. The important edge case is that `i` and `j` could be the same index; this is fine because the operations are sequential, not concurrent. Another edge case: the mutexes may be the same object `l1` and `l2` are the same reference; then locking the same mutex twice sequentially is allowed as long as it is unlocked before re-locking. The function should not throw from locking (but mutex operations can throw only in rare cases; ignore). Time complexity is O(1) (two atomic-like operations), space O(1). The function returns the updated value of `a[i]` after increment.

#include <mutex>
#include <vector>

// Performs a[i]++ while holding l1, releases it, then performs a[j]-- while holding l2.
// Assumes i and j are valid indices. Returns the new value of a[i].
int nonInterleavedIncrements(std::vector<int>& a, std::mutex& l1, std::mutex& l2, int i, int j) {
    // Lock l1 and increment a[i]
    {
        std::lock_guard<std::mutex> guard1(l1);
        a[i]++;
    } // l1 released here

    // Now lock l2 and decrement a[j]
    {
        std::lock_guard<std::mutex> guard2(l2);
        a[j]--;
    } // l2 released here

    return a[i];
}

#include <cassert>
#include <mutex>
#include <vector>

// The solution function is defined above (include it here or rely on prior definition).

int main() {
    std::mutex m1, m2;
    
    // Basic test: distinct indices
    std::vector<int> a = {0, 10};
    int result = nonInterleavedIncrements(a, m1, m2, 0, 1);
    assert(result == 1);
    assert(a[0] == 1 && a[1] == 9);
    
    // Test same index: i == j
    std::vector<int> b = {5};
    result = nonInterleavedIncrements(b, m1, m2, 0, 0);
    assert(result == 6);
    assert(b[0] == 5); // because after increment (6) then decrement (5) -> final 5, but returned 6
    
    // Test with negative and zero values
    std::vector<int> c = {-3, 0, 7};
    result = nonInterleavedIncrements(c, m1, m2, 0, 2);
    assert(result == -2); // -3+1 = -2
    assert(c[0] == -2 && c[2] == 6); // 7-1 = 6
    
    // Test repeated calls (sequential)
    std::vector<int> d = {10, 20};
    result = nonInterleavedIncrements(d, m1, m2, 1, 0);
    assert(result == 21);
    result = nonInterleavedIncrements(d, m1, m2, 1, 0);
    assert(result == 22);
    assert(d[0] == 8 && d[1] == 22); // 20->21->22, 10->9->8
    
    // Test when l1 and l2 are the same mutex (allowed because sequential locking)
    std::mutex single;
    std::vector<int> e = {0, 0};
    result = nonInterleavedIncrements(e, single, single, 0, 1);
    assert(result == 1);
    assert(e[0] == 1 && e[1] == -1);
    
    return 0;
}
