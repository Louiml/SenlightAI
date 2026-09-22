// Given three integers `n` (number of boxes arranged in a line, indexed 1 to n), `k` (number of operations that will be performed, but note that initially every box contains `k`), and a list of `k` operations, where each operation is a character `'L'` or `'R'` followed by a positive integer `x`, write a C++ function `long long countDistributions(int n, int k, const std::vector<std::pair<char,int>>& operations)` that returns the number of possible final distributions of candies modulo `998244353`, computed as follows. Initially, each of the n boxes contains exactly `k` candies. For each operation: if the operation is `'L' x`, then for every box `i` with `1 ≤ i < x`, decrease the candy count in box `i` by 1; if the operation is `'R' x`, then for every box `i` with `x < i ≤ n`, decrease the candy count in box `i` by 1. After all operations, for each operation in the original order, the box specified by `x` in that operation (that is, the `x` value) must have exactly 1 candy remaining (enforced by setting that box to 1). The final answer is the product over all boxes `i = 1..n` of the candy count in box `i`, taken modulo `998244353`. You may assume that the input is valid, so after the operations and resets, every box has a positive integer candy count. The function should handle edge cases such as operations with `x = 1` for `'L'` (no boxes affected) and `x = n` for `'R'` (no boxes affected), and should not rely on global variables. The input operations are given in the order they are performed; the vector is not necessarily sorted.

// The key to solving this efficiently is to avoid simulating each operation naively, which could be `O(n*k)` if done directly. Instead, note that each operation `'L' x` subtracts 1 from all boxes with index `< x`, and each `'R' x` subtracts 1 from all boxes with index `> x`. We can maintain a difference array (or sweep line) to record the total number of decrements applied to each box due to all operations. Specifically, create an array `diff` of size `n+2` (to handle prefix/suffix updates). For each `'L' x`, we decrement boxes 1 to x-1; this is equivalent to doing `diff[1] -= 1` and `diff[x] += 1` (since we are decreasing counts, but we will accumulate net decrements). Similarly, for `'R' x`, we decrement boxes x+1 to n; that is `diff[x+1] -= 1` and `diff[n+1] += 1` (if we allow index n+1 as sentinel). After processing all operations, compute the net decrement per box by taking prefix sums of `diff` over indices 1 to n. Then, the candy count for each box before resets is `k + net_decrement` (since net_decrement is negative or zero). Then, for each operation in the vector (in the original order), we must set the box at position `x` (the second element of the pair) to exactly 1 candy. Since these assignments override the computed count, we simply record these forced positions in a set or a boolean array. Finally, for each box `i`, if it is forced to 1, use `1`; otherwise use `max(1, k + net_decrement[i])` (though the problem guarantees positivity). Multiply all these values modulo `998244353`. Time complexity is `O(n + k)`, and space complexity is `O(n)`. Edge cases: when `x=1` for `'L'`, the range 1 to 0 is empty, so no update needed (we can conditionally apply). When `x=n` for `'R'`, the range n+1 to n is empty, similarly. Also, the modulus is prime but we only need multiplication, so just take mod after each multiplication.

#include <bits/stdc++.h>

// Count distributions modulo 998244353 given n boxes, initial k, and operations.
long long countDistributions(int n, int k, const std::vector<std::pair<char,int>>& operations) {
    const long long MOD = 998244353LL;
    std::vector<int> diff(n + 2, 0);  // diff[1..n+1] for prefix sums

    // Apply each operation's decrement to the difference array.
    for (const auto& op : operations) {
        char ch = op.first;
        int x = op.second;
        if (ch == 'L') {
            // Decrement boxes 1 to x-1
            if (x > 1) {
                diff[1] -= 1;      // start decrement at box 1
                diff[x] += 1;      // stop before box x
            }
        } else { // 'R'
            // Decrement boxes x+1 to n
            if (x < n) {
                diff[x + 1] -= 1;  // start decrement at box x+1
                diff[n + 1] += 1;  // stop after box n (sentinel)
            }
        }
    }

    // Compute net decrement for each box and the candy count before resets.
    std::vector<int> candy(n + 1, 0);
    int current_decrement = 0;
    for (int i = 1; i <= n; ++i) {
        current_decrement += diff[i];
        // current_decrement is negative or zero; candy count = k + current_decrement
        candy[i] = k + current_decrement;
    }

    // Mark boxes that are forced to 1 by the operations (original order).
    std::vector<bool> forced(n + 1, false);
    for (const auto& op : operations) {
        int x = op.second;
        forced[x] = true;
    }

    // Compute the product modulo MOD.
    long long ans = 1;
    for (int i = 1; i <= n; ++i) {
        long long val = forced[i] ? 1LL : static_cast<long long>(candy[i]);
        ans = (ans * val) % MOD;
    }
    return ans;
}

#include <cassert>
#include <vector>
#include <utility>

// Assume countDistributions is declared above.

int main() {
    // Example 1: n=3, k=2, operations: L 3, R 1
    // Initial: [2,2,2]
    // L3: decrement boxes 1,2 -> [1,1,2]
    // R1: decrement boxes 2,3 -> [1,0,1] (but we'll recompute and then force boxes 3 and 1 to 1)
    // After operations: net decrement for box1: -1, box2: -2, box3: -1 -> [1,0,1] then forced: box3=1, box1=1 -> [1,0,1] but box2 is 0 invalid? actually k=2 and net=-2 gives 0, but problem guarantees positive; here we assume input valid, but for test we'll choose valid case.
    // Let's build a valid test: n=3, k=3, ops: L2, R2
    // Initial [3,3,3]
    // L2: decrement box1 -> [2,3,3]
    // R2: decrement box3 -> [2,3,2]
    // Forced: box2=1 (from L2) and box2=1 (from R2), so box2=1 -> final [2,1,2] product=4
    assert(countDistributions(3, 3, {{'L',2},{'R',2}}) == 4);

    // Example 2: n=1, k=5, ops: L1 (does nothing)
    // Initial [5], no decrements (L1 no boxes), forced box1=1 -> product=1
    assert(countDistributions(1, 5, {{'L',1}}) == 1);

    // Example 3: n=4, k=2, ops: R4 (does nothing), L4
    // Initial [2,2,2,2]
    // R4: no boxes affected
    // L4: decrement boxes 1,2,3 -> [1,1,1,2]
    // Forced: box4=1, box4=1 (both ops have x=4), so box4=1 -> final [1,1,1,1] product=1
    assert(countDistributions(4, 2, {{'R',4},{'L',4}}) == 1);

    // Example 4: n=2, k=1, ops: (none) -> product = 1*1=1
    assert(countDistributions(2, 1, {}) == 1);

    // Example 5: n=3, k=2, ops: L3, R1
    // Initial [2,2,2]
    // L3: decrement 1,2 -> [1,1,2]
    // R1: decrement 2,3 -> [1,0,1] but result must be positive; let's adjust to k=3 to be valid:
    // Here we use n=3,k=3, ops L3,R1:
    // Initial [3,3,3] -> L3: [2,2,3] -> R1: [2,1,2] -> forced box3=1, box1=1 -> [1,1,1] product=1
    assert(countDistributions(3, 3, {{'L',3},{'R',1}}) == 1);

    // Example 6: n=5, k=10, ops: L5, R1, L5 (two L's)
    // Initial [10,10,10,10,10]
    // L5: decrement 1-4 -> [9,9,9,9,10]
    // R1: decrement 2-5 -> [9,8,8,8,9]
    // L5: decrement 1-4 again -> [8,7,7,7,9]
    // forced: box5=1, box1=1, box5=1 => box5=1, box1=1 -> final [1,7,7,7,1] product=1*7*7*7*1=343
    assert(countDistributions(5, 10, {{'L',5},{'R',1},{'L',5}}) == 343);

    // Example 7: n=4, k=0 (allowed? we assume positive, but test with k=0 and ops that yield positive)
    // n=4,k=2, ops L2, R1, L3
    // Initial [2,2,2,2]
    // L2: decrement box1 -> [1,2,2,2]
    // R1: decrement boxes 2-4 -> [1,1,1,1]
    // L3: decrement boxes 1-2 -> [0,0,1,1] invalid; but problem says valid, so skip this. Use k=3:
    // n=4,k=3, ops L2,R1,L3 -> initial [3,3,3,3]
    // L2: [2,3,3,3]
    // R1: [2,2,2,2]
    // L3: [1,1,2,2] -> forced boxes 2,1,3=1 -> final [1,1,1,2] product=2
    assert(countDistributions(4, 3, {{'L',2},{'R',1},{'L',3}}) == 2);

    // Example 8: n=3, k=5, ops L1, R3 (both no-op) -> final [5,5,5] forced boxes 1 and 3 to 1 -> [1,5,1] product=5
    assert(countDistributions(3, 5, {{'L',1},{'R',3}}) == 5);

    // Example 9: n=2, k=4, ops R2, L2
    // Initial [4,4]
    // R2: no decrement (since x=2=n)
    // L2: decrement box1 -> [3,4]
    // forced: box2=1, box2=1 -> final [3,1] product=3
    assert(countDistributions(2, 4, {{'R',2},{'L',2}}) == 3);

    // Example 10: n=1, k=1, ops L1 (no effect) -> forced box1=1 -> product=1
    assert(countDistributions(1, 1, {{'L',1}}) == 1);

    return 0;
}
