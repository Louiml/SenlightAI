// Write a C++ function `estimateGyroRate` that simulates the adaptive gyroscope rate estimation logic from the provided `SensorFusion::process` method. Given a sequence of gyroscope event timestamps (in nanoseconds) as a `std::vector<int64_t>`, the function must compute an estimated gyro rate in Hz using the same recursive smoothing formula: for each consecutive pair of timestamps with a positive difference less than 50,000,000 ns (0.05 s), compute `dT` in seconds, then `freq = 1/dT`; if `freq` is between 100 and 1000 Hz inclusive, update the estimate as `estimate = freq + (estimate - freq) * alpha`, where `alpha = 1/(1+dT)`. The initial estimate is `200.0` Hz. Timestamps with invalid differences (non-positive or ≥50 ms) are ignored for updating but still advance the "previous timestamp" reference. The function must return the final estimated rate as a `float`. If the input vector is empty, return the initial estimate `200.0f`. The function must be `const`-qualified appropriately if it were a member, but as a free function, it should take the vector by `const&` and return `float`.
// The algorithm iterates through the timestamps in order, maintaining `prevTimestamp` (initially the first timestamp) and `estimatedRate` (initially 200.0). For each subsequent timestamp `current`, we compute `delta = current - prevTimestamp`. If `delta > 0` and `delta < 50'000'000`, we accept it: `dT = delta / 1e9`, then `freq = 1.0f / dT`. We check `100 <= freq && freq < 1000` (note strict `<` on upper bound as in original). If valid, we apply smoothing: `alpha = 1.0f / (1.0f + dT)` and `estimatedRate = freq + (estimatedRate - freq) * alpha`. After processing (regardless of validity), we set `prevTimestamp = current`. Edge cases: empty input returns 200.0; a single timestamp has no pairs so returns 200.0; timestamps that are equal or decreasing or have too-large gap are ignored but still update the previous reference. The formula simplifies to `estimatedRate = freq * alpha + estimatedRate * (1 - alpha)`, but the original form is fine. Time complexity is O(n) for n timestamps; space is O(1) besides the input vector.
#include <vector>
#include <cstdint>

// Estimate the gyroscope event rate from a sequence of timestamps (ns).
// Uses adaptive smoothing similar to Android's SensorFusion.
// Initial estimate is 200 Hz. Only valid intervals (0 < dt < 5e7 ns)
// with resulting frequency in [100, 1000) Hz update the estimate.
float estimateGyroRate(const std::vector<int64_t>& timestamps) {
    const float kInitialRate = 200.0f;
    if (timestamps.empty()) {
        return kInitialRate;
    }

    float estimatedRate = kInitialRate;
    int64_t previousTimestamp = timestamps[0];

    for (size_t i = 1; i < timestamps.size(); ++i) {
        const int64_t currentTimestamp = timestamps[i];
        const int64_t delta = currentTimestamp - previousTimestamp;

        if (delta > 0 && delta < static_cast<int64_t>(5e7)) {
            const float dT = static_cast<float>(delta) / 1e9f;
            const float freq = 1.0f / dT;
            if (freq >= 100.0f && freq < 1000.0f) {
                const float alpha = 1.0f / (1.0f + dT);
                estimatedRate = freq + (estimatedRate - freq) * alpha;
            }
        }

        previousTimestamp = currentTimestamp;
    }

    return estimatedRate;
}
#include <cassert>
#include <vector>
#include <cstdint>

int main() {
    // Empty input returns initial rate.
    assert(estimateGyroRate({}) == 200.0f);

    // Single timestamp: no valid pairs, stays initial.
    assert(estimateGyroRate({1000}) == 200.0f);

    // Perfect 200 Hz (5 ms intervals) should keep around 200.
    std::vector<int64_t> ts200;
    for (int i = 0; i <= 100; ++i) {
        ts200.push_back(i * 5'000'000LL);
    }
    float r = estimateGyroRate(ts200);
    assert(r > 199.0f && r < 201.0f);

    // Perfect 500 Hz (2 ms intervals) should converge near 500.
    std::vector<int64_t> ts500;
    for (int i = 0; i <= 100; ++i) {
        ts500.push_back(i * 2'000'000LL);
    }
    r = estimateGyroRate(ts500);
    assert(r > 490.0f && r < 510.0f);

    // Invalid intervals: negative gap and too large gap are ignored.
    // Sequence: first-valid 5ms, then a negative gap (ignored, prev updates
    // to that timestamp), then a 10ms gap from there (valid, 100 Hz).
    std::vector<int64_t> mixed = {0, 5'000'000, 4'000'000, 14'000'000};
    // After first pair: freq=200 => estimate near 200.
    // Negative gap ignored, prev=4'000'000.
    // Last delta=10'000'000 => freq=100 => update.
    float rm = estimateGyroRate(mixed);
    assert(rm > 100.0f && rm < 200.0f);

    // Out-of-range frequency (delta=1ns => 1e9 Hz, >1000) should be ignored.
    std::vector<int64_t> invalid_high = {0, 1};
    assert(estimateGyroRate(invalid_high) == 200.0f);

    // Out-of-range low (delta=0.02s => 50 Hz, <100) should be ignored.
    std::vector<int64_t> invalid_low = {0, 20'000'000};
    assert(estimateGyroRate(invalid_low) == 200.0f);

    return 0;
}
