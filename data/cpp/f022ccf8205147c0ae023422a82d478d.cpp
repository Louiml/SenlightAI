// Given a `cv::Mat` image (either 8-bit grayscale or 8-bit BGR color) along with a binary ROI mask (all pixels either 0 or 255) of the same size, write a C++ function that computes a pixel-wise "local contrast" score and returns it as a `CV_32FC1` matrix. For each pixel within the ROI that is far enough from the image border (at least 2 pixels away), compute the average absolute difference between that pixel's intensity (for grayscale) or the mean of its three channel values (for color) and the corresponding mean value of each of its 8-connected neighbors. Pixels at the border (within 1 pixel of the edge) or outside the ROI should have a score of 0. The function must handle empty images, assert that the ROI matches the image size and type, and work with continuous images only. The output matrix must have the same dimensions as the input and be initialized to zeros.
// The solution iterates over every pixel in the image. For each pixel that is both inside the ROI (ROI value not equal to 0) and at least 2 pixels away from all borders (i.e., `x >= 2 && x < cols-2 && y >= 2 && y < rows-2` for the 8-neighborhood without including the pixel itself; actually to have all 8 neighbors we need `x >= 1 && x < cols-1 && y >= 1 && y < rows-1`, but the problem says "at least 2 pixels away" – that would mean exclude the outer two rows/columns, but that seems overly restrictive; the natural interpretation is that we need to have all 8 neighbors, so `x >= 1 && x < cols-1 && y >= 1 && y < rows-1`. However, the spec says "at least 2 pixels away" – to be safe we'll follow the literal spec: require `x >= 2 && x < cols-2 && y >= 2 && y < rows-2`, meaning we only consider pixels where all 8 neighbors exist and additionally the pixel is not in the outer two rows/columns. That is a conservative interpretation; in practice, this just reduces the valid region. I'll follow the spec exactly. For each valid pixel, compute the local mean intensity: for grayscale, simply the pixel value; for color, average of B,G,R. Then compute the mean intensity of each of the 8 neighbors (using the same intensity definition) and sum the absolute differences, divide by 8. Store this value in the output at that pixel. For all other pixels (border, outside ROI) leave as zero. Time complexity is O(rows*cols*8) = O(N) for N pixels, space O(N) for output. Edge cases: empty image – assert it's not empty; ROI must be same size and type CV_8UC1; ROI values must be binary (0 or 255) – we can assert that the count of values not equal to 0 or 255 is zero, similar to the original snippet. The image must be continuous. The output is CV_32FC1.
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <cassert>
#include <cmath>

/**
 * Computes a local contrast score for each pixel inside the ROI.
 * For each pixel at least 2 pixels away from borders and with ROI==255,
 * computes average absolute difference between its intensity and the mean
 * intensity of its 8 neighbors. Others get 0.
 *
 * @param image   Input continuous CV_8UC1 or CV_8UC3 image.
 * @param roi     Binary ROI mask (CV_8UC1), same size as image, values 0 or 255.
 * @return        CV_32FC1 matrix of same size as image with contrast scores.
 */
cv::Mat computeLocalContrast(const cv::Mat& image, const cv::Mat& roi) {
    CV_Assert(!image.empty());
    CV_Assert(image.isContinuous());
    CV_Assert(image.type() == CV_8UC1 || image.type() == CV_8UC3);
    CV_Assert(roi.size() == image.size());
    CV_Assert(roi.type() == CV_8UC1);
    CV_Assert(roi.isContinuous());
    
    // Validate ROI is binary (all values 0 or 255)
    cv::Mat nonBin;
    cv::bitwise_and(cv::Mat((roi != 0) | (roi != 255)), cv::Mat::ones(roi.size(), CV_8UC1), nonBin);
    CV_Assert(cv::countNonZero(roi & ~((roi == 0) | (roi == 255))) == 0); // simpler: check all values are in {0,255}

    const int rows = image.rows;
    const int cols = image.cols;
    const int channels = image.channels();
    
    cv::Mat output = cv::Mat::zeros(rows, cols, CV_32FC1);
    
    const uchar* imgData = image.data;
    const uchar* roiData = roi.data;
    float* outData = output.ptr<float>(0);
    
    // Offsets for 8 neighbors (dx, dy) excluding center
    const int dx[8] = {-1, 0, 1, -1, 1, -1, 0, 1};
    const int dy[8] = {-1, -1, -1, 0, 0, 1, 1, 1};
    
    for (int y = 2; y < rows - 2; ++y) {
        for (int x = 2; x < cols - 2; ++x) {
            const size_t idx = y * cols + x; // since continuous, row stride = cols*channels
            if (roiData[idx] == 0) continue;
            
            float centerIntensity = 0.0f;
            if (channels == 1) {
                centerIntensity = static_cast<float>(imgData[idx]);
            } else {
                const uchar* p = imgData + idx * 3;
                centerIntensity = (static_cast<float>(p[0]) + static_cast<float>(p[1]) + static_cast<float>(p[2])) / 3.0f;
            }
            
            float sumDiff = 0.0f;
            for (int n = 0; n < 8; ++n) {
                const int nx = x + dx[n];
                const int ny = y + dy[n];
                const size_t nidx = ny * cols + nx;
                float neighborIntensity = 0.0f;
                if (channels == 1) {
                    neighborIntensity = static_cast<float>(imgData[nidx]);
                } else {
                    const uchar* p = imgData + nidx * 3;
                    neighborIntensity = (static_cast<float>(p[0]) + static_cast<float>(p[1]) + static_cast<float>(p[2])) / 3.0f;
                }
                sumDiff += std::fabs(centerIntensity - neighborIntensity);
            }
            
            outData[idx] = sumDiff / 8.0f;
        }
    }
    
    return output;
}
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <cassert>
#include <cmath>

// The function from the solution
cv::Mat computeLocalContrast(const cv::Mat& image, const cv::Mat& roi);

int main() {
    // Test 1: Grayscale constant image, ROI all 255, expect all zeros (contrast = 0)
    cv::Mat gray = cv::Mat::ones(10, 10, CV_8UC1) * 100;
    cv::Mat roiOnes = cv::Mat::ones(10, 10, CV_8UC1) * 255;
    cv::Mat contrast1 = computeLocalContrast(gray, roiOnes);
    CV_Assert(contrast1.type() == CV_32FC1);
    CV_Assert(contrast1.size() == gray.size());
    for (int y = 0; y < 10; ++y)
        for (int x = 0; x < 10; ++x)
            if (y >= 2 && y < 8 && x >= 2 && x < 8)
                assert(contrast1.at<float>(y, x) == 0.0f);
            else
                assert(contrast1.at<float>(y, x) == 0.0f); // border also zero

    // Test 2: Check that border pixels (within 1 of edge) are zero even if ROI=255
    cv::Mat img2 = cv::Mat::zeros(5, 5, CV_8UC3);
    img2.at<cv::Vec3b>(2,2) = cv::Vec3b(255, 255, 255); // white center
    cv::Mat roi2 = cv::Mat::ones(5, 5, CV_8UC1) * 255;
    cv::Mat contrast2 = computeLocalContrast(img2, roi2);
    // The center (2,2) is not within "at least 2 pixels away" because border is at 0..4, so valid range is 2..2? Actually rows-2=3, so y from 2 to 2 inclusive? Wait rows=5 so y from 2 to 5-2-1=2, so only y=2. Same for x=2. So only pixel (2,2) qualifies.
    // But (2,2) has neighbors all black (0) and center white (255). Compute average diff = 255 for each neighbor? Actually each neighbor's intensity =0, center=255, diff=255, sum=8*255=2040, /8=255.
    float val = contrast2.at<float>(2,2);
    assert(std::fabs(val - 255.0f) < 1e-5);
    // Border pixel (0,0) must be zero
    assert(contrast2.at<float>(0,0) == 0.0f);

    // Test 3: ROI restricts processing
    cv::Mat img3 = cv::Mat::zeros(6, 6, CV_8UC1);
    img3.at<uchar>(2,2) = 10;
    // ROI with all zeros except pixel (2,2) set to 255
    cv::Mat roi3 = cv::Mat::zeros(6, 6, CV_8UC1);
    roi3.at<uchar>(2,2) = 255;
    cv::Mat contrast3 = computeLocalContrast(img3, roi3);
    // Only pixel (2,2) is in ROI; it is not in border (y from 2 to 3? rows=6 so y valid from 2 to 3, x from 2 to 3). So (2,2) qualifies. Its neighbors all have intensity 0, center 10, avg diff=10.
    assert(std::fabs(contrast3.at<float>(2,2) - 10.0f) < 1e-5);
    // Pixel (2,3) is in ROI but not set to 255? Actually roi3 only (2,2) is 255, so (2,3) is 0, so contrast should be 0.
    assert(contrast3.at<float>(2,3) == 0.0f);

    // Test 4: Color image with known pattern
    cv::Mat color = cv::Mat::zeros(8, 8, CV_8UC3);
    // Make a 3x3 block in center with color (0,0,255) red all others black
    color.at<cv::Vec3b>(3,3) = cv::Vec3b(0,0,255); // but neighbors are black; center intensity = (0+0+255)/3 = 85. Neighbors intensity = 0. So avg diff = 85.
    cv::Mat roiColor = cv::Mat::ones(8,8,CV_8UC1)*255;
    cv::Mat contrastColor = computeLocalContrast(color, roiColor);
    // Valid range for 8x8: y from 2 to 5, x from 2 to 5. The center pixel (3,3) qualifies.
    float expected = 85.0f;
    assert(std::fabs(contrastColor.at<float>(3,3) - expected) < 1e-5);

    // Test 5: Assertion failure on mismatched ROI size
    bool threw = false;
    try {
        cv::Mat badRoi = cv::Mat::ones(9,9,CV_8UC1)*255;
        computeLocalContrast(color, badRoi);
    } catch (cv::Exception&) {
        threw = true;
    }
    assert(threw);

    return 0;
}
