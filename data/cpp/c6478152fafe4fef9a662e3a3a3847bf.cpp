You are given a string `s` consisting of lowercase English letters, and two integers `n` (the length of `s`) and `k` (a divisor of `n`). Your task is to write a C++ function `constructGroups` that takes `n`, `k`, and `s` as parameters and returns a `std::vector<std::string>` representing `k` groups, each of length `n/k`. The groups are built greedily: for each group (from 0 to k-1), you repeatedly pick the smallest available character (by alphabetical order) from the remaining multiset of characters of `s`, and add it to the current group, but you must stop adding characters to the current group as soon as you have picked exactly `n/k` characters total for that group. If at any point there are no remaining characters of a certain letter, skip that letter. The output vector should contain the groups in the order they were constructed. Assume the input is always valid: `k` divides `n`, `n > 0`, `k > 0`, and `s` has exactly `n` lowercase letters. The function must not modify the input string, and you should not use global variables.

#include <cassert>
#include <vector>
#include <string>

// Include the constructGroups function here (or link appropriately)
// For brevity, declared above.

int main() {
    // Basic test: n=4, k=2, t=2
    {
        std::string s = "aabb";
        auto groups = constructGroups(4, 2, s);
        assert(groups.size() == 2);
        assert(groups[0] == "aa");
        assert(groups[1] == "bb");
    }

    // Test with all distinct letters: n=3, k=3, t=1
    {
        std::string s = "cba";
        auto groups = constructGroups(3, 3, s);
        assert(groups.size() == 3);
        assert(groups[0] == "a");
        assert(groups[1] == "b");
        assert(groups[2] == "c");
    }

    // Test with duplicate distribution: n=6, k=3, t=2
    {
        std::string s = "aabbbb";
        auto groups = constructGroups(6, 3, s);
        assert(groups.size() == 3);
        // First group: two 'a's? But there are only two 'a's total, so first group gets 'a' and 'b' (since after taking 'a', next smallest is 'b')
        assert(groups[0] == "ab");
        assert(groups[1] == "bb");
        assert(groups[2] == "bb");
    }

    // Single character repeated: n=5, k=5, t=1
    {
        std::string s = "zzzzz";
        auto groups = constructGroups(5, 5, s);
        assert(groups.size() == 5);
        for (const auto& g : groups) {
            assert(g == "z");
        }
    }

    // Mixed letters with spare counts: n=4, k=1, t=4
    {
        std::string s = "dcba";
        auto groups = constructGroups(4, 1, s);
        assert(groups.size() == 1);
        assert(groups[0] == "abcd");  // sorted order as we take smallest each time
    }

    // Edge: all same letter, k=2, n=4
    {
        std::string s = "aaaa";
        auto groups = constructGroups(4, 2, s);
        assert(groups.size() == 2);
        assert(groups[0] == "aa");
        assert(groups[1] == "aa");
    }

    // Larger test: n=8, k=2, t=4
    {
        std::string s = "aabbccdd";
        auto groups = constructGroups(8, 2, s);
        assert(groups.size() == 2);
        assert(groups[0] == "aabb");
        assert(groups[1] == "ccdd");
    }

    // Verify that total characters in all groups equals n and each group length equals t
    {
        std::string s = "abcabcabc"; // n=9, k=3, t=3
        auto groups = constructGroups(9, 3, s);
        assert(groups.size() == 3);
        int total = 0;
        for (const auto& g : groups) {
            assert(g.size() == 3);
            total += g.size();
        }
        assert(total == 9);
    }

    return 0;
}

#include <vector>
#include <string>

// Constructs k groups of equal length from the multiset of characters in s,
// picking the smallest available character for each group greedily.
std::vector<std::string> constructGroups(int n, int k, const std::string& s) {
    // Frequency of each lowercase letter
    std::vector<int> freq(26, 0);
    for (char c : s) {
        freq[c - 'a']++;
    }

    int t = n / k;  // size of each group
    std::vector<std::string> result;
    result.reserve(k);

    for (int group = 0; group < k; ++group) {
        int remaining = t;
        std::string current;
        current.reserve(t);

        // Always pick the smallest available letter first
        for (int letter = 0; letter < 26 && remaining > 0; ++letter) {
            if (freq[letter] == 0) continue;
            --remaining;
            current.push_back(static_cast<char>('a' + letter));
            --freq[letter];
        }
        result.push_back(current);
    }

    return result;
}

// The problem simulates a greedy distribution of characters from a frequency count into `k` groups of equal size `t = n/k`. The main algorithm: first compute a frequency array `freq[26]` for the characters in `s`. Then, repeat `k` times: for each group, initialize a counter `remaining = t`. Iterate over letters from `'a'` to `'z'` in increasing order. For each letter, if its frequency is zero, skip it. Otherwise, decrement `remaining`, append that character to the current group string, decrement its frequency, and if `remaining` becomes zero, break out of the loop. This guarantees that each group gets exactly `t` characters, and that within each group, characters appear in non-decreasing alphabetical order (since we always pick the smallest available letter first). A critical edge case is ensuring that the total number of characters across all groups sums to `n`, which is guaranteed because we only stop a group when it has exactly `t` characters and we have enough total frequency to fill all groups. Also note that this greedy process may produce groups that are not necessarily contiguous substrings of the original string; it just uses the multiset of characters. Time complexity is O(k * 26) = O(n) since k ≤ n and each iteration scans at most 26 letters, so O(26k) which is O(n) in the worst case (when k is large, e.g., k=n, t=1). Space complexity is O(26 + k * t) = O(n) for the frequency array and the output vector of strings.
