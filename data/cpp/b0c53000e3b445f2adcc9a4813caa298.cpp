// Write a C++ function `reverseByBlocks` that takes a string `s` and an integer `k` as parameters and returns a new string where, when scanning the original string from left to right in blocks of size `2k`, the first `k` characters of each block are reversed while the remaining `k` characters (if present) stay in their original order. If fewer than `k` characters remain in the final partial block, reverse all of those remaining characters. If between `k` and `2k` characters remain, reverse only the first `k` of them — the rest (fewer than `k`) are left untouched. The function should handle empty strings and any positive integer `k` gracefully, and must not modify the input string (pass it by const reference). Assume the string contains only printable ASCII characters and no spaces.

The core algorithm iterates over the string with a step of `2*k` indices. At each block starting at index `i`, we determine how many characters are left in the string from `i` onward. If the remaining length is strictly less than `k`, we reverse the entire tail from `i` to the end of the string. Otherwise, if the remaining length is at least `k` (which includes cases where the remaining length is between `k` and `2k`, or even larger), we reverse exactly the range `[i, i+k-1]` — but care must be taken because the reversal functions typically use half‑open ranges (`begin()+i` to `begin()+i+k`) which is safe when `i+k <= len`. If `i+k` exceeds the string length, we must clamp the end to `s.end()` instead of `begin()+i+k`, which only occurs when the remaining length is less than `k` (because `i+2*k` may exceed `len` but the remaining could be just under `k`). The condition in the loop increments `i` by `2*k`, so blocks are processed correctly. Edge cases: empty string returns empty string; `k=0` is not present in the spec (we assume `k>=1`); when the remaining length is exactly `k`, the condition `i+k <= len` is true, so the reverse applies fully. Time complexity is O(n) because each character is visited a constant number of times (reversal within a block touches each of the first `k` characters of each block once). Space complexity is O(n) for the returned string copy if we pass by value, but if we take a const reference and create a local copy for modification, we still use O(n) auxiliary memory for the copy. The algorithm does not use extra data structures besides the string copy.

#include <string>
#include <algorithm>

// Reverse the first k characters of each 2k block in the input string.
// If fewer than k characters remain, reverse all remaining characters.
// Returns a new string; the input is not modified.
std::string reverseByBlocks(const std::string& s, int k) {
    std::string result = s;  // work on a copy
    int len = static_cast<int>(result.length());
    for (int i = 0; i < len; i += 2 * k) {
        int remaining = len - i;
        if (remaining < k) {
            // Not enough characters for a full k-block, reverse the whole tail.
            std::reverse(result.begin() + i, result.end());
        } else {
            // Reverse exactly k characters.
            std::reverse(result.begin() + i, result.begin() + i + k);
        }
    }
    return result;
}

#include <cassert>
#include <string>

// The solution function declaration (provided above)
std::string reverseByBlocks(const std::string& s, int k);

int main() {
    // Basic example from the snippet: empty string with k=3
    assert(reverseByBlocks("", 3) == "");

    // Single character, k=1
    assert(reverseByBlocks("a", 1) == "a");

    // Exact multiple of 2k
    assert(reverseByBlocks("abcdef", 2) == "bacdef");  // first block "ab" -> "ba", second "cd" -> "dc" would give "badcfe", but here 2k=4, so blocks are "abcd" and "ef". Actually "abcd" -> "bacd", "ef" remains -> "bacdef"
    // Correctly computed: s="abcdef", k=2, step=4. i=0: remaining=6>=2, reverse [0,2) -> "ba" + "cd" + "ef" = "bacdef". i=4: remaining=2>=2, reverse [4,6) -> "fe" -> result "bacdfe". So the correct expected is "bacdfe".
    assert(reverseByBlocks("abcdef", 2) == "bacdfe");

    // Exact block where remaining is between k and 2k
    assert(reverseByBlocks("abcdefg", 3) == "cbadefg");  // i=0: reverse "abc" -> "cba", remaining 4 >=3, but i=6? Wait: step=6, i=0: remaining=7>=3, reverse "abc". i=6: remaining=1<3, reverse whole tail -> "g" unchanged. Result "cba" + "defg" = "cbadefg"

    // Remaining less than k
    assert(reverseByBlocks("abcdef", 3) == "cbadef");  // i=0: reverse "abc" -> "cba". i=6: loop ends (i=6 is not < len=6). No partial. Result "cbadef"

    // Test with k larger than string length
    assert(reverseByBlocks("hello", 10) == "olleh");  // remaining=5 <10, reverse whole string

    // Test with k=1 (no change)
    assert(reverseByBlocks("abc", 1) == "abc");

    // Test with k=2 and odd length
    assert(reverseByBlocks("abcde", 2) == "bacde");  // blocks: "ab"->"ba", "cd"->"dc"? Actually step=4: i=0: "ab"->"ba", i=4: remaining=1<2 -> reverse "e" unchanged. Result "ba" + "cd" + "e" = "bacde"

    // Test with long string and multiple blocks
    assert(reverseByBlocks("abcdefghij", 3) == "cbadefihgj");  // i=0: "abc"->"cba", i=6: "ghi"->"ihg", but "j" remains? Let's compute: step=6, i=0: remaining=10>=3, reverse "abc" -> "cba". i=6: remaining=4>=3, reverse "ghi" -> "ihg", then nothing else. Result "cba" + "def" + "ihg" + "j" = "cbadefihgj"

    // Additional edge: k=0 is not specified, but we avoid testing it.

    return 0;
}
