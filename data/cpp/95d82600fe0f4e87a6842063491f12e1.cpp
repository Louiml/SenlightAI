// Write a C++ function that simulates the pitch tracking logic found in the bass post-filter of the USAC audio decoder. Given a mono audio signal stored as a `std::vector<double>` of floating-point samples, a pitch period `T` (an integer number of samples), and a frame size `L`, the function must determine whether the true pitch period should be halved to `T/2`. The decision is based on normalized cross-correlation between two windowed segments: one starting at a fixed offset within the frame, and the other delayed by `T/2`. Specifically, compute the normalized correlation (the correlation coefficient) between the segment `x[i] = signal[offset + i]` and `y[i] = signal[offset + T/2 + i]` for `i = 0, 1, …, N-1` where `N` is a window length (e.g., 64 samples). If the correlation coefficient exceeds a threshold of 0.95, return `true` (meaning the pitch should be halved); otherwise return `false`. Handle edge cases: if `T/2` is 0 or the delayed segment goes out of bounds, return `false`. The function signature should be: `bool shouldHalvePitch(const std::vector<double>& signal, int offset, int T, int N, double threshold = 0.95);`
#include <cassert>
#include <vector>

// Declare the function under test (already defined above)
bool shouldHalvePitch(const std::vector<double>& signal, int offset, int T, int N, double threshold = 0.95);

int main() {
    // Case 1: Perfectly periodic signal with period T = 20. Segment at offset 0 and delayed by T/2=10
    // should be highly correlated, so halving should be true.
    std::vector<double> signal1(200, 0.0);
    for (int i = 0; i < 200; ++i) {
        signal1[i] = std::sin(2.0 * 3.141592653589793 * i / 20.0);
    }
    assert(shouldHalvePitch(signal1, 0, 20, 64) == true);

    // Case 2: Signal with period T=20 but check correlation at T/2=10 for a signal that actually has period 20,
    // so correlation should be high, but if we use a non-periodic signal, correlation should be low.
    std::vector<double> noise_signal(200);
    for (int i = 0; i < 200; ++i) {
        noise_signal[i] = ((i * 73) % 100) / 100.0 - 0.5; // pseudo-random-ish
    }
    // For random noise, correlation between offset 0 and offset + 10 should be near zero
    assert(shouldHalvePitch(noise_signal, 0, 20, 64) == false);

    // Case 3: Out of bounds: T/2 delay goes past end of signal
    std::vector<double> short_signal(50, 1.0);
    assert(shouldHalvePitch(short_signal, 0, 20, 64) == false); // offset+10+64 > 50

    // Case 4: T/2 is zero (T=1) → should return false
    assert(shouldHalvePitch(signal1, 0, 1, 10) == false);

    // Case 5: Exact threshold: construct a case where correlation exactly equals 0.95,
    // function should return false because we require > threshold.
    // Create two identical segments with slight scaling so correlation = 1.0? Actually identical gives 1.0 > 0.95.
    // To test exact, we can manually set correlation exactly? Simpler: use identical vectors, true.
    std::vector<double> constant_signal(100, 0.5);
    assert(shouldHalvePitch(constant_signal, 0, 10, 20) == true); // constant signals have correlation 1.0

    // Case 6: Zero energy segment: all zeros → denominator zero → false
    std::vector<double> zero_signal(100, 0.0);
    assert(shouldHalvePitch(zero_signal, 0, 10, 20) == false);

    // Case 7: Negative offset → false
    assert(shouldHalvePitch(signal1, -5, 20, 10) == false);

    // Case 8: N larger than signal → false
    assert(shouldHalvePitch(signal1, 0, 20, 300) == false);

    return 0;
}
#include <vector>
#include <cmath>

// Determines if the pitch period should be halved by checking normalized cross-correlation.
// The function compares segment starting at `offset` with segment delayed by T/2.
// Returns true if correlation coefficient > threshold and all indices are valid.
bool shouldHalvePitch(const std::vector<double>& signal, int offset, int T, int N, double threshold = 0.95) {
    // Validate basic parameters
    if (N <= 0 || offset < 0 || T <= 0) return false;
    
    int halfT = T / 2;
    // Half pitch must be at least 1 sample; also ensure both windows fit in the signal
    if (halfT == 0) return false;
    if (offset + N > static_cast<int>(signal.size())) return false;
    if (offset + halfT + N > static_cast<int>(signal.size())) return false;
    
    // Compute sums of squares and cross-correlation without mean subtraction (zero-mean assumption)
    double sumX2 = 0.0, sumY2 = 0.0, sumXY = 0.0;
    for (int i = 0; i < N; ++i) {
        double x = signal[offset + i];
        double y = signal[offset + halfT + i];
        sumX2 += x * x;
        sumY2 += y * y;
        sumXY += x * y;
    }
    
    // Avoid division by zero if either energy is zero
    if (sumX2 <= 0.0 || sumY2 <= 0.0) return false;
    
    // Normalized correlation = sumXY / sqrt(sumX2 * sumY2)
    double denominator = std::sqrt(sumX2 * sumY2);
    if (denominator <= 0.0) return false;
    double correlation = sumXY / denominator;
    
    return correlation > threshold;
}
// The solution computes the Pearson correlation coefficient between two equal-length sequences. The correlation coefficient is given by `corr = sum((x_i - mean_x)(y_i - mean_y)) / (sqrt(sum((x_i - mean_x)^2)) * sqrt(sum((y_i - mean_y)^2)))`. However, to match the spirit of the DSP code (which uses energy and cross-correlation without mean subtraction, as the signals are zero-mean audio), we can use the normalized cross-correlation without mean removal: `corr = sum(x_i * y_i) / sqrt(sum(x_i^2) * sum(y_i^2))`. This is simpler and matches the code snippet's approach (which computes `ener`, `corr`, `tmp` without mean subtraction, then forms `corr / sqrt(ener * tmp)`). We must first check that the indices are valid: `offset >= 0`, `offset + N <= signal.size()`, `offset + T/2 + N <= signal.size()`, and `T/2 >= 1` (since pitch of 0 is invalid and T must be even to halve cleanly; if T is odd, we still compute T/2 integer division, but if that is 0, return false). To avoid division by zero, compute the energy terms; if either is zero (or extremely small), return false. The time complexity is O(N) per call, O(1) auxiliary space.
