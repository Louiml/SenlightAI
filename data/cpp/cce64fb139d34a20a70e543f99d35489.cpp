/*
Write a C++ function `int countValidTriangles(const std::vector<int>& sides)` that, given a vector of positive integers representing side lengths, returns the number of triples `(i, j, k)` with `0 <= i < j < k < n` such that the three sides can form a non-degenerate triangle (i.e., the sum of any two sides is strictly greater than the third side). The input may contain up to 10^5 elements, and values may be unsorted and contain duplicates. The function must be efficient enough to handle large inputs and must not modify the original vector.
*/
#include <vector>
#include <algorithm>

// Count the number of triples (i,j,k) that can form a non-degenerate triangle.
// The function does not modify the input vector.
int countValidTriangles(const std::vector<int>& sides) {
    const size_t n = sides.size();
    if (n < 3) {
        return 0;
    }
    
    // Work on a copy to keep the input unmodified.
    std::vector<int> arr = sides;
    std::sort(arr.begin(), arr.end());
    
    long long count = 0;
    for (int high = static_cast<int>(n) - 1; high >= 2; --high) {
        int low = 0;
        int i = high - 1;
        while (low < i) {
            // Use long long to avoid overflow.
            if (static_cast<long long>(arr[low]) + arr[i] > arr[high]) {
                count += static_cast<long long>(i - low);
                --i;
            } else {
                ++low;
            }
        }
    }
    
    // The result fits in int for n <= 10^5.
    return static_cast<int>(count);
}
#include <cassert>
#include <vector>

int main() {
    // Basic example: sides [2, 2, 3, 4] -> valid triples: (2,2,3), (2,3,4) twice? Let's check: sorted [2,2,3,4] 
    // high=3 (4): low=0,i=2: 2+3>4 yes => add 2-0=2 (pairs: (2,3,4) and (2,3,4) with two 2's) then i=1: 2+2>4 no => low=1, i=1 stop. count=2
    // high=2 (3): low=0,i=1: 2+2>3 yes => add 1-0=1 (pair (2,2,3)) count=3.
    std::vector<int> v1 = {2, 2, 3, 4};
    assert(countValidTriangles(v1) == 3);
    
    // No triples possible.
    std::vector<int> v2 = {1, 2, 3};
    assert(countValidTriangles(v2) == 0);
    
    // Equilateral triangle.
    std::vector<int> v3 = {5, 5, 5, 5};
    // All combinations of 3 from 4 = 4 triples, each valid.
    assert(countValidTriangles(v3) == 4);
    
    // Unsorted and duplicates.
    std::vector<int> v4 = {4, 6, 3, 7};
    // Sorted: [3,4,6,7]
    // high=3(7): low=0,i=2: 3+6>7 yes => add 2 => (3,6,7),(4,6,7); i=1: 3+4>7 no => low=1, i=1 stop
    // high=2(6): low=0,i=1: 3+4>6 yes => add 1 => (3,4,6) total = 3
    assert(countValidTriangles(v4) == 3);
    
    // Large values to ensure no overflow.
    std::vector<int> v5 = {1000000000, 1000000000, 1000000000};
    assert(countValidTriangles(v5) == 1);
    
    // Edge case: fewer than 3 elements.
    std::vector<int> v6 = {1, 2};
    assert(countValidTriangles(v6) == 0);
    
    // All same small values.
    std::vector<int> v7 = {1, 1, 1, 1, 1};
    // C(5,3) = 10
    assert(countValidTriangles(v7) == 10);
    
    return 0;
}
// The standard approach is to sort the array first because after sorting, the triangle inequality reduces to checking only that the sum of the two smaller sides is greater than the largest side (since the other two inequalities are automatically satisfied). After sorting in ascending order, fix the largest side as the element at index `high` (from `n-1` down to `2`). For each fixed `high`, use a two-pointer technique: set `low = 0` and `i = high - 1`. While `low < i`, if `arr[low] + arr[i] > arr[high]`, then all pairs from `low` to `i-1` with `i` also satisfy the condition (because increasing `low` only increases the sum), so we add `(i - low)` to the count and decrement `i`. Otherwise, increment `low`. This counts all valid pairs for the fixed `high` in linear time. The total time complexity is `O(n log n)` due to sorting, and the two-pointer scanning is `O(n^2)` in the worst case overall, but that is the optimal for this problem (since there are `O(n^2)` possible pairs). Space complexity is `O(1)` extra if we sort a copy; if we sort the original, it would be in-place but the function should not modify the input, so we copy the vector, leading to `O(n)` auxiliary space. Edge cases: fewer than 3 elements → return 0; duplicate lengths are fine; very large sums might overflow `int` – but since side lengths are positive integers and `n` up to 10^5, the maximum sum of two sides could be up to `2 * 10^5` if values are bounded, but if values can be up to `10^9`, then sum could overflow `int`, so use `long long` or `long` for the sum check. The solution uses `long long` for safety.
