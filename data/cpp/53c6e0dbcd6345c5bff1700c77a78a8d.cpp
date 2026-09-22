Inspired by the signal-processing decorrelation machinery above, consider a simplified “audio branch processor” that operates on a small block of samples. Write a standalone C++ function that takes a vector of real-valued samples and a single integer parameter `order` (representing a filter order, analogous to the decorrelation filter orders seen in the snippet). The function must apply a simple first-order difference filter with a configurable delay: for each sample index `i`, the output `y[i]` equals `x[i]` minus `x[i - delay]`, where `delay` is a positive integer given by the parameter (with the constraint that if `order` is even, `delay = order/2 + 1`; if `order` is odd, `delay = (order+1)/2`). For indices where `i - delay < 0`, the output equals `x[i]` (i.e., no subtraction, treating samples before the block as zero). The function must modify the input vector in place (transforming it into the filtered output) and return `true` on success, or `false` if the input vector is empty or if `order` is negative. This operation should resemble a simplified version of the decorrelation filtering that the original code performs on frequency-domain hybrid bands—but here we keep it deliberately simple and self-contained.
#include <cassert>
#include <vector>

// The free function is declared above; here is the test harness.
#include "solution.h" // in the actual exercise, the solution is in a header-like snippet.

int main() {
    // Basic test: order 0 => delay 1
    std::vector<double> v1 = {1.0, 2.0, 3.0, 4.0};
    assert(applyDelayDifferenceFilter(v1, 0) == true);
    // y[0]=1, y[1]=2-1=1, y[2]=3-2=1, y[3]=4-3=1
    assert(v1.size() == 4);
    assert(v1[0] == 1.0);
    assert(v1[1] == 1.0);
    assert(v1[2] == 1.0);
    assert(v1[3] == 1.0);

    // Test order 1 => delay 1 as well (odd)
    std::vector<double> v2 = {5.0, 7.0, 10.0};
    assert(applyDelayDifferenceFilter(v2, 1) == true);
    // y[0]=5, y[1]=7-5=2, y[2]=10-7=3
    assert(v2[0] == 5.0);
    assert(v2[1] == 2.0);
    assert(v2[2] == 3.0);

    // Test order 2 => delay = 2/2+1 = 2
    std::vector<double> v3 = {1.0, 1.0, 1.0, 1.0, 1.0};
    assert(applyDelayDifferenceFilter(v3, 2) == true);
    // y[0]=1, y[1]=1, y[2]=1-1=0, y[3]=1-1=0, y[4]=1-1=0
    assert(v3[0] == 1.0);
    assert(v3[1] == 1.0);
    assert(v3[2] == 0.0);
    assert(v3[3] == 0.0);
    assert(v3[4] == 0.0);

    // Test order 3 => delay = (3+1)/2 = 2
    std::vector<double> v4 = {1.0, 2.0, 3.0, 4.0, 5.0};
    assert(applyDelayDifferenceFilter(v4, 3) == true);
    // y[0]=1, y[1]=2, y[2]=3-1=2, y[3]=4-2=2, y[4]=5-3=2
    assert(v4[0] == 1.0);
    assert(v4[1] == 2.0);
    assert(v4[2] == 2.0);
    assert(v4[3] == 2.0);
    assert(v4[4] == 2.0);

    // Test single-element vector with order 0
    std::vector<double> v5 = {42.0};
    assert(applyDelayDifferenceFilter(v5, 0) == true);
    assert(v5[0] == 42.0);

    // Test empty vector returns false
    std::vector<double> v6;
    assert(applyDelayDifferenceFilter(v6, 0) == false);

    // Test negative order returns false
    std::vector<double> v7 = {1.0, 2.0};
    assert(applyDelayDifferenceFilter(v7, -1) == false);
    // ensure vector unchanged on failure
    assert(v7[0] == 1.0);
    assert(v7[1] == 2.0);

    // Test larger delay: order 4 => delay = 3
    std::vector<double> v8 = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0};
    assert(applyDelayDifferenceFilter(v8, 4) == true);
    // y[0]=1, y[1]=2, y[2]=3, y[3]=4-1=3, y[4]=5-2=3, y[5]=6-3=3
    assert(v8[0] == 1.0);
    assert(v8[1] == 2.0);
    assert(v8[2] == 3.0);
    assert(v8[3] == 3.0);
    assert(v8[4] == 3.0);
    assert(v8[5] == 3.0);

    return 0;
}
#include <vector>
#include <cstddef>

/**
 * @brief Apply a delay-and-subtract filter in place.
 *
 * For a vector of real samples, given a filter order parameter,
 * we compute a delay according to the rule:
 *   if order is even, delay = order/2 + 1
 *   if order is odd,  delay = (order+1)/2
 * The output for index i is:
 *   y[i] = x[i] - x[i - delay]   if i >= delay
 *   y[i] = x[i]                  otherwise
 *
 * The operation modifies the vector in place.
 *
 * @param samples  The vector to be filtered (modified in place).
 * @param order    Non-negative integer filter order.
 * @return true on success, false if the vector is empty or order is negative.
 */
bool applyDelayDifferenceFilter(std::vector<double>& samples, int order) {
    if (samples.empty() || order < 0) {
        return false;
    }

    // Compute delay from order according to parity rule.
    int delay = (order % 2 == 0) ? (order / 2 + 1) : ((order + 1) / 2);
    // delay is guaranteed >= 1 because order >= 0.

    // Iterate from the end towards the beginning so that samples[i - delay]
    // is still the original value when we read it (since i - delay < i and
    // we haven't modified it yet when moving backwards).
    for (std::size_t i = samples.size(); i-- > 0; ) {
        if (i >= static_cast<std::size_t>(delay)) {
            samples[i] -= samples[i - static_cast<std::size_t>(delay)];
        }
        // else: leave sample unchanged.
    }

    return true;
}
// The key is to recognize that the operation is a simple delay-and-subtract filter. We compute a delay value from the given `order` according to the parity rule: `delay = order/2 + 1` for even, `delay = (order+1)/2` for odd. Then, for each position `i` in the vector, if `i < delay`, we keep the sample unchanged; otherwise we subtract the sample at position `i - delay`. In-place modification is straightforward because each output element depends only on an earlier (already unmodified if we process from front to back? Actually careful: if we process from left to right, the element at `i - delay` is earlier and we have already modified it. But the mathematically correct output uses the original input at `x[i - delay]`, so we must not overwrite the input before reading the needed earlier value. Since we process in increasing `i`, the value at `i - delay` has already been overwritten if we are modifying in place. To avoid that, we either process from right to left (because the dependency is on an earlier index, but if processing from right to left, the earlier index has not yet been modified because we are moving backwards) — wait, if we process from right to left, `i - delay` is smaller than `i`, and we have not visited it yet (since we go from high to low), so the original value is still intact. That works. Alternatively, we could copy the vector first, but that uses extra memory. The simpler approach is to iterate from the end backwards (from `size-1` down to `0`), so that when we read `x[i - delay]`, it is still the original value. Edge cases: empty vector returns false; negative order returns false; delay could be zero if order is -1? But negative order is rejected. Delay is always at least 1 for non-negative order because even order 0 gives delay = 1, odd order 1 gives delay=1. So no division by zero or infinite loops. Time complexity O(n) where n is the vector size, space complexity O(1) (only a few integers for delay and loop index). The function returns a bool to indicate success. We'll use `std::vector<double>` as the sample type, and a free function named `applyDelayDifferenceFilter`.
