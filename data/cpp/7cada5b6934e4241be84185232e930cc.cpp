// Write a C++ function `void shortestArcQuaternion(const float q1x, const float q1y, const float q1z, const float q1w, const float q2x, const float q2y, const float q2z, const float q2w, float& outX, float& outY, float& outZ, float& outW)` that computes a spherical linear interpolation (slerp) between two quaternions, but with the special property that it always interpolates along the shortest arc on the 4D unit sphere. This is achieved by first checking the dot product of the two quaternions: if the dot product is negative, negate the second quaternion (flip its sign) before performing the interpolation. The function must handle the edge cases where the input quaternions are identical or approximately antipodal by returning the first quaternion (or a normalized copy) without division-by-zero. The function must work for any interpolation parameter `t` in the closed interval `[0,1]`, though for the task assume `t` is provided as a fixed value of `0.5` (you may hard-code this or pass it as an extra parameter; but the task requires the signature above with no `t` parameter, so inside the function use `t = 0.5f`). The result must be a valid unit quaternion (or close to it). The function must not use external math libraries; implement the `sin` and `acos` using `<cmath>`. Provide a clean implementation with proper `const` correctness on input parameters (pass by value for scalars) and output via references.

The main algorithm is straightforward spherical linear interpolation (slerp). Given two quaternions `q1 = (x1,y1,z1,w1)` and `q2 = (x2,y2,z2,w2)`, the standard slerp formula is:
`q(t) = (sin((1-t)θ) * q1 + sin(tθ) * q2) / sin(θ)`, where `θ = acos(dot(q1,q2))` and `dot = x1*x2 + y1*y2 + z1*z2 + w1*w2`.
However, to always take the shortest arc, we must ensure the dot product is non-negative. If `dot < 0`, we flip all components of `q2` (i.e., `q2' = -q2`), because `q2` and `-q2` represent the same rotation. After flipping, we recompute the dot product. Then we compute `θ = acos(dot)`. Edge cases:
- If the absolute value of the dot product is very close to 1 (i.e., the quaternions are nearly identical or nearly antipodal after flipping), then the denominator `sin(θ)` approaches zero, causing division by zero. In that case, we either return `q1` (if identical) or the flipped `q2` (if nearly antipodal). But since we flip negative dot to positive, if `dot` is near 1 after flipping, that means the quaternions are nearly identical, so returning `q1` is safe. If `dot` is near -1 before flipping, after flipping it becomes near 1, so same situation.
- Additionally, if `dot` is exactly ±1, we return `q1` (or `-q1`? But since we flip, `q2'` becomes `q1` if exactly identical). For safety, we check `fabs(dot) >= 1.0f - 1e-6f` and return `q1`.
- For the case where `t=0.5`, the formula simplifies to `(q1 + q2') / (2 * cos(θ/2))`? But we'll use the generic formula for clarity.
Time complexity is O(1) since we perform a constant number of arithmetic operations. Space complexity O(1). The function uses `std::sin`, `std::acos`, and `std::sqrt` from `<cmath>`. We must handle the case where `sin(θ)` is near zero separately. The final output should be normalized to avoid drift, but since we use exact formula it should be close; but we can add a normalization step for safety. However, the task does not require normalization, but it's good practice. We'll include a small normalization if the squared length is not near 1. We'll output via references.

#include <cmath>

// Perform shortest-arc spherical linear interpolation (slerp) between two quaternions.
// Fixed interpolation parameter t = 0.5. Inputs are passed by value; outputs via references.
// The function ensures the shortest arc by negating q2 if the dot product is negative.
void shortestArcQuaternion(float q1x, float q1y, float q1z, float q1w,
                           float q2x, float q2y, float q2z, float q2w,
                           float& outX, float& outY, float& outZ, float& outW) {
    const float t = 0.5f;  // fixed interpolation parameter

    // Compute dot product.
    float dot = q1x * q2x + q1y * q2y + q1z * q2z + q1w * q2w;

    // If dot is negative, flip q2 to ensure shortest arc.
    if (dot < 0.0f) {
        q2x = -q2x;
        q2y = -q2y;
        q2z = -q2z;
        q2w = -q2w;
        dot = -dot;
    }

    // If quaternions are nearly identical (dot close to 1), return q1.
    const float epsilon = 1e-6f;
    if (dot > 1.0f - epsilon) {
        outX = q1x;
        outY = q1y;
        outZ = q1z;
        outW = q1w;
        return;
    }

    // Compute angle and sine.
    float theta = std::acos(dot);
    float sinTheta = std::sin(theta);

    // Guard against division by zero (shouldn't happen after the dot check, but be safe).
    if (std::fabs(sinTheta) < epsilon) {
        outX = q1x;
        outY = q1y;
        outZ = q1z;
        outW = q1w;
        return;
    }

    // Slerp coefficients.
    float sinTheta1 = std::sin((1.0f - t) * theta) / sinTheta;
    float sinTheta2 = std::sin(t * theta) / sinTheta;

    // Compute interpolated quaternion.
    float x = sinTheta1 * q1x + sinTheta2 * q2x;
    float y = sinTheta1 * q1y + sinTheta2 * q2y;
    float z = sinTheta1 * q1z + sinTheta2 * q2z;
    float w = sinTheta1 * q1w + sinTheta2 * q2w;

    // Normalize result to maintain unit length (optional but good practice).
    float norm = std::sqrt(x * x + y * y + z * z + w * w);
    if (norm > epsilon) {
        norm = 1.0f / norm;
        outX = x * norm;
        outY = y * norm;
        outZ = z * norm;
        outW = w * norm;
    } else {
        outX = q1x;
        outY = q1y;
        outZ = q1z;
        outW = q1w;
    }
}

#include <cassert>
#include <cmath>

// Declaration of the function under test.
void shortestArcQuaternion(float q1x, float q1y, float q1z, float q1w,
                           float q2x, float q2y, float q2z, float q2w,
                           float& outX, float& outY, float& outZ, float& outW);

// Helper to compare floats with tolerance.
bool almostEqual(float a, float b, float tol = 1e-4f) {
    return std::fabs(a - b) < tol;
}

int main() {
    // Test 1: Identical quaternions (identity) -> output should be identity.
    {
        float x, y, z, w;
        shortestArcQuaternion(0,0,0,1, 0,0,0,1, x,y,z,w);
        assert(almostEqual(x,0.0f) && almostEqual(y,0.0f) && almostEqual(z,0.0f) && almostEqual(w,1.0f));
    }

    // Test 2: q2 is negation of q1 (antipodal). Shortest arc should return q1 because they represent same rotation.
    {
        float x, y, z, w;
        shortestArcQuaternion(0.5f,0.5f,0.5f,0.5f, -0.5f,-0.5f,-0.5f,-0.5f, x,y,z,w);
        assert(almostEqual(x,0.5f) && almostEqual(y,0.5f) && almostEqual(z,0.5f) && almostEqual(w,0.5f));
    }

    // Test 3: Interpolation between identity and 90-degree rotation about Z axis.
    // q1 = (0,0,0,1), q2 = (0,0,sin(45°), cos(45°)) = (0,0,0.70710678,0.70710678).
    // At t=0.5, result should be (0,0,sin(22.5°), cos(22.5°)) ≈ (0,0,0.382683,0.923880).
    {
        float x, y, z, w;
        float s45 = std::sqrt(0.5f); // sin(45°) = cos(45°)
        shortestArcQuaternion(0,0,0,1, 0,0,s45,s45, x,y,z,w);
        float expectZ = std::sin(22.5f * M_PI / 180.0f);
        float expectW = std::cos(22.5f * M_PI / 180.0f);
        assert(almostEqual(x,0.0f) && almostEqual(y,0.0f) && almostEqual(z,expectZ) && almostEqual(w,expectW));
    }

    // Test 4: q2 is flipped sign but same rotation, should produce same result as without flip.
    {
        float x1,y1,z1,w1, x2,y2,z2,w2;
        // q1 = (0.1,0.2,0.3,0.9) normalized approx; q2 = (0.4,0.5,0.6,0.7) normalized.
        float q1x=0.1f, q1y=0.2f, q1z=0.3f, q1w=0.9f;
        float q2x=0.4f, q2y=0.5f, q2z=0.6f, q2w=0.7f;
        // Normalize manually to unit length for consistency.
        float n1 = std::sqrt(q1x*q1x+q1y*q1y+q1z*q1z+q1w*q1w);
        q1x/=n1; q1y/=n1; q1z/=n1; q1w/=n1;
        float n2 = std::sqrt(q2x*q2x+q2y*q2y+q2z*q2z+q2w*q2w);
        q2x/=n2; q2y/=n2; q2z/=n2; q2w/=n2;
        shortestArcQuaternion(q1x,q1y,q1z,q1w, q2x,q2y,q2z,q2w, x1,y1,z1,w1);
        shortestArcQuaternion(q1x,q1y,q1z,q1w, -q2x,-q2y,-q2z,-q2w, x2,y2,z2,w2);
        assert(almostEqual(x1,x2) && almostEqual(y1,y2) && almostEqual(z1,z2) && almostEqual(w1,w2));
    }

    // Test 5: Result should be a unit quaternion (length ≈ 1).
    {
        float x, y, z, w;
        shortestArcQuaternion(0.1f,0.2f,0.3f,0.9f, 0.4f,0.5f,0.6f,0.7f, x,y,z,w);
        float len = std::sqrt(x*x+y*y+z*z+w*w);
        assert(almostEqual(len, 1.0f, 1e-4f));
    }

    // Test 6: Interpolation between two quaternions at t=0.5 should be symmetric.
    // Interpolating q1->q2 should give same result as q2->q1 (after swapping inputs).
    {
        float x1,y1,z1,w1, x2,y2,z2,w2;
        float q1x=0.f, q1y=0.f, q1z=0.70710678f, q1w=0.70710678f; // 90° about Z
        float q2x=0.f, q2y=0.70710678f, q2z=0.f, q2w=0.70710678f; // 90° about Y
        shortestArcQuaternion(q1x,q1y,q1z,q1w, q2x,q2y,q2z,q2w, x1,y1,z1,w1);
        shortestArcQuaternion(q2x,q2y,q2z,q2w, q1x,q1y,q1z,q1w, x2,y2,z2,w2);
        // Because slerp(q1,q2,0.5) = slerp(q2,q1,0.5) up to sign, but our function ensures shortest arc,
        // both should yield the same orientation. Since both dot products are positive (original dot = 0.5), no flip,
        // and the formulas are symmetric.
        assert(almostEqual(x1,x2) && almostEqual(y1,y2) && almostEqual(z1,z2) && almostEqual(w1,w2));
    }

    return 0;
}
