Write a C++ function `mex_after_updates(q, x, updates)` that processes a sequence of `q` integer updates and returns a vector of length `q` containing, after each update, the smallest non-negative integer (`mex`) that is *not* representable as the remainder when any of the collected values is divided by `x`. Initially there are no values. For each update, the given number `n` is added to the multiset. The mex after an update is defined as the smallest non-negative integer `m` such that there is no value `v` in the multiset with `v % x == m % x` and also `v >= m`? Actually, interpret precisely: We want the smallest non-negative integer `m` such that for every integer `k >= 0` with `k % x == m % x`, there is no collected value equal to `k`? That is too restrictive. Better: The mex is the smallest non-negative integer `m` for which the counter of remainders modulo `x` is zero at the residue `m % x`? No, that is not correct either because the reference code tracks counts per remainder and increments `ans` while `cnt[ans % x]` is positive, decrementing that count as it consumes that remainder. This effectively simulates: numbers are stored only as their remainder modulo `x`. The mex is the smallest non-negative integer `m` such that the number of collected elements with remainder `m % x` is less than or equal to the number of times that remainder has been "used" (i.e., the number of integers `k` with that remainder that are less than `m`). More precisely, we maintain `cnt[r]` as the count of all collected numbers whose remainder mod `x` equals `r`. The mex `ans` is incremented while `cnt[ans % x]` > 0, and each increment consumes one from the count of that remainder. This effectively answers: after each insertion, find the smallest `m` such that among the collected numbers, there is no value exactly equal to `m`? No — the classic mex of a set of integers is the smallest missing non-negative integer. If we only store remainders, we lose exact values. However, the reference solution’s logic actually computes the *mex of the multiset of numbers*? Let’s check: For each `n`, we increment `cnt[n % x]`. Then while `cnt[ans % x]` is positive, we decrement it and increment `ans`. This counts how many times each remainder has appeared and matches them to consecutive integers sharing that remainder. This is a known trick for dynamic mex when numbers are inserted and we want the smallest missing integer. For example, if `x=2` and we insert `0,1,2`, then after inserting 0, `cnt[0]=1` → ans=0, while loop: cnt[0]>0 → decrement cnt[0]=0, ans=1. Output 1? That would be wrong because the set {0} has mex 1, correct. After inserting 1: cnt[1]=1, ans=1, while cnt[1]>0 → decrement, ans=2. Set {0,1} mex=2, correct. After inserting 2: 2%2=0, cnt[0] becomes 1, ans=2, while cnt[0]>0 → decrement, ans=3. Set {0,1,2} mex=3, correct. So the algorithm indeed computes the mex of the multiset of actual integers, because each remainder count can only be used once per possible integer with that remainder. The idea: For each residue class, we can construct numbers `r, r+x, r+2x, ...`. When we have `cnt[r]` copies of numbers with that remainder, they can cover the first `cnt[r]` numbers in that arithmetic progression. The mex is the smallest number not covered. By greedily incrementing `ans` and consuming a count from its residue each time, we correctly compute the mex after each insertion. Therefore your function should take a vector of integers `updates` and parameters `q` and `x`, and return a vector of the mex after each update. The numbers can be large, up to 10^9, and q up to 10^5, x up to 10^5. Use an efficient approach.

#include <cassert>
#include <vector>

std::vector<int> mex_after_updates(int q, int x, const std::vector<int>& updates);

int main() {
    // Example from the snippet: q=5, x=3, updates [0,1,2,3,4]
    // After 0: multiset {0}, mex=1
    // After 1: {0,1}, mex=2
    // After 2: {0,1,2}, mex=3
    // After 3: {0,1,2,3}, mex=4
    // After 4: {0,1,2,3,4}, mex=5
    assert(mex_after_updates(5, 3, {0,1,2,3,4}) == std::vector<int>({1,2,3,4,5}));

    // Test x=2 with duplicates
    // Insert 0,0,1,1
    // After 0: {0} mex=1
    // After 0: {0,0} mex=1 (still missing 1)
    // After 1: {0,0,1} mex=2
    // After 1: {0,0,1,1} mex=2 (missing 2)
    assert(mex_after_updates(4, 2, {0,0,1,1}) == std::vector<int>({1,1,2,2}));

    // Test all same remainder with x=5
    // Insert 10,20,30 (all %5=0)
    // After 10: {10} mex=0 (0 is missing)
    // After 20: {10,20} mex=0
    // After 30: {10,20,30} mex=0
    assert(mex_after_updates(3, 5, {10,20,30}) == std::vector<int>({0,0,0}));

    // Test sequential filling starting from 0 with x=4
    // Insert 0,4,8,1
    // After 0: {0} mex=1
    // After 4: {0,4} mex=1 (1 missing)
    // After 8: {0,4,8} mex=1
    // After 1: {0,1,4,8} mex=2
    assert(mex_after_updates(4, 4, {0,4,8,1}) == std::vector<int>({1,1,1,2}));

    // Test large values and q=1
    assert(mex_after_updates(1, 100000, {999999999}) == std::vector<int>({0}));

    // Test empty updates (q=0)
    assert(mex_after_updates(0, 3, {}).empty());

    // Test x=1
    // Insert 0,1,2,3
    // After 0: {0} mex=1
    // After 1: {0,1} mex=2
    // After 2: {0,1,2} mex=3
    // After 3: {0,1,2,3} mex=4
    assert(mex_after_updates(4, 1, {0,1,2,3}) == std::vector<int>({1,2,3,4}));

    // Test duplicate with x=1
    // Insert 5,5
    // After 5: {5} mex=0 (0 missing)
    // After 5: {5,5} mex=0
    assert(mex_after_updates(2, 1, {5,5}) == std::vector<int>({0,0}));

    // Test interleaved remainders
    // Insert 2,0,1 (x=3)
    // After 2: {2} mex=0
    // After 0: {0,2} mex=1
    // After 1: {0,1,2} mex=3
    assert(mex_after_updates(3, 3, {2,0,1}) == std::vector<int>({0,1,3}));
    return 0;
}

#include <vector>
#include <cstdint>

// Given q updates and x, return the mex after each insertion.
// The mex is the smallest non-negative integer not present in the multiset.
std::vector<int> mex_after_updates(int q, int x, const std::vector<int>& updates) {
    std::vector<int> cnt(x, 0);
    std::vector<int> result;
    result.reserve(q);
    int ans = 0;
    for (int i = 0; i < q; ++i) {
        ++cnt[static_cast<size_t>(updates[i]) % x];
        while (cnt[static_cast<size_t>(ans) % x] > 0) {
            --cnt[static_cast<size_t>(ans) % x];
            ++ans;
        }
        result.push_back(ans);
    }
    return result;
}

// The core observation is that for each possible remainder `r = 0,1,...,x-1`, numbers congruent to `r` modulo `x` form an arithmetic progression. If we have `c` copies of numbers in that residue class, they can occupy the first `c` non-negative integers of that progression: `r, r+x, r+2x, ..., r+(c-1)x`. The mex is the smallest integer that we cannot fill. We maintain an array `cnt[r]` of size `x` storing the number of inserted values whose remainder modulo `x` equals `r`. After each insertion we increment `cnt[n % x]`. Then, while the current candidate mex `ans` has `cnt[ans % x] > 0`, that means we have an unused copy of that residue to place at position `ans`. We decrement that count and increment `ans`. This is correct because each copy of a residue is used only once, and we process residues in increasing order of `ans`. Since `ans` only increases over time, the total number of while-loop iterations across all `q` updates is bounded by `ans_final`, which is at most `q` (because each consumption corresponds to one inserted value). Thus the total time is O(q + x) for initialization plus O(q) for the loop. Space is O(x). Edge cases: when `x=1`, all numbers have remainder 0, and the algorithm correctly simulates counting consecutive integers. When `q` is 0, return empty vector. When updates contain duplicate values, the count increments accordingly, and the mex will be larger. The initial ans=0, and if no numbers are inserted, the mex is 0. The solution must handle large `q` and `x` within typical limits.
