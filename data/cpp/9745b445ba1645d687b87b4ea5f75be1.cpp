/*
Write a standalone C++ function that simulates the interleaved merge operation shown in the snippet. Given two input vectors `src0` and `src1` of equal length (assume even length for simplicity), and an output vector `dst`, the function must copy the elements in the pattern: first `src0[0]`, then `src1[0]`, then `src0[1]`, `src1[1]`, and so on, placing them contiguously into `dst`. The function must handle arbitrary element types via templates and must not allocate memory; it should write directly into a pre-allocated `dst` vector. The function signature should be `template<typename T> void interleaveMerge(const std::vector<T>& src0, const std::vector<T>& src1, std::vector<T>& dst)`. The operation must be safe: if `dst` is not large enough to hold `2 * src0.size()` elements, the behavior is undefined; the caller must ensure sufficient capacity. The function should be `const`‑correct: the source vectors are read-only. No return value is needed. The function must be self-contained with necessary headers.
*/
#include <vector>
#include <cstddef>

// Interleaves elements of src0 and src1 into dst: dst[0]=src0[0], dst[1]=src1[0], dst[2]=src0[1], ...
// Precondition: dst.size() >= 2 * min(src0.size(), src1.size()).
// If src0 and src1 have different sizes, only the first min(size) elements are interleaved.
template<typename T>
void interleaveMerge(const std::vector<T>& src0, const std::vector<T>& src1, std::vector<T>& dst) {
    const std::size_t count = (src0.size() < src1.size()) ? src0.size() : src1.size();
    for (std::size_t i = 0; i < count; ++i) {
        dst[2 * i]     = src0[i];
        dst[2 * i + 1] = src1[i];
    }
}
#include <cassert>
#include <vector>
#include <cstddef>

// The function under test (declared here for completeness, but assumed included).
template<typename T>
void interleaveMerge(const std::vector<T>& src0, const std::vector<T>& src1, std::vector<T>& dst) {
    const std::size_t count = (src0.size() < src1.size()) ? src0.size() : src1.size();
    for (std::size_t i = 0; i < count; ++i) {
        dst[2 * i]     = src0[i];
        dst[2 * i + 1] = src1[i];
    }
}

int main() {
    // Test 1: basic even-length arrays
    std::vector<int> a = {1, 3, 5};
    std::vector<int> b = {2, 4, 6};
    std::vector<int> out(6, -1);
    interleaveMerge(a, b, out);
    assert(out == std::vector<int>({1, 2, 3, 4, 5, 6}));

    // Test 2: empty inputs
    std::vector<double> empty1, empty2;
    std::vector<double> out2(4, 0.0);
    interleaveMerge(empty1, empty2, out2);
    assert(out2 == std::vector<double>({0.0, 0.0, 0.0, 0.0}));

    // Test 3: different sizes (only min length interleaved)
    std::vector<char> c1 = {'x', 'y', 'z'};
    std::vector<char> c2 = {'p'};
    std::vector<char> out3(6, '?');
    interleaveMerge(c1, c2, out3);
    assert(out3[0] == 'x' && out3[1] == 'p');
    // Remaining slots untouched (still '?')
    assert(out3[2] == '?' && out3[3] == '?' && out3[4] == '?' && out3[5] == '?');

    // Test 4: single-element vectors
    std::vector<int> s1 = {42};
    std::vector<int> s2 = {-1};
    std::vector<int> out4(2, 0);
    interleaveMerge(s1, s2, out4);
    assert(out4 == std::vector<int>({42, -1}));

    // Test 5: larger vectors with many values, verify pattern
    const std::size_t n = 1000;
    std::vector<int> left(n), right(n);
    for (std::size_t i = 0; i < n; ++i) {
        left[i] = static_cast<int>(i);
        right[i] = static_cast<int>(i + 10000);
    }
    std::vector<int> res(2 * n, -1);
    interleaveMerge(left, right, res);
    for (std::size_t i = 0; i < n; ++i) {
        assert(res[2*i] == static_cast<int>(i));
        assert(res[2*i+1] == static_cast<int>(i + 10000));
    }

    return 0;
}
// The core task is a classic even‑odd interleaving merge. The algorithm is straightforward: iterate over indices `i` from `0` to `src0.size()-1`, and for each `i` write `src0[i]` to `dst[2*i]` and `src1[i]` to `dst[2*i+1]`. This assumes both source vectors have the same size; if they differ, the behavior should be defined (e.g., take the minimum length or throw). In this task we assume equal sizes for simplicity, but we can add a check to handle unequal sizes by using the smaller size and leaving the rest of `dst` untouched. Edge cases: empty input vectors lead to no writes, which is safe. The loop index `2*i` and `2*i+1` must not overflow; since we rely on the caller providing a `dst` with sufficient size, no explicit bounds check is added. Time complexity is O(n) where n is the size of each source vector. Space complexity is O(1) auxiliary (no extra storage). The solution uses template generic code to support any type with copy semantics.
