/*
Write a C++ function `performWienerDeblur(const cv::Mat& blurredImage, int motionLength, double motionAngleDegrees, double signalToNoiseRatio)` that takes a grayscale image (8-bit unsigned) simulated with uniform linear motion blur and returns an 8-bit deblurred image (also grayscale), using a Wiener filter constructed from a point spread function (PSF) derived from the blur parameters. The PSF should be a thin ellipse of total length `motionLength` (in pixels) oriented at `motionAngleDegrees` (0 meaning horizontal), centered in a filter size matching the image dimensions (rounded down to even width and height). Apply edge tapering to the input before filtering to suppress boundary artifacts, perform the deconvolution in the frequency domain using `cv::dft` and `cv::mulSpectrums`, and normalize the output to the full 8-bit range. The function must not use any external files, and it must handle cases where the PSF length is zero or the image is not loaded by returning an empty matrix (i.e., an `cv::Mat` whose `empty()` returns true). The output should be stored as an 8-bit `CV_8U` matrix.
*/

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <cmath>

// Compute the PSF as a thin ellipse with given length and angle (degrees).
static void calcPSF(cv::Mat& outputImg, cv::Size filterSize, int len, double theta) {
    cv::Mat h(filterSize, CV_32F, cv::Scalar(0));
    cv::Point point(filterSize.width / 2, filterSize.height / 2);
    // OpenCV angle is measured clockwise; we need 90 - theta to match standard orientation.
    cv::ellipse(h, point, cv::Size(0, cvRound(float(len) / 2.0)), 90.0 - theta, 0, 360, cv::Scalar(255), cv::FILLED);
    cv::Scalar summa = cv::sum(h);
    outputImg = h / summa[0];
}

// Shift the zero-frequency component to the center of the spectrum.
static void fftshift(const cv::Mat& inputImg, cv::Mat& outputImg) {
    outputImg = inputImg.clone();
    int cx = outputImg.cols / 2;
    int cy = outputImg.rows / 2;
    cv::Mat q0(outputImg, cv::Rect(0, 0, cx, cy));
    cv::Mat q1(outputImg, cv::Rect(cx, 0, cx, cy));
    cv::Mat q2(outputImg, cv::Rect(0, cy, cx, cy));
    cv::Mat q3(outputImg, cv::Rect(cx, cy, cx, cy));
    cv::Mat tmp;
    q0.copyTo(tmp);
    q3.copyTo(q0);
    tmp.copyTo(q3);
    q1.copyTo(tmp);
    q2.copyTo(q1);
    tmp.copyTo(q2);
}

// Compute the Wiener filter from the PSF.
static void calcWnrFilter(const cv::Mat& input_h_PSF, cv::Mat& output_G, double nsr) {
    cv::Mat h_PSF_shifted;
    fftshift(input_h_PSF, h_PSF_shifted);
    cv::Mat planes[2] = { cv::Mat_<float>(h_PSF_shifted.clone()), cv::Mat::zeros(h_PSF_shifted.size(), CV_32F) };
    cv::Mat complexI;
    cv::merge(planes, 2, complexI);
    cv::dft(complexI, complexI);
    cv::split(complexI, planes);
    cv::Mat denom;
    cv::pow(cv::abs(planes[0]), 2, denom);
    denom += nsr;
    cv::divide(planes[0], denom, output_G);
}

// Apply edge tapering to reduce boundary artifacts.
static void edgetaper(const cv::Mat& inputImg, cv::Mat& outputImg, double gamma = 5.0, double beta = 0.2) {
    int Nx = inputImg.cols;
    int Ny = inputImg.rows;
    cv::Mat w1(1, Nx, CV_32F, cv::Scalar(0));
    cv::Mat w2(Ny, 1, CV_32F, cv::Scalar(0));

    float* p1 = w1.ptr<float>(0);
    float* p2 = w2.ptr<float>(0);
    float dx = float(2.0 * CV_PI / Nx);
    float x = float(-CV_PI);
    for (int i = 0; i < Nx; i++) {
        p1[i] = float(0.5 * (std::tanh((x + gamma / 2) / beta) - std::tanh((x - gamma / 2) / beta)));
        x += dx;
    }
    float dy = float(2.0 * CV_PI / Ny);
    float y = float(-CV_PI);
    for (int i = 0; i < Ny; i++) {
        p2[i] = float(0.5 * (std::tanh((y + gamma / 2) / beta) - std::tanh((y - gamma / 2) / beta)));
        y += dy;
    }
    cv::Mat w = w2 * w1;
    cv::multiply(inputImg, w, outputImg);
}

// Perform 2D frequency-domain filtering with the given transfer function H.
static void filter2DFreq(const cv::Mat& inputImg, cv::Mat& outputImg, const cv::Mat& H) {
    cv::Mat planes[2] = { cv::Mat_<float>(inputImg.clone()), cv::Mat::zeros(inputImg.size(), CV_32F) };
    cv::Mat complexI;
    cv::merge(planes, 2, complexI);
    cv::dft(complexI, complexI, cv::DFT_SCALE);

    cv::Mat planesH[2] = { cv::Mat_<float>(H.clone()), cv::Mat::zeros(H.size(), CV_32F) };
    cv::Mat complexH;
    cv::merge(planesH, 2, complexH);
    cv::Mat complexIH;
    cv::mulSpectrums(complexI, complexH, complexIH, 0);

    cv::idft(complexIH, complexIH);
    cv::split(complexIH, planes);
    outputImg = planes[0];
}

// Main solution function: deblur an 8-bit grayscale image using a Wiener filter.
cv::Mat performWienerDeblur(const cv::Mat& blurredImage, int motionLength, double motionAngleDegrees, double signalToNoiseRatio) {
    // Validate inputs
    if (blurredImage.empty() || motionLength <= 0 || signalToNoiseRatio <= 0) {
        return cv::Mat(); // Return empty matrix on invalid input
    }

    // Work with even dimensions only
    cv::Rect roi(0, 0, blurredImage.cols & -2, blurredImage.rows & -2);
    if (roi.width == 0 || roi.height == 0) {
        return cv::Mat();
    }

    // Compute PSF and Wiener filter
    cv::Mat h, Hw;
    calcPSF(h, roi.size(), motionLength, motionAngleDegrees);
    calcWnrFilter(h, Hw, 1.0 / signalToNoiseRatio);

    // Convert to float, apply edge tapering
    cv::Mat imgInFloat;
    blurredImage(roi).convertTo(imgInFloat, CV_32F);
    cv::Mat imgTapered;
    edgetaper(imgInFloat, imgTapered);

    // Filter in frequency domain
    cv::Mat imgOut;
    filter2DFreq(imgTapered, imgOut, Hw);

    // Normalize and convert back to 8-bit
    cv::normalize(imgOut, imgOut, 0, 255, cv::NORM_MINMAX);
    cv::Mat result;
    imgOut.convertTo(result, CV_8U);
    return result;
}

#include <cassert>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>

// The solution function is declared here (assumed to be in the same translation unit or included).
// For brevity, we include the declaration:
cv::Mat performWienerDeblur(const cv::Mat& blurredImage, int motionLength, double motionAngleDegrees, double signalToNoiseRatio);

int main() {
    // Create a simple synthetic test image: 100x100 checkerboard pattern
    cv::Mat testImg(100, 100, CV_8U, cv::Scalar(0));
    for (int y = 0; y < 100; y++) {
        for (int x = 0; x < 100; x++) {
            testImg.at<uchar>(y, x) = ((x / 10 + y / 10) % 2) ? 255 : 0;
        }
    }

    // Test 1: Valid input should produce a non-empty result of same size
    cv::Mat result = performWienerDeblur(testImg, 5, 0.0, 100.0);
    assert(!result.empty());
    assert(result.rows == 100 && result.cols == 100);
    assert(result.type() == CV_8U);

    // Test 2: Zero motion length should return empty matrix
    cv::Mat result2 = performWienerDeblur(testImg, 0, 0.0, 100.0);
    assert(result2.empty());

    // Test 3: Non-positive SNR should return empty matrix
    cv::Mat result3 = performWienerDeblur(testImg, 5, 0.0, 0.0);
    assert(result3.empty());

    // Test 4: Empty input should return empty matrix
    cv::Mat emptyImg;
    cv::Mat result4 = performWienerDeblur(emptyImg, 5, 0.0, 100.0);
    assert(result4.empty());

    // Test 5: Odd-sized input is cropped to even dimensions (e.g., 101x99 -> 100x98)
    cv::Mat oddImg(101, 99, CV_8U, cv::Scalar(128));
    cv::Mat result5 = performWienerDeblur(oddImg, 3, 45.0, 50.0);
    assert(!result5.empty());
    assert(result5.rows == 100 && result5.cols == 98);

    // Test 6: Result values must stay within 0-255 (implicitly tested by CV_8U)
    // Test that non-degenerate output has some variation (not all zeros or all 255)
    cv::Mat result6 = performWienerDeblur(testImg, 10, 30.0, 200.0);
    assert(!result6.empty());
    cv::Scalar meanVal = cv::mean(result6);
    assert(meanVal[0] > 0.0 && meanVal[0] < 255.0);

    // Test 7: Different SNR values produce different outputs (sanity check)
    cv::Mat lowSnr = performWienerDeblur(testImg, 5, 0.0, 10.0);
    cv::Mat highSnr = performWienerDeblur(testImg, 5, 0.0, 1000.0);
    assert(!lowSnr.empty() && !highSnr.empty());
    double diff = cv::norm(lowSnr, highSnr, cv::NORM_L2);
    assert(diff > 0.0);

    // Test 8: Different angles produce different outputs (sanity check)
    cv::Mat angle0 = performWienerDeblur(testImg, 8, 0.0, 100.0);
    cv::Mat angle45 = performWienerDeblur(testImg, 8, 45.0, 100.0);
    assert(!angle0.empty() && !angle45.empty());
    double diffAngle = cv::norm(angle0, angle45, cv::NORM_L2);
    assert(diffAngle > 0.0);

    // Test 9: Very large SNR approaches inverse filtering but still stable
    cv::Mat bigSnr = performWienerDeblur(testImg, 4, 90.0, 1e9);
    assert(!bigSnr.empty());
    assert(cv::mean(bigSnr)[0] > 0.0 && cv::mean(bigSnr)[0] < 255.0);

    // Test 10: Minimal image (2x2) works without error
    cv::Mat tiny(2, 2, CV_8U, cv::Scalar(0));
    tiny.at<uchar>(0,0) = 100;
    tiny.at<uchar>(0,1) = 200;
    tiny.at<uchar>(1,0) = 50;
    tiny.at<uchar>(1,1) = 150;
    cv::Mat tinyResult = performWienerDeblur(tiny, 2, 0.0, 100.0);
    assert(!tinyResult.empty());
    assert(tinyResult.rows == 2 && tinyResult.cols == 2);

    return 0;
}

// The solution reconstructs the original image from a linear motion blur by performing Wiener deconvolution in the Fourier domain. First, the PSF is computed as a binary ellipse with major axis length equal to `motionLength`, rotated by `motionAngleDegrees` (converted to the OpenCV convention where angle is measured clockwise from the positive x-axis), and then normalized so its values sum to 1. The PSF is zero-padded to the even-dimensioned filter size and then shifted to have its origin at the center of the frequency domain (using a quadrant swap, i.e., `fftshift` analog). The Wiener filter is constructed as \( H^* / (|H|^2 + nsr) \), where \( H \) is the 2D DFT of the shifted PSF and \( nsr = 1/SNR \). The blurred image is converted to `CV_32F` and tapered using a cosine-tanh window function to reduce edge discontinuities. The tapering window is formed as the outer product of two 1D windows along the x and y axes, each computed as \( 0.5(\tanh((x+\gamma/2)/\beta) - \tanh((x-\gamma/2)/\beta)) \) with default \( \gamma=5, \beta=0.2 \). The tapered image is then transformed with `DFT_SCALE`, multiplied element-wise with the Wiener filter using `cv::mulSpectrums`, and inverse transformed to obtain the deblurred result. Finally, the floating-point output is normalized to the [0, 255] range (using `cv::normalize` with `NORM_MINMAX`) and converted to `CV_8U`. Important edge cases include ensuring the image dimensions are even (by cropping), handling a zero-length motion (return empty matrix), and avoiding division by zero by adding the noise-to-signal ratio term. The time complexity is \( O(W H \log(W H)) \) due to the two FFTs (forward and inverse), and the space complexity is \( O(W H) \) for the intermediate matrices.
