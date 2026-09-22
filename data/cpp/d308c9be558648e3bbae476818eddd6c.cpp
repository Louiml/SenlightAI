/*
Implement a histogram frequency table class `Histogram` that supports a fixed integer range `[min, max)` (min inclusive, max exclusive) and stores counts for each integer value in that range. The class must provide: a constructor taking the range, `add(value, count=1)` to increment counts (values outside the range are clamped to the nearest bucket), `totalCount()` returning the total number of samples added, `mode()` returning the value with the highest count (if ties, the smallest value), and `mean()` returning the arithmetic mean of all added values as a `double`. The class must also validate the range in the constructor (if `min >= max`, throw `std::invalid_argument`). All methods must be `const` where appropriate, and memory must be managed correctly (no leaks).
*/
#include <stdexcept>
#include <vector>
#include <algorithm>

class Histogram {
public:
    // Construct histogram for values in [min, max) — min inclusive, max exclusive.
    Histogram(int min, int max) : min_(min), max_(max), total_count_(0) {
        if (min >= max) {
            throw std::invalid_argument("Histogram range must have min < max");
        }
        buckets_.assign(static_cast<size_t>(max - min), 0);
    }

    // Add 'count' samples of the given value. Values outside the range are clamped.
    void add(int value, long long count = 1) {
        int index;
        if (value < min_) {
            index = 0;
        } else if (value >= max_) {
            index = static_cast<int>(buckets_.size()) - 1;
        } else {
            index = value - min_;
        }
        buckets_[static_cast<size_t>(index)] += count;
        total_count_ += count;
    }

    // Total number of samples added.
    long long totalCount() const {
        return total_count_;
    }

    // Value with the highest count; ties resolve to the smallest value.
    // Returns min_ if no samples.
    int mode() const {
        if (total_count_ == 0) {
            return min_;
        }
        long long best_count = -1;
        int best_value = min_;
        for (size_t i = 0; i < buckets_.size(); ++i) {
            if (buckets_[i] > best_count) {
                best_count = buckets_[i];
                best_value = min_ + static_cast<int>(i);
            }
        }
        return best_value;
    }

    // Mean of all added values. Returns 0.0 if no samples.
    double mean() const {
        if (total_count_ == 0) {
            return 0.0;
        }
        long long sum = 0;
        for (size_t i = 0; i < buckets_.size(); ++i) {
            sum += static_cast<long long>(min_ + static_cast<int>(i)) * buckets_[i];
        }
        return static_cast<double>(sum) / static_cast<double>(total_count_);
    }

private:
    int min_;
    int max_;
    std::vector<long long> buckets_;
    long long total_count_;
};
#include <cassert>
#include <cmath>

int main() {
    // Basic usage
    Histogram h(0, 10);
    h.add(3);
    h.add(3);
    h.add(5);
    h.add(7, 2);
    assert(h.totalCount() == 5);
    assert(h.mode() == 3);
    assert(std::fabs(h.mean() - ((3.0*2 + 5 + 7.0*2) / 5.0)) < 1e-9);

    // Clamping at lower edge
    Histogram h2(0, 5);
    h2.add(-100);
    h2.add(-1);
    h2.add(0);
    assert(h2.totalCount() == 3);
    assert(h2.mode() == 0);
    assert(std::fabs(h2.mean() - 0.0) < 1e-9);

    // Clamping at upper edge
    Histogram h3(0, 5);
    h3.add(5);
    h3.add(100);
    h3.add(4);
    assert(h3.totalCount() == 3);
    assert(h3.mode() == 4); // both 4 and 5 have count 1; 4 is smaller
    assert(std::fabs(h3.mean() - ((4.0 + 5.0 + 4.0) / 3.0)) < 1e-9);

    // Empty histogram
    Histogram h4(-2, 3);
    assert(h4.totalCount() == 0);
    assert(h4.mode() == -2);
    assert(std::fabs(h4.mean() - 0.0) < 1e-9);

    // Tie-breaking mode (values 1 and 3 both have count 2)
    Histogram h5(0, 10);
    h5.add(1, 2);
    h5.add(3, 2);
    assert(h5.mode() == 1);

    // Invalid range throws
    bool threw = false;
    try {
        Histogram bad(5, 5);
    } catch (const std::invalid_argument&) {
        threw = true;
    }
    assert(threw);

    // Large counts and clamping duplicate
    Histogram h6(0, 2);
    h6.add(-5, 1000000);
    h6.add(0, 1);
    assert(h6.totalCount() == 1000001);
    assert(h6.mode() == 0); // 0 has 1000000 + 1 = 2000001 if not clamped? Actually 0 bucket gets 1000000 from -5 and 1 from 0.
    assert(h6.totalCount() == 1000001);
    assert(h6.mode() == 0);
    assert(std::fabs(h6.mean() - (0.0)) < 1e-9);

    // Single element
    Histogram h7(10, 20);
    h7.add(15);
    assert(h7.totalCount() == 1);
    assert(h7.mode() == 15);
    assert(std::fabs(h7.mean() - 15.0) < 1e-9);
}
// The solution uses a contiguous array of counts of size `(max - min)`, where index `i` corresponds to value `min + i`. The constructor validates the range and initializes all counts to zero. `add` clamps the input value to the valid range: if `value < min`, it increments bucket `0`; if `value >= max`, it increments the last bucket; otherwise increments bucket `value - min`. For `totalCount`, keep a running sum updated in `add`. `mode` scans all buckets from lowest to highest index, tracking the maximum count; because it scans in increasing order, ties resolve to the smallest value. `mean` computes the sum of `(value * count)` over all buckets and divides by total count, returning `0.0` if total count is zero. Time complexity: `add` is O(1), `mode` and `mean` are O(range size). Space complexity is O(range size) for the bucket array. Edge cases: empty range throws; large counts may overflow `int` but the task uses `long long` for counts and sums to be safe; clamping works correctly for values far outside the range.
