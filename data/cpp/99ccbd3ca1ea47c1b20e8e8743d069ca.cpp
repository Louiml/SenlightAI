Write a C++ function `long long countFixedAssignments(int N, const std::vector<int>& perm)` that receives an integer `N` and a vector `perm` of length `N` containing values from `1` to `N` or `-1`. The vector represents a partial permutation where `-1` denotes an unfilled position and any other value `t` at position `i` means position `i` is forced to map to `t`. It is guaranteed that no value appears more than once among non-`-1` entries, but it is possible that a non-`-1` value `t` equals its own index `i` (a fixed point) or that some values are missing entirely. The function must count, modulo `1000000007`, the number of ways to replace each `-1` with a distinct integer from `1` to `N` not already used elsewhere, so that the resulting mapping is a permutation (i.e., each number from 1 to N appears exactly once). Return the count modulo `1e9+7`.

#include <cassert>
#include <vector>

long long countFixedAssignments(int N, const std::vector<int>& perm);

int main() {
    // Already a complete permutation: only 1 way.
    assert(countFixedAssignments(3, {1, 2, 3}) == 1);
    // All positions free: derangements of 3 elements (2 ways).
    assert(countFixedAssignments(3, {-1, -1, -1}) == 2);
    // One free position, but its index is 2 and value 2 is missing? Actually index 2 free, value 2 unused → that would be a fixed point, so derangement count 0.
    // Let's construct: N=2, perm = {-1, -1}? That's derangements of 2 = 1. Better: N=2, perm = {-1, 1}? Then position 1 free, missing value 2. No fixed point possible, so 1 way.
    assert(countFixedAssignments(2, {-1, 1}) == 1);
    // N=4, perm = {-1, -1, 3, 1}. Free positions: 1,2. Missing values: 2,4. Fixed-point potential: pos1 value1? 1 is used, pos2 value2? 2 missing and pos2 free → potential fixed point. So derangements of 2 = 1.
    assert(countFixedAssignments(4, {-1, -1, 3, 1}) == 1);
    // N=4, perm = {-1, 2, -1, 4}. Free positions: 1,3. Missing values: 1,3. Both free positions are potential fixed points. Derangements of 2 = 1.
    assert(countFixedAssignments(4, {-1, 2, -1, 4}) == 1);
    // N=5, perm = {-1, -1, -1, 4, 5}. Free: 1,2,3. Missing: 1,2,3. All three are potential fixed points. Derangements of 3 = 2.
    assert(countFixedAssignments(5, {-1, -1, -1, 4, 5}) == 2);
    // N=5, perm = {1, -1, -1, -1, -1}. Free: 2,3,4,5. Missing: 2,3,4,5. All free positions are potential fixed points. Derangements of 4 = 9.
    assert(countFixedAssignments(5, {1, -1, -1, -1, -1}) == 9);
    // N=1, perm = {-1} → derangements of 1 = 0.
    assert(countFixedAssignments(1, {-1}) == 0);
    // Large case check modulo: N=2000 all free → derangements !2000 mod 1e9+7.
    std::vector<int> big(2000, -1);
    long long ans = countFixedAssignments(2000, big);
    // Compute !2000 manually using recurrence: D(0)=1, D(1)=0, D(n)=(n-1)*(D(n-1)+D(n-2)) mod.
    long long d0 = 1, d1 = 0;
    for (int n = 2; n <= 2000; ++n) {
        long long d2 = (n - 1) * (d0 + d1) % MOD;
        d0 = d1;
        d1 = d2;
    }
    assert(ans == d1);
    return 0;
}

#include <vector>
#include <cstdint>

const int MOD = 1000000007;

// Count modulo 1e9+7 the number of ways to complete a partial permutation.
// 'perm' has length N, where -1 denotes an empty position, and every other value
// is a distinct integer from 1 to N.
long long countFixedAssignments(int N, const std::vector<int>& perm) {
    // Precompute factorials up to N (assume N <= 2000).
    std::vector<long long> fact(N + 1, 1);
    for (int i = 1; i <= N; ++i) {
        fact[i] = fact[i - 1] * i % MOD;
    }

    // Precompute binomial coefficients C[n][k] for n up to N.
    std::vector<std::vector<long long>> comb(N + 1, std::vector<long long>(N + 1, 0));
    comb[0][0] = 1;
    for (int n = 1; n <= N; ++n) {
        comb[n][0] = comb[n][n] = 1;
        for (int k = 1; k < n; ++k) {
            comb[n][k] = (comb[n - 1][k - 1] + comb[n - 1][k]) % MOD;
        }
    }

    // Count total free positions (cnt1) and positions that are both free and
    // whose index value is not used anywhere (cnt2).
    std::vector<bool> marked(N + 1, false);
    int cnt1 = 0;
    for (int i = 1; i <= N; ++i) {
        if (perm[i - 1] == -1) {
            ++cnt1;
        } else {
            marked[i] = true;       // index i is used as a position
            marked[perm[i - 1]] = true; // value used
        }
    }

    int cnt2 = 0;
    for (int i = 1; i <= N; ++i) {
        if (!marked[i]) {
            ++cnt2;
        }
    }

    // Inclusion–exclusion over the cnt2 positions that could become fixed points.
    long long ans = 0;
    for (int i = 0; i <= cnt2; ++i) {
        long long term = fact[cnt1 - i] * comb[cnt2][i] % MOD;
        if (i % 2 == 0) {
            ans = (ans + term) % MOD;
        } else {
            ans = (ans - term + MOD) % MOD;
        }
    }
    return ans;
}

// This is a derangement‑style counting problem with partially fixed positions. Let `cnt1` be the number of `-1` entries (free positions). Let `cnt2` be the number of integers from `1` to `N` that never appear in any of the non-`-1` entries (available values). These counts are equal because the number of free positions equals the number of missing values. We need to assign each free position a distinct missing value. However, some non-`-1` entries may map to themselves, and more importantly, some free positions might have an index that equals a missing value – but we are not forbidding that; we are simply counting bijections between the set of free positions and the set of missing values. This is exactly counting the number of perfect matchings, which is `cnt2!` if no restrictions existed. But there is a subtle restriction: some of the missing values might be equal to indices that are already used in a non-`-1` entry? Actually, a missing value `v` is one that does not appear in the input. If we assign `v` to a free position at index `i`, that is always allowed. However, the original snippet counts the number of ways to complete the permutation using the inclusion‑exclusion principle: we want to count permutations of the `cnt2` missing values over the `cnt1` free positions such that no free position at index `i` is assigned the value `i` if that value happens to be one of the missing values? Wait – the snippet treats the problem as counting derangements of `cnt2` items relative to a set of forbidden positions. Let’s reinterpret: The `-1` positions and their missing values form a bipartite matching problem. But the snippet’s `chk` logic marks both index `i` and value `t` as “used” when `t != -1`. That means if a value `v` is missing, its index `v` is not marked, so it could be a free position. Then the number of ways to assign the `cnt2` missing values to the `cnt2` free positions such that no free position at index `i` receives the value `i` is exactly the number of derangements of `cnt2` elements. The snippet computes this using the formula: sum over `i=0..cnt2` of `(-1)^i * C(cnt2, i) * fact[cnt1 - i]` where `cnt1 == cnt2`. But wait, in the snippet `cnt1` is the number of `-1` entries and `cnt2` is the number of unmarked indices (which equals the number of missing values). The inclusion‑exclusion selects `i` positions that are forced to be fixed points (i.e., assign value equal to index), then the remaining `cnt1 - i` positions can be arranged arbitrarily with the remaining values, giving `fact[cnt1 - i]` ways. So the correct interpretation is: we need to count the number of permutations of the missing values onto the free positions with no fixed point (no free position `p` gets value `p`). However, note that a fixed point is only possible if the index `p` is both free (has `-1`) and the value `p` is missing (not used elsewhere). That is exactly why only the set of `-1` positions that are also unmarked indices matter – but in the snippet, `cnt2` counts all unmarked indices, not just those that are free. Actually `chk` marks indices and values separately. If an index `i` is not marked, that means it didn’t appear as a value? Let’s analyze: The code marks `chk[i]=true` for every position `i` that is not `-1`, and also marks `chk[t]=true` for every given value `t`. So after processing, `chk[i]` is true if either position `i` is already filled (non-`-1`) or the value `i` appears in the input. An index `i` is unmarked (`!chk[i]`) only if position `i` is `-1` AND the value `i` does not appear anywhere. That is exactly the set of indices that are both free and whose value is missing – these are the positions that could potentially become fixed points. So `cnt2` counts the number of free positions whose index equals a missing value. And `cnt1` is the total number of free positions. Since the number of missing values is exactly `cnt1`, and among those missing values, exactly `cnt2` of them are equal to some free position’s index? Actually, consider a missing value `v` – if `v` is not equal to any index of a free position, then it’s missing but never a potential fixed point. However, the number of missing values equals `cnt1`, and the number of unmarked indices is `cnt2`. These are not necessarily equal – the snippet uses `cnt1` and `cnt2` separately. The formula `sum_{i=0..cnt2} (-1)^i * C(cnt2, i) * fact[cnt1 - i]` works because we choose `i` of the `cnt2` positions that are forced to be fixed points (using the missing value equal to their index), and then the remaining `cnt1 - i` free positions can be filled with the remaining `cnt1 - i` missing values in any order. This is a standard inclusion‑exclusion over the `cnt2` “bad” positions. The answer is the number of bijections between the set of all free positions and the set of all missing values such that none of the `cnt2` specific positions get their own index value. Edge cases: `cnt1` may be 0 (already a permutation) – then answer is 1. Also `cnt2` may be 0 – meaning no possible fixed points, then answer is `fact[cnt1]` because any assignment works. Time complexity: precompute factorials and combinations up to 2000 in O(2000^2) or use O(2000) for factorial, then the summation is O(cnt2) ≤ O(N). Space O(N).
