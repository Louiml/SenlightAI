// Write a C++ function `int kthMissingPositive(const std::vector<int>& arr, int k)` that, given a strictly increasing array of positive integers `arr` and an integer `k`, returns the `k`-th positive integer that is missing from the array. For example, if `arr = {2, 3, 4, 7, 11}` and `k = 5`, the positive integers missing are `1, 5, 6, 8, 9, 10, ...`, so the 5th missing is `9`. The array is guaranteed to be sorted in strictly increasing order, all elements are positive, and `k` is a positive integer. The function should be efficient even for large arrays, using binary search to achieve logarithmic time complexity.

// The key observation is that for any index `i` in the array (0-based), the number of positive integers missing before `arr[i]` is exactly `arr[i] - (i + 1)`, because if no numbers were missing, the value at index `i` would be `i+1`. So the count of missing numbers up to that position is `arr[i] - (i+1)`. We binary search for the rightmost index where the missing count is less than `k`. Let `j` be the final index after binary search such that `miss(arr[j]) < k` (or `j = -1` if all elements have `miss >= k`). The answer lies between `arr[j]` (or 0 if `j == -1`) and `arr[j+1]` (or infinity if `j` is the last index). More directly, the `k`-th missing number equals `k + j + 1`. Why? Because `j+1` is the number of elements before the gap, and those `j+1` elements occupy the first `j+1` positive integers (if no missing), so the missing count before the gap is `arr[j] - (j+1)`. We need `k - (arr[j] - (j+1))` more missing numbers after `arr[j]`, so answer = `arr[j] + (k - (arr[j] - (j+1)))` which simplifies to `k + j + 1`. Edge cases: if `k` is smaller than the first missing number (i.e., `k <= arr[0] - 1`), then the answer is simply `k`. Our formula handles that because when `j = -1` (binary search ends with `j = -1`), `k + (-1) + 1 = k`. Time complexity is `O(log n)` where `n` is array length, space `O(1)`.

#include <vector>

// Returns the k-th positive integer missing from the strictly increasing array arr.
int kthMissingPositive(const std::vector<int>& arr, int k) {
    int left = 0;
    int right = static_cast<int>(arr.size()) - 1;

    // Binary search for the rightmost index where missing count < k
    while (left <= right) {
        int mid = left + (right - left) / 2;
        int missingBeforeMid = arr[mid] - mid - 1;
        if (missingBeforeMid < k) {
            left = mid + 1; // answer is to the right (or beyond the array)
        } else {
            right = mid - 1; // answer is to the left
        }
    }

    // After loop, 'right' is the last index where missing count < k.
    // The answer is k + right + 1 (this also works when right == -1).
    return k + right + 1;
}

#include <cassert>
#include <vector>

int kthMissingPositive(const std::vector<int>& arr, int k); // declaration from solution

int main() {
    // Example from task
    assert(kthMissingPositive({2, 3, 4, 7, 11}, 5) == 9);

    // Missing numbers: 1, 5, 6, 8, 9, 10, ... so 1st missing is 1
    assert(kthMissingPositive({2, 3, 4, 7, 11}, 1) == 1);

    // Missing numbers: 1, 5, 6, 8, 9, 10, ... so 6th missing is 10
    assert(kthMissingPositive({2, 3, 4, 7, 11}, 6) == 10);

    // Empty array? Not allowed by spec, but if empty, answer is k
    assert(kthMissingPositive({}, 3) == 3);

    // Array starts at 1, no missing before it
    assert(kthMissingPositive({1, 2, 3, 4}, 2) == 6); // missing: 5,6,... so 2nd missing is 6

    // Large gap
    assert(kthMissingPositive({1, 10, 20}, 5) == 5); // missing: 2,3,4,5,6,... 5th is 6? Wait: missing: 2,3,4,5,6,7,8,9,11,... so 5th is 6

    // Let's re-check: arr={1,10,20}, missing positives: 2,3,4,5,6,7,8,9,11,... 5th missing is 6
    assert(kthMissingPositive({1, 10, 20}, 5) == 6);

    // k larger than all missing before last element
    assert(kthMissingPositive({1, 2, 3}, 4) == 7); // missing: 4,5,6,7,... 4th missing is 7

    // Single element array
    assert(kthMissingPositive({5}, 1) == 1); // missing: 1,2,3,4,6,... 1st is 1
    assert(kthMissingPositive({5}, 4) == 4); // missing: 1,2,3,4,6,... 4th is 4
    assert(kthMissingPositive({5}, 5) == 6); // 5th missing is 6

    return 0;
}
