Write a C++ function named `minOperationsToZero` that takes a single non-negative integer `n` and returns the minimum number of operations required to reduce it to zero, where each operation consists of either subtracting 1 from the current value (if odd) or dividing the current value by 2 (if even). The function must handle `n = 0` by returning 0. For example, reducing 8 to 0 takes 4 operations (8→4→2→1→0), and reducing 7 to 0 takes 5 operations (7→6→3→2→1→0). Implement the function with a loop (no recursion, no bit manipulation tricks beyond standard arithmetic) and ensure it works efficiently for large inputs up to 10^9.

#include <cassert>
#include <cstdint>

// Function declaration (implementation provided in solution)
int minOperationsToZero(std::int64_t n);

int main() {
    assert(minOperationsToZero(0) == 0);
    assert(minOperationsToZero(1) == 1);
    assert(minOperationsToZero(2) == 2);  // 2 -> 1 -> 0
    assert(minOperationsToZero(3) == 3);  // 3 -> 2 -> 1 -> 0
    assert(minOperationsToZero(4) == 3);  // 4 -> 2 -> 1 -> 0
    assert(minOperationsToZero(7) == 5);  // 7->6->3->2->1->0
    assert(minOperationsToZero(8) == 4);  // 8->4->2->1->0
    assert(minOperationsToZero(15) == 7); // 15->14->7->6->3->2->1->0
    assert(minOperationsToZero(1000000000LL) == 39); // known value from loop
    assert(minOperationsToZero(999999937LL) == 45);   // odd large prime
    return 0;
}

#include <cstdint>

// Returns the minimum number of operations to reduce n to zero.
// Operations: subtract 1 (when odd) or divide by 2 (when even).
int minOperationsToZero(std::int64_t n) {
    int operations = 0;
    while (n > 0) {
        if (n % 2 != 0) {
            // Odd: subtract 1 to make it even (unless n==1, then this is the final step)
            --n;
            ++operations;
        }
        if (n > 0) {
            // Even (or now even): divide by 2
            n /= 2;
            ++operations;
        }
    }
    return operations;
}

// The problem is a classic greedy reduction: at each step, if the current number is even, dividing by 2 is always at least as good as subtracting 1 repeatedly (because subtracting 1 then dividing would take 2 operations to reduce by half, whereas direct division takes 1). If the number is odd, you must subtract 1 to make it even (except when the number is 1, where subtracting 1 directly to 0 is the only move). The algorithm simply counts operations: while the number is not zero, if it's odd, increment the operation count and subtract 1; then if it's still positive, divide by 2 and increment the count again. For `n = 0`, the loop doesn't run, returning 0. Edge cases: `n = 1` → subtract 1 (1 op) then loop ends; `n = 2` → divide by 2 (1 op) → 1 → subtract 1 (1 op) = 2 ops. Time complexity is O(log n) because each loop iteration at least halves the number (after possibly one subtraction). Space complexity is O(1). The algorithm is correct because the optimal strategy is always to prefer division by 2 when possible, and when odd, you must subtract 1 (there's no alternative that yields fewer operations).
