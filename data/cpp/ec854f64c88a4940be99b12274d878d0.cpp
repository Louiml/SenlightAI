Write a C++ function `countOccurrenceSum` that takes a vector of strings (the dictionary, where each string is non-empty and contains only lowercase English letters) and a single query string `text` (also non-empty, lowercase letters). For each dictionary string `s`, if `s` can be found as a contiguous substring anywhere in `text`, the function must add the length of `s` to the result. The function returns the total sum of lengths of all dictionary strings that occur at least once in `text`. Duplicate dictionary strings must each be counted separately if they appear; ignore case, and treat strings of different lengths independently. Note that overlapping occurrences still count the string only once per dictionary entry (i.e., if a dictionary string appears multiple times, we count its length only once). However, if two different dictionary strings (even with the same content) both appear, we add each of their lengths. The dictionary may contain up to 10^5 strings, each of total length up to 10^6, and the text length can be up to 10^6. The function should be efficient for large inputs.

// We need to determine, for each dictionary string, whether it occurs as a substring in the query text. A naive approach comparing every dictionary string with every position in text would be O(total dictionary length * text length) which is too slow. We can use polynomial rolling hash with two different bases to reduce collision probability. Precompute powers of both bases up to the maximum possible string length (1e6). Then compute the hash of the query text prefix-wise so that any substring hash can be obtained in O(1). For each dictionary string, compute its hash, then iterate over all starting positions in text where it could fit (from 0 to textLen - dictLen) and compare the substring hash with the dictionary hash. If equal, add dictionary length to result and break early for that dictionary string. This handles duplicates naturally because each dictionary string is processed separately. Edge cases: empty dictionary (return 0), dictionary string longer than text (skip), and hash collisions (mitigated by using two bases). Complexity: building powers O(maxLen), hashing text O(textLen), and for each dictionary string, hash computation O(dictLen) and scanning O(textLen) in worst case. If total dictionary length is M, total time is O(M + textLen * number of distinct dictionary lengths) which is acceptable for given constraints. Space: O(maxLen) for powers and O(textLen) for text hashes.

#include <string>
#include <vector>
#include <utility>
#include <cstddef>

// Count the sum of lengths of dictionary strings that appear as substrings in text.
long long countOccurrenceSum(const std::vector<std::string>& dictionary, const std::string& text) {
    const long long BASE1 = 37;
    const long long BASE2 = 41;
    const long long MOD1 = 1000000007;
    const long long MOD2 = 1000000009;

    int maxLen = 1;
    for (const auto& s : dictionary) {
        if (static_cast<int>(s.size()) > maxLen) maxLen = s.size();
    }
    maxLen = std::max(maxLen, static_cast<int>(text.size()));

    // Precompute powers of both bases modulo mods.
    std::vector<long long> pow1(maxLen + 1), pow2(maxLen + 1);
    pow1[0] = 1;
    pow2[0] = 1;
    for (int i = 1; i <= maxLen; ++i) {
        pow1[i] = (pow1[i - 1] * BASE1) % MOD1;
        pow2[i] = (pow2[i - 1] * BASE2) % MOD2;
    }

    // Prefix hashes for text.
    int n = static_cast<int>(text.size());
    std::vector<long long> pre1(n + 1, 0), pre2(n + 1, 0);
    for (int i = 0; i < n; ++i) {
        pre1[i + 1] = (pre1[i] * BASE1 + text[i]) % MOD1;
        pre2[i + 1] = (pre2[i] * BASE2 + text[i]) % MOD2;
    }

    auto getTextHash = [&](int l, int r) -> std::pair<long long, long long> {
        // l and r are 0-based indices, inclusive
        int len = r - l + 1;
        long long h1 = (pre1[r + 1] - (pre1[l] * pow1[len]) % MOD1 + MOD1) % MOD1;
        long long h2 = (pre2[r + 1] - (pre2[l] * pow2[len]) % MOD2 + MOD2) % MOD2;
        return {h1, h2};
    };

    long long result = 0;
    for (const auto& s : dictionary) {
        int len = static_cast<int>(s.size());
        if (len > n) continue;
        // Compute hash of dictionary string
        long long h1 = 0, h2 = 0;
        for (char c : s) {
            h1 = (h1 * BASE1 + c) % MOD1;
            h2 = (h2 * BASE2 + c) % MOD2;
        }
        // Scan all possible starting positions in text
        bool found = false;
        for (int start = 0; start <= n - len; ++start) {
            auto p = getTextHash(start, start + len - 1);
            if (p.first == h1 && p.second == h2) {
                found = true;
                break;
            }
        }
        if (found) {
            result += len;
        }
    }
    return result;
}

#include <cassert>
#include <string>
#include <vector>

// Declaration from solution
long long countOccurrenceSum(const std::vector<std::string>& dictionary, const std::string& text);

int main() {
    // Basic test: single dictionary string appears
    assert(countOccurrenceSum({"abc"}, "xxabcxx") == 3);
    // Dictionary string not present
    assert(countOccurrenceSum({"xyz"}, "abcde") == 0);
    // Multiple dictionary strings, some appear, some don't
    assert(countOccurrenceSum({"a", "ab", "abc", "abcd"}, "cab") == 4); // "a" occurs, "ab" occurs, "abc" occurs, "abcd" too long
    // Duplicates in dictionary count each separately
    assert(countOccurrenceSum({"ok", "ok", "no"}, "okay") == 4); // two "ok" each add 2
    // Dictionary string appears multiple times but counted once
    assert(countOccurrenceSum({"ana"}, "banana") == 3); // "ana" appears twice but sum only 3
    // Empty dictionary
    assert(countOccurrenceSum({}, "anything") == 0);
    // Longer dictionary string than text
    assert(countOccurrenceSum({"longstring"}, "short") == 0);
    // Exact match with whole text
    assert(countOccurrenceSum({"hello"}, "hello") == 5);
    // Overlap of different lengths
    assert(countOccurrenceSum({"aa", "a"}, "aaa") == 3); // "aa" occurs, "a" occurs, sum=2+1=3
    // Large random but small test for correctness
    assert(countOccurrenceSum({"ab", "ba", "abc"}, "ababba") == 2 + 2 + 0); // "ab" appears, "ba" appears, "abc" not
    return 0;
}
