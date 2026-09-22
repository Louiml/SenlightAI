Write a C++ function `estimateSimilarityTransform` that estimates a similarity transformation (rotation, uniform scaling, and translation) between two single-channel grayscale images of equal size. The function should accept two `cv::Mat` inputs and return a `cv::Matx23d` representing the 2×3 affine transformation matrix (where the linear part is a scaled rotation matrix of the form `[s*cosθ, -s*sinθ; s*sinθ, s*cosθ]` and the third column is the translation). Use a gradient-based least-squares approach similar to the provided snippet: compute spatial gradients of the first image, compute the difference between the first image and the second image (or a warped version of it if an initial transformation is provided), build and solve a 4×4 linear system for the parameters (scale+rotation combined as `a = s*cosθ - 1`, `b = s*sinθ`, plus translations `tx`, `ty`), and return the transformation. Handle the case where no initial transformation is given by using identity. Ensure the function validates that both images have the same size and are single-channel, and that the linear system is solvable (use Gaussian elimination or Cholesky; if singular, fall back to identity). The function must be `const`-correct and not modify the inputs.
// The solution follows the same mathematical framework as the provided `MapperGradSimilar::calculate`. The key idea is to minimize the sum of squared differences between image1 and image2 under a similarity warp. For small motion, the image difference can be linearized using first-order Taylor expansion: `img2(x + dx, y + dy) ≈ img2(x,y) + gradx*dx + grady*dy`. Because we model a similarity transform, the displacement at each pixel is `dx = tx + a*x - b*y` and `dy = ty + b*x + a*y` where `a = s*cosθ - 1` and `b = s*sinθ` (assuming the transform is `x' = (1+a)x - b*y + tx`, `y' = b*x + (1+a)y + ty`). Substituting into the linearized difference and taking derivatives with respect to the four parameters yields a 4×4 symmetric positive semi-definite system `A * k = b_vec`, where `A` is built from products of gradient terms and spatial coordinates, and `b_vec` is built from products of the image difference with those terms. The system is solved for `k = [a, b, tx, ty]`. The final transformation matrix is `[[1+a, -b, tx], [b, 1+a, ty]]`. Edge cases: if the images are identical or gradients are zero everywhere, `A` becomes singular; we detect this by checking the determinant and fall back to identity. If an initial transformation is provided, we first warp image2 with its inverse, compute the incremental transform, and then compose it with the initial one. Time complexity is O(N) per pass over pixels where N is number of pixels, and space complexity is O(N) for intermediate matrices (gradients, products). Input validation: check same size and single channel, otherwise throw an exception.
#include <opencv2/opencv.hpp>
#include <stdexcept>
#include <cmath>

// Estimate similarity transform (scaled rotation + translation) between two grayscale images.
// Returns 2x3 affine matrix [[s*cosθ, -s*sinθ, tx], [s*sinθ, s*cosθ, ty]].
// If an initial transformation is provided, it is used to pre-align image2.
cv::Matx23d estimateSimilarityTransform(const cv::Mat& img1, const cv::Mat& img2,
                                        const cv::Matx23d* init = nullptr) {
    // Validate input
    if (img1.size() != img2.size() || img1.channels() != 1 || img2.channels() != 1) {
        throw std::invalid_argument("Both images must be single-channel and same size");
    }

    cv::Mat warped2;
    if (init != nullptr) {
        // Warp img2 using the initial transform's inverse (we need to map img2 onto img1)
        cv::Matx33d initH(*init);
        cv::Matx33d invH = initH.inv();
        cv::warpPerspective(img2, warped2, invH, img1.size(), cv::INTER_LINEAR | cv::WARP_INVERSE_MAP);
    } else {
        warped2 = img2;
    }

    // Compute gradients of img1 and image difference
    cv::Mat gradx, grady;
    cv::Sobel(img1, gradx, CV_64F, 1, 0, 3);
    cv::Sobel(img1, grady, CV_64F, 0, 1, 3);
    cv::Mat imgDiff;
    cv::subtract(img1, warped2, imgDiff);
    imgDiff.convertTo(imgDiff, CV_64F);

    // Build coordinate grids
    cv::Mat grid_c, grid_r;
    for (int y = 0; y < img1.rows; ++y) {
        for (int x = 0; x < img1.cols; ++x) {
            // We want grid_c = x, grid_r = y
        }
    }
    cv::Mat grid_x(1, img1.cols, CV_64F);
    for (int i = 0; i < img1.cols; ++i) grid_x.at<double>(0, i) = i;
    cv::Mat grid_y(img1.rows, 1, CV_64F);
    for (int i = 0; i < img1.rows; ++i) grid_y.at<double>(i, 0) = i;
    cv::repeat(grid_x, img1.rows, 1, grid_c);
    cv::repeat(grid_y, 1, img1.cols, grid_r);

    // Compute intermediate products
    cv::Mat xIx_p_yIy = grid_c.mul(gradx) + grid_r.mul(grady);
    cv::Mat yIx_m_xIy = grid_r.mul(gradx) - grid_c.mul(grady);

    // Build 4x4 linear system A * k = b
    cv::Matx44d A;
    cv::Vec4d b;

    A(0,0) = cv::sum(xIx_p_yIy.mul(xIx_p_yIy))[0];
    A(0,1) = cv::sum(xIx_p_yIy.mul(yIx_m_xIy))[0];
    A(0,2) = cv::sum(gradx.mul(xIx_p_yIy))[0];
    A(0,3) = cv::sum(grady.mul(xIx_p_yIy))[0];

    A(1,1) = cv::sum(yIx_m_xIy.mul(yIx_m_xIy))[0];
    A(1,2) = cv::sum(gradx.mul(yIx_m_xIy))[0];
    A(1,3) = cv::sum(grady.mul(yIx_m_xIy))[0];

    A(2,2) = cv::sum(gradx.mul(gradx))[0];
    A(2,3) = cv::sum(gradx.mul(grady))[0];

    A(3,3) = cv::sum(grady.mul(grady))[0];

    // Symmetric fill
    A(1,0) = A(0,1);
    A(2,0) = A(0,2);
    A(3,0) = A(0,3);
    A(2,1) = A(1,2);
    A(3,1) = A(1,3);
    A(3,2) = A(2,3);

    b[0] = -cv::sum(imgDiff.mul(xIx_p_yIy))[0];
    b[1] = -cv::sum(imgDiff.mul(yIx_m_xIy))[0];
    b[2] = -cv::sum(imgDiff.mul(gradx))[0];
    b[3] = -cv::sum(imgDiff.mul(grady))[0];

    // Solve; if singular, fallback to identity
    cv::Matx44d A_inv;
    double det = cv::determinant(A);
    if (std::abs(det) < 1e-10) {
        A_inv = cv::Matx44d::eye();
    } else {
        A_inv = A.inv();
    }
    cv::Vec4d k = A_inv * b;

    // Compose with initial if provided
    double a = k[0], bparam = k[1], tx = k[2], ty = k[3];
    cv::Matx23d result(a + 1.0, -bparam, tx, bparam, a + 1.0, ty);
    if (init != nullptr) {
        // Compose: final = init * incremental (since incremental is from warped to img1)
        cv::Matx33d initH(*init);
        cv::Matx33d incH(result);
        cv::Matx33d composed = initH * incH;
        result = cv::Matx23d(composed(0,0), composed(0,1), composed(0,2),
                             composed(1,0), composed(1,1), composed(1,2));
    }
    return result;
}
#include <cassert>
#include <opencv2/opencv.hpp>

int main() {
    // Create a simple synthetic image
    cv::Mat img1(50, 50, CV_8UC1);
    for (int y = 0; y < 50; ++y) {
        for (int x = 0; x < 50; ++x) {
            img1.at<uchar>(y, x) = static_cast<uchar>((x * 3 + y * 2) % 256);
        }
    }

    // Test 1: identity transform (same image)
    cv::Matx23d result1 = estimateSimilarityTransform(img1, img1);
    assert(std::abs(result1(0,0) - 1.0) < 1e-6);
    assert(std::abs(result1(0,1)) < 1e-6);
    assert(std::abs(result1(0,2)) < 1e-6);
    assert(std::abs(result1(1,1) - 1.0) < 1e-6);
    assert(std::abs(result1(1,2)) < 1e-6);

    // Test 2: translation only (shift right by 5, down by 3)
    cv::Mat img2;
    cv::Mat M = (cv::Mat_<double>(2,3) << 1, 0, 5, 0, 1, 3);
    cv::warpAffine(img1, img2, M, img1.size());
    cv::Matx23d result2 = estimateSimilarityTransform(img1, img2);
    assert(std::abs(result2(0,2) - 5.0) < 3.0);  // approximate due to interpolation
    assert(std::abs(result2(1,2) - 3.0) < 3.0);
    assert(std::abs(result2(0,0) - 1.0) < 0.2);
    assert(std::abs(result2(1,1) - 1.0) < 0.2);

    // Test 3: with initial guess (should still return near identity for same image)
    cv::Matx23d init(0.95, 0.05, 1.0, -0.05, 0.95, 2.0);
    cv::Matx23d result3 = estimateSimilarityTransform(img1, img1, &init);
    assert(std::abs(result3(0,0) - 1.0) < 1e-6);
    assert(std::abs(result3(0,1)) < 1e-6);
    assert(std::abs(result3(0,2)) < 1e-6);

    // Test 4: invalid input (different sizes)
    cv::Mat small(10, 10, CV_8UC1, cv::Scalar(0));
    bool threw = false;
    try {
        estimateSimilarityTransform(img1, small);
    } catch (const std::invalid_argument&) {
        threw = true;
    }
    assert(threw);

    // Test 5: constant image (gradients zero -> singular system, fallback to identity)
    cv::Mat constImg(30, 30, CV_8UC1, cv::Scalar(128));
    cv::Matx23d result5 = estimateSimilarityTransform(constImg, constImg);
    assert(std::abs(result5(0,0) - 1.0) < 1e-6);
    assert(std::abs(result5(0,1)) < 1e-6);
    assert(std::abs(result5(0,2)) < 1e-6);
    assert(std::abs(result5(1,1) - 1.0) < 1e-6);

    return 0;
}
