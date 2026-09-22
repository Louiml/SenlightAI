Write a C++ function that performs the SAXPY operation (single-precision `alpha * x + y`) on vectors of `float`, but only for the first `k` elements, where `k` is the smaller of the vector size `n` and a given limit `max_k`. The function must take a `const std::vector<float>&` for `x`, a `std::vector<float>&` for `y` (which is modified in place), a `float alpha`, and an unsigned integer `max_k`. It should update the first `k = min(n, max_k)` elements of `y` as `y[i] = alpha * x[i] + y[i]`, leaving any remaining elements of `y` untouched. The function must be named `saxpy_limited`, accept arguments in the order `(const std::vector<float>& x, std::vector<float>& y, float alpha, size_t max_k)`, and not return any value. Assume the input vectors are always the same size, but `max_k` may be zero or larger than the vector size. Use `size_t` for the loop index to avoid signed/unsigned mismatches, and add `const` to any parameter that is not modified.

// The solution iterates over the first `k = std::min(x.size(), max_k)` indices. For each index `i` in that range, we perform the fused multiply-add operation `y[i] = alpha * x[i] + y[i]`. The main edge case is when `max_k` is zero—then the loop should not execute at all, leaving `y` unchanged. Another edge case is when `max_k` exceeds the vector size; we must not access out-of-bounds elements, so we clamp to `x.size()`. Since both vectors are guaranteed to have the same size (per the specification), we can use either `x.size()` or `y.size()`; using `x.size()` is fine because we only read from `x`. The time complexity is `O(k)` where `k = min(n, max_k)`, and space complexity is `O(1)` beyond the input vectors because we only use a loop index and a local `k` variable. The function modifies `y` in place, so no extra vector allocation is needed.

#include <cstddef>
#include <vector>
#include <algorithm>

// Performs y[i] = alpha * x[i] + y[i] for the first k elements,
// where k = min(x.size(), max_k). Leaves elements beyond k unchanged.
void saxpy_limited(const std::vector<float>& x, std::vector<float>& y, float alpha, size_t max_k) {
    const size_t k = std::min(x.size(), max_k);
    for (size_t i = 0; i < k; ++i) {
        y[i] = alpha * x[i] + y[i];
    }
}

#include <cassert>
#include <vector>

// Function under test (included here for standalone compilation)
void saxpy_limited(const std::vector<float>& x, std::vector<float>& y, float alpha, size_t max_k);

int main() {
    // Basic case with all elements updated
    std::vector<float> x1 = {1.0f, 2.0f, 3.0f};
    std::vector<float> y1 = {10.0f, 20.0f, 30.0f};
    saxpy_limited(x1, y1, 2.0f, 3);
    assert((y1 == std::vector<float>{12.0f, 24.0f, 36.0f}));

    // max_k larger than vector size – all elements updated
    std::vector<float> x2 = {1.0f, 2.0f};
    std::vector<float> y2 = {0.0f, 0.0f};
    saxpy_limited(x2, y2, 0.5f, 100);
    assert((y2 == std::vector<float>{0.5f, 1.0f}));

    // max_k zero – nothing changes
    std::vector<float> x3 = {5.0f, 6.0f};
    std::vector<float> y3 = {1.0f, 2.0f};
    saxpy_limited(x3, y3, 3.0f, 0);
    assert((y3 == std::vector<float>{1.0f, 2.0f}));

    // max_k smaller than vector size – only first elements updated
    std::vector<float> x4 = {1.0f, 2.0f, 3.0f, 4.0f};
    std::vector<float> y4 = {1.0f, 1.0f, 1.0f, 1.0f};
    saxpy_limited(x4, y4, 10.0f, 2);
    assert((y4 == std::vector<float>{11.0f, 21.0f, 1.0f, 1.0f}));

    // Empty vector – no crash, no change
    std::vector<float> x5;
    std::vector<float> y5;
    saxpy_limited(x5, y5, 1.0f, 5);
    assert(y5.empty());

    // Negative alpha
    std::vector<float> x6 = {2.0f, 4.0f};
    std::vector<float> y6 = {10.0f, 20.0f};
    saxpy_limited(x6, y6, -1.5f, 2);
    assert((y6 == std::vector<float>{7.0f, 14.0f}));

    return 0;
}
