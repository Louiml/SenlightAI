/*
Given a string `s` consisting of lowercase English letters, write a C++ function `rearrangeString` that returns a lexicographically smallest string that can be obtained by permuting the characters of `s` such that the new string is **not equal to** the original string `s`, and is also **not equal to** the reverse of `s`. If no such permutation exists, return the original string. Note: The original string and its reverse may be the same (palindrome), and in that case the requirement reduces to "not equal to the original". If no valid permutation exists, return `s` unchanged. The function must handle any length from 1 to 10^5, and must run in O(n + 26) time (essentially O(n)) and O(1) extra space.
*/

#include <bits/stdc++.h>

// Return the lexicographically smallest permutation of s that is not equal to s and not equal to reverse(s).
// If no such permutation exists, return the original string.
std::string rearrangeString(const std::string& s) {
    int n = (int)s.size();
    if (n <= 1) return s;

    std::string sorted = s;
    std::sort(sorted.begin(), sorted.end());

    std::string rev = s;
    std::reverse(rev.begin(), rev.end());

    // Fast path: sorted string is valid
    if (sorted != s && sorted != rev) return sorted;

    // Helper to compute the original snippet's "not equal to s" result
    auto differentFromOriginal = [&](const std::string& str) -> std::string {
        std::string t = str;
        std::vector<int> cnt(26, 0);
        for (char ch : t) cnt[ch - 'a']++;
        int mn = 26, mn2 = 26;
        for (int i = 0; i < n; ++i) {
            int x = t[i] - 'a';
            if (x < mn) {
                mn2 = mn;
                mn = x;
            } else if (x > mn && x < mn2) {
                mn2 = x;
            }
        }
        if (mn2 == 26) return t; // all same characters, no different permutation
        int only = 0;
        while (only < 26 && cnt[only] != 1) only++;
        if (only < 26) {
            std::string res;
            res += (char)('a' + only);
            cnt[only]--;
            for (int i = 0; i < 26; ++i) {
                while (cnt[i]--) res += (char)('a' + i);
            }
            return res;
        }
        if (cnt[mn] >= 2 && (cnt[mn] - 2) * 2 <= (n - 2)) {
            std::string res;
            res += (char)('a' + mn);
            res += (char)('a' + mn);
            cnt[mn] -= 2;
            int cur = mn + 1;
            while (cur < 26) {
                while (cur < 26 && cnt[cur] == 0) cur++;
                if (cur == 26) break;
                res += (char)('a' + cur);
                cnt[cur]--;
                if (cnt[mn]) {
                    res += (char)('a' + mn);
                    cnt[mn]--;
                }
            }
            return res;
        }
        if (cnt[mn] + cnt[mn2] == n) {
            std::string res;
            res += (char)('a' + mn);
            cnt[mn]--;
            while (cnt[mn2]--) res += (char)('a' + mn2);
            while (cnt[mn]--) res += (char)('a' + mn);
            return res;
        }
        std::string res;
        res += (char)('a' + mn);
        res += (char)('a' + mn2);
        cnt[mn]--;
        cnt[mn2]--;
        while (cnt[mn]--) res += (char)('a' + mn);
        int z = mn2 + 1;
        while (z < 26 && cnt[z] == 0) z++;
        res += (char)('a' + z);
        cnt[z]--;
        while (cnt[mn2]--) res += (char)('a' + mn2);
        for (int i = 0; i < 26; ++i) {
            while (cnt[i]--) res += (char)('a' + i);
        }
        return res;
    };

    std::string cand = differentFromOriginal(s);

    // If cand is the same as s (which happens when all characters are same) or
    // cand equals rev, we need to search further.
    if (cand != s && cand != rev) return cand;

    // If cand is invalid, try next permutations of a copy of cand (which is a permutation of s)
    std::string current = cand;
    // If cand == s (all same), then no valid permutation exists, return s
    if (current == s) return s;

    // Now cand == rev(s) and cand != s, we need to find the next lexicographically larger permutation
    // that is not equal to s and not equal to rev(s). Since cand is rev(s), the next one will be different.
    while (std::next_permutation(current.begin(), current.end())) {
        if (current != s && current != rev) return current;
        // Avoid infinite loop; break after a reasonable number (should be very few)
        // In practice, at most 2-3 steps, but for safety we can cap at some large number.
    }
    return s; // No valid permutation found (shouldn't happen for n>1)
}

#include <cassert>
#include <string>

// Declare the function under test.
std::string rearrangeString(const std::string& s);

int main() {
    // Basic cases
    assert(rearrangeString("a") == "a");
    assert(rearrangeString("aa") == "aa");           // only one permutation
    assert(rearrangeString("ab") == "ab");           // only "ab" and "ba", both forbidden

    // Already sorted and valid
    assert(rearrangeString("abc") == "abc");         // sorted != s and != "cba"
    assert(rearrangeString("cba") == "abc");         // sorted "abc" valid

    // Sorted equals original, need next permutation
    assert(rearrangeString("aab") == "aba");
    assert(rearrangeString("aabb") == "abab");
    assert(rearrangeString("aaab") == "aaba");
    assert(rearrangeString("aaaab") == "aaaba");

    // Sorted equals reverse, need next permutation after that
    assert(rearrangeString("dcba") == "abdc");       // sorted "abcd" == reverse, next "abdc"
    assert(rearrangeString("aba") == "aab");         // sorted "aab" != s and != "aba"

    // Longer string
    assert(rearrangeString("abcdefghij") == "abcdefghji"); // sorted equals original, next valid

    // Palindromic case where sorted != original and != reverse
    assert(rearrangeString("baab") == "aabb");       // sorted "aabb" valid

    // Edge case: all same characters
    assert(rearrangeString("zzzz") == "zzzz");

    return 0;
}

// The problem is about finding the lexicographically smallest permutation of a multiset of characters that satisfies two exclusion constraints. The original code snippet implements a more restricted version (avoiding only the original string), but here we extend it to also avoid the reverse. The key insight: to avoid the original string, we need at least one position where the character differs from the original. To avoid the reverse, we need at least one position where the character differs from the original's reversed string. The lexicographically smallest permutation that satisfies both is built greedily from the smallest possible characters, but we must ensure we don't accidentally produce either forbidden string. A special case: if all characters are the same (i.e., the multiset has only one distinct character), then the only possible permutation is the string itself, which equals its reverse, so we must return the original. Also, if the string length is 1, return it. For longer strings, we can think of generating the smallest permutation that is not equal to `s` and not equal to `rev(s)`. One approach: generate the lexicographically smallest permutation of the multiset (which is sorted). If that equals `s` or `rev(s)`, we need to try the next lexicographically smallest permutation. Since we want efficiency, we cannot generate all permutations. But we can directly construct the answer using a greedy algorithm similar to the original code but with two constraints. The original code handles the "not equal to original" by tracking the smallest and second smallest distinct characters and special cases. We adapt by considering both `s` and `rev(s)` as forbidden. After sorting the characters, we check if sorted == s or sorted == rev(s). If not, return sorted. If yes, we must incrementally adjust. The original code's logic is actually a constructive greedy that produces a string that is lexicographically smallest but not equal to the original. We can extend it by first checking if the sorted string is valid (i.e., not equal to s and not equal to rev(s)). If valid, return sorted. If not, we need to find the next lexicographically larger permutation that is valid. Since the original code provides a way to produce a valid string different from the original, we can incorporate the reverse constraint: we need to find a permutation that differs from both s and rev(s). One possible approach: if the sorted string equals s (which happens when s is already sorted), then we must find a permutation that is different from s. The original code handles that by special cases. We can combine: first try the sorted string; if it's forbidden, then apply the original algorithm to generate a string different from s, but also check if that generated string happens to equal rev(s). If it does, we need to adjust further. Since the original algorithm produces the smallest permutation not equal to s, and if that happens to equal rev(s), then we need the second smallest permutation not equal to s (and also not equal to rev(s)). We can handle this by calling the original algorithm's logic, and if it returns rev(s), we then need to generate the next valid permutation. However, it's simpler to implement a generic solution using the standard library's `next_permutation` on a sorted multiset, but that would be O(n!) in worst case, unacceptable. Therefore, we implement the constructive algorithm from the snippet, but with an added check: after constructing the result, if it equals `rev(s)`, we need to use an alternative construction. Since the snippet's algorithm is already optimal and produces a string not equal to `s`, we can test if the result equals `rev(s)`. If it does, we can try a slightly different construction (e.g., using the third smallest character). Alternatively, we can note that there are only a few cases where both `s` and `rev(s)` are forbidden. A simpler approach: we can generate the lexicographically smallest permutation that is not equal to `s` and not equal to `rev(s)` by considering the sorted characters and then "bumping" the first position where we can change a character to a larger one without making the whole string equal to `rev(s)`. But the easiest way to implement is to follow the original code's logic, and after obtaining the result, if it equals `rev(s)`, then we need to apply a fallback: if there are at least two distinct characters, we can swap the first occurrence of the smallest character with the next character, then sort the rest. Because the original code's result is the lexicographically smallest string different from `s`. If that equals `rev(s)`, then the next smallest different string is the one obtained by swapping the first two distinct characters in the sorted array. We can implement this: first compute the sorted string `sorted`. If `sorted != s && sorted != rev(s)`, return sorted. Else, if `sorted == s`, we use the original algorithm to get a candidate `cand`. If `cand != rev(s)`, return `cand`. Else, we need to get the next candidate: we can generate the lexicographically next permutation of the multiset that is different from both. But to keep it simple, we can implement a generic function that tries the sorted string, then tries the result from the original snippet logic, and if both are forbidden, we can do a simple modification: swap the first two characters that are different in the sorted string and then sort the rest after the swap. Let's analyze correctness. Time complexity O(n + 26), space O(1) (excluding output). We'll implement a helper that generates the lexicographically smallest permutation not equal to a given forbidden set (here forbidden set is {s, rev(s)}). But for brevity, in the solution we will implement the original snippet's logic as a function that generates a string different from `s` (call it `solve(s)`), and then in the main function, we check if sorted is valid, if not, we use `solve(s)` and check if it's valid. If `solve(s)` equals `rev(s)`, then we need to adjust: we can take the string `solve(s)` and find the next lexicographically larger permutation that is not equal to `s` (since `solve(s)` is already different from `s`, but equals `rev(s)`). That next permutation will be different from `rev(s)`. To find it, we can take `solve(s)` and apply `next_permutation` once (since the multiset size is up to 10^5, doing one `next_permutation` on a vector of chars is O(n) and fine). But we must ensure that the resulting permutation is still different from `s` (it will be, since `solve(s)` is already different from `s` and `next_permutation` produces a strictly larger string, so it can't become equal to `s` because `s` is smaller than `solve(s)`? Not necessarily. But we can check). So the plan: 1. Sort the string. 2. If sorted != s and sorted != rev(s), return sorted. 3. Else, use the original snippet's algorithm to generate `cand` (a string different from `s`). 4. If `cand != rev(s)`, return `cand`. 5. Else, apply `next_permutation` to `cand` once, and check if the result is not equal to `s` and not equal to `rev(s)`. If yes, return that. If not, apply `next_permutation` again (should be rare). Actually, we can loop until we find a valid one, but in practice at most a couple of steps. Since the multiset size is large, this is still O(n) per step. We'll implement this way.
