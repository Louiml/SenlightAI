Write a C++ function named `firstLongestRunIndex` that takes a non-empty string as input and returns the zero-based index of the first character of the longest consecutive run of identical characters. A run is any maximal sequence of the same character. If multiple runs share the same maximum length, return the starting index of the earliest one. For example, `firstLongestRunIndex("aaabbbbaa")` should return 3 (the run "bbbb" starts at index 3), and `firstLongestRunIndex("abc")` should return 0 because all runs have length 1 and the first run starts at index 0. The function must be robust for strings containing any printable ASCII characters, including spaces, digits, and punctuation.

#include <cassert>

int main() {
    assert(firstLongestRunIndex("abbcccddddcccbba") == 6);
    assert(firstLongestRunIndex("aaabbbbaa") == 3);
    assert(firstLongestRunIndex("abc") == 0);
    assert(firstLongestRunIndex("a") == 0);
    assert(firstLongestRunIndex("aaaa") == 0);
    assert(firstLongestRunIndex("aabbaa") == 0);
    assert(firstLongestRunIndex("bbbaaa") == 0);
    assert(firstLongestRunIndex("aaabbb") == 0);
    assert(firstLongestRunIndex("abcccbb") == 2);
    assert(firstLongestRunIndex("  xx  ") == 0); // spaces count
}

#include <string>

// Returns the starting index of the first longest run of identical characters.
int firstLongestRunIndex(const std::string& str) {
    if (str.empty()) return 0; // Not expected per task, but safe.

    int bestStart = 0;
    int bestLength = 0;

    int currentStart = 0;
    int currentLength = 1;

    for (std::size_t i = 1; i < str.size(); ++i) {
        if (str[i] == str[i - 1]) {
            ++currentLength;
        } else {
            if (currentLength > bestLength) {
                bestLength = currentLength;
                bestStart = currentStart;
            }
            currentStart = static_cast<int>(i);
            currentLength = 1;
        }
    }

    if (currentLength > bestLength) {
        bestLength = currentLength;
        bestStart = currentStart;
    }

    return bestStart;
}

// The solution scans the string from left to right while tracking the current run's starting index and length. Initialize the current run's starting index to 0, its length to 1, and set the best run's starting index to 0 and best length to 0. For each character from index 1 to the end, if it equals the previous character, increment the current run length; otherwise, compare the current run length with the best so far. If the current run length is strictly greater than the best length, update the best starting index and length. Then reset the current run to the new character with starting index at the current position and length 1. After the loop, perform one final comparison for the last run. Edge cases include single-character strings (the loop does not run, and the final comparison updates the best correctly), strings where the entire string is one run, and ties where only the first longest run is kept because we use strict greater-than. Time complexity is O(n) with one pass, and space complexity is O(1) auxiliary.
