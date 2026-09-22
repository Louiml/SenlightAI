/*
Given a sequence of `n` positive integers (where `1 ≤ n ≤ 10^5`), write a C++ function `long long minimumAdjustmentMoves(const std::vector<int>& numbers)` that returns the minimum total number of moves required to transform the sequence into a permutation of `1,2,...,n`. In one move, you can increase or decrease a single element by 1. The function should work directly on the given vector without modifying it. The result may exceed 32-bit integer range, so return it as `long long`.
*/

#include <vector>
#include <algorithm>
#include <cstdlib>  // for std::abs

// Return the minimum total moves to turn a multiset of n positive integers
// into a permutation of 1..n, where each move changes an element by ±1.
long long minimumAdjustmentMoves(const std::vector<int>& numbers) {
    // Work on a sorted copy to preserve the original input.
    std::vector<int> sortedNumbers = numbers;
    std::sort(sortedNumbers.begin(), sortedNumbers.end());

    long long totalMoves = 0;
    const std::size_t n = sortedNumbers.size();
    for (std::size_t i = 0; i < n; ++i) {
        // In a sorted permutation, element at index i should be i+1.
        totalMoves += std::abs(static_cast<long long>(i + 1) - sortedNumbers[i]);
    }
    return totalMoves;
}

#include <cassert>
#include <vector>

// Declaration of the solution function (normally provided in the solution section).
long long minimumAdjustmentMoves(const std::vector<int>& numbers);

int main() {
    // Already a permutation
    assert(minimumAdjustmentMoves({1, 2, 3, 4}) == 0);
    // Slightly off
    assert(minimumAdjustmentMoves({4, 1, 3, 2}) == 0);
    // Duplicates and gaps: [1,1,1,4] -> target [1,2,3,4]? Actually n=4, sorted [1,1,1,4] => abs(1-1)+abs(2-1)+abs(3-1)+abs(4-4)=0+1+2+0=3
    assert(minimumAdjustmentMoves({1, 1, 1, 4}) == 3);
    // Single element
    assert(minimumAdjustmentMoves({7}) == 6);
    // Larger values with negative differences
    assert(minimumAdjustmentMoves({10, 1, 2}) == 7); // sorted [1,2,10] => 0+0+7=7
    // All far away
    assert(minimumAdjustmentMoves({100, 100, 100}) == (99+98+97) ); // 294
    // Mixed duplicates and large numbers
    assert(minimumAdjustmentMoves({5, 5, 1, 5}) == 6); // sorted [1,5,5,5] => 0+3+2+1=6
    // n=5 with reverse permutation
    assert(minimumAdjustmentMoves({5, 4, 3, 2, 1}) == 0);
    // Worst case simple: n=2 [10,10] -> sorted [10,10] => 9+8=17
    assert(minimumAdjustmentMoves({10, 10}) == 17);
    return 0;
}

// The optimal strategy is to sort the input array first. For a fixed length `n`, the target permutation that minimizes the sum of absolute differences is the sorted sequence `1,2,...,n`. This is a classic result: for sorted arrays, the sum of absolute deviations is minimized when both sequences are sorted in the same order (rearrangement inequality). After sorting, for each index `i` (0-based), the ideal value is `i+1`. The total moves is the sum over all elements of `abs((i+1) - arr[i])`. Edge cases: when `n=1`, the result is `abs(1 - arr[0])`; when all elements are already a permutation, the result is 0; duplicates do not require special handling because sorting still aligns them to the nearest missing target. Time complexity is O(n log n) due to sorting, and space complexity is O(1) auxiliary (ignoring input storage) since we sort in-place copy or use extra vector if we must not modify input—here we copy to avoid side effects.
