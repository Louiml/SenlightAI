Write a C++ function `std::vector<unsigned int> radixSortIndices(const std::vector<float>& values)` that returns a vector of indices into the input vector such that the corresponding values are sorted in ascending numerical order. The indices must be 0-based, stable (equal values maintain their original relative order), and the function must use a radix sort on the unsigned integer bit representation of the floats. Handle positive, negative, zero, NaN, and infinities correctly according to IEEE 754: negative numbers sort before positive numbers, with more negative (larger magnitude) first, and NaN values should be placed at the end in ascending order relative to each other (so all NaN are after all finite values). The input vector must not be modified. If the input is empty, return an empty vector. The algorithm must use `O(n + k)` time and `O(n)` auxiliary space where `k` is the radix (fixed at 256).
#include <cassert>
#include <vector>
#include <cmath>

// The solution function is declared elsewhere (radixSortIndices).
// This test program verifies correctness for various cases.

int main() {
    // Basic positive and negative floats.
    {
        std::vector<float> values = {3.5f, -1.2f, 0.0f, 2.0f, -0.5f};
        auto idx = radixSortIndices(values);
        std::vector<float> sorted;
        for (auto i : idx) sorted.push_back(values[i]);
        assert((sorted == std::vector<float>{-1.2f, -0.5f, 0.0f, 2.0f, 3.5f}));
    }

    // Duplicate values should preserve original order (stable).
    {
        std::vector<float> values = {1.0f, 1.0f, 0.0f, 1.0f, 0.0f};
        auto idx = radixSortIndices(values);
        // Sorted order: 0,0,1,1,1 with original indices for duplicates.
        assert(idx.size() == 5);
        assert(idx[0] == 2);
        assert(idx[1] == 4);
        assert(idx[2] == 0);
        assert(idx[3] == 1);
        assert(idx[4] == 3);
    }

    // Single element.
    {
        std::vector<float> values = {42.0f};
        auto idx = radixSortIndices(values);
        assert(idx.size() == 1 && idx[0] == 0);
    }

    // Empty input.
    {
        std::vector<float> values;
        auto idx = radixSortIndices(values);
        assert(idx.empty());
    }

    // All negative numbers sorted from most negative to least negative.
    {
        std::vector<float> values = {-3.0f, -10.0f, -1.5f, -0.0f};
        auto idx = radixSortIndices(values);
        std::vector<float> sorted;
        for (auto i : idx) sorted.push_back(values[i]);
        assert((sorted == std::vector<float>{-10.0f, -3.0f, -1.5f, -0.0f}));
    }

    // NaN handling: all NaN at the end in original relative order.
    {
        std::vector<float> values = {1.0f, NAN, -2.0f, NAN, 0.0f};
        auto idx = radixSortIndices(values);
        // Sorted finite: -2,0,1; then NaN at positions 1 and 3 in original order.
        assert(idx.size() == 5);
        assert(idx[0] == 2); // -2.0f
        assert(idx[1] == 4); // 0.0f
        assert(idx[2] == 0); // 1.0f
        assert(idx[3] == 1); // first NaN
        assert(idx[4] == 3); // second NaN
    }

    // Mixed large values, including infinities.
    {
        std::vector<float> values = {INFINITY, -INFINITY, 0.0f, -0.0f, 100.5f, -100.5f};
        auto idx = radixSortIndices(values);
        std::vector<float> sorted;
        for (auto i : idx) sorted.push_back(values[i]);
        assert((sorted == std::vector<float>{-INFINITY, -100.5f, -0.0f, 0.0f, 100.5f, INFINITY}));
    }

    // Random-like input with many duplicates.
    {
        std::vector<float> values = {0.0f, 0.0f, -0.0f, 1.0f, -1.0f, 1.0f, -1.0f};
        auto idx = radixSortIndices(values);
        std::vector<float> sorted;
        for (auto i : idx) sorted.push_back(values[i]);
        assert((sorted == std::vector<float>{-1.0f, -1.0f, -0.0f, 0.0f, 0.0f, 1.0f, 1.0f}));
    }

    return 0;
}
#include <vector>
#include <cstdint>
#include <cstring>
#include <cmath>
#include <algorithm>

// Returns indices into the input vector such that the values are sorted ascending.
// Uses a stable radix sort on a transformed unsigned representation of the floats.
// NaN values are placed at the end, preserving their original relative order.
std::vector<unsigned int> radixSortIndices(const std::vector<float>& values) {
    using Index = unsigned int;
    const size_t n = values.size();
    if (n == 0) {
        return {};
    }
    if (n > 0x7FFFFFFF) {
        // Not expected, but guard against overflow in index arithmetic.
        return {};
    }

    // Transform each float to an unsigned 32-bit key that preserves sort order.
    std::vector<uint32_t> keys(n);
    for (size_t i = 0; i < n; ++i) {
        float v = values[i];
        uint32_t bits;
        std::memcpy(&bits, &v, sizeof(bits));
        if (std::isnan(v)) {
            // Place all NaN at the very end.
            bits = 0xFFFFFFFFu;
        } else if (bits & 0x80000000u) {
            // Negative number: flip all bits to invert the magnitude ordering.
            bits = ~bits;
        } else {
            // Positive number: set the sign bit to make it larger than any negative.
            bits |= 0x80000000u;
        }
        keys[i] = bits;
    }

    // Two buffers for indices; we alternate passes.
    std::vector<Index> ranksA(n);
    std::vector<Index> ranksB(n);
    Index* current = ranksA.data();
    Index* next = ranksB.data();
    // Initially invalid: we will fill next on first pass.
    bool currentValid = false;

    // Histogram and offsets for 8-bit radix passes.
    uint32_t histogram[256];
    uint32_t* links[256];

    for (int pass = 0; pass < 4; ++pass) {
        // Reset histogram.
        std::fill(histogram, histogram + 256, 0u);

        // Count occurrences of each byte value in this pass.
        const int shift = pass * 8;
        for (size_t i = 0; i < n; ++i) {
            uint32_t key = keys[i];
            unsigned char byte = static_cast<unsigned char>((key >> shift) & 0xFF);
            ++histogram[byte];
        }

        // Compute starting offsets for each bucket.
        links[0] = next;
        for (int i = 1; i < 256; ++i) {
            links[i] = links[i - 1] + histogram[i - 1];
        }

        // Distribute indices into the next buffer.
        if (!currentValid) {
            // First pass: use sequential indices.
            for (size_t i = 0; i < n; ++i) {
                uint32_t key = keys[i];
                unsigned char byte = static_cast<unsigned char>((key >> shift) & 0xFF);
                *links[byte]++ = static_cast<Index>(i);
            }
        } else {
            // Subsequent passes: use previously sorted indices.
            for (size_t i = 0; i < n; ++i) {
                Index idx = current[i];
                uint32_t key = keys[idx];
                unsigned char byte = static_cast<unsigned char>((key >> shift) & 0xFF);
                *links[byte]++ = idx;
            }
        }

        // Swap current and next buffers.
        std::swap(current, next);
        currentValid = !currentValid;
    }

    // After the loop, current holds the sorted indices.
    std::vector<Index> result(current, current + n);
    return result;
}
// The core idea is to transform each IEEE 754 float into an unsigned 32-bit integer that preserves the correct sort order when interpreted as unsigned. For a float `f`, its bit pattern `bits` is read via `memcpy`. To map the float ordering to unsigned ordering: if `bits` has the sign bit set (i.e., `bits & 0x80000000` is non-zero, meaning the float is negative), then to get the correct order we must flip all bits (complement) so that more negative numbers (which have larger exponent and mantissa patterns) become smaller unsigned values. If the sign bit is clear (positive float), we set the sign bit to 1 (i.e., `bits | 0x80000000`) so that all positive numbers become larger unsigned values than all negative numbers. This mapping is monotonic: for any two floats `a <= b`, the mapped unsigned integer of `a` is `<=` that of `b` in unsigned order, and for equal floats the mapping is identical. After this transformation, we run a least-significant-byte-first radix sort on the mapped integers to produce stable indices. The stability is crucial because input may contain duplicate values and we must preserve original order. After sorting, the indices are returned. Edge cases: NaN has a bit pattern with exponent all ones and mantissa non-zero. The mapping still works: NaN with sign bit 0 becomes a large positive unsigned, and NaN with sign bit 1 (sign bit set) flips to a small unsigned? Actually, both NaN encodings (positive and negative sign) are treated uniformly: we always flip bits if sign bit is set, so negative NaN becomes a small unsigned value, and positive NaN becomes a large unsigned value. This would place negative NaN before all finite numbers, which is incorrect. To handle NaN correctly, we detect NaN before mapping: `std::isnan(v)`. For all NaN values, we assign a sentinel mapping that places them at the very end, and we further order them by their original bit pattern (unsigned) to ensure stability relative to each other. We can achieve this by first detecting NaN and assigning them a special high value (e.g., `UINT32_MAX`) plus we need to differentiate between multiple NaN so we use the original bit pattern but with sign bit set to 1 and forced exponent to all ones, but we also flip the mantissa? Simpler: for each float, if `std::isnan`, map to `0xFFFFFFFF` (which is the highest unsigned value). Since all NaN map to the same value, the radix sort will keep them in original order (stable), which satisfies "NaN placed at the end in ascending order relative to each other" because if the input order of NaN values is, say, NaN1, NaN2, then the output will keep that same order, which is ascending in original order. So we don't need to sort NaN by magnitude; just place them all at the end. The mapping for finite floats: `uint32_t bits; memcpy(&bits, &v, sizeof(bits)); if (bits & 0x80000000) bits = ~bits; else bits |= 0x80000000;`. For NaN, we override bits to `0xFFFFFFFF`. This ensures all NaN are at the very end after all finite values. The radix sort uses 4 passes (8 bits each), with a histogram of 256 counters per pass. We maintain two buffers of indices (size `n`) and alternate between them. The algorithm is: first pass over mapped values to build histogram for byte 0 (LSB), then compute cumulative offsets, then distribute indices into the other buffer. Repeat for bytes 1, 2, 3. After four passes, the output indices are in sorted order. Time complexity is \(O(4n)\) which is \(O(n)\), and space is \(O(n + 256)\) for the buffers and histogram. Because we map floats identically for equal values, stability is preserved through the pass.
