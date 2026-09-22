// Write a C++ function `bool isScrambleString(const std::string& a, const std::string& b)` that determines whether string `b` can be obtained from string `a` through a series of recursive "scramble" operations. A scramble operation on a string `s` of length `n` consists of splitting it at some position `k` (1 ≤ k < n) into two non-empty substrings `s1 = s[0..k-1]` and `s2 = s[k..n-1]`, then optionally swapping the order of these two substrings (i.e., concatenating `s2 + s1`), and finally recursively applying the same scramble operation independently to each of the two resulting substrings (or leaving them as-is if they are length 1). The function should return `true` if it is possible to transform `a` into `b` through any sequence of such recursive operations, and `false` otherwise. Strings consist only of lowercase English letters, may be empty, and lengths of `a` and `b` are equal in all valid test cases. Use memoization for efficiency — implement the recursive helper function with an `std::unordered_map` keyed by a combined string of both substrings separated by a space. Assume the function will be called with strings where `a.size() == b.size()`.
The solution is based on recursive partitioning with memoization. The base cases: (1) if the two strings are of different lengths, return false (though not expected here); (2) if both are empty, return true; (3) if they are equal, return true; (4) if length is 1 and they are not equal, return false (since no split is possible). Before recursion, we perform a quick character-frequency check to prune: for each character in `a` and `b`, we increment/decrement a count array of size 26; if any count is nonzero, we return false because the multiset of characters must match. Then we iterate split positions `i` from 1 to `n-1`. For each split, we check two possibilities: (a) **no swap**: recursively check `a[0..i-1]` vs `b[0..i-1]` and `a[i..n-1]` vs `b[i..n-1]`; (b) **with swap**: check `a[0..i-1]` vs `b[n-i..n-1]` (i.e., the last `i` characters of `b`) and `a[i..n-1]` vs `b[0..n-i-1]`. If either combination returns true, we return true immediately. The recursion terminates because each recursive call reduces the string length. Memoization stores results for each pair `(a, b)` as a key `a + " " + b` to avoid recomputation, which is crucial because the number of distinct substrings can be exponential without it. Edge cases: equal strings, single-character strings, strings with same characters but different orders that are not scrambles, and strings where only the last split allows a match. Time complexity: with memoization, each pair of substrings is computed at most once. For a string of length `n`, there are `O(n^2)` distinct substrings of `a` and `O(n^2)` of `b`, but valid pairs are constrained to equal lengths, giving `O(n^3)` possible pairs in the worst case. For each pair, we iterate over `O(n)` split positions and do `O(1)` map lookups and substring comparisons (substring creation is O(n) per call, but we can optimize by passing indices; here using `substr` is acceptable for clarity). So worst-case time is `O(n^4)` due to substring copies, but with `substr` it's still acceptable for moderate `n`; theoretical optimal is `O(n^4)` with memoization and character counting. Space complexity is `O(n^3)` for the memo map plus `O(n)` recursion stack depth.
#include <string>
#include <unordered_map>
#include <vector>

// Recursive helper with memoization.
bool scrambleHelper(const std::string& a, size_t aStart, size_t aLen,
                    const std::string& b, size_t bStart, size_t bLen,
                    std::unordered_map<std::string, bool>& memo) {
    // Different lengths cannot be scrambles.
    if (aLen != bLen) return false;
    // Both empty → true.
    if (aLen == 0) return true;
    // Extract the two substrings for comparison and memo key.
    std::string subA = a.substr(aStart, aLen);
    std::string subB = b.substr(bStart, bLen);
    // If equal → true.
    if (subA == subB) return true;
    // Length 1 and not equal → false.
    if (aLen == 1) return false;

    // Memoization check.
    std::string key = subA + " " + subB;
    auto it = memo.find(key);
    if (it != memo.end()) return it->second;

    // Character frequency check (all must match).
    std::vector<int> count(26, 0);
    for (size_t i = 0; i < aLen; ++i) {
        count[subA[i] - 'a']++;
        count[subB[i] - 'a']--;
    }
    for (int v : count) {
        if (v != 0) {
            memo[key] = false;
            return false;
        }
    }

    // Try all split positions.
    for (size_t i = 1; i < aLen; ++i) {
        // No swap: a[0..i-1] vs b[0..i-1] and a[i..] vs b[i..]
        if (scrambleHelper(a, aStart, i, b, bStart, i, memo) &&
            scrambleHelper(a, aStart + i, aLen - i, b, bStart + i, bLen - i, memo)) {
            memo[key] = true;
            return true;
        }
        // Swap: a[0..i-1] vs b[aLen-i..] and a[i..] vs b[0..aLen-i-1]
        if (scrambleHelper(a, aStart, i, b, bStart + (bLen - i), i, memo) &&
            scrambleHelper(a, aStart + i, aLen - i, b, bStart, bLen - i, memo)) {
            memo[key] = true;
            return true;
        }
    }

    memo[key] = false;
    return false;
}

// Public interface: returns true if b is a scramble of a.
bool isScrambleString(const std::string& a, const std::string& b) {
    if (a.size() != b.size()) return false;
    std::unordered_map<std::string, bool> memo;
    return scrambleHelper(a, 0, a.size(), b, 0, b.size(), memo);
}
#include <cassert>
#include <string>

// The function is defined above in the solution section.
int main() {
    // Basic cases.
    assert(isScrambleString("great", "great") == true);        // equal
    assert(isScrambleString("ab", "ba") == true);              // single split + swap
    assert(isScrambleString("ab", "ab") == true);              // single split no swap
    assert(isScrambleString("a", "b") == false);               // different single char
    assert(isScrambleString("abc", "cba") == true);            // swap at root: "c"+"ba"? Actually works
    assert(isScrambleString("great", "eatgr") == true);        // from original snippet
    assert(isScrambleString("great", "rgeat") == true);        // known scramble
    assert(isScrambleString("great", "rgtae") == true);        // another known scramble
    assert(isScrambleString("abc", "bac") == true);            // swap left part
    assert(isScrambleString("abcd", "badc") == true);          // multiple splits
    assert(isScrambleString("abcde", "caebd") == false);       // not a scramble
    assert(isScrambleString("", "") == true);                  // both empty
    assert(isScrambleString("a", "a") == true);                // same single char
    assert(isScrambleString("abc", "acb") == true);            // swap right part
    assert(isScrambleString("xyz", "zyx") == true);            // full reverse via swap
    assert(isScrambleString("abcdef", "fedcba") == true);      // full reverse
    return 0;
}
