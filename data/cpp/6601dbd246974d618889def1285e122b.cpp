Write a C++ function named `minimumDeletionsForXor` that takes a non-empty `std::vector<int>` of 32-bit integers and returns the minimum number of elements that must be removed so that the remaining elements have a bitwise XOR value equal to zero. If the entire vector already has XOR zero, return 0. The vector may contain duplicates, negative numbers, and all values within the 32-bit signed integer range. The function must use a recursive divide-and-conquer approach based on the most significant bit, and must not use any global or static state. The solution must handle up to \(10^5\) elements efficiently.

The core observation: If the XOR of all elements is already zero, the answer is 0, because we need to delete nothing. Otherwise, consider the highest bit position (from bit 30 down to 0). Partition the numbers into those with that bit set (group `l`) and those with that bit cleared (group `r`). For the XOR to become zero after deletions, we can either delete all elements from one group entirely (then the other group must itself be made XOR-zero) or keep elements from both groups and delete enough to make the overall XOR zero. However, a key insight from the given snippet is that the optimal strategy is: if one group is empty, recurse only on the other; if both are non-empty, then the best is `1 + max(solve(l), solve(r))`, meaning we delete at least one element from the larger (in terms of needed deletions) side and then recurse. More formally, let `f(v, p)` be the maximum number of elements we can keep such that their XOR is zero, considering only bits `p` down to 0. If `v.size() <= 1`, we can keep at most 0 if `v.size()==1` because a single non-zero element cannot have XOR zero, and 0 if empty. If we split by bit `p`, then if one side is empty, we must take from the non-empty side and recurse. If both are non-empty, to have XOR zero, we must either delete all of one side and solve the other, or keep some from both; but the recursion structure shows the answer is `1 + max(solve(l), solve(r))` – meaning we are forced to delete at least one element from the side that yields the larger kept count, and then solve that side. This is because if both sides are non-empty, the bit `p` must appear an even number of times in the kept set, but since it appears in every element of `l` and no element of `r`, we either delete all of `l` or all of `r` (or delete an odd number from each, but that reduces further). The optimal is to keep the side with more allowed elements and delete at least one from the other side, then recursively solve the larger side. After computing `solve(v, 30)`, the number of deletions is `n - solve`, because `solve` returns the maximum number of elements we can keep with XOR zero. Edge cases: empty vector (not allowed per task, but handle gracefully), single element (must delete it because a single non-zero element cannot have XOR zero), duplicate zeros (keep them all), negative numbers (bitwise operations work fine because of two's complement). Time complexity: Each recursive call processes each element exactly once per level, and the depth is at most 31, so \(O(31 \cdot n) = O(n)\). Space complexity: Each level creates new vectors, but total memory at any point is \(O(n)\) because vectors are split and not copied beyond the current recursion stack, but careful: the recursion creates new vectors at each call, and they are not freed until the call returns; worst-case the total size of all vectors across the recursion tree is \(O(n \log n)\) if we consider all levels, but at any single moment the active stack holds at most \(O(n)\) elements because each level holds partitions of the original. More precisely, the peak memory is \(O(n)\) because the sum of sizes of vectors on the call stack is at most \(n\) (each element appears in exactly one vector on the stack). So space is \(O(n)\) auxiliary.

#include <vector>
#include <algorithm>

// Recursively compute the maximum number of elements we can keep such that their XOR is zero,
// considering only bits from 'bit' down to 0.
static int maxKeepXor(const std::vector<int>& v, int bit) {
    if (v.size() <= 1) {
        // With 0 elements, keep 0; with 1 element, the XOR is that element (non-zero unless it's 0),
        // but 0 alone has XOR 0, so we could keep 0? Actually a single 0 has XOR 0, but our base case
        // returns v.size() if all elements are zero? We need to check: if v.size()==1 and v[0]==0, we keep 1.
        // But we also need to handle the case where v contains a single 0. However, the recursion splits
        // by bits, and a single 0 will always go to the 'r' (bit=0) side. So we need to adjust the base case.
        // The standard approach: if v contains only one element, that element's XOR is itself. If it's zero, we can keep it, else not.
        // But the given snippet's base case returns v.size() for size<=1, which would incorrectly allow keeping a single non-zero.
        // To be correct, we need to check if the sole element is zero. Let's handle that properly.
        for (int x : v) {
            if (x != 0) return 0;
        }
        return static_cast<int>(v.size());
    }
    if (bit < 0) {
        // No bits left, all elements must be zero (since otherwise they'd differ at some bit).
        // If all elements are zero, we can keep all of them. If any non-zero, they'd differ, but this case shouldn't occur
        // because we split by bits, so when bit<0 all elements must be identical (zero). So return v.size().
        return static_cast<int>(v.size());
    }
    std::vector<int> l, r;
    for (int x : v) {
        if (x & (1 << bit)) {
            l.push_back(x);
        } else {
            r.push_back(x);
        }
    }
    if (l.empty()) return maxKeepXor(r, bit - 1);
    if (r.empty()) return maxKeepXor(l, bit - 1);
    // Both non-empty: we must delete at least one element from the side that gives the larger kept count,
    // then recurse on that side. The kept count is 1 + max(solve(l), solve(r)).
    return 1 + std::max(maxKeepXor(l, bit - 1), maxKeepXor(r, bit - 1));
}

// Public function: returns minimum deletions so that remaining elements' XOR is zero.
int minimumDeletionsForXor(const std::vector<int>& nums) {
    if (nums.empty()) return 0;
    int maxKeep = maxKeepXor(nums, 30);
    return static_cast<int>(nums.size()) - maxKeep;
}

#include <cassert>
#include <vector>
#include <iostream>

// Declaration from solution (assume included)
int minimumDeletionsForXor(const std::vector<int>& nums);

int main() {
    // Case 1: Already XOR zero -> delete 0
    assert(minimumDeletionsForXor({0, 0, 0}) == 0);
    assert(minimumDeletionsForXor({1, 2, 3}) == 0); // 1^2^3=0

    // Case 2: Single zero -> keep it, delete 0
    assert(minimumDeletionsForXor({0}) == 0);

    // Case 3: Single non-zero -> must delete it
    assert(minimumDeletionsForXor({5}) == 1);

    // Case 4: Two same numbers -> XOR zero, delete 0
    assert(minimumDeletionsForXor({7, 7}) == 0);

    // Case 5: Two different numbers -> need to delete one to get XOR zero (by removing one, the other remains non-zero, not zero; actually removing one leaves a single non-zero, not zero, so need to delete both? Let's think: {1,2} -> options: delete both -> empty set XOR 0 (2 deletions); delete one -> {1} XOR=1, {2} XOR=2, not zero. So minimum deletions=2. But our function? Let's test.
    assert(minimumDeletionsForXor({1, 2}) == 2);

    // Case 6: {1,1,2} -> XOR of all is 2, not zero. We can delete 2 and keep {1,1} XOR=0, so 1 deletion.
    assert(minimumDeletionsForXor({1, 1, 2}) == 1);

    // Case 7: {3,5,6} -> 3^5^6=0, so 0 deletions.
    assert(minimumDeletionsForXor({3, 5, 6}) == 0);

    // Case 8: Large mixed with zeros and negatives: {-1, -1, 5} -> XOR of all: -1^-1=0, 0^5=5, not zero. Delete 5 -> keep {-1,-1} XOR=0, 1 deletion.
    assert(minimumDeletionsForXor({-1, -1, 5}) == 1);

    // Case 9: {0,1,1} -> XOR=0, so 0 deletions.
    assert(minimumDeletionsForXor({0, 1, 1}) == 0);

    std::cout << "All tests passed!" << std::endl;
    return 0;
}
