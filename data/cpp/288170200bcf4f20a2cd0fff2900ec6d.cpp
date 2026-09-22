// Given a string `s` of length `n` that contains only the characters `'.'`, `'|'`, and `'*'`, write a C++ function `bool isAsteriskWithinBars(const std::string& s)` that returns `true` if and only if there exists at least one `'*'` character located strictly between the first and last vertical bar `'|'` in the string (inclusive of the positions of the first and last bars). If there are fewer than two `'|'` characters in the string, the function must return `false`. The input string is guaranteed to be non‑empty. The function should not modify the input and should operate efficiently even for large strings.

// The algorithm is straightforward: first, scan the string to find the index of the first occurrence of `'|'` and the index of the last occurrence of `'|'`. If no bar is found (or equivalently less than two bars), return `false`. Otherwise, iterate from that first index to the last index (inclusive). If any character in that range is `'*'`, return `true`; otherwise return `false`. Edge cases include: (1) empty string (not allowed but handle defensively), (2) string with only one `'|'` or no `'|'` (must return `false`), (3) a `'*'` that appears before the first bar or after the last bar (must be ignored), and (4) a `'*'` exactly at the first or last bar position (which is impossible because the positions contain `'|'`, but the loop is inclusive and checks each character). Time complexity is \(O(n)\) because we scan the string once to find bars and once to check the range (these can be combined into a single pass). Auxiliary space complexity is \(O(1)\) beyond the input.

#include <string>
#include <algorithm>

// Returns true iff there is an asterisk between the first and last vertical bar (inclusive).
bool isAsteriskWithinBars(const std::string& s) {
    if (s.empty()) {
        return false;
    }

    int firstBar = -1;
    int lastBar = -1;

    // Find first and last vertical bar in a single pass.
    for (int i = 0; i < static_cast<int>(s.size()); ++i) {
        if (s[i] == '|') {
            if (firstBar == -1) {
                firstBar = i;
            }
            lastBar = i;
        }
    }

    // If no bar found, or only one bar (firstBar == lastBar), return false.
    if (firstBar == -1 || firstBar == lastBar) {
        return false;
    }

    // Check positions between first and last bar (inclusive) for an asterisk.
    for (int i = firstBar; i <= lastBar; ++i) {
        if (s[i] == '*') {
            return true;
        }
    }

    return false;
}

#include <cassert>
#include <string>

// Declare the solution function (typically would be in a header).
bool isAsteriskWithinBars(const std::string& s);

int main() {
    // No bars.
    assert(isAsteriskWithinBars(".....*.....") == false);
    // One bar only.
    assert(isAsteriskWithinBars("...|....") == false);
    // Asterisk before first bar.
    assert(isAsteriskWithinBars("*..|...|..") == false);
    // Asterisk after last bar.
    assert(isAsteriskWithinBars("..|...|..*") == false);
    // Asterisk exactly between bars.
    assert(isAsteriskWithinBars("..|.*.|..") == true);
    // Asterisk adjacent to bars but inside.
    assert(isAsteriskWithinBars("|*|") == true);
    assert(isAsteriskWithinBars("|.|") == false);
    // Multiple bars and multiple asterisks, one inside.
    assert(isAsteriskWithinBars("|..*..|....*") == true);
    // Bars with no asterisk inside.
    assert(isAsteriskWithinBars("|....|") == false);
    // Long string with bars at ends and star inside.
    assert(isAsteriskWithinBars("|....*....|") == true);
    return 0;
}
