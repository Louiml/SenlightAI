// Write a C++ function `long long countSubarraysWithoutRepeatedBits(const std::vector<unsigned int>& arr)` that, given a non-empty vector of 32-bit unsigned integers, returns the number of contiguous subarrays where no two numbers in the subarray share any set bit position (i.e., for every bit position 0..20, at most one number in the subarray has that bit set). For example, with input `{5, 3, 2}` (binary 101, 011, 010), the subarray `[5,3]` is invalid because both share bit 0, but `[5,2]` and `[3,2]` are valid. The result may be large, so use `long long`. Assume the vector size is at most 200,000 and each element fits in 21 bits (values < 2^21). Count only subarrays of length at least 1.
The problem is a classic two-pointer (sliding window) approach. Maintain a set (or boolean array of size 21) representing which bit positions are already used in the current window `[left, right)`. For each `left`, we try to extend `right` as far as possible while ensuring that the new number at `right` does not share any bit with the current set. If the new number has no overlap, we add its bits to the set, increment `right`, and add `(right - left)` to the answer (the number of valid subarrays starting at `left` and ending at `right-1`). If an overlap occurs, we break and do not advance `right`. After processing `left`, we remove the bits of `arr[left]` from the set to prepare for the next `left`. Edge cases: all numbers are zero (no bits set) → every subarray is valid; numbers may have the same bit but not necessarily; duplicates are allowed; the sliding window ensures we count each valid subarray exactly once. Time complexity is O(N * 21) due to the constant bit checks, space O(1) (or O(21) for the boolean array). Overall O(N) with a small constant.
#include <vector>
#include <set>

// Count contiguous subarrays where no two numbers share any set bit position (0..20).
long long countSubarraysWithoutRepeatedBits(const std::vector<unsigned int>& arr) {
    const int MAX_BITS = 21;
    const int n = static_cast<int>(arr.size());
    long long ans = 0;
    int right = 0;
    bool used[MAX_BITS] = {false};

    for (int left = 0; left < n; ++left) {
        while (right < n) {
            bool conflict = false;
            for (int b = 0; b < MAX_BITS; ++b) {
                if ((arr[right] & (1u << b)) && used[b]) {
                    conflict = true;
                    break;
                }
            }
            if (conflict) break;
            // Add bits of arr[right] to used
            for (int b = 0; b < MAX_BITS; ++b) {
                if (arr[right] & (1u << b)) used[b] = true;
            }
            ans += (right - left + 1);
            ++right;
        }
        // Remove bits of arr[left] from used
        for (int b = 0; b < MAX_BITS; ++b) {
            if (arr[left] & (1u << b)) used[b] = false;
        }
    }
    return ans;
}
#include <cassert>
#include <vector>

// Assume countSubarraysWithoutRepeatedBits is defined above (omitted here for brevity).

int main() {
    // Basic cases
    assert(countSubarraysWithoutRepeatedBits({5, 3, 2}) == 4); // [5],[3],[2],[5,2],[3,2] => 5? Let's compute: subarrays: [5] valid, [5,3] invalid, [5,3,2] invalid, [3] valid, [3,2] valid, [2] valid -> total 4? Wait: actually valid: [5],[3],[2],[3,2] = 4, [5,2] is not contiguous. So answer 4.
    assert(countSubarraysWithoutRepeatedBits({0, 0, 0}) == 6); // all subarrays valid
    assert(countSubarraysWithoutRepeatedBits({1, 2, 4}) == 6); // powers of two, all non-empty subarrays valid (3+2+1)
    assert(countSubarraysWithoutRepeatedBits({1, 1}) == 2); // [1] and [1] only
    assert(countSubarraysWithoutRepeatedBits({1, 2, 1}) == 5); // [1],[2],[1],[1,2],[2,1]? Wait [1,2] valid, [2,1] valid, [1,2,1] invalid -> total 5
    assert(countSubarraysWithoutRepeatedBits({3, 5, 6}) == 3); // each single element
    assert(countSubarraysWithoutRepeatedBits({2}) == 1);
    assert(countSubarraysWithoutRepeatedBits({1, 3, 2, 4}) == 6); // manual count: singles=4, pairs: [1,3] invalid, [3,2] valid, [2,4] valid, triples: [1,3,2] invalid, [3,2,4] invalid, all invalid -> total 4+2=6
    assert(countSubarraysWithoutRepeatedBits({}) == 0); // empty input but spec says non-empty, still test robust
    // Large input test: numbers with no overlapping bits (e.g., 1,2,4,8,...)
    std::vector<unsigned int> large;
    for (int i = 0; i < 200000; ++i) large.push_back(1u << (i % 21));
    assert(countSubarraysWithoutRepeatedBits(large) == 200000LL * (200000 + 1) / 2); // all subarrays valid because each bit appears at most once per window
    return 0;
}
