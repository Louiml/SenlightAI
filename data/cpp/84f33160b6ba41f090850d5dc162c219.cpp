/*
Write a C++ function named `trackMeanShiftWindow` that simulates the core logic of the OpenCV meanshift tracking algorithm on a sequence of synthetic frames. The function must accept a vector of `cv::Mat` frames (each in BGR format), an initial `cv::Rect` tracking window, and a `cv::TermCriteria` object (with `EPS|COUNT`). For each frame, it must compute the HSV histogram of the initial ROI (using the same color range filtering as the snippet), then use `calcBackProject` and `meanShift` to iteratively update the window, and finally store the resulting rectangle in a caller-provided `std::vector<cv::Rect>`. The function should return `true` if all frames were processed, or `false` if any frame is empty or the initial window is invalid (zero-area or outside the frame). Assume OpenCV headers are available and that the input frames have consistent dimensions.
*/
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/video/tracking.hpp>
#include <vector>

// Simulate meanshift tracking on a sequence of BGR frames.
// frames: input sequence of color images (non-empty, same size)
// initial_window: starting location for tracking
// criteria: termination criteria for meanShift (EPS|COUNT)
// tracked_windows: output vector of rectangles, one per frame
// Returns true on success, false if any frame is empty or the initial window is invalid.
bool trackMeanShiftWindow(
    const std::vector<cv::Mat>& frames,
    const cv::Rect& initial_window,
    const cv::TermCriteria& criteria,
    std::vector<cv::Rect>& tracked_windows)
{
    if (frames.empty() || initial_window.width <= 0 || initial_window.height <= 0)
        return false;

    // Ensure the initial window is within the first frame bounds
    const cv::Mat& first_frame = frames.front();
    if (initial_window.x < 0 || initial_window.y < 0 ||
        initial_window.x + initial_window.width > first_frame.cols ||
        initial_window.y + initial_window.height > first_frame.rows)
    {
        return false;
    }

    // Compute histogram from the first frame's ROI
    cv::Mat roi = first_frame(initial_window);
    cv::Mat hsv_roi, mask;
    cv::cvtColor(roi, hsv_roi, cv::COLOR_BGR2HSV);
    cv::inRange(hsv_roi, cv::Scalar(0, 60, 32), cv::Scalar(180, 255, 255), mask);

    float range_[] = {0, 180};
    const float* range[] = {range_};
    int histSize[] = {180};
    int channels[] = {0};
    cv::Mat roi_hist;
    cv::calcHist(&hsv_roi, 1, channels, mask, roi_hist, 1, histSize, range);
    cv::normalize(roi_hist, roi_hist, 0, 255, cv::NORM_MINMAX);

    // Track on each frame
    tracked_windows.clear();
    tracked_windows.reserve(frames.size());

    for (const auto& frame : frames) {
        if (frame.empty())
            return false;

        cv::Mat hsv, backproj;
        cv::cvtColor(frame, hsv, cv::COLOR_BGR2HSV);
        cv::calcBackProject(&hsv, 1, channels, roi_hist, backproj, range);

        // Keep a mutable copy of the current window for meanShift
        cv::Rect current_window = tracked_windows.empty() ? initial_window : tracked_windows.back();
        cv::meanShift(backproj, current_window, criteria);

        tracked_windows.push_back(current_window);
    }

    return true;
}
#include <cassert>
#include <vector>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>

int main() {
    // Create a synthetic 200x200 black frame with a red rectangle as target
    const int width = 200, height = 200;
    cv::Mat frame(height, width, CV_8UC3, cv::Scalar(0, 0, 0));
    cv::rectangle(frame, cv::Rect(50, 40, 30, 30), cv::Scalar(0, 0, 255), cv::FILLED); // red rectangle

    std::vector<cv::Mat> frames;
    frames.push_back(frame.clone());
    cv::Mat shifted = frame.clone();
    cv::rectangle(shifted, cv::Rect(60, 45, 30, 30), cv::Scalar(0, 0, 255), cv::FILLED);
    frames.push_back(shifted);

    // Initial window that overlaps the red rectangle
    cv::Rect initial(55, 40, 30, 30);
    cv::TermCriteria criteria(cv::TermCriteria::EPS | cv::TermCriteria::COUNT, 10, 1);

    std::vector<cv::Rect> result;
    bool ok = trackMeanShiftWindow(frames, initial, criteria, result);
    assert(ok);
    assert(result.size() == 2);
    // Ensure both windows have positive area
    assert(result[0].area() > 0);
    assert(result[1].area() > 0);
    // The second window should be shifted right/down (approximately) toward the new rectangle
    assert(result[1].x >= result[0].x);
    assert(result[1].y >= result[0].y);

    // Test invalid input: empty frame
    std::vector<cv::Mat> bad_frames;
    bad_frames.push_back(cv::Mat());
    ok = trackMeanShiftWindow(bad_frames, initial, criteria, result);
    assert(!ok);

    // Test invalid initial window (outside frame)
    cv::Rect outside_win(250, 250, 20, 20);
    ok = trackMeanShiftWindow(frames, outside_win, criteria, result);
    assert(!ok);

    // Test zero-area window
    cv::Rect zero_win(10, 10, 0, 0);
    ok = trackMeanShiftWindow(frames, zero_win, criteria, result);
    assert(!ok);

    return 0;
}
// The solution mirrors the original snippet’s pipeline: for the first frame, extract the ROI from the initial rectangle, convert to HSV, apply an `inRange` mask with `Scalar(0, 60, 32)` and `Scalar(180, 255, 255)` to isolate colored objects, calculate a 1D histogram on the hue channel (180 bins, range 0–180) using the mask, and normalize to 0–255. For each subsequent frame (including the first, though the window is unchanged there), convert the full frame to HSV, compute a backprojection using the stored histogram, and call `meanShift` to refine the window. The output rectangles are pushed into the result vector. Edge cases: if any frame is empty, return false; if the initial rectangle has non-positive width/height or lies entirely outside the frame, return false immediately. The histogram uses only the hue channel, so dark or low-saturation pixels are filtered by the mask, matching the snippet. Time complexity is O(F * (W*H + iterations)) per frame, where W and H are frame dimensions and iterations are bounded by the termination criteria (default 10). Space complexity is O(W*H) for temporary Mats per frame, plus O(F) for the result rectangle list.
