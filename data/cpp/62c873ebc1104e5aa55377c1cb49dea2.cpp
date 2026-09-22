Write a standalone C++ function named `decomposeSquarePose` that takes a square length (`double squareSize`) and an array of its detected corner image points (a `std::vector<cv::Point2f>` of exactly 4 points, ordered clockwise starting from the top-left) along with an OpenCV camera matrix (`cv::InputArray cameraMatrix`) and distortion coefficients (`cv::InputArray distCoeffs`). The function must compute and return a `std::vector<cv::Mat>` containing two possible 4×4 pose transformation matrices (model-to-camera, as `CV_32F` matrices), each representing a plausible rigid transformation (rotation + translation) that maps the square from world coordinates (centered at origin, plane z=0, corners at (±squareSize/2, ±squareSize/2, 0)) to the camera coordinate system that produced the given image projections. The two solutions arise from the planar ambiguity of pose estimation from four coplanar points. The function must use only the IPPE algorithm logic (homography computation from square corners, canonical form solution, convergence to two rotations, and translation via linear least squares), reusing the helper routines `getRTMatrix`, `computeRotations`, `computeTranslation`, `homographyFromSquarePoints`, and `solveCanonicalForm` provided in the task context. It should not depend on the full IPPE class, but you may copy and adapt the necessary helper functions from the code snippet. The solution must handle both `CV_32F` and `CV_64F` image point types internally (convert to `CV_64F` for processing) and return the two pose matrices in no particular order (they are the two possible solutions). Include necessary OpenCV headers and use `cv::InputArray` for inputs and `std::vector<cv::Mat>` as the return type.
#include <opencv2/opencv.hpp>
#include <cassert>
#include <vector>
#include <cmath>

int main() {
    // Define a simple intrinsic matrix (no distortion).
    cv::Mat K = (cv::Mat_<double>(3,3) << 1000, 0, 640,
                                            0, 1000, 360,
                                            0, 0, 1);
    std::vector<double> distCoeffs = {0, 0, 0, 0, 0};

    // Test 1: Fronto-parallel square, centered.
    // Object points: square corners at (±0.5, ±0.5, 0); camera at z=2.
    double size = 1.0;
    std::vector<cv::Point3f> obj3D = {
        cv::Point3f(-0.5f, 0.5f, 0.0f),
        cv::Point3f( 0.5f, 0.5f, 0.0f),
        cv::Point3f( 0.5f,-0.5f, 0.0f),
        cv::Point3f(-0.5f,-0.5f, 0.0f)
    };
    cv::Mat rvec = (cv::Mat_<double>(3,1) << 0, 0, 0);
    cv::Mat tvec = (cv::Mat_<double>(3,1) << 0, 0, 2);
    std::vector<cv::Point2f> imgPts;
    cv::projectPoints(obj3D, rvec, tvec, K, distCoeffs, imgPts);

    auto poses = decomposeSquarePose(size, imgPts, K, distCoeffs);
    assert(poses.size() == 2);
    // Both poses should have translation z close to 2.
    for (const auto& M : poses) {
        assert(std::abs(M.at<float>(2,3) - 2.0f) < 1e-3);
        // Rotation should be close to identity.
        double diff = std::abs(M.at<float>(0,0) - 1.0) + std::abs(M.at<float>(1,1) - 1.0) + std::abs(M.at<float>(2,2) - 1.0);
        assert(diff < 1e-3);
    }

    // Test 2: Square rotated by 30 degrees around X axis.
    rvec = (cv::Mat_<double>(3,1) << 0.5236, 0, 0); // ~30 degrees
    tvec = (cv::Mat_<double>(3,1) << 0, 0, 3);
    cv::projectPoints(obj3D, rvec, tvec, K, distCoeffs, imgPts);
    poses = decomposeSquarePose(size, imgPts, K, distCoeffs);
    assert(poses.size() == 2);
    // Both translations should have z near 3.
    for (const auto& M : poses) {
        assert(std::abs(M.at<float>(2,3) - 3.0f) < 1e-2);
    }

    // Test 3: No camera matrix (points already normalized).
    // Normalized image points for the same fronto-parallel case: focal=1.
    std::vector<cv::Point2f> normPts;
    for (auto& p : imgPts) {
        normPts.emplace_back((p.x - 640) / 1000.0f, (p.y - 360) / 1000.0f);
    }
    cv::Mat empty;
    auto posesNorm = decomposeSquarePose(size, normPts, empty, empty);
    assert(posesNorm.size() == 2);
    for (const auto& M : posesNorm) {
        assert(std::abs(M.at<float>(2,3) - 3.0f) < 1e-2);
    }

    // Test 4: Square with distortion coefficients (simulate undistortion needed).
    std::vector<double> dist = {0.1, -0.05, 0.001, 0.002, 0.0};
    cv::projectPoints(obj3D, rvec, tvec, K, dist, imgPts);
    poses = decomposeSquarePose(size, imgPts, K, dist);
    assert(poses.size() == 2);
    for (const auto& M : poses) {
        assert(std::abs(M.at<float>(2,3) - 3.0f) < 1e-1); // looser tolerance due to distortion
    }

    // Test 5: Check that the two poses are different (not identical) for non-fronto-parallel.
    rvec = (cv::Mat_<double>(3,1) << 0.2, -0.3, 0.1);
    tvec = (cv::Mat_<double>(3,1) << 0.1, -0.2, 2.5);
    cv::projectPoints(obj3D, rvec, tvec, K, distCoeffs, imgPts);
    poses = decomposeSquarePose(size, imgPts, K, distCoeffs);
    assert(poses.size() == 2);
    bool different = false;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (std::abs(poses[0].at<float>(i,j) - poses[1].at<float>(i,j)) > 1e-4)
                different = true;
        }
    }
    assert(different); // The two solutions should differ in general.

    return 0;
}
#include <opencv2/opencv.hpp>
#include <vector>
#include <cmath>
#include <cassert>
#include <limits>

// Helper: Convert a 3x3 rotation matrix and 3x1 translation into a 4x4 pose matrix.
static cv::Mat getRTMatrix(const cv::Mat& R, const cv::Mat& T, int forceType) {
    cv::Mat M;
    cv::Mat R64, T64;
    if (R.depth() == CV_32F) R.convertTo(R64, CV_64F);
    else R64 = R.clone();
    if (T.depth() == CV_32F) T.convertTo(T64, CV_64F);
    else T64 = T.clone();

    cv::Mat Matrix = cv::Mat::eye(4, 4, CV_64FC1);
    cv::Mat R33 = Matrix(cv::Rect(0, 0, 3, 3));
    if (R64.total() == 3) {
        cv::Rodrigues(R64, R33);
    } else if (R64.total() == 9) {
        R64.copyTo(R33);
    }
    for (int i = 0; i < 3; i++)
        Matrix.at<double>(i, 3) = T64.ptr<double>(0)[i];

    if (forceType == -1) return Matrix;
    else {
        cv::Mat MTyped;
        Matrix.convertTo(MTyped, forceType);
        return MTyped;
    }
}

// Helper: Compute a rotation that rotates vector 'a' to the Z axis.
static void rotateVec2ZAxis(const cv::Mat& a, cv::Mat& Ra) {
    Ra.create(3, 3, CV_64FC1);
    double ax = a.at<double>(0);
    double ay = a.at<double>(1);
    double az = a.at<double>(2);
    double nrm = std::sqrt(ax*ax + ay*ay + az*az);
    ax /= nrm; ay /= nrm; az /= nrm;

    double c = az;
    if (std::abs(1.0 + c) < std::numeric_limits<float>::epsilon()) {
        Ra.setTo(0.0);
        Ra.at<double>(0,0) = 1.0;
        Ra.at<double>(1,1) = 1.0;
        Ra.at<double>(2,2) = -1.0;
    } else {
        double d = 1.0 / (1.0 + c);
        double ax2 = ax*ax, ay2 = ay*ay, axay = ax*ay;
        Ra.at<double>(0,0) = -ax2*d + 1.0;
        Ra.at<double>(0,1) = -axay*d;
        Ra.at<double>(0,2) = -ax;
        Ra.at<double>(1,0) = -axay*d;
        Ra.at<double>(1,1) = -ay2*d + 1.0;
        Ra.at<double>(1,2) = -ay;
        Ra.at<double>(2,0) = ax;
        Ra.at<double>(2,1) = ay;
        Ra.at<double>(2,2) = 1.0 - (ax2+ay2)*d;
    }
}

// Helper: Compute two rotation candidates from the homography's Jacobian at (0,0).
static void computeRotations(double j00, double j01, double j10, double j11,
                             double p, double q, cv::Mat& R1, cv::Mat& R2) {
    R1.create(3, 3, CV_64FC1);
    R2.create(3, 3, CV_64FC1);

    cv::Mat v(3, 1, CV_64FC1);
    v.at<double>(0) = p;
    v.at<double>(1) = q;
    v.at<double>(2) = 1.0;
    cv::Mat Rv;
    rotateVec2ZAxis(v, Rv);
    Rv = Rv.t();

    double rv00 = Rv.at<double>(0,0), rv01 = Rv.at<double>(0,1), rv02 = Rv.at<double>(0,2);
    double rv10 = Rv.at<double>(1,0), rv11 = Rv.at<double>(1,1), rv12 = Rv.at<double>(1,2);
    double rv20 = Rv.at<double>(2,0), rv21 = Rv.at<double>(2,1), rv22 = Rv.at<double>(2,2);

    double b00 = rv00 - p * rv20;
    double b01 = rv01 - p * rv21;
    double b10 = rv10 - q * rv20;
    double b11 = rv11 - q * rv21;

    double dtinv = 1.0 / (b00*b11 - b01*b10);
    double binv00 = dtinv * b11;
    double binv01 = -dtinv * b01;
    double binv10 = -dtinv * b10;
    double binv11 = dtinv * b00;

    double a00 = binv00*j00 + binv01*j10;
    double a01 = binv00*j01 + binv01*j11;
    double a10 = binv10*j00 + binv11*j10;
    double a11 = binv10*j01 + binv11*j11;

    double ata00 = a00*a00 + a01*a01;
    double ata01 = a00*a10 + a01*a11;
    double ata11 = a10*a10 + a11*a11;
    double gamma = std::sqrt(0.5 * (ata00 + ata11 + std::sqrt((ata00-ata11)*(ata00-ata11) + 4.0*ata01*ata01)));

    double rtilde00 = a00 / gamma;
    double rtilde01 = a01 / gamma;
    double rtilde10 = a10 / gamma;
    double rtilde11 = a11 / gamma;

    double rtilde00_2 = rtilde00*rtilde00;
    double rtilde01_2 = rtilde01*rtilde01;
    double rtilde10_2 = rtilde10*rtilde10;
    double rtilde11_2 = rtilde11*rtilde11;

    double b0 = std::sqrt(-rtilde00_2 - rtilde10_2 + 1.0);
    double b1 = std::sqrt(-rtilde01_2 - rtilde11_2 + 1.0);
    double sp = (-rtilde00*rtilde01 - rtilde10*rtilde11);
    if (sp < 0) b1 = -b1;

    R1.at<double>(0,0) = (rtilde00)*rv00 + (rtilde10)*rv01 + (b0)*rv02;
    R1.at<double>(0,1) = (rtilde01)*rv00 + (rtilde11)*rv01 + (b1)*rv02;
    R1.at<double>(0,2) = (b1*rtilde10 - b0*rtilde11)*rv00 + (b0*rtilde01 - b1*rtilde00)*rv01 + (rtilde00*rtilde11 - rtilde01*rtilde10)*rv02;
    R1.at<double>(1,0) = (rtilde00)*rv10 + (rtilde10)*rv11 + (b0)*rv12;
    R1.at<double>(1,1) = (rtilde01)*rv10 + (rtilde11)*rv11 + (b1)*rv12;
    R1.at<double>(1,2) = (b1*rtilde10 - b0*rtilde11)*rv10 + (b0*rtilde01 - b1*rtilde00)*rv11 + (rtilde00*rtilde11 - rtilde01*rtilde10)*rv12;
    R1.at<double>(2,0) = (rtilde00)*rv20 + (rtilde10)*rv21 + (b0)*rv22;
    R1.at<double>(2,1) = (rtilde01)*rv20 + (rtilde11)*rv21 + (b1)*rv22;
    R1.at<double>(2,2) = (b1*rtilde10 - b0*rtilde11)*rv20 + (b0*rtilde01 - b1*rtilde00)*rv21 + (rtilde00*rtilde11 - rtilde01*rtilde10)*rv22;

    R2.at<double>(0,0) = (rtilde00)*rv00 + (rtilde10)*rv01 + (-b0)*rv02;
    R2.at<double>(0,1) = (rtilde01)*rv00 + (rtilde11)*rv01 + (-b1)*rv02;
    R2.at<double>(0,2) = (b0*rtilde11 - b1*rtilde10)*rv00 + (b1*rtilde00 - b0*rtilde01)*rv01 + (rtilde00*rtilde11 - rtilde01*rtilde10)*rv02;
    R2.at<double>(1,0) = (rtilde00)*rv10 + (rtilde10)*rv11 + (-b0)*rv12;
    R2.at<double>(1,1) = (rtilde01)*rv10 + (rtilde11)*rv11 + (-b1)*rv12;
    R2.at<double>(1,2) = (b0*rtilde11 - b1*rtilde10)*rv10 + (b1*rtilde00 - b0*rtilde01)*rv11 + (rtilde00*rtilde11 - rtilde01*rtilde10)*rv12;
    R2.at<double>(2,0) = (rtilde00)*rv20 + (rtilde10)*rv21 + (-b0)*rv22;
    R2.at<double>(2,1) = (rtilde01)*rv20 + (rtilde11)*rv21 + (-b1)*rv22;
    R2.at<double>(2,2) = (b0*rtilde11 - b1*rtilde10)*rv20 + (b1*rtilde00 - b0*rtilde01)*rv21 + (rtilde00*rtilde11 - rtilde01*rtilde10)*rv22;
}

// Helper: Compute translation for a given rotation using linear least squares.
static void computeTranslation(const cv::Mat& objectPoints, const cv::Mat& normalizedImgPoints,
                               const cv::Mat& R, cv::Mat& t) {
    size_t n = static_cast<size_t>(normalizedImgPoints.total());
    t.create(3, 1, CV_64FC1);

    double ATA00 = n;
    double ATA02 = 0, ATA11 = n, ATA12 = 0, ATA20 = 0, ATA21 = 0, ATA22 = 0;
    double ATb0 = 0, ATb1 = 0, ATb2 = 0;
    double S00, S01, S02, S10, S11, S12, S20, S21, S22;

    for (size_t i = 0; i < n; i++) {
        double rx = R.at<double>(0,0)*objectPoints.at<cv::Vec2d>(i)(0) + R.at<double>(0,1)*objectPoints.at<cv::Vec2d>(i)(1);
        double ry = R.at<double>(1,0)*objectPoints.at<cv::Vec2d>(i)(0) + R.at<double>(1,1)*objectPoints.at<cv::Vec2d>(i)(1);
        double rz = R.at<double>(2,0)*objectPoints.at<cv::Vec2d>(i)(0) + R.at<double>(2,1)*objectPoints.at<cv::Vec2d>(i)(1);
        double a2 = -normalizedImgPoints.at<cv::Vec2d>(i)(0);
        double b2 = -normalizedImgPoints.at<cv::Vec2d>(i)(1);
        ATA02 += a2; ATA12 += b2; ATA20 += a2; ATA21 += b2; ATA22 += a2*a2 + b2*b2;
        double bx = -a2*rz - rx;
        double by = -b2*rz - ry;
        ATb0 += bx; ATb1 += by; ATb2 += a2*bx + b2*by;
    }

    double detAInv = 1.0 / (ATA00*ATA11*ATA22 - ATA00*ATA12*ATA21 - ATA02*ATA11*ATA20);
    S00 = ATA11*ATA22 - ATA12*ATA21;
    S01 = ATA02*ATA21;
    S02 = -ATA02*ATA11;
    S10 = ATA12*ATA20;
    S11 = ATA00*ATA22 - ATA02*ATA20;
    S12 = -ATA00*ATA12;
    S20 = -ATA11*ATA20;
    S21 = -ATA00*ATA21;
    S22 = ATA00*ATA11;

    t.at<double>(0) = detAInv * (S00*ATb0 + S01*ATb1 + S02*ATb2);
    t.at<double>(1) = detAInv * (S10*ATb0 + S11*ATb1 + S12*ATb2);
    t.at<double>(2) = detAInv * (S20*ATb0 + S21*ATb1 + S22*ATb2);
}

// Helper: Compute homography from square corner points (ordered clockwise from top-left).
static void homographyFromSquarePoints(const cv::Mat& targetPoints, double halfLength, cv::Mat& H) {
    H.create(3, 3, CV_64FC1);
    double p1x, p1y, p2x, p2y, p3x, p3y, p4x, p4y;
    if (targetPoints.type() == CV_32FC2) {
        p1x = -targetPoints.at<cv::Vec2f>(0)(0); p1y = -targetPoints.at<cv::Vec2f>(0)(1);
        p2x = -targetPoints.at<cv::Vec2f>(1)(0); p2y = -targetPoints.at<cv::Vec2f>(1)(1);
        p3x = -targetPoints.at<cv::Vec2f>(2)(0); p3y = -targetPoints.at<cv::Vec2f>(2)(1);
        p4x = -targetPoints.at<cv::Vec2f>(3)(0); p4y = -targetPoints.at<cv::Vec2f>(3)(1);
    } else {
        p1x = -targetPoints.at<cv::Vec2d>(0)(0); p1y = -targetPoints.at<cv::Vec2d>(0)(1);
        p2x = -targetPoints.at<cv::Vec2d>(1)(0); p2y = -targetPoints.at<cv::Vec2d>(1)(1);
        p3x = -targetPoints.at<cv::Vec2d>(2)(0); p3y = -targetPoints.at<cv::Vec2d>(2)(1);
        p4x = -targetPoints.at<cv::Vec2d>(3)(0); p4y = -targetPoints.at<cv::Vec2d>(3)(1);
    }

    double detsInv = -1 / (halfLength * (p1x*p2y - p2x*p1y - p1x*p4y + p2x*p3y - p3x*p2y + p4x*p1y + p3x*p4y - p4x*p3y));
    H.at<double>(0,0) = detsInv * (p1x*p3x*p2y - p2x*p3x*p1y - p1x*p4x*p2y + p2x*p4x*p1y - p1x*p3x*p4y + p1x*p4x*p3y + p2x*p3x*p4y - p2x*p4x*p3y);
    H.at<double>(0,1) = detsInv * (p1x*p2x*p3y - p1x*p3x*p2y - p1x*p2x*p4y + p2x*p4x*p1y + p1x*p3x*p4y - p3x*p4x*p1y - p2x*p4x*p3y + p3x*p4x*p2y);
    H.at<double>(0,2) = detsInv * halfLength * (p1x*p2x*p3y - p2x*p3x*p1y - p1x*p2x*p4y + p1x*p4x*p2y - p1x*p4x*p3y + p3x*p4x*p1y + p2x*p3x*p4y - p3x*p4x*p2y);
    H.at<double>(1,0) = detsInv * (p1x*p2y*p3y - p2x*p1y*p3y - p1x*p2y*p4y + p2x*p1y*p4y - p3x*p1y*p4y + p4x*p1y*p3y + p3x*p2y*p4y - p4x*p2y*p3y);
    H.at<double>(1,1) = detsInv * (p2x*p1y*p3y - p3x*p1y*p2y - p1x*p2y*p4y + p4x*p1y*p2y + p1x*p3y*p4y - p4x*p1y*p3y - p2x*p3y*p4y + p3x*p2y*p4y);
    H.at<double>(1,2) = detsInv * halfLength * (p1x*p2y*p3y - p3x*p1y*p2y - p2x*p1y*p4y + p4x*p1y*p2y - p1x*p3y*p4y + p3x*p1y*p4y + p2x*p3y*p4y - p4x*p2y*p3y);
    H.at<double>(2,0) = -detsInv * (p1x*p3y - p3x*p1y - p1x*p4y - p2x*p3y + p3x*p2y + p4x*p1y + p2x*p4y - p4x*p2y);
    H.at<double>(2,1) = detsInv * (p1x*p2y - p2x*p1y - p1x*p3y + p3x*p1y + p2x*p4y - p4x*p2y - p3x*p4y + p4x*p3y);
    H.at<double>(2,2) = 1.0;
}

// Main function: Decompose the pose of a square from its image points using IPPE.
std::vector<cv::Mat> decomposeSquarePose(double squareSize,
                                         const std::vector<cv::Point2f>& imgPoints,
                                         cv::InputArray cameraMatrix,
                                         cv::InputArray distCoeffs) {
    assert(imgPoints.size() == 4);

    // Convert image points to normalized coordinates if camera matrix provided.
    cv::Mat normalizedPts;
    if (cameraMatrix.empty()) {
        cv::Mat(imgPoints).convertTo(normalizedPts, CV_64FC2);
    } else {
        cv::undistortPoints(imgPoints, normalizedPts, cameraMatrix, distCoeffs);
        if (normalizedPts.depth() != CV_64F)
            normalizedPts.convertTo(normalizedPts, CV_64F);
    }

    // Build canonical square object points (2D, centered, plane z=0).
    double half = squareSize / 2.0;
    cv::Mat canonicalObj(1, 4, CV_64FC2);
    canonicalObj.at<cv::Vec2d>(0) = cv::Vec2d(-half, half);
    canonicalObj.at<cv::Vec2d>(1) = cv::Vec2d(half, half);
    canonicalObj.at<cv::Vec2d>(2) = cv::Vec2d(half, -half);
    canonicalObj.at<cv::Vec2d>(3) = cv::Vec2d(-half, -half);

    // Compute homography from canonical square corners to normalized image points.
    cv::Mat H;
    homographyFromSquarePoints(normalizedPts, half, H);

    // Extract Jacobian at origin.
    double j00 = H.at<double>(0,0) - H.at<double>(2,0)*H.at<double>(0,2);
    double j01 = H.at<double>(0,1) - H.at<double>(2,1)*H.at<double>(0,2);
    double j10 = H.at<double>(1,0) - H.at<double>(2,0)*H.at<double>(1,2);
    double j11 = H.at<double>(1,1) - H.at<double>(2,1)*H.at<double>(1,2);
    double v0 = H.at<double>(0,2);
    double v1 = H.at<double>(1,2);

    // Compute two rotation candidates.
    cv::Mat R1, R2;
    computeRotations(j00, j01, j10, j11, v0, v1, R1, R2);

    // Compute translations for each rotation.
    cv::Mat t1, t2;
    computeTranslation(canonicalObj, normalizedPts, R1, t1);
    computeTranslation(canonicalObj, normalizedPts, R2, t2);

    // Build 4x4 pose matrices and convert to CV_32F.
    std::vector<cv::Mat> poses;
    poses.push_back(getRTMatrix(R1, t1, CV_32F));
    poses.push_back(getRTMatrix(R2, t2, CV_32F));

    return poses;
}
// The task is to implement a pose estimation function for a square planar target, returning two possible solutions because a planar object viewed from a single image has a two‑fold ambiguity in rotation (the "flip" ambiguity). The solution approach follows the IPPE (Infinitely Positive‑Plane Estimation) method:
// 1. **Undistort the image points**: If a camera matrix is provided, use `cv::undistortPoints` to convert the detected corners to normalized pixel coordinates. If empty, assume the given points are already normalized.
// 2. **Compute the homography** mapping the canonical square corners (centered at origin, half‑size `squareSize/2`) to the normalized image points. Use the analytic formula for a square (provided in `homographyFromSquarePoints`) which directly gives the homography matrix `H` (3×3, `CV_64F`).
// 3. **Derive the Jacobian** of the homography at the origin (0,0) using the entries of `H`: `j00 = H00 - H20*H02`, `j01 = H01 - H21*H02`, `j10 = H10 - H20*H12`, `j11 = H11 - H21*H12`. Also extract the translation of the origin: `v0 = H02`, `v1 = H12`.
// 4. **Compute two rotation candidates** from the Jacobian using the algorithm from `computeRotations` (which performs a 2×2 SVD‑like decomposition, reconstructs the rotation matrices via the Rodrigues‑type formula, and applies the rotation that aligns the image origin vector with the z‑axis). This yields two 3×3 rotation matrices `R1` and `R2`.
// 5. **For each rotation, compute the translation** by solving a linear least‑squares system `At = b` where `A` is built from the normalized image points and the rotation applied to the canonical object points (as in `computeTranslation`). This gives translation vectors `t1` and `t2`.
// 6. **Assemble the 4×4 pose matrices** `[R|t]` (with bottom row [0,0,0,1]) and convert them to `CV_32F` matrices using the helper `getRTMatrix` (which also handles the conversion from rotation matrix or rotation vector). Return both matrices in a vector.
//
// Edge cases: The homography must be well‑conditioned (non‑collinear points, square not degenerate). The translation computation assumes the points are not all on a line; the linear system is solved by normal‑equation inversion, which requires the matrix to be invertible (non‑zero determinant). The code handles both `CV_32F` and `CV_64F` input types by converting to `CV_64F` internally. The two solutions are distinct when the square is not viewed face‑on; for a face‑on view they may coincide, but the algorithm still returns two copies. Time complexity is O(1) because the number of points is fixed at 4; space complexity is O(1) for the temporary matrices.
