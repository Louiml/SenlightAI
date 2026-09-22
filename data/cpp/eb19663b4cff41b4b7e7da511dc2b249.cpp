Given a sequence of `n` integers `a[0], a[1], ..., a[n-1]` and a step size `m`, you need to find an integer constant `c` such that the number of positions `i` where `a[i] == c + i*m` is maximized. If there are multiple such constants, choose the smallest one. After determining the optimal `c`, your function should output the total number of changes needed (i.e., `n - (count of already matching positions)`) and then list the operations required to transform the original sequence into the target sequence `b[i] = c + i*m`. For each position `i`, if `a[i] > b[i]`, output a line "- index value" meaning decrease element at that 1-based index by `value`; if `a[i] < b[i]`, output "+ index value" meaning increase it. If `a[i] == b[i]`, no line is output for that position. Implement a function that takes as input `n`, `m`, and a vector of integers `a`, and returns a pair: the first element is the minimal number of changes, and the second is a vector of strings representing the operations in the exact format described. All integers fit in 32-bit signed range. If no operation is needed, return an empty vector for operations.
#include <bits/stdc++.h>
#include <cassert>

// Include the solution function here (or declare it above)

int main() {
    // Test 1: Simple case, all match with c=5, m=2
    std::vector<long long> a1 = {5, 7, 9, 11};
    auto res1 = minimizeAdjustments(4, 2, a1);
    assert(res1.first == 0);
    assert(res1.second.empty());

    // Test 2: One change needed
    std::vector<long long> a2 = {5, 8, 9, 11};
    auto res2 = minimizeAdjustments(4, 2, a2);
    assert(res2.first == 1);
    assert(res2.second.size() == 1);
    // The only mismatch is at index 2 (0-based) where a[2]=8, target=5+2*2=9 → need +1
    assert(res2.second[0] == "+ 3 1");

    // Test 3: Negative constants
    std::vector<long long> a3 = {-3, -1, 1, 3};
    auto res3 = minimizeAdjustments(4, 2, a3);
    // Correct c = -3 (since -3+0*2=-3, -1? Actually -1-1*2=-3, 1-2*2=-3, 3-3*2=-3)
    assert(res3.first == 0);
    assert(res3.second.empty());

    // Test 4: Tie break with smallest c
    // Two possible c values each appear twice: c=0 (from i=0 and i=2) and c=1 (from i=1 and i=3)
    std::vector<long long> a4 = {0, 1, 0, 1}; // m=1
    auto res4 = minimizeAdjustments(4, 1, a4);
    // For c=0: targets {0,1,2,3} → only indices 0,1 match → 2 changes
    // For c=1: targets {1,2,3,4} → only indices 0,1? Wait a[0]=0 !=1, a[1]=1 ==2? no. Actually compute:
    // i=0: a=0, c=0 → match; i=1: a=1, c=0 → match; i=2: a=0, c=0? Actually c = a[2]-2*1 = -2 ≠0. So c=0 appears from i=0 and i=1? Let's recalc: For m=1:
    // i=0: c=0, i=1: c=0, i=2: c=-2, i=3: c=-2. So c=0 appears twice, c=-2 appears twice. Smallest c is -2. That gives targets: -2,-1,0,1. Indices matching: i=2 (0? a[2]=0 == -2+2=-? actually -2+2*1=0, yes) and i=3 (a[3]=1 == -2+3=1, yes). So 2 changes.
    assert(res4.first == 2);
    assert(res4.second.size() == 2);

    // Test 5: All different, choose the most frequent
    std::vector<long long> a5 = {10, 20, 30, 40, 50};
    auto res5 = minimizeAdjustments(5, 5, a5);
    // c values: 10, 15, 20, 25, 30 → each appears once → choose smallest c=10
    // targets: 10,15,20,25,30 → matches only index0 → 4 changes
    assert(res5.first == 4);
    assert(res5.second.size() == 4);

    // Test 6: Large m, ensure 64-bit correctness
    std::vector<long long> a6 = {1000000000LL, 2000000000LL, 3000000000LL};
    auto res6 = minimizeAdjustments(3, 1000000000LL, a6);
    // c values: 1e9, 1e9, 1e9 → all match → 0 changes
    assert(res6.first == 0);
    assert(res6.second.empty());

    // Test 7: Duplicates with negative difference
    std::vector<long long> a7 = {0, 0, 0};
    auto res7 = minimizeAdjustments(3, 2, a7);
    // c values: 0, -2, -4 → each appears once, smallest c = -4
    // targets: -4,-2,0 → matches only index2 → 2 changes
    assert(res7.first == 2);
    assert(res7.second.size() == 2);

    // Test 8: Edge case n=1
    std::vector<long long> a8 = {7};
    auto res8 = minimizeAdjustments(1, 3, a8);
    assert(res8.first == 0);
    assert(res8.second.empty());

    // Test 9: Negative m? Not in spec, but assume m can be any integer. For m=0, target constant for all indices.
    std::vector<long long> a9 = {5, 5, 5, 5};
    auto res9 = minimizeAdjustments(4, 0, a9);
    // c values: all 5 → matches all → 0 changes
    assert(res9.first == 0);
    assert(res9.second.empty());

    // Test 10: Mixed signs
    std::vector<long long> a10 = {1, -1, 1, -1};
    auto res10 = minimizeAdjustments(4, 2, a10);
    // c: 1, -3, -3, -7 → c=-3 appears twice → targets: -3,-1,1,3 → index0? a[0]=1 != -3, index1=-1==-1 match, index2=1==1 match, index3=-1 !=3 → 2 changes
    assert(res10.first == 2);
    assert(res10.second.size() == 2);

    std::cout << "All tests passed!\n";
    return 0;
}
#include <bits/stdc++.h>

// Returns a pair: first = minimal number of adjustments, second = vector of operation strings.
// Each operation string is either "- idx val" (decrease) or "+ idx val" (increase), idx is 1-based.
std::pair<int, std::vector<std::string>> minimizeAdjustments(int n, long long m, const std::vector<long long>& a) {
    // Count frequencies of c = a[i] - i*m
    std::unordered_map<long long, int> freq;
    long long bestC = 0;
    int maxFreq = 0;
    for (int i = 0; i < n; ++i) {
        long long c = a[i] - static_cast<long long>(i) * m;
        int newCount = ++freq[c];
        // Choose smallest c when tie: if newCount > maxFreq, update; if equal, update only if c < bestC
        if (newCount > maxFreq || (newCount == maxFreq && freq.size() == 1) || (newCount == maxFreq && c < bestC)) {
            if (newCount > maxFreq || (newCount == maxFreq && (freq.size() == 1 || c < bestC))) {
                bestC = c;
                maxFreq = newCount;
            }
        }
    }
    // However the above logic is flawed when first entry. Simpler: track best separately:
    // Reset and do a clean pass.
    bestC = 0;
    maxFreq = 0;
    bool first = true;
    for (auto& kv : freq) {
        long long c = kv.first;
        int cnt = kv.second;
        if (first || cnt > maxFreq || (cnt == maxFreq && c < bestC)) {
            bestC = c;
            maxFreq = cnt;
            first = false;
        }
    }

    int adjustments = n - maxFreq;
    std::vector<std::string> ops;
    ops.reserve(adjustments);
    for (int i = 0; i < n; ++i) {
        long long target = bestC + static_cast<long long>(i) * m;
        long long diff = a[i] - target;
        if (diff > 0) {
            ops.push_back("- " + std::to_string(i + 1) + " " + std::to_string(diff));
        } else if (diff < 0) {
            ops.push_back("+ " + std::to_string(i + 1) + " " + std::to_string(-diff));
        }
    }
    return {adjustments, ops};
}
// The key insight is that for each index `i`, the value `a[i] - i*m` gives the constant `c` that would make that position match. We want the value of `c` that appears most frequently among these computed values, because choosing that `c` maximizes the number of positions already matching the arithmetic progression. Since the problem asks for the smallest constant in case of ties, we must scan the frequency array in increasing order of `c` and pick the first one with the maximal frequency. The values of `a[i] - i*m` can be positive, zero, or negative, and `i*m` can be large (up to ~1e6 * 1e6 = 1e12), but since `a[i]` fits in 32-bit, we need to use 64-bit arithmetic for the difference. We use an offset to handle negative differences by shifting indices into an array of size large enough to cover the possible range. The range is limited: the maximum absolute value of `a[i] - i*m` is at most `|a[i]| + |i*m|`, which for `n ≤ 1e6` and `m ≤ 1e6` could be up to ~1e12, so we cannot allocate an array that large. Instead, we use a hash map (unordered_map) to count frequencies of these differences, then find the key with the maximum frequency and smallest value. After choosing `c`, we iterate over indices and compute `b[i] = c + i*m` using 64-bit arithmetic, compare with `a[i]`, and generate the operation strings. The total number of operations is exactly `n - maxFreq`. Time complexity is O(n) for counting and O(n) for generating operations. Space complexity is O(n) for the hash map and the output vector, plus O(1) extra for counting beyond the map.
