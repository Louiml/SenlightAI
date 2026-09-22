// Write a C++ function `bool canArrangePairs(int n, int x, const std::vector<int>& a, const std::vector<int>& b)` that determines whether it is possible to pair each element from array `a` with exactly one element from array `b` (using every element exactly once) such that for every pair, the sum of the two elements is less than or equal to `x`. If such a pairing exists, return `true`; otherwise, return `false`. The arrays are 1-indexed in the original problem but for the function, use 0-indexed vectors of size `n`. The function must handle arrays with duplicate values and any order of input. Assume `1 <= n <= 50` and `1 <= x <= 100` and all array elements are positive integers (1 to 100).

#include <cassert>
#include <vector>

// Declaration of the function under test (assumed provided separately).
bool canArrangePairs(int n, int x, const std::vector<int>& a, const std::vector<int>& b);

int main() {
    // Test 1: Example from original snippet - n=3, x=10, a={2,3,4}, b={8,7,6} -> sums: 2+8=10, 3+7=10, 4+6=10 -> Yes
    assert(canArrangePairs(3, 10, {2, 3, 4}, {8, 7, 6}) == true);
    
    // Test 2: Same arrays but x=9 -> max sum 10 > 9 -> No
    assert(canArrangePairs(3, 9, {2, 3, 4}, {8, 7, 6}) == false);
    
    // Test 3: Single element pair that fits
    assert(canArrangePairs(1, 5, {2}, {3}) == true);
    
    // Test 4: Single element pair that does not fit
    assert(canArrangePairs(1, 5, {2}, {4}) == false);
    
    // Test 5: Duplicates, n=4, x=8, a={1,1,1,1}, b={7,7,7,7} -> each sum 8 -> Yes
    assert(canArrangePairs(4, 8, {1,1,1,1}, {7,7,7,7}) == true);
    
    // Test 6: Duplicates, same but x=7 -> sums 8 > 7 -> No
    assert(canArrangePairs(4, 7, {1,1,1,1}, {7,7,7,7}) == false);
    
    // Test 7: Already paired trivial case, n=2, x=10, a={5,5}, b={5,5} -> sums 10 -> Yes
    assert(canArrangePairs(2, 10, {5,5}, {5,5}) == true);
    
    // Test 8: Unsorted input, n=5, x=15, a={10,1,5,2,8}, b={4,14,6,9,1} -> sorted a:1,2,5,8,10; sorted b:14,9,6,4,1 -> sums:15,11,11,12,11 all <=15 -> Yes
    assert(canArrangePairs(5, 15, {10,1,5,2,8}, {4,14,6,9,1}) == true);
    
    // Test 9: Same but x=14 -> first sum 15>14 -> No
    assert(canArrangePairs(5, 14, {10,1,5,2,8}, {4,14,6,9,1}) == false);
    
    return 0;
}

#include <vector>
#include <algorithm>

// Determine if arrays a and b can be paired so every pair sum <= x.
// Sorts a ascending, b descending, then checks each paired sum.
bool canArrangePairs(int n, int x, const std::vector<int>& a, const std::vector<int>& b) {
    // Copy inputs so we can sort without modifying caller's data.
    std::vector<int> aSorted = a;
    std::vector<int> bSorted = b;
    
    std::sort(aSorted.begin(), aSorted.end());
    std::sort(bSorted.begin(), bSorted.end(), std::greater<int>());
    
    for (int i = 0; i < n; ++i) {
        if (aSorted[i] + bSorted[i] > x) {
            return false;
        }
    }
    return true;
}

// The optimal pairing strategy is to sort one array in ascending order and the other in descending order, then pair the i-th smallest of `a` with the i-th largest of `b`. This is a classic greedy argument: to minimize the maximum sum among pairs, you match the smallest from one list with the largest from the other. If even this best-case pairing produces any pair whose sum exceeds `x`, then no other pairing can work, because any other pairing would only increase some sums. Sort `a` ascending using `std::sort`, sort `b` descending using `std::sort` with `std::greater<int>()`. Then for each index `i` from 0 to n-1, check if `a[i] + b[i] > x`. If any pair exceeds `x`, return `false`; otherwise return `true`. Edge cases: when `n = 1`, the single pair must be checked; duplicate values are handled naturally by sorting. Time complexity is O(n log n) due to sorting, space complexity is O(1) auxiliary (excluding input vectors).
