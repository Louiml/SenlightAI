// Write a standalone C++ function that accepts an `Eigen::ArrayXf` of floating-point values and a non-negative integer `n`, and returns a new `Eigen::ArrayXf` containing the first `n` elements of the input array, but with each of those elements multiplied by 2. If `n` is greater than the array size, the function should instead return a copy of the entire input array (i.e., treat `n` as if it were the array size). The function must not modify the input array. You may assume the input array is non-empty, but `n` can be 0, in which case the returned array must be empty (size 0). Your solution must use Eigen’s block operations (`head`, `segment`, or similar) and must be correct for any valid `Eigen::ArrayXf` input.

The core idea is to leverage Eigen’s block operations to avoid copying the entire array unnecessarily. Since the task requires returning a new array (not a view), we must explicitly create a new `Eigen::ArrayXf` object. The logic: first, determine the effective number of elements to process as `k = std::min(n, static_cast<int>(v.size()))`. Then, create a copy of the input array. After that, use the `head(k)` block on the copy to multiply those first `k` elements by 2 (using `*= 2`). If `k == 0`, `head(0)` is valid and multiplies nothing, so the copy remains unchanged. If `n >= v.size()`, then `k == v.size()`, so we multiply all elements — the entire array is affected, which matches the specification. The solution is straightforward and has time complexity `O(m)` where `m` is the size of the input array (due to copying), and space complexity `O(m)` for the returned array. Edge cases: empty input is not allowed by task, but if it were, `min(0, 0)` would be 0 and `head(0)` works. Also note that `v.size()` returns `Eigen::Index` (a signed type), so careful with `n` being an `int` — casting to `Eigen::Index` avoids sign mismatch warnings.

#include <Eigen/Dense>
#include <algorithm> // for std::min

// Return a copy of the input array, but with the first 'n' elements multiplied by 2.
// If n >= input.size(), all elements are multiplied by 2.
Eigen::ArrayXf doubleFirstNElements(const Eigen::ArrayXf& v, int n) {
    // Clamp n to the valid range [0, v.size()]
    const Eigen::Index k = std::min(static_cast<Eigen::Index>(n), v.size());

    // Make a copy of the input array, then modify the first k elements.
    Eigen::ArrayXf result = v;
    result.head(k) *= 2.0f;  // Multiply the first k elements in place.

    return result;
}

#include <Eigen/Dense>
#include <cassert>

// Forward declaration for testing (already defined in solution)
Eigen::ArrayXf doubleFirstNElements(const Eigen::ArrayXf& v, int n);

int main() {
    // Test basic case: n < size
    Eigen::ArrayXf v1(6);
    v1 << 1, 2, 3, 4, 5, 6;
    Eigen::ArrayXf r1 = doubleFirstNElements(v1, 3);
    assert(r1.size() == 6);
    assert(r1(0) == 2 && r1(1) == 4 && r1(2) == 6);
    assert(r1(3) == 4 && r1(4) == 5 && r1(5) == 6);

    // Test n == size
    Eigen::ArrayXf v2(3);
    v2 << 1, 2, 3;
    Eigen::ArrayXf r2 = doubleFirstNElements(v2, 3);
    assert(r2(0) == 2 && r2(1) == 4 && r2(2) == 6);

    // Test n > size
    Eigen::ArrayXf r3 = doubleFirstNElements(v2, 10);
    assert(r3(0) == 2 && r3(1) == 4 && r3(2) == 6);

    // Test n == 0 → returns unchanged copy
    Eigen::ArrayXf v4(2);
    v4 << 5, 7;
    Eigen::ArrayXf r4 = doubleFirstNElements(v4, 0);
    assert(r4.size() == 2);
    assert(r4(0) == 5 && r4(1) == 7);

    // Test that original input is not modified
    Eigen::ArrayXf v5(4);
    v5 << 1, 1, 1, 1;
    doubleFirstNElements(v5, 2);
    assert(v5(0) == 1 && v5(1) == 1 && v5(2) == 1 && v5(3) == 1);

    // Test negative numbers
    Eigen::ArrayXf v6(3);
    v6 << -1, -2, -3;
    Eigen::ArrayXf r6 = doubleFirstNElements(v6, 2);
    assert(r6(0) == -2 && r6(1) == -4 && r6(2) == -3);

    return 0;
}
