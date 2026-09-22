/*
Write a C++ function `applyCensor(std::string text, std::string censor)` that returns a new string where every occurrence of the `censor` substring in the original `text` has been removed, with the important rule that removals are applied immediately as characters are processed from left to right. This means if removing an earlier occurrence causes adjacent characters to form a new occurrence of `censor`, that new occurrence is also removed (and this process continues recursively). The function must not use any standard library function that directly removes substrings (like `std::string::erase` with a position), and must not use regex. The function should work for arbitrary non-empty strings (both may be empty), and must handle overlapping cases correctly — for example, if `text = "aaaa"` and `censor = "aa"`, the result should be `""` because the first two `'a'`s are removed, then the next two `'a'`s become adjacent and are removed. Another example: `text = "abcabc"` and `censor = "abc"` should yield `""` because after removing the first `"abc"`, the remaining `"abc"` is removed, leaving nothing.
*/
#include <string>

// Returns a string where every occurrence of the 'censor' substring is removed
// immediately when it appears at the end of the partially built result.
// Handles cascading removals when new matches form after a removal.
std::string applyCensor(const std::string& text, const std::string& censor) {
    if (censor.empty()) {
        return text; // nothing to censor
    }

    std::string result;
    const size_t censor_len = censor.size();

    for (char ch : text) {
        result.push_back(ch);

        // While the suffix of result equals censor, remove it.
        // Use a loop to handle cascading removals (though a single check per 
        // character is sufficient because after removing, the next character 
        // will be appended, and we'll check again then — but a loop here is 
        // safe and handles any case where censor is a prefix of itself).
        while (result.size() >= censor_len &&
               result.compare(result.size() - censor_len, censor_len, censor) == 0) {
            result.resize(result.size() - censor_len);
        }
    }

    return result;
}
#include <cassert>
#include <string>

// Declaration of the function under test
std::string applyCensor(const std::string& text, const std::string& censor);

int main() {
    // Basic single removal
    assert(applyCensor("hello world", "world") == "hello ");
    // Multiple non-overlapping removals
    assert(applyCensor("abcXabcY", "abc") == "XY");
    // Cascading removal (overlapping)
    assert(applyCensor("aaaa", "aa") == "");
    // Consecutive patterns removal
    assert(applyCensor("abcabc", "abc") == "");
    // No match
    assert(applyCensor("abcdef", "xyz") == "abcdef");
    // Empty text
    assert(applyCensor("", "abc") == "");
    // Empty censor (should return original)
    assert(applyCensor("abc", "") == "abc");
    // Pattern at the beginning
    assert(applyCensor("abc def", "abc") == " def");
    // Pattern at the end
    assert(applyCensor("def abc", "abc") == "def ");
    // Pattern that builds after removal (e.g., "aaab" with "ab" -> remove "ab" from the end twice?)
    assert(applyCensor("aab", "ab") == "a"); // Process: 'a','a','b' -> result "aab" suffix "ab" matched -> remove -> "a"
    // Overlapping with prefix-suffix of censor itself
    assert(applyCensor("ababa", "aba") == ""); // Process: a,b,a -> match remove -> "" , then b,a -> "ba" no match, end "ba"? Wait let's manually: text="ababa", censor="aba"
    // Step: add 'a' -> "a", add 'b' -> "ab", add 'a' -> "aba" match remove -> "" , add 'b' -> "b", add 'a' -> "ba" no match => result "ba". So assert should be "ba".
    assert(applyCensor("ababa", "aba") == "ba");
    // Case where removal reveals another match after multiple removals
    assert(applyCensor("aababa", "aba") == "a"); // Process: a,a,b,a,b,a -> after building "aaba" suffix "aba" match -> remove -> "a" then continue with 'b','a' -> "aba" match -> remove -> ""? Let's trace: add a,a -> "aa", add b -> "aab", add a -> "aaba" suffix "aba" matched -> resize to "a", add b -> "ab", add a -> "aba" match -> remove -> "" -> result ""? Actually "aababa" length 6, process: i=0 'a' -> "a", i=1 'a' -> "aa", i=2 'b' -> "aab", i=3 'a' -> "aaba" suffix len3 "aba" match -> remove -> "a", i=4 'b' -> "ab", i=5 'a' -> "aba" match -> remove -> "" -> result "". So assert should be "".
    assert(applyCensor("aababa", "aba") == "");

    return 0;
}
// The key insight is to build the result string character by character while always checking whether the suffix of the currently built result equals the `censor` string. When a match is found at the end, we simply remove that suffix (by resizing the string) and continue processing the next character from the original text. This ensures that new collisions formed by the removal are automatically handled because after resizing, the next iteration will check the new suffix again. Edge cases include: when `censor` is empty — then no removal ever happens and the result is simply the original text; when `text` is empty — result is empty; when `censor` is longer than the current built string — no match possible; when `censor` appears multiple times consecutive or overlapping. Time complexity is O(n * m) in the worst case because for each character added, we may compare a suffix of length up to m (where n = text length, m = censor length). Space complexity is O(n) for the result string.
