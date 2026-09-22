Write a C++ function `printUncommonCharacters(const std::string& s1, const std::string& s2)` that prints, separated by spaces and in any order, all characters that appear in exactly one of the two input strings. Characters that appear in both strings (even if with different frequencies) must be excluded entirely. The function should print each uncommon character exactly once, ignoring duplicates within a single string. The output should be sent to `std::cout` followed by a trailing space after each printed character. The function must not return anything; it only outputs to the standard output. Handle empty strings gracefully by printing only the uncommon characters from the non-empty string.

The solution uses an unordered set to track characters from the first string. First, insert every unique character of `s1` into the set. Then, iterate through `s2`: for each character, if it is not in the set, print it immediately (since it is definitely uncommon, as it appears in `s2` but not in `s1`). If it is in the set, it means the character exists in both strings, so we need to remove it from the set later to ensure it is not printed as an `s1`-only character. Collect such "common" characters in a separate vector to avoid modifying the set while iterating. After processing all of `s2`, erase each common character from the set. Finally, iterate over the set and print every remaining character, which now represents characters that appear only in `s1`. This handles duplicates naturally since the set only stores unique characters, and the immediate printing of `s2`-only characters ensures they are printed once even if repeated in `s2`. Edge cases include empty strings (the set or loop simply does nothing) and cases where all characters are common (the set becomes empty after erasure). Time complexity is O(n + m) where n and m are lengths of the two strings, due to constant-time average set operations. Space complexity is O(min(n, m)) for the set and vector, ignoring output size.

#include <iostream>
#include <unordered_set>
#include <vector>
#include <string>

// Prints characters that appear in exactly one of the two input strings.
// Each uncommon character is printed exactly once, followed by a space.
void printUncommonCharacters(const std::string& s1, const std::string& s2) {
    std::unordered_set<char> inFirst;
    std::vector<char> commonChars;

    // Insert all unique characters from s1
    for (char c : s1) {
        inFirst.insert(c);
    }

    // Process s2: print characters not in s1, and collect common ones
    for (char c : s2) {
        if (inFirst.find(c) == inFirst.end()) {
            std::cout << c << ' ';  // appears only in s2
        } else {
            commonChars.push_back(c);  // appears in both
        }
    }

    // Remove all common characters from the set
    for (char c : commonChars) {
        inFirst.erase(c);
    }

    // Print remaining characters that appear only in s1
    for (char c : inFirst) {
        std::cout << c << ' ';
    }
}

#include <cassert>

// Redirect stdout to a string to verify output
#include <sstream>
#include <iostream>

int main() {
    auto runTest = [](const std::string& s1, const std::string& s2, const std::string& expected) {
        std::ostringstream buffer;
        std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());
        printUncommonCharacters(s1, s2);
        std::cout.rdbuf(old);
        std::string output = buffer.str();
        // Sort output for comparison because order of set iteration is unspecified
        std::sort(output.begin(), output.end());
        std::string sortedExpected = expected;
        std::sort(sortedExpected.begin(), sortedExpected.end());
        assert(output == sortedExpected);
    };

    // Original example
    runTest("characters", "alphabets", "c h r e s l p b"); // uncommon: c, h, r, e, s (from s1) and l, p, b (from s2)

    // Duplicates in one string
    runTest("aabbcc", "a", "b c");

    // Empty string
    runTest("", "xyz", "x y z");

    // All common
    runTest("abc", "abc", "");

    // No overlap
    runTest("abc", "def", "a b c d e f");

    // Single character each, different
    runTest("a", "b", "a b");

    // Single character same
    runTest("a", "a", "");

    // Case sensitivity
    runTest("aA", "a", "A");

    // Longer strings with repeated common characters
    runTest("hello", "world", "h e w r d");
}
