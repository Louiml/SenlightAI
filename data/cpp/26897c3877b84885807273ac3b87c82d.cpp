Write a C++ function named `averageGradientClone` that takes a background image, a foreground image, a grayscale mask, and integer offsets `offsetX` and `offsetY`, and returns a new image where the foreground region defined by the mask is smoothly blended into the background at the given offset using an averaging gradient-based Poisson cloning method. The function should implement the core idea of the `CLONE_AVERAGED_GRADIENTS` technique: compute the gradient fields of both background and foreground over the overlapping region, average the gradients, and then reconstruct the result via a simplified Poisson solver (e.g., using a Jacobi or Gauss-Seidel iteration) so that the final image has no visible seams. The output image must have the same size and type as the background. You may assume the foreground and mask are already cropped to reasonable sizes, and the offset may be negative, meaning the foreground can extend beyond the background bounds; only the valid overlap region should be processed. The function must be self-contained with no external dependencies beyond standard C++ and OpenCV headers (if allowed) or a simple custom image representation. For this task, use OpenCV's `cv::Mat` for image handling.

#include <cassert>
#include <opencv2/opencv.hpp>
// Declaration of the function from solution
cv::Mat averageGradientClone(const cv::Mat&, const cv::Mat&, const cv::Mat&, int, int);

int main() {
    // Create a simple test: background is white, foreground is black square, mask is white square
    cv::Mat bg(20, 20, CV_8UC3, cv::Scalar(255, 255, 255));
    cv::Mat fg(10, 10, CV_8UC3, cv::Scalar(0, 0, 0));
    cv::Mat mask(10, 10, CV_8UC1, cv::Scalar(255));
    
    // Place foreground at (5,5) within background
    cv::Mat result = averageGradientClone(bg, fg, mask, 5, 5);
    assert(result.size() == bg.size());
    assert(result.type() == bg.type());
    // At center of foreground, should be significantly darker than background
    cv::Vec3b center = result.at<cv::Vec3b>(10, 10);
    assert(center[0] < 200 && center[1] < 200 && center[2] < 200);
    // At far background corner, should remain white
    cv::Vec3b corner = result.at<cv::Vec3b>(0, 0);
    assert(corner[0] == 255 && corner[1] == 255 && corner[2] == 255);
    
    // Test with negative offset (foreground partially outside background)
    cv::Mat fg2(8, 8, CV_8UC3, cv::Scalar(100, 100, 100));
    cv::Mat mask2(8, 8, CV_8UC1, cv::Scalar(255));
    cv::Mat result2 = averageGradientClone(bg, fg2, mask2, -3, -2);
    assert(result2.size() == bg.size());
    // Only pixels within background should be changed
    assert(result2.at<cv::Vec3b>(0, 0)[0] < 255); // top-left area is covered by foreground
    
    // Test when no overlap (offset far away)
    cv::Mat result3 = averageGradientClone(bg, fg, mask, 50, 50);
    assert(result3 == bg);
    
    // Test grayscale image
    cv::Mat bgGray(10, 10, CV_8UC1, cv::Scalar(200));
    cv::Mat fgGray(5, 5, CV_8UC1, cv::Scalar(50));
    cv::Mat maskGray(5, 5, CV_8UC1, cv::Scalar(255));
    cv::Mat result4 = averageGradientClone(bgGray, fgGray, maskGray, 2, 2);
    assert(result4.type() == CV_8UC1);
    assert(result4.at<uchar>(2, 2) < 200); // changed pixel
    
    // Test zero mask (foreground fully transparent)
    cv::Mat maskZero(5, 5, CV_8UC1, cv::Scalar(0));
    cv::Mat result5 = averageGradientClone(bg, fg, maskZero, 5, 5);
    assert(result5 == bg);
    
    return 0;
}

#include <opencv2/opencv.hpp>
#include <vector>

// Average-gradient Poisson clone: blend foreground into background smoothly.
// Parameters:
//   background - base image (CV_8UC3 or CV_8UC1)
//   foreground - foreground image (same type as background)
//   mask - binary single-channel mask (nonzero = foreground)
//   offsetX, offsetY - placement of foreground top-left on background
// Returns: result image same size as background
cv::Mat averageGradientClone(const cv::Mat& background,
                             const cv::Mat& foreground,
                             const cv::Mat& mask,
                             int offsetX, int offsetY) {
    // Prepare result as copy of background
    cv::Mat result = background.clone();
    
    // Determine overlap rectangle in background coordinates
    int bgW = background.cols, bgH = background.rows;
    int fgW = foreground.cols, fgH = foreground.rows;
    
    // Overlap in background coordinates
    int bgLeft = std::max(0, offsetX);
    int bgTop  = std::max(0, offsetY);
    int bgRight = std::min(bgW, offsetX + fgW);
    int bgBottom = std::min(bgH, offsetY + fgH);
    
    if (bgRight <= bgLeft || bgBottom <= bgTop) {
        // No overlap, just return background copy
        return result;
    }
    
    int overlapW = bgRight - bgLeft;
    int overlapH = bgBottom - bgTop;
    
    // Corresponding foreground rectangle
    int fgLeft = bgLeft - offsetX;
    int fgTop  = bgTop  - offsetY;
    
    // Extract overlap regions
    cv::Mat bgOverlap = background(cv::Rect(bgLeft, bgTop, overlapW, overlapH));
    cv::Mat fgOverlap = foreground(cv::Rect(fgLeft, fgTop, overlapW, overlapH));
    cv::Mat maskOverlap = mask(cv::Rect(fgLeft, fgTop, overlapW, overlapH));
    
    // Number of channels
    int channels = background.channels();
    
    // Compute averaged gradient fields (x and y) for each channel
    std::vector<cv::Mat> gradX(channels), gradY(channels);
    for (int c = 0; c < channels; ++c) {
        gradX[c] = cv::Mat::zeros(overlapH, overlapW, CV_32F);
        gradY[c] = cv::Mat::zeros(overlapH, overlapW, CV_32F);
    }
    
    // Temporary float images for calculations
    cv::Mat bgFloat, fgFloat;
    if (channels == 3) {
        cv::cvtColor(bgOverlap, bgFloat, cv::COLOR_BGR2GRAY);
        cv::cvtColor(fgOverlap, fgFloat, cv::COLOR_BGR2GRAY);
    } else {
        bgOverlap.convertTo(bgFloat, CV_32F);
        fgOverlap.convertTo(fgFloat, CV_32F);
    }
    
    // For simplicity, process single-channel gradient (for grayscale) or each channel separately
    // Here we implement multi-channel by splitting
    std::vector<cv::Mat> bgChannels, fgChannels;
    cv::split(bgOverlap, bgChannels);
    cv::split(fgOverlap, fgChannels);
    
    for (int c = 0; c < channels; ++c) {
        cv::Mat bgChan = bgChannels[c];
        cv::Mat fgChan = fgChannels[c];
        for (int y = 0; y < overlapH; ++y) {
            for (int x = 0; x < overlapW; ++x) {
                // Skip if mask is zero
                if (maskOverlap.at<uchar>(y, x) == 0) continue;
                // Gradient in x: forward difference for last column, central otherwise
                float bgGx, bgGy, fgGx, fgGy;
                if (x == overlapW - 1) {
                    bgGx = bgChan.at<uchar>(y, x) - bgChan.at<uchar>(y, x - 1);
                    fgGx = fgChan.at<uchar>(y, x) - fgChan.at<uchar>(y, x - 1);
                } else {
                    bgGx = bgChan.at<uchar>(y, x + 1) - bgChan.at<uchar>(y, x);
                    fgGx = fgChan.at<uchar>(y, x + 1) - fgChan.at<uchar>(y, x);
                }
                if (y == overlapH - 1) {
                    bgGy = bgChan.at<uchar>(y, x) - bgChan.at<uchar>(y - 1, x);
                    fgGy = fgChan.at<uchar>(y, x) - fgChan.at<uchar>(y - 1, x);
                } else {
                    bgGy = bgChan.at<uchar>(y + 1, x) - bgChan.at<uchar>(y, x);
                    fgGy = fgChan.at<uchar>(y + 1, x) - fgChan.at<uchar>(y, x);
                }
                gradX[c].at<float>(y, x) = (bgGx + fgGx) / 2.0f;
                gradY[c].at<float>(y, x) = (bgGy + fgGy) / 2.0f;
            }
        }
    }
    
    // Compute divergence of gradient field (Laplacian of desired result)
    std::vector<cv::Mat> divergence(channels);
    for (int c = 0; c < channels; ++c) {
        divergence[c] = cv::Mat::zeros(overlapH, overlapW, CV_32F);
        for (int y = 0; y < overlapH; ++y) {
            for (int x = 0; x < overlapW; ++x) {
                if (maskOverlap.at<uchar>(y, x) == 0) continue;
                float div = 0.0f;
                if (x > 0) div -= gradX[c].at<float>(y, x - 1);
                if (x < overlapW - 1) div += gradX[c].at<float>(y, x);
                if (y > 0) div -= gradY[c].at<float>(y - 1, x);
                if (y < overlapH - 1) div += gradY[c].at<float>(y, x);
                divergence[c].at<float>(y, x) = div;
            }
        }
    }
    
    // Iterative Poisson reconstruction using Jacobi method
    // Use floating-point working copy, then clamp and convert back
    std::vector<cv::Mat> work(channels);
    std::vector<cv::Mat> resultChannels;
    cv::split(result, resultChannels);
    for (int c = 0; c < channels; ++c) {
        // Initialize work from background overlap region
        work[c] = cv::Mat::zeros(overlapH, overlapW, CV_32F);
        for (int y = 0; y < overlapH; ++y)
            for (int x = 0; x < overlapW; ++x)
                work[c].at<float>(y, x) = static_cast<float>(resultChannels[c].at<uchar>(bgTop + y, bgLeft + x));
    }
    
    const int iterations = 500; // fixed iteration count for simplicity
    for (int iter = 0; iter < iterations; ++iter) {
        for (int c = 0; c < channels; ++c) {
            cv::Mat next = work[c].clone();
            for (int y = 0; y < overlapH; ++y) {
                for (int x = 0; x < overlapW; ++x) {
                    if (maskOverlap.at<uchar>(y, x) == 0) continue;
                    float sum = 0.0f;
                    int count = 0;
                    if (x > 0) { sum += work[c].at<float>(y, x - 1); ++count; }
                    if (x < overlapW - 1) { sum += work[c].at<float>(y, x + 1); ++count; }
                    if (y > 0) { sum += work[c].at<float>(y - 1, x); ++count; }
                    if (y < overlapH - 1) { sum += work[c].at<float>(y + 1, x); ++count; }
                    if (count > 0) {
                        next.at<float>(y, x) = (sum - divergence[c].at<float>(y, x)) / count;
                    }
                }
            }
            work[c] = next;
        }
    }
    
    // Copy reconstructed result back to destination
    for (int c = 0; c < channels; ++c) {
        for (int y = 0; y < overlapH; ++y) {
            for (int x = 0; x < overlapW; ++x) {
                if (maskOverlap.at<uchar>(y, x) == 0) continue;
                float val = work[c].at<float>(y, x);
                // Clamp to [0,255]
                val = std::max(0.0f, std::min(255.0f, val));
                resultChannels[c].at<uchar>(bgTop + y, bgLeft + x) = static_cast<uchar>(val);
            }
        }
    }
    cv::merge(resultChannels, result);
    
    return result;
}

// The solution involves three main stages:
// 1. **Overlap computation**: Determine the rectangular region where the foreground (placed at offset) intersects the background. This requires mapping background coordinates to foreground coordinates by subtracting the offset. Handle negative offsets by clamping the overlap rectangle to the background bounds and correspondingly adjusting the foreground region.
// 2. **Gradient computation and averaging**: For each pixel in the overlap region, compute gradients in the x and y directions using finite differences (e.g., central differences for interior pixels, forward/backward differences at boundaries). Compute the foreground gradient and background gradient over the overlap, then average them elementwise for each color channel.
// 3. **Reconstruction via iterative solver**: Start with the background image as an initial guess (copy the entire background to the destination). Then, for the overlap region, iteratively update each pixel to satisfy the equation `(4 * f(x,y) - f(x+1,y) - f(x-1,y) - f(x,y+1) - f(x,y-1)) = divGrad(x,y)`, where `divGrad` is the divergence of the averaged gradient field. Use a fixed number of Jacobi iterations (e.g., 1000) or until convergence (change below a threshold). After reconstruction, copy only the reconstructed pixels back to the destination inside the overlap region, leaving the rest of the background unchanged. Edge cases: (a) if no overlap exists, return a copy of the background; (b) handle multi-channel images by processing each channel independently; (c) mask values may be non-binary, but for simplicity, assume the mask is binary (nonzero means foreground). Complexity: Let `W` and `H` be the overlap width and height, and `C` the number of channels. Gradient computation is `O(W*H*C)`. Each Jacobi iteration is `O(W*H*C)`. With `I` iterations, total time is `O(I*W*H*C)`. Space is `O(W*H*C)` for the working copy.
