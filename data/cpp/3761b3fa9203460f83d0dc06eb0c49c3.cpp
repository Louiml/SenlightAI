Given two lowercase English strings `s` and `t` of equal length `n` (1 ≤ n ≤ 200,000), you may perform an unlimited number of moves on `s`, where each move swaps two adjacent characters. The cost of moving a character from position `i` to position `j` (with `i < j`) using adjacent swaps is exactly `j - i`. Determine the minimum total number of adjacent swaps required to make `s` lexicographically smaller than `t`. If it is never possible to make `s` lexicographically smaller than `t`, return `-1`. You are to write a function `long long minAdjacentSwapsToBeSmaller(std::string s, std::string t)` that returns this minimum cost, where the cost is the total number of adjacent swaps performed (each move counts as 1 operation, regardless of which characters are swapped). Note that you may stop at any point once `s` becomes lexicographically smaller than `t`, and you do not need to fully sort or rearrange `s`; you only need to achieve the condition.
We need to find the minimum number of adjacent swaps to reach any string `p` that is a permutation of `s` (by repeatedly swapping adjacent characters) such that `p` is lexicographically smaller than `t`. Since adjacent swaps cost exactly the inversion distance from the original positions, we can think in terms of moving characters from `s` to match a prefix of `t` and then, at the first position where we need a smaller character than `t[i]`, we can bring that smaller character to the current position and stop.

The core observation: For each prefix position `i` from 0 to n-1, we try to match `t[0..i-1]` exactly using the earliest available occurrences of those characters from `s`. At position `i`, if we can place any character strictly smaller than `t[i]` there, we can achieve lexicographic smallness with cost equal to the swaps already used to match the prefix plus the number of swaps needed to bring that smaller character to the current front (which is its original index minus the number of already-used positions before it). We take the minimum over all such choices. If we cannot match the prefix and also don't have a smaller character at that point, we break because it's impossible to continue.

We maintain for each letter (a-z) a list of positions where it occurs in `s`, processed from back to front (so the back of the list is the smallest unused index). We also maintain a Fenwick tree (Binary Indexed Tree) initialized with 1 at every original index, representing the current positions of unused characters after we "remove" used ones. The cost to bring a character originally at index `p` to the current front position (after removing used ones before it) is `fenwick.pref(p-1)` (number of unused positions before `p`).

Algorithm:
- Build vectors `pos[26]` of indices where each letter appears in `s`, reversed so back is smallest index.
- Initialize Fenwick tree size n with all 1s.
- `ops = 0` (swaps used to match prefix so far).
- `ans = INF`.
- For each position `i` from 0 to n-1:
  - Let `y = t[i]-'a'`.
  - For each letter `x < y` that has an unused occurrence, try to use its smallest unused index `p = pos[x].back()`: candidate cost = `ops + fenwick.pref(p-1)`. Update `ans = min(ans, candidate)`.
  - If the letter `y` has an unused occurrence, take it: `p = pos[y].back()`, add `fenwick.pref(p-1)` to `ops`, remove it (update Fenwick to 0 at `p`), and pop from `pos[y]`. Then continue to next i.
  - If `y` has no unused occurrence, break (cannot match prefix further; only smaller letters at this position could have worked, but we already considered them).

At the end, if `ans` remains INF, return -1, else return `ans`.

Time complexity: O(n * 26 + n log n) = O(n log n) for n up to 200,000. Space O(n).

Edge cases: If `s` is already lexicographically smaller than `t` at position 0, then ans = 0 (because the loop at i=0 will consider smaller letters and one might be at position 0 directly). If no smaller character exists ever, return -1. The Fenwick tree's `pref(-1)` is 0, handled correctly.
#include <bits/stdc++.h>
using namespace std;

// Fenwick tree (1-indexed internally but we use 0-based for positions)
struct Fenwick {
    int n;
    vector<int> bit;
    Fenwick(int n) : n(n), bit(n, 0) {}
    void add(int idx, int delta) { // idx 0-based
        for (; idx < n; idx |= idx + 1) bit[idx] += delta;
    }
    int sum(int idx) { // sum of [0..idx] inclusive, idx 0-based
        int res = 0;
        for (; idx >= 0; idx = (idx & (idx + 1)) - 1) res += bit[idx];
        return res;
    }
};

// Returns minimum adjacent swaps to make s lexicographically smaller than t, or -1.
long long minAdjacentSwapsToBeSmaller(const std::string& s, const std::string& t) {
    const int n = static_cast<int>(s.size());
    vector<vector<int>> pos(26);
    for (int i = 0; i < n; ++i) {
        pos[s[i] - 'a'].push_back(i);
    }
    // reverse so back() gives the smallest unused index for that letter
    for (auto& vec : pos) reverse(vec.begin(), vec.end());

    Fenwick fw(n);
    for (int i = 0; i < n; ++i) fw.add(i, 1);

    long long ops = 0; // swaps used to match prefix so far
    long long ans = LLONG_MAX; // minimal additional cost found

    for (int i = 0; i < n; ++i) {
        const int y = t[i] - 'a';
        // Try placing a strictly smaller character at position i
        for (int x = 0; x < y; ++x) {
            if (!pos[x].empty()) {
                int p = pos[x].back();
                int cost = fw.sum(p - 1); // number of unused characters before p
                ans = min(ans, ops + static_cast<long long>(cost));
            }
        }
        // Try to place t[i] itself (to continue matching the prefix)
        if (!pos[y].empty()) {
            int p = pos[y].back();
            ops += static_cast<long long>(fw.sum(p - 1));
            fw.add(p, -1);
            pos[y].pop_back();
        } else {
            break; // cannot match this prefix character
        }
    }

    return (ans == LLONG_MAX) ? -1 : ans;
}
#include <cassert>
#include <string>

// The function under test (declared above; include its implementation before main)
long long minAdjacentSwapsToBeSmaller(const std::string& s, const std::string& t);

int main() {
    // Already smaller at position 0
    assert(minAdjacentSwapsToBeSmaller("abc", "bca") == 0);

    // Need one swap to bring 'a' to front from position 1
    assert(minAdjacentSwapsToBeSmaller("bac", "abc") == 1);

    // Two swaps: move 'a' from position 0 to position 1? Actually "cba" vs "abc": need to make 'a' at front? Let's compute.
    // "cba": positions c=0,b=1,a=2. To be smaller than "abc", at i=0 we need char < 'a'? impossible, so break.
    // But at i=0 we consider smaller letters than 'a' none, so ans stays INF -> -1.
    assert(minAdjacentSwapsToBeSmaller("cba", "abc") == -1);

    // Example: s="abc", t="acb": at i=0 'a' matches, at i=1 we need char < 'c', can bring 'b' from index 1 (already there) cost 0? Actually position 1 is 'b', cost 0 for b, ans=0. So returns 0 because s "abc" < "acb"? Yes.
    assert(minAdjacentSwapsToBeSmaller("abc", "acb") == 0);

    // Need to move 'b' from index 2 to index 1: cost 1 (swap b with a? actually "aac" vs "aba"? Let's make concrete.)
    // s="aac", t="aba": at i=0 'a' matches, i=1 need < 'b', can bring 'a' from index 2: cost = fw.sum(1) where original index 2, before it unused indices 0,1 (both a,a) so cost=2. So ans=2.
    assert(minAdjacentSwapsToBeSmaller("aac", "aba") == 2);

    // Multiple tests with larger n
    std::string s1 = "abcde", t1 = "abcfd";
    // s already lexicographically smaller at i=3? "abcd" < "abcf" yes, ans 0
    assert(minAdjacentSwapsToBeSmaller(s1, t1) == 0);

    // s = "edcba", t = "abcde" -> impossible because first char cannot be < 'a'
    assert(minAdjacentSwapsToBeSmaller("edcba", "abcde") == -1);

    // Single character
    assert(minAdjacentSwapsToBeSmaller("a", "a") == -1);
    assert(minAdjacentSwapsToBeSmaller("a", "b") == 0);

    // Duplicates: s="baa", t="aab": need to bring 'a' to front? At i=0 need < 'a'? none. Break -> -1? But we can match? Let's test: i=0 t='a', we can take 'a' from index 1 cost 1, ops=1. i=1 t='a', take 'a' from index 2 cost? after removing index1, before index2 unused indices 0 ('b'), so cost=1, ops=2. i=2 t='b', need char < 'b'? only 'a' none left, break. ans never updated because no smaller letter ever. So return -1. But can we make "baa" < "aab"? No. So correct.
    assert(minAdjacentSwapsToBeSmaller("baa", "aab") == -1);

    // Another case: s="aba", t="baa": at i=0 need < 'b', can bring 'a' from index 0 cost 0 -> ans=0. So return 0.
    assert(minAdjacentSwapsToBeSmaller("aba", "baa") == 0);

    return 0;
}
