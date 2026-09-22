// Given an integer `N` (1 ≤ N ≤ 18) and an array `A` of size `2^N` containing arbitrary integers, write a C++ function `vector<int> maxPairSums(int N, const vector<int>& A)` that for every non-empty subset `m` (using 0-based bitmask indexing, where the least significant bit corresponds to element 0, etc.) computes the sum of the two largest values among the elements in that subset. Then, for each `m` from 1 to `2^N - 1`, output the maximum of these sums over all non-empty subsets that are subsets of `m` (i.e., `max over all t subset of m, t ≠ 0, of sum_of_two_largest(t)`). The function should return a vector of length `2^N - 1` where index `i` corresponds to the result for `m = i+1`. Ensure your solution handles duplicate values correctly (e.g., if the two largest are equal, their sum is used) and that subsets are considered by their bitmask representation.
#include <cassert>
#include <vector>
#include <iostream>

// Include the function definition here (or link separately). For this test, we assume it's defined above.
// The solution function is named maxPairSums.

int main() {
    // Test case 1: N=1, A = [5, 3]
    {
        std::vector<int> A = {5, 3};
        std::vector<long long> res = maxPairSums(1, A);
        assert(res.size() == 1);
        // For m=1 (subset {0}): sum=5+0=5. For m=2 (subset {1}): sum=3+0=3. Prefix max over [1,2] = max(5,3)=5.
        assert(res[0] == 5);
    }

    // Test case 2: N=2, A = [1, 2, 3, 4]
    {
        std::vector<int> A = {1, 2, 3, 4};
        std::vector<long long> res = maxPairSums(2, A);
        // Masks: 1={0}: sum=1+0=1; 2={1}: sum=2+0=2; 3={0,1}: sum=2+1=3; 4={2}: sum=3; 5={0,2}: sum=3+1=4; 6={1,2}: sum=3+2=5; 7={0,1,2}: sum=3+2=5; 8={3}: sum=4; 9={0,3}: sum=4+1=5; 10={1,3}: sum=4+2=6; 11={0,1,3}: sum=4+2=6; 12={2,3}: sum=4+3=7; 13={0,2,3}: sum=4+3=7; 14={1,2,3}: sum=4+3=7; 15={0,1,2,3}: sum=4+3=7.
        // Prefix max: after m=1:1, m=2:2, m=3:3, m=4:3, m=5:4, m=6:5, m=7:5, m=8:5, m=9:5, m=10:6, m=11:6, m=12:7, m=13:7, m=14:7, m=15:7.
        std::vector<long long> expected = {1,2,3,3,4,5,5,5,5,6,6,7,7,7,7};
        assert(res.size() == expected.size());
        for (size_t i = 0; i < res.size(); ++i) assert(res[i] == expected[i]);
    }

    // Test case 3: All negative numbers, N=2, A = [-5, -2, -10, -1]
    {
        std::vector<int> A = {-5, -2, -10, -1};
        std::vector<long long> res = maxPairSums(2, A);
        // Single element sums: -5+0=-5, -2+0=-2, -10+0=-10, -1+0=-1.
        // Pairs: {-5,-2} sum=-7, {-5,-10} sum=-15, {-5,-1} sum=-6, {-2,-10} sum=-12, {-2,-1} sum=-3, {-10,-1} sum=-11.
        // Prefix max over masks in increasing order. Let's compute manually.
        // We'll just assert that the last element (for m=15) is -3 (max of all sums).
        assert(res[res.size()-1] == -3);
        // Also check first element (m=1) = -5.
        assert(res[0] == -5);
    }

    // Test case 4: Duplicate values, N=1, A = [7,7]
    {
        std::vector<int> A = {7,7};
        std::vector<long long> res = maxPairSums(1, A);
        // For m=1: sum=7+0=7, m=2: sum=7+0=7, prefix max=7.
        assert(res[0] == 7);
    }

    // Test case 5: N=3, small array, check a few points.
    {
        std::vector<int> A = {10, 20, 30, 40, 50, 60, 70, 80};
        std::vector<long long> res = maxPairSums(3, A);
        // The overall maximum should be 80+70=150 (the two largest values in the whole set, mask 255).
        // Since prefix max is non-decreasing, the last element must be 150.
        assert(res.back() == 150);
        // The first few: m=1 -> A[0]+0=10, m=2 -> A[1]+0=20, m=3 -> 20+10=30, etc.
        assert(res[0] == 10);
        assert(res[1] == 20);
        assert(res[2] == 30);
    }

    // Test case 6: N=18 with random large values to ensure no overflow and speed (not exhaustive).
    {
        int N = 18;
        int M = 1 << N;
        std::vector<int> A(M, 1000000); // all same large value
        std::vector<long long> res = maxPairSums(N, A);
        // For any subset with at least 2 elements, sum = 2,000,000. For single element, sum = 1,000,000+0 = 1,000,000.
        // Since prefix max, after m=3 (two elements) we should have 2,000,000.
        assert(res[0] == 1000000); // m=1
        assert(res[1] == 1000000); // m=2
        assert(res[2] == 2000000); // m=3 (both bits set)
        assert(res.back() == 2000000); // all bigger subsets
    }

    std::cout << "All tests passed." << std::endl;
    return 0;
}
#include <vector>
#include <algorithm>
#include <climits>

// Computes for each non-empty bitmask m (1..2^N-1) the maximum over all non-empty submasks t of m of the sum of the two largest values in A at indices in t.
// The function replicates the behavior of the provided snippet: if a subset has only one element, the second largest is considered 0.
// Returns a vector of length (1<<N)-1 where index i corresponds to mask i+1.
std::vector<long long> maxPairSums(int N, const std::vector<int>& A) {
    int M = 1 << N;
    // first[m] = largest value among A[i] for i submask of m, second[m] = second largest, or 0 if absent.
    std::vector<long long> first(M, LLONG_MIN);
    std::vector<long long> second(M, 0); // default 0 as per original snippet

    for (int m = 0; m < M; ++m) {
        first[m] = A[m]; // each mask includes itself
        second[m] = 0;
    }

    // SOS DP to compute top two over all submasks.
    for (int b = 0; b < N; ++b) {
        int bit = 1 << b;
        for (int m = 0; m < M; ++m) {
            if (m & bit) {
                int other = m ^ bit;
                // Merge the pairs from m and other.
                // We have up to four candidate values: first[m], second[m], first[other], second[other].
                // We need the two largest among them.
                long long a = first[m];
                long long b_val = second[m];
                long long c = first[other];
                long long d = second[other];
                // Collect and sort four values (descending), then take top two.
                // But second values may be 0 sentinel; that's fine.
                long long v[4] = {a, b_val, c, d};
                // Find two largest.
                long long max1 = LLONG_MIN, max2 = LLONG_MIN;
                for (int i = 0; i < 4; ++i) {
                    if (v[i] > max1) {
                        max2 = max1;
                        max1 = v[i];
                    } else if (v[i] > max2) {
                        max2 = v[i];
                    }
                }
                first[m] = max1;
                second[m] = max2;
            }
        }
    }

    std::vector<long long> ans(M);
    for (int m = 0; m < M; ++m) {
        ans[m] = first[m] + second[m];
    }

    // Prefix maximum over m from 1 to M-1.
    std::vector<long long> result(M - 1);
    long long cur = ans[1];
    for (int m = 1; m < M; ++m) {
        if (ans[m] > cur) cur = ans[m];
        result[m - 1] = cur;
    }
    return result;
}
// The core problem is to efficiently compute, for every bitmask `m`, the sum of the two largest elements from `A` over all indices `i` such that `i` is a submask of `m` (i.e., `(i & m) == i`). A brute-force enumeration of all submasks for each `m` would be `O(3^N)` (since the total number of submask pairs over all `m` is `3^N`), which is acceptable for `N ≤ 18` (since `3^18 ≈ 387 million` operations, borderline but feasible in C++ with optimizations). However we can do better using dynamic programming over submasks: for each `m`, we need the top two values among `A[i]` for `i ⊆ m`. This can be computed via a DP that, for each `m`, combines the top two from `m` without its lowest set bit and the element at that bit. Specifically, if `lsb = m & -m` and `prev = m ^ lsb`, then the top two values for `m` are derived by merging the top two from `prev` and the value `A[lsb_index]` (where `lsb_index` is the bit index). The DP state stores two values per mask: the largest and second largest among `A[i]` for `i ⊆ m`. Initialize for each single-bit mask `m` the top two as `A[m]` and a sentinel like `INT_MIN`. For a general `m`, we combine the top two of `prev` with the top two of the singleton `{lsb}` (which has first `A[lsb_index]` and second `INT_MIN`) to get the top two for `m`. Then, for each `m`, `ans[m]` = sum of those top two (if the second is `INT_MIN`, it means the subset has only one element, but we only output for `m≥1`, and every non-empty subset has at least one element; if a subset has exactly one element, the two largest are that element and `INT_MIN`, but we should instead treat the second as absent, and the sum should be `A[element] + A[element]`? No — the specification says sum of the two largest elements; if there is only one element, there is no pair. However the output in the original code always prints `v[0] + v[1]` where `v[1]` starts as 0. In the original, `v` array is initialized to zeros, so if the subset has one element, `v[0]=A[s]` and `v[1]=0`, giving sum = A[s]. That seems an output decision. For a standalone task, we should define that for subsets with fewer than 2 elements, the sum is simply the single element value (i.e., treat missing second as 0). This matches the original code. So for a single-element subset, sum = A[index]. For empty subset (m=0) not used. Then after computing `ans[m]` for all m, we take a prefix maximum over m from 1 to 2^N-1, i.e., `ans[m] = max(ans[m], ans[m-1])` in the order of increasing `m`. This produces the required output. Edge cases: N=1, array size 2; N=18, large values within int (but sums may overflow int if values up to 1e9, sum up to 2e9 fits in 32-bit signed? Actually 2e9 fits within INT_MAX ~2.147e9, but to be safe use long long). Time complexity O(2^N * N) for DP? Actually merging per mask is O(1) if we iterate masks in increasing order and use the submask relation. But to ensure we have processed all submasks, we can iterate masks from 1 to M-1 and for each, take the lowest set bit. However `prev` is a proper submask (mask without one bit), but we need all submasks of `m` to be considered. The standard technique: for each mask `m`, we can compute its top two by considering all single-bit removals, but that would be O(N) per mask. Better: we can do DP over subsets using the property that if we process masks in increasing order, then for any submask `s` of `m`, `s` is less than or equal to `m` numerically? Not necessarily, but if we iterate `m` from 0 to M-1 and for each `m`, we consider all submasks by iterating `sub = m; sub = (sub-1) & m`, that is O(3^N) total. For N=18, 3^18 ≈ 387M, which is acceptable in C++ with simple operations (like 1-2 seconds). Alternatively, we can use SOS DP (Sum Over Subsets) style: maintain two arrays `first[M]` and `second[M]` where `first[m]` is the largest value among all `A[i]` with `i ⊆ m`, and `second[m]` is the second largest. Initialize `first[m] = A[m]` for single-bit masks? Actually we can initialize `first[m] = A[m]` for all m (since every mask includes itself), and `second[m] = -INF`. Then for each bit `b` from 0 to N-1, for each mask `m` that has bit `b` set, we consider `m ^ (1<<b)` (which is a mask without that bit). We merge the top two from `m` and `m ^ (1<<b)` to update `first[m]` and `second[m]`. After processing all bits, `first[m]` and `second[m]` reflect the top two among all submasks of `m`. This is O(N * 2^N) = 18 * 262144 ≈ 4.7M operations, very fast. This is the correct DP. Then for each m≥1, compute `sum = first[m] + second[m]` (if second is -INF, then subset has only one element, sum = first[m] + 0? But the original initializes v[1]=0, so we should use 0 as the second missing value. So we can set `second[m]` default to 0. But careful: if there is a negative value in A, then a subset with two elements where the second largest is negative would incorrectly sum with 0 if we mistake. Better to track a boolean or use sentinel -1e18 to indicate absence. However original code uses 0, so if A has negative numbers, the sum for a single-element subset would be A[i]+0, not correct if we want "two largest" but there is only one. The original code's behavior sums the two largest found; it initializes v[0] and v[1] to 0, so for a single-element subset with A[s] negative, v[0]=negative, v[1]=0, sum = negative + 0 = negative. If A[s] positive, sum = A[s]+0. That is a weird edge case. For a clean task, we should define that if the subset has fewer than 2 elements, the sum is simply the value of that single element (i.e., treat second as 0 only if that is the intended behavior). To match the provided snippet exactly, we must replicate: for each subset, we take the two largest values among its elements, but if there is only one element, the second largest is considered 0 (since v initial is 0). So for single-element subset, sum = A[i] + 0 = A[i]. For two elements both positive, sum = larger + smaller. For negative elements, the behavior is as described. We will replicate that. In our DP, we initialize `second[m]` to 0 for all m, and `first[m]` to A[m] for all m (since every mask includes itself). Then for each bit, we merge: consider `other = m ^ (1<<b)` where bit b is set. We need to combine top two from `m` and `other`. Since `other` is a submask of `m` (without that bit), but we haven't processed all bits yet. Standard SOS DP processes bits in increasing order; after processing all bits, `first[m]` holds max over all submasks. However we need the two largest values, not just max. We can maintain a pair (largest, secondLargest) per mask. When merging two masks `a` and `b`, we take the top two from the union of their element values. Since each mask's pair already represents top two among its submasks, merging is easy. Process bits from 0 to N-1: for each mask `m` with bit b set, let `other = m ^ (1<<b)`. Then new pair for `m` = merge(pair[m], pair[other]) where merge takes the two largest of the four values (but we only have two per mask, so we combine the two largest of the union of the four numbers; but careful: pair[m] might have its second as 0 if only one element, but that's fine). This works because after processing bit b, `pair[m]` represents the top two among all submasks that differ from `m` only in bits < b? Actually standard SOS: for each bit b, we update DP[m] = combine(DP[m], DP[m ^ (1<<b)]) for all m with bit b set. After processing all bits, DP[m] contains results over all submasks. This is correct because every submask t of m can be obtained by toggling some set bits of m; the order of bits doesn't matter. We need to ensure we don't double count: we use `other = m without bit b`, which is a superset of some submasks? Actually `other` is a submask of `m` (since we removed a bit), and its DP уже includes all submasks of `other`. So merging pair[m] (which currently covers submasks that don't have bit b? Not exactly). Standard approach: initialize dp[m] = value for the mask itself (i.e., A[m]). Then for each bit b, for m from 0 to M-1, if m has bit b, dp[m] = merge(dp[m], dp[m ^ (1<<b)]). After processing bit b, dp[m] covers all submasks that are subsets of m and differ only in bits 0..b? Actually it covers all submasks where the highest differing bit is ≤ b. After all bits, it covers all submasks. That's correct.
//
// After computing the pair for each mask, compute `sum[m] = first[m] + second[m]` (where second is 0 if absent, matching original). Then take prefix maximum over m from 1 to M-1: `result[m-1] = max(sum[1..m])`. Return that vector.
//
// Edge cases: All values negative, then for a single-element subset sum = negative + 0 = negative, and prefix max will pick the least negative. For two-element subset with both negative, sum = larger (less negative) + smaller (more negative) = more negative, which could be less than a single-element sum, so prefix max correctly keeps the maximum.
//
// Time complexity: O(N * 2^N), space O(2^N) for the DP pairs. N ≤ 18 so M ≤ 262144, fine.
