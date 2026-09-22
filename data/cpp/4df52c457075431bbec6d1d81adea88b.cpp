/*
Given an array of N integers (2 ≤ N ≤ 50,000) where each element is in the range [-10^9, 10^9], your task is to write a C++ function `transformArray` that takes a `std::vector<int>` as input and returns a `std::vector<std::pair<int,int>>` representing a sequence of operations `(i, j)` (0-indexed). Each operation means: set `A[i] = A[i] + A[j]` (i.e., add the value at index j to the value at index i). The goal is to produce a sequence of at most 2N operations such that after executing all operations in order, the final array is **non-decreasing** (sorted in non-decreasing order). The function must always find a valid solution for any input. If multiple valid solutions exist, any correct one is acceptable. The returned vector contains pairs where each pair is `(source_index, target_index)` meaning `A[source_index] += A[target_index]`, and indices are 0-based. Note: The original snippet uses 1-based output, but here we require 0-based in the return value.
*/
#include <vector>
#include <utility>
#include <algorithm>
#include <cmath>

// Returns a sequence of operations (i, j) meaning A[i] += A[j] (0-indexed).
// After applying all operations in order, the array becomes non-decreasing.
// Always produces exactly 2*N operations.
std::vector<std::pair<int,int>> transformArray(const std::vector<int>& A) {
    const int N = static_cast<int>(A.size());
    std::vector<int> arr = A; // local copy to track current values if needed, but not strictly necessary for generating ops

    // Find min and max values and their first occurrence indices.
    int min_val = *std::min_element(arr.begin(), arr.end());
    int max_val = *std::max_element(arr.begin(), arr.end());
    int min_idx = -1, max_idx = -1;
    for (int i = 0; i < N; ++i) {
        if (min_idx == -1 && arr[i] == min_val) min_idx = i;
        if (max_idx == -1 && arr[i] == max_val) max_idx = i;
    }

    std::vector<std::pair<int,int>> result;
    result.reserve(2 * N);

    if (std::abs(min_val) <= std::abs(max_val)) {
        // Use the maximum value (non-negative) to make everything non-negative and non-decreasing.
        int seed_idx = max_idx;
        for (int i = 0; i < N; ++i) {
            result.emplace_back(i, seed_idx);
            result.emplace_back(i, seed_idx);
            seed_idx = i; // now the seed is the amplified value at index i
        }
    } else {
        // Use the minimum value (non-positive) to make everything non-positive, then process in reverse.
        int seed_idx = min_idx;
        for (int i = N - 1; i >= 0; --i) {
            result.emplace_back(i, seed_idx);
            result.emplace_back(i, seed_idx);
            seed_idx = i;
        }
    }

    return result;
}
#include <cassert>
#include <vector>
#include <utility>
#include <algorithm>
#include <iostream>

// Include the solution function here (or link it).
// For completeness, we'll copy the solution inline.
std::vector<std::pair<int,int>> transformArray(const std::vector<int>& A) {
    const int N = static_cast<int>(A.size());
    std::vector<int> arr = A;
    int min_val = *std::min_element(arr.begin(), arr.end());
    int max_val = *std::max_element(arr.begin(), arr.end());
    int min_idx = -1, max_idx = -1;
    for (int i = 0; i < N; ++i) {
        if (min_idx == -1 && arr[i] == min_val) min_idx = i;
        if (max_idx == -1 && arr[i] == max_val) max_idx = i;
    }
    std::vector<std::pair<int,int>> result;
    result.reserve(2 * N);
    if (std::abs(min_val) <= std::abs(max_val)) {
        int seed_idx = max_idx;
        for (int i = 0; i < N; ++i) {
            result.emplace_back(i, seed_idx);
            result.emplace_back(i, seed_idx);
            seed_idx = i;
        }
    } else {
        int seed_idx = min_idx;
        for (int i = N - 1; i >= 0; --i) {
            result.emplace_back(i, seed_idx);
            result.emplace_back(i, seed_idx);
            seed_idx = i;
        }
    }
    return result;
}

// Helper: apply operations and check if result is non-decreasing.
bool check(const std::vector<int>& input, const std::vector<std::pair<int,int>>& ops) {
    std::vector<int> arr = input;
    for (const auto& op : ops) {
        arr[op.first] += arr[op.second];
    }
    for (size_t i = 1; i < arr.size(); ++i) {
        if (arr[i] < arr[i-1]) return false;
    }
    return true;
}

int main() {
    // Test 1: simple positive array
    std::vector<int> a1 = {3, 1, 2};
    auto ops1 = transformArray(a1);
    assert(ops1.size() == 2 * a1.size());
    assert(check(a1, ops1));

    // Test 2: negative values with larger absolute max
    std::vector<int> a2 = {-5, 2, -1};
    auto ops2 = transformArray(a2);
    assert(ops2.size() == 2 * a2.size());
    assert(check(a2, ops2));

    // Test 3: all negative, min has larger absolute value
    std::vector<int> a3 = {-10, -3, -7};
    auto ops3 = transformArray(a3);
    assert(ops3.size() == 2 * a3.size());
    assert(check(a3, ops3));

    // Test 4: mixed with zero
    std::vector<int> a4 = {0, 5, -2};
    auto ops4 = transformArray(a4);
    assert(ops4.size() == 2 * a4.size());
    assert(check(a4, ops4));

    // Test 5: all equal
    std::vector<int> a5 = {7, 7, 7, 7};
    auto ops5 = transformArray(a5);
    assert(ops5.size() == 2 * a5.size());
    assert(check(a5, ops5));

    // Test 6: large negative and large positive
    std::vector<int> a6 = {1000000000, -1000000000};
    auto ops6 = transformArray(a6);
    assert(ops6.size() == 2 * a6.size());
    assert(check(a6, ops6));

    // Test 7: random-ish small
    std::vector<int> a7 = {-1, 2, -3, 4, -5};
    auto ops7 = transformArray(a7);
    assert(ops7.size() == 2 * a7.size());
    assert(check(a7, ops7));

    // Test 8: single pair both positive
    std::vector<int> a8 = {10, 1};
    auto ops8 = transformArray(a8);
    assert(ops8.size() == 2 * a8.size());
    assert(check(a8, ops8));

    // Test 9: all negative equal
    std::vector<int> a9 = {-4, -4, -4};
    auto ops9 = transformArray(a9);
    assert(ops9.size() == 2 * a9.size());
    assert(check(a9, ops9));

    // Test 10: N=2 with min having bigger abs
    std::vector<int> a10 = {-100, 50};
    auto ops10 = transformArray(a10);
    assert(ops10.size() == 2 * a10.size());
    assert(check(a10, ops10));

    std::cout << "All tests passed!" << std::endl;
    return 0;
}
// The approach uses an "amplification" strategy. First, find the element with the maximum absolute value among all elements. If the absolute value of the maximum element is at least as large as the absolute value of the minimum element, then the maximum element (call it `M`) is non-negative (since abs(max) >= abs(min) implies max ≥ 0). We can repeatedly add `M` to every other element (including itself) to make all elements non-negative. Specifically, we choose the index of `M`, then for i from 0 to N-1: we perform `A[i] += A[max_idx]` twice (so that each element at index i becomes `A[i] + 2*M`), and then we update `max_idx = i` so that the next additions use the newly amplified value (which is even larger). Each operation doubles the "seed" value, so after N iterations, all elements become non-negative, and because we always add a non-negative number, the values grow. Then we can perform a bubble-sort-like set of additions: for i from 0 to N-2, we add the (previously fixed) element at index i to index i+1 repeatedly until `A[i+1] >= A[i]`. But since we have the amplification trick, it's easier: after making all elements non-negative, we can just add the maximum (which is now at the last index) to all earlier elements in reverse order to achieve sorted order. Actually, the standard solution is: pick the element with max absolute value. If it's the max (non-negative), then for i=0..N-1, do two operations `(i, max_idx)` and set `max_idx=i`. This makes the array non-decreasing because each new value is the previous value plus something that is at least the previous maximum, so it grows. Similarly, if the min has larger absolute value (and thus is non-positive), we do the same but iterate from N-1 down to 0 (so we add a large negative number to make all elements non-positive, then they are non-increasing but we need non-decreasing, so we reverse the order of indices). The key invariant: after processing index i, all elements from 0 to i (or i to N-1) are already in the correct relative order and the "seed" value becomes the current element at the processed index, which is guaranteed to be at least as large (or as small) as all previously processed elements. Time complexity is O(N) for finding min/max and O(N) for generating the 2N operations (exactly 2N operations, each pair is simply pushed). Space complexity is O(N) for the result vector. Edge cases: N=2 trivial, all equal values, negative and positive mixed. The solution always uses exactly 2N operations, which is within the allowed limit of 2N.
