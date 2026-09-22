// Write a C++ function that decodes a given `message` string using a `key` string according to the following substitution cipher rule: each distinct lowercase letter that appears in `key` (ignoring spaces) is mapped in order of first appearance to the alphabet letters 'a', 'b', 'c', ... (i.e., the first distinct letter in `key` maps to 'a', the second to 'b', etc.). The `message` string may contain lowercase letters and spaces; each lowercase letter in `message` is replaced by the character it maps to under this substitution, while spaces remain unchanged. Letters that appear in `message` but never appear in `key` are guaranteed not to occur, so you do not need to handle them. Return the fully decoded string.
The solution builds a mapping from each letter that appears in `key` (excluding spaces) to the next unused lowercase alphabet letter, in the order the letters first appear in `key`. A frequency array or hash map can track which letters have already been assigned; iterate through `key` left to right, and for every non-space character not yet assigned, assign it the current alphabet letter starting from 'a' and increment. After the mapping is complete, iterate through `message`; for each non-space character, replace it with its mapped letter, and for spaces, keep them as is. Edge cases: `key` might contain repeated letters (only the first occurrence matters), spaces in `key` are ignored, `message` may be empty (return empty), and `message` may have multiple spaces. Time complexity is O(n + m) where n is key length and m is message length, and space complexity is O(1) (fixed 26-size array or map) plus the output string.
#include <string>
#include <vector>

// Decode a message using a substitution key derived from the first occurrences of letters in key.
std::string decodeMessage(const std::string& key, const std::string& message) {
    std::vector<int> mapping(26, -1);
    char next_letter = 'a';

    for (char c : key) {
        if (c != ' ' && mapping[c - 'a'] == -1) {
            mapping[c - 'a'] = next_letter - 'a';
            ++next_letter;
        }
    }

    std::string result = message;
    for (char& c : result) {
        if (c != ' ') {
            c = static_cast<char>(mapping[c - 'a'] + 'a');
        }
    }

    return result;
}
#include <cassert>
#include <string>

// The solution function is declared above (include the header in actual usage).
int main() {
    assert(decodeMessage("the quick brown fox jumps over the lazy dog", "vkng") == "test");
    assert(decodeMessage("eljuxhpwnyrdgtqkviszcfmabo", "zwx hnfx lqantp mnoeius ycgk vcnjrdb") == "the five boxing wizards jump quickly");
    assert(decodeMessage("a b c", "a b c") == "a b c");
    assert(decodeMessage("abcdefghijklmnopqrstuvwxyz", "hello") == "hello");
    assert(decodeMessage("zyxwvutsrqponmlkjihgfedcba", "zyx") == "abc");
    assert(decodeMessage("abc", "a a a") == "a a a");
    assert(decodeMessage("", "") == "");
    assert(decodeMessage("single", "  s i n g l e  ") == "  a b c d e f  ");
    assert(decodeMessage("abc abc", "b") == "b");
    assert(decodeMessage("x y z", "x y z") == "a b c");
}
