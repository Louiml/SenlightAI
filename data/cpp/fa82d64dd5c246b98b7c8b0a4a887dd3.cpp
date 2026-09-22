Write a C++ function that takes a vector of integers and a target sum `s`, and returns the length of the shortest contiguous subarray whose sum is at least `s`. If no such subarray exists, return 0. The input vector may contain both positive and negative integers, and the target `s` can be any integer (including non-positive values, in which case the shortest valid subarray is simply the first element if it is ≥ `s`, or possibly a longer subarray if the first element is smaller — but with negative numbers, the sliding window logic must handle carefully; for simplicity, assume the vector may be empty and handle that case). Use a sliding window (two-pointer) technique, and do not use any extra data structures beyond constant extra space. Ensure the function is `const`-correct and works for large input sizes.
// The core algorithm is a two-pointer sliding window. We maintain `start` and `end` indices representing the current window, and `sum` as the running sum of `arr[start..end]`. Initially, `start=0`, `end=0`, `sum=arr[0]`, and `len=1`. We iterate while `end < n`. At each step:
// - If `sum >= s`, we record the current window length as a candidate for the minimum, then shrink the window from the left by subtracting `arr[start]` and incrementing `start`. We also decrement `len`. If after shrinking `start > end`, we reset both to `start` (which equals the old `end+1`) and set `len=1`, then add that new element.
// - If `sum < s`, we expand the window to the right by incrementing `end` and adding `arr[end]` to `sum`, then increment `len`.
//
// Edge cases: (1) empty vector — the original code reads `arr[0]` which is undefined; we handle by checking if `n==0` and returning 0. (2) If the first element already meets the threshold, we can return 1 immediately, but the general logic handles it. (3) If the total sum of all elements is less than `s`, the minimum length remains at its initial sentinel value (n+1) and we return 0. (4) Negative numbers: the window shrinking condition might prematurely shrink, but the algorithm still finds the shortest window because it only records length when `sum >= s`, and shrinking only occurs when the window is valid. However, with negative numbers, a longer window might have a lower sum, so after recording, we shrink; this is correct because any larger window containing the current one would have sum ≥ current (since we only add positive/negative? actually adding negative could decrease sum, so you cannot guarantee that extending further would keep sum ≥ s. But the classic sliding window works only if all elements are non-negative. Given the snippet uses it, we assume the problem intends non-negative integers for correctness, but we'll state that the solution assumes non-negative elements, as is typical for this problem. We'll mention this in the analysis.)
//
// Time complexity: O(n) since each index is added and removed at most once. Space complexity: O(1) auxiliary.
#include <vector>
#include <algorithm>

// Returns the length of the shortest contiguous subarray whose sum is at least s.
// If no such subarray exists, returns 0.
// The vector is assumed to contain non-negative integers for correct sliding window behavior.
int shortestSubarrayWithSumAtLeast(const std::vector<int>& arr, int s) {
    const int n = static_cast<int>(arr.size());
    if (n == 0) {
        return 0;
    }

    int minLen = n + 1; // sentinel for "not found"
    int windowSum = arr[0];
    int start = 0;
    int end = 0;
    int currentLen = 1;

    while (end < n) {
        if (windowSum >= s) {
            minLen = std::min(minLen, currentLen);
            // Shrink window from left
            windowSum -= arr[start];
            ++start;
            --currentLen;
            if (start > end) {
                // Window became empty; move start and end to next element
                ++end;
                start = end;
                if (end < n) {
                    windowSum = arr[end];
                    currentLen = 1;
                }
            }
        } else {
            // Expand window to right
            ++end;
            if (end < n) {
                windowSum += arr[end];
                ++currentLen;
            }
        }
    }

    return (minLen == n + 1) ? 0 : minLen;
}
#include <cassert>
#include <vector>
#include <iostream>

// Declare the function from the solution (for testing; in a real project, this would be in a header).
int shortestSubarrayWithSumAtLeast(const std::vector<int>& arr, int s);

int main() {
    // Basic cases with non-negative numbers
    assert(shortestSubarrayWithSumAtLeast({1, 2, 3, 4, 5}, 7) == 2); // [3,4] or [4,5]? Actually [3,4]=7 len2
    assert(shortestSubarrayWithSumAtLeast({2, 3, 1, 2, 4, 3}, 7) == 2); // [4,3] len2
    assert(shortestSubarrayWithSumAtLeast({1, 1, 1, 1}, 5) == 0); // total sum=4 <5
    assert(shortestSubarrayWithSumAtLeast({5}, 5) == 1);
    assert(shortestSubarrayWithSumAtLeast({5, 1, 1}, 5) == 1);
    assert(shortestSubarrayWithSumAtLeast({1, 2, 3}, 6) == 3);
    assert(shortestSubarrayWithSumAtLeast({1, 2, 3}, 1) == 1);
    assert(shortestSubarrayWithSumAtLeast({10, 2, 3}, 10) == 1);
    assert(shortestSubarrayWithSumAtLeast({0, 0, 1}, 1) == 1); // [1] at index2
    assert(shortestSubarrayWithSumAtLeast({}, 5) == 0); // empty vector

    std::cout << "All tests passed!" << std::endl;
    return 0;
}
