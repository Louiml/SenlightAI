// Write a C++ function `minimumRemainingString(string s, int k)` that takes a lowercase English string `s` and an integer `k`. The function should remove characters from `s` so that after removal, the number of distinct characters remaining in `s` is minimized, and among all ways to achieve that minimal distinct count, the remaining string preserves the original relative order of characters. You may remove at most `k` characters in total, each removed character counts toward `k` by the frequency of that character (i.e., if you decide to remove character `c` entirely, you must remove all occurrences of `c`, and the cost is the frequency of `c`). You cannot partially remove a character (either keep all occurrences or remove all). If a character is removed, it contributes its full frequency to `k`. The function returns the resulting string after removals (preserving order). If multiple solutions exist with the same minimal distinct count and same total removals (which is forced by minimal distinct count), any valid resulting string is acceptable. You may assume `k` is non-negative and if `k` is large enough, the result could be an empty string.
// The approach is to count frequencies of each of the 26 letters. Then sort pairs of `(frequency, letter)` in ascending order. Iterate over this sorted list: for each character with frequency > 0, if its frequency is ≤ remaining `k`, it means we can remove that entire character (all its occurrences) using up to `k` removals; we do so, reduce `k` by its frequency, and mark that character as removed. Otherwise, we cannot remove it entirely, so we must keep it. The minimal number of distinct characters is simply the number of characters we are forced to keep (since we always remove as many as possible starting from the smallest frequencies). After deciding which characters to remove, we scan the original string and append only characters that are not marked as removed, preserving order. Edge cases: if `k` is 0, no removals happen. If all characters can be removed (k ≥ total length), the result is an empty string. Duplicate characters do not affect the distinct count logic. Time complexity is O(n + 26 log 26) = O(n) effectively, space O(1) auxiliary (ignoring output string).
#include <string>
#include <vector>
#include <algorithm>
#include <utility>

// Given a lowercase string s and a budget k, remove entire characters (all occurrences)
// to minimize the number of distinct characters left, preserving order of kept characters.
std::string minimumRemainingString(const std::string& s, int k) {
    if (k <= 0) {
        return s;
    }
    std::vector<int> freq(26, 0);
    for (char c : s) {
        freq[c - 'a']++;
    }

    // Pair of (frequency, letter_index)
    std::vector<std::pair<int,int>> items;
    for (int i = 0; i < 26; ++i) {
        if (freq[i] > 0) {
            items.emplace_back(freq[i], i);
        }
    }

    std::sort(items.begin(), items.end());

    std::vector<bool> removed(26, false);
    int remaining_budget = k;
    for (const auto& p : items) {
        int f = p.first;
        int idx = p.second;
        if (f <= remaining_budget) {
            removed[idx] = true;
            remaining_budget -= f;
        }
    }

    std::string result;
    result.reserve(s.size());
    for (char c : s) {
        if (!removed[c - 'a']) {
            result.push_back(c);
        }
    }
    return result;
}
#include <cassert>
#include <string>

// The solution function is declared above (or included here via header).
// Test cases.
int main() {
    // Simple case: remove one entire character with lowest frequency.
    assert(minimumRemainingString("aabbbcc", 2) == "bbbcc");
    // Remove character with freq exactly equal to k.
    assert(minimumRemainingString("aabbc", 2) == "bbc");
    // k = 0, no changes.
    assert(minimumRemainingString("abc", 0) == "abc");
    // Remove all characters.
    assert(minimumRemainingString("aaabb", 5) == "");
    // k too small to remove any full character.
    assert(minimumRemainingString("aaabbb", 2) == "aaabbb");
    // Multiple characters removable, choose smallest frequencies first.
    // 'a' freq 1, 'b' freq 2, 'c' freq 3, k=3 → remove 'a' and 'b' (cost 3), keep 'c'.
    assert(minimumRemainingString("aabbccc", 3) == "ccc");
    // Only one distinct after removal.
    assert(minimumRemainingString("xyyzzz", 3) == "zzz");
    // Large k but not enough to remove second smallest because of budget.
    // 'a' freq 2, 'b' freq 2, 'c' freq 1, k=3 → remove 'c' (cost1) then 'a' or 'b' (cost2), total cost3, keep the other.
    std::string res = minimumRemainingString("aabbc", 3);
    // Valid outcomes: keep all 'a' (aab) or keep all 'b' (bbb) — both have length 3 with one distinct.
    assert(res == "aab" || res == "bbb");
    // Edge case: empty input string.
    assert(minimumRemainingString("", 5) == "");
    return 0;
}
