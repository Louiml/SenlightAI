/*
Write a C++ function `int permutationDistance(const std::vector<int>& p, const std::vector<int>& q)` that takes two permutations of the integers from 1 to n (both of size n, where n ≥ 1) and returns the absolute difference between their lexicographic positions (1-indexed) in the sorted list of all permutations of 1..n. For example, if p = {1,2,3} and q = {3,2,1}, their positions are 1 and 6, so the answer is 5. You may assume both inputs are valid permutations of the same size, and n ≤ 10 to fit within standard arithmetic ranges. Do not use `std::next_permutation` in your solution; instead, implement the position calculation directly using factorial number system (Lehmer code). The function must be `const`-correct and self-contained (no global variables).
*/

#include <vector>
#include <algorithm>
#include <cstdlib>

// Helper to compute the factorial of a non-negative integer.
long long factorial(int x) {
    long long result = 1;
    for (int i = 2; i <= x; ++i) result *= i;
    return result;
}

// Compute the 1-indexed lexicographic rank of a permutation of 1..n.
long long lexicographicRank(const std::vector<int>& perm) {
    int n = static_cast<int>(perm.size());
    std::vector<bool> used(n + 1, false);
    long long rank = 1; // 1-indexed
    for (int i = 0; i < n; ++i) {
        int count = 0;
        // Count unused numbers smaller than perm[i]
        for (int num = 1; num < perm[i]; ++num) {
            if (!used[num]) ++count;
        }
        rank += count * factorial(n - 1 - i);
        used[perm[i]] = true;
    }
    return rank;
}

// Returns the absolute difference between the lexicographic ranks of two permutations.
int permutationDistance(const std::vector<int>& p, const std::vector<int>& q) {
    long long rankP = lexicographicRank(p);
    long long rankQ = lexicographicRank(q);
    return static_cast<int>(std::llabs(rankP - rankQ));
}

#include <cassert>
#include <vector>

// The solution function is assumed to be defined above.
int main() {
    // Basic examples
    assert(permutationDistance({1,2,3}, {3,2,1}) == 5);
    assert(permutationDistance({1,2,3}, {1,2,3}) == 0);
    assert(permutationDistance({3,1,2}, {2,1,3}) == 2); // ranks 4 and 2

    // n=1 edge case
    assert(permutationDistance({1}, {1}) == 0);

    // n=4 cases
    assert(permutationDistance({1,2,3,4}, {4,3,2,1}) == 23); // ranks 1 and 24
    assert(permutationDistance({2,1,4,3}, {4,3,2,1}) == 22); // ranks 2 and 24

    // Larger n=5
    assert(permutationDistance({1,2,3,4,5}, {5,4,3,2,1}) == 119);

    // Adjacent permutations differ by 1
    assert(permutationDistance({1,2,3,4}, {1,2,4,3}) == 1);

    // Random sanity: p=[3,1,4,2] rank? Let's verify manually: 
    // For 3: count=2 => +2*6=12 => rank=13
    // For 1: count=0 => +0 => rank=13
    // For 4: count=2 (2 and 3 unused? Actually unused after 3,1: {2,4}? Wait used 3 and 1, so unused {2,4}, numbers <4 unused: 2 => count=1) => +1*1=1 => rank=14
    // Last: 2 => +0 => rank=14
    // q=[2,4,1,3] rank? For 2: count=1 => +1*6=6 => rank=7; for 4: count=2 (unused {1,3,4}, <4:1,3 =>2) => +2*2=4 => rank=11; for 1: count=0 => rank=11; last 3: +0 => rank=11. diff=3.
    assert(permutationDistance({3,1,4,2}, {2,4,1,3}) == 3);

    return 0;
}

// The naive approach of generating all permutations and indexing them is correct but wasteful, especially for n up to 10 where 10! = 3,628,800 is manageable, but for n up to 12 or more it becomes prohibitive. The efficient approach uses the factorial number system: for a given permutation, its lexicographic rank (1-indexed) can be computed by iterating from left to right and counting how many unused numbers are smaller than the current element. For each position i (0-indexed), let `count` be the number of unused numbers less than `p[i]`. Then add `count * (n-1-i)!` to the rank. Start rank at 1 (since lexicographic positions are 1-indexed). Use a data structure (e.g., a Fenwick tree or simply a boolean visited array) to track which numbers have been used. Since n ≤ 10, a simple `std::vector<bool>` and a linear scan is fine. After computing ranks for both p and q, the answer is `abs(rankP - rankQ)`. Edge cases: n=1, both permutations identical (distance 0), and permutations where the order is reversed (maximum distance). Time complexity is O(n^2) per function call for the linear scans, or O(n log n) with a Fenwick tree; space is O(n). For n ≤ 10, O(n^2) is trivial.
