// Write a C++ function that takes two positive integer arguments representing the breadth and height of a triangle, and returns its area as an integer. The area must be computed using integer arithmetic with truncation toward zero (i.e., using `(breadth * height) / 2`). If either argument is non-positive, the function should return 0 to indicate an invalid input. The function must be named `computeTriangleArea` and should accept parameters by value with appropriate `const` qualification. Do not include a `main` function; only provide the function definition.

#include <cassert>

int main() {
    assert(computeTriangleArea(3, 4) == 6);
    assert(computeTriangleArea(5, 5) == 12); // 25/2 truncates to 12
    assert(computeTriangleArea(1, 1) == 0); // 1/2 = 0
    assert(computeTriangleArea(0, 10) == 0);
    assert(computeTriangleArea(-2, 5) == 0);
    assert(computeTriangleArea(100000, 100000) == 5000000000); // potential overflow handled
    return 0;
}

// Compute the integer area of a triangle given positive breadth and height.
// Returns 0 for non-positive dimensions.
int computeTriangleArea(const int breadth, const int height) {
    if (breadth <= 0 || height <= 0) {
        return 0;
    }
    // Use long long to avoid overflow during multiplication.
    long long product = static_cast<long long>(breadth) * static_cast<long long>(height);
    return static_cast<int>(product / 2);
}

// The solution simply multiplies the breadth and height and divides by 2. Since both are integers, the division truncates toward zero, matching the behavior of the original snippet. Edge cases include zero or negative inputs, which should return 0 as a sentinel for invalid dimensions. Also consider potential overflow when multiplying two large `int` values; to avoid undefined behavior, we can cast to a wider type (e.g., `long long`) before multiplication, then truncate the result back to `int`. The time complexity is O(1) and space complexity is O(1), making it constant.
