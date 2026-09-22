/*
Write a standalone C++ function `long long minimumPaintingTime(const std::vector<int>& boards, int painters)` that, given a vector of positive integers representing the lengths of consecutive boards and a positive integer number of painters `painters`, returns the minimum possible time needed to paint all boards under the constraint that each painter must be assigned a contiguous subarray of boards, all painters start simultaneously, and each painter takes 1 unit of time per unit of board length. The function should handle the case where the number of painters exceeds the number of boards (in which case each board can be assigned to a separate painter). The solution must not modify the input vector and must be correct for large total lengths (up to 10^9 per board and up to 10^5 boards), using appropriate 64‑bit integer types.
*/
#include <vector>
#include <algorithm>
#include <numeric>

// Returns the minimum possible completion time when `painters` painters
// paint contiguous segments of `boards`, each taking 1 time unit per board unit.
long long minimumPaintingTime(const std::vector<int>& boards, int painters) {
    long long lo = *std::max_element(boards.begin(), boards.end());
    long long hi = std::accumulate(boards.begin(), boards.end(), 0LL);
    
    while (lo < hi) {
        long long mid = lo + (hi - lo) / 2;
        long long currentSum = 0;
        int neededPainters = 1;
        
        for (int length : boards) {
            if (currentSum + length > mid) {
                neededPainters++;
                currentSum = length;
            } else {
                currentSum += length;
            }
        }
        
        if (neededPainters <= painters) {
            hi = mid;  // feasible, try smaller
        } else {
            lo = mid + 1;  // not feasible, need larger
        }
    }
    return lo;
}
#include <cassert>
#include <vector>

long long minimumPaintingTime(const std::vector<int>& boards, int painters);

int main() {
    // Basic example: [10,20,30,40], k=2 -> partition as [10,20,30] and [40]? 
    // Better: [10,20,30] sum=60, [40] sum=40 → max=60, but [10,20] sum=30 and [30,40] sum=70 → max=70.
    // Minimum is 60? Actually optimal: [10,20,30] =60, [40]=40 → max 60. Test:
    assert(minimumPaintingTime({10,20,30,40}, 2) == 60);
    
    // Single board
    assert(minimumPaintingTime({5}, 1) == 5);
    
    // Many painters: answer is max element
    assert(minimumPaintingTime({1,2,3,4}, 4) == 4);
    assert(minimumPaintingTime({1,2,3,4}, 10) == 4);
    
    // All equal boards
    assert(minimumPaintingTime({7,7,7}, 2) == 14);  // [7,7] and [7] or [7] and [7,7]
    
    // Large values to check long long
    std::vector<int> big(100000, 1000000000);
    assert(minimumPaintingTime(big, 1000) == 100000000000LL); // each painter gets 100 boards
    
    // Suppose list 1,2,3 with 2 painters: optimal [1,2] and [3] → max=3; or [1] and [2,3] → max=5; so 3.
    assert(minimumPaintingTime({1,2,3}, 2) == 3);
    
    // Zero not allowed but ensure positive input works
    assert(minimumPaintingTime({5,10,15,20}, 3) == 20);
    
    // More painters than boards
    assert(minimumPaintingTime({4,5,6}, 5) == 6);
    
    return 0;
}
// The problem reduces to partitioning the array of board lengths into at most `painters` contiguous segments such that the maximum sum of any segment is minimized. This is a classic binary search on the answer problem. The minimum possible answer is at least the maximum single board length, and at most the total sum of all boards. For a candidate maximum time `T`, we simulate assigning boards to painters greedily from left to right: we give each painter as many consecutive boards as possible without exceeding `T`, and count how many painters are needed. If that count is ≤ `painters`, then `T` is feasible (since we can leave some painters idle); otherwise `T` is too small. We binary search over the range `[maxElement, totalSum]` to find the smallest feasible `T`. Edge cases: if `painters` ≥ number of boards, the answer is simply the maximum board length (but binary search handles it automatically). All calculations must use `long long` to avoid overflow. Time complexity: O(n log totalSum) with O(1) auxiliary space (excluding input storage).
