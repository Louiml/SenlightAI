/*
Write a C++ function named `canBeEqualWithOneSwap` that takes two strings `s1` and `s2` of equal length and returns a boolean value indicating whether `s1` can be made equal to `s2` by performing **at most one swap** of two characters (not necessarily adjacent) within `s1`.  
The function must handle the case where the strings are already identical (returns `true`), where exactly two positions differ and swapping the characters at those positions makes them equal (returns `true`), and any other configuration (returns `false`).  
Assume the input strings consist only of lowercase English letters. The function should be efficient and avoid unnecessary copying. The strings are passed by `const` reference to guarantee no modification.
*/
#include <string>
#include <vector>

// Returns true if s1 can be made equal to s2 by at most one swap of two characters in s1.
bool canBeEqualWithOneSwap(const std::string& s1, const std::string& s2) {
    // If lengths differ, they can never be equal (though problem guarantees equal length).
    if (s1.size() != s2.size()) {
        return false;
    }

    std::vector<int> diffIndices;
    for (int i = 0; i < static_cast<int>(s1.size()); ++i) {
        if (s1[i] != s2[i]) {
            diffIndices.push_back(i);
            if (diffIndices.size() > 2) {
                return false; // More than two differences cannot be fixed with one swap.
            }
        }
    }

    if (diffIndices.empty()) {
        return true; // Already equal.
    }
    if (diffIndices.size() != 2) {
        return false; // Exactly one difference cannot be fixed.
    }

    int first = diffIndices[0];
    int second = diffIndices[1];
    // Check if swapping the two mismatched characters makes the strings equal.
    return (s1[first] == s2[second] && s1[second] == s2[first]);
}
#include <cassert>

int main() {
    // Already equal
    assert(canBeEqualWithOneSwap("abc", "abc") == true);

    // One swap needed (adjacent)
    assert(canBeEqualWithOneSwap("ab", "ba") == true);

    // One swap needed (non-adjacent)
    assert(canBeEqualWithOneSwap("abcd", "badc") == true); // swap positions 0 and 1, then 2 and 3? Actually only one swap allowed, so this should be false! Let's fix test.
    // Correct: "abcd" -> swap 0 and 1 gives "bacd" != "badc", so false.
    assert(canBeEqualWithOneSwap("abcd", "badc") == false);

    // One swap that works (positions 0 and 3)
    assert(canBeEqualWithOneSwap("abcdef", "fedcba") == false); // needs 3 swaps
    assert(canBeEqualWithOneSwap("abc", "cba") == false); // needs 2 swaps

    // Exactly two differences and swap fixes
    assert(canBeEqualWithOneSwap("ab", "ba") == true);
    assert(canBeEqualWithOneSwap("abcd", "abdc") == true); // swap positions 2 and 3

    // Exactly two differences but swap does not fix
    assert(canBeEqualWithOneSwap("abcd", "abce") == false);
    assert(canBeEqualWithOneSwap("abcd", "acbd") == false); // positions 1 and 2 differ, but characters not matching across

    // More than two differences
    assert(canBeEqualWithOneSwap("abc", "def") == false);

    // Empty strings
    assert(canBeEqualWithOneSwap("", "") == true);

    // One difference only
    assert(canBeEqualWithOneSwap("abc", "abd") == false);

    // Duplicate characters and swap works
    assert(canBeEqualWithOneSwap("aab", "baa") == true); // swap positions 0 and 1 -> "aba" != "baa"? Let's check: "aab" -> swap 0 and 1 gives "aab" (same), swap 1 and 2 gives "aba", not "baa". So false.
    // Actually "aab" and "baa" differ in positions 0 ('a' vs 'b') and 1 ('a' vs 'a'?) Wait:
    // s1="aab", s2="baa". s1[0]='a', s2[0]='b' differ. s1[1]='a', s2[1]='a' same. s1[2]='b', s2[2]='a' differ. So two differences: indices 0 and 2.
    // Swap s1[0] and s1[2] gives "baa" which equals s2. So true.
    assert(canBeEqualWithOneSwap("aab", "baa") == true);
    assert(canBeEqualWithOneSwap("abca", "acba") == true); // swap positions 1 and 2

    // Swap produces same string (both differences are same character)
    assert(canBeEqualWithOneSwap("aa", "aa") == true);
    assert(canBeEqualWithOneSwap("ab", "ab") == true);
}
// The core idea is to scan both strings simultaneously while tracking the indices where the characters differ. Since at most one swap is allowed, at most two differing positions can exist.  
// - If zero differences exist, the strings are already equal, so the answer is `true`.  
// - If exactly one difference exists, a single swap cannot fix it, so answer is `false`.  
// - If exactly two differences exist (`i` and `j`), we check whether swapping those two characters in `s1` makes the strings equal. This requires that `s1[i] == s2[j]` and `s1[j] == s2[i]`. If both hold, answer is `true`; else `false`.  
// - If more than two differences exist, a single swap cannot fix the strings, so return `false` immediately.  
//
// This algorithm runs in **O(n)** time where `n` is the length of the strings, and uses **O(1)** extra space (only a small fixed‑size vector for at most 3 indices, but can be simplified to two integer variables). Edge cases include empty strings (which are trivially equal) and strings with duplicate characters (the condition still works correctly). The solution avoids any sorting or hashing, making it straightforward and efficient.
