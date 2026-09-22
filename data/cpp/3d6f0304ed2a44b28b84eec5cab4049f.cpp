Write a C++ function named `marrHildrethHashBits` that takes a single-channel 8-bit grayscale image represented as a `cv::Mat` (with `CV_8U` type), applies the Marr-Hildreth perceptual hash pipeline, and returns a `std::vector<uchar>` containing exactly 72 bits of hash data (i.e., 9 bytes). The function must internally replicate the following steps: Gaussian blur with a 7x7 kernel and zero standard deviation, resizing to 512x512 using cubic interpolation, histogram equalization, convolution with a Marr-Hildreth (Mexican hat) kernel generated using alpha=2.0 and scale=1.0 (kernel size is `2*sigma+1` where `sigma = int(4 * pow(alpha, scale))`), then dividing the filtered image into 16x16 blocks over a 31x31 grid (i.e., the filtered image is clipped to 31*16=496 pixels in height and width, discarding extra border pixels), summing each block to form a 31x31 float matrix, and finally iterating over non-overlapping 3x3 windows starting at row/column offsets 0, 4, 8, ..., 28 to compare each of the 9 values inside the window against the window's average, storing one hash bit per comparison (1 if greater, 0 otherwise) packed into bytes in big-endian order (first bit is the most significant bit of byte 0). The function should throw a `cv::Exception` or return an empty vector if the input is not `CV_8U`. Do not modify the input image. The function should be self-contained and not rely on any external hashing or OpenCV high-level `img_hash` module.

#include <cassert>
#include <opencv2/opencv.hpp>
#include <vector>

// Forward declaration
std::vector<uchar> marrHildrethHashBits(const cv::Mat& input);

int main() {
    // Create a small test image (e.g., a simple gradient)
    cv::Mat img(64, 64, CV_8U, cv::Scalar(128));
    for (int i = 0; i < 64; ++i) {
        for (int j = 0; j < 64; ++j) {
            img.at<uchar>(i, j) = static_cast<uchar>((i + j) % 256);
        }
    }

    // Test 1: valid input produces 9 bytes
    auto hash1 = marrHildrethHashBits(img);
    assert(hash1.size() == 9);

    // Test 2: same input produces same hash (deterministic)
    auto hash2 = marrHildrethHashBits(img);
    assert(hash1 == hash2);

    // Test 3: a constant image gives a specific pattern (all bits? Need to be careful)
    cv::Mat constImg(50, 50, CV_8U, cv::Scalar(100));
    auto hashConst = marrHildrethHashBits(constImg);
    // After Gaussian blur and equalization, a constant image becomes uniform 128,
    // filtering and block sums produce constant values, and all bits will be 0 because values are equal to avg.
    // But because of floating rounding, some might be slightly > avg? In practice, after sum, it's exactly equal.
    // We'll just check that all bytes are 0.
    bool allZero = true;
    for (uchar b : hashConst) {
        if (b != 0) { allZero = false; break; }
    }
    assert(allZero);

    // Test 4: different images should likely differ (not guaranteed but good sanity)
    cv::Mat img2(64, 64, CV_8U, cv::Scalar(255));
    for (int i = 0; i < 64; ++i) {
        for (int j = 0; j < 64; ++j) {
            img2.at<uchar>(i, j) = static_cast<uchar>((i * 7 + j * 3) % 256);
        }
    }
    auto hash3 = marrHildrethHashBits(img2);
    assert(hash3 != hash1); // extremely likely different

    // Test 5: invalid type throws exception
    cv::Mat colorImg(32, 32, CV_8UC3, cv::Scalar(0, 0, 0));
    bool threw = false;
    try {
        marrHildrethHashBits(colorImg);
    } catch (const std::invalid_argument&) {
        threw = true;
    }
    assert(threw);

    // Test 6: empty image throws
    cv::Mat emptyImg;
    threw = false;
    try {
        marrHildrethHashBits(emptyImg);
    } catch (const std::invalid_argument&) {
        threw = true;
    }
    assert(threw);

    // Test 7: hash length and bit packing (first byte should have bits in order)
    // Construct a synthetic blocks matrix? Not easy from here, but we can test that the
    // function works with a very small image (must be resized to 512)
    cv::Mat smallImg(10, 10, CV_8U, cv::Scalar(0));
    smallImg.at<uchar>(5,5) = 255;
    auto hashSmall = marrHildrethHashBits(smallImg);
    assert(hashSmall.size() == 9);

    // Test 8: verify that the filter2D step does not produce NaN (checks kernel validity)
    cv::Mat randImg(50, 50, CV_8U);
    cv::randu(randImg, 0, 256);
    auto hashRand = marrHildrethHashBits(randImg);
    assert(hashRand.size() == 9);

    // Test 9: Hamming distance between identical images should be 0 (simple check)
    auto hashA = marrHildrethHashBits(randImg);
    auto hashB = marrHildrethHashBits(randImg);
    int hamming = 0;
    for (size_t i = 0; i < hashA.size(); ++i) {
        uchar diff = hashA[i] ^ hashB[i];
        while (diff) { hamming += diff & 1; diff >>= 1; }
    }
    assert(hamming == 0);

    // Test 10: test with a different alpha/scale? Not required but we can ensure no crash
    // The function doesn't accept parameters, so we just ensure stable behavior.
    // All done.

    return 0;
}

#include <opencv2/opencv.hpp>
#include <vector>
#include <stdexcept>

// Compute a 72-bit Marr-Hildreth perceptual hash of a grayscale image.
// Input must be CV_8U (single channel). Returns 9 bytes (72 bits) packed in big-endian order.
std::vector<uchar> marrHildrethHashBits(const cv::Mat& input) {
    if (input.empty() || input.type() != CV_8U) {
        throw std::invalid_argument("Input must be a non-empty single-channel 8-bit image");
    }

    // Step 1: Gaussian blur with 7x7 kernel
    cv::Mat blurImg;
    cv::GaussianBlur(input, blurImg, cv::Size(7, 7), 0);

    // Step 2: Resize to 512x512
    cv::Mat resizeImg;
    cv::resize(blurImg, resizeImg, cv::Size(512, 512), 0, 0, cv::INTER_CUBIC);

    // Step 3: Histogram equalization
    cv::Mat equalizeImg;
    cv::equalizeHist(resizeImg, equalizeImg);

    // Step 4: Generate Marr-Hildreth kernel with alpha=2, scale=1
    const float alpha = 2.0f;
    const float scale = 1.0f;
    int sigma = static_cast<int>(4 * std::pow(alpha, scale)); // 8
    float ratio = std::pow(alpha, -scale); // 0.5
    cv::Mat kernel(2*sigma+1, 2*sigma+1, CV_32F);
    for (int row = 0; row < kernel.rows; ++row) {
        float ydiff = static_cast<float>(row - sigma);
        float ypos = ratio * ydiff;
        for (int col = 0; col < kernel.cols; ++col) {
            float xdiff = static_cast<float>(col - sigma);
            float xpos = ratio * xdiff;
            float r2 = xpos*xpos + ypos*ypos;
            kernel.at<float>(row, col) = (2.0f - r2) * std::exp(r2 / 2.0f);
        }
    }

    // Step 5: Convolve with kernel
    cv::Mat freImg;
    cv::filter2D(equalizeImg, freImg, CV_32F, kernel);

    // Step 6: Build 31x31 block sums for 16x16 non-overlapping regions
    const int blockSize = 16;
    const int gridSize = 31;
    cv::Mat blocks(gridSize, gridSize, CV_32F, cv::Scalar(0.0f));
    for (int r = 0; r < gridSize; ++r) {
        for (int c = 0; c < gridSize; ++c) {
            cv::Rect roi(c*blockSize, r*blockSize, blockSize, blockSize);
            blocks.at<float>(r, c) = static_cast<float>(cv::sum(freImg(roi))[0]);
        }
    }

    // Step 7: Extract 72 bits using 3x3 windows strided by 4
    std::vector<uchar> hash(9, 0);
    int bitIndex = 0;
    int byteIndex = 0;
    uchar currentByte = 0;
    for (int row = 0; row < 29; row += 4) {
        for (int col = 0; col < 29; col += 4) {
            cv::Rect win(col, row, 3, 3);
            cv::Mat blockROI = blocks(win);
            float avg = static_cast<float>(cv::sum(blockROI)[0] / 9.0);
            for (int i = 0; i < 3; ++i) {
                const float* bptr = blockROI.ptr<float>(i);
                for (int j = 0; j < 3; ++j) {
                    currentByte <<= 1;
                    if (bptr[j] > avg) {
                        currentByte |= 0x01;
                    }
                    ++bitIndex;
                    if (bitIndex % 8 == 0) {
                        hash[byteIndex++] = currentByte;
                        currentByte = 0;
                    }
                }
            }
        }
    }
    return hash;
}

// The solution follows the exact computational pipeline of the provided snippet. First, validate that the input matrix is single-channel, 8-bit unsigned; otherwise, throw a meaningful exception. Since OpenCV's `cv::Mat` can be multi-channel, we assume the user passes a gray image (e.g., after `cvtColor`). Next, create a smooth version using `cv::GaussianBlur` with a 7x7 kernel and `sigma=0` (OpenCV computes sigma automatically). Resize the blurred image to exactly 512x512 using `cv::INTER_CUBIC`. Apply `cv::equalizeHist` to the resized image. Generate the Marr-Hildreth kernel: compute `sigma = int(4 * pow(2.0, 1.0)) = int(8.0) = 8`, so the kernel size is 17x17; for each pixel at offset `(dx, dy)` (where center is 0,0), compute `x = dx / 2` and `y = dy / 2` because `ratio = pow(2.0, -1.0) = 0.5`, then compute `r2 = x*x + y*y`, and set the kernel value to `(2 - r2) * exp(r2 / 2)`. Convolve the equalized image with this kernel using `cv::filter2D` with output type `CV_32F`. Then, initialize a 31x31 float matrix `blocks` to zero. For each block row `r` (0..30) and block column `c` (0..30), take the region of interest in the filtered image with top-left corner at `(r*16, c*16)` and dimensions 16x16 (note: row is vertical coordinate, column is horizontal coordinate, so the ROI rect is `Rect(col*16, row*16, 16, 16)` in (x,y) order). Compute the sum of all pixels in that ROI using `cv::sum` and assign to `blocks.at<float>(r,c)`. Finally, create a 1x9 byte matrix (or `std::vector<uchar>` of size 9) and fill it by iterating over the 31x31 blocks with window top-left at `(row=0,4,...,28)` and `(col=0,4,...,28)`, each window is 3x3. Compute the average of the 9 values. Then, for each element in the 3x3 window (row-major order), shift the current byte left by 1, and if the element is greater than the average, set bit 0. When 8 bits have been accumulated, write the byte to the next position. Edge cases: ensure the input is not empty; if the input is smaller than 512, resizing scales up; the ROI extraction for blocks should not go out of bounds because we use 31*16=496 <= 512, leaving 16 pixels unused at the bottom and right edges, which is ignored. Time complexity is dominated by the 512x512 operations: Gaussian blur O(512^2 · 49), resize O(512^2), equalizeHist O(512^2), filter2D O(512^2 · 17^2), block sums O(31^2·256), and hash bit extraction O(9·9·9) which is trivial. Overall O(512^2 · 289) ≈ O(7.5·10^7) operations, acceptable for a single image. Space complexity is O(512^2) for the intermediate images plus O(31^2) for blocks.
