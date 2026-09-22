Given a sequence of integers that may contain duplicates, and a positive integer `k` not exceeding the number of distinct integers in the sequence, write a C++ function that returns a vector of strings (or a single string with newline separators) representing all unique combinations of exactly `k` elements chosen from the distinct integers, sorted in ascending order. The distinct integers must first be extracted from the input while preserving uniqueness, then sorted. The combinations must be generated in lexicographic (dictionary) order based on the sorted distinct integers, where each combination is a sequence of `k` integers printed in increasing order. For example, if the distinct sorted integers are `{1, 5, 7}` and `k=2`, the output should be lines: `1 5`, `1 7`, `5 7`. The function must not produce duplicate combinations, and must handle cases where the input contains negative numbers or zero, as long as they are within a reasonable range (e.g., absolute value ≤ 1000). The function should be standalone, not require a main loop, and be efficient for inputs with up to 1000 distinct integers.
The problem reduces to generating all combinations of size `k` from a set of `m` distinct integers (where `m` is the number of unique values after removing duplicates). The main steps are: (1) read all integers from a container (e.g., vector<int>), store them in an unordered set or boolean array to eliminate duplicates, then copy to a vector and sort. (2) Use an iterative combination generation algorithm based on an index array `b` of length `k`, initialized to `1..k` (1-based indices into the sorted distinct vector). At each step, print the corresponding elements, then find the rightmost position `i` (from `k` down to 1) where `b[i]` is not at its maximum possible value (`m - k + i` for 1-based). If found, increment `b[i]` and set all following positions to `b[j-1]+1`. If no such position exists, terminate. This is the standard "next combination" algorithm and produces combinations in lexicographic order. Edge cases: `k=0` (should produce empty output or handle gracefully — spec says positive k, so ignore), `k > m` (not allowed by constraints, but function could assert or return empty), duplicates are removed before sorting. Time complexity: The number of combinations is C(m, k), and each combination takes O(k) time to print, so total O(k * C(m, k)). Space complexity is O(m) for storing distinct integers and O(k) for the index array.
#include <vector>
#include <string>
#include <algorithm>
#include <sstream>

// Generate all combinations of exactly k elements from the distinct integers in 'input'.
// Returns a vector of strings, each string containing k integers separated by spaces.
std::vector<std::string> generateCombinations(const std::vector<int>& input, int k) {
    // Step 1: Extract distinct integers using a boolean array for values in range [-1000, 1000]
    const int OFFSET = 1000;
    bool seen[2001] = {false};
    std::vector<int> distinct;
    for (int x : input) {
        if (x < -1000 || x > 1000) continue; // ignore out-of-range values
        int idx = x + OFFSET;
        if (!seen[idx]) {
            seen[idx] = true;
            distinct.push_back(x);
        }
    }
    std::sort(distinct.begin(), distinct.end());
    
    int m = static_cast<int>(distinct.size());
    if (k <= 0 || k > m) {
        return {}; // invalid k, return empty result
    }
    
    // Step 2: Initialize combination indices (1-based)
    std::vector<int> b(k + 1);
    for (int i = 1; i <= k; ++i) {
        b[i] = i;
    }
    
    std::vector<std::string> result;
    while (true) {
        // Build the current combination string
        std::ostringstream oss;
        for (int i = 1; i <= k; ++i) {
            if (i > 1) oss << " ";
            oss << distinct[b[i] - 1];
        }
        result.push_back(oss.str());
        
        // Find the rightmost index that can be incremented
        int pos = 0;
        for (int i = k; i >= 1; --i) {
            if (b[i] != m - k + i) {
                pos = i;
                break;
            }
        }
        if (pos == 0) {
            break; // no more combinations
        }
        
        // Increment and reset following indices
        b[pos]++;
        for (int j = pos + 1; j <= k; ++j) {
            b[j] = b[j - 1] + 1;
        }
    }
    
    return result;
}
#include <cassert>
#include <vector>
#include <string>

// The solution function is declared above (included here for completeness)
// In a real test, include the solution header or copy the function.

int main() {
    // Test 1: Basic case
    std::vector<int> input1 = {1, 2, 3, 4};
    std::vector<std::string> res1 = generateCombinations(input1, 2);
    assert(res1.size() == 6);
    assert(res1[0] == "1 2");
    assert(res1[1] == "1 3");
    assert(res1[2] == "1 4");
    assert(res1[3] == "2 3");
    assert(res1[4] == "2 4");
    assert(res1[5] == "3 4");

    // Test 2: Duplicates are removed
    std::vector<int> input2 = {5, 5, 1, 1, 1, 3};
    auto res2 = generateCombinations(input2, 2);
    assert(res2.size() == 3);
    assert(res2[0] == "1 3");
    assert(res2[1] == "1 5");
    assert(res2[2] == "3 5");

    // Test 3: Negative numbers and zero
    std::vector<int> input3 = {-2, 0, 1};
    auto res3 = generateCombinations(input3, 2);
    assert(res3.size() == 3);
    assert(res3[0] == "-2 0");
    assert(res3[1] == "-2 1");
    assert(res3[2] == "0 1");

    // Test 4: k = 1
    std::vector<int> input4 = {7, 7, 9};
    auto res4 = generateCombinations(input4, 1);
    assert(res4.size() == 2);
    assert(res4[0] == "7");
    assert(res4[1] == "9");

    // Test 5: k equals number of distinct values
    std::vector<int> input5 = {10, 20, 30};
    auto res5 = generateCombinations(input5, 3);
    assert(res5.size() == 1);
    assert(res5[0] == "10 20 30");

    // Test 6: Invalid k (k > distinct count)
    std::vector<int> input6 = {1, 2};
    auto res6 = generateCombinations(input6, 3);
    assert(res6.empty());

    // Test 7: Single distinct value
    std::vector<int> input7 = {42};
    auto res7 = generateCombinations(input7, 1);
    assert(res7.size() == 1);
    assert(res7[0] == "42");

    // Test 8: Out-of-range values ignored (per spec, but we allow filtering)
    std::vector<int> input8 = {-1000, 1000, 1500, 2000};
    auto res8 = generateCombinations(input8, 2);
    // Only -1000 and 1000 are within [-1000,1000]? Actually 1500 and 2000 are out of range, but our implementation ignores them,
    // but let's define behavior: we ignore out-of-range so only 2 values remain => 1 combination
    assert(res8.size() == 1);
    assert(res8[0] == "-1000 1000");

    return 0;
}
