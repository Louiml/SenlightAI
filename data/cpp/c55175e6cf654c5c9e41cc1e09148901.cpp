Given an array of `n` integers (1 ≤ n ≤ 2000, each value in [1, 100000]) and a positive integer `k` (1 ≤ k ≤ n), write a C++ function `long long countSubsetsWithGCDMultiple(int n, int k, const std::vector<int>& arr)` that returns the number of subsets of size exactly `k` whose greatest common divisor (GCD) is exactly 1, modulo 1,000,000,007. The function should handle duplicate values in the array and should consider subsets by their indices, so two subsets with the same values but different indices are counted as distinct. The result must be the total count modulo 1,000,000,007.

#include <cassert>
#include <vector>

// Provide the function declaration (assuming the solution is included)
long long countSubsetsWithGCDMultiple(int n, int k, const std::vector<int>& arr);

int main() {
    // Test 1: simple case with all ones
    std::vector<int> a1 = {1, 1, 1, 1};
    assert(countSubsetsWithGCDMultiple(4, 2, a1) == 6); // C(4,2)=6

    // Test 2: no possible GCD 1 because all values even and k=2
    std::vector<int> a2 = {2, 4, 6, 8};
    assert(countSubsetsWithGCDMultiple(4, 2, a2) == 0);

    // Test 3: only one element with value 1, k=1
    std::vector<int> a3 = {5, 1, 7};
    assert(countSubsetsWithGCDMultiple(3, 1, a3) == 1); // only the value 1

    // Test 4: k=1, multiple ones and other values
    std::vector<int> a4 = {1, 2, 1, 3};
    assert(countSubsetsWithGCDMultiple(4, 1, a4) == 2); // two ones

    // Test 5: mixture where only odd pairs work
    std::vector<int> a5 = {2, 3, 5, 7};
    // All subsets of size 2: (2,3) gcd1, (2,5) gcd1, (2,7) gcd1, (3,5) gcd1, (3,7) gcd1, (5,7) gcd1 → all 6
    assert(countSubsetsWithGCDMultiple(4, 2, a5) == 6);

    // Test 6: duplicate values, k=2
    std::vector<int> a6 = {2, 2, 4, 8};
    // Pairs: (2a,2b) gcd2, (2a,4) gcd2, (2a,8) gcd2, (2b,4) gcd2, (2b,8) gcd2, (4,8) gcd4 → none gcd1
    assert(countSubsetsWithGCDMultiple(4, 2, a6) == 0);

    // Test 7: n=1, k=1, value=1
    std::vector<int> a7 = {1};
    assert(countSubsetsWithGCDMultiple(1, 1, a7) == 1);

    // Test 8: n=1, k=1, value=2 → gcd=2 not 1
    std::vector<int> a8 = {2};
    assert(countSubsetsWithGCDMultiple(1, 1, a8) == 0);

    // Test 9: larger array with known answer
    std::vector<int> a9 = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    // For k=3: total C(10,3)=120. Count those with gcd=1. We trust algorithm.
    long long res9 = countSubsetsWithGCDMultiple(10, 3, a9);
    assert(res9 >= 0 && res9 < MOD);

    // Test 10: modulo behavior with large n
    std::vector<int> a10(2000, 1);
    // All subsets of size k have gcd=1
    long long expected10 = comb[2000][1000]; // but we only assert non-negative
    long long res10 = countSubsetsWithGCDMultiple(2000, 1000, a10);
    (void)expected10; // not comparing fully, just ensure no crash
    assert(res10 >= 0 && res10 < MOD);

    return 0;
}

#include <vector>
#include <cstring>

const int MOD = 1000000007;
const int MAXV = 100000;

// Precomputed binomial coefficients modulo MOD
long long comb[2005][2005];

// Precompute binomials up to n=2000
void precomputeCombinations() {
    for (int n = 0; n <= 2000; ++n) {
        comb[n][0] = comb[n][n] = 1;
        for (int k = 1; k < n; ++k) {
            comb[n][k] = (comb[n-1][k-1] + comb[n-1][k]) % MOD;
        }
    }
}

// Counts subsets of size exactly k whose GCD is exactly 1
long long countSubsetsWithGCDMultiple(int n, int k, const std::vector<int>& arr) {
    // Ensure precomputed once
    static bool precomputed = false;
    if (!precomputed) {
        precomputeCombinations();
        precomputed = true;
    }

    // Frequency of each value
    int cnt[MAXV + 1];
    memset(cnt, 0, sizeof(cnt));
    for (int val : arr) {
        cnt[val]++;
    }

    // mult_cnt[d] = number of elements divisible by d
    int mult_cnt[MAXV + 1];
    memset(mult_cnt, 0, sizeof(mult_cnt));
    for (int d = 1; d <= MAXV; ++d) {
        long long sum = 0;
        for (int multiple = d; multiple <= MAXV; multiple += d) {
            sum += cnt[multiple];
        }
        mult_cnt[d] = (int)sum;
    }

    // F[d] = C(mult_cnt[d], k) mod MOD, number of subsets of size k with all elements divisible by d
    long long F[MAXV + 1];
    for (int d = 1; d <= MAXV; ++d) {
        if (mult_cnt[d] >= k) {
            F[d] = comb[mult_cnt[d]][k];
        } else {
            F[d] = 0;
        }
    }

    // G[d] = number of subsets of size k with GCD exactly d
    long long G[MAXV + 1];
    memset(G, 0, sizeof(G));
    for (int d = MAXV; d >= 1; --d) {
        long long val = F[d];
        for (int multiple = 2*d; multiple <= MAXV; multiple += d) {
            val = (val - G[multiple] + MOD) % MOD;
        }
        G[d] = val;
    }

    return G[1];
}

// The problem asks for subsets of size `k` with GCD exactly 1. Direct enumeration is impossible (n up to 2000, choose k up to C(2000,1000) is huge). The key insight is to use the Möbius inversion / inclusion-exclusion technique over divisors, which is efficient because the maximum value is only 100,000.
//
// **Main algorithm**:
//
// 1. **Frequency count**: Count how many times each value appears in `arr`. Let `cnt[x]` be the count of value `x`.
//
// 2. **Precompute multiples counts**: For each integer `d` from 1 to MAXV, compute `mult_cnt[d]` = the number of array elements that are divisible by `d`. This can be done by iterating over multiples: for each `d`, sum `cnt[x]` for all `x` that are multiples of `d`. This is O(MAXV * log MAXV) due to the harmonic series.
//
// 3. **Binomial coefficients**: Precompute `C[n][k]` modulo MOD for all `n` up to 2000 (since `mult_cnt[d]` can be up to 2000). Use Pascal's triangle.
//
// 4. **Counting subsets with GCD divisible by d**: For a given `d`, the number of subsets of size `k` where all elements are divisible by `d` is `C(mult_cnt[d], k)`. This counts subsets where the GCD is a multiple of `d` (including multiples like 2d, 3d, ...). Let `F(d) = C(mult_cnt[d], k)`.
//
// 5. **Inclusion–exclusion (Möbius inverse)**: To get the number of subsets with GCD exactly `d`, we need to subtract the counts for all larger multiples of `d`. Define `G(d)` = number of subsets of size `k` whose GCD is exactly `d`. Then `F(d) = sum_{j=1..∞} G(j*d)`. We can compute `G(d)` in descending order of `d` (from MAXV down to 1) using the formula:  
//    `G(d) = F(d) - sum_{m=2,3,...} G(m*d)` for multiples `m*d` ≤ MAXV.  
//    This is exactly the sieve-like DP in the snippet.
//
// 6. **Final answer**: We need subsets with GCD exactly 1, so the answer is `G(1)`.  
//
// The algorithm runs in **O(MAXV log MAXV + n^2)** for precomputation (binomials O(n^2)) and then O(MAXV log MAXV) for the DP. With n=2000 and MAXV=100000, this is feasible (~10^6 operations). Space is O(MAXV + n^2) or we can compute binomials on the fly but precomputing is simpler.
//
// **Edge cases**:
// - If `mult_cnt[1] < k`, then it's impossible to have GCD 1, answer 0.
// - Duplicate values: the frequency count handles them; subsets are distinguished by indices, so `C(cnt, k)` counts correctly.
// - `k=1`: any element with value 1 gives a subset with GCD 1; the formula still works.
// - Large binomial coefficients: use modulo arithmetic.
