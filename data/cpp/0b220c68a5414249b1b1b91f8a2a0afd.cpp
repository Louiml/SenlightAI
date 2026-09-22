// Given a lowercase English string `s` (possibly empty), write a C++ function `countSubstringsByDistinctCharacters` that returns a vector of integers. For each positive integer `k` from 1 to 26, compute the number of non-empty substrings of `s` that contain **at most** `k` distinct characters. Then, return the list of distinct positive differences between consecutive counts: specifically, for each `k` from 1 to 26, if `count(k) - count(k-1) > 0`, append that difference to the result vector (where `count(0) = 0`). The ordering of the returned vector must follow increasing `k`. The function must not rely on global variables or external input/output; it should process the string entirely within the function.
The core problem is counting substrings with at most `k` distinct characters for each `k` from 1 to 26. A direct approach for each `k` would use a sliding window (two pointers) that maintains the frequency of each character in the current window. The algorithm: initialize two pointers `left` and `right` to 0, and a frequency array of size 26. For a given `k`, keep expanding `right` while the number of distinct characters in the window is at most `k`. For each valid window ending at `right`, the number of new substrings ending at `right` with at most `k` distinct characters is `(right - left + 1)` because any substring starting from `left` up to `right` is valid. When the window has more than `k` distinct characters, increment `left` and update character frequencies until the distinct count drops back to `k` or below. This runs in O(n) per `k`, giving O(26n) overall, which is fine for typical string lengths (up to ~10^5). After computing `count(k)` for all `k`, we subtract consecutive values and collect positive differences. Edge cases: an empty string yields an empty result vector (since all counts are 0). If the string has fewer than 26 distinct characters, some `count(k)` will be identical, and those differences are zero and omitted. Time complexity is O(26·|s|) and space is O(1) for the frequency array plus O(1) for the output (up to 26 entries).
#include <string>
#include <vector>

// Count substrings with at most k distinct characters using a sliding window.
static long long countAtMostK(const std::string& s, int k) {
    if (k <= 0 || s.empty()) return 0;
    
    int freq[26] = {0};
    int distinct = 0;
    int left = 0;
    long long total = 0;
    
    for (int right = 0; right < static_cast<int>(s.size()); ++right) {
        int idx = s[right] - 'a';
        if (freq[idx] == 0) ++distinct;
        ++freq[idx];
        
        while (distinct > k) {
            int leftIdx = s[left] - 'a';
            --freq[leftIdx];
            if (freq[leftIdx] == 0) --distinct;
            ++left;
        }
        
        total += static_cast<long long>(right - left + 1);
    }
    return total;
}

// Return positive differences between consecutive counts for k=1..26.
std::vector<long long> countSubstringsByDistinctCharacters(const std::string& s) {
    std::vector<long long> result;
    long long prev = 0;
    for (int k = 1; k <= 26; ++k) {
        long long cur = countAtMostK(s, k);
        if (cur - prev > 0) {
            result.push_back(cur - prev);
        }
        prev = cur;
    }
    return result;
}
#include <cassert>
#include <vector>
#include <string>

// Include the solution code or paste it here before main.

int main() {
    // Test 1: empty string -> empty result
    assert(countSubstringsByDistinctCharacters("") == std::vector<long long>{});

    // Test 2: single character "aa" -> only one difference (for k=1: 3 substrings)
    // counts: k=1: 3, k>=2: 3, differences: 3
    assert(countSubstringsByDistinctCharacters("aa") == std::vector<long long>{3});

    // Test 3: "abc" -> counts: k=1:3, k=2:5, k=3:6, differences: 3,2,1
    assert(countSubstringsByDistinctCharacters("abc") == std::vector<long long>({3,2,1}));

    // Test 4: "abab" -> counts: k=1:4, k=2:10, k>=3:10, differences: 4,6
    assert(countSubstringsByDistinctCharacters("abab") == std::vector<long long>({4,6}));

    // Test 5: "aaaaa" -> only k=1 difference: 15 substrings
    assert(countSubstringsByDistinctCharacters("aaaaa") == std::vector<long long>{15});

    // Test 6: all 26 letters "abcdefghijklmnopqrstuvwxyz" -> counts increase by 24? Actually each substring has distinct letters up to 26, total substrings = 26*27/2 = 351. Differences are 26, 25, 24, ..., 1.
    std::string alphabet = "abcdefghijklmnopqrstuvwxyz";
    std::vector<long long> expected;
    for (int k = 1; k <= 26; ++k) expected.push_back(27 - k); // 26,25,...,1
    assert(countSubstringsByDistinctCharacters(alphabet) == expected);

    // Test 7: mixed case "abcabc" -> counts k=1:6, k=2:15, k=3:21, differences 6,9,6
    assert(countSubstringsByDistinctCharacters("abcabc") == std::vector<long long>({6,9,6}));
}
