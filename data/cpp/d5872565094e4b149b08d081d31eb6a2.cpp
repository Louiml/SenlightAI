Write a C++ function `std::vector<uint32_t> radixSortIndices(const std::vector<float>& data)` that returns a vector of indices (as `uint32_t`) that would sort the input array of IEEE 754 single-precision floating-point values in ascending order. The function must implement a **radix sort** algorithm that operates on the raw bit representation of the floats, correctly handling negative numbers (which have the sign bit set, making their raw bits Larger than positive values when interpreted as unsigned integers) by flipping the bits for negative floats before sorting. The returned vector must contain the original indices such that `data[result[0]] ≤ data[result[1]] ≤ ... ≤ data[result[n-1]]`. The sort must be efficient: it should run in \(O(n)\) time on the number of values \(n\), with extra space proportional to \(n\) for the index arrays. You may assume the input contains at least one element, and all values are finite (no NaN or infinity). The function must not modify the input vector.
// The standard approach for radix-sorting floating-point numbers is to transform the bit pattern into a form where the standard unsigned-integer byte-wise ordering matches the desired float ordering. For positive floats (sign bit = 0), the raw unsigned integer representation already orders correctly (larger magnitude => larger bits). For negative floats (sign bit = 1), the raw bits are larger than any positive float because the sign bit is the MSB, so we need to invert the ordering: for a negative float, the larger the magnitude, the smaller the float, and its raw bits are smaller when we flip all bits. The conventional trick is:
// 1. Extract the 32-bit IEEE representation `u`.
// 2. If the sign bit is set (i.e., `u & 0x80000000`), flip all bits (`u = ~u`). Otherwise, set the sign bit (`u = u | 0x80000000`). This maps the float's order to the order of the resulting unsigned integer.
// 3. Then, perform an LSD radix sort on these transformed 32-bit values using 4 passes of 8 bits each (256 buckets per pass). Since the transformed integers are non-negative and correctly ordered as unsigned, the radix sort yields the correct ordering of the original floats.
//
// The implementation in the solution below does not explicitly pre-transform each float into a separate array; instead, it computes the transformed byte on the fly during each pass, avoiding an extra array. It uses two index arrays `indices` and `temp` of size `n`, and a histogram of size 256 per pass. The algorithm:
// - Initialize `indices` to `[0,1,...,n-1]`.
// - For each byte position `p` from 0 (LSB) to 3 (MSB):
//   - Count how many times each transformed byte value occurs at position `p` (by iterating over all `i` in `indices` and computing the transformed byte for `data[i]`).
//   - Compute prefix sums to get starting offsets for each bucket.
//   - Scatter the indices into `temp` in bucket order (stable sort).
//   - Swap `indices` and `temp`.
// After 4 passes, `indices` contains the indices in sorted order. The transform for a given float `x` and byte position `p` is:
// ```
// uint32_t u = *reinterpret_cast<const uint32_t*>(&x);
// uint32_t t = (u & 0x80000000) ? ~u : (u | 0x80000000);
// uint8_t b = (t >> (p*8)) & 0xFF;
// ```
// This transform ensures that the order of `t` matches the order of `x`. Handling negative numbers is naturally included. Edge cases: all values positive, all negative, mixed, duplicates—all are correctly handled by the radix sort. Time complexity is \(O(4n) = O(n)\), and space complexity is \(O(n + 256)\) for the two index arrays and the histograms.
#include <cstdint>
#include <vector>
#include <cstring>

// Returns indices that sort 'data' in ascending order using radix sort.
// Assumes data is non-empty and contains only finite floats.
std::vector<uint32_t> radixSortIndices(const std::vector<float>& data) {
    const uint32_t n = static_cast<uint32_t>(data.size());

    // Initial indices: 0, 1, ..., n-1
    std::vector<uint32_t> indices(n);
    for (uint32_t i = 0; i < n; ++i) indices[i] = i;

    // Temporary buffer for each pass
    std::vector<uint32_t> temp(n);

    // Histogram for each of 4 passes (256 buckets each)
    std::vector<uint32_t> histogram(256);
    std::vector<uint32_t> offsets(256);

    // LSD radix sort: 4 passes of 8 bits
    for (uint32_t pass = 0; pass < 4; ++pass) {
        // Clear histogram
        std::fill(histogram.begin(), histogram.end(), 0);

        // Count occurrences of each transformed byte at this pass
        for (uint32_t i = 0; i < n; ++i) {
            uint32_t u;
            std::memcpy(&u, &data[indices[i]], sizeof(uint32_t));
            // Transform: flip all bits for negatives, set sign bit for positives
            uint32_t t = (u & 0x80000000u) ? ~u : (u | 0x80000000u);
            uint8_t byte = static_cast<uint8_t>((t >> (pass * 8)) & 0xFF);
            ++histogram[byte];
        }

        // Compute offsets (prefix sums)
        uint32_t sum = 0;
        for (uint32_t i = 0; i < 256; ++i) {
            offsets[i] = sum;
            sum += histogram[i];
        }

        // Scatter indices into temp based on the byte
        for (uint32_t i = 0; i < n; ++i) {
            uint32_t u;
            std::memcpy(&u, &data[indices[i]], sizeof(uint32_t));
            uint32_t t = (u & 0x80000000u) ? ~u : (u | 0x80000000u);
            uint8_t byte = static_cast<uint8_t>((t >> (pass * 8)) & 0xFF);
            temp[offsets[byte]++] = indices[i];
        }

        // Swap for next pass
        indices.swap(temp);
    }

    return indices;
}
#include <cassert>
#include <vector>
#include <cstdint>
#include <cmath>

// Include the solution function here (or link appropriately)
// For brevity, assume the function is defined above.

int main() {
    // Test 1: simple positive floats
    {
        std::vector<float> data = {3.5f, 1.2f, 2.8f, 0.1f};
        auto idx = radixSortIndices(data);
        assert(idx.size() == 4);
        assert(data[idx[0]] == 0.1f);
        assert(data[idx[1]] == 1.2f);
        assert(data[idx[2]] == 2.8f);
        assert(data[idx[3]] == 3.5f);
    }

    // Test 2: negative and positive mixed
    {
        std::vector<float> data = {-1.0f, 0.5f, -2.5f, 3.0f, -0.0f};
        auto idx = radixSortIndices(data);
        // Sorted: -2.5, -1.0, -0.0, 0.5, 3.0
        assert(data[idx[0]] == -2.5f);
        assert(data[idx[1]] == -1.0f);
        assert(std::signbit(data[idx[2]])); // -0.0
        assert(data[idx[2]] == -0.0f);
        assert(data[idx[3]] == 0.5f);
        assert(data[idx[4]] == 3.0f);
    }

    // Test 3: all negative
    {
        std::vector<float> data = {-3.0f, -1.0f, -2.0f};
        auto idx = radixSortIndices(data);
        assert(data[idx[0]] == -3.0f);
        assert(data[idx[1]] == -2.0f);
        assert(data[idx[2]] == -1.0f);
    }

    // Test 4: duplicates
    {
        std::vector<float> data = {2.0f, 2.0f, 1.0f, 2.0f};
        auto idx = radixSortIndices(data);
        assert(data[idx[0]] == 1.0f);
        for (int i = 1; i < 4; ++i) assert(data[idx[i]] == 2.0f);
    }

    // Test 5: single element
    {
        std::vector<float> data = {42.0f};
        auto idx = radixSortIndices(data);
        assert(idx.size() == 1);
        assert(idx[0] == 0);
    }

    // Test 6: large values and subnormals
    {
        std::vector<float> data = {1.0e-45f, -1.0e-45f, 1.0e38f, -1.0e38f};
        auto idx = radixSortIndices(data);
        assert(data[idx[0]] == -1.0e38f);
        assert(data[idx[1]] == -1.0e-45f);
        assert(data[idx[2]] == 1.0e-45f);
        assert(data[idx[3]] == 1.0e38f);
    }

    // Test 7: verify index correctness (not just values)
    {
        std::vector<float> data = {10.0f, -5.0f, 7.0f, -5.0f, 0.0f};
        auto idx = radixSortIndices(data);
        std::vector<float> sorted;
        for (auto i : idx) sorted.push_back(data[i]);
        assert(sorted[0] == -5.0f && sorted[1] == -5.0f);
        assert(sorted[2] == 0.0f);
        assert(sorted[3] == 7.0f);
        assert(sorted[4] == 10.0f);
        // Ensure all indices are unique
        std::vector<bool> seen(data.size(), false);
        for (auto i : idx) { assert(!seen[i]); seen[i] = true; }
    }

    return 0;
}
