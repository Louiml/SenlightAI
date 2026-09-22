/*
Implement a C++ function `std::string zFunctionToString(const std::vector<int>& z_function)` that reconstructs a lexicographically smallest string over the lowercase English alphabet (a–z) whose Z-function exactly matches the given input. The input is guaranteed to be a valid Z-function of some non-empty string (i.e., `z[0]` equals the length of the string, `z[i] >= 0`, and consistency conditions hold). The function must return the reconstructed string. If multiple strings match, return the lexicographically smallest one (preferring 'a' when possible). The input will never be empty and all Z-function values are valid for the alphabet size 26 used.
*/
#include <string>
#include <vector>

// Reconstruct the lexicographically smallest string from its Z-function.
// z_function[0] must equal the string's length. All values are valid for a 26-letter alphabet.
std::string zFunctionToString(const std::vector<int>& z_function) {
    const int ALPHABET_SIZE = 26;
    std::string result = "a";  // First character is always 'a' for lexicographic minimality

    std::vector<bool> allowed(ALPHABET_SIZE, true);
    allowed[0] = false;  // Since z[1] cannot be 0 if it equals the first char? Actually, z[1] can be anything, but 'a' is often forbidden if z[i]==0. We'll manage dynamically.

    int known_pref_len = 0;  // How many positions ahead are already determined from previous Z-values
    int char_pos = 0;        // Offset in the prefix to copy from

    for (int i = 1; i < static_cast<int>(z_function.size()); ++i) {
        if (z_function[i] > known_pref_len) {
            // This Z-value extends further than what we already know.
            known_pref_len = z_function[i];
            // Reset allowed characters because the character after the known prefix must be different
            // from the character at that position in the prefix.
            allowed.assign(ALPHABET_SIZE, true);
            char_pos = 0;
        }

        if (known_pref_len > 0) {
            // We have a known character to copy.
            result += result[char_pos];
            // If this Z-value exactly equals the length we just covered and we are not at the very beginning,
            // then the next character (at position known_pref_len) is forbidden for the upcoming unknown.
            if (z_function[i] == known_pref_len && i > known_pref_len) {
                if (known_pref_len < static_cast<int>(result.size())) {
                    allowed[result[known_pref_len] - 'a'] = false;
                }
            }
            ++char_pos;
            --known_pref_len;
        } else {
            // known_pref_len == 0: we need a new character that is allowed.
            // 'a' is usually forbidden because if z[i]==0, the character at i differs from str[0].
            char chosen = 'b';  // Start from 'b' because 'a' is often not allowed
            // But we must check all from 0 to find the smallest allowed.
            for (int c = 0; c < ALPHABET_SIZE; ++c) {
                if (allowed[c]) {
                    chosen = static_cast<char>('a' + c);
                    break;
                }
            }
            result += chosen;
            // After choosing a new character, reset allowed for future (except that future z[i] may be >0).
            allowed.assign(ALPHABET_SIZE, true);
        }
    }

    return result;
}
#include <cassert>
#include <string>
#include <vector>

// Include the solution function here (or in a header).
std::string zFunctionToString(const std::vector<int>& z_function);

int main() {
    // Basic example: Z-function of "a" is [1].
    assert(zFunctionToString({1}) == "a");

    // Z-function of "aaa" is [3,2,1].
    assert(zFunctionToString({3,2,1}) == "aaa");

    // Z-function of "abacaba" is [7,0,1,0,3,0,1].
    assert(zFunctionToString({7,0,1,0,3,0,1}) == "abacaba");

    // Z-function of "abcab" is [5,0,0,2,0].
    assert(zFunctionToString({5,0,0,2,0}) == "abcab");

    // Z-function of "aaaa" is [4,3,2,1].
    assert(zFunctionToString({4,3,2,1}) == "aaaa");

    // Z-function of "ab" is [2,0].
    assert(zFunctionToString({2,0}) == "ab");

    // A more complex case: "aab" Z = [3,1,0].
    assert(zFunctionToString({3,1,0}) == "aab");

    // Edge case: string "z" but to be lexicographically minimal, we still get "a", but check that Z[0]=1 works.
    // Provided input is valid, so it will reconstruct to "a".
    assert(zFunctionToString({1}) == "a");

    // Verify with a round-trip for a random-ish string.
    std::string test = "banana";
    std::vector<int> z(test.size());
    z[0] = test.size();
    int l = 0, r = 0;
    for (int i = 1; i < (int)test.size(); ++i) {
        if (i <= r) z[i] = std::min(r - i + 1, z[i - l]);
        while (i + z[i] < (int)test.size() && test[z[i]] == test[i + z[i]]) ++z[i];
        if (i + z[i] - 1 > r) { l = i; r = i + z[i] - 1; }
    }
    // The reconstruction may return a lexicographically smaller string than "banana" 
    // (e.g., "anana"? Actually not valid), but it must have the same Z-function.
    std::string reconstructed = zFunctionToString(z);
    // Verify that the reconstructed string's Z-function matches the original.
    std::vector<int> z2(reconstructed.size());
    z2[0] = reconstructed.size();
    int l2 = 0, r2 = 0;
    for (int i = 1; i < (int)reconstructed.size(); ++i) {
        if (i <= r2) z2[i] = std::min(r2 - i + 1, z2[i - l2]);
        while (i + z2[i] < (int)reconstructed.size() && reconstructed[z2[i]] == reconstructed[i + z2[i]]) ++z2[i];
        if (i + z2[i] - 1 > r2) { l2 = i; r2 = i + z2[i] - 1; }
    }
    assert(z == z2);

    return 0;
}
// The problem is the inverse of computing a Z-function: given the Z-array, reconstruct a string. The Z-function property is that for each position `i > 0`, `z[i]` is the length of the longest common prefix between the string and the suffix starting at `i`. We construct the string greedily from left to right. We maintain a "known prefix" state: when at position `i`, we may have already determined some characters from previous Z-values. Specifically, we track `known_pref_len` (how many positions ahead are already known due to a previous Z-value that extends past `i`) and `char_pos` (the current offset in the already-built prefix to copy). If `z[i] > known_pref_len`, it means the Z-value at `i` extends further than what we already know, so we update `known_pref_len = z[i]` and reset the allowed-character set (because the next unknown character after the known prefix must differ from the character at that position in the prefix). Then, while `known_pref_len > 0`, we append `str[char_pos]` (copying from the beginning), decrement the counters, and if the Z-value exactly equals the extended length and `i > known_pref_len`, we mark the character at position `known_pref_len` (in the prefix) as forbidden for the next unknown character. When `known_pref_len == 0`, we need a new character that has not been forbidden; choose the smallest allowed letter (`'a'` is often forbidden because `z[i] == 0` implies the next character differs from `str[0]`). Edge cases include direct copying when `z[i]` is large, and the initial reset of allowed characters. Time complexity is O(n * alphabet_size) due to the character-choice loop, and space is O(n + alphabet_size). The alphabet size is constant 26, so it is O(n) time and space.
