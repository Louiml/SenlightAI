/*
Given an integer `n` (the length of a binary string), an integer `k` (a period length), and a string `s` of length `n` consisting of characters `'0'`, `'1'`, and `'?'`, write a C++ function that determines whether it is possible to replace every `'?'` with either `'0'` or `'1'` so that the resulting string is `k`-periodic (i.e., `s[i] == s[i % k]` for all `0 <= i < n`) and the number of `'0'`s in the first `k` characters is at most `k/2` and the number of `'1'`s in the first `k` characters is also at most `k/2`. Return `true` if such a replacement exists, and `false` otherwise.
*/
#include <string>
#include <vector>

// Determine if a binary string with '?' can be made k-periodic with balanced counts.
bool canMakePeriodic(std::string& s, int n, int k) {
    // Force the first k characters based on later positions.
    for (int i = k; i < n; ++i) {
        if (s[i] == '?') continue;
        if (s[i % k] == '?') {
            s[i % k] = s[i];
        } else if (s[i % k] != s[i]) {
            return false;
        }
    }

    // Count known 0s and 1s in the first k characters.
    int cnt0 = 0, cnt1 = 0;
    for (int i = 0; i < k; ++i) {
        if (s[i] == '0') cnt0++;
        else if (s[i] == '1') cnt1++;
    }

    // The number of each character cannot exceed half of k.
    if (cnt0 > k / 2 || cnt1 > k / 2) {
        return false;
    }
    return true;
}
#include <cassert>
#include <string>

// Include the solution function declaration here (or link to it).
bool canMakePeriodic(std::string& s, int n, int k);

int main() {
    // Test 1: Basic valid case with ?'s
    std::string s1 = "1??1";
    assert(canMakePeriodic(s1, 4, 2) == true);

    // Test 2: Contradiction in later positions
    std::string s2 = "01??0";
    assert(canMakePeriodic(s2, 5, 2) == false);

    // Test 3: Too many zeros in first block
    std::string s3 = "0001";
    assert(canMakePeriodic(s3, 4, 2) == false);

    // Test 4: All '?' with k=2, n=4, valid
    std::string s4 = "????";
    assert(canMakePeriodic(s4, 4, 2) == true);

    // Test 5: k=1, only one character allowed repeated, any '?' okay
    std::string s5 = "0?1";
    assert(canMakePeriodic(s5, 3, 1) == false); // because zeros=1, ones=1, k/2=0, both exceed

    // Test 6: k=3, n=6, balanced
    std::string s6 = "?1?0??";
    assert(canMakePeriodic(s6, 6, 3) == true);

    // Test 7: k odd, e.g., 3, with 2 ones forced
    std::string s7 = "11?";
    assert(canMakePeriodic(s7, 3, 3) == false); // cnt1=2 > 3/2=1

    // Test 8: k=4, n=4, all '?' -> true
    std::string s8 = "????";
    assert(canMakePeriodic(s8, 4, 4) == true);

    // Test 9: k=2, n=6, string "10??10" valid
    std::string s9 = "10??10";
    assert(canMakePeriodic(s9, 6, 2) == true);

    return 0;
}
// The key observation is that the periodicity constraint forces every position `i >= k` to match its corresponding position `i % k` in the first block. First, scan positions from `i = k` to `n-1`. If `s[i]` is not `'?'`, and `s[i%k]` is `'?'`, then we can assign `s[i%k] = s[i]` because the first block's value is forced. If both are not `'?'` and they differ, the condition is impossible, so return `false`. After this pass, the first `k` characters contain no contradictions; they may still contain `'?'` values that are not forced. Then count the known `'0'` and `'1'` in the first `k` characters. For a valid replacement, the number of `'0'`s cannot exceed `k/2` and the number of `'1'`s cannot exceed `k/2`, because there are exactly `k` positions and each `'?'` can fill either side, but if one side already exceeds half, it's impossible. If both counts are within limits, return `true`. Time complexity is `O(n)` scanning the string twice (once for the main pass, once for counting). Space complexity is `O(1)` beyond the input string (we modify the input string in place, which is allowed if we pass by reference).
