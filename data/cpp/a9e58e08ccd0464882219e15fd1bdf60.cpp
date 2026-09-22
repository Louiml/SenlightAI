Write a standalone C++ function that accepts a 2x2 symmetric matrix represented as four floating-point values (a, b, c, d) in row-major order, where the matrix is `[[a, b], [c, d]]` and b == c must hold for symmetry. The function must return a `std::vector<float>` containing the two eigenvalues in ascending order. If the matrix is not symmetric (i.e., b and c differ by more than 1e-6), throw a `std::invalid_argument`. For the symmetric case, use the closed-form quadratic formula: eigenvalues are `(a+d)/2 ± sqrt(((a-d)/2)^2 + b^2)`. Handle floating-point precision by using `float` operations and the `sqrt` function. The function must be `const`-correct, take parameters by value, and not rely on any external linear algebra library.

#include <cassert>
#include <cmath>
#include <vector>

// (Include the solution function here or in a header.)

int main() {
    // Test 1: Simple matrix [[1,2],[2,3]] from the snippet, eigenvalues ~ -0.236, 4.236
    auto e1 = symmetricEigenvalues(1.0f, 2.0f, 2.0f, 3.0f);
    assert(e1.size() == 2);
    assert(std::fabs(e1[0] - (-0.23606798f)) < 1e-5f);
    assert(std::fabs(e1[1] - 4.23606798f) < 1e-5f);

    // Test 2: Diagonal matrix [[2,0],[0,5]] => eigenvalues 2 and 5
    auto e2 = symmetricEigenvalues(2.0f, 0.0f, 0.0f, 5.0f);
    assert(std::fabs(e2[0] - 2.0f) < 1e-6f);
    assert(std::fabs(e2[1] - 5.0f) < 1e-6f);

    // Test 3: Matrix [[3,1],[1,3]] => eigenvalues 2 and 4
    auto e3 = symmetricEigenvalues(3.0f, 1.0f, 1.0f, 3.0f);
    assert(std::fabs(e3[0] - 2.0f) < 1e-6f);
    assert(std::fabs(e3[1] - 4.0f) < 1e-6f);

    // Test 4: Degenerate [[4,0],[0,4]] => both eigenvalues 4
    auto e4 = symmetricEigenvalues(4.0f, 0.0f, 0.0f, 4.0f);
    assert(std::fabs(e4[0] - 4.0f) < 1e-6f);
    assert(std::fabs(e4[1] - 4.0f) < 1e-6f);

    // Test 5: Non-symmetric matrix throws
    bool threw = false;
    try {
        symmetricEigenvalues(1.0f, 2.0f, 3.0f, 4.0f);
    } catch (const std::invalid_argument&) {
        threw = true;
    }
    assert(threw);

    // Test 6: Very close to symmetric within tolerance works
    auto e6 = symmetricEigenvalues(1.0f, 2.0f, 2.0000001f, 3.0f);
    assert(e6.size() == 2);

    // Test 7: Negative values and non-positive discriminant clamps
    auto e7 = symmetricEigenvalues(-1.0f, 1.0f, 1.0f, -1.0f);
    assert(std::fabs(e7[0] - (-2.0f)) < 1e-6f);
    assert(std::fabs(e7[1] - 0.0f) < 1e-6f);

    return 0;
}

#include <vector>
#include <cmath>
#include <stdexcept>

// Compute eigenvalues of a 2x2 symmetric matrix [[a,b],[c,d]] with b≈c.
// Returns eigenvalues in ascending order. Throws std::invalid_argument if not symmetric.
std::vector<float> symmetricEigenvalues(float a, float b, float c, float d) {
    const float tolerance = 1e-6f;
    if (std::fabs(b - c) > tolerance) {
        throw std::invalid_argument("Matrix is not symmetric enough.");
    }

    const float meanDiag = (a + d) / 2.0f;
    const float halfDiff = (a - d) / 2.0f;
    float discriminant = halfDiff * halfDiff + b * b;
    if (discriminant < 0.0f) {
        discriminant = 0.0f; // clamp to zero for numerical safety
    }
    const float sqrtDisc = std::sqrt(discriminant);

    const float lambda1 = meanDiag - sqrtDisc; // smaller eigenvalue
    const float lambda2 = meanDiag + sqrtDisc; // larger eigenvalue
    return {lambda1, lambda2};
}

// The solution computes the eigenvalues of a real symmetric 2x2 matrix using the characteristic polynomial. For matrix `[[a, b], [c, d]]`, the trace `T = a+d` and determinant `D = a*d - b*c`. The eigenvalues are `(T/2) ± sqrt((T/2)^2 - D)`. However, for symmetric matrices where b == c, this simplifies to `(a+d)/2 ± sqrt(((a-d)/2)^2 + b^2)`, which is numerically stable when the two diagonal entries are close. Edge cases: if the matrix is exactly symmetric (or within tolerance), the formula works; if the discriminant is slightly negative due to rounding, clamp to zero. Time complexity is O(1), space complexity O(1) auxiliary. Throw `std::invalid_argument` if symmetry tolerance is violated. The eigenvalues are returned in ascending order by simply checking which of the two is smaller.
