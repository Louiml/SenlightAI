Write a C++ function `int minimalCoinPairs(int c, int d)` that, given two non-negative integers `c` and `d`, determines the minimum number of operations needed to make both numbers equal to zero. In one operation, you may add the same integer `x` (where `x` can be any integer, possibly negative, and the same `x` is added to both `c` and `d`) to both numbers. The function should return the minimum number of operations, or `-1` if it is impossible to make both numbers zero. For example, if `c = 1` and `d = 1`, you can add `-1` to both once, so the answer is `1`. If `c = 0` and `d = 0`, zero operations are needed. If `c = 1` and `d = 2`, one operation cannot fix the difference because the same `x` is added to both, so it becomes impossible to reach zero simultaneously; the answer is `-1`. The function should handle all non-negative integer inputs including very large values (up to `2^31 - 1`), using `long long` for safety.
#include <cassert>

int main() {
    assert(minimalCoinPairs(0, 0) == 0);
    assert(minimalCoinPairs(1, 1) == 1);
    assert(minimalCoinPairs(5, 5) == 1);
    assert(minimalCoinPairs(0, 1) == -1);
    assert(minimalCoinPairs(1, 2) == -1);
    assert(minimalCoinPairs(100000, 100000) == 1);
    assert(minimalCoinPairs(100000, 99999) == -1);
    assert(minimalCoinPairs(0, 0) == 0);
    assert(minimalCoinPairs(2, 3) == -1);
    assert(minimalCoinPairs(7, 7) == 1);
}
#include <cstdint>
#include <cstdlib>

// Determine the minimum number of operations to make both c and d zero,
// where each operation adds the same integer to both. Returns -1 if impossible.
long long minimalCoinPairs(long long c, long long d) {
    if (c == 0 && d == 0) {
        return 0;
    }
    if (c == d) {
        return 1;
    }
    return -1;
}
// The key observation is that since the same value `x` is added to both `c` and `d` in each operation, the difference `c - d` remains invariant. Therefore, after any number of operations, the two numbers will still differ by the same amount. To reach both being zero, their difference must be zero at the end, which requires the initial difference to be zero. If `c != d`, it is impossible, and we return `-1`. If `c == d`, we need to reduce both to zero. If both are zero, we need 0 operations. If both are equal and positive, we need exactly one operation: add `-c` (or `-d`) to both, bringing them both to zero. This solution is based on the invariant of the difference. Edge cases: `(c=0, d=0)` returns 0; `(c=1, d=1)` returns 1; `(c=0, d=5)` returns -1 because difference is non-zero; `(c=5, d=5)` returns 1. The algorithm runs in O(1) time and O(1) auxiliary space, independent of input size.
