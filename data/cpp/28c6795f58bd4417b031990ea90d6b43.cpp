Write a C++ function `int maxEmptyGap(const std::string& s)` that takes a string `s` consisting only of uppercase letters `'A'` and `'B'`. The function must compute and return the maximum number of consecutive `'B'` characters that appear immediately after an `'A'` character, without any other `'A'` in between. More precisely, for every occurrence of `'A'` at index `i`, count how many characters from `i+1` onward are `'B'` until either the string ends or another `'A'` is encountered; return the largest such count across all starting `'A'` positions. If there is no `'A'` in the string, or if every `'A'` is immediately followed by another `'A'` or end of string, return `0`. For example, for `"ABBA"`, the first `'A'` is followed by `"BB"` (gap length 2), and the last `'A'` is at the end with gap length 0, so the result is 2. For `"BAB"`, the only `'A'` is followed by `"B"` (gap length 1), so return 1. For `"AA"` or `"BB"`, return 0. The input string may be empty (in which case return 0), and you may use the standard library. The function must be declared as `int maxEmptyGap(const std::string& s);` and be const-correct.

The solution is a single-pass traversal of the string. The key observation is that we only need to examine segments that start with an `'A'`. We iterate through the string with an index `i`. When we encounter `'A'` at position `i`, we start a counter `gap = 0` and then scan forward from `i+1` while characters are `'B'`, incrementing `gap` each time. The scan stops when we hit another `'A'` or reach the end. After the inner loop, we update the global maximum with `gap`. We then set `i` to the position just after the last `'B'` we counted (which is either the index of the next `'A'` or the end of string), so we do not re-scan the same `'B'` segment. This means each character is visited at most twice overall (once in the outer loop and possibly once in the inner scan), giving linear time. Edge cases: empty string returns 0; string with no `'A'` returns 0; string with consecutive `'A'` yields gap 0 for the first `'A'` because the inner loop immediately sees `'A'` and stops; string ending with `'A'` yields gap 0 for that last `'A'`. The space complexity is O(1) auxiliary, not counting the input string itself.

#include <string>
#include <algorithm>

// Returns the maximum number of consecutive 'B' characters immediately following
// an 'A' in the string, until another 'A' or the end of the string is reached.
int maxEmptyGap(const std::string& s) {
    int result = 0;
    const std::size_t n = s.size();
    std::size_t i = 0;

    while (i < n) {
        if (s[i] == 'A') {
            // Count consecutive 'B's after this 'A'
            std::size_t gap = 0;
            std::size_t j = i + 1;
            while (j < n && s[j] == 'B') {
                ++gap;
                ++j;
            }
            result = std::max(result, static_cast<int>(gap));
            // Jump to the position just after the counted 'B's (either next 'A' or end)
            i = j;
        } else {
            // Non-'A' character (should be 'B' but handle gracefully): just skip
            ++i;
        }
    }
    return result;
}

#include <cassert>
#include <string>

// The solution function is declared above.

int main() {
    // Basic examples
    assert(maxEmptyGap("ABBA") == 2);
    assert(maxEmptyGap("BAB") == 1);
    assert(maxEmptyGap("AA") == 0);
    assert(maxEmptyGap("BB") == 0);
    assert(maxEmptyGap("") == 0);

    // Multiple gaps, take max
    assert(maxEmptyGap("ABBBAB") == 3);      // first gap 3, second gap 1
    assert(maxEmptyGap("ABBABBBBA") == 4);   // gaps: 2, 4, 0

    // Consecutive A's stop counting
    assert(maxEmptyGap("AABBB") == 3);       // only second A has gap 3
    assert(maxEmptyGap("AAAB") == 1);        // last A has gap 1

    // Ends with A or starts with B
    assert(maxEmptyGap("BA") == 0);
    assert(maxEmptyGap("BABBBB") == 4);
    assert(maxEmptyGap("BBBABBB") == 3);

    // Large string performance check (optional but ensures no infinite loop)
    std::string large(100000, 'B');
    large += 'A';
    large += std::string(50000, 'B');
    assert(maxEmptyGap(large) == 50000);

    return 0;
}
