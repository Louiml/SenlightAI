/*
Write a C++ function `printSubsetsWithSum` that takes a vector of distinct integers `arr`, a target sum `target`, and a starting index `index`, and prints every subset of `arr` (starting from `index`) whose elements sum exactly to `target`. The function must use recursion with backtracking and print each matching subset in the format `[ element1 element2 ... ]` with a newline after each subset. The elements within a subset should be printed in the order they appear in `arr`. The function should handle empty arrays, zero target, negative target, and cases where no subset matches. If `target` is zero, an empty subset should be printed as `[ ]`. The function must not return a value; it should only print results to standard output. You may assume the input vector contains distinct positive integers (but still handle negative targets gracefully).
*/
#include <iostream>
#include <vector>

// Recursively find and print all subsets of arr (starting from index) whose sum equals target.
// Prints each matching subset as "[ e1 e2 ... ]" with a newline.
void printSubsetsWithSum(const std::vector<int>& arr, int target, std::vector<int>& subset, int index) {
    // Base case: target reached, print current subset.
    if (target == 0) {
        std::cout << "[ ";
        for (int num : subset) {
            std::cout << num << " ";
        }
        std::cout << "]" << std::endl;
        return;
    }

    // Stop if end of array or overshoot.
    if (index == static_cast<int>(arr.size()) || target < 0) {
        return;
    }

    // Include current element in subset.
    subset.push_back(arr[index]);
    printSubsetsWithSum(arr, target - arr[index], subset, index + 1);

    // Backtrack: remove and try excluding current element.
    subset.pop_back();
    printSubsetsWithSum(arr, target, subset, index + 1);
}
#include <cassert>
#include <iostream>
#include <sstream>
#include <vector>

// Redirect cout to capture output for testing.
void runTest(const std::vector<int>& arr, int target, const std::string& expected) {
    std::ostringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());

    std::vector<int> subset;
    printSubsetsWithSum(arr, target, subset, 0);

    std::cout.rdbuf(old);
    assert(buffer.str() == expected);
}

int main() {
    // Test 1: Example from prompt
    runTest({2, 3, 5}, 5, "[ 2 3 ]\n[ 5 ]\n");

    // Test 2: No matching subset (empty output)
    runTest({1, 2, 3}, 10, "");

    // Test 3: Zero target prints empty subset
    runTest({1, 2, 3}, 0, "[ ]\n");

    // Test 4: Single element equals target
    runTest({7}, 7, "[ 7 ]\n");

    // Test 5: Negative target yields nothing
    runTest({1, 2, 3}, -1, "");

    // Test 6: Multiple subsets with same elements? Assume distinct, but test 1+4=5 and 5
    runTest({1, 4, 5}, 5, "[ 1 4 ]\n[ 5 ]\n");

    // Test 7: Empty array with nonzero target
    runTest({}, 3, "");

    // Test 8: Pathological large target, no match
    runTest({1, 2, 3}, 100, "");

    // Test 9: All elements sum to target
    runTest({1, 2, 3}, 6, "[ 1 2 3 ]\n");

    // Test 10: Duplicate subsets not expected, but test with target equals one element
    runTest({5, 5, 5}, 5, "[ 5 ]\n[ 5 ]\n[ 5 ]\n");

    std::cout << "All tests passed!" << std::endl;
    return 0;
}
// The solution uses standard recursive backtracking. At each index, two branches are explored: one that includes `arr[index]` in the current subset (reducing the remaining target by `arr[index]`) and one that excludes it (keeping the target unchanged). The base case occurs when `target == 0`, at which point the current subset is printed. If `index` reaches the end of the array or `target` becomes negative, the recursion stops for that branch. A backtracking step (popping the last element) ensures the subset vector is restored before exploring the exclusion branch. Edge cases include: an empty array (no output unless target is zero, but the function will immediately index==size, so nothing printed), zero target (prints `[ ]` immediately at the first call), negative target (immediately returns), and duplicates not present by assumption. Time complexity is \(O(2^n)\) in the worst case because every subset may need to be considered, and space complexity is \(O(n)\) for the recursion stack and the subset vector.
