// Create a C++ function that computes the numerical derivative of a discrete-time signal using a sliding window of samples. The function should take a stream of timestamped sample values (as pairs of time in microseconds and value), maintain a fixed-size buffer of recent samples (size 11), and return the current derivative estimate (change in value per microsecond) scaled by 1e6 to get units per second. The derivative should be estimated using linear regression over the buffered samples, and the function must correctly handle an empty buffer by returning 0.0f. Design the function to be called repeatedly with new samples, and ensure it is reusable for multiple independent signal streams.

// The solution maintains a circular buffer of up to 11 most recent (time, value) pairs. When a new sample arrives, it is appended, evicting the oldest if the buffer is full. To compute the derivative, use least-squares linear regression on the buffered points: compute the slope as `sum((t_i - t_mean) * (v_i - v_mean)) / sum((t_i - t_mean)^2)`. This slope is in value per microsecond. Multiply by 1e6 to convert to value per second. Edge cases: if fewer than 2 samples exist, regression is undefined, so return 0.0f. If all timestamps are identical (denominator zero), return 0.0f to avoid division by zero. Time complexity per call is O(k) where k is the buffer size (max 11), since we iterate over all buffered samples. Space complexity is O(k) for the buffer. The implementation uses `std::vector` or C-style arrays with head/tail indices for efficiency, but a simple vector with push_back and erase is acceptable given the small fixed size.

#include <vector>
#include <cstdint>

// Maintains a sliding window of samples to compute a derivative.
// Call update() with each new sample; getSlope() returns the derivative in units/second.
class DerivativeFilter {
public:
    // Update the filter with a new sample. time_us is in microseconds.
    void update(float value, uint32_t time_us) {
        samples_.push_back({static_cast<float>(time_us), value});
        if (samples_.size() > kMaxSamples) {
            samples_.erase(samples_.begin());
        }
    }
    
    // Returns the current derivative estimate scaled to units per second.
    // Returns 0.0f if fewer than 2 samples or if time values are constant.
    float slope() const {
        if (samples_.size() < 2) {
            return 0.0f;
        }
        
        // Compute means
        double t_mean = 0.0;
        double v_mean = 0.0;
        for (const auto& s : samples_) {
            t_mean += s.first;
            v_mean += s.second;
        }
        t_mean /= samples_.size();
        v_mean /= samples_.size();
        
        // Compute slope via least squares
        double numerator = 0.0;
        double denominator = 0.0;
        for (const auto& s : samples_) {
            double dt = s.first - t_mean;
            numerator += dt * (s.second - v_mean);
            denominator += dt * dt;
        }
        
        if (denominator == 0.0) {
            return 0.0f;
        }
        
        // Slope is in value/microsecond; scale to value/second
        return static_cast<float>((numerator / denominator) * 1.0e6);
    }
    
private:
    static constexpr size_t kMaxSamples = 11;
    std::vector<std::pair<float, float>> samples_;  // (time_us, value)
};

#include <cmath>
#include <cassert>
#include <cstdint>
#include <iostream>

// The solution function is the DerivativeFilter class above.
// Test code below verifies behavior with known signals and edge cases.

int main() {
    // Test 1: Empty filter returns 0
    DerivativeFilter f1;
    assert(f1.slope() == 0.0f);

    // Test 2: Single sample returns 0
    DerivativeFilter f2;
    f2.update(1.0f, 1000);
    assert(f2.slope() == 0.0f);

    // Test 3: Linear signal: slope = 2 units/sec (2 units/microsecond * 1e6)
    // Times: 1000, 2000, 3000; Values: 1, 3, 5 (value = (time-1000)*2e-6 + 1)
    DerivativeFilter f3;
    f3.update(1.0f, 1000);
    f3.update(3.0f, 2000);
    f3.update(5.0f, 3000);
    assert(std::abs(f3.slope() - 2.0f) < 1e-3f);

    // Test 4: Constant signal => slope 0
    DerivativeFilter f4;
    f4.update(4.0f, 500);
    f4.update(4.0f, 600);
    f4.update(4.0f, 700);
    assert(std::abs(f4.slope()) < 1e-6f);

    // Test 5: Identical timestamps (invalid) => 0
    DerivativeFilter f5;
    f5.update(1.0f, 1000);
    f5.update(2.0f, 1000);
    assert(f5.slope() == 0.0f);

    // Test 6: Buffer size limited to 11; older samples evicted
    DerivativeFilter f6;
    // Fill with 15 samples of a known linear function
    // For times 1..15, value = time (so slope = 1e6 units/sec)
    for (uint32_t t = 1; t <= 15; ++t) {
        f6.update(static_cast<float>(t), t);  // value equals time in microseconds
    }
    // Last 11 samples: t=5..15, value=t => slope = 1.0 units/microsecond => 1e6
    assert(std::abs(f6.slope() - 1.0e6f) < 1e-2f);

    // Test 7: Noise tolerance (small random noise around a line)
    DerivativeFilter f7;
    for (uint32_t t = 1; t <= 10; ++t) {
        float exact = 3.0f * t;  // slope 3 units/sec
        float noisy = exact + (t % 2 == 0 ? 0.1f : -0.1f);
        f7.update(noisy, t);
    }
    // Slope should be near 3 (1e6 * 3e-6 = 3) but not exact
    assert(std::abs(f7.slope() - 3.0f) < 0.5f);

    std::cout << "All tests passed!" << std::endl;
    return 0;
}
