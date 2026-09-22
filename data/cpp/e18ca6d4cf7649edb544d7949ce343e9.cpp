// Write a C++ function that, given a positive integer `N` and a non-negative integer `K`, returns the lexicographically smallest string of length `N` consisting only of lowercase letters `'a'` and `'b'`, such that the total number of pairs of positions `(i, j)` with `i < j`, `v[i] == 'a'`, and `v[j] == 'b'` is exactly `K`. If no such string exists, return an empty string. The function signature is `std::string kthAbString(long long N, long long K)`. Assume `N` fits in a 64-bit signed integer and `K` is within `[0, N*(N-1)/2]` but may be impossible to achieve if `K` is too large relative to the number of `'a'`s possible. The string must be generated greedily by deciding each position from left to right.
// For a string with a single `'b'`, placing it after `K` leading `'a'`s yields exactly `K` pairs and is the lexicographically smallest when `K ≤ N-1`. For larger `K`, we must use exactly two `'b'`s. Suppose the string is `a^p b a^q b a^r` with `p+q+r = N-2`. Then each of the two `'b'`s contributes the number of `'a'`s before it: the first contributes `p`, the second contributes `p+q`. Thus total pairs `K = 2p + q`. To minimize lexicographic order, we maximize `p` (the number of leading `'a'`s). With `K` fixed, `p` must satisfy `p ≤ K/2` and `p ≤ N-2`, and also `q = K - 2p ≥ 0` and `p+q ≤ N-2` (so that `r ≥ 0`). The latter inequality translates to `p ≥ K - (N-2)`. Therefore the optimal `p` is the largest integer not exceeding `min(K/2, N-2)` that also satisfies `p ≥ K - (N-2)`. Since the guarantee ensures feasibility, such a `p` exists. The time complexity is `O(N)` for constructing the string, and space complexity is `O(N)` for the result.
#include <string>
#include <algorithm>

// Returns the lexicographically smallest string of length N with exactly K 'a'-before-'b' pairs.
std::string constructString(long long N, long long K) {
    if (N == 0) return "";
    if (K == 0) {
        return std::string(N, 'a');
    }
    // Case with only one 'b'
    if (K <= N - 1) {
        std::string result;
        result.append(K, 'a');
        result.push_back('b');
        result.append(N - K - 1, 'a');
        return result;
    }
    // Case with two 'b's
    long long lower = K - (N - 2);
    if (lower < 0) lower = 0;
    long long upper = std::min(K / 2, N - 2);
    long long p = upper;
    if (p < lower) {
        // Not feasible under guarantee; try to adjust downward
        p = lower;
    }
    long long q = K - 2 * p;
    long long r = N - 2 - p - q;
    if (p < 0 || q < 0 || r < 0) {
        return "";
    }
    std::string result;
    result.append(p, 'a');
    result.push_back('b');
    result.append(q, 'a');
    result.push_back('b');
    result.append(r, 'a');
    return result;
}
#include <cassert>
#include <string>

// Declare the function to test
std::string constructString(long long N, long long K);

int main() {
    assert(constructString(1, 0) == "a");
    assert(constructString(5, 0) == "aaaaa");
    assert(constructString(5, 3) == "aaaba");
    assert(constructString(5, 4) == "aaaab");
    assert(constructString(5, 5) == "aabab");
    assert(constructString(5, 6) == "aaabb");
    assert(constructString(6, 7) == "aaabab");
    assert(constructString(6, 8) == "aaaabb");
    assert(constructString(2, 0) == "aa");
    assert(constructString(2, 1) == "ab");
    return 0;
}
