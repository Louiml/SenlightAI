// Given a string consisting only of digits `'1'`, `'2'`, and `'3'` (with no spaces or other characters), write a C++ function `int shortestSubstringWithAllThree(const std::string& s)` that returns the length of the shortest contiguous substring that contains at least one `'1'`, one `'2'`, and one `'3'`. If no such substring exists (e.g., the string lacks one of the required digits), return `0`. For example, for input `"123"` the answer is `3` (the whole string), and for input `"113322"` the shortest valid substring is `"132"` (length 3), so the result is `3`. The string length is between 1 and 100,000, and the function must be efficient.

We solve the problem with a single left‑to‑right scan using a sliding window concept, but implemented without actually shrinking a window. Instead, we maintain the most recent occurrence index of each digit (`1`, `2`, `3`) as we iterate through the string. For each character at position `i`, we update the last occurrence of that digit. Then, if all three digits have been seen at least once, we compute the length of the substring that starts at the earliest of the three last occurrences and ends at `i`. The length is `i - minIndex + 1`. We take the minimum over all such candidates. This works because any valid substring ending at `i` must start no later than the smallest last occurrence, and to be as short as possible, it should start exactly at that smallest last occurrence. We do not need to consider earlier starts because they would only lengthen the substring. Edge cases: if any digit never appears, the answer remains 0. If the string length is small or all digits appear early, the algorithm still works. Time complexity is O(n) and space complexity is O(1) (only three integer variables and a result). The string length up to 100,000 fits easily within these bounds.

#include <string>
#include <algorithm>
#include <climits>

// Returns the length of the shortest contiguous substring containing '1', '2', and '3'.
// Returns 0 if no such substring exists.
int shortestSubstringWithAllThree(const std::string& s) {
    // last occurrence indices for '1', '2', '3' respectively
    int last[3] = {-1, -1, -1};
    int best = INT_MAX;

    for (std::size_t i = 0; i < s.size(); ++i) {
        // Convert char to index 0,1,2
        int idx = s[i] - '1';
        last[idx] = static_cast<int>(i);

        // If all three have been seen
        if (last[0] != -1 && last[1] != -1 && last[2] != -1) {
            int earliest = std::min({last[0], last[1], last[2]});
            int length = static_cast<int>(i) - earliest + 1;
            best = std::min(best, length);
        }
    }

    return (best == INT_MAX) ? 0 : best;
}

#include <cassert>
#include <string>

// Function under test is declared above (shortestSubstringWithAllThree)
int main() {
    assert(shortestSubstringWithAllThree("123") == 3);
    assert(shortestSubstringWithAllThree("113322") == 3);
    assert(shortestSubstringWithAllThree("111") == 0);
    assert(shortestSubstringWithAllThree("1") == 0);
    assert(shortestSubstringWithAllThree("132") == 3);
    assert(shortestSubstringWithAllThree("231") == 3);
    assert(shortestSubstringWithAllThree("111222333") == 3);
    assert(shortestSubstringWithAllThree("123123") == 3);
    assert(shortestSubstringWithAllThree("312") == 3);
    assert(shortestSubstringWithAllThree("333111222") == 3);
    return 0;
}
