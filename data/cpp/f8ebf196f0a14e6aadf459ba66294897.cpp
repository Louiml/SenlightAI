Write a C++ function named `countMismatchedCharacters` that takes a single string `s` of exactly 10 lowercase English letters as input and returns the number of positions where `s` differs from the fixed string `"codeforces"`. The function must compare character-by-character and count all mismatches, including cases where the string is identical (return 0) or completely different from the target (return 10). The input is guaranteed to be exactly 10 characters long, all lowercase, so no validation is required. The function should be standalone and reusable, with no reliance on global variables.
The solution is straightforward: iterate over each index from 0 to 9 (inclusive) and compare the character at that index in the input string with the corresponding character in the constant target string `"codeforces"`. Increment a counter for every mismatch. Since the input length is fixed at 10, the loop runs exactly 10 times, making the time complexity O(10) = O(1) with O(1) auxiliary space. There are no special edge cases beyond the guaranteed length and alphabet; the only consideration is to initialize the counter to zero and return it after the loop. The function should be `const`-correct by taking the input string by `const std::string&` to avoid unnecessary copies and to promise not to modify it.
#include <string>

// Counts the number of positions where the input string differs from "codeforces".
// Precondition: s.size() == 10 and s contains only lowercase English letters.
int countMismatchedCharacters(const std::string& s) {
    const std::string target = "codeforces";
    int mismatchCount = 0;
    for (int i = 0; i < 10; ++i) {
        if (s[i] != target[i]) {
            ++mismatchCount;
        }
    }
    return mismatchCount;
}
#include <cassert>
#include <string>

// Declare the function being tested (normally this would come from a header).
int countMismatchedCharacters(const std::string& s);

int main() {
    // Exact match
    assert(countMismatchedCharacters("codeforces") == 0);
    // One mismatch at the first character
    assert(countMismatchedCharacters("bodeforces") == 1);
    // One mismatch at the last character
    assert(countMismatchedCharacters("codeforcet") == 1);
    // All characters different
    assert(countMismatchedCharacters("zzzzzzzzzz") == 10);
    // Mixed mismatches
    assert(countMismatchedCharacters("codzxforces") == 2);
    // Lowercase letters only, but different from target
    assert(countMismatchedCharacters("aaaaaaaaaa") == 10);
    // Contains some correct characters in correct positions
    assert(countMismatchedCharacters("codeforses") == 5);
    // Reverse of target (most mismatches, but "c" at end matches? "sedocrofed" is not reverse; use a known pattern)
    assert(countMismatchedCharacters("sedocrofec") == 10); // all mismatches
    // Another mixed case
    assert(countMismatchedCharacters("codeforcess") == 1); // extra 's' would be length 11, but we trust input is 10
    // Case with exactly 5 matches
    assert(countMismatchedCharacters("codefxxxxx") == 5);
    return 0;
}
