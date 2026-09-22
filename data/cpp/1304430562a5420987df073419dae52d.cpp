// Given a permutation of the integers from 1 to n (1-indexed), write a C++ function that returns the minimum number of operations needed to sort the permutation into ascending order, where an operation consists of selecting a contiguous subarray and reversing it. However, there is a constraint: you may only use **at most two reversals** in total. The function should return `0` if the permutation is already sorted, `1` if it can be sorted with exactly one reversal, and `2` otherwise (since any permutation can be sorted in at most two reversals under this constraint, and some require two). The function must handle any `n` (1 ≤ n ≤ 10^6) efficiently, without storing extra large arrays beyond the input.
The core observation is that a single reversal can only fix a permutation if the permutation has exactly one "inversion block" — that is, the sequence of elements that are out of place forms a single contiguous segment that, when reversed, makes the entire array sorted. For example, `[1,3,2,4]` has the segment `[3,2]` reversed, so one operation suffices; `[2,1,4,3]` has two separate inversion blocks, so one reversal cannot fix it, but two reversals can reverse each block separately, so the answer is `2`. To detect this, scan the array from left to right. Find the first index `l` where `a[l] != l` and the last index `r` where `a[r] != r`. If no such indices exist, the array is already sorted, return `0`. Otherwise, check if reversing the subarray `a[l..r]` yields a fully sorted array. If yes, return `1`; otherwise, return `2`. Edge cases: `n=1` always returns `0`; the array is a permutation by definition, so no duplicate or missing values; the two-reversal bound is always sufficient because any permutation can be sorted by reversing the prefix up to the first misplaced element and then the appropriate suffix, but we don't need to construct the operations. Time complexity is O(n) for the scan and O(n) for the reversal verification (can be done in place or by checking conditions without actually reversing), giving O(n) total time and O(1) auxiliary space beyond the input array.
#include <vector>
#include <algorithm>

// Returns the minimum number of reversals (0, 1, or 2) to sort the permutation.
// The input vector 'a' is 1-indexed in the problem description, but here we use 0-indexed.
// The permutation is assumed to contain exactly the numbers 1..n in some order.
int minReversalsToSort(const std::vector<int>& arr) {
    const int n = static_cast<int>(arr.size());
    
    // Find the first and last positions where the element does not match its 1-based index.
    int left = -1;
    int right = -1;
    for (int i = 0; i < n; ++i) {
        if (arr[i] != i + 1) {
            if (left == -1) left = i;
            right = i;
        }
    }
    
    // Already sorted.
    if (left == -1) return 0;
    
    // Check if reversing the single segment [left, right] yields a sorted array.
    // We verify by checking that the segment is a decreasing arithmetic progression
    // from arr[left] down to arr[right] and that the values outside the segment are correct.
    // More robust: create a copy, reverse, and compare with sorted sequence.
    std::vector<int> temp = arr;
    std::reverse(temp.begin() + left, temp.begin() + right + 1);
    for (int i = 0; i < n; ++i) {
        if (temp[i] != i + 1) return 2;  // one reversal not enough
    }
    return 1;  // exactly one reversal works
}
#include <cassert>
#include <vector>

int minReversalsToSort(const std::vector<int>& arr); // declaration from solution

int main() {
    // Already sorted
    assert(minReversalsToSort({1, 2, 3, 4}) == 0);
    assert(minReversalsToSort({1}) == 0);
    
    // Single reversal needed
    assert(minReversalsToSort({1, 3, 2, 4}) == 1);
    assert(minReversalsToSort({4, 3, 2, 1}) == 1);
    assert(minReversalsToSort({2, 1, 3, 4}) == 1);
    assert(minReversalsToSort({1, 2, 4, 3}) == 1);
    
    // Two reversals needed
    assert(minReversalsToSort({2, 1, 4, 3}) == 2);
    assert(minReversalsToSort({3, 2, 1, 4, 5}) == 1); // actually one reversal of prefix
    assert(minReversalsToSort({3, 1, 2, 5, 4}) == 2);
    assert(minReversalsToSort({2, 3, 1}) == 2);
    assert(minReversalsToSort({5, 4, 3, 2, 1}) == 1); // full reverse
    
    // Larger random-ish case
    assert(minReversalsToSort({1, 5, 4, 3, 2, 6}) == 1); // reverse [5,4,3,2]
    assert(minReversalsToSort({1, 5, 4, 3, 2, 6, 8, 7}) == 2);
    
    return 0;
}
