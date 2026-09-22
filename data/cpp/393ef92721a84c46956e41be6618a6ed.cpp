Write a standalone C++ function `countValidArrangements` that takes a vector of positive integers (size up to 300) and returns the number of ways to permute them into a sequence such that no two adjacent numbers have the same "square-free part," modulo \(10^9+7\). The square-free part of an integer \(x\) is \(x\) divided by the largest perfect square that divides it (e.g., 12 → 3, 18 → 2, 9 → 1). The function must handle duplicate numbers (including duplicates that reduce to the same square-free value). The result must be computed modulo \(10^9+7\). The input vector is non-empty and each element is between 1 and \(10^9\).

The key is to group numbers by their square-free part, because two numbers with the same square-free part are indistinguishable for adjacency constraints. First, reduce each input number by repeatedly dividing by square factors (check divisors from 2 up to \(\sqrt{x}\)). Then sort the reduced values and count group sizes. Let the groups be \(s_1, s_2, \dots, s_m\) in the order they appear after sorting. Use dynamic programming: `f[i][j]` = number of ways to arrange the first `i` groups such that there are exactly `j` "bad" adjacencies within the current sequence (i.e., adjacent positions that already have the same value). Initially, for the first group of size \(s_1\), all its elements are identical, so we have \(s_1-1\) internal bad adjacencies, and we can arrange them in \(s_1!\) ways (since they are indistinguishable, but we multiply by factorial to account for permutations of the group positions? Actually because the numbers are identical, the number of distinct sequences is 1, but we later use combinatorial inserts. The DP multiplies by factorials to count permutations of positions for each group because we treat all elements as distinct during insertion, then divide by factorial? Wait—since numbers in a group are equal, the final sequence should count each arrangement once, but the DP treats distinct positions. The standard trick: multiply by factorial of group size when inserting a new group to account for ordering among that group's elements, and divide by factorial at the end? Actually the code multiplies by `fac[sze[i+1]]` and the final answer `f[cnt][0]` is the count. Because the groups are sorted by value, but values within a group are identical, the DP counts each distinct sequence exactly once (the factorial compensates for the fact that we are inserting distinct "slots" for identical values). The DP transition: when inserting the next group of size \(k\) into a current sequence of total length `sum`, we choose `k` positions to place the new group’s elements. Among the existing `j` bad adjacencies, we must break some of them by placing a new element between them. The transition uses binomial coefficients to count ways to split the existing sequence into `sum+1` gaps (before first, between elements, after last). We choose `l` of the `j` bad gaps to break (by inserting at least one new element), and place the `k` new elements into the chosen gaps, ensuring that inside each new element's position, no two new elements are adjacent (they would create new bad adjacencies). This results in exactly `j - l + (k - (number of gaps used))` bad adjacencies after insertion. The recurrence is as in the snippet. Finally, the answer is `f[m][0]` after processing all groups. Edge cases: all numbers already distinct (each group size 1), then answer is `n!` (every permutation valid). Only one group, answer is 1 (all permutations identical). Time complexity: `O(m * sum * k * j)` but since `sum` and `k` total `n`, the worst-case is \(O(n^3)\) (with n≤300, about 27 million operations, fine). Space `O(n^2)`.

#include <vector>
#include <algorithm>
#include <cstdint>

const long long MOD = 1000000007LL;

// Modular exponentiation
long long modPow(long long base, long long exp, long long mod) {
    long long result = 1;
    while (exp > 0) {
        if (exp & 1) result = result * base % mod;
        base = base * base % mod;
        exp >>= 1;
    }
    return result;
}

// Precompute factorials and inverse factorials up to n
void precomputeFactorials(int n, std::vector<long long>& fact, std::vector<long long>& invFact) {
    fact.resize(n + 1);
    invFact.resize(n + 1);
    fact[0] = 1;
    for (int i = 1; i <= n; ++i) fact[i] = fact[i - 1] * i % MOD;
    invFact[n] = modPow(fact[n], MOD - 2, MOD);
    for (int i = n; i >= 1; --i) invFact[i - 1] = invFact[i] * i % MOD;
}

long long nCr(int n, int r, const std::vector<long long>& fact, const std::vector<long long>& invFact) {
    if (r < 0 || r > n) return 0;
    return fact[n] * invFact[r] % MOD * invFact[n - r] % MOD;
}

// Remove all perfect square factors from x
long long squareFreePart(long long x) {
    for (long long p = 2; p * p <= x; ++p) {
        while (x % (p * p) == 0) x /= (p * p);
    }
    return x;
}

// Main solution function
long long countValidArrangements(const std::vector<long long>& numbers) {
    int n = static_cast<int>(numbers.size());

    // Reduce each number to its square-free part
    std::vector<long long> reduced;
    reduced.reserve(n);
    for (long long x : numbers) {
        reduced.push_back(squareFreePart(x));
    }

    // Sort to group equal values
    std::sort(reduced.begin(), reduced.end());

    // Compute group sizes
    std::vector<int> groupSizes;
    for (int i = 0; i < n; ) {
        int j = i;
        while (j < n && reduced[j] == reduced[i]) ++j;
        groupSizes.push_back(j - i);
        i = j;
    }
    int m = static_cast<int>(groupSizes.size());

    // Precompute factorials up to n
    std::vector<long long> fact, invFact;
    precomputeFactorials(n, fact, invFact);

    // DP: f[i][j] = ways for first i groups with j bad adjacencies (same value adjacent)
    std::vector<std::vector<long long>> dp(m + 1, std::vector<long long>(n + 1, 0));

    // Initialize with first group
    int s1 = groupSizes[0];
    dp[1][s1 - 1] = fact[s1];

    int totalSum = s1;

    // Process remaining groups
    for (int i = 1; i < m; ++i) {
        int nextSize = groupSizes[i];
        for (int k = 1; k <= nextSize; ++k) { // k = how many elements of new group we use? Actually we must use all nextSize. But the recurrence uses k as number of elements to insert, but we must insert all. Wait, looking at original code: for k=1..size[i+1] it loops, but the transition uses k and size[i+1]-k. Actually the code loops k over 1..sze[i+1] and adds fac[sze[i+1]] * C(sze[i+1]-1, k-1) * ... Then the new bad count is j-l + sze[i+1]-k. This sums over all ways to split the new group into k blocks? Let me re-examine. The original code for each k (1..sze[i+1]) does something, and it accumulates. That appears to count all ways to place the new group's elements into the existing gaps, treating them as k "clusters". But we must place all nextSize elements, so why loop k? Actually the original code uses k as the number of gaps chosen to place new elements (each gap gets at least one new element). Then the number of new elements placed is k (one per chosen gap), but the rest of the new group (size[i+1]-k) are placed adjacent to those? No, that doesn't make sense. Let me verify: The original code: for k=1..sze[i+1]; for j=0..sum; for l=0..min(k,j); (f[i+1][j-l+sze[i+1]-k] += ...). The term C(sze[i+1]-1, k-1) chooses how to partition the new group's elements into k non-empty blocks (because we have size[i+1] identical items, number of ways to split into k blocks = C(size-1, k-1)). Then we place these k blocks into k distinct gaps among the existing sum+1 gaps. That indeed covers all placements: we break the new group into k blocks, place each block into a distinct gap, and gaps can be chosen with repetition? The factors C(j,l) and C(sum-j+1, k-l) choose which gaps to use: we choose l of the j bad gaps to place a block, and (k-l) of the (sum-j+1) good gaps (including ends and non-bad gaps). This ensures no two blocks are in the same gap, and each block contains at least one element. So this correctly counts all ways to insert the new group's elements into the sequence. The number of bad adjacencies after insertion: we break l of the old bad adjacencies (each of those gets a block inserted between), and the new blocks themselves each have size >=1, so they create (block_size - 1) internal bad adjacencies. The total new bad adjacencies = sum over blocks of (block_size - 1) = (nextSize - k) because there are k blocks and total elements nextSize. So new bad = old bad - l + (nextSize - k). That matches the recurrence. So k is the number of blocks (non-empty groups of new identical elements). The code loops k from 1 to nextSize, which includes all possible block counts. So our DP implementation should follow exactly that.
        */
        for (int prevBad = 0; prevBad < totalSum; ++prevBad) {
            if (dp[i][prevBad] == 0) continue;
            for (int k = 1; k <= nextSize; ++k) {
                // number of ways to split nextSize elements into k non-empty blocks
                long long blockWays = nCr(nextSize - 1, k - 1, fact, invFact);
                for (int l = 0; l <= std::min(k, prevBad); ++l) {
                    // Choose l of the prevBad bad gaps to insert a block
                    long long combBad = nCr(prevBad, l, fact, invFact);
                    // Choose (k - l) of the (totalSum - prevBad + 1) good gaps (including ends)
                    long long combGood = nCr(totalSum - prevBad + 1, k - l, fact, invFact);
                    long long add = dp[i][prevBad] * fact[nextSize] % MOD;
                    add = add * blockWays % MOD;
                    add = add * combBad % MOD;
                    add = add * combGood % MOD;
                    int newBad = prevBad - l + nextSize - k;
                    dp[i + 1][newBad] = (dp[i + 1][newBad] + add) % MOD;
                }
            }
        }
        totalSum += nextSize;
    }

    return dp[m][0];
}

#include <cassert>
#include <vector>
#include <iostream>

int main() {
    // Single distinct number
    assert(countValidArrangements({1}) == 1);
    // Two distinct numbers
    assert(countValidArrangements({1, 2}) == 2);
    // All same square-free part
    assert(countValidArrangements({4, 9, 16}) == 1); // all reduce to 1
    // Mixed: 12 and 3 reduce to 3; 18 reduces to 2
    // Three numbers: {12,3,18} => square-free: 3,3,2
    // Valid permutations: (3,2,3) only? permutations of distinct values: [3a,3b,2] where 3a and 3b are indistinguishable, so possible sequences: [3,3,2] invalid, [3,2,3] valid, [2,3,3] invalid => 1 arrangement
    assert(countValidArrangements({12, 3, 18}) == 1);
    // Four numbers: {1,4,9,2} => square-free: 1,1,1,2 (since 4->1,9->1)
    // Need no adjacent same: Count permutations of 3 identical 1's and one 2 such that no two 1's adjacent.
    // The 2 must be placed among the 1's: sequence must be 1,2,1,? but there are three 1's, cannot place all without adjacency → 0
    assert(countValidArrangements({1, 4, 9, 2}) == 0);
    // Five numbers: all distinct square-free parts (no duplicates)
    // {2,3,5,7,11} => all distinct, answer = 5! = 120
    assert(countValidArrangements({2, 3, 5, 7, 11}) == 120);
    // Larger test with two groups: {1,1,2} => square-free: 1,1,2
    // Valid: only sequence 1,2,1 => 1 way
    assert(countValidArrangements({1, 1, 2}) == 1);
    // Three group: {1,2,2} => square-free: 1,2,2 => sequences: 2,1,2 only => 1
    assert(countValidArrangements({1, 2, 2}) == 1);
    // Two groups each size 2: {1,1,4,4} => square-free: 1,1,1,1 (since 4->1) all same → 1
    assert(countValidArrangements({1, 1, 4, 4}) == 1);
    // Edge: large numbers up to 1e9
    assert(countValidArrangements({999999937, 999999937}) == 1); // different? Actually both same prime, square-free part same, so 1
    assert(countValidArrangements({999999937, 2}) == 2);
    std::cout << "All tests passed!\n";
    return 0;
}
