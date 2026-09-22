/*
Write a standalone C++ function `computeBestFitTransform` that takes two 2D point clouds represented as `std::vector<Eigen::Vector2f>` (source and target) and returns a 2x3 affine transformation matrix `Eigen::Matrix<float, 2, 3>` that best aligns the source points to the target points using the closed-form least-squares rigid transformation (rotation + translation) with no scaling. The function must handle the case where the point clouds have different sizes by considering only the first `min(source.size(), target.size())` points from each cloud. The function should compute the optimal rotation and translation that minimizes the sum of squared Euclidean distances between corresponding points after transformation. Use the SVD-based method (e.g., via `Eigen::JacobiSVD`) to compute the rotation matrix, and account for the special case where the determinant of the rotation matrix is -1 (reflection) by flipping the sign of the last column of the rotation matrix. Ensure the function is `const`-correct, takes inputs by `const` reference, and returns the result as a 2x3 matrix where the first two columns are the rotation matrix and the third column is the translation vector. The function must not modify the input clouds.
*/
#include <Eigen/Dense>
#include <vector>
#include <algorithm>

/**
 * Compute the best-fit rigid transformation (rotation + translation) that aligns
 * source points to target points in 2D using least-squares (Kabsch algorithm).
 * 
 * The function matches points pairwise (source[i] to target[i]) for the first
 * min(source.size(), target.size()) points. If either cloud is empty, it returns
 * an identity rotation and zero translation.
 * 
 * @param source First point cloud (column vectors).
 * @param target Second point cloud (column vectors).
 * @return 2x3 affine matrix: [R | t] where R is 2x2 rotation and t is 2x1 translation.
 */
Eigen::Matrix<float, 2, 3> computeBestFitTransform(
    const std::vector<Eigen::Vector2f>& source,
    const std::vector<Eigen::Vector2f>& target) 
{
    // Determine the number of correspondences to use.
    const size_t n = std::min(source.size(), target.size());

    // If no correspondences, return identity transformation.
    if (n == 0) {
        Eigen::Matrix<float, 2, 3> result;
        result.setIdentity();
        result(0, 2) = 0.0f;
        result(1, 2) = 0.0f;
        return result;
    }

    // Compute centroids.
    Eigen::Vector2f centroid_source = Eigen::Vector2f::Zero();
    Eigen::Vector2f centroid_target = Eigen::Vector2f::Zero();
    for (size_t i = 0; i < n; ++i) {
        centroid_source += source[i];
        centroid_target += target[i];
    }
    centroid_source /= static_cast<float>(n);
    centroid_target /= static_cast<float>(n);

    // Compute cross-covariance matrix H.
    Eigen::Matrix2f H = Eigen::Matrix2f::Zero();
    for (size_t i = 0; i < n; ++i) {
        Eigen::Vector2f source_centered = source[i] - centroid_source;
        Eigen::Vector2f target_centered = target[i] - centroid_target;
        H += source_centered * target_centered.transpose();
    }

    // Perform SVD on H.
    Eigen::JacobiSVD<Eigen::Matrix2f> svd(H, Eigen::ComputeFullU | Eigen::ComputeFullV);
    Eigen::Matrix2f U = svd.matrixU();
    Eigen::Matrix2f V = svd.matrixV();

    // Compute rotation matrix R = V * U^T.
    Eigen::Matrix2f R = V * U.transpose();

    // Ensure R is a proper rotation (det = +1). If det = -1, correct by flipping
    // the sign of the last column of V (and recompute R).
    if (R.determinant() < 0.0f) {
        V.col(1) = -V.col(1);
        R = V * U.transpose();
    }

    // Compute translation vector.
    Eigen::Vector2f t = centroid_target - R * centroid_source;

    // Assemble the 2x3 affine matrix.
    Eigen::Matrix<float, 2, 3> result;
    result.block<2,2>(0,0) = R;
    result.col(2) = t;

    return result;
}
#include <Eigen/Dense>
#include <vector>
#include <cassert>
#include <cmath>

// Function declaration (assuming it is in the same translation unit or included).
Eigen::Matrix<float, 2, 3> computeBestFitTransform(
    const std::vector<Eigen::Vector2f>& source,
    const std::vector<Eigen::Vector2f>& target);

int main() {
    // Test 1: Perfect alignment with translation only.
    {
        std::vector<Eigen::Vector2f> source = { {1.0f, 2.0f}, {3.0f, 4.0f}, {5.0f, 6.0f} };
        std::vector<Eigen::Vector2f> target = { {3.0f, 4.0f}, {5.0f, 6.0f}, {7.0f, 8.0f} };
        auto transform = computeBestFitTransform(source, target);
        // Expected rotation = identity, translation = (2,2)
        Eigen::Matrix<float, 2, 3> expected;
        expected << 1.0f, 0.0f, 2.0f,
                    0.0f, 1.0f, 2.0f;
        assert(transform.isApprox(expected, 1e-5f));
    }

    // Test 2: Rotation by 90 degrees (pi/2) with no translation.
    {
        std::vector<Eigen::Vector2f> source = { {1.0f, 0.0f}, {2.0f, 0.0f} };
        std::vector<Eigen::Vector2f> target = { {0.0f, 1.0f}, {0.0f, 2.0f} };
        auto transform = computeBestFitTransform(source, target);
        Eigen::Matrix<float, 2, 3> expected;
        expected << 0.0f, -1.0f, 0.0f,
                    1.0f, 0.0f, 0.0f;
        assert(transform.isApprox(expected, 1e-5f));
    }

    // Test 3: Uniform scaling is not allowed; but we test with pure rotation + translation.
    {
        std::vector<Eigen::Vector2f> source = { {0.0f, 0.0f}, {2.0f, 0.0f}, {2.0f, 2.0f}, {0.0f, 2.0f} };
        std::vector<Eigen::Vector2f> target = { {4.0f, 2.0f}, {4.0f, 4.0f}, {2.0f, 4.0f}, {2.0f, 2.0f} };
        auto transform = computeBestFitTransform(source, target);
        // Expected rotation = -90 degrees (or equivalently 270) and translation?
        // Let's compute manually: rotation matrix for -90 deg: [0 1; -1 0]
        // Then translation t = centroid_target - R*centroid_source.
        // centroid_source = (1,1), centroid_target = (3,3)
        // R*centroid_source = (1, -1) => t = (3,3) - (1,-1) = (2,4)
        Eigen::Matrix<float, 2, 3> expected;
        expected << 0.0f, 1.0f, 2.0f,
                   -1.0f, 0.0f, 4.0f;
        assert(transform.isApprox(expected, 1e-5f));
    }

    // Test 4: Empty clouds should return identity with zero translation.
    {
        std::vector<Eigen::Vector2f> source = {};
        std::vector<Eigen::Vector2f> target = { {1.0f, 1.0f} };
        auto transform = computeBestFitTransform(source, target);
        Eigen::Matrix<float, 2, 3> expected;
        expected << 1.0f, 0.0f, 0.0f,
                    0.0f, 1.0f, 0.0f;
        assert(transform.isApprox(expected, 1e-5f));
    }

    // Test 5: Different sizes; use the first min.
    {
        std::vector<Eigen::Vector2f> source = { {0.0f, 0.0f}, {1.0f, 0.0f}, {2.0f, 2.0f} };
        std::vector<Eigen::Vector2f> target = { {2.0f, 0.0f}, {3.0f, 0.0f}, {4.0f, 2.0f} };
        // Use only first two points of source and target: translation (2,0), rotation identity.
        auto transform = computeBestFitTransform(source, target);
        Eigen::Matrix<float, 2, 3> expected;
        expected << 1.0f, 0.0f, 2.0f,
                    0.0f, 1.0f, 0.0f;
        assert(transform.isApprox(expected, 1e-5f));
    }

    // Test 6: Points forming a line (rank-1 covariance) – should still give a valid rotation.
    {
        std::vector<Eigen::Vector2f> source = { {0.0f, 0.0f}, {1.0f, 1.0f}, {2.0f, 2.0f} };
        std::vector<Eigen::Vector2f> target = { {0.0f, 0.0f}, {0.0f, 1.0f}, {0.0f, 2.0f} };
        auto transform = computeBestFitTransform(source, target);
        // The optimal rotation is not uniquely defined, but the SVD method picks a valid one.
        // We just check that applying the transform yields small error.
        Eigen::Matrix<float, 2, 3> T = transform;
        Eigen::Vector2f p0, p1;
        p0 = T * Eigen::Vector3f(0.0f, 0.0f, 1.0f);
        p1 = T * Eigen::Vector3f(1.0f, 1.0f, 1.0f);
        // The transformed source points should be close to the target.
        assert((p0 - Eigen::Vector2f(0.0f, 0.0f)).norm() < 1e-5f);
        assert((p1 - Eigen::Vector2f(0.0f, 1.0f)).norm() < 1e-4f);
    }

    return 0;
}
// The solution uses the standard closed-form solution for absolute orientation (Kabsch algorithm) adapted to 2D without scaling. First, compute the centroids of both point clouds (using only the paired points). Subtract centroids to get centered coordinates. Build a 2x2 cross-covariance matrix H = sum over i of (source_centered_i * target_centered_i^T). Then perform SVD on H: H = U * S * V^T. The optimal rotation matrix is R = V * U^T. To ensure a proper rotation (det = +1), check if det(R) < 0 and flip the sign of the last column of V (or equivalently multiply the last column of R by -1). The translation is t = centroid_target - R * centroid_source. The final 2x3 matrix has R in the first two columns and t in the third column. The time complexity is O(n) for computing centroids, covariance, and SVD (which is constant for a 2x2 matrix). Space complexity is O(n) for storing centered vectors if we copy them, or O(1) if we compute on the fly, but we will use O(n) for clarity. Edge cases: empty inputs – handle by returning identity rotation with zero translation (or maybe return all zeros? Since the problem does not specify, we return identity with translation (0,0) to be safe). Different sizes – truncate to the minimum size. The SVD on a 2x2 matrix is always possible. When the target or source has only one point, the covariance matrix is rank 1, SVD still works, and the rotation will be identity (or possibly a reflection, which we fix). Also, if the points are already perfectly aligned, cov matrix is symmetric and SVD yields identity.
