// Write a C++ function named `countGoodSubstrings` that takes a string `s` and an integer `k` as parameters and returns the number of substrings of `s` that have length exactly `k` and contain no repeated characters. The function must handle cases where `k` is larger than the string length (returning 0) and where the string contains only lowercase English letters. For example, given `s = "abacaba"` and `k = 3`, the valid substrings are `"aba"` (has repeat) → invalid, `"bac"` (all unique) → valid, `"aca"` (repeat) → invalid, `"cab"` (all unique) → valid, `"aba"` (repeat) → invalid, so the result should be 2. The function should be efficient for inputs up to length 10,000.

// The optimal approach uses a sliding window with a fixed size `k`. Maintain a frequency array for the 26 lowercase letters, and a counter `uniques` that tracks how many distinct characters are currently in the window. Expand the window by adding the right character; if that character’s count becomes 1, increment `uniques`. If the window size exceeds `k`, remove the left character; if its count becomes 0, decrement `uniques`. Whenever the window length equals `k`, check if `uniques == k` — if so, all characters are distinct, so increment the result. Edge cases: if `k` is 0 (though specified k ≥ 1, handle it defensively by returning 0), if `k` > `s.length()`, the loop will never produce a window of length `k`, so result remains 0. The algorithm runs in O(n) time and uses O(1) extra space (the frequency array is fixed size 26).

#include <string>
#include <array>

// Count substrings of length k with no repeated characters.
// Returns the number of valid substrings.
int countGoodSubstrings(const std::string& s, int k) {
    if (k <= 0 || k > static_cast<int>(s.size())) {
        return 0;
    }

    const int n = static_cast<int>(s.size());
    std::array<int, 26> count{};
    int result = 0;
    int uniques = 0;

    for (int right = 0; right < n; ++right) {
        // Add current character to window
        int addIdx = s[right] - 'a';
        ++count[addIdx];
        if (count[addIdx] == 1) {
            ++uniques;
        }

        // Remove left character if window size exceeds k
        if (right >= k) {
            int leftIdx = s[right - k] - 'a';
            --count[leftIdx];
            if (count[leftIdx] == 0) {
                --uniques;
            }
        }

        // Check if window of size k is valid
        if (right >= k - 1 && uniques == k) {
            ++result;
        }
    }

    return result;
}

#include <cassert>

int main() {
    // Basic examples
    assert(countGoodSubstrings("abacaba", 3) == 2);
    assert(countGoodSubstrings("xyzzaz", 3) == 1);
    assert(countGoodSubstrings("aababcabc", 3) == 4);

    // Edge cases
    assert(countGoodSubstrings("abc", 3) == 1);
    assert(countGoodSubstrings("abc", 4) == 0);
    assert(countGoodSubstrings("a", 1) == 1);
    assert(countGoodSubstrings("", 1) == 0);
    assert(countGoodSubstrings("aaaa", 2) == 0);

    // All unique long string
    std::string allUnique = "abcdefghijklmnopqrstuvwxyz";
    assert(countGoodSubstrings(allUnique, 26) == 1);
    assert(countGoodSubstrings(allUnique, 25) == 2);

    // k = 1 always returns string length (all single chars unique)
    assert(countGoodSubstrings("hello", 1) == 5);

    return 0;
}
