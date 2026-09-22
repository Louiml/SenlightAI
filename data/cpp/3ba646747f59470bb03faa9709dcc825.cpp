Write a C++ function that, given two integers `n` and `k` where `n` is the length of a string and `k` is its total numeric value (where `'a'` = 1, `'b'` = 2, ..., `'z'` = 26), returns the lexicographically smallest string of length `n` with that exact numeric value. The function must handle any valid inputs where `1 <= n <= 10^5` and `n <= k <= 26 * n`. The returned string should contain only lowercase English letters. For example, with `n = 3` and `k = 27`, the smallest possible string is `"aay"` (since `"aax"` is too small and `"aaz"` = 28 exceeds the value).
The key insight is to build the string from right to left (least significant positions), placing as large a character as possible at each step while leaving enough room for the remaining positions to the left. Since we want the lexicographically smallest string, we want the earliest characters (leftmost) to be as small as possible, meaning `'a'` unless forced to be larger. Working from the rightmost position, for each position `i` (0-indexed from left), we compute the maximum value we can place there without making the remaining `(n-1-i)` positions impossible to fill (each must be at least 1). This maximum is `min(26, k - (n-1-i))`. If this maximum is 26, we place `'z'` and subtract 26 from `k`; otherwise we place the character corresponding to that maximum value and then set `k` to the remaining positions' requirement (which will be exactly `n-1-i`). This greedy approach works because any smaller character at a rightmost position would require a larger character to its left, making the string lexicographically larger (since a character at a more significant position has more weight than one at a less significant position). Edge cases include `k` exactly equal to `n` (all `'a'`), `k` equal to `26*n` (all `'z'`), and values where some middle positions must be `'z'` while earlier ones are `'a'`. Time complexity is `O(n)` because we iterate through all `n` positions once, and space complexity is `O(n)` for the result string (plus `O(1)` auxiliary).
#include <string>

// Return the lexicographically smallest string of length n with total numeric value k
// where 'a'=1, 'b'=2, ..., 'z'=26. Assumes 1 <= n <= 100000 and n <= k <= 26*n.
std::string smallestString(int n, int k) {
    std::string result(n, 'a');
    for (int i = 0; i < n; ++i) {
        // Maximum value we can put at position (n-1-i) without making left positions impossible
        int remainingPositions = n - 1 - i;
        int maxValue = k - remainingPositions; // must be at most 26, at least 1
        if (maxValue >= 26) {
            result[n - 1 - i] = 'z';
            k -= 26;
        } else {
            result[n - 1 - i] = static_cast<char>('a' + maxValue - 1);
            k = remainingPositions; // all left positions will be 'a'
        }
    }
    return result;
}
#include <cassert>
#include <string>
#include "solution.h" // Assume the above function is in this header; or paste directly

int main() {
    // Basic cases
    assert(smallestString(3, 27) == "aay");
    assert(smallestString(3, 28) == "aaz");
    assert(smallestString(3, 3) == "aaa");
    assert(smallestString(5, 5) == "aaaaa");
    assert(smallestString(5, 130) == "zzzzz"); // 5*26 = 130
    
    // Edge: exactly one character
    assert(smallestString(1, 1) == "a");
    assert(smallestString(1, 26) == "z");
    
    // Intermediate values
    assert(smallestString(2, 27) == "az");
    assert(smallestString(2, 52) == "zz");
    assert(smallestString(4, 50) == "aazz");
    
    // Larger example
    assert(smallestString(10, 100) == "aaaaaaaqzz");
    
    // Ensure all returned strings have correct length and sum
    for (int n = 1; n <= 10; ++n) {
        for (int k = n; k <= 26 * n; ++k) {
            std::string s = smallestString(n, k);
            assert(s.length() == static_cast<size_t>(n));
            int sum = 0;
            for (char c : s) sum += (c - 'a' + 1);
            assert(sum == k);
        }
    }
    
    return 0;
}
