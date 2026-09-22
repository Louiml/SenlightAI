/*
Write a C++ function `std::pair<int,int> findTwoLargestDivisors(int n, const std::vector<int>& arr)` that accepts an integer `n` (the number of elements) and a vector of `n` positive integers, and returns a pair `{x, y}` where `x` is the maximum element of the vector, and `y` is the largest element (from the sorted array) such that either `x % y != 0` (y does not divide x) or `y` appears at least twice in the array (i.e., the element at index `i` has the same value as the element at index `i-1` when the array is sorted ascending). The search for `y` starts from the largest element and goes downward (checking indices from `n-1` down to `1`). If no such `y` exists (i.e., all elements are distinct divisors of `x`), return `{x, x}`. Ensure the function handles edge cases like `n=1` (in this case, `x` is the only element, and the condition `x % x == 0` holds but since there's no `i-1` for `i=0`, you must decide: if `n==1`, return `{x,x}`). The function should not modify the input vector.
*/
#include <vector>
#include <algorithm>
#include <utility>

// Given a vector of positive integers, returns {max_element, largest_element_that_either_does_not_divide_max_or_is_duplicate}.
// If no such second element exists, returns {max, max}.
std::pair<int,int> findTwoLargestDivisors(int n, const std::vector<int>& arr) {
    // Copy to avoid modifying input, sort ascending.
    std::vector<int> sorted = arr;
    std::sort(sorted.begin(), sorted.end());

    // Maximum element is the last one.
    int x = sorted[n - 1];

    // Special case: only one element.
    if (n == 1) {
        return {x, x};
    }

    // Search from the largest index down to index 1.
    for (int i = n - 2; i >= 1; --i) {
        // Condition: either arr[i] does not divide x, or arr[i] duplicates the previous element.
        if (x % sorted[i] != 0 || sorted[i] == sorted[i - 1]) {
            return {x, sorted[i]};
        }
    }

    // Also check index 0 (if we haven't returned yet). For i=0, there is no i-1, so only divisibility matters.
    if (x % sorted[0] != 0) {
        return {x, sorted[0]};
    }

    // All elements are distinct divisors of x.
    return {x, x};
}
#include <cassert>
#include <vector>
#include <utility>

// Declaration of the function under test (already provided above).
std::pair<int,int> findTwoLargestDivisors(int n, const std::vector<int>& arr);

int main() {
    // Basic case: max=10, elements [2,5,10] -> 10%2==0, 10%5==0, but 10%10==0; all divide, so return {10,10}
    assert(findTwoLargestDivisors(3, {2, 5, 10}) == std::make_pair(10, 10));

    // Case with non-divisor: max=10, elements [3,5,10] -> 10%3!=0, so y=3 (largest non-divisor from top? Actually scan from index 1: i=1 (value 5) divides 10, no duplicate, continue; i=0 (value 3) does not divide, returns {10,3})
    assert(findTwoLargestDivisors(3, {3, 5, 10}) == std::make_pair(10, 3));

    // Duplicate of max: [4,4] -> x=4, i=0, arr[0]==4, no i-1, but x%4==0, so no return from loop; then check index 0: x%4==0, so returns {4,4} (since all divide? Actually duplicate? The condition in loop checks i>=1, so i=0 is not considered, but we have a final check for index 0. Since 4%4==0, no return, final return {4,4})
    assert(findTwoLargestDivisors(2, {4, 4}) == std::make_pair(4, 4));

    // Duplicate non-max: [2,2,4] -> x=4, i=1: value 2, x%2==0, but sorted[1]==sorted[0] (2==2), so condition true, returns {4,2}
    assert(findTwoLargestDivisors(3, {2, 2, 4}) == std::make_pair(4, 2));

    // Single element: {7} -> returns {7,7}
    assert(findTwoLargestDivisors(1, {7}) == std::make_pair(7, 7));

    // Non-divisor at highest index (index n-2): [2,6,12] -> x=12, i=1: 6 divides 12, no duplicate; i=0: 2 divides 12, so no return; final returns {12,12}
    assert(findTwoLargestDivisors(3, {2, 6, 12}) == std::make_pair(12, 12));

    // Another: [6,10,10] -> x=10, i=1: value 10, x%10==0, but sorted[1]==sorted[0]? sorted[1]=10, sorted[0]=6, not equal; continue; i=0: 6 does not divide 10, return {10,6}
    assert(findTwoLargestDivisors(3, {6, 10, 10}) == std::make_pair(10, 6));

    // Large array with duplicates and non-divisors: [1,3,3,5,15] -> x=15, i=3: value 5 divides 15, no dup (5 vs 3); i=2: value 3 divides 15, but sorted[2]==sorted[1] (3==3), returns {15,3}
    assert(findTwoLargestDivisors(5, {1, 3, 3, 5, 15}) == std::make_pair(15, 3));

    // All distinct divisors: [1,2,4,8] -> x=8, i=2: 4 divides, no dup; i=1: 2 divides; i=0: 1 divides; final returns {8,8}
    assert(findTwoLargestDivisors(4, {1, 2, 4, 8}) == std::make_pair(8, 8));
}
// The problem is essentially a variant of finding two numbers from a multiset where the first is the maximum, and the second is the largest element that either fails to divide the maximum or is duplicated. The approach: first sort the array ascending (copy to avoid modifying input). The maximum `x` is the last element. Then iterate from the second-to-last index down to index 0 (or handle `n==1` separately). For each index `i` (starting at `n-2`), check if `x % arr[i] != 0` (i.e., `arr[i]` does not divide `x`) OR `arr[i] == arr[i-1]` (duplicate). If either condition is true, set `y = arr[i]` and return. If the loop completes without finding such `y`, return `{x, x}`. Edge cases: when `n==1`, return `{x,x}` because there is no second element. When all elements are distinct divisors of `x` (e.g., `[1,2,4]` for `x=4`), the loop fails, so return `{x,x}`. Duplicate handling: if the maximum appears multiple times, then at the first iteration `i=n-2`, if `arr[i]==x` (since duplicate of max), the condition `arr[i]==arr[i-1]` (if `i-1` exists) is true, so `y=x` is returned, giving `{x,x}`. Time complexity: O(n log n) due to sorting, plus O(n) for scanning, so O(n log n). Space complexity: O(n) for the copy of the vector (or O(1) if sorting in-place is allowed; but we need to not modify input, so O(n) copy).
