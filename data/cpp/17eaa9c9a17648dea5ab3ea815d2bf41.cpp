Write a C++ function `int missingKthInteger(const std::vector<int>& numbers, int k)` that takes a vector of positive integers (already provided in arbitrary order and containing no duplicates) and an integer `k` (which may be 0). The function should determine the smallest positive integer `x` such that exactly `k` of the given numbers are strictly less than `x`. In other words, after sorting the input numbers, the answer is the value of the `k`-th smallest element if that element is not equal to the next larger element; if `k` equals 0, the answer is 1 unless the smallest number in the input is also 1, in which case no such `x` exists. If no valid `x` exists, the function should return -1. The input vector may be empty (in which case the answer is 1 if `k==0` else -1 because there are no numbers to be less than `x`). The function should work for up to 10^5 elements and values up to 10^9.

// Sort the input array in ascending order. The condition "exactly k numbers are strictly less than x" translates to: `x` must be greater than the `k`-th smallest element (if `k>0`) and less than or equal to the next element (if `k < n`). If `k == 0`, then no numbers are less than `x`, so `x` must be 1 (the smallest positive integer) unless the smallest element is also 1, in which case 1 is in the array and cannot be the answer. For `k > 0`, we need the `k`-th smallest element (1-indexed) to be strictly less than the next distinct value. That is, after sorting, if `v[k-1] == v[k]` (where `v[k]` exists), then there are more than `k` elements equal to that value, so any `x` greater than that value would have more than `k` numbers less than it, and any `x` less than that value would have fewer. Thus no solution. If `v[k-1] < v[k]` or `k == n`, then the answer is `v[k-1]` (since all `k` numbers are less than it, and the next element is strictly larger). Edge cases: empty vector, `k > n` (then -1), `k == 0` with smallest element 1 (then -1). Time complexity O(n log n) for sorting, O(1) additional space.

#include <vector>
#include <algorithm>

// Return the smallest positive integer x such that exactly k elements of numbers are strictly less than x.
// If no such x exists, return -1.
int missingKthInteger(const std::vector<int>& numbers, int k) {
    const int n = static_cast<int>(numbers.size());
    if (k > n) return -1;
    if (n == 0) {
        return (k == 0) ? 1 : -1;
    }

    std::vector<int> sorted = numbers;
    std::sort(sorted.begin(), sorted.end());

    if (k == 0) {
        // Need no numbers less than x; smallest positive x is 1, unless 1 is in the array.
        return (sorted[0] == 1) ? -1 : 1;
    }

    // k >= 1, so sorted[k-1] is the k-th smallest element (1-indexed).
    // We need sorted[k-1] to be strictly less than any element to its right.
    if (k < n && sorted[k-1] == sorted[k]) {
        return -1; // More than k copies, no x works.
    }
    return sorted[k-1];
}

#include <cassert>
#include <vector>

int findKthElement(const std::vector<int>& numbers, int k);

int main() {
    // Basic distinct elements
    assert(findKthElement({5, 1, 3}, 1) == 1);
    assert(findKthElement({5, 1, 3}, 2) == 3);
    assert(findKthElement({5, 1, 3}, 3) == 5);

    // k = 0 cases
    assert(findKthElement({2, 3, 4}, 0) == 1);
    assert(findKthElement({1, 2, 3}, 0) == -1);

    // Duplicate causes -1
    assert(findKthElement({2, 2, 3}, 2) == -1); // second smallest is 2, but duplicate
    assert(findKthElement({2, 2, 3}, 1) == 2);   // first smallest unique? sorted[0]=2, sorted[1]=2, equal -> -1? Wait k=1: sorted[0]=2, sorted[1]=2 equal, so -1.
    // Correct: for k=1, check sorted[0]==sorted[1] -> true, so -1.
    assert(findKthElement({2, 2, 3}, 1) == -1);
    assert(findKthElement({2, 2, 3}, 3) == 3); // k=n, no next to compare, returns 3

    // Empty vector
    assert(findKthElement({}, 0) == 1);
    assert(findKthElement({}, 1) == -1);

    // Larger array with last element
    assert(findKthElement({10, 20, 30, 40}, 4) == 40);
    assert(findKthElement({10, 20, 30, 40}, 0) == 1);
    assert(findKthElement({10, 20, 30, 40}, 2) == 20);

    // Duplicate at end
    assert(findKthElement({1, 2, 2}, 2) == -1); // sorted[1]=2, sorted[2]=2 equal
    assert(findKthElement({1, 2, 2}, 3) == 2); // k=n, returns 2
    return 0;
}
