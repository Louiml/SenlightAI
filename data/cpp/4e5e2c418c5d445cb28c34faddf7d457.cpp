Given a string `s` consisting only of lowercase English letters, write a C++ function `countPalindromicSubsequences` that returns the number of distinct palindromic subsequences of length exactly 3. A subsequence is a sequence derived from the original string by deleting some or no characters without changing the order of the remaining characters. Two palindromic subsequences are considered distinct if they differ in at least one character at some position, regardless of their indices in the original string. For example, in the string `"aabca"`, valid palindromic subsequences of length 3 are `"aba"`, `"aca"`, `"aaa"`, and `"abc"` is not a palindrome, so the answer is 3.

// The key observation is that any palindrome of length 3 has the form `X Y X`, where the first and last characters are equal, and the middle character can be anything. Therefore, for each possible outer character `c` (from `'a'` to `'z'`), we only need to know the first occurrence and the last occurrence of `c` in the string. If there is at least one character between the first and last occurrence, then for every unique letter that appears strictly between those two indices, we can form a valid palindrome `"c + letter + c"`. To avoid counting duplicates, we use a set to collect all distinct middle characters for each outer character. We iterate over all 26 letters, and for each, if it exists and the gap between its first and last occurrence is at least 1 (i.e., there is at least one character in between), we count the number of distinct middle characters in that range and add it to the answer. Edge cases include strings of length less than 3 (no palindromic subsequences of length 3), all same characters (e.g., `"aaa"` → only `"aaa"` counts, giving answer 1), and repeated outer characters with a gap but no distinct middle characters (e.g., `"aa"` has no middle). Time complexity is O(n + 26 * n) = O(n) since we traverse the string once to find first and last occurrences, then for each of the 26 possible outer characters, we scan the subrange in the worst case, but the total scanning across all outer characters is at most 26*n, which simplifies to O(26n) = O(n) because the alphabet size is constant. Space complexity is O(1) for the fixed-size arrays and the set used at any time (set size ≤ 26), so O(1) auxiliary space.

#include <string>
#include <vector>
#include <set>
#include <algorithm>

// Count distinct palindromic subsequences of length exactly 3.
// A subsequence is distinct if its character sequence differs.
int countPalindromicSubsequences(const std::string& s) {
    const int ALPHABET_SIZE = 26;
    int n = static_cast<int>(s.size());
    if (n < 3) {
        return 0;
    }

    // first[i] = first index of character 'a' + i, -1 if not present
    // last[i] = last index of character 'a' + i, -1 if not present
    std::vector<int> first(ALPHABET_SIZE, -1);
    std::vector<int> last(ALPHABET_SIZE, -1);

    for (int i = 0; i < n; ++i) {
        int idx = s[i] - 'a';
        if (first[idx] == -1) {
            first[idx] = i;
        }
        last[idx] = i;
    }

    int count = 0;
    for (int outer = 0; outer < ALPHABET_SIZE; ++outer) {
        if (first[outer] == -1) {
            continue; // character does not appear
        }
        int left = first[outer];
        int right = last[outer];
        if (right - left < 2) {
            continue; // need at least one character in between
        }
        // Collect unique middle characters between left+1 and right-1
        std::set<char> middle_chars;
        for (int j = left + 1; j < right; ++j) {
            middle_chars.insert(s[j]);
        }
        count += static_cast<int>(middle_chars.size());
    }

    return count;
}

#include <cassert>
#include <string>

// Function declared elsewhere (from the solution)
int countPalindromicSubsequences(const std::string& s);

int main() {
    // Basic examples
    assert(countPalindromicSubsequences("aabca") == 3); // "aba", "aca", "aaa"
    assert(countPalindromicSubsequences("abc") == 0);   // no length-3 palindrome
    assert(countPalindromicSubsequences("aaa") == 1);   // "aaa"
    assert(countPalindromicSubsequences("a") == 0);     // too short

    // Edge cases with repeats and gaps
    assert(countPalindromicSubsequences("bbcbaba") == 4); // "bbb", "bcb", "bab", "aba"
    assert(countPalindromicSubsequences("abca") == 1);   // "aba" or "aca"? only "aba" actually, middle 'b' only, so 1
    assert(countPalindromicSubsequences("zzzz") == 1);   // "zzz"
    assert(countPalindromicSubsequences("abcabc") == 3); // "aba", "aca", "bcb"? Let's check: 'a': first 0, last 3 → middle {b,c} → 2; 'b': first 1, last 4 → middle {c,a} → 2; 'c': first 2, last 5 → middle {a,b} → 2; all unique? "aba","aca","bab","bcb","cac","cbc" but many duplicate? Actually distinct sequences: "aba","aca","bab","bcb","cac","cbc" = 6? Wait careful: The function counts distinct sequences, not indices. For "abcabc", sequences: outer a: middle b → "aba", middle c → "aca"; outer b: middle c → "bcb", middle a → "bab"; outer c: middle a → "cac", middle b → "cbc". All six are distinct, so answer should be 6. So assert 6.
    assert(countPalindromicSubsequences("abcabc") == 6);
    assert(countPalindromicSubsequences("abcd") == 0);

    // All 26 letters present with gaps
    std::string all = "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz";
    // For each outer char, middle set contains all 25 other letters? Actually because string repeats, for each outer char first index = i, last = i+26, middle has all other 25 letters plus the same outer char? Since s[i] and s[i+26] are same, middle includes that same char as well? Example outer 'a': first index 0, last index 26, middle indices 1..25 are b..z, so 25 distinct letters. So total = 26 * 25 = 650. But also outer char itself appears inside? No, middle indices 1..25 do not include 'a' because first 'a' is at 0 and next 'a' is at 26, so no 'a' in middle. So answer = 650.
    assert(countPalindromicSubsequences(all) == 650);

    return 0;
}
