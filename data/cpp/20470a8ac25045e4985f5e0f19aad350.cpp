/*
Write a C++ function named `computeHomographyAndPose` that takes a vector of 2D image points (measured in pixels) and a vector of 3D object points (all lying on the z=0 plane, with x and y coordinates in world units such as meters), along with a camera intrinsic matrix and distortion coefficients. The function must first undistort the image points using `cv::undistortPoints`, then compute a homography that maps the planar object points (dropping the z-coordinate) to the undistorted image points via `cv::findHomography`. From this homography, extract the rotation matrix and translation vector using the standard decomposition: normalize the first column to unit length, compute the third column as the cross product of the first two, and then enforce a proper rotation matrix via SVD polar decomposition. The function should return the rotation vector (Rodrigues form) and translation vector as a `std::pair<cv::Mat, cv::Mat>` (each being a 3x1 CV_64F Mat). Assume the homography normalization ensures the first column's norm is 1; no need to handle degenerate inputs (e.g., collinear points), but you must ensure the resulting rotation matrix has determinant +1. The function signature should be: `std::pair<cv::Mat, cv::Mat> computeHomographyAndPose(const std::vector<cv::Point2f>& imagePoints, const std::vector<cv::Point3f>& objectPoints, const cv::Mat& cameraMatrix, const cv::Mat& distCoeffs);`
*/
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/calib3d.hpp>
#include <utility>

// Compute homography and extract pose (rotation vector + translation) from coplanar object points.
std::pair<cv::Mat, cv::Mat> computeHomographyAndPose(
    const std::vector<cv::Point2f>& imagePoints,
    const std::vector<cv::Point3f>& objectPoints,
    const cv::Mat& cameraMatrix,
    const cv::Mat& distCoeffs)
{
    // Step 1: Undistort the image points to get normalized coordinates.
    std::vector<cv::Point2f> undistortedPoints;
    cv::undistortPoints(imagePoints, undistortedPoints, cameraMatrix, distCoeffs);

    // Step 2: Build planar object points (drop the z coordinate, which is 0).
    std::vector<cv::Point2f> objectPlanar;
    objectPlanar.reserve(objectPoints.size());
    for (const auto& pt : objectPoints) {
        objectPlanar.push_back(cv::Point2f(pt.x, pt.y));
    }

    // Step 3: Estimate homography from object plane to undistorted image.
    cv::Mat H = cv::findHomography(objectPlanar, undistortedPoints);

    // Step 4: Decompose homography into rotation and translation.
    // Normalize the first column to have unit norm.
    double norm = cv::norm(H.col(0));
    H /= norm;

    cv::Mat r1 = H.col(0).clone();
    cv::Mat r2 = H.col(1).clone();
    cv::Mat r3 = r1.cross(r2);

    // Build the rotation matrix (3x3).
    cv::Mat R(3, 3, CV_64F);
    for (int i = 0; i < 3; ++i) {
        R.at<double>(i, 0) = r1.at<double>(i, 0);
        R.at<double>(i, 1) = r2.at<double>(i, 0);
        R.at<double>(i, 2) = r3.at<double>(i, 0);
    }

    // Enforce proper rotation via SVD polar decomposition.
    cv::Mat W, U, Vt;
    cv::SVDecomp(R, W, U, Vt);
    R = U * Vt;
    if (cv::determinant(R) < 0) {
        Vt.at<double>(2, 0) *= -1;
        Vt.at<double>(2, 1) *= -1;
        Vt.at<double>(2, 2) *= -1;
        R = U * Vt;
    }

    // Translation vector is the third column of the normalized homography.
    cv::Mat tvec = H.col(2).clone();

    // Convert rotation matrix to rotation vector (Rodrigues form).
    cv::Mat rvec;
    cv::Rodrigues(R, rvec);

    return {rvec, tvec};
}
#include <cassert>
#include <cmath>
#include <opencv2/core.hpp>
#include <vector>

// Forward declaration of the solution function (already defined above).
std::pair<cv::Mat, cv::Mat> computeHomographyAndPose(
    const std::vector<cv::Point2f>& imagePoints,
    const std::vector<cv::Point3f>& objectPoints,
    const cv::Mat& cameraMatrix,
    const cv::Mat& distCoeffs);

int main() {
    // Camera intrinsics (simple pinhole, no distortion initially).
    cv::Mat K = (cv::Mat_<double>(3,3) << 800.0, 0.0, 320.0,
                                            0.0, 800.0, 240.0,
                                            0.0, 0.0, 1.0);
    cv::Mat dist = (cv::Mat_<double>(1,4) << 0.0, 0.0, 0.0, 0.0);

    // Known object points on a 3x3 grid with square size = 1.
    std::vector<cv::Point3f> objPts;
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            objPts.push_back(cv::Point3f((float)j, (float)i, 0.0f));

    // Compute image points via hypothetical pose: rotation 0, translation (0,0,100).
    std::vector<cv::Point2f> imgPts;
    for (const auto& p : objPts) {
        double x = p.x * 800.0 + 320.0;  // X = fx*X + cx
        double y = p.y * 800.0 + 240.0;  // Y = fy*Y + cy (since z=100, scale 1)
        imgPts.push_back(cv::Point2f((float)x, (float)y));
    }

    auto result = computeHomographyAndPose(imgPts, objPts, K, dist);
    cv::Mat rvec = result.first;
    cv::Mat tvec = result.second;

    // Since rotation is identity, rvec should be near zero.
    assert(cv::norm(rvec) < 1e-6);
    // Translation should approximately be (0,0,100). Note: normalized homography gives t = (0,0,100)/800? Actually normalized by first column norm, which is 800. So tvec = (0,0,100)/800 = (0,0,0.125). But after normalization, the scale is ambiguous; we only care about direction. Here we check that tvec is proportional to (0,0,1).
    double tx = tvec.at<double>(0);
    double ty = tvec.at<double>(1);
    double tz = tvec.at<double>(2);
    assert(std::abs(tx) < 1e-6);
    assert(std::abs(ty) < 1e-6);
    assert(tz > 0); // Positive z.

    // Test a rotated pose: rotate 90 degrees around Y axis.
    cv::Mat R_known = (cv::Mat_<double>(3,3) << 0, 0, 1,
                                                0, 1, 0,
                                                -1, 0, 0);
    cv::Mat t_known = (cv::Mat_<double>(3,1) << 5.0, 0.0, 3.0);
    std::vector<cv::Point2f> imgPtsRot;
    for (const auto& p : objPts) {
        cv::Mat p3 = (cv::Mat_<double>(3,1) << p.x, p.y, 0.0);
        cv::Mat pc = R_known * p3 + t_known;
        double X = pc.at<double>(0), Y = pc.at<double>(1), Z = pc.at<double>(2);
        imgPtsRot.push_back(cv::Point2f((float)(K.at<double>(0,0)*X/Z + K.at<double>(0,2)),
                                        (float)(K.at<double>(1,1)*Y/Z + K.at<double>(1,2))));
    }

    auto result2 = computeHomographyAndPose(imgPtsRot, objPts, K, dist);
    cv::Mat rvec2 = result2.first;
    cv::Mat tvec2 = result2.second;

    // Recover rotation matrix from rvec2 and compare to known rotation (up to scale).
    cv::Mat R2;
    cv::Rodrigues(rvec2, R2);
    // Check that R2 is close to known rotation (up to a sign? Actually should be exact).
    cv::Mat diffR = R2 - R_known;
    assert(cv::norm(diffR) < 1e-3);
    // Translation direction: normalized tvec should match t_known/||t_known||.
    double normT = cv::norm(tvec2);
    cv::Mat t_expected = t_known / cv::norm(t_known);
    cv::Mat t_actual = tvec2 / normT;
    assert(cv::norm(t_actual - t_expected) < 1e-3);

    // Edge case: all points collinear should still produce some output (though pose is ambiguous).
    std::vector<cv::Point3f> collinearPts = {cv::Point3f(0,0,0), cv::Point3f(1,0,0), cv::Point3f(2,0,0)};
    std::vector<cv::Point2f> collinearImg = {cv::Point2f(320,240), cv::Point2f(1120,240), cv::Point2f(1920,240)};
    auto result3 = computeHomographyAndPose(collinearImg, collinearPts, K, dist);
    // Should not crash; output dimensions are 3x1.
    assert(result3.first.rows == 3 && result3.first.cols == 1);
    assert(result3.second.rows == 3 && result3.second.cols == 1);

    return 0;
}
// The approach follows the classic pose estimation for planar objects: first correct lens distortion by calling `cv::undistortPoints` with the camera intrinsics and distortion coefficients—this transforms the raw pixel coordinates into normalized image coordinates (approximately metric). Next, since the object points lie on a plane (z=0), we project them to 2D by taking (x, y) and use `cv::findHomography` to find a 3x3 matrix H such that for each corresponding pair, the homogeneous mapping holds. The homography relates the plane to the image as H = [r1 r2 t], where r1 and r2 are the first two columns of the rotation matrix and t is the translation vector. To extract a valid rotation, we normalize H by dividing by the norm of its first column (which should equal 1/focal length in a scaled sense). Then set the rotation matrix's first two columns as the normalized columns, and the third as their cross product. Because numerical errors may cause the cross product to be slightly non-orthonormal, we perform SVD and set R = U*V^T, which gives the closest proper rotation. If the determinant is negative, we flip the third column of Vt to ensure det(R)=+1. Finally, we convert R to a rotation vector via `cv::Rodrigues`. The time complexity is dominated by `undistortPoints` (O(n) for n points) and `findHomography` (O(n) plus SVD on a 2n×9 matrix, which is O(n) in practice but ~O(9^3) constant). SVD of a 3×3 matrix is O(1). Memory usage is O(n) for intermediate vectors.
