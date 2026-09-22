// Write a C++ function that implements a subset of a 3D transformation library: it must accept a 4x4 matrix stored in column-major order as a flat `float` array of size 16 (index mapping: element at row r and column c is at index c*4 + r), and perform the following operations in sequence: (1) scale by given x, y, z factors, (2) rotate around the X axis by a given angle in degrees, then (3) translate by given offsets. The function must modify the matrix in place, not return a new one. Assume all inputs are valid (non-zero scale factors, finite angles). Your solution must include a helper that loads the identity matrix, and use the provided column-major conventions exactly as shown below. The function signature must be: `void ApplyTransformSequence(float* mtx, float xScale, float yScale, float zScale, float xRotDeg, float yTrans, float zTrans);`
#include <cassert>
#include <cmath>

// Forward declaration of the solution function (defined above, but declared here for clarity)
void ApplyTransformSequence(float*, float, float, float, float, float, float);

int main() {
    // Helper to compare matrices with tolerance.
    auto near = [](float a, float b) { return std::fabs(a - b) < 1e-5f; };

    // Test 1: Identity (scale 1, no rotation, no translation)
    {
        float m[16];
        ApplyTransformSequence(m, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f);
        assert(near(m[0],1) && near(m[5],1) && near(m[10],1) && near(m[15],1));
        for (int i = 0; i < 16; ++i) {
            if (i == 0 || i == 5 || i == 10 || i == 15) continue;
            assert(near(m[i], 0.0f));
        }
    }

    // Test 2: Pure scaling (no rotation, no translation)
    {
        float m[16];
        ApplyTransformSequence(m, 2.0f, 3.0f, 4.0f, 0.0f, 0.0f, 0.0f);
        assert(near(m[0],2.0f)); // X basis x
        assert(near(m[5],3.0f)); // Y basis y
        assert(near(m[10],4.0f)); // Z basis z
        assert(near(m[15],1.0f)); // homogeneous
        // All off-diagonal elements should be 0
        for (int i = 0; i < 16; ++i) {
            if (i == 0 || i == 5 || i == 10 || i == 15) continue;
            assert(near(m[i], 0.0f));
        }
    }

    // Test 3: Rotation 90 degrees around X, then translation in Y
    // Expected: Y basis becomes (0,0,-1), Z basis becomes (0,1,0)
    // Translation (0, 10, 0) affects column 3
    {
        float m[16];
        ApplyTransformSequence(m, 1.0f, 1.0f, 1.0f, 90.0f, 10.0f, 0.0f);
        // Column 0 (X) unchanged
        assert(near(m[0],1.0f) && near(m[1],0.0f) && near(m[2],0.0f) && near(m[3],0.0f));
        // Column 1 (Y) becomes (0, cos90, sin90) = (0,0,1)
        assert(near(m[4],0.0f) && near(m[5],0.0f) && near(m[6],1.0f) && near(m[7],0.0f));
        // Column 2 (Z) becomes (0, -sin90, cos90) = (0,-1,0)
        assert(near(m[8],0.0f) && near(m[9],-1.0f) && near(m[10],0.0f) && near(m[11],0.0f));
        // Column 3 (translation) = original (0,0,0,1) + yTrans * Y basis (0,0,1,0)
        // So translation = (0, 10*0, 10*1, 10*0) = (0,0,10,1) in columns
        assert(near(m[12],0.0f) && near(m[13],0.0f) && near(m[14],10.0f) && near(m[15],1.0f));
    }

    // Test 4: Combined scale (2,2,2), rotate 180 around X, translate y=5, z=-3
    // Scale first: basis vectors are (2,0,0), (0,2,0), (0,0,2)
    // Rotate 180: Y becomes (0,-2,0), Z becomes (0,0,-2)
    // Translate: adding yTrans and zTrans in world coordinates: (0,5,-3)
    // Translation column = old translation (0,0,0,1) + 5*Y_basis + (-3)*Z_basis
    // = (0,0,0) + 5*(0,-2,0) + (-3)*(0,0,-2) = (0,-10,6)
    {
        float m[16];
        ApplyTransformSequence(m, 2.0f, 2.0f, 2.0f, 180.0f, 5.0f, -3.0f);
        // Column 1 should be (0, -2, 0) because cos180=-1, sin180=0
        assert(near(m[4],0.0f) && near(m[5],-2.0f) && near(m[6],0.0f) && near(m[7],0.0f));
        // Column 2 should be (0, 0, -2)
        assert(near(m[8],0.0f) && near(m[9],0.0f) && near(m[10],-2.0f) && near(m[11],0.0f));
        // Translation column: previous translation (0,0,0,1) plus contributions
        // from yTrans and zTrans: yTrans * Y_basis = 5*(0,-2,0) = (0,-10,0)
        // zTrans * Z_basis = -3*(0,0,-2) = (0,0,6)
        // Total = (0,-10,6,1)
        assert(near(m[12],0.0f) && near(m[13],-10.0f) && near(m[14],6.0f) && near(m[15],1.0f));
    }

    // Test 5: Non-uniform scale and rotation 45 degrees
    // Verify Y and Z columns with mathematical expectation
    {
        float m[16];
        ApplyTransformSequence(m, 1.0f, 2.0f, 3.0f, 45.0f, 0.0f, 0.0f);
        const float s45 = std::sqrt(2.0f) / 2.0f; // ~0.7071
        // Y basis after scaling is (0,2,0) then rotate 45: (0, 2*cos45, 2*sin45) = (0, 2s, 2s)
        assert(near(m[4],0.0f) && near(m[5],2.0f*s45) && near(m[6],2.0f*s45) && near(m[7],0.0f));
        // Z basis after scaling is (0,0,3) then rotate 45: (0, -3*sin45, 3*cos45) = (0, -3s, 3s)
        assert(near(m[8],0.0f) && near(m[9],-3.0f*s45) && near(m[10],3.0f*s45) && near(m[11],0.0f));
        // Translation stays zero
        assert(near(m[12],0.0f) && near(m[13],0.0f) && near(m[14],0.0f) && near(m[15],1.0f));
    }

    return 0;
}
#include <cmath>

// Multiply all elements of a 4x4 column-major matrix by the given scale factors.
// The matrix is indexed as: element (row, col) is at index col*4 + row.
// Column 0 is X, column 1 is Y, column 2 is Z, column 3 is translation.
static void ApplyScale(float* m, float sx, float sy, float sz) {
    // Scale the X basis vector (column 0)
    m[0] *= sx;
    m[1] *= sx;
    m[2] *= sx;
    m[3] *= sx;

    // Scale the Y basis vector (column 1)
    m[4] *= sy;
    m[5] *= sy;
    m[6] *= sy;
    m[7] *= sy;

    // Scale the Z basis vector (column 2)
    m[8] *= sz;
    m[9] *= sz;
    m[10] *= sz;
    m[11] *= sz;

    // Column 3 (translation) is unchanged
}

// Rotate the matrix around the X axis by the given angle in degrees.
// The rotation is applied as M = M * RotationX, affecting columns 1 and 2.
static void ApplyRotateX(float* m, float angleDeg) {
    const float rad = angleDeg * static_cast<float>(M_PI) / 180.0f;
    const float cosA = std::cos(rad);
    const float sinA = std::sin(rad);

    // Save original Y basis vector (column 1)
    const float oldY0 = m[4];
    const float oldY1 = m[5];
    const float oldY2 = m[6];
    const float oldY3 = m[7];

    // New Y = oldY * cos + oldZ * sin
    m[4] = oldY0 * cosA + m[8]  * sinA;
    m[5] = oldY1 * cosA + m[9]  * sinA;
    m[6] = oldY2 * cosA + m[10] * sinA;
    m[7] = oldY3 * cosA + m[11] * sinA;

    // New Z = -oldY * sin + oldZ * cos
    m[8]  = -oldY0 * sinA + m[8]  * cosA;
    m[9]  = -oldY1 * sinA + m[9]  * cosA;
    m[10] = -oldY2 * sinA + m[10] * cosA;
    m[11] = -oldY3 * sinA + m[11] * cosA;
}

// Translate the matrix by the given offsets in world space.
// The translation is added to the last column (indices 12-15).
static void ApplyTranslate(float* m, float tx, float ty, float tz) {
    m[12] += m[0] * tx + m[4] * ty + m[8]  * tz;
    m[13] += m[1] * tx + m[5] * ty + m[9]  * tz;
    m[14] += m[2] * tx + m[6] * ty + m[10] * tz;
    m[15] += m[3] * tx + m[7] * ty + m[11] * tz;
}

// Apply a sequence of transformations to a 4x4 column-major matrix:
// start from identity, then scale, rotate around X, and translate.
void ApplyTransformSequence(float* mtx, float xScale, float yScale, float zScale,
                            float xRotDeg, float yTrans, float zTrans) {
    // Load identity into the matrix.
    mtx[0] = mtx[5] = mtx[10] = mtx[15] = 1.0f;
    mtx[1] = mtx[2] = mtx[3] = mtx[4] = 0.0f;
    mtx[6] = mtx[7] = mtx[8] = mtx[9] = 0.0f;
    mtx[11] = mtx[12] = mtx[13] = mtx[14] = 0.0f;

    ApplyScale(mtx, xScale, yScale, zScale);
    ApplyRotateX(mtx, xRotDeg);
    ApplyTranslate(mtx, 0.0f, yTrans, zTrans); // no x translation input per spec
}
// The main algorithm processes the matrix in three stages, each modifying the 16-element array in place. First, load the identity matrix into the array (set diagonal elements 0,5,10,15 to 1.0 and all others to 0). Second, apply scaling: since scaling multiplies each column of the matrix by a factor (and the identity matrix has columns corresponding to basis vectors), we multiply the appropriate elements for each column: for column 0 (indices 0-3) multiply by xScale, column 1 (indices 4-7) by yScale, column 2 (indices 8-11) by zScale, and leave column 3 (indices 12-15) unchanged because the translation column is not scaled. Third, apply rotation around X: for a column-major matrix, the rotation matrix multiplies the current matrix on the right (M = M * R). The rotation around X affects columns 1 and 2 (the Y and Z basis vectors). The updated columns are computed as: new col1 = col1 * cos(angle) + col2 * sin(angle), new col2 = -col1 * sin(angle) + col2 * cos(angle). This requires storing the old values of col1 (indices 4-7) before overwriting. Fourth, apply translation: adding the translation vector to the last column (indices 12-15) using the current matrix's basis vectors: for each row r, mtx[12+r] += xTrans * mtx[r] + yTrans * mtx[4+r] + zTrans * mtx[8+r]. Since we start from identity, but the translation must be applied after rotation, the final translation is in world space. Edge cases: zero scale factors would make the matrix singular, but we assume valid input. Angles are converted from degrees to radians (multiply by π/180). Use `cosf` and `sinf` and include `<cmath>`. Time complexity is O(1) because the matrix size is fixed (16 elements). Space complexity is O(1) auxiliary, using only a few local variables for temporary storage.
