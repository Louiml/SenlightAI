// Write a C++ function `canMakeAllElementsSmallEnough(std::vector<int>& numbers, int limit)` that determines whether there exists a way to make every element in the array less than or equal to a given positive integer `limit`. You can perform any number of operations, where each operation replaces an element `x` with `a + b` for any two (not necessarily distinct) elements `a` and `b` currently in the array, and then removes the original `x`. You may also choose to leave the array unchanged. The function should take a vector of integers (length between 1 and 1,000,000) and the limit (positive integer up to 1e9), and return `true` if it is possible to make all numbers ≤ limit, and `false` otherwise. The vector may be modified, but you must restore it to its original order and values before returning (or work on a copy). Duplicate values and negative integers are allowed, but the limit is always positive.
// The key insight is that if the largest element in the array is already ≤ limit, then the answer is trivially `true`. If not, we must use the two smallest elements to "merge" larger elements down. Since we can always split a large element into a sum of two smaller ones, the optimal strategy is to sort the array. The smallest element `min1` and second smallest `min2` are the best candidates to produce sums ≤ limit, because using any larger elements would only make sums larger. If `min1 + min2` is > limit, then no sum of any two elements (except possibly the largest if it's already small, but we already handled that) can be ≤ limit, so it's impossible. If `min1 + min2` ≤ limit, then we can repeatedly replace the current largest element with `min1 + min2` (which is ≤ limit) and also keep the original elements. In fact, we only need to worry about the largest element; once we can create a sum ≤ limit, we can keep replacing the largest element until all rest are ≤ limit, because after replacing the largest with the sum, the new largest will be at most the previous second largest, which is ≤ previous largest, and we can continue. Thus directly, it suffices to check whether `min(largest, min1+min2) ≤ limit`. Edge cases: array size 1 – only need that single element ≤ limit; negative numbers – sorting puts most negative first; if there are negative numbers, min1+min2 might be very small or negative, which is always ≤ limit, so answer is true unless the largest is already > limit but min1+min2 > limit? That can't happen because if min1+min2 > limit and min1 is most negative, then all elements are positive and large, so impossible. Complexity: O(n log n) for sorting, O(1) extra space if we sort in place; but we need to restore original order if we modify, so we either copy (O(n) space) or sort a copy. Time O(n log n), space O(n) for copy.
#include <algorithm>
#include <vector>

/**
 * Determines if we can make all numbers in the array <= limit by repeatedly
 * replacing any element x with the sum of two existing elements a and b.
 * The original array is not modified.
 */
bool canMakeAllElementsSmallEnough(const std::vector<int>& numbers, int limit) {
    if (numbers.empty()) return true; // Edge case not in spec but safe.
    // Work on a copy to avoid modifying input.
    std::vector<int> sorted = numbers;
    std::sort(sorted.begin(), sorted.end());
    
    int n = sorted.size();
    // If the largest is already ok, we are done.
    if (sorted.back() <= limit) return true;
    // If we have only one element and it's too big, impossible.
    if (n == 1) return false;
    // Smallest and second smallest.
    int smallest = sorted[0];
    int secondSmallest = sorted[1];
    // The only way to reduce the largest is to replace it with a sum of two numbers.
    // The smallest possible sum is smallest + secondSmallest.
    // If that sum exceeds limit, no operation can ever produce a value <= limit.
    return (long long)smallest + secondSmallest <= limit;
}
#include <cassert>
#include <vector>

// Assume function declaration from solution.
bool canMakeAllElementsSmallEnough(const std::vector<int>&, int);

int main() {
    // Basic: already all small
    assert(canMakeAllElementsSmallEnough({1, 2, 3}, 3) == true);
    // One element too big, cannot fix
    assert(canMakeAllElementsSmallEnough({10}, 5) == false);
    // Two small enough to sum and fix the big one
    assert(canMakeAllElementsSmallEnough({5, 1, 2, 10}, 5) == true); // 1+2=3 <=5
    // Two small sum too large to fix the big one
    assert(canMakeAllElementsSmallEnough({4, 4, 10}, 7) == false); // 4+4=8 >7
    // Negative numbers: sum is very small, so always possible if any pair works
    assert(canMakeAllElementsSmallEnough({-5, 1, 100}, 10) == true); // -5+1=-4 <=10
    // All equal and too big but sum of two equals? 6+6=12 >10
    assert(canMakeAllElementsSmallEnough({6, 6, 6}, 10) == false);
    // Large numbers, limit is exactly sum
    assert(canMakeAllElementsSmallEnough({3, 4, 10}, 7) == true); // 3+4=7
    // Edge: two elements, second is large, first is small, sum works
    assert(canMakeAllElementsSmallEnough({1, 100}, 50) == false); // 1+100=101 >50
    assert(canMakeAllElementsSmallEnough({1, 100}, 101) == true); // 1+100=101, but largest 100 already <=101? Actually 100<=101 true
    // Ensure original vector not modified
    std::vector<int> v = {2, 9, 1};
    canMakeAllElementsSmallEnough(v, 5);
    assert(v.size() == 3 && v[0] == 2 && v[1] == 9 && v[2] == 1);
    return 0;
}
