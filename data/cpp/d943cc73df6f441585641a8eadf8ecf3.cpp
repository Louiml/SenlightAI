// Write a C++ function `bool subsetWithSumExists(int arr[], int n, int targetSum, std::vector<int>& result)` that, given an array of integers `arr` of size `n` and a target sum `targetSum`, finds any subset (not necessarily contiguous) whose elements sum exactly to `targetSum`. If such a subset exists, the function should store the chosen elements into the `result` vector (in the order they appear in the original array) and return `true`. If no such subset exists, the function should leave `result` empty and return `false`. The function must work for arrays with negative numbers, positive numbers, zeros, and duplicate values. Assume `n >= 0` and `targetSum` can be any integer. The subset can be empty (sum 0), which is allowed only if `targetSum == 0`. The function should be recursive and must not use global variables.

#include <cassert>
#include <vector>
#include <numeric>

int main() {
    // Test 1: Basic case
    int arr1[] = {1, 2, 3};
    std::vector<int> result;
    bool ok = subsetWithSumExists(arr1, 3, 5, result);
    assert(ok == true);
    assert(std::accumulate(result.begin(), result.end(), 0) == 5);

    // Test 2: No solution
    int arr2[] = {1, 2};
    result.clear();
    ok = subsetWithSumExists(arr2, 2, 10, result);
    assert(ok == false);
    assert(result.empty());

    // Test 3: Empty subset for sum 0
    int arr3[] = {1, 2, 3};
    result.clear();
    ok = subsetWithSumExists(arr3, 3, 0, result);
    assert(ok == true);
    assert(result.empty());

    // Test 4: Negative numbers
    int arr4[] = {-3, -1, 2, 4};
    result.clear();
    ok = subsetWithSumExists(arr4, 4, 0, result);
    assert(ok == true);
    assert(std::accumulate(result.begin(), result.end(), 0) == 0);

    // Test 5: Duplicate values
    int arr5[] = {2, 2, 2};
    result.clear();
    ok = subsetWithSumExists(arr5, 3, 4, result);
    assert(ok == true);
    assert(std::accumulate(result.begin(), result.end(), 0) == 4);

    // Test 6: Empty array, sum 0
    int arr6[] = {};
    result.clear();
    ok = subsetWithSumExists(arr6, 0, 0, result);
    assert(ok == true);
    assert(result.empty());

    // Test 7: Empty array, sum non-zero
    ok = subsetWithSumExists(arr6, 0, 5, result);
    assert(ok == false);
    assert(result.empty());

    // Test 8: Single element matching
    int arr8[] = {7};
    result.clear();
    ok = subsetWithSumExists(arr8, 1, 7, result);
    assert(ok == true);
    assert(result.size() == 1 && result[0] == 7);

    // Test 9: Many elements, sum known
    int arr9[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    result.clear();
    ok = subsetWithSumExists(arr9, 10, 55, result);
    assert(ok == true);
    assert(std::accumulate(result.begin(), result.end(), 0) == 55);

    // Test 10: Zeros and negatives
    int arr10[] = {0, 0, -1, -2, 3};
    result.clear();
    ok = subsetWithSumExists(arr10, 5, 0, result);
    assert(ok == true);
    assert(std::accumulate(result.begin(), result.end(), 0) == 0);

    return 0;
}

#include <vector>

// Recursive helper to search for a subset summing to targetSum.
// arr: input array, n: number of elements, index: current position,
// currentSum: sum so far, targetSum: desired sum, temp: current subset candidates,
// result: stores the final subset if found.
bool subsetSumHelper(const int arr[], int n, int index, int currentSum, int targetSum,
                     std::vector<int>& temp, std::vector<int>& result) {
    if (index == n) {
        if (currentSum == targetSum) {
            result = temp; // copy the valid subset
            return true;
        }
        return false;
    }

    // Option 1: include arr[index]
    temp.push_back(arr[index]);
    if (subsetSumHelper(arr, n, index + 1, currentSum + arr[index], targetSum, temp, result))
        return true;
    temp.pop_back(); // backtrack

    // Option 2: exclude arr[index]
    if (subsetSumHelper(arr, n, index + 1, currentSum, targetSum, temp, result))
        return true;

    return false;
}

// Public function to find any subset summing to targetSum.
bool subsetWithSumExists(const int arr[], int n, int targetSum, std::vector<int>& result) {
    result.clear(); // ensure empty on failure
    if (n < 0) return false; // invalid input
    std::vector<int> temp;
    bool found = subsetSumHelper(arr, n, 0, 0, targetSum, temp, result);
    if (!found) result.clear();
    return found;
}

// The problem is a classic subset sum decision problem with the additional requirement of retrieving one valid subset. The solution uses a recursive backtracking approach: at each index, we have two possibilities — either include the current element in the subset or exclude it. We traverse the array from index 0 to n. When we reach the end (index == n), we check whether the accumulated sum equals the target. If yes, we have found a valid subset — we then return true and propagate the result upward. Because we need a single subset (any one), we can stop as soon as we find a valid combination. To reconstruct the subset, we maintain a temporary vector that is filled while recursing; when we find a solution, that vector already contains the chosen elements. The base case handles the scenario where all elements have been considered. This approach explores all 2^n combinations in the worst case, but early termination can reduce exploration if a solution is found quickly. Edge cases: empty array (n=0) — only valid if targetSum is 0 (returns empty subset). Negative numbers and zeros are handled naturally by the recursive accumulation. Duplicate values are fine since each index is considered independently. Time complexity is O(2^n) in the worst case, with O(n) auxiliary space for the recursion stack and the result vector (excluding the input array). The function should be `const`-correct where possible and use pass-by-reference for the result vector to avoid copying.
