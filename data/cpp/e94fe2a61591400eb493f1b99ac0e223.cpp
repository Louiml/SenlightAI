// Write a C++ function named `findTripletWithSum` that takes a `std::vector<int>` (by const reference) and a target integer `target`. The function must return a `std::vector<int>` containing exactly three integers from the input vector whose sum equals `target`. If no such triplet exists, return an empty vector. The input vector may contain duplicate values, negative numbers, and is not guaranteed to be sorted. If multiple triplets satisfy the condition, any one of them is acceptable. The solution must not modify the input vector and must handle edge cases such as vectors with fewer than 3 elements, duplicates, and large negative/positive values without undefined behavior.

The classic approach is to sort the array first, then use a two-pointer technique within a fixed first element. Sorting the input into a local copy allows us to use the two-pointer method: for each index `i` from `0` to `n-3`, set `left = i+1` and `right = n-1`. Compute `current_sum = arr[i] + arr[left] + arr[right]`. If `current_sum == target`, we return the triplet. If `current_sum < target`, increment `left`; if `current_sum > target`, decrement `right`. To avoid counting the same element twice, we fix `i` and never let `left` or `right` cross. Also, after each iteration over `i`, we can skip duplicate values of `arr[i]` to avoid redundant checks (though it’s not strictly required for correctness). Important edge cases: if the vector has fewer than 3 elements, return an empty vector immediately; if after the loop no triplet is found, return an empty vector. Time complexity is O(n log n) due to sorting, plus O(n²) for the nested loop (since for each `i` we scan with two pointers), giving overall O(n²). Space complexity is O(1) auxiliary (excluding the copy for sorting, which is O(n) if we copy the input).

#include <vector>
#include <algorithm>

// Returns any triplet of elements from arr whose sum equals target.
// Returns an empty vector if no such triplet exists or if arr.size() < 3.
std::vector<int> findTripletWithSum(const std::vector<int>& arr, int target) {
    const int n = static_cast<int>(arr.size());
    if (n < 3) {
        return {};
    }

    std::vector<int> sorted = arr;  // local copy for sorting
    std::sort(sorted.begin(), sorted.end());

    for (int i = 0; i < n - 2; ++i) {
        // Skip duplicate first elements to avoid redundant checks
        if (i > 0 && sorted[i] == sorted[i - 1]) {
            continue;
        }

        int left = i + 1;
        int right = n - 1;

        while (left < right) {
            long long current_sum = static_cast<long long>(sorted[i]) +
                                    static_cast<long long>(sorted[left]) +
                                    static_cast<long long>(sorted[right]);
            if (current_sum == target) {
                return {sorted[i], sorted[left], sorted[right]};
            }
            if (current_sum < target) {
                ++left;
            } else {
                --right;
            }
        }
    }

    return {};
}

#include <cassert>
#include <vector>

// The function declaration (provided here for clarity; normally include the header)
std::vector<int> findTripletWithSum(const std::vector<int>& arr, int target);

int main() {
    // Basic test with a clear triplet
    std::vector<int> v1 = {1, 3, 6, 7, 9, 12};
    auto r1 = findTripletWithSum(v1, 22);
    assert(r1.size() == 3);
    assert(r1[0] + r1[1] + r1[2] == 22);

    // No triplet exists
    std::vector<int> v2 = {1, 2, 3, 4, 5};
    assert(findTripletWithSum(v2, 100).empty());

    // Vector with fewer than 3 elements
    std::vector<int> v3 = {1, 2};
    assert(findTripletWithSum(v3, 3).empty());

    // Negative numbers and duplicates
    std::vector<int> v4 = {-5, -5, -1, 0, 2, 4};
    auto r4 = findTripletWithSum(v4, -6);
    assert(r4.size() == 3);
    assert(r4[0] + r4[1] + r4[2] == -6);

    // Triplet at the very beginning after sorting
    std::vector<int> v5 = {0, -1, 2, -3, 1};
    auto r5 = findTripletWithSum(v5, -2);
    assert(r5.size() == 3);
    assert(r5[0] + r5[1] + r5[2] == -2);

    // Large negative target with only negative numbers
    std::vector<int> v6 = {-10, -20, -30, -40};
    auto r6 = findTripletWithSum(v6, -60);
    assert(r6.size() == 3);
    assert(r6[0] + r6[1] + r6[2] == -60);

    // All same numbers, target equals three times the value
    std::vector<int> v7 = {5, 5, 5, 5};
    auto r7 = findTripletWithSum(v7, 15);
    assert(r7.size() == 3);
    assert(r7[0] + r7[1] + r7[2] == 15);

    // Target exactly matches a triplet that includes the largest element
    std::vector<int> v8 = {1, 2, 3, 4, 5};
    auto r8 = findTripletWithSum(v8, 12);
    assert(r8.size() == 3);
    assert(r8[0] + r8[1] + r8[2] == 12);

    return 0;
}
