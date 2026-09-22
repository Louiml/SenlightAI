Write a C++ function named `computeHomographyFromPoses` that takes two 4x4 camera-to-world transformation matrices (as `cv::Mat` of type `CV_64F`), a 3x3 camera intrinsic matrix (as `cv::Mat` of type `CV_64F`), and an output parameter `cv::Mat& homography`. The function must extract the 3x3 rotation submatrices from each pose, compute the relative rotation \( R_{2 \to 1} = R_1 R_2^T \), then compute the homography \( H = K R_{2 \to 1} K^{-1} \), normalize it so \( H(2,2) = 1 \), and store the result in the output parameter. The function must return `true` on success and `false` if any input matrix is empty, has incorrect dimensions, or is not of type `CV_64F`. The function should not print anything or modify the input matrices; it must handle division-by-zero gracefully by returning `false` if normalization fails (i.e., if the bottom-right element is zero). For all valid inputs, the output homography should be a 3x3 `CV_64F` matrix with its bottom-right element equal to 1. Ensure proper use of `const` correctness and include all necessary OpenCV headers.

The solution follows the standard formula for computing a homography from two camera rotations and known intrinsics when the camera rotates about its optical center. First, extract the 3x3 rotation matrices \( R_1 \) and \( R_2 \) from the top-left 3x3 block of each 4x4 pose. The relative rotation that maps points from camera 2's coordinate frame to camera 1's frame is \( R_{2 \to 1} = R_1 R_2^T \) (since \( R_2^T \) is the inverse of \( R_2 \)). The homography that warps an image captured from camera 2 to appear as if captured from camera 1 is \( H = K R_{2 \to 1} K^{-1} \). After matrix multiplication, normalize by dividing all elements by \( H(2,2) \) to make the transformation invariant to scale. Edge cases: input matrices must be exactly 4x4 for poses and 3x3 for intrinsics, all of type `CV_64F`; empty or misdimensioned inputs return false. If after normalization the divisor is zero, return false to avoid division by zero. The algorithm involves constant-size matrix multiplications and inversions (3x3 inverse is \( O(1) \) in practice), so time complexity is \( O(1) \) and auxiliary space is \( O(1) \) beyond the output matrix allocation. No iteration over image data is performed; the function is purely algebraic.

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>

/**
 * Compute the homography that maps points from camera 2's image to camera 1's image,
 * given the two camera-to-world poses and the camera intrinsic matrix.
 *
 * @param pose1 4x4 CV_64F camera-to-world transformation for camera 1.
 * @param pose2 4x4 CV_64F camera-to-world transformation for camera 2.
 * @param K     3x3 CV_64F camera intrinsic matrix.
 * @param homography Output 3x3 CV_64F homography matrix (normalized so H(2,2)=1).
 * @return true on success, false on invalid input or normalization failure.
 */
bool computeHomographyFromPoses(const cv::Mat& pose1, const cv::Mat& pose2,
                                const cv::Mat& K, cv::Mat& homography)
{
    // Validate input sizes and types
    if (pose1.size() != cv::Size(4, 4) || pose2.size() != cv::Size(4, 4) || K.size() != cv::Size(3, 3))
        return false;
    if (pose1.type() != CV_64F || pose2.type() != CV_64F || K.type() != CV_64F)
        return false;
    if (pose1.empty() || pose2.empty() || K.empty())
        return false;

    // Extract rotation submatrices (top-left 3x3)
    cv::Mat R1 = pose1(cv::Rect(0, 0, 3, 3)).clone();  // clone to get continuous matrix if needed
    cv::Mat R2 = pose2(cv::Rect(0, 0, 3, 3)).clone();

    // Relative rotation R_2to1 = R1 * R2^T
    cv::Mat R_2to1 = R1 * R2.t();

    // Compute homography H = K * R_2to1 * K^{-1}
    cv::Mat K_inv = K.inv();
    if (K_inv.empty())
        return false;

    cv::Mat H = K * R_2to1 * K_inv;

    // Normalize so that H(2,2) == 1
    double denominator = H.at<double>(2, 2);
    if (std::abs(denominator) < 1e-12)
        return false;

    H /= denominator;

    homography = H.clone();
    return true;
}

#include <cassert>
#include <cmath>
#include <opencv2/core.hpp>

// Include the solution function (in practice this would be in a header)
// For the test, assume the function is defined above.

int main() {
    // Test 1: Identity pose for both, identity intrinsics -> homography should be identity
    cv::Mat pose1 = cv::Mat::eye(4, 4, CV_64F);
    cv::Mat pose2 = cv::Mat::eye(4, 4, CV_64F);
    cv::Mat K = cv::Mat::eye(3, 3, CV_64F);
    cv::Mat H;
    assert(computeHomographyFromPoses(pose1, pose2, K, H) == true);
    assert(H.size() == cv::Size(3, 3));
    assert(H.type() == CV_64F);
    assert(cv::norm(H - cv::Mat::eye(3, 3, CV_64F)) < 1e-9);
    assert(std::abs(H.at<double>(2, 2) - 1.0) < 1e-9);

    // Test 2: Rotation about Y by 90 degrees between poses (simplified)
    // Use a synthetic rotation: R1 = identity, R2 = rotation about Z by 90 deg
    cv::Mat poseA = cv::Mat::eye(4, 4, CV_64F);
    cv::Mat poseB = cv::Mat::eye(4, 4, CV_64F);
    double theta = CV_PI / 2.0;
    poseB.at<double>(0, 0) = std::cos(theta);
    poseB.at<double>(0, 1) = -std::sin(theta);
    poseB.at<double>(1, 0) = std::sin(theta);
    poseB.at<double>(1, 1) = std::cos(theta);
    cv::Mat K2 = (cv::Mat_<double>(3,3) << 700.0, 0.0, 320.0,
                                            0.0, 700.0, 240.0,
                                            0.0, 0.0, 1.0);
    cv::Mat H2;
    assert(computeHomographyFromPoses(poseA, poseB, K2, H2) == true);
    // For this specific case, H should be K * Rz(-theta) * K^{-1} because R_2to1 = I * R2^T
    cv::Mat expect = K2 * (poseB(cv::Rect(0,0,3,3)).t()) * K2.inv();
    expect /= expect.at<double>(2,2);
    assert(cv::norm(H2 - expect) < 1e-9);

    // Test 3: Invalid input sizes
    cv::Mat badPose = cv::Mat::eye(3, 3, CV_64F);
    cv::Mat goodK = cv::Mat::eye(3, 3, CV_64F);
    assert(computeHomographyFromPoses(badPose, poseB, goodK, H) == false);

    // Test 4: Invalid type (CV_32F instead of CV_64F)
    cv::Mat floatPose = cv::Mat::eye(4, 4, CV_32F);
    assert(computeHomographyFromPoses(floatPose, poseB, goodK, H) == false);

    // Test 5: Empty input
    cv::Mat empty;
    assert(computeHomographyFromPoses(empty, poseB, goodK, H) == false);

    // Test 6: Verification of normalized bottom-right element
    cv::Mat poseC = cv::Mat::eye(4, 4, CV_64F);
    poseC.at<double>(1, 0) = 0.1;  // some rotation
    cv::Mat K3 = (cv::Mat_<double>(3,3) << 100.0, 0.0, 50.0,
                                            0.0, 100.0, 50.0,
                                            0.0, 0.0, 1.0);
    cv::Mat H3;
    assert(computeHomographyFromPoses(pose1, poseC, K3, H3) == true);
    assert(std::abs(H3.at<double>(2, 2) - 1.0) < 1e-12);

    return 0;
}
