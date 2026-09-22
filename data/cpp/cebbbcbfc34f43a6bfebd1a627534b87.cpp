Write a C++ function named `heightDifference` that takes two integers representing the heights of two mountains, `h1` and `h2`, and returns the result of subtracting `h2` from `h1` (i.e., `h1 - h2`). The function should be free of any I/O operations; it should simply compute and return the difference as an integer. The task should handle all possible integer inputs, including negative values, zero, and values that might cause overflow if not considered — but since the problem is straightforward subtraction, use standard `int` arithmetic, and note that typical C++ `int` overflow is undefined behavior, so for a robust standalone exercise, you may assume inputs are within the range `[-1000, 1000]` to avoid overflow. However, the function itself should not impose any such restriction; it just performs the subtraction as given.
// The solution is trivial: directly return `h1 - h2`. Since the problem is a direct arithmetic operation, no additional algorithm is needed. Edge cases include negative differences (when `h2 > h1`), which are handled naturally by integer subtraction. Zero differences occur when both heights are equal. The main consideration is potential integer overflow if both inputs are very large with opposite signs, but for a pedagogical task, we can assume reasonably sized inputs or use `long long` internally to be safe. For simplicity and to match the given snippet exactly, we will use `int` parameters and return type, but we could also use a `long long` return to prevent overflow in extreme cases. The time complexity is `O(1)` and space complexity is `O(1)`.
// Computes the difference between two heights: h1 - h2.
// The result is returned as an integer. No input or output is performed.
int heightDifference(int h1, int h2) {
    return h1 - h2;
}
#include <cassert>

int main() {
    assert(heightDifference(10, 5) == 5);
    assert(heightDifference(5, 10) == -5);
    assert(heightDifference(7, 7) == 0);
    assert(heightDifference(0, 0) == 0);
    assert(heightDifference(-3, 2) == -5);
    assert(heightDifference(100, -100) == 200);
    assert(heightDifference(-5, -10) == 5);
    assert(heightDifference(1, 0) == 1);
    assert(heightDifference(0, 1) == -1);
    assert(heightDifference(123, 456) == -333);
    return 0;
}
