// Given a binary string `s` of length `n` where each character is either `'0'` or `'1'`, write a C++ function `int longestBalancedSubstring(const std::string& s)` that returns the length of the longest contiguous substring in which the number of `'1'`s equals the number of `'0'`s. If no such substring exists, return `0`. The string is non-empty, and you may assume it contains only the characters `'0'` and `'1'`. Your function must be efficient enough for `n` up to 10^6 and must not use any extra data structures beyond a few variables and a hash map.
The key insight is to transform the problem into a prefix-sum equality problem. Replace every `'1'` with `+1` and every `'0'` with `-1`. Then a substring `s[l..r]` (inclusive) has an equal number of `'1'`s and `'0'`s if and only if the prefix sum up to `r` equals the prefix sum up to `l-1`. Therefore, we compute prefix sums incrementally and maintain a hash map that stores the earliest index at which each prefix sum value was first seen. For each new prefix sum, if it has been seen before, the length of the valid substring ending at the current position is `current_index - first_seen_index`, and we update the answer to the maximum of such lengths. We initialize the map with prefix sum `0` at index `-1` to handle substrings starting at the very beginning. Edge cases: the whole string may be balanced, or no balanced substring exists (e.g., all `'1'`s), in which case the answer is `0`. Time complexity is `O(n)` because each character is processed once and hash map operations are average `O(1)`. Space complexity is `O(n)` in the worst case because the hash map may store up to `n+1` distinct prefix sums.
#include <string>
#include <unordered_map>
#include <algorithm>

// Returns the length of the longest substring with equal numbers of '1' and '0'.
int longestBalancedSubstring(const std::string& s) {
    std::unordered_map<int, int> first_seen;
    first_seen[0] = -1;  // prefix sum 0 occurs at index -1 (before the string)
    
    int sum = 0;
    int best = 0;
    
    for (int i = 0; i < static_cast<int>(s.size()); ++i) {
        sum += (s[i] == '1') ? 1 : -1;
        
        auto it = first_seen.find(sum);
        if (it == first_seen.end()) {
            first_seen[sum] = i;  // first occurrence of this sum
        } else {
            best = std::max(best, i - it->second);
        }
    }
    return best;
}
#include <cassert>
#include <string>

int longestBalancedSubstring(const std::string& s);

int main() {
    assert(longestBalancedSubstring("01") == 2);
    assert(longestBalancedSubstring("10") == 2);
    assert(longestBalancedSubstring("111") == 0);
    assert(longestBalancedSubstring("000") == 0);
    assert(longestBalancedSubstring("1010") == 4);
    assert(longestBalancedSubstring("1100") == 4);
    assert(longestBalancedSubstring("1001") == 4);
    assert(longestBalancedSubstring("111000") == 6);
    assert(longestBalancedSubstring("00110011") == 8);
    assert(longestBalancedSubstring("0") == 0);
}
