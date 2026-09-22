/*
Write a standalone C++ function that performs feature-based image matching between a reference image and a video frame using ORB (Oriented FAST and Rotated BRIEF) descriptors, a FLANN-based matcher with the LSH (Locality-Sensitive Hashing) index for binary descriptors, and Lowe's ratio test to filter matches. The function should take as input a reference image file path (as `const std::string&`) and return the number of good matches found, while also drawing the matches onto an output image (passed by reference as a `cv::Mat&`). The function must not rely on OpenCV's non-free SURF or SIFT modules; instead, it should use ORB which is freely available in the core `features2d` module. The video capture will be handled externally, so the function only processes a single test image (the current frame) against the reference image. The output image should be the concatenation of the test image and reference image side-by-side with colored lines connecting matched keypoints, and only matches passing the ratio test (where the best match distance is less than 0.7 times the second-best match distance) should be drawn. The function should return the count of good matches to allow the caller to assess confidence. Handle the case where the reference image cannot be loaded by returning 0 and leaving the output image empty. Assume the test image is valid and grayscale conversion is performed internally.
*/

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/features2d.hpp>
#include "opencv2/flann/miniflann.hpp"

#include <vector>

// Perform ORB-based matching between a reference image and a test image.
// Returns the number of good matches after Lowe's ratio test.
// The output image contains the test image (left) and reference (right) with matches drawn.
int matchORBFeatures(const std::string& refImagePath, const cv::Mat& testImg, cv::Mat& dstImg) {
    // Load reference image
    cv::Mat refImg = cv::imread(refImagePath, cv::IMREAD_COLOR);
    if (refImg.empty()) {
        dstImg = cv::Mat(); // empty output
        return 0;
    }

    // Convert both images to grayscale
    cv::Mat refGray, testGray;
    cv::cvtColor(refImg, refGray, cv::COLOR_BGR2GRAY);
    cv::cvtColor(testImg, testGray, cv::COLOR_BGR2GRAY);

    // Create ORB detector/extractor
    cv::Ptr<cv::ORB> orb = cv::ORB::create();

    // Detect keypoints and compute descriptors for reference
    std::vector<cv::KeyPoint> refKeypoints;
    cv::Mat refDescriptors;
    orb->detectAndCompute(refGray, cv::noArray(), refKeypoints, refDescriptors);

    // Detect keypoints and compute descriptors for test
    std::vector<cv::KeyPoint> testKeypoints;
    cv::Mat testDescriptors;
    orb->detectAndCompute(testGray, cv::noArray(), testKeypoints, testDescriptors);

    // If either has no descriptors, return 0
    if (refDescriptors.empty() || testDescriptors.empty()) {
        dstImg = cv::Mat();
        return 0;
    }

    // Configure FLANN matcher for binary descriptors (LSH)
    cv::Ptr<cv::flann::IndexParams> indexParams = cv::makePtr<cv::flann::LshIndexParams>(12, 20, 2);
    cv::Ptr<cv::flann::SearchParams> searchParams = cv::makePtr<cv::flann::SearchParams>(50);
    cv::FlannBasedMatcher matcher(indexParams, searchParams);

    // Find two best matches for each test descriptor
    std::vector<std::vector<cv::DMatch>> knnMatches;
    matcher.knnMatch(testDescriptors, refDescriptors, knnMatches, 2);

    // Apply Lowe's ratio test
    std::vector<cv::DMatch> goodMatches;
    const float ratioThreshold = 0.7f;
    for (const auto& matchPair : knnMatches) {
        if (matchPair.size() >= 2) {
            if (matchPair[0].distance < ratioThreshold * matchPair[1].distance) {
                goodMatches.push_back(matchPair[0]);
            }
        }
    }

    // Draw matches: test image on left, reference on right
    cv::drawMatches(testImg, testKeypoints, refImg, refKeypoints,
                    goodMatches, dstImg, cv::Scalar(0, 0, 255), cv::Scalar(255, 0, 0),
                    std::vector<char>(), cv::DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);

    return static_cast<int>(goodMatches.size());
}

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <cassert>

// Declaration of the function being tested (assume it's in a header or above)
int matchORBFeatures(const std::string& refImagePath, const cv::Mat& testImg, cv::Mat& dstImg);

int main() {
    // Create a simple synthetic test image with known content
    cv::Mat testImg(200, 200, CV_8UC3, cv::Scalar(0, 0, 0));
    // Draw a white square in the middle
    cv::rectangle(testImg, cv::Rect(50, 50, 100, 100), cv::Scalar(255, 255, 255), cv::FILLED);
    // Save it as a temporary reference file
    cv::imwrite("test_ref.png", testImg);
    // Same image used as test should produce many good matches
    cv::Mat out;
    int goodCount = matchORBFeatures("test_ref.png", testImg, out);
    assert(goodCount > 0); // At least one good match expected
    assert(!out.empty()); // Output image should be drawn

    // Test with a non-existent reference file
    cv::Mat emptyOut;
    int countEmpty = matchORBFeatures("nonexistent.png", testImg, emptyOut);
    assert(countEmpty == 0);
    assert(emptyOut.empty());

    // Test with a completely different image (black) should yield few matches, but still valid
    cv::Mat blackTest(200, 200, CV_8UC3, cv::Scalar(0, 0, 0));
    cv::Mat out2;
    int countBlack = matchORBFeatures("test_ref.png", blackTest, out2);
    assert(countBlack >= 0); // Should not crash; may be 0 or more
    assert(!out2.empty() || countBlack == 0); // Either output drawn or count zero

    // Clean up temporary file
    std::remove("test_ref.png");
    return 0;
}

// The solution involves loading the reference image from the given file path, converting it to grayscale if it isn't already (using `cv::cvtColor` with `COLOR_BGR2GRAY` if needed), and checking for failure—if the image is empty, return 0. Next, create an ORB detector/extractor using `cv::ORB::create()` (which returns a pointer to a feature detector and descriptor extractor). Detect keypoints and compute descriptors for the reference image. For the test image (passed by value or as a const reference), also convert to grayscale, detect keypoints, and compute descriptors. Then, configure a FLANN-based matcher suited for binary descriptors: use `cv::FlannBasedMatcher` with a `cv::makePtr<cv::flann::LshIndexParams>(12, 20, 2)` (or similar parameters) because ORB produces binary descriptors. Call `knnMatch` with k=2 to get the two nearest neighbors for each test descriptor. Apply Lowe's ratio test: for each pair of matches, if the distance of the best match is less than 0.7 times the distance of the second-best match, keep the best match in a `goodMatches` vector. Also verify that `matches[i]` has at least two entries to avoid out-of-bounds. Then, use `cv::drawMatches` with the test image and reference image, the keypoints and good matches, an output image, and flags to draw only the good matches with colored lines (e.g., red lines on green keypoints). Finally, return the `goodMatches.size()` count. Edge cases: empty reference image, no keypoints detected in either image (then knnMatch returns empty), or fewer than two matches for any keypoint (guarded by the size check). Time complexity: ORB feature detection is O(number of pixels), descriptor extraction is O(keypoints), and knnMatch using LSH is approximately O(n * m / k) where n and m are keypoint counts, but typically fast. Space complexity: O(n + m + number of matches) for storing keypoints, descriptors, and match vectors.
