/*
Write a C++ function `partitionIntoTwoEqualSumSets(int n)` that, given an integer `n` (1 ≤ n ≤ 10^5), determines whether the set `{1, 2, ..., n}` can be partitioned into two disjoint non-empty subsets such that the sum of elements in each subset is equal. If no such partition exists, return a `std::pair<bool, std::pair<std::vector<int>, std::vector<int>>>` where the first element is `false` and the vectors are empty. If a partition exists, return `{true, {subset1, subset2}}` where `subset1` and `subset2` are any valid partition (order within each vector and their relative order does not matter). The function must be self-contained, use only standard C++ headers, and avoid global variables. The selected partition must use each integer from 1 to n exactly once, and the sum of each subset must equal `n*(n+1)/4`. Handle the edge cases where `n < 3` or the total sum is odd by returning `false`. The algorithm should run in `O(n)` time and `O(n)` space.
*/

#include <vector>
#include <utility>

// Partition the set {1, 2, ..., n} into two subsets with equal sum.
// Returns {true, {subset1, subset2}} if possible, otherwise {false, {{}, {}}}.
std::pair<bool, std::pair<std::vector<int>, std::vector<int>>>
partitionIntoTwoEqualSumSets(int n) {
    long long total = static_cast<long long>(n) * (n + 1) / 2;
    if (n < 3 || total % 2 != 0) {
        return {false, {{}, {}}};
    }
    long long target = total / 2;
    
    std::vector<int> first;
    std::vector<int> second;
    long long sum1 = 0, sum2 = 0;
    
    // Greedy: place larger numbers first into the subset with smaller current sum.
    for (int i = n; i >= 1; --i) {
        if (sum1 <= sum2) {
            first.push_back(i);
            sum1 += i;
        } else {
            second.push_back(i);
            sum2 += i;
        }
    }
    
    // The greedy always succeeds for valid n, but assert internally for safety.
    if (sum1 != target || sum2 != target) {
        return {false, {{}, {}}};
    }
    return {true, {first, second}};
}

#include <cassert>
#include <vector>
#include <numeric>
#include <algorithm>
#include <iostream>

// Solution function declaration (provided above).
std::pair<bool, std::pair<std::vector<int>, std::vector<int>>>
partitionIntoTwoEqualSumSets(int n);

int main() {
    // Helper to verify a partition.
    auto valid = [](int n, const std::vector<int>& a, const std::vector<int>& b) {
        if ((int)a.size() + (int)b.size() != n) return false;
        std::vector<bool> used(n + 1, false);
        long long sumA = 0, sumB = 0;
        for (int x : a) { if (x < 1 || x > n || used[x]) return false; used[x] = true; sumA += x; }
        for (int x : b) { if (x < 1 || x > n || used[x]) return false; used[x] = true; sumB += x; }
        return sumA == sumB && sumA == (long long)n*(n+1)/4;
    };

    // Test n=1,2,3 (n<3 => false; n=3 total=6 even, target=3, partition {1,2} and {3})
    auto r1 = partitionIntoTwoEqualSumSets(1);
    assert(!r1.first && r1.second.first.empty() && r1.second.second.empty());
    auto r2 = partitionIntoTwoEqualSumSets(2);
    assert(!r2.first);
    auto r3 = partitionIntoTwoEqualSumSets(3);
    assert(r3.first && valid(3, r3.second.first, r3.second.second));

    // Test n=4 total=10 odd => cannot partition
    auto r4 = partitionIntoTwoEqualSumSets(4);
    assert(!r4.first);

    // Test n=7 total=28 even, target=14; normal case
    auto r7 = partitionIntoTwoEqualSumSets(7);
    assert(r7.first && valid(7, r7.second.first, r7.second.second));

    // Test n=8 total=36 even, target=18
    auto r8 = partitionIntoTwoEqualSumSets(8);
    assert(r8.first && valid(8, r8.second.first, r8.second.second));

    // Test n=11 total=66 even, target=33
    auto r11 = partitionIntoTwoEqualSumSets(11);
    assert(r11.first && valid(11, r11.second.first, r11.second.second));

    // Test a larger n where n%4 is 1 or 2 -> total odd? Actually total odd when n%4==1 or 2.
    // n=5 total=15 odd
    auto r5 = partitionIntoTwoEqualSumSets(5);
    assert(!r5.first);
    // n=9 total=45 odd
    auto r9 = partitionIntoTwoEqualSumSets(9);
    assert(!r9.first);
    // n=10 total=55 odd
    auto r10 = partitionIntoTwoEqualSumSets(10);
    assert(!r10.first);

    // Test n=100 (valid) and check sums
    auto r100 = partitionIntoTwoEqualSumSets(100);
    assert(r100.first && valid(100, r100.second.first, r100.second.second));

    std::cout << "All tests passed!\n";
    return 0;
}

// The problem asks to split numbers `1..n` into two groups with equal sums. The total sum is `S = n*(n+1)/2`. An equal split is possible only if `S` is even and `n >= 3` (for n=1 or 2 the sums cannot be equal because the total is too small). Let `target = S/2`. The constructive approach used in the reference code processes numbers from largest to smallest. It maintains a "turn" flag and a counter `rem`. Initially `turn = true`, `rem = 1`. For each `i` from `n` down to `1`, it places `i` into subset1 if `turn` is true, else subset2. After placing, it decrements `rem`. When `rem` becomes 0, it flips `turn` and resets `rem = 2`. This pattern places numbers in blocks of sizes 1, 2, 2, 2, ... alternating between the two subsets. This works because for any `n` satisfying the condition, this greedy allocation yields exactly the target sum for each subset. A simpler and more standard approach is to iterate from `n` down to `1`, and for each number, add it to the subset with the smaller current sum (or to subset1 if its sum is smaller, else to subset2). Since we go from largest to smallest, this greedy always achieves the target when a partition exists. After building both subsets, we can verify their sums equal `target`. The time complexity is `O(n)` because we iterate once. Space complexity is `O(n)` to store the two vectors.
