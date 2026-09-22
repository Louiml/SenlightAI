// Given an integer matrix stored as an `Eigen::Matrix3f`, write a C++ function named `computeTraceAndMaxElement` that returns a `std::pair<float, float>` where the first element is the trace (sum of the diagonal elements) and the second element is the maximum value among all 9 matrix entries. The function must take the matrix by const reference (to avoid copying) and must work for any 3x3 matrix, including matrices with negative values, zeros, and duplicate extreme values. The function should not modify the input matrix, must use appropriate `const` correctness, and return the result as a pair.

The solution is straightforward: loop through all matrix elements to find the maximum, while simultaneously summing only the diagonal entries (where row index equals column index). For a 3x3 matrix, `mat(i, i)` for `i = 0, 1, 2` gives the diagonal. The trace is initialized to `0.0f` and added to for each diagonal element. The maximum is initialized to the first element (or `mat(0,0)`) and updated whenever a larger value is found. Edge cases: negative values (maximum can be negative if all entries are negative, so initialize with `mat(0,0)` rather than `0`), duplicate values (no special handling needed). Since the matrix size is fixed at 3x3, the time complexity is O(1) (constant number of iterations, specifically 9 element accesses), and space complexity is O(1) (only scalar temporaries and the returned pair). The function is `const`‑correct because the input is `const Matrix3f&` and the function does not modify anything.

#include <Eigen/Core>
#include <utility>
#include <algorithm>

// Compute the trace (sum of diagonal elements) and the maximum element of a 3x3 matrix.
// Returns a pair: first = trace, second = maximum element.
std::pair<float, float> computeTraceAndMaxElement(const Eigen::Matrix3f& mat) {
    float trace = 0.0f;
    // Initialize max with the first element to handle all‑negative matrices correctly.
    float maxElem = mat(0, 0);

    for (int i = 0; i < 3; ++i) {
        trace += mat(i, i);               // diagonal element
        for (int j = 0; j < 3; ++j) {
            maxElem = std::max(maxElem, mat(i, j));
        }
    }

    return {trace, maxElem};
}

#include <Eigen/Core>
#include <cassert>
#include <utility>

// Declaration of the solution function (assumes it is provided separately).
std::pair<float, float> computeTraceAndMaxElement(const Eigen::Matrix3f& mat);

int main() {
    // Basic case with all positive values.
    Eigen::Matrix3f m1;
    m1 << 1.0f, 2.0f, 3.0f,
          4.0f, 5.0f, 6.0f,
          7.0f, 8.0f, 9.0f;
    auto r1 = computeTraceAndMaxElement(m1);
    assert(r1.first == 15.0f);  // 1 + 5 + 9
    assert(r1.second == 9.0f);

    // Matrix with negative values (max is negative).
    Eigen::Matrix3f m2;
    m2 << -1.0f, -2.0f, -3.0f,
          -4.0f, -5.0f, -6.0f,
          -7.0f, -8.0f, -9.0f;
    auto r2 = computeTraceAndMaxElement(m2);
    assert(r2.first == -15.0f); // -1 + -5 + -9
    assert(r2.second == -1.0f);

    // Matrix with zeros and duplicates.
    Eigen::Matrix3f m3;
    m3 << 0.0f, 0.0f, 2.0f,
          2.0f, 0.0f, 0.0f,
          0.0f, 0.0f, 0.0f;
    auto r3 = computeTraceAndMaxElement(m3);
    assert(r3.first == 0.0f);   // all diagonals are zero
    assert(r3.second == 2.0f);

    // Identity matrix.
    Eigen::Matrix3f m4 = Eigen::Matrix3f::Identity();
    auto r4 = computeTraceAndMaxElement(m4);
    assert(r4.first == 3.0f);
    assert(r4.second == 1.0f);

    // Matrix with floating point values.
    Eigen::Matrix3f m5;
    m5 << 1.5f, -2.0f, 0.25f,
          3.0f, 4.5f, -0.5f,
          2.2f, 1.1f, -3.3f;
    auto r5 = computeTraceAndMaxElement(m5);
    // trace = 1.5 + 4.5 + (-3.3) = 2.7
    assert(std::abs(r5.first - 2.7f) < 1e-5);
    // max = 4.5
    assert(r5.second == 4.5f);

    // All same value.
    Eigen::Matrix3f m6 = Eigen::Matrix3f::Constant(7.0f);
    auto r6 = computeTraceAndMaxElement(m6);
    assert(r6.first == 21.0f); // 7 + 7 + 7
    assert(r6.second == 7.0f);

    // Mixed signs with max on diagonal.
    Eigen::Matrix3f m7;
    m7 << 10.0f, -1.0f, -2.0f,
          -3.0f,  5.0f, -4.0f,
          -5.0f, -6.0f,  1.0f;
    auto r7 = computeTraceAndMaxElement(m7);
    assert(r7.first == 16.0f); // 10 + 5 + 1
    assert(r7.second == 10.0f);

    // Zero matrix.
    Eigen::Matrix3f m8 = Eigen::Matrix3f::Zero();
    auto r8 = computeTraceAndMaxElement(m8);
    assert(r8.first == 0.0f);
    assert(r8.second == 0.0f);

    return 0;
}
