Write a C++ function that applies a 3x3 rotation matrix (stored in row-major order as an array of 9 floats) to three components of sensor data (gyroscope, accelerometer, and compass). The function should take the rotation matrix, three input vectors (each with x, y, z components), and produce three output vectors by multiplying the matrix with each input vector. The function must be `const`-correct, accept the rotation matrix as `const float*`, and return a struct containing the three transformed vectors. Assume the rotation matrix is orthonormal and contains only values 0, 1, and -1 (as typical for axis swaps), but the function should handle general values correctly. Edge cases include the identity matrix (no change) and negative scaling (e.g., flipping an axis). The function should not modify the inputs.

// The solution computes, for each input vector (gyro, accel, compass), the matrix-vector product: `output[i] = sum_{j=0..2} matrix[3*i+j] * input[j]`. This is done independently for each vector because the rotation applies to each sensor's 3D vector. The implementation uses a small loop over rows and columns for clarity, but given the matrix has exactly 9 values, a direct unrolled calculation is also fine. The main edge cases are: (1) identity matrix – multiplying by rows like {1,0,0} etc. returns the same vector; (2) rows with zeros and a single ±1 – the logic picks the corresponding component and scales by ±1; (3) the function must be `const`-correct: the inputs are taken by const reference or value, and the function returns a new struct. Time complexity is O(1) because the matrix size is fixed (3x3) and we process exactly three vectors, each with 3 components, so 27 multiplications and 18 additions total. Space complexity is O(1) for the output struct.

#include <array>

// Represents three 3D sensor vectors (gyro, accel, compass).
struct SensorData {
    std::array<float, 3> gyro;
    std::array<float, 3> accel;
    std::array<float, 3> compass;
};

// Apply a 3x3 row-major rotation matrix to each sensor vector.
// matrix is a pointer to 9 floats in row-major order.
SensorData applyAxisRotation(const float* matrix,
                             const std::array<float, 3>& gyro,
                             const std::array<float, 3>& accel,
                             const std::array<float, 3>& compass) {
    SensorData result;

    // Helper lambda to multiply a 3x3 matrix by a 3D vector.
    auto transform = [&](const std::array<float, 3>& v) -> std::array<float, 3> {
        std::array<float, 3> out = {};
        for (int row = 0; row < 3; ++row) {
            float sum = 0.0f;
            for (int col = 0; col < 3; ++col) {
                sum += matrix[3 * row + col] * v[col];
            }
            out[row] = sum;
        }
        return out;
    };

    result.gyro = transform(gyro);
    result.accel = transform(accel);
    result.compass = transform(compass);

    return result;
}

#include <cassert>
#include <cmath>

int main() {
    // Identity matrix
    const float identity[9] = {1,0,0, 0,1,0, 0,0,1};
    std::array<float,3> g = {1.0f, -2.0f, 3.0f};
    std::array<float,3> a = {0.5f, 0.0f, -1.5f};
    std::array<float,3> c = {100.0f, 200.0f, 300.0f};
    auto res = applyAxisRotation(identity, g, a, c);
    assert(res.gyro == g);
    assert(res.accel == a);
    assert(res.compass == c);

    // Flip X axis (negate X component)
    const float flipX[9] = {-1,0,0, 0,1,0, 0,0,1};
    res = applyAxisRotation(flipX, g, a, c);
    assert(res.gyro[0] == -1.0f && res.gyro[1] == -2.0f && res.gyro[2] == 3.0f);
    assert(res.accel[0] == -0.5f && res.accel[1] == 0.0f && res.accel[2] == -1.5f);
    assert(res.compass[0] == -100.0f && res.compass[1] == 200.0f && res.compass[2] == 300.0f);

    // Swap X and Y axes (rotation matrix: [0,1,0; 1,0,0; 0,0,1])
    const float swapXY[9] = {0,1,0, 1,0,0, 0,0,1};
    res = applyAxisRotation(swapXY, g, a, c);
    assert(res.gyro[0] == -2.0f && res.gyro[1] == 1.0f && res.gyro[2] == 3.0f);
    assert(res.accel[0] == 0.0f && res.accel[1] == 0.5f && res.accel[2] == -1.5f);
    assert(res.compass[0] == 200.0f && res.compass[1] == 100.0f && res.compass[2] == 300.0f);

    // Negative swap with sign change: matrix [0,-1,0; 1,0,0; 0,0,-1]
    const float complex[9] = {0,-1,0, 1,0,0, 0,0,-1};
    res = applyAxisRotation(complex, g, a, c);
    assert(res.gyro[0] == 2.0f && res.gyro[1] == 1.0f && res.gyro[2] == -3.0f);
    assert(res.accel[0] == 0.0f && res.accel[1] == 0.5f && res.accel[2] == 1.5f);
    assert(res.compass[0] == -200.0f && res.compass[1] == 100.0f && res.compass[2] == -300.0f);

    // All zeros matrix (degenerate but testable)
    const float zeros[9] = {0,0,0, 0,0,0, 0,0,0};
    res = applyAxisRotation(zeros, g, a, c);
    assert(res.gyro[0] == 0.0f && res.gyro[1] == 0.0f && res.gyro[2] == 0.0f);
    assert(res.accel[0] == 0.0f && res.accel[1] == 0.0f && res.accel[2] == 0.0f);
    assert(res.compass[0] == 0.0f && res.compass[1] == 0.0f && res.compass[2] == 0.0f);

    // General matrix with fractional values (not orthonormal, but valid)
    const float general[9] = {0.5f, 0.0f, 0.0f, 0.0f, 2.0f, 0.0f, 0.0f, 0.0f, -1.0f};
    res = applyAxisRotation(general, g, a, c);
    assert(std::fabs(res.gyro[0] - 0.5f) < 1e-6f);
    assert(std::fabs(res.gyro[1] + 4.0f) < 1e-6f);
    assert(std::fabs(res.gyro[2] + 3.0f) < 1e-6f);
    assert(std::fabs(res.accel[0] - 0.25f) < 1e-6f);
    assert(std::fabs(res.accel[1] - 0.0f) < 1e-6f);
    assert(std::fabs(res.accel[2] - 1.5f) < 1e-6f);
    assert(std::fabs(res.compass[0] - 50.0f) < 1e-6f);
    assert(std::fabs(res.compass[1] - 400.0f) < 1e-6f);
    assert(std::fabs(res.compass[2] + 300.0f) < 1e-6f);

    return 0;
}
