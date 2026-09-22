/*
Write a C++ function that processes multiple pairs of integers from standard input and returns a formatted string summarizing all case results. The function should take three parameters: the number of cases, and two arrays (or vectors) of integers representing the first and second values for each case. It should return a single string where each case produces a line formatted exactly as "Case i: sum" where i is the 1-based case number and sum is the integer sum of the two input values for that case. The function must preserve input order, handle all integer values (including negative numbers and zero), and produce no extra whitespace except for a single newline after each case line. The function should not read from or write to standard input/output; it must only work with the provided data structures.
*/
#include <string>
#include <vector>
#include <cstddef>

// Returns a formatted string with one "Case i: sum" line per input pair.
std::string formatCaseSums(int n, const std::vector<int>& a, const std::vector<int>& b) {
    std::string result;
    for (int i = 0; i < n; ++i) {
        int sum = a[i] + b[i];
        result += "Case " + std::to_string(i + 1) + ": " + std::to_string(sum) + '\n';
    }
    return result;
}
#include <cassert>
#include <string>
#include <vector>

int main() {
    // Test with three normal positive cases
    std::vector<int> a1 = {1, 2, 3};
    std::vector<int> b1 = {4, 5, 6};
    assert(formatCaseSums(3, a1, b1) == "Case 1: 5\nCase 2: 7\nCase 3: 9\n");
    
    // Test with negative numbers and zero
    std::vector<int> a2 = {-5, 0, 10};
    std::vector<int> b2 = {7, -2, -10};
    assert(formatCaseSums(3, a2, b2) == "Case 1: 2\nCase 2: -2\nCase 3: 0\n");
    
    // Test with a single case
    std::vector<int> a3 = {100};
    std::vector<int> b3 = {200};
    assert(formatCaseSums(1, a3, b3) == "Case 1: 300\n");
    
    // Test with zero cases (empty result)
    std::vector<int> a4;
    std::vector<int> b4;
    assert(formatCaseSums(0, a4, b4) == "");
    
    // Test with larger values (still within int range)
    std::vector<int> a5 = {100000, -100000};
    std::vector<int> b5 = {99999, -99999};
    assert(formatCaseSums(2, a5, b5) == "Case 1: 199999\nCase 2: -199999\n");
    
    // Test that order is preserved correctly with mixed signs
    std::vector<int> a6 = {1, -1, 5};
    std::vector<int> b6 = {-1, 1, -5};
    assert(formatCaseSums(3, a6, b6) == "Case 1: 0\nCase 2: 0\nCase 3: 0\n");
}
// The solution involves iterating through the input arrays in parallel, computing the sum for each pair, and appending a formatted line to a result string. Since the problem specifies a fixed number of cases, no special handling for invalid input is needed beyond assuming the arrays are of size n. The main algorithm is a simple loop from index 0 to n-1, using `std::to_string` to convert the sum to a string, and constructing each line as "Case " + std::to_string(i+1) + ": " + std::to_string(sum). A newline character is added after each case. The time complexity is O(n) because we process each pair exactly once, and the space complexity is O(n) because we need to build and return the output string that contains n lines. Edge cases include n=0 (empty input) where the function returns an empty string, negative sums, and sums that are zero. No overflow is expected for typical test cases, but if needed, using `long long` could be safer; however, for this task, `int` is sufficient given the standard constraints.
