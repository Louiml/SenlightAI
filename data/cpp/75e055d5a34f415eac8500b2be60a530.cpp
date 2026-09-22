// Write a C++ function `std::string longestHomogeneousRun(const std::string& s)` that takes a non-empty string consisting of printable ASCII characters (e.g., letters, digits, punctuation, no spaces) and returns a string formatted as `"index repeatedChar"` where `index` is the zero-based position where the longest contiguous run of identical characters begins, and `repeatedChar` is the character forming that run. If there are multiple runs of the same maximal length, choose the one that appears **earliest** in the string (i.e., the smallest starting index). The returned string must have exactly one space between the index and the character. For example, for `"aabbbcc"` the longest run is `"bbb"` starting at index 2, so return `"2 b"`; for `"xxx"` return `"0 x"`; for `"ab"` both runs have length 1, but the first run starts at index 0, so return `"0 a"`. Handle edge cases like a single-character string and strings where all characters are the same (starting index always 0). Do not write a `main` function in the solution; only the free function and necessary headers.

// The algorithm performs a single pass through the string. Maintain variables: `currentChar`, `currentRunLength`, `currentRunStart`, and `maxRunLength`, `maxRunStart`, `maxRunChar`. Initialize with `currentChar = s[0]`, `currentRunLength = 1`, `currentRunStart = 0`, and set the max variables similarly. Iterate from index 1 to `s.length()-1`. For each character, if it equals `currentChar`, increment `currentRunLength`. Otherwise (character changes), we finalize the current run: if `currentRunLength` is greater than `maxRunLength`, update the max variables; then reset `currentChar = s[i]`, `currentRunLength = 1`, and `currentRunStart = i`. After the loop, check the last run (e.g., if the string ends with the longest run) and update max if needed. Because we update max only when strictly greater, ties naturally resolve to the earlier starting index (since later equal-length runs won't replace the earlier max). Edge cases: empty string is not expected per specification (but could be handled by returning empty), single character returns `"0 c"`. Time complexity is O(n) for one pass; space complexity is O(1) auxiliary, plus the length of the output string (constant format, not dependent on n). No extra data structures are needed.

#include <string>
#include <cstddef>

// Return the starting index and character of the longest contiguous run of identical characters.
// If multiple runs have equal maximal length, choose the one with the smallest starting index.
// The output format is "startingIndex char" (e.g., "3 x").
std::string longestHomogeneousRun(const std::string& s) {
    if (s.empty()) {
        return "";
    }

    // Current run state
    char currentChar = s[0];
    std::size_t currentRunLength = 1;
    std::size_t currentRunStart = 0;

    // Best (longest) run found so far
    char maxChar = currentChar;
    std::size_t maxRunLength = 1;
    std::size_t maxRunStart = 0;

    for (std::size_t i = 1; i < s.length(); ++i) {
        if (s[i] == currentChar) {
            ++currentRunLength;
        } else {
            // Finalize the previous run
            if (currentRunLength > maxRunLength) {
                maxRunLength = currentRunLength;
                maxRunStart = currentRunStart;
                maxChar = currentChar;
            }
            // Start a new run with the current character
            currentChar = s[i];
            currentRunLength = 1;
            currentRunStart = i;
        }
    }

    // Finalize the last run after the loop
    if (currentRunLength > maxRunLength) {
        maxRunLength = currentRunLength;
        maxRunStart = currentRunStart;
        maxChar = currentChar;
    }

    // Build output as "index character" (e.g., "3 x")
    return std::to_string(maxRunStart) + " " + maxChar;
}

#include <cassert>
#include <string>

// Declare the function being tested
std::string longestHomogeneousRun(const std::string& s);

int main() {
    // Basic tests
    assert(longestHomogeneousRun("aabbbcc") == "2 b");
    assert(longestHomogeneousRun("a") == "0 a");
    assert(longestHomogeneousRun("ab") == "0 a");
    assert(longestHomogeneousRun("xxx") == "0 x");
    
    // Tie cases: choose the earliest occurrence
    assert(longestHomogeneousRun("aabb") == "0 a");
    assert(longestHomogeneousRun("112233") == "0 1");
    
    // Runs at the end of the string
    assert(longestHomogeneousRun("aaab") == "0 a");
    assert(longestHomogeneousRun("baaa") == "1 a");
    
    // All same characters
    assert(longestHomogeneousRun("zzzzz") == "0 z");
    
    // Mixed with punctuation
    assert(longestHomogeneousRun("!!!...!!!") == "0 !");
    
    // Longer run later, but not longer than an earlier one
    assert(longestHomogeneousRun("abbbaa") == "1 b");
    
    return 0;
}
