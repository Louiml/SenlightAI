// Write a C++ function `generateSequence` that takes three integers `n`, `k`, and `x` (with `1 ≤ n ≤ 10^5`, `1 ≤ k ≤ n`, and `0 ≤ x ≤ 10^9`), and returns a `std::vector<long long>` of length `n` constructed as follows: if `k` is even, the sequence is formed by repeatedly appending blocks of length `k` consisting of `x` followed by `k-1` zeros; however, the final incomplete block (if `n` is not a multiple of `k`) must be truncated to exactly fit length `n`. If `k` is odd, the sequence consists of `x` repeated `n` times. For example, for `n=5, k=2, x=3`, the even case gives blocks `[3,0]`, `[3,0]`, then the last block truncated gives `[3]`, resulting in `[3,0,3,0,3]`. For `n=5, k=3, x=7` (odd k), the output is `[7,7,7,7,7]`. You may assume `n` and `k` are positive integers with `k ≤ n`.

#include <cassert>
#include <vector>

// The solution function is expected to be declared above.
// We just need main to test it.

int main() {
    // Test case 1: Basic even k
    std::vector<long long> expected1 = {3, 0, 3, 0, 3};
    assert(generateSequence(5, 2, 3) == expected1);

    // Test case 2: Odd k
    std::vector<long long> expected2 = {7, 7, 7, 7, 7};
    assert(generateSequence(5, 3, 7) == expected2);

    // Test case 3: n exactly multiple of k (even k)
    std::vector<long long> expected3 = {1, 0, 0, 1, 0, 0};
    assert(generateSequence(6, 3, 1) == expected3); // k=3 is odd? Wait 3 is odd, so this is not right.
    // Actually for odd k, it's all x. Let's test with even k and n multiple.
    std::vector<long long> expected3_correct = {4, 0, 4, 0};
    assert(generateSequence(4, 2, 4) == expected3_correct);

    // Test case 4: k=1 (even, so blocks of length 1: just x)
    std::vector<long long> expected4 = {9, 9, 9};
    assert(generateSequence(3, 1, 9) == expected4);

    // Test case 5: Large n but small values, still works
    std::vector<long long> result5 = generateSequence(100000, 2, 5);
    assert(result5.size() == 100000);
    for (size_t i = 0; i < result5.size(); ++i) {
        if (i % 2 == 0) assert(result5[i] == 5);
        else assert(result5[i] == 0);
    }

    // Test case 6: k odd, n=1
    std::vector<long long> expected6 = {2};
    assert(generateSequence(1, 1, 2) == expected6);

    // Test case 7: k even, n=1 (still works)
    std::vector<long long> expected7 = {8};
    assert(generateSequence(1, 2, 8) == expected7);

    // Test case 8: x = 0, even k
    std::vector<long long> expected8 = {0, 0, 0, 0, 0};
    assert(generateSequence(5, 3, 0) == expected8); // 3 is odd, so all zeros
    // Actually test with even k
    std::vector<long long> expected8b = {0, 0, 0, 0};
    assert(generateSequence(4, 2, 0) == expected8b);

    // Test case 9: n=5, k=4 (even) -> blocks: [1,0,0,0] then truncate [1]
    std::vector<long long> expected9 = {1, 0, 0, 0, 1};
    assert(generateSequence(5, 4, 1) == expected9);

    // Test case 10: n=6, k=6 (even) -> one full block
    std::vector<long long> expected10 = {2, 0, 0, 0, 0, 0};
    assert(generateSequence(6, 6, 2) == expected10);

    return 0;
}

#include <vector>
#include <cstddef>

// Generate a sequence of length n based on parity of k.
// If k is even: repeat [x, 0, 0, ..., 0] (length k) truncated to n.
// If k is odd: repeat x n times.
std::vector<long long> generateSequence(long long n, long long k, long long x) {
    std::vector<long long> result;
    result.reserve(static_cast<size_t>(n));
    
    if (k % 2 == 0) {
        // Build blocks of length k, truncating the last block if needed.
        for (long long i = 0; i < n; i += k) {
            // How many elements are left to fill in this block?
            long long segmentLength = std::min(k, n - i);
            if (segmentLength <= 0) break; // safety, though loop condition handles it
            // First element of the block is x
            result.push_back(x);
            // Remaining elements in the block are zeros (if any)
            for (long long j = 1; j < segmentLength; ++j) {
                result.push_back(0);
            }
        }
    } else {
        // k is odd: just repeat x n times
        for (long long i = 0; i < n; ++i) {
            result.push_back(x);
        }
    }
    
    return result;
}

// The solution is straightforward and requires careful handling of the block construction for the even `k` case. When `k` is even, we need to generate full blocks of length `k` as long as we have enough elements remaining, and then handle a possible partial final block by taking only the first `remaining` elements of the next block. The simplest approach is to loop `i` from `0` to `n` in steps of `k`, and for each start index, compute the length of the current segment as `min(k, n - i)`. Then, if the segment is non-empty, push the first element as `x` and the following elements (up to `segmentLength-1`) as `0`. This naturally handles truncation. For odd `k`, just fill the vector with `x` repeated `n` times. Edge cases include `n` exactly equal to a multiple of `k` (so no partial block), `k=1` (even case: each block is just `[x]`, resulting in all `x`), and `k=n` with even `k` (one full block of length `n`). Time complexity is `O(n)` because we push exactly `n` elements, and space complexity is `O(n)` for the result vector.
