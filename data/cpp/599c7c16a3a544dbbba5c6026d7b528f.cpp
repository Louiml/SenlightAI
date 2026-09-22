// Write a C++ function that takes two integers `n` and `k` (where `n >= 1` and `1 <= k <= 26`), and returns a string constructed as follows: start with a string containing the first `k` lowercase English letters in alphabetical order (i.e., "abc..." up to length `k`). Then, concatenate this initial string to itself repeatedly until the total length becomes exactly `n * k`. The final string should contain exactly `n` repetitions of the initial block. For example, for `n = 3` and `k = 2`, the initial block is "ab", and the result is "ababab". The function must handle the case where `n = 1` (returning just the initial block) and must work for `k = 26` (the full alphabet). The function should be named `repeatBlock` and accept parameters `(int n, int k)`. Ensure the function is `const`-correct where applicable.
// The solution is straightforward: build the base block by iterating from `0` to `k-1` and appending `char('a' + i)` to a string. Then, append this base block `n` times to the result string. Since `n * k` can be large (up to, say, `10^5` or more), we use `reserve` to avoid reallocations. The main edge case is when `n = 1` (just the base block) and when `k = 26` (base block is the full alphabet). The time complexity is O(n*k) because we append `n` copies of length `k`, and space complexity is O(n*k) for the returned string. No extra auxiliary space besides the result and base block (which is O(k)). The function should validate that `n >= 1` and `1 <= k <= 26` (though we can assume valid input for simplicity).
#include <string>
#include <cassert>

// Repeat the first k lowercase letters n times.
// Precondition: n >= 1, 1 <= k <= 26.
std::string repeatBlock(int n, int k) {
    // Build the base block: first k lowercase letters.
    std::string base;
    base.reserve(static_cast<size_t>(k));
    for (int i = 0; i < k; ++i) {
        base += static_cast<char>('a' + i);
    }

    // Build the final string by repeating the base block n times.
    std::string result;
    result.reserve(static_cast<size_t>(n) * static_cast<size_t>(k));
    for (int i = 0; i < n; ++i) {
        result += base;
    }
    return result;
}
#include <cassert>
#include <string>

// Assume repeatBlock is declared above.

int main() {
    // Test basic cases.
    assert(repeatBlock(1, 1) == "a");
    assert(repeatBlock(1, 3) == "abc");
    assert(repeatBlock(2, 2) == "abab");
    assert(repeatBlock(3, 2) == "ababab");
    assert(repeatBlock(4, 1) == "aaaa");
    assert(repeatBlock(2, 26) == "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz");
    assert(repeatBlock(5, 3) == "abcabcabcabcabc");
    assert(repeatBlock(10, 4) == "abcdabcdabcdabcdabcdabcdabcdabcdabcdabcd");
    // Test with n=1 and k=26.
    assert(repeatBlock(1, 26) == "abcdefghijklmnopqrstuvwxyz");
    // Test with larger n and k to ensure no issues.
    std::string s = repeatBlock(1000, 26);
    assert(s.length() == 26000);
    // Spot-check first and last characters.
    assert(s[0] == 'a');
    assert(s[25999] == 'z');
}
