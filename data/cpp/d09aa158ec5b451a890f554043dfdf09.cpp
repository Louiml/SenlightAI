/*
Write a C++ function that determines whether a given string contains the same number of the character `'p'` (case-insensitive) and the character `'y'` (case-insensitive). The function should return `true` if the counts are equal, and `false` otherwise. The input string may contain any characters (letters, digits, punctuation, spaces), and the counts of `'p'`/`'P'` and `'y'`/`'Y'` should be compared ignoring case. An empty string or a string with zero occurrences of both letters is considered to have equal counts, so it should return `true`. You must implement the function with a meaningful name, apply `const` correctness for the input parameter, and use a standard library approach for iteration.
*/

#include <string>

// Returns true if the number of 'p'/'P' characters equals the number of 'y'/'Y' characters.
bool hasEqualPAndY(const std::string& s) {
    int countP = 0;
    int countY = 0;

    for (char c : s) {
        if (c == 'p' || c == 'P') {
            ++countP;
        } else if (c == 'y' || c == 'Y') {
            ++countY;
        }
    }

    return countP == countY;
}

#include <cassert>

int main() {
    assert(hasEqualPAndY("pPyy") == true);
    assert(hasEqualPAndY("pY") == true);
    assert(hasEqualPAndY("Pyy") == false);
    assert(hasEqualPAndY("y") == false);
    assert(hasEqualPAndY("") == true);
    assert(hasEqualPAndY("abc") == true);
    assert(hasEqualPAndY("pY pY") == true);
    assert(hasEqualPAndY("PpYyY") == false);
    assert(hasEqualPAndY("yyyPPP") == true);
    assert(hasEqualPAndY("123p456Y") == true);
}

// The solution works by iterating over each character in the input string once and maintaining two counters: one for occurrences of `'p'` or `'P'` and another for `'y'` or `'Y'`. For each character, we use a simple equality check against both lowercase and uppercase variants to increment the appropriate counter. After the loop, we compare the two counters; if they are equal, the function returns `true`, otherwise `false`. Edge cases include an empty string (both counters remain zero, so the result is `true`) and strings where only one of the letters appears (counts differ, so `false`). The algorithm has a time complexity of \(O(n)\), where \(n\) is the length of the string, and uses \(O(1)\) auxiliary space since only two integer counters are needed. No special handling for case conversion libraries is required because we explicitly compare against both cases.
