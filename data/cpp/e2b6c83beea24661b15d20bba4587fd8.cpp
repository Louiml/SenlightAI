// Write a C++ function named `canFormConsecutiveSequence` that takes a `const std::vector<int>&` representing a permutation of integers from 1 to n (where n is the vector size) and returns a `std::string` containing `"Yes"` if the array can be sorted into a consecutive sequence by only swapping adjacent elements where the right element is exactly one greater than the left element, and `"No"` otherwise. Specifically, the function must check that for every pair of adjacent elements in the original array, if the left element is less than the right element, then the difference must be exactly 1. If any adjacent pair violates this condition (i.e., left < right and difference ≠ 1), the array cannot be made consecutive through such swaps, so return `"No"`. Otherwise, return `"Yes"`. The input vector will have at least one element, and all elements are distinct positive integers.

The problem reduces to checking a local condition on every adjacent pair. The given code snippet incorrectly assumes that if `a[i] < a[i+1]` and the difference is not 1, then it's impossible, but if `a[i] > a[i+1]`, it always accepts. However, the intended logic is exactly that: we only care about increasing adjacent pairs that skip values. If an adjacent pair is increasing but not by exactly 1, then there is no way to fix it via swapping because swapping adjacent elements that are not consecutive in value will never make them consecutive—the larger element must move left past the smaller, but that would require an intermediate value. Since the array is a permutation of 1..n, the only acceptable increasing adjacent difference is 1; any other increasing difference (e.g., 2, 3, etc.) means there is a "gap" that cannot be filled by the other elements because they are distinct and all values are present somewhere. However, if a pair is decreasing, that is fine because we can swap them to make them increasing, and the difference is already positive (e.g., 3 then 2 can become 2 then 3). Therefore, the algorithm iterates through the vector from index 0 to n-2, and for each pair, if `a[i] < a[i+1]` and `a[i+1] - a[i] != 1`, return `"No"`. Otherwise, after the loop, return `"Yes"`. Edge cases: single element always returns `"Yes"` because no adjacent pairs exist; a sequence already perfectly increasing by 1 returns `"Yes"`; a sequence with any increasing gap of more than 1 returns `"No"`. Time complexity is O(n) with O(1) auxiliary space.

#include <string>
#include <vector>

// Check if the given vector can be made into a consecutive sequence
// under the rule that only adjacent swaps of elements that differ by 1 are allowed.
// Returns "Yes" if every increasing adjacent pair has a difference of exactly 1,
// otherwise "No".
std::string canFormConsecutiveSequence(const std::vector<int>& arr) {
    for (size_t i = 0; i + 1 < arr.size(); ++i) {
        if (arr[i] < arr[i + 1] && arr[i + 1] - arr[i] != 1) {
            return "No";
        }
    }
    return "Yes";
}

#include <cassert>
#include <vector>
#include <string>

// The solution function is declared above in the actual code, but for test completeness we include a declaration here.
std::string canFormConsecutiveSequence(const std::vector<int>& arr);

int main() {
    // Single element: trivially yes
    assert(canFormConsecutiveSequence({1}) == "Yes");

    // Already consecutive increasing sequence
    assert(canFormConsecutiveSequence({1,2,3,4}) == "Yes");

    // Decreasing pairs are allowed
    assert(canFormConsecutiveSequence({4,3,2,1}) == "Yes");

    // Mixed: decreasing then increasing by 1 is fine
    assert(canFormConsecutiveSequence({3,1,2}) == "Yes");

    // Increasing gap of 2 is not allowed
    assert(canFormConsecutiveSequence({1,3}) == "No");

    // Increasing gap of 3 later in sequence
    assert(canFormConsecutiveSequence({2,1,4,3}) == "Yes"); // all pairs are decreasing

    // Case with an increasing gap of 2 in middle
    assert(canFormConsecutiveSequence({1,2,4,3}) == "No");

    // Larger permutation with a skip
    assert(canFormConsecutiveSequence({1,3,2,4}) == "No");

    // Fully shuffled but all adjacent increases by 1
    assert(canFormConsecutiveSequence({2,3,1}) == "Yes");

    return 0;
}
