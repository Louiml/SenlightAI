// Given an array of `n` integers where each value is between `0` and `n` inclusive (with possible duplicates), and two parameters `m` (number of items to choose) and `k` (a non-negative integer), write a C++ function that returns the total number of ways to select `m` distinct elements from the array such that the maximum value among the selected elements is `x` and all selected elements are within the range `[x-k, x]`. More precisely, for each distinct value `x` that appears in the array, count the number of `m`-element subsets whose maximum is exactly `x` and whose minimum is at least `x-k`. The total over all valid `x` is returned modulo `1,000,000,007`. The function takes as input: `n`, `m`, `k`, and a vector `freq` of size `n+1` where `freq[v]` is the number of occurrences of value `v` in the original array. You may assume `n >= 1`, `m >= 1`, `k >= 0`, and that the total sum of `freq` equals `n`. The result must be computed modulo the prime `1,000,000,007`.
We precompute factorials up to `n` modulo `MOD` to allow fast binomial coefficient `nCr(num, r)` using modular inverse via Fermat's little theorem (`bigmod(base, MOD-2)`). For each distinct value `maxNum` from `1` to `n` (since `0` can never be the maximum of a non-empty selection), we compute the number of available elements less than or equal to `maxNum-1` but greater than or equal to `maxNum-k` (i.e., the count of elements `v` with `maxNum-k <= v <= maxNum-1`). This is done using a prefix sum array `csFreq` over `freq`. Let `lessCount` be that count. For each possible `toInclude` from `0` to `freq[maxNum]-1` (we must include at least one occurrence of `maxNum` to make it the maximum, but the problem statement from the snippet iterates `toInclude` from `0` to `freq[maxNum]` considering selections where `maxNum` might not be included? Actually in the snippet it loops `toInclude` from `0` to `freq[maxNum]` but uses `nLessThan+toInclude < m-1` as a continue condition, and then adds `nCr(nLessThan+toInclude, m-1)`. This counts selections where the total number of chosen elements from the "less than or equal to maxNum" pool is `m-1` after picking `toInclude` copies of `maxNum`. However, to have maximum exactly `maxNum`, we must include at least one `maxNum`. The snippet's loop starts at `0` but the condition `nLessThan+toInclude < m-1` skips invalid counts; when `toInclude=0`, we are counting subsets that contain no `maxNum`, which would not have maximum `maxNum` unless `m-1` elements are chosen from less-than values and `maxNum` is not included, but then maximum would be less. Actually the snippet's logic: For each `maxNum`, it computes `nLessThan` = count of values in `[maxNum-k, maxNum-1]`. Then for each `toInclude` from `0` to `freq[maxNum]-1` (the snippet uses `freq[maxNum]` but that is inclusive? It uses `For (toInclude, 0, freq[maxNum])` which goes from 0 to freq-1? Actually `For` macro is `for(i=x;i<y;i++)` so it goes 0 to `freq[maxNum]-1`). It then checks if `nLessThan+toInclude < m-1` then continue. Then adds `nCr(nLessThan+toInclude, m-1)`. This counts the number of ways to choose `m-1` other elements from the pool consisting of `nLessThan` elements (from `[maxNum-k, maxNum-1]`) plus `toInclude` copies of `maxNum` (but since `maxNum` copies are identical, choosing `toInclude` of them gives only 1 way per combination of less-than elements). However, this overcounts because when `toInclude` is 0, we are not including `maxNum` at all, but that would give maximum less than `maxNum`. To fix, we should start `toInclude` from 1, not 0. But the snippet's logic might be intended for counting subsets where the maximum is at most `maxNum` and at least `maxNum-k`? Let's interpret the problem clearly: We want the number of subsets of size `m` such that the maximum element in the subset is exactly `x` and the minimum is at least `x-k`. The snippet counts for each `maxNum` the sum over `toInclude` of `nCr(nLessThan+toInclude, m-1)` but that is actually the number of ways to choose `m-1` elements from the set of all elements that are ≤ `maxNum` and ≥ `maxNum-k`, including `toInclude` copies of `maxNum` chosen from the `freq[maxNum]` occurrences. However, to have maximum exactly `maxNum`, we must include at least one `maxNum`. So the correct loop should be `toInclude` from 1 to `freq[maxNum]`. But the snippet uses from 0, which would count subsets whose maximum is strictly less than `maxNum` as well, which is wrong. Possibly the snippet is for a different problem (maybe counting subsets with maximum ≤ maxNum and minimum ≥ maxNum-k). To create a coherent task, we must define a clear spec. I'll define the task as: "Given frequency array, count the number of subsets of size `m` such that the maximum element in the subset is exactly `x` and the minimum element is at least `x-k` for some `x` from 1 to n. Sum over all x, modulo MOD." This matches the typical "number of ways to choose m numbers where the maximum is exactly x and the smallest is at least x-k". The solution: For each `x`, let `A` = count of values in `[max(0, x-k), x-1]` (since values are non-negative). Let `B = freq[x]`. We need to choose at least 1 occurrence of `x` and the rest from the `A` values plus remaining `B-1` copies of `x`? Actually, to have maximum exactly `x`, we must include at least one `x`. The other `m-1` elements can be any from the set of values in `[x-k, x-1]` (which has count `A`) and also additional copies of `x` (but including more than one copy of `x` is allowed). However, if we include an additional copy of `x`, the maximum is still `x`. So the total pool of allowed elements for the other `m-1` slots is `A + (freq[x]-1)` because we already use one `x`, and the remaining `freq[x]-1` copies of `x` can also be chosen. But the problem might consider distinct values? Usually "elements" are positions, so copies are distinct. The snippet treats each occurrence as distinct by using binomial coefficients on counts of identical values. So the number of ways to choose `m-1` additional elements from `A + freq[x] - 1` distinct occurrences? But the snippet does `nCr(nLessThan+toInclude, m-1)` where `toInclude` goes from 0 to `freq[x]-1` and `nLessThan` is the count of values in `[x-k, x-1]`. This is summing over `toInclude` (number of additional copies of `x` included beyond the first) the number of ways to choose `m-1` total elements from `nLessThan` (distinct values, each with exactly one occurrence? Wait, `freq` gives counts per value, but the snippet uses `nLessThan` as a count of elements (not distinct values) because `csFreq` is cumulative frequencies, so `csFreq[maxNum-1]-csFreq[max(0,maxNum-k-1)]` gives the total number of occurrences of values in that range. So `nLessThan` is the total count of individual elements (with duplicates counted) in the range `[maxNum-k, maxNum-1]`. Then for each `toInclude` (number of copies of `maxNum` chosen among the `freq[maxNum]`), it computes `nCr(nLessThan+toInclude, m-1)`. That counts the number of subsets of size `m-1` from the pool of elements consisting of all elements in `[maxNum-k, maxNum-1]` plus `toInclude` specific copies of `maxNum` (but since all copies are identical, choosing `toInclude` copies from `freq[maxNum]` gives `C(freq[maxNum], toInclude)` multiplied by `nCr(nLessThan, m-1-toInclude)`. The snippet's formula `nCr(nLessThan+toInclude, m-1)` is incorrect because it treats the `toInclude` copies as distinct but also counts combinations where the chosen `toInclude` copies are not fixed; actually the sum over `toInclude` of `C(freq, toInclude)*C(nLessThan, m-1-toInclude)` equals `C(nLessThan+freq, m-1)` by Vandermonde's identity. But the snippet does not multiply by `C(freq, toInclude)`, so it's wrong. Given the snippet is likely buggy, we must design a correct task. I'll define the task as simpler: Given an array of counts `freq[0..n]`, compute the total number of ways to select `m` distinct indices (i.e., elements) such that the maximum value among selected is exactly `x` and the minimum is at least `x-k`. Since elements are distinct by their positions even if values are equal, the total number of subsets is sum over x of (number of ways to choose at least one occurrence of x and the rest from the pool of occurrences with values in [x-k, x]). The correct formula: For each x, let `low = max(0, x-k)`. Let `C1 = sum_{v=low}^{x-1} freq[v]` (total occurrences of values less than x but >= low). Let `C2 = freq[x]`. The number of subsets of size m with maximum exactly x is `sum_{a=1}^{m} C(C2, a) * C(C1, m-a)` where `a` is the number of chosen copies of x, and `m-a` chosen from the pool of C1 elements. Since we require at least one x, a>=1. So total ways = sum_{x=1}^{n} sum_{a=1}^{min(m, C2)} C(C2, a) * C(C1, m-a). This is the correct combinatorial count. The snippet's approach of iterating `toInclude` from 0 is flawed. So I will create the task with the correct definition, and the solution will follow this formula. I'll also include edge cases: if `m` > total available elements, answer 0; if `k` is large, low becomes 0; if `C1` not enough for `m-a`, skip. Complexity O(n * m) if done naively, but we can optimize using precomputed factorials and iterate over possible `a` for each x, which is O(n * m) worst-case, but m could be up to n. However, we can do it in O(n * m) which is fine for n up to ~2000, but for larger we might need better. Since the snippet uses maxn=200005, we can assume n up to 200000, m up to n. O(n*m) would be too slow if m ~ n ~ 200k. So we need a smarter approach. Observe that for each x, sum_{a=1}^{min(m,C2)} C(C2,a)*C(C1,m-a) = C(C1+C2,m) - C(C1,m) (by Vandermonde, subtract the term with a=0). So total ways = sum_{x=1}^{n} [ C(totalAvailableInRange, m) - C(C1, m) ] where totalAvailableInRange = C1 + C2 = sum_{v=low}^{x} freq[v]. This simplification is exactly what the snippet attempts but with incorrect handling: The snippet loops `maxNum` and for each `toInclude` from 0 to freq-1, it adds `nCr(nLessThan+toInclude, m-1)` – that is not the same as `C(C1+C2, m) - C(C1,m)`. Actually the snippet's sum over `toInclude` from 0 to freq-1 of `C(nLessThan+toInclude, m-1)` equals? Let `A = nLessThan`, `B = freq[x]`. Sum_{t=0}^{B-1} C(A+t, m-1). That is not a standard identity. So definitely buggy. Thus I'll design the task based on the correct formula. Given the complexity, we can compute for each x the value `C(prefixSumUpToX - prefixSumBeforeLow, m) - C(prefixSumUpToX-1 - prefixSumBeforeLow, m)` where `prefixSumUpToX` = sum_{v=0}^{x} freq[v], and `prefixSumBeforeLow` = sum_{v=0}^{low-1} freq[v]. Let `totalRange = sum_{v=low}^{x} freq[v]`, `lessThanX = sum_{v=low}^{x-1} freq[v]`. Then contribution = C(totalRange, m) - C(lessThanX, m). Sum over x from 1 to n. This is O(n) after prefix sums. That's the intended solution. Edge cases: if m=0? The problem says m>=1. If totalRange < m then C=0. If lessThanX < m then C=0. Also values start at 0, but x must be at least 1 because maximum cannot be 0 if we choose positive? Actually if array contains zeros, maximum could be 0 only if all selected are 0. But x ranges 1..n in snippet. To be safe, include x=0? But the snippet iterates from 1 to n. We'll define x from 1 to n because maximum 0 would require all selected be 0, and if m>0, that's possible. Let's include x=0 as well. But to keep consistent with snippet, I'll define x from 0 to n. However, the problem statement says values between 0 and n inclusive. We'll iterate x=0..n. For x=0, low = max(0,0-k)=0, so range [0,0] includes only zeros. Contribution = C(freq[0], m) - C(0, m) = C(freq[0], m) because lessThanX=0. That counts subsets of size m all zeros. That is correct. So we iterate x from 0 to n. Also k can be large, so low=0. Complexity O(n) time, O(n) space for factorial and prefix sums. Mod operations. We'll implement a function `countSubsets(int n, int m, int k, const vector<int>& freq)` returning long long mod. Precompute factorials up to n globally inside the function (static local) or pass. Since the solution must be a free function without main, we can define static factorial array of size maxN (e.g., 200005) and a helper to compute mod inverse. We'll use `long long` for safety.
#include <bits/stdc++.h>
using namespace std;

static const long long MOD = 1000000007LL;
static const int MAXN = 200005;

static long long fact[MAXN];

// Fast exponentiation modulo MOD
static long long modPow(long long base, long long exp) {
    long long result = 1;
    base %= MOD;
    while (exp > 0) {
        if (exp & 1) result = (result * base) % MOD;
        base = (base * base) % MOD;
        exp >>= 1;
    }
    return result;
}

// Modular inverse using Fermat's little theorem
static long long modInverse(long long val) {
    return modPow(val, MOD - 2);
}

// Binomial coefficient C(n, r) modulo MOD, returns 0 if r<0 or r>n
static long long nCr(int n, int r) {
    if (r < 0 || r > n) return 0;
    long long res = fact[n];
    res = res * modInverse(fact[r]) % MOD;
    res = res * modInverse(fact[n - r]) % MOD;
    return res;
}

// Precompute factorials up to MAXN-1
static void initFactorials() {
    fact[0] = 1;
    for (int i = 1; i < MAXN; ++i) {
        fact[i] = fact[i - 1] * i % MOD;
    }
}

// Main solution function
// freq[v] = number of occurrences of value v in the array, for v=0..n
// We need to count subsets of size m such that maximum is exactly x and minimum >= x-k.
// Sum over all possible x (0 <= x <= n). Return the count modulo MOD.
long long countValidSubsets(int n, int m, int k, const vector<int>& freq) {
    initFactorials(); // safe if called once; but we call each time, can use static flag
    static bool factorialReady = false;
    if (!factorialReady) {
        initFactorials();
        factorialReady = true;
    }

    // Prefix sum of frequencies: pref[i] = sum_{v=0}^{i} freq[v]
    vector<long long> pref(n + 1, 0);
    pref[0] = freq[0];
    for (int i = 1; i <= n; ++i) {
        pref[i] = pref[i - 1] + freq[i];
    }

    long long totalWays = 0;

    for (int x = 0; x <= n; ++x) {
        int low = max(0, x - k);
        // total occurrences in [low, x]
        long long totalRange = pref[x] - (low > 0 ? pref[low - 1] : 0);
        // total occurrences in [low, x-1] (i.e., less than x)
        long long lessThanX = totalRange - freq[x];

        long long waysAll = nCr((int)totalRange, m);       // subsets choosing only from [low, x]
        long long waysWithoutX = nCr((int)lessThanX, m);   // subsets choosing only from [low, x-1]

        long long contrib = (waysAll - waysWithoutX + MOD) % MOD;
        totalWays = (totalWays + contrib) % MOD;
    }

    return totalWays;
}
#include <bits/stdc++.h>
#include <cassert>
using namespace std;

// Declaration of the function under test (assume it is provided above)
long long countValidSubsets(int n, int m, int k, const vector<int>& freq);

int main() {
    // Test 1: n=3, m=1, k=0, freq = [1,1,1] (values 0,1,2 each once)
    // Subsets of size 1: maximum exactly x means just pick x, and minimum >= x-0 so any single element is valid.
    // All 3 singletons are valid -> answer 3.
    vector<int> f1 = {1, 1, 1};
    assert(countValidSubsets(2, 1, 0, f1) == 3);

    // Test 2: n=3, m=2, k=1, freq = [1,1,1]
    // Valid pairs: (0,1) max=1 min=0>=0? k=1 so min>=max-1=0 ok; (1,2) max=2 min=1>=1 ok; (0,2) max=2 min=0>=1? 0>=1 false, invalid.
    // So 2 ways.
    vector<int> f2 = {1, 1, 1};
    assert(countValidSubsets(2, 2, 1, f2) == 2);

    // Test 3: n=1, m=2, k=5, freq = [0,2] (two copies of value 1)
    // Only possible subset of size 2 is {1,1}, max=1 min=1, low=0, range [0,1] includes both, lessThanX=0
    // waysAll = C(2,2)=1, waysWithoutX = C(0,2)=0 -> contrib=1.
    vector<int> f3 = {0, 2};
    assert(countValidSubsets(1, 2, 5, f3) == 1);

    // Test 4: n=0, m=1, k=0, freq = [2] (two zeros)
    // Only x=0, range[0,0] total=2, less=0, waysAll=C(2,1)=2, waysWithoutX=0 -> answer 2.
    vector<int> f4 = {2};
    assert(countValidSubsets(0, 1, 0, f4) == 2);

    // Test 5: Large n to check modulo with repeated numbers
    // n=2, freq = [1,3], m=2, k=0
    // x=0: range[0,0] total=1, less=0, waysAll=C(1,2)=0 -> 0
    // x=1: range[1,1] total=3, less=0, waysAll=C(3,2)=3 -> 3
    // x=2: range[2,2] total=0? freq[2]=0, but we have n=2, freq[2] default 0, total=0 -> 0
    // So answer 3 (pairs from three copies of 1).
    vector<int> f5 = {1, 3, 0};
    assert(countValidSubsets(2, 2, 0, f5) == 3);

    // Test 6: k large enough to include all smaller values
    // n=2, freq=[1,1,1], m=2, k=10
    // All pairs: (0,1),(0,2),(1,2) all have min >= max-10 -> valid, answer 3.
    vector<int> f6 = {1, 1, 1};
    assert(countValidSubsets(2, 2, 10, f6) == 3);

    // Test 7: m > total possible elements with one x range
    // n=1, freq=[0,1], m=2, k=0 -> only one element, cannot pick 2 -> 0
    vector<int> f7 = {0, 1};
    assert(countValidSubsets(1, 2, 0, f7) == 0);

    // Test 8: all same value multiple copies, m=3
    // n=1, freq=[0,5], m=3, k=1 -> choose 3 out of 5 copies of 1 -> C(5,3)=10
    vector<int> f8 = {0, 5};
    assert(countValidSubsets(1, 3, 1, f8) == 10);

    // Test 9: edge with zero included and k=0
    // n=1, freq=[2,0], m=1, k=0 -> only x=0, choose one of two zeros ->2
    vector<int> f9 = {2, 0};
    assert(countValidSubsets(1, 1, 0, f9) == 2);

    // Test 10: mixed, n=3, freq=[0,2,1], m=2, k=1
    // possible x=1: range[0,1] total=freq[0]+freq[1]=0+2=2, less=0 -> waysAll=C(2,2)=1
    // x=2: range[1,2] total=freq[1]+freq[2]=2+1=3, less=2 -> waysAll=C(3,2)=3, waysWithoutX=C(2,2)=1 -> contrib=2
    // x=3: freq[3]=0 -> 0. Total=1+2=3
    vector<int> f10 = {0, 2, 1};
    assert(countValidSubsets(2, 2, 1, f10) == 3);

    return 0;
}
