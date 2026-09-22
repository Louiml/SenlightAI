// Write a C++ function that takes two integers, `start` and `end`, as parameters and returns the sum of all integers from `start` up to and including `end` (inclusive). The function must handle the case where `start` > `end` by returning 0 (i.e., treating an invalid range as an empty sum). The function should use a loop similar to the provided snippet, but generalized for any range. The function must be const-correct and not modify its input parameters.

// The solution initializes a sum accumulator to 0 and a loop counter to the `start` value. The loop continues while the counter is less than or equal to `end`, adding the counter to the sum and then incrementing it. The key edge case is when `start` is greater than `end`: the loop condition is never true, so the function returns 0, which correctly represents an empty range. Another edge case is when `start` equals `end`: the loop runs exactly once, returning that single value. The algorithm runs in O(n) time, where n = max(0, end - start + 1), and uses O(1) auxiliary space. The function should be declared `const`-correct by passing parameters by value (since they are primitive types) and not modifying them internally beyond using a copy in the loop counter.

// Returns the sum of all integers from start to end inclusive.
// Returns 0 if start > end.
int sumRange(int start, int end) {
    int sum = 0;
    int current = start;
    while (current <= end) {
        sum += current;
        ++current;
    }
    return sum;
}

#include <cassert>

int main() {
    // Basic range
    assert(sumRange(1, 10) == 55);
    // Range starting at 50, ending at 100 (mimics the snippet)
    assert(sumRange(50, 100) == 3825);
    // Single element range
    assert(sumRange(7, 7) == 7);
    // Negative range
    assert(sumRange(-3, 3) == 0);
    // Negative-only range
    assert(sumRange(-5, -1) == -15);
    // start > end should return 0
    assert(sumRange(10, 1) == 0);
    // start > end with equal? already covered
    // Large range to ensure no overflow within int (use smaller for safety)
    assert(sumRange(0, 100) == 5050);
    // Zero range
    assert(sumRange(0, 0) == 0);
    // Negative start, positive end
    assert(sumRange(-2, 2) == 0);
    // Large negative to large positive (sum should be 0)
    assert(sumRange(-10, 10) == 0);
    return 0;
}
