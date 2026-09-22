/*
Write a C++ function `minimumSumOfK` that takes a vector of integers and an integer `k`, and returns the minimum possible sum of exactly `k` distinct elements chosen from the vector. The input vector may contain duplicate values, but the chosen elements must be at different indices (so you can pick the same numeric value multiple times if it appears multiple times in the vector). If `k` is less than or equal to 0, or `k` is greater than the size of the vector, the function should return the integer `0` (as an error/sentinel value). The function must not modify the input vector.
*/

#include <vector>
#include <algorithm>
#include <cstddef>

// Returns the minimum sum of exactly k chosen elements from the input vector.
// Returns 0 if k is invalid (k <= 0 or k > v.size()).
long long minimumSumOfK(const std::vector<int>& v, int k) {
    if (k <= 0 || static_cast<size_t>(k) > v.size()) {
        return 0LL;
    }
    std::vector<int> sorted = v;  // copy to avoid modifying input
    std::sort(sorted.begin(), sorted.end());
    long long sum = 0;
    for (int i = 0; i < k; ++i) {
        sum += sorted[i];
    }
    return sum;
}

#include <cassert>
#include <vector>

int main() {
    // Basic test with distinct values
    std::vector<int> v1 = {5, 3, 1, 4, 2};
    assert(minimumSumOfK(v1, 3) == 6); // 1+2+3

    // Duplicate values
    std::vector<int> v2 = {4, 4, 4, 4};
    assert(minimumSumOfK(v2, 2) == 8); // 4+4

    // Negative values
    std::vector<int> v3 = {-1, -5, -3, 10};
    assert(minimumSumOfK(v3, 2) == -8); // -5 + -3

    // k = 1
    std::vector<int> v4 = {7, 2, 9};
    assert(minimumSumOfK(v4, 1) == 2);

    // k equals full size
    std::vector<int> v5 = {3, 1, 2};
    assert(minimumSumOfK(v5, 3) == 6);

    // invalid k: zero
    assert(minimumSumOfK(v1, 0) == 0);

    // invalid k: negative
    assert(minimumSumOfK(v1, -2) == 0);

    // invalid k: too large
    assert(minimumSumOfK(v1, 6) == 0);

    // empty vector
    std::vector<int> v6;
    assert(minimumSumOfK(v6, 0) == 0);

    // original vector not modified
    std::vector<int> v7 = {9, 8, 7};
    minimumSumOfK(v7, 2);
    assert(v7[0] == 9 && v7[1] == 8 && v7[2] == 7);
}

// The problem reduces to selecting the `k` smallest elements from the array, because any other selection would produce a larger or equal sum. The straightforward approach is to sort a copy of the vector in ascending order and then sum the first `k` elements. Since the input vector must not be modified, we copy it first. Edge cases: if `k <= 0` or `k > vector.size()`, return `0`. Also, if the vector is empty, `k` must be 0 or less, so return `0`. Sorting takes \(O(n \log n)\) time and the copy takes \(O(n)\) additional space. An alternative heap-based approach could do this in \(O(n \log k)\), but sorting is simpler and sufficient here. The sum can be computed with a long long to avoid overflow, but the problem statement likely expects int; we'll use long long for safety but return as long long. Complexity: \(O(n \log n)\) time, \(O(n)\) auxiliary space for the copy.
