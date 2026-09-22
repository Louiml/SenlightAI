// Write a C++ function named `printTargetSumPairs` that takes an array of integers, the number of elements in the array (n), and a target integer (t). The function should find and print every distinct pair of elements (i, j) with i < j such that arr[i] + arr[j] == t, but each pair must be printed with the smaller number first, and when two pairs contain the same two numbers (e.g., (2,5) from positions 0 and 3 vs (5,2) from positions 2 and 4), they are considered the same **value pair** and should only be printed once, regardless of which array indices produced them. The output format for each printed pair is `"smaller and larger"` each on its own line, in the order the pairs are first encountered while scanning the array with nested loops (i from 0 to n-2, j from i+1 to n-1). If no such pairs exist, the function should print nothing. The function must not modify the input array and should work for arrays with negative numbers, zeros, and duplicate values.
#include <cassert>
#include <sstream>
#include <iostream>

// Redirect cout to test output. We'll use a simple helper to capture.
// Since we need to test the print function, we temporarily switch cout's buffer.
void runTest(const int arr[], int n, int target, const std::string& expected) {
    std::stringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());
    printTargetSumPairs(arr, n, target);
    std::cout.rdbuf(old);
    assert(buffer.str() == expected);
}

int main() {
    // Basic case
    int arr1[] = {1, 2, 3, 4, 5};
    runTest(arr1, 5, 5, "1 and 4\n2 and 3\n");

    // Negative numbers and zeros
    int arr2[] = {-3, -2, 0, 2, 3};
    runTest(arr2, 5, 0, "-3 and 3\n-2 and 2\n");

    // Duplicate values produce same pair only once
    int arr3[] = {1, 1, 4, 4, 5};
    runTest(arr3, 5, 5, "1 and 4\n");

    // No pairs
    int arr4[] = {1, 2, 3};
    runTest(arr4, 3, 10, "");

    // Single element
    int arr5[] = {7};
    runTest(arr5, 1, 7, "");

    // All identical values, target twice the value
    int arr6[] = {3, 3, 3};
    runTest(arr6, 3, 6, "3 and 3\n");

    // Larger array, multiple distinct pairs
    int arr7[] = {1, -1, 2, -2, 0, 0};
    runTest(arr7, 6, 0, "-2 and 2\n-1 and 1\n0 and 0\n");

    std::cout << "All tests passed!" << std::endl;
    return 0;
}
#include <iostream>
#include <set>
#include <utility>

// Prints all distinct value pairs (a, b) with a < b such that a + b == target.
// Each pair is printed once, in the order first encountered, as "a and b".
void printTargetSumPairs(const int arr[], int n, int target) {
    std::set<std::pair<int, int>> printedPairs;

    for (int i = 0; i <= n - 2; ++i) {
        for (int j = i + 1; j <= n - 1; ++j) {
            int sum = arr[i] + arr[j];
            if (sum == target) {
                int smaller = (arr[i] < arr[j]) ? arr[i] : arr[j];
                int larger  = (arr[i] < arr[j]) ? arr[j] : arr[i];
                auto candidate = std::make_pair(smaller, larger);

                if (printedPairs.find(candidate) == printedPairs.end()) {
                    printedPairs.insert(candidate);
                    std::cout << smaller << " and " << larger << std::endl;
                }
            }
        }
    }
}
// The solution uses two nested loops: an outer loop with index `i` from 0 to n-2 and an inner loop with index `j` from i+1 to n-1. For each pair (i, j), compute the sum `arr[i] + arr[j]`. If the sum equals the target `t`, we need to output the pair with the smaller value first. To avoid printing duplicate value pairs (for example, if the array contains the pair `(2,5)` multiple times with different indices), we must track which value pairs have already been printed. A `std::set<std::pair<int,int>>` works well: before printing, construct the normalized pair `(min(arr[i],arr[j]), max(arr[i],arr[j]))` and check if it exists in the set. If not, insert it and print. This ensures each distinct value pair is printed only once, preserving the order of first occurrence. Edge cases: empty array (n=0) — loops do not run; array with one element — no pairs; negative numbers and zeros — the min/max logic works; duplicates — the set prevents repeated printing. Time complexity is O(n²) for the loops, plus O(log p) for each set insertion where p is the number of valid pairs; in the worst case p is O(n²), so the total is O(n² log n). Space complexity is O(p) for the set, which is at most O(n²) in the worst case (e.g., all elements equal to t/2, each pair sums to t). Since the problem asks for printing, we use `std::cout` inside the function.
