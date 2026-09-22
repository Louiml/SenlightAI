/*
Write a C++ function `std::string decodeString(const std::string& compressed)` that takes a run-length encoded string like `"a3b2c10"` (where each character is followed by its repetition count, and counts can contain multiple digits) and returns the fully expanded string, e.g., `"aaabbcccccccccc"`. The input string is guaranteed to be non-empty and well-formed: it always alternates between a single lowercase letter (or any non-digit character) and a positive integer (≥1). The function must preserve the original order of characters and expand each run exactly as specified. If the input is empty or malformed, the behavior is undefined; you may assume valid input. The output string length may be large, so the solution should be efficient.
*/
#include <string>

// Expand a run-length encoded string like "a3b2c10" into "aaabbcccccccccc".
// The input is guaranteed to be well-formed: alternating non-digit characters
// and positive integers (possibly multi-digit).
std::string decodeString(const std::string& compressed) {
    std::string result;
    result.reserve(compressed.size() * 2); // heuristic to reduce reallocations

    size_t i = 0;
    while (i < compressed.size()) {
        // Current character to repeat
        char ch = compressed[i];
        ++i;

        // Parse the following digits to get the count (at least one digit exists)
        int count = 0;
        while (i < compressed.size() && compressed[i] >= '0' && compressed[i] <= '9') {
            count = count * 10 + (compressed[i] - '0');
            ++i;
        }

        // Append the character count times
        result.append(count, ch);
    }

    return result;
}
#include <cassert>
#include <string>

// The solution function is declared above; here we test it.
int main() {
    // Basic single-digit counts
    assert(decodeString("a1b1c1") == "abc");
    assert(decodeString("x3") == "xxx");
    assert(decodeString("a2b3") == "aabbb");

    // Multi-digit counts
    assert(decodeString("a10") == "aaaaaaaaaa");
    assert(decodeString("b12c1") == "bbbbbbbbbbbbc");

    // Mixed scenario with larger expansions
    assert(decodeString("a2b3c4") == "aabbbcccc");

    // Single character with count 1
    assert(decodeString("z1") == "z");

    // Longer pattern with repeating letters and multi-digit counts
    assert(decodeString("p2q10r1") == "ppqqqqqqqqqqr");

    // Counts can be large but the test uses a small expansion
    assert(decodeString("m5") == "mmmmm");

    // Ensures the order and counts are preserved
    assert(decodeString("a1b2c3d4") == "abbcccdddd");

    return 0;
}
// The main approach is a single pass over the compressed string. We iterate character by character: when we encounter a letter, we store it and then read the following digits to build the count (handling multi-digit counts by multiplying the accumulated value by 10 and adding each digit). After we have a `(character, count)` pair, we append the character `count` times to the result string. Edge cases include counts like `"10"` or `"100"` that require digit accumulation, and a compressed string that ends right after a letter (though a valid input always has a number after each letter). The time complexity is O(L + R), where L is the length of the compressed input and R is the length of the fully expanded output (since appending each character takes constant amortized time). The space complexity is O(R) for the result string, plus O(L) for the input itself if counted. The algorithm uses no extra auxiliary data structures beyond a loop and simple variables.
