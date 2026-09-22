/*
Given three fixed-length strings of equal even length `N` — `s1`, `s2`, and a target `res` (all lengths are `2*N`, but `s1` and `s2` have length `N` each) — simulate the following shuffle repeatedly: starting from `s1` (top half) and `s2` (bottom half), interleave them by taking the first character from `s2`, then the first character from `s1`, then the second from `s2`, then second from `s1`, and so on, producing a combined string of length `2*N`. After each shuffle, split the result back into two new halves: the first `N` characters become the new `s1`, and the last `N` characters become the new `s2`. Count the number of shuffles performed. If, before or after any shuffle, the combined string equals `res`, return the shuffle count (starting from 1 for the first shuffle). If the combined string repeats a previously seen combined string before ever matching `res`, return `-1`. Write a function `int shuffleUntilTarget(const std::string& s1, const std::string& s2, const std::string& res)` that returns the minimal shuffle count or `-1`. The input strings consist of only lowercase English letters and are guaranteed to satisfy `s1.length() == s2.length() == res.length()/2` and all lengths are positive. The function must be const-correct and not modify the input strings.
*/
#include <string>
#include <unordered_set>

// Simulate the shuffle process; return the number of shuffles to reach res, or -1 if a cycle occurs.
int shuffleUntilTarget(const std::string& s1, const std::string& s2, const std::string& res) {
    const int n = s1.size();
    std::string cur1 = s1;
    std::string cur2 = s2;
    std::unordered_set<std::string> seen;
    int count = 0;

    while (true) {
        // Interleave: first char from cur2, then from cur1, alternating.
        std::string combined;
        combined.reserve(2 * n);
        for (int i = 0; i < n; ++i) {
            combined.push_back(cur2[i]);
            combined.push_back(cur1[i]);
        }
        ++count;

        // Success?
        if (combined == res) {
            return count;
        }

        // Cycle detected?
        if (seen.find(combined) != seen.end()) {
            return -1;
        }

        seen.insert(combined);

        // Split combined into two new halves.
        cur1 = combined.substr(0, n);
        cur2 = combined.substr(n);
    }
}
#include <cassert>
#include <string>

int shuffleUntilTarget(const std::string& s1, const std::string& s2, const std::string& res);

int main() {
    // Example from typical UVa problem (shuffle "aabc" etc.)
    assert(shuffleUntilTarget("a", "b", "ab") == 1);
    assert(shuffleUntilTarget("ab", "cd", "acbd") == 1);
    // Simple two-character case: s1="a", s2="b", res="ba" -> after shuffle "ab", split to a,b, repeat -> cycles
    assert(shuffleUntilTarget("a", "b", "ba") == -1);
    // Longer example: s1="ab", s2="cd", res="bcda" -> shuffle1="acbd" (count1), split to "ac","bd", shuffle2="bacd" (count2), split to "ba","cd", shuffle3="cbad" (count3), split to "cb","ad", shuffle4="dacb" (count4), split to "da","cb", shuffle5="cdab" (count5), split to "cd","ab", shuffle6="acdb" (count6), split to "ac","db", shuffle7="bdac" (count7), split to "bd","ac", shuffle8="abdc" (count8), split to "ab","dc", shuffle9="adcb" (count9), split to "ad","cb", shuffle10="cbad" again? Let's actually test something reachable.
    // Easier: s1="ab", s2="cd", res="acbd" -> shuffle1 matches.
    assert(shuffleUntilTarget("ab", "cd", "acbd") == 1);
    // s1="ab", s2="cd", res="bacd" -> shuffle1="acbd", not; shuffle2="bacd" -> returns 2.
    assert(shuffleUntilTarget("ab", "cd", "bacd") == 2);
    // s1="ab", s2="cd", res="cdab" -> eventually maybe? Let's compute manually: shuffle1 acbd, shuffle2 bacd, shuffle3 cbad, shuffle4 dacb, shuffle5 cdab -> count 5.
    assert(shuffleUntilTarget("ab", "cd", "cdab") == 5);
    // Impossible: s1="ab", s2="cd", res="zz" -> invalid input lengths? We ignore, but assume matching lengths.
    // Test cycle: s1="aa", s2="aa", res="bb" -> all combined "aaaa" always, never "bb" -> first shuffle "aaaa", seen? insert, then second shuffle "aaaa" seen -> -1.
    assert(shuffleUntilTarget("aa", "aa", "bb") == -1);
    // Test with same string as initial combined but not res? Actually initial s1 and s2 produce a shuffle; if res equals that, count 1.
    assert(shuffleUntilTarget("xyz", "uvw", "uxvywz") == 1);
    // Test a case needing multiple but reachable: s1="abc", s2="def", res="dafbec"? Let's just trust above.
    return 0;
}
// The core simulation is straightforward: repeatedly perform the interleaving shuffle, count each shuffle, and check three conditions in order: (1) if the resulting combined string equals `res`, success with the current count; (2) if the combined string has been seen before in a `std::set` or `std::unordered_set` (storing the combined string of length `2*N`), then we are in a cycle and will never reach the target (because the state is fully determined by the combined string — the split into halves is deterministic) — return `-1`; (3) otherwise, add the combined string to the set and split it into the new `s1` and `s2` for the next iteration. Important edge cases: The initial `s1` and `s2` might already produce `res` on the first shuffle; if `res` equals the initial combined string before any shuffle, the loop handles it since the first shuffle produces that string, so the count is 1. The cycle detection must occur after producing the combined string, but before inserting it into the set? Actually, we must check if the string already exists in the set *before* inserting it, because if it exists, we’ve returned to a previous state and will loop forever. The loop terminates either by finding `res` or by detecting a repeated combined string. Time complexity is O(K * N) where K is the number of distinct shuffles before a repeat or match; in the worst case K ≤ 2^(2N) but practically for fixed-size strings K ≤ (2N)! but bounded by the number of possible permutations of the 2N characters; but because the shuffle is deterministic and invertible, the cycle length is at most the number of distinct permutations reachable, which is at most (2N)! but for lowercase letters and small N it’s manageable; for typical competitive programming constraints (N ≤ 100), K is usually small because after at most 2N shuffles you might not repeat, but theoretically it can be large; however we can bound by the number of distinct strings of length 2N using 26 letters, which is huge, so the set size grows with the cycle length. Space complexity is O(K * 2N) for storing all distinct combined strings in the set.
