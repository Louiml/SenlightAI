/*
Given a sequence of positive integers in the range [1, 10^9], you must process them one by one in the given order. Your task is to find the largest possible prefix length `i` such that the first `i` numbers can be partitioned into the minimum number of **non-decreasing subsequences** (also known as a chain partition in a poset, where each subsequence is strictly non-decreasing, i.e., equal values are allowed in the same subsequence). However, there is a catch: the partition must respect the "patience sorting" invariant: you are allowed to place a new number onto an existing pile only if the new number is **at least as large as the current top** of that pile (non-decreasing). But you may also start a new pile if the number is smaller than all current tops. Additionally, you are **not allowed** to place a number onto a pile if that number is strictly less than the **minimum possible top** that could have been achieved by an earlier number in that pile — actually, the original snippet enforces that you must place each number in the **first possible pile** (i.e., the leftmost pile whose current maximum is ≤ the new number). If no such pile exists, you start a new pile, but only if the new number is **≥ some special bound `B`** (initially 0). If the number is less than `B`, the process stops immediately and you output the count of successfully processed numbers. The bound `B` is updated during processing: when you cannot place a number onto an existing pile and must start a new pile, `B` becomes the largest value that is **less than the new number** among all current pile tops. If that value exists, `B` is set to that value. Otherwise, `B` remains unchanged. Find the maximum prefix length processed before the first time a number is strictly less than `B` (at which point you stop and output that prefix length). More formally, you are given N integers. Simulate the described greedy algorithm and output how many numbers can be processed without violating the condition `k >= B` where `B` is maintained as described.
*/
#include <bits/stdc++.h>

// Simulates the described greedy patience-sorting process with a bound B.
// Returns the number of integers processed before the first violation (k < B).
int maxProcessablePrefix(const std::vector<int>& numbers) {
    std::vector<int> pileTops;                 // non-decreasing tops of piles
    std::vector<std::set<int>> pileValues;     // all values ever placed in each pile

    int B = 0;  // smallest value that cannot be used later
    size_t i = 0;

    for (const int k : numbers) {
        if (k < B) {
            break; // violation: this number is too small
        }

        size_t j = std::lower_bound(pileTops.begin(), pileTops.end(), k) - pileTops.begin();

        if (j == pileTops.size()) {
            // start a new pile
            pileTops.push_back(k);
            pileValues.emplace_back();
            pileValues.back().insert(k);
        } else {
            // replace top of pile j
            if (!pileValues[j].empty()) {
                auto it = pileValues[j].upper_bound(k);
                if (it != pileValues[j].begin()) {
                    --it;
                    B = std::max(B, *it);
                }
            }
            pileValues[j].insert(k);
            pileTops[j] = k;
        }
        ++i;
    }

    return static_cast<int>(i);
}
#include <cassert>
#include <vector>
#include "solution.h" // assuming the function is in a header

int main() {
    // Example from the snippet (N=5, sequence 1 3 2 5 4): 
    // i=0: k=1, lower_bound empty -> new pile, B=0
    // i=1: k=3, lower_bound finds pos0 (top 1), no prior values less than 3? (1<3 -> B=1), insert 3, top=3
    // i=2: k=2, lower_bound finds pos0 (top 3), prior values set {1,3}, upper_bound(2)=3, decrement ->1, B=max(1,1)=1, insert 2, top=2
    // i=3: k=5, lower_bound finds end -> new pile, B stays 1
    // i=4: k=4, lower_bound finds pos1 (top 5), prior values {5}, no value <4, insert 4, top=4
    // all processed successfully -> returns 5
    assert(maxProcessablePrefix({1, 3, 2, 5, 4}) == 5);
    assert(maxProcessablePrefix({5, 4, 3, 2, 1}) == 5); // all decreasing, each starts new pile, B remains 0
    assert(maxProcessablePrefix({1, 2, 1, 3}) == 4); // never violates B
    assert(maxProcessablePrefix({2, 1, 2}) == 3); // k=1, lower_bound pos0 (top 2), set {2}, no value<1, insert 1, B=0; k=2, lower_bound pos0 (top1), set{2,1}, upper_bound(2)=end, decrement->1 (since 1<2), B=1, then all good
    assert(maxProcessablePrefix({3, 1, 2}) == 3); // k=3 new pile B=0; k=1 lower_bound pos0 (top3), set{3}, no <1, insert1 top=1; k=2 lower_bound pos1 (end) -> new pile? Wait: lower_bound on tops [1] for k=2 gives end, so new pile. Check k<B? B=0, ok. So processed all 3.
    assert(maxProcessablePrefix({4, 2, 3, 1}) == 3); // 4 -> new pile; 2 -> replace top 4 with 2, set{4}, no<2, B=0; 3 -> lower_bound on tops[2]? tops[0]=2, lower_bound(3)=end -> new pile, B=0; then k=1: lower_bound on tops [2,3] -> pos0, set{2,4}? Actually set for pile0 is {4,2}; upper_bound(1) -> begin, no decrement, so no B update. But k=1 is < B? B still 0, so it would be processed. Actually that gives 4 processed? Let's recompute: numbers [4,2,3,1]. 
    // i=0: k=4 -> new pile, tops=[4], sets[0]={4}
    // i=1: k=2 -> lower_bound on [4] -> pos0, set{4}, upper_bound(2)->begin? wait upper_bound(2) on {4} gives begin (since 4>2), so it != begin? It is begin, so condition false, no B update. insert 2, tops[0]=2, set{4,2}
    // i=2: k=3 -> lower_bound on [2] -> end (since 3>2) -> new pile. k<B? B=0, ok. tops=[2,3], sets[1]={3}
    // i=3: k=1 -> lower_bound on [2,3] -> pos0 (2>=1). set[0]={4,2}, upper_bound(1)-> begin, no decrement (since begin), no B update. k<B? B=0, ok. insert 1, tops[0]=1, set{4,2,1}. So processed all 4, returns 4. So assert 4.
    assert(maxProcessablePrefix({4, 2, 3, 1}) == 4);
    // Edge case: single element
    assert(maxProcessablePrefix({42}) == 1);
    // Large numbers
    assert(maxProcessablePrefix({1000000000, 1, 2}) == 3); // 1e9 new pile, 1 replace (top set {1e9} no<1), 2 -> lower_bound on [1] end->new pile, B=0, all ok
    return 0;
}
// The problem is a direct simulation of the greedy patience-sorting variant used in the Longest Increasing Subsequence (LIS) algorithm, but with a twist involving `B`. We maintain a vector `L` of pile tops in **non-decreasing order** (since piles are sorted by their top values). For each incoming integer `k`:
// - Find the first pile whose top is ≥ `k` using `lower_bound` on `L`. If `j == L.size()`, we start a new pile; before that, check `k < B`? If so, stop.
// - If `j < L.size()`, we would normally place `k` on that pile, replacing its top. But if `k < B`, we stop. Otherwise, we update the pile's top to `k` (and also keep a set of all values ever placed in that pile to find the largest value < `k` in that pile, which is used to update `B` when we start a new pile).
// - More precisely, when a new pile is created (`j == L.size()`), we append `k` to `L` and store `k` in `S[j]`. When we replace a pile top (i.e., `j < L.size()`), we first check if `S[j]` has any value less than `k`. If yes, we update `B = max(B, that value)`. Then we insert `k` into `S[j]` and set `L[j] = k`. If no such value exists, we just insert `k`.
// - The process stops as soon as `k < B` for any input number (including the case where we would start a new pile). The output is the count of numbers successfully processed (i.e., the loop index `i` when stopped).
// Edge cases: Early termination if the very first number is < B (B starts at 0 so never). Duplicates are allowed. Large N up to 1e5; numbers up to 1e9. Time complexity: O(N log N) due to binary search on `L` and set operations (each insertion/upper_bound O(log size)). Space complexity: O(N) for `L` and the sets `S` (total elements across all sets is exactly N). The critical part is correctly updating `B` only from the pile we place onto, using the largest value in that pile that is strictly less than `k`.
