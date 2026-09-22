// Given a vector of non-negative integers `arr` and a vector of queries, where each query is a pair `[L, R]` with `0 <= L <= R < arr.size()`, write a C++ function `xorQueries` that returns a vector of integers containing the bitwise XOR of the subarray `arr[L..R]` for each query. The function should compute answers efficiently without recalculating the XOR from scratch for each query. The input arrays are read-only, and the function must be `const`-correct, taking the arrays by `const` reference. Assume `arr` is never empty and each query is valid.
// The brute-force approach would XOR each subarray element-by-element, leading to \(O(n \cdot m)\) time where \(n\) is the array size and \(m\) is the number of queries. Instead, we use a prefix XOR array: define `prefix[i]` as the XOR of `arr[0]` through `arr[i]`. Then for any query `[L, R]`, the XOR of `arr[L..R]` equals `prefix[R] ^ (L == 0 ? 0 : prefix[L-1])`, because XOR cancels out the prefix elements before `L`. This works because XOR is its own inverse (`x ^ x = 0`). Build the prefix array in a single pass, then answer each query in constant time, giving total \(O(n + m)\) time and \(O(n)\) auxiliary space for the prefix array, plus the output vector of size \(m\). Edge cases: `L = 0` requires handling by XORing with 0 (identity), and a single-element array works fine. No overflow concerns because XOR operates on integer bit patterns.
#include <vector>

// Return the XOR of subarray arr[L..R] for each query [L, R].
std::vector<int> xorQueries(const std::vector<int>& arr, const std::vector<std::vector<int>>& queries) {
    int n = arr.size();
    std::vector<int> prefix(n);
    prefix[0] = arr[0];
    for (int i = 1; i < n; ++i) {
        prefix[i] = prefix[i - 1] ^ arr[i];
    }

    std::vector<int> result;
    result.reserve(queries.size());
    for (const auto& q : queries) {
        int left = q[0];
        int right = q[1];
        int left_prefix = (left == 0) ? 0 : prefix[left - 1];
        result.push_back(prefix[right] ^ left_prefix);
    }
    return result;
}
#include <cassert>
#include <vector>

int main() {
    // Test case 1: basic queries
    std::vector<int> arr1 = {1, 3, 4, 8};
    std::vector<std::vector<int>> queries1 = {{0, 1}, {1, 2}, {0, 3}, {3, 3}};
    std::vector<int> expected1 = {2, 7, 14, 8};
    assert(xorQueries(arr1, queries1) == expected1);

    // Test case 2: single element array
    std::vector<int> arr2 = {5};
    std::vector<std::vector<int>> queries2 = {{0, 0}};
    std::vector<int> expected2 = {5};
    assert(xorQueries(arr2, queries2) == expected2);

    // Test case 3: all zeros
    std::vector<int> arr3 = {0, 0, 0, 0};
    std::vector<std::vector<int>> queries3 = {{0, 3}, {1, 2}, {2, 2}};
    std::vector<int> expected3 = {0, 0, 0};
    assert(xorQueries(arr3, queries3) == expected3);

    // Test case 4: larger numbers and varying lengths
    std::vector<int> arr4 = {8, 16, 32, 64, 128};
    std::vector<std::vector<int>> queries4 = {{0, 0}, {1, 3}, {0, 4}, {2, 2}};
    std::vector<int> expected4 = {8, 112, 248, 32};
    assert(xorQueries(arr4, queries4) == expected4);

    // Test case 5: adjacent queries sharing prefix
    std::vector<int> arr5 = {2, 4, 6, 8, 10};
    std::vector<std::vector<int>> queries5 = {{1, 3}, {0, 2}, {3, 4}};
    std::vector<int> expected5 = {4 ^ 6 ^ 8, 2 ^ 4 ^ 6, 8 ^ 10};
    assert(xorQueries(arr5, queries5) == expected5);

    return 0;
}
