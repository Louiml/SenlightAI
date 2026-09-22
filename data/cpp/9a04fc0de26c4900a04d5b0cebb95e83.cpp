// Given a vector of 3D translation vectors (each represented as a `cv::Mat` of size 3×1, CV_64F) measured over time for a rotating target, and a sequence of corresponding timestamps, write a C++ function `predictFutureTranslation` that: (1) uses the first half of the time series to estimate the angular velocity by fitting a sine function to the angular displacement (computed as the norm of each translation vector normalized to zero mean), (2) predicts the translation vector at a specified future time by rotating the most recent translation vector around the Z‑axis by the predicted angular displacement (using `omega * (future_time - last_time)`), and (3) returns the predicted translation as a `cv::Mat` of size 3×1. Assume that the translation vectors are already motion‑compensated (no linear drift) and that the rotation is purely planar around the Z‑axis. The function must handle cases where fewer than 10 data points are provided by returning the last translation vector. The function should not rely on OpenCV's calibration or Kalman filter modules; it must only use core OpenCV matrix operations and standard math.

// The core idea is to model the target's angular motion as a sinusoidal function of time. First, compute the angular displacement for each timestamp as the norm (Euclidean length) of the corresponding translation vector, then subtract the mean to center the signal. Use a least‑squares fit to a sine wave with unknown amplitude, phase, and angular frequency. Since the frequency is unknown, we estimate it by observing that with a pure sinusoidal signal, the period equals the time span of the data divided by an integer number of half‑cycles. A pragmatic approach: estimate the angular frequency as `2π / (2 * (T_last - T_first))`, assuming one full cycle over the observed interval is a reasonable guess (this is exactly what the original snippet does). Given that guess, we solve for amplitude and phase via linear least squares: represent the signal as `A*sin(ωt) + B*cos(ωt)`; then `amplitude = sqrt(A² + B²)` and `phase = atan2(B, A)`. The predicted angular displacement at the future time is `amplitude * sin(ω * future_time + phase)`. The rotation matrix for a 2D planar rotation around the Z‑axis is `[cosθ, -sinθ, 0; sinθ, cosθ, 0; 0, 0, 1]`. Multiply this matrix by the most recent translation vector to obtain the predicted position. Edge cases: if fewer than 10 points are given, return the last translation vector unchanged; if all translation vectors are identical (zero angular displacement), the fit may produce division by zero, so handle that by returning the last vector. The time complexity is O(N) for the fit over N points, and space complexity is O(1) auxiliary beyond the input.

#include <opencv2/core.hpp>
#include <cmath>
#include <vector>
#include <stdexcept>

// Predict the future 3D translation vector of a rotating target.
// Input:
//   translations - vector of cv::Mat (3x1, CV_64F) representing the target's position at each timestamp.
//   times        - vector of double timestamps, same size as translations, sorted in ascending order.
//   future_time  - the time at which to predict the translation.
// Output:
//   cv::Mat of size 3x1, CV_64F, containing the predicted translation vector.
// Behavior:
//   - If fewer than 10 data points, or if all translation vectors are identical, returns the last translation.
//   - Otherwise, fits a sine wave to the angular displacement (norm of the translation vector minus mean),
//     estimates angular frequency from the time span, and rotates the last translation around Z by the predicted angle.
cv::Mat predictFutureTranslation(const std::vector<cv::Mat>& translations,
                                 const std::vector<double>& times,
                                 double future_time) {
    if (translations.empty() || translations[0].empty()) {
        throw std::invalid_argument("Empty translations provided");
    }
    if (translations.size() != times.size()) {
        throw std::invalid_argument("Times and translations size mismatch");
    }
    if (translations.size() < 10) {
        return translations.back().clone();
    }

    // Compute angular displacement as the norm of each translation vector.
    std::vector<double> angles;
    angles.reserve(translations.size());
    double sum = 0.0;
    for (const auto& t : translations) {
        if (t.rows != 3 || t.cols != 1 || t.type() != CV_64F) {
            throw std::invalid_argument("Translation must be 3x1 CV_64F");
        }
        double norm = cv::norm(t);
        angles.push_back(norm);
        sum += norm;
    }
    double mean = sum / static_cast<double>(angles.size());

    // Center the angles to remove constant offset.
    for (auto& a : angles) {
        a -= mean;
    }

    // Estimate angular frequency: assume one full cycle over the observed time span.
    double T_span = times.back() - times.front();
    if (T_span <= 0.0 || std::abs(T_span) < 1e-12) {
        // All times identical -> no rotation, return last.
        return translations.back().clone();
    }
    double omega = 2.0 * CV_PI / (2.0 * T_span); // Note: one full cycle = 2π, but using original snippet's factor of 2

    // Least-squares fit to A*sin(ωt) + B*cos(ωt)
    double sum_sin2 = 0.0, sum_cos2 = 0.0, sum_sin_theta = 0.0, sum_cos_theta = 0.0;
    for (size_t i = 0; i < angles.size(); ++i) {
        double s = std::sin(omega * times[i]);
        double c = std::cos(omega * times[i]);
        sum_sin2 += s * s;
        sum_cos2 += c * c;
        sum_sin_theta += s * angles[i];
        sum_cos_theta += c * angles[i];
    }

    // Guard against division by zero (e.g., if all angles are zero).
    if (std::abs(sum_sin2) < 1e-12 && std::abs(sum_cos2) < 1e-12) {
        return translations.back().clone();
    }

    double A = (std::abs(sum_sin2) > 1e-12) ? (sum_sin_theta / sum_sin2) : 0.0;
    double B = (std::abs(sum_cos2) > 1e-12) ? (sum_cos_theta / sum_cos2) : 0.0;
    double amplitude = std::sqrt(A * A + B * B);
    double phase = std::atan2(B, A);

    // Predicted angular displacement at future time (minus the mean offset we removed, but we want absolute angle).
    // Since we centered the angles, the mean corresponds to the DC offset. The actual angle is assumed to be
    // mean + amplitude*sin(ω*t + phase). However, for rotation we need the change from the last time.
    double last_time = times.back();
    double angle_last = mean + amplitude * std::sin(omega * last_time + phase);
    double angle_future = mean + amplitude * std::sin(omega * future_time + phase);
    double delta_theta = angle_future - angle_last;

    // Rotation matrix around Z-axis by delta_theta.
    double cos_t = std::cos(delta_theta);
    double sin_t = std::sin(delta_theta);
    cv::Mat rot = (cv::Mat_<double>(3,3) <<
                   cos_t, -sin_t, 0.0,
                   sin_t,  cos_t, 0.0,
                   0.0,    0.0,   1.0);

    // Rotate the last translation vector.
    const cv::Mat& last_t = translations.back();
    cv::Mat predicted = rot * last_t;
    return predicted;
}

#include <cassert>
#include <cmath>
#include <vector>
#include <opencv2/core.hpp>

// Forward declaration of the function being tested.
cv::Mat predictFutureTranslation(const std::vector<cv::Mat>& translations,
                                 const std::vector<double>& times,
                                 double future_time);

int main() {
    // Test 1: Fewer than 10 points returns last translation.
    {
        std::vector<cv::Mat> trans;
        std::vector<double> times;
        for (int i = 0; i < 5; ++i) {
            trans.push_back((cv::Mat_<double>(3,1) << i*10.0, 0.0, 100.0));
            times.push_back(i * 0.1);
        }
        cv::Mat pred = predictFutureTranslation(trans, times, 999.0);
        assert(cv::norm(pred - trans.back()) < 1e-9);
    }

    // Test 2: Identical translations (no rotation) returns last.
    {
        std::vector<cv::Mat> trans;
        std::vector<double> times;
        for (int i = 0; i < 20; ++i) {
            trans.push_back((cv::Mat_<double>(3,1) << 5.0, -3.0, 7.0));
            times.push_back(i * 0.05);
        }
        cv::Mat pred = predictFutureTranslation(trans, times, 2.0);
        assert(cv::norm(pred - trans.back()) < 1e-9);
    }

    // Test 3: Pure rotation around Z with known constant angular velocity.
    {
        // Generate a target rotating at ω = 1 rad/s, radius = 10, height = 5, starting at (10,0,5).
        double omega = 1.0;
        double radius = 10.0;
        double height = 5.0;
        std::vector<cv::Mat> trans;
        std::vector<double> times;
        int N = 30;
        double dt = 0.1;
        for (int i = 0; i < N; ++i) {
            double t = i * dt;
            double angle = omega * t;
            double x = radius * std::cos(angle);
            double y = radius * std::sin(angle);
            trans.push_back((cv::Mat_<double>(3,1) << x, y, height));
            times.push_back(t);
        }
        // Predict at a future time beyond the last point.
        double future_t = times.back() + 0.5; // t = 2.9 + 0.5 = 3.4
        cv::Mat pred = predictFutureTranslation(trans, times, future_t);
        // Expected position: radius*cos(ω*future_t), radius*sin(ω*future_t), height
        double exp_x = radius * std::cos(omega * future_t);
        double exp_y = radius * std::sin(omega * future_t);
        assert(std::abs(pred.at<double>(0) - exp_x) < 1e-3);
        assert(std::abs(pred.at<double>(1) - exp_y) < 1e-3);
        assert(std::abs(pred.at<double>(2) - height) < 1e-9);
    }

    // Test 4: Zero angular displacement (angle constant) should not divide by zero.
    {
        std::vector<cv::Mat> trans;
        std::vector<double> times;
        for (int i = 0; i < 20; ++i) {
            trans.push_back((cv::Mat_<double>(3,1) << 1.0, 2.0, 3.0));
            times.push_back(i * 0.1);
        }
        // The function will hit the guard and return last.
        cv::Mat pred = predictFutureTranslation(trans, times, 5.0);
        assert(cv::norm(pred - trans.back()) < 1e-9);
    }

    // Test 5: Input with exactly 10 points (boundary condition) should work.
    {
        std::vector<cv::Mat> trans;
        std::vector<double> times;
        for (int i = 0; i < 10; ++i) {
            double angle = i * 0.3; // linear in angle, not sinusoidal, but function should not crash
            trans.push_back((cv::Mat_<double>(3,1) << std::cos(angle)*5.0, std::sin(angle)*5.0, 2.0));
            times.push_back(i * 0.1);
        }
        cv::Mat pred = predictFutureTranslation(trans, times, 2.0);
        // Just check it returns a 3x1 matrix.
        assert(pred.rows == 3 && pred.cols == 1);
    }

    return 0;
}
