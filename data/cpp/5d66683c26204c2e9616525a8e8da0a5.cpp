// Write a C++ function named `reverseStringEveryK` that takes a non-empty string `s` and an integer `k` (where `k >= 1`). The function should reverse the first `k` characters of every `2k`-character block of the string. More precisely: for each block starting at index `i = 0, 2k, 4k, ...`, reverse the substring from `i` up to but not including `i + k` if that many characters exist; if fewer than `k` characters remain from position `i` to the end, reverse all remaining characters. The function should return a new string with the transformations applied, leaving the original input unchanged.

// The core algorithm is a direct loop over the string with a step size of `2*k`. At each starting index `i`, we check the number of characters remaining: `s.length() - i`. If this remaining count is less than `k`, we reverse the substring from `i` to the end of the string. Otherwise, we reverse exactly the next `k` characters (from `i` to `i + k`). The loop increments `i` by `2*k` after each block, so every block is processed independently without overlap. Edge cases include: when `k` is larger than the string length, only one reversal happens (the whole string); when `k` equals the string length, the entire string is reversed once; when `k` is 1, every character is reversed individually, which yields no change to the string; and when the remaining characters are exactly `k` or more, the standard `k`-length reversal applies. The time complexity is O(n), where n is the length of the string, because each character is touched at most twice (once during a reversal and once during traversal). The space complexity is O(1) auxiliary, aside from the returned string which is a copy of the input (since we modify a local copy).

#include <string>
#include <algorithm>

// Reverse the first k characters of every 2k block in the string.
// If fewer than k characters remain in a block, reverse all remaining.
std::string reverseStringEveryK(const std::string& s, int k) {
    std::string result = s;
    const int n = static_cast<int>(result.length());
    const int blockSize = 2 * k;
    
    for (int i = 0; i < n; i += blockSize) {
        if (n - i < k) {
            // Reverse the remaining substring from i to end.
            std::reverse(result.begin() + i, result.end());
        } else {
            // Reverse exactly k characters starting at i.
            std::reverse(result.begin() + i, result.begin() + i + k);
        }
    }
    return result;
}

#include <cassert>
#include <string>

// Declaration of the function under test.
std::string reverseStringEveryK(const std::string& s, int k);

int main() {
    // Basic example from the original problem.
    assert(reverseStringEveryK("abcdefg", 2) == "bacdfeg");
    
    // k larger than the string length: reverse whole string.
    assert(reverseStringEveryK("abcd", 5) == "dcba");
    
    // k equals string length: reverse once.
    assert(reverseStringEveryK("hello", 5) == "olleh");
    
    // k equals 1: no change (each 1-character block reversed is itself).
    assert(reverseStringEveryK("abc", 1) == "abc");
    
    // Exactly k characters remaining in the last block.
    assert(reverseStringEveryK("abcdefgh", 3) == "cbadefhg");
    
    // Fewer than k characters remaining in the last block.
    assert(reverseStringEveryK("abcdefghij", 4) == "dcbaefghji");
    
    // Single-character string.
    assert(reverseStringEveryK("x", 2) == "x");
    
    // Empty string edge (though problem says non-empty, test defensively).
    assert(reverseStringEveryK("", 3) == "");
    
    return 0;
}
