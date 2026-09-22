Given an integer \( n \) and an array of \( n \) integers \( a_1, a_2, \dots, a_n \), write a C++ function `long long computeSumReduction(const std::vector<int>& arr)` that returns the value of the expression \( 1 + \sum_{i=1}^{n} (a_i - 1) \). In other words, compute the sum of all elements minus 1 for each element, then add 1 to the total. The function must handle arbitrarily large positive integers (up to \( 10^9 \)) and a large \( n \) (up to \( 10^5 \)), so use a 64-bit integer type for the result. Handle the edge case where the array is empty: the function should return 1 (since the sum is 0 and adding 1 gives 1). The input array is non-empty in the original problem, but for robustness, your function should handle empty arrays as well.

The problem reduces to a straightforward linear pass over the array. For each element \( a_i \), we add \( a_i - 1 \) to an accumulator, then after the loop we add 1 to the accumulator and return it. This is mathematically equivalent to \( 1 + (\sum a_i) - n \), but we can just compute directly without overflow because we use `long long`. Important edge cases: an empty array (should return 1, since sum of nothing is 0 and 0 + 1 = 1), arrays with all ones (each contributes 0, final result 1), and arrays with large values (each contribution may be up to \( 10^9 - 1 \), and with \( n = 10^5 \) the total can be up to \( 10^{14} \), which fits in `long long`). Time complexity: \( O(n) \), space complexity: \( O(1) \) additional space.

#include <vector>
#include <cstdint>

// Compute 1 + sum_{i=1}^{n} (arr[i-1] - 1)
// Returns a long long to handle large sums.
long long computeSumReduction(const std::vector<int>& arr) {
    long long result = 1; // Start with the +1 at the end
    for (int value : arr) {
        result += static_cast<long long>(value) - 1;
    }
    return result;
}

#include <cassert>
#include <vector>

// declaration from solution
long long computeSumReduction(const std::vector<int>& arr);

int main() {
    // Basic case from snippet
    std::vector<int> arr1 = {3, 5, 2};
    assert(computeSumReduction(arr1) == (3-1)+(5-1)+(2-1)+1); // 2+4+1+1=8

    // All ones: each contributes 0, result 1
    std::vector<int> arr2 = {1, 1, 1, 1};
    assert(computeSumReduction(arr2) == 1);

    // Single element
    std::vector<int> arr3 = {10};
    assert(computeSumReduction(arr3) == 10); // (10-1)+1 = 10

    // Empty array (robustness)
    std::vector<int> arr4;
    assert(computeSumReduction(arr4) == 1);

    // Large values
    std::vector<int> arr5 = {1000000000, 1000000000, 1000000000};
    long long expected = 1 + 3 * (1000000000LL - 1);
    assert(computeSumReduction(arr5) == expected);

    // Mixed values
    std::vector<int> arr6 = {0, 5, -3, 7};
    long long expected6 = 1 + (0-1)+(5-1)+(-3-1)+(7-1) = 1 -1+4-4+6 = 6;
    assert(computeSumReduction(arr6) == 6);

    return 0;
}
