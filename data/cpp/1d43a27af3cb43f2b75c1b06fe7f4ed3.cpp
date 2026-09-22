/*
Implement a pure C++ function `computePosePOSIT` that performs one iteration of the POSIT (Pose from Orthography and Scaling with ITerations) algorithm for estimating the 3D rotation matrix and translation vector of an object from 2D image points, given a set of 3D object points, their 2D projections, and a focal length. The function should take as input: a vector of `cv::Point3f` (or plain struct with `x,y,z`) for object points (at least 4), a vector of `cv::Point2f` (or plain struct with `x,y`) for image points, a positive focal length, and an initial guess for the object-to-camera rotation (as 3x3 row-major float array) and translation (as 3-element float array) — but only the rotation is updated inside the loop; the translation should be updated at the end. The algorithm must: (1) compute the object vectors relative to the first point, (2) compute the pseudoinverse of the 3×3 Gram matrix of these vectors, (3) initialize the image vectors as differences from the first image point, (4) iterate: compute the row vectors `i` and `j` of the rotation using the pseudoinverse, normalize them, set the third row as their cross product, compute a scale factor from the average norms, and if this is the first iteration, skip the update of image vectors (since they are already initialized), otherwise update the image vectors using the current pose's third row and the current inverse scale, (5) stop after exactly one iteration (as per the POSIT algorithm's first step), and finally output the rotation (normalized rows) and translation computed as `(imagePoint0.x/scale, imagePoint0.y/scale, 1/inv_Z)` where `inv_Z = scale/focalLength`. Return `true` on success and `false` on invalid input (null pointers, too few points, non-positive focal length). Edge cases: handle degenerate Gram matrix by returning `false`. Use simple `float` arithmetic. Provide a standalone function with no external OpenCV dependencies — use your own tiny point structs, but name them as typical. Do not include a main function in the solution.
*/

#include <cmath>
#include <vector>
#include <stdexcept>

// Minimal point types to match typical usage
struct Point3f {
    float x, y, z;
};

struct Point2f {
    float x, y;
};

/**
 * Performs one iteration of the POSIT algorithm to estimate rotation and translation.
 * @param objectPoints   Array of at least 4 3D points (object coordinates).
 * @param imagePoints    Array of at least 4 2D points (image coordinates).
 * @param focalLength    Positive focal length in pixels.
 * @param rotation       Output 3x3 row-major rotation matrix (float[9]).
 * @param translation    Output translation vector (float[3]).
 * @return true on success, false on invalid input or degenerate geometry.
 */
bool computePosePOSIT(const Point3f* objectPoints, const Point2f* imagePoints,
                      int numPoints, float focalLength,
                      float* rotation, float* translation) {
    // Input validation
    if (!objectPoints || !imagePoints || !rotation || !translation)
        return false;
    if (numPoints < 4)
        return false;
    if (focalLength <= 0.0f)
        return false;

    int N = numPoints - 1;
    const int k = N; // number of rows in matrix A (N x 3)

    // Allocate temporary storage
    std::vector<float> objVecs(3 * N);
    std::vector<float> invMatr(3 * N);
    std::vector<float> imgVecs(2 * N);

    // Build object vectors relative to first point
    const Point3f& p0 = objectPoints[0];
    for (int i = 0; i < N; ++i) {
        objVecs[i]       = objectPoints[i+1].x - p0.x;
        objVecs[N + i]   = objectPoints[i+1].y - p0.y;
        objVecs[2*N + i] = objectPoints[i+1].z - p0.z;
    }

    // Compute pseudoinverse via normal equations: B = (A^T A)^{-1} A^T
    // A is N x 3, stored column-major as objVecs: a0[i]=objVecs[i], a1[i]=objVecs[N+i], a2[i]=objVecs[2N+i]
    float ata00 = 0.0f, ata01 = 0.0f, ata02 = 0.0f;
    float ata11 = 0.0f, ata12 = 0.0f, ata22 = 0.0f;
    for (int i = 0; i < N; ++i) {
        float a0 = objVecs[i];
        float a1 = objVecs[N + i];
        float a2 = objVecs[2*N + i];
        ata00 += a0 * a0;
        ata11 += a1 * a1;
        ata22 += a2 * a2;
        ata01 += a0 * a1;
        ata02 += a0 * a2;
        ata12 += a1 * a2;
    }

    // Compute inverse of 3x3 matrix ata using adjugate
    float p00 = ata11 * ata22 - ata12 * ata12;
    float p01 = -(ata01 * ata22 - ata12 * ata02);
    float p02 = ata12 * ata01 - ata11 * ata02;
    float p11 = ata00 * ata22 - ata02 * ata02;
    float p12 = -(ata00 * ata12 - ata01 * ata02);
    float p22 = ata00 * ata11 - ata01 * ata01;

    float det = ata00 * p00 + ata01 * p01 + ata02 * p02;
    if (std::fabs(det) < 1e-12f)
        return false; // degenerate: points are collinear or coplanar in a line

    float inv_det = 1.0f / det;

    // Compute pseudoinverse B (3 x N) stored as columns: b[0][i] = invMatr[i], b[1][i] = invMatr[N+i], b[2][i] = invMatr[2N+i]
    for (int i = 0; i < N; ++i) {
        float a0 = objVecs[i];
        float a1 = objVecs[N + i];
        float a2 = objVecs[2*N + i];

        invMatr[i]       = (p00 * a0 + p01 * a1 + p02 * a2) * inv_det;
        invMatr[N + i]   = (p01 * a0 + p11 * a1 + p12 * a2) * inv_det;
        invMatr[2*N + i] = (p02 * a0 + p12 * a1 + p22 * a2) * inv_det;
    }

    // Initialize image vectors: differences from first image point
    const Point2f& ip0 = imagePoints[0];
    for (int i = 0; i < N; ++i) {
        imgVecs[i]     = imagePoints[i+1].x - ip0.x;
        imgVecs[N + i] = imagePoints[i+1].y - ip0.y;
    }

    // Compute first two rows of rotation by multiplying pseudoinverse with image vectors
    // row_i = B * imgVecs_x, row_j = B * imgVecs_y
    float row_i[3] = {0,0,0};
    float row_j[3] = {0,0,0};
    for (int kk = 0; kk < 3; ++kk) {
        for (int i = 0; i < N; ++i) {
            row_i[kk] += invMatr[kk*N + i] * imgVecs[i];
            row_j[kk] += invMatr[kk*N + i] * imgVecs[N + i];
        }
    }

    // Compute norms before normalization
    float inorm_sq = row_i[0]*row_i[0] + row_i[1]*row_i[1] + row_i[2]*row_i[2];
    float jnorm_sq = row_j[0]*row_j[0] + row_j[1]*row_j[1] + row_j[2]*row_j[2];
    float inorm = std::sqrt(inorm_sq);
    float jnorm = std::sqrt(jnorm_sq);
    if (inorm < 1e-12f || jnorm < 1e-12f)
        return false;

    // Normalize rows
    float invInorm = 1.0f / inorm;
    float invJnorm = 1.0f / jnorm;
    for (int i = 0; i < 3; ++i) {
        row_i[i] *= invInorm;
        row_j[i] *= invJnorm;
    }

    // Set first two rows in output rotation
    rotation[0] = row_i[0]; rotation[1] = row_i[1]; rotation[2] = row_i[2];
    rotation[3] = row_j[0]; rotation[4] = row_j[1]; rotation[5] = row_j[2];

    // Third row = cross product of row_i and row_j
    rotation[6] = row_i[1] * row_j[2] - row_i[2] * row_j[1];
    rotation[7] = row_i[2] * row_j[0] - row_i[0] * row_j[2];
    rotation[8] = row_i[0] * row_j[1] - row_i[1] * row_j[0];

    // Scale and translation (as per original code, but only one iteration)
    // Note: we use the squared norms for scale to match original behavior:
    // scale = (inorm_sq + jnorm_sq) / 2.0, but since we normalized, the scale
    // should be based on pre-normalization norms. The original code computes
    // inorm and jnorm as sum of squares, then normalizes, then scale = (inorm+jnorm)/2.
    // We'll use the sum of squares (which is the same as the norms squared).
    float scale = (inorm_sq + jnorm_sq) / 2.0f;
    if (scale < 1e-12f)
        return false;

    float inv_scale = 1.0f / scale;
    float inv_Z = scale / focalLength; // inv_Z = scale * (1/focalLength)

    translation[0] = ip0.x * inv_scale;
    translation[1] = ip0.y * inv_scale;
    translation[2] = 1.0f / inv_Z;

    return true;
}

#include <cassert>
#include <cmath>
#include <cstdio>

// Declare the function (or include the header where it's defined)
bool computePosePOSIT(const Point3f* objectPoints, const Point2f* imagePoints,
                      int numPoints, float focalLength,
                      float* rotation, float* translation);

int main() {
    // Test case: a simple square in 3D, viewed with known pose
    // Object points (unit square in XY plane, z=0)
    Point3f obj[4] = { {0,0,0}, {1,0,0}, {1,1,0}, {0,1,0} };
    // Project them with a simple orthographic projection scaled by focal length,
    // assuming rotation identity and translation (0,0,1)
    // For simplicity, use the known solution: R=I, t=(0,0,1) with focal=1
    // Image points: (x, y) from (X/t_z * f + cx, Y/t_z * f + cy) but we set cx=cy=0, f=1, t_z=1
    Point2f img[4] = { {0,0}, {1,0}, {1,1}, {0,1} };
    float rot[9];
    float trans[3];

    // Call function
    bool ok = computePosePOSIT(obj, img, 4, 1.0f, rot, trans);
    assert(ok);

    // Check that rotation is close to identity (first two rows)
    // Expected R = [[1,0,0],[0,1,0],[0,0,1]]
    const float eps = 1e-4f;
    assert(std::fabs(rot[0] - 1.0f) < eps);
    assert(std::fabs(rot[1]) < eps);
    assert(std::fabs(rot[2]) < eps);
    assert(std::fabs(rot[3]) < eps);
    assert(std::fabs(rot[4] - 1.0f) < eps);
    assert(std::fabs(rot[5]) < eps);
    // Third row is cross product, should be (0,0,1)
    assert(std::fabs(rot[6]) < eps);
    assert(std::fabs(rot[7]) < eps);
    assert(std::fabs(rot[8] - 1.0f) < eps);

    // Check translation
    // For our simple case: scale should be 1 (since inorm_sq = 1 and jnorm_sq = 1)
    // So t = (img[0].x/1, img[0].y/1, 1/ (scale/focal) = focal/scale = 1)
    assert(std::fabs(trans[0] - 0.0f) < eps);
    assert(std::fabs(trans[1] - 0.0f) < eps);
    assert(std::fabs(trans[2] - 1.0f) < eps);

    // Test invalid input: too few points
    Point3f obj2[3] = { {0,0,0}, {1,0,0}, {0,1,0} };
    Point2f img2[3] = { {0,0}, {1,0}, {0,1} };
    float rot2[9];
    float trans2[3];
    assert(!computePosePOSIT(obj2, img2, 3, 1.0f, rot2, trans2));

    // Test non-positive focal length
    assert(!computePosePOSIT(obj, img, 4, 0.0f, rot, trans));
    assert(!computePosePOSIT(obj, img, 4, -2.0f, rot, trans));

    // Test null pointers
    assert(!computePosePOSIT(nullptr, img, 4, 1.0f, rot, trans));
    assert(!computePosePOSIT(obj, nullptr, 4, 1.0f, rot, trans));
    assert(!computePosePOSIT(obj, img, 4, 1.0f, nullptr, trans));
    assert(!computePosePOSIT(obj, img, 4, 1.0f, rot, nullptr));

    // Test degenerate collinear points (det=0)
    Point3f obj3[4] = { {0,0,0}, {1,0,0}, {2,0,0}, {3,0,0} }; // all on x-axis
    Point2f img3[4] = { {0,0}, {1,0}, {2,0}, {3,0} };
    assert(!computePosePOSIT(obj3, img3, 4, 1.0f, rot, trans));

    printf("All tests passed.\n");
    return 0;
}

// The solution is a direct translation of the core computation from the given snippet but simplified to exactly one iteration, which is the fundamental POSIT step. The approach:  
// - Validate inputs: at least 4 points, focal length > 0, non-null output pointers.  
// - Let `N = numPoints - 1`. Compute object vectors as `objVecs[i] = point[i+1] - point[0]` for each coordinate, stored in a flat array of size `3*N`.  
// - Compute the pseudoinverse of the 3×N matrix using the normal equation: `B = (A^T A)^{-1} A^T`. Compute the 3×3 Gram matrix `ata` by summing outer products, then compute its inverse using the closed-form 3×3 adjugate/determinant. If determinant is near zero, return false. Then multiply the inverse Gram matrix by `A^T` to get the pseudoinverse, stored as `3×N` flat array.  
// - Initialize image vectors as `imgVectors[i] = imagePoints[i+1].x - imagePoints[0].x` and similarly for y.  
// - Compute the first two rows of rotation as `row_i = pseudoinverse * imgVectors_x` (where imgVectors_x is the first N values), and `row_j = pseudoinverse * imgVectors_y`. Normalize each row separately.  
// - Set the third row as the cross product of row_i and row_j.  
// - Compute scale = average of norms (but since we normalized, the norms are 1, so scale is actually the original norm before normalization — but in the original code they normalize first and then compute scale as (inorm+jnorm)/2 where inorm and jnorm were the pre-normalization sums of squares. So to replicate, compute `inorm = dot(row_i, row_i)` before normalization, then after normalization compute scale = (inorm + jnorm)/2. However, note that after normalization, the norms are 1, so we must capture the pre-normalization values.  
// - Compute `inv_Z = scale / focalLength`.  
// - Compute translation as: `translation[0] = imagePoints[0].x / scale`, `translation[1] = imagePoints[0].y / scale`, `translation[2] = 1.0f / inv_Z` (which is `focalLength / scale`).  
// - The function performs exactly one iteration; no termination criteria needed.  
// - Time complexity: O(N) for computing object vectors and pseudoinverse and matrix-vector products; O(1) for the 3×3 inverse. Space: O(N) for storing arrays.  
// - Edge cases: If the Gram matrix is singular (e.g., all points collinear), return false. Ensure floating-point division by zero is avoided.
