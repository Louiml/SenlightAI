Write a C++ function named `computeSumUntilLimit` that takes an integer `limit` as input and returns the sum of all integers from 0 up to but not including `limit`. The function must handle negative limits by returning 0 (since the loop would never execute because `i == limit` is never reached when `i` starts at 0 and increases). For a non-negative limit, iterate from 0 upward, adding each integer to a running total, and break the loop when the current integer equals `limit`. The function should use a `for` loop with an explicit `break` statement, mirroring the logic in the provided snippet. Ensure the function is `const`-correct in the sense that it does not modify its parameter. Provide the function signature as `int computeSumUntilLimit(int limit)`. The implementation must be self-contained, including any necessary standard headers, and must not include a `main` function.

#include <cassert>

int main() {
    assert(computeSumUntilLimit(0) == 0);
    assert(computeSumUntilLimit(-5) == 0);
    assert(computeSumUntilLimit(1) == 0);  // sum from 0 to 0 = 0
    assert(computeSumUntilLimit(2) == 1);  // 0 + 1
    assert(computeSumUntilLimit(5) == 10); // 0+1+2+3+4
    assert(computeSumUntilLimit(10) == 45); // 0..9 sum
    assert(computeSumUntilLimit(100) == 4950); // matches snippet's loop sum when limit=100
}

#include <iostream>

// Compute the sum of all integers from 0 up to but not including limit.
// Returns 0 for limit <= 0.
int computeSumUntilLimit(int limit) {
    if (limit <= 0) {
        return 0;
    }
    int sum = 0;
    for (int i = 0; ; ++i) {  // infinite loop, break when i == limit
        if (i == limit) {
            break;
        }
        sum += i;
    }
    return sum;
}

// The solution directly follows the loop pattern from the snippet: initialize `sum` to 0, then iterate `i` from 0 to 99 (or more generally, up to `limit`), checking if `i == limit` and breaking if so; otherwise add `i` to `sum`. For `limit <= 0`, the condition `i == limit` is never true because `i` starts at 0 and increases positively, but since the loop continues to 99, the sum would be the sum from 0 to 99 (which is 4950). However, the task specifies that negative limits should return 0, so we must add an early return `if (limit <= 0) return 0;` to handle that edge case. For positive limits, the loop runs from `i = 0` up to `i = limit - 1` (since when `i == limit`, it breaks), so the sum is `0 + 1 + ... + (limit - 1) = limit * (limit - 1) / 2`. The loop's upper bound of 99 in the snippet is arbitrary; to make it general, we can loop while `true` or use a large upper bound like `INT_MAX`, but safer to loop from 0 upward indefinitely until break. The time complexity is O(limit) for positive limits, and O(1) for non-positive. Space complexity is O(1).
