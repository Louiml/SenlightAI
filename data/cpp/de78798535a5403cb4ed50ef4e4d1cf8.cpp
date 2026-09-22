/*
Write a C++ function `countHomogenousSubstrings` that takes a non-empty string `s` consisting of lowercase English letters and returns the total number of contiguous substrings in which every character is the same (i.e., homogenous substrings), modulo `1,000,000,007`. A single character is considered a homogenous substring. For example, in `"abbcccaa"`, the homogenous substrings are: for `"a"` (positions 0-0) → 1, for `"bb"` → 3, for `"ccc"` → 6, for `"aa"` → 3, total = 13. The function must handle strings up to length 100,000 efficiently, and you must return the result as an `int`.
*/
#include <string>
#include <cstdint>

// Count all homogenous substrings (contiguous substrings with equal characters) modulo 1e9+7.
// A single character is a valid homogenous substring.
int countHomogenousSubstrings(const std::string& s) {
    if (s.empty()) {
        return 0;
    }

    const int MOD = 1000000007;
    std::size_t left = 0;
    std::size_t right = 1;
    long long total = 0;

    while (right < s.size()) {
        if (s[right] != s[right - 1]) {
            long long len = right - left;
            total = (total + (len * (len + 1)) / 2) % MOD;
            left = right;
        }
        ++right;
    }

    // Process the final run.
    long long len = right - left;
    total = (total + (len * (len + 1)) / 2) % MOD;

    return static_cast<int>(total);
}
#include <cassert>
#include <string>
#include "solution.h" // Assume the function is declared in this header

int main() {
    // Basic cases
    assert(countHomogenousSubstrings("a") == 1);
    assert(countHomogenousSubstrings("ab") == 2);          // "a" →1, "b" →1
    assert(countHomogenousSubstrings("aa") == 3);          // "a","a","aa" → 3
    assert(countHomogenousSubstrings("abb") == 4);         // "a"→1, "bb"→3 → total 4

    // Provided example
    assert(countHomogenousSubstrings("abbcccaa") == 13);
    // Another example with mixed runs
    assert(countHomogenousSubstrings("xyzz") == 5);        // "x"→1, "y"→1, "zz"→3 → total 5
    // All same characters
    assert(countHomogenousSubstrings("zzzzz") == 15);      // 5*6/2 = 15
    // Long alternating pattern
    assert(countHomogenousSubstrings("abc") == 3);         // "a","b","c" → 3
    // Large run to check modulo behavior (string of 100,000 'a's)
    std::string big(100000, 'a');
    assert(countHomogenousSubstrings(big) == 99986);       // 100000*100001/2 mod 1e9+7 = 5000050000 mod 1000000007 = 49998? Let's compute: 100000*100001=10,000,100,000; /2=5,000,050,000; mod 1,000,000,007 = 5,000,050,000 - 5*1,000,000,007 = 5,000,050,000 - 5,000,000,035 = 49,965? Actually 5,000,050,000 - 4*1,000,000,007 = 5,000,050,000 - 4,000,000,028 = 1,000,049,972 which is > MOD, so subtract again: 49,965. Let me compute correctly: 1,000,000,007*4 = 4,000,000,028; 5,000,050,000 - 4,000,000,028 = 1,000,049,972; now MOD = 1,000,000,007, so 1,000,049,972 - 1,000,000,007 = 49,965. So result is 49965. I'll correct that.)
    assert(countHomogenousSubstrings(big) == 49965);

    return 0;
}
// The key observation is that for any maximal run of identical characters of length `L`, the number of homogenous substrings entirely within that run is the sum of natural numbers from 1 to L, i.e., `L*(L+1)/2`. This holds because for length-1 substrings there are L of them, for length-2 there are L-1, ..., for length-L there is 1, summing to L + (L-1) + ... + 1 = L*(L+1)/2. The algorithm scans the string once, maintaining a `left` index marking the start of the current run and a `right` index that moves forward. When the character at `right` differs from the character at `left`, the run ends, and the contribution of that run is added to the total modulo the given modulus, then `left` is set to `right`. After the loop, the final run (from `left` to the end) is processed. Edge cases include a string of length 1, where the single run contributes `1*(2)/2 = 1`; an empty string is not allowed by the spec, but the code would handle it gracefully. The use of `long long` prevents overflow during multiplication of len*(len+1) before taking modulo. Time complexity is O(n) with a single pass, and space complexity is O(1) beyond the input string.
