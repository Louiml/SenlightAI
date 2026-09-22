Write a C++ function named `zeroOutMiddleSegment` that accepts a fixed-size vector of exactly 4 integers (representing a row vector) by const reference, and returns a new fixed-size vector of 4 integers where the two middle elements (at indices 1 and 2) are set to zero, while the first and last elements remain unchanged. The function must not modify the input vector. For example, if the input is `{5, -3, 7, 2}`, the returned vector must be `{5, 0, 0, 2}`. The solution must handle all possible integer values, including negative numbers and zeros, and must work correctly regardless of the initial values in the vector. Use the Eigen library’s `RowVector4i` type for the vector representation.

The task is straightforward: given a fixed-size vector of length 4, we need to produce a new vector with the middle positions zeroed. The main algorithm involves copying the input vector and then setting elements at indices 1 and 2 to zero. Since the input is passed by const reference, we must create a copy to avoid modifying the original. This is a constant-time operation because the size is fixed at 4. Edge cases include vectors with already-zero middle elements, negative numbers, or extreme integer values; no special handling is needed because setting to zero is explicit. The time complexity is O(1) because only a constant number of operations are performed (copy 4 elements, assign 2 zeros). The space complexity is O(1) as we only allocate a new fixed-size vector (which is the return value) and no dynamic memory.

#include <Eigen/Core>

// Return a copy of the input vector with elements at indices 1 and 2 set to zero.
// The input vector is not modified.
Eigen::RowVector4i zeroOutMiddleSegment(const Eigen::RowVector4i& input) {
    Eigen::RowVector4i result = input; // copy
    result[1] = 0;
    result[2] = 0;
    return result;
}

#include <Eigen/Core>
#include <cassert>

int main() {
    Eigen::RowVector4i v1(1, 2, 3, 4);
    Eigen::RowVector4i out1 = zeroOutMiddleSegment(v1);
    assert(out1 == Eigen::RowVector4i(1, 0, 0, 4));

    Eigen::RowVector4i v2(-5, -100, 42, 7);
    Eigen::RowVector4i out2 = zeroOutMiddleSegment(v2);
    assert(out2 == Eigen::RowVector4i(-5, 0, 0, 7));

    // Input remains unchanged
    assert(v2 == Eigen::RowVector4i(-5, -100, 42, 7));

    // Already zero in the middle
    Eigen::RowVector4i v3(0, 0, 0, 0);
    Eigen::RowVector4i out3 = zeroOutMiddleSegment(v3);
    assert(out3 == Eigen::RowVector4i(0, 0, 0, 0));

    // Extreme values
    Eigen::RowVector4i v4(2147483647, -2147483647, 1, -1);
    Eigen::RowVector4i out4 = zeroOutMiddleSegment(v4);
    assert(out4 == Eigen::RowVector4i(2147483647, 0, 0, -1));
}
