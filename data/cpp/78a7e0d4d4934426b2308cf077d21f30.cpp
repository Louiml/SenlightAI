// Write a C++ function named `rotationZ` that takes a rotation angle in radians (as a `double`) and a pointer to a 9-element `double` array, and fills the array with the 3x3 rotation matrix around the Z-axis in column-major order (i.e., the array index corresponds to column-major layout: element `A[col*3 + row]`). The matrix must have the form: `[cos(t), sin(t), 0; -sin(t), cos(t), 0; 0, 0, 1]` where `t` is the input angle. The function must modify the array in place and return `void`. Handle any input angle, including negative, zero, or very large values, and ensure that for `t=0` the identity matrix is produced.

The problem requires constructing a standard 3D rotation matrix about the Z-axis. The approach is straightforward: compute the cosine and sine of the input angle `t` using standard library functions `std::cos` and `std::sin` from `<cmath>`. Then assign the matrix elements in column-major order: for column 0 (indices 0,1,2) we set `A[0] = cos(t)`, `A[1] = sin(t)`, `A[2] = 0`; for column 1 (indices 3,4,5) we set `A[3] = -sin(t)`, `A[4] = cos(t)`, `A[5] = 0`; for column 2 (indices 6,7,8) we set `A[6] = 0`, `A[7] = 0`, `A[8] = 1`. No special handling of edge cases is needed because `sin` and `cos` are defined for all real numbers and produce periodic results. For `t=0`, `cos(0)=1`, `sin(0)=0`, yielding the identity matrix. The time complexity is O(1) because only two trigonometric calls and constant assignments are performed. Space complexity is O(1) auxiliary space, since we only modify the provided array. The function must be `const`-safe in the sense that it does not modify the input angle by value (passed by value), and it uses a non-const pointer for output as required.

#include <cmath>

// Fills a 3x3 rotation matrix about the Z-axis (column-major order) given an angle in radians.
// The output array A has 9 elements, with A[col * 3 + row] layout.
void rotationZ(double t, double A[9]) {
    const double c = std::cos(t);
    const double s = std::sin(t);

    A[0] = c;   A[3] = -s;  A[6] = 0.0;
    A[1] = s;   A[4] = c;   A[7] = 0.0;
    A[2] = 0.0; A[5] = 0.0; A[8] = 1.0;
}

#include <cassert>
#include <cmath>

// Function declaration (assuming the solution is in the same translation unit)
void rotationZ(double t, double A[9]);

int main() {
    // Test 1: zero angle -> identity matrix
    double A1[9];
    rotationZ(0.0, A1);
    assert(A1[0] == 1.0 && A1[1] == 0.0 && A1[2] == 0.0);
    assert(A1[3] == 0.0 && A1[4] == 1.0 && A1[5] == 0.0);
    assert(A1[6] == 0.0 && A1[7] == 0.0 && A1[8] == 1.0);

    // Test 2: 90 degrees (pi/2) -> known values
    double A2[9];
    rotationZ(M_PI / 2.0, A2);
    const double eps = 1e-12;
    assert(std::fabs(A2[0] - 0.0) < eps && std::fabs(A2[1] - 1.0) < eps && std::fabs(A2[2] - 0.0) < eps);
    assert(std::fabs(A2[3] + 1.0) < eps && std::fabs(A2[4] - 0.0) < eps && std::fabs(A2[5] - 0.0) < eps);
    assert(std::fabs(A2[6] - 0.0) < eps && std::fabs(A2[7] - 0.0) < eps && std::fabs(A2[8] - 1.0) < eps);

    // Test 3: negative angle -pi/2 -> mirror of +pi/2
    double A3[9];
    rotationZ(-M_PI / 2.0, A3);
    assert(std::fabs(A3[0] - 0.0) < eps && std::fabs(A3[1] + 1.0) < eps && std::fabs(A3[2] - 0.0) < eps);
    assert(std::fabs(A3[3] - 1.0) < eps && std::fabs(A3[4] - 0.0) < eps && std::fabs(A3[5] - 0.0) < eps);
    assert(std::fabs(A3[6] - 0.0) < eps && std::fabs(A3[7] - 0.0) < eps && std::fabs(A3[8] - 1.0) < eps);

    // Test 4: full rotation 2*pi -> close to identity
    double A4[9];
    rotationZ(2.0 * M_PI, A4);
    assert(std::fabs(A4[0] - 1.0) < eps && std::fabs(A4[1] - 0.0) < eps && std::fabs(A4[2] - 0.0) < eps);
    assert(std::fabs(A4[3] - 0.0) < eps && std::fabs(A4[4] - 1.0) < eps && std::fabs(A4[5] - 0.0) < eps);
    assert(std::fabs(A4[6] - 0.0) < eps && std::fabs(A4[7] - 0.0) < eps && std::fabs(A4[8] - 1.0) < eps);

    // Test 5: large angle (1000 radians) still produces orthonormal rows/columns (basic property check)
    double A5[9];
    rotationZ(1000.0, A5);
    double col0_norm_sq = A5[0]*A5[0] + A5[1]*A5[1] + A5[2]*A5[2];
    double col1_norm_sq = A5[3]*A5[3] + A5[4]*A5[4] + A5[5]*A5[5];
    double dot_col0_col1 = A5[0]*A5[3] + A5[1]*A5[4] + A5[2]*A5[5];
    assert(std::fabs(col0_norm_sq - 1.0) < eps);
    assert(std::fabs(col1_norm_sq - 1.0) < eps);
    assert(std::fabs(dot_col0_col1 - 0.0) < eps);
    assert(std::fabs(A5[6] - 0.0) < eps && std::fabs(A5[7] - 0.0) < eps && std::fabs(A5[8] - 1.0) < eps);

    return 0;
}
