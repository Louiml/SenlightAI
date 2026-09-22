Write a C++ function named `smallestGreaterThanThreshold` that takes a vector of integers `arr` and a vector of queries, where each query is a vector of three integers `[left, right, threshold]`. For each query, the function must return, in the same order as the queries, the smallest element in `arr` from index `left` to index `right` (inclusive) that is strictly greater than `threshold`. If no such element exists, return `-1` for that query. You may assume all indices in the queries are valid (0 ≤ left ≤ right < arr.size()), and the input vectors are non-empty and contain at least one query. The function should not modify the input vectors (use `const` references where appropriate). Your solution must be self-contained and include the necessary headers.
// The core idea is to process each query independently by scanning the subarray defined by `left` and `right`. For each query, initialize a candidate answer to a very large value (e.g., `INT_MAX` from `<climits>` or a large constant like `1000001` if values are known to be smaller, but using `INT_MAX` is safer). Iterate through indices `j` from `left` to `right`. For each element `arr[j]`, check if it is strictly greater than the query's `threshold` and also smaller than the current candidate. If so, update the candidate to `arr[j]`. After finishing the scan, if the candidate was updated (i.e., it is no longer the initial sentinel), push that candidate into the result vector; otherwise, push `-1`. Edge cases: if the threshold is very large, no element satisfies the condition, so the sentinel remains and `-1` is returned. If the threshold is smaller than all elements, the answer is simply the minimum value in that range that is greater than the threshold—this is naturally handled by the scan. Since each query scans a subarray, the time complexity is O(sum of lengths of all query ranges) in the worst case O(Q * N), where Q is number of queries and N is arr size. Auxiliary space is O(1) besides the result vector, which is O(Q).
#include <vector>
#include <climits>

// For each query [left, right, threshold], return the smallest element in
// arr[left..right] (inclusive) that is strictly greater than threshold,
// or -1 if no such element exists.
std::vector<int> smallestGreaterThanThreshold(
    const std::vector<int>& arr,
    const std::vector<std::vector<int>>& queries
) {
    std::vector<int> results;
    results.reserve(queries.size());

    for (const auto& query : queries) {
        int left = query[0];
        int right = query[1];
        int threshold = query[2];

        int best = INT_MAX;
        for (int j = left; j <= right; ++j) {
            if (arr[j] > threshold && arr[j] < best) {
                best = arr[j];
            }
        }

        if (best == INT_MAX) {
            results.push_back(-1);
        } else {
            results.push_back(best);
        }
    }

    return results;
}
#include <cassert>
#include <vector>

// The solution function is assumed to be declared above; this main provides
// runnable assertions to verify correctness.

int main() {
    // Basic case: query range [1,3] with threshold 2, smallest >2 is 3
    std::vector<int> arr1 = {1, 3, 5, 2, 4};
    std::vector<std::vector<int>> q1 = {{1, 3, 2}};
    std::vector<int> res1 = smallestGreaterThanThreshold(arr1, q1);
    assert(res1.size() == 1 && res1[0] == 3);

    // No element greater than threshold in range
    std::vector<int> arr2 = {10, 20, 30};
    std::vector<std::vector<int>> q2 = {{0, 2, 100}};
    std::vector<int> res2 = smallestGreaterThanThreshold(arr2, q2);
    assert(res2.size() == 1 && res2[0] == -1);

    // Single element range, element itself greater than threshold
    std::vector<int> arr3 = {7};
    std::vector<std::vector<int>> q3 = {{0, 0, 5}};
    std::vector<int> res3 = smallestGreaterThanThreshold(arr3, q3);
    assert(res3.size() == 1 && res3[0] == 7);

    // Multiple queries, mixed results
    std::vector<int> arr4 = {5, 1, 8, 3, 9, 2};
    std::vector<std::vector<int>> q4 = {{0, 5, 4}, {2, 4, 7}, {1, 3, 1}};
    std::vector<int> res4 = smallestGreaterThanThreshold(arr4, q4);
    assert(res4.size() == 3);
    assert(res4[0] == 5); // from {5,8,9} min >4 is 5
    assert(res4[1] == 8); // from {8,3,9} min >7 is 8
    assert(res4[2] == 3); // from {1,8,3} min >1 is 3

    // Duplicate values and threshold equal to some values
    std::vector<int> arr5 = {2, 2, 3, 3, 4};
    std::vector<std::vector<int>> q5 = {{0, 4, 2}, {2, 3, 3}};
    std::vector<int> res5 = smallestGreaterThanThreshold(arr5, q5);
    assert(res5.size() == 2);
    assert(res5[0] == 3); // smallest >2 is 3
    assert(res5[1] == -1); // no >3 in indices 2..3 (values 3,3)

    // Negative numbers and large threshold
    std::vector<int> arr6 = {-5, -1, -10, 0, 5};
    std::vector<std::vector<int>> q6 = {{0, 3, -6}, {1, 4, 100}};
    std::vector<int> res6 = smallestGreaterThanThreshold(arr6, q6);
    assert(res6.size() == 2);
    assert(res6[0] == -5); // -5 > -6 and smallest
    assert(res6[1] == -1); // sentinel -> -1 since no >100

    // Entire array, threshold below all values
    std::vector<int> arr7 = {4, 2, 5, 1};
    std::vector<std::vector<int>> q7 = {{0, 3, 0}};
    std::vector<int> res7 = smallestGreaterThanThreshold(arr7, q7);
    assert(res7.size() == 1 && res7[0] == 1);

    // Empty result for a query with empty range is not possible per constraints,
    // but test a range where left == right and threshold equals the value
    std::vector<int> arr8 = {9};
    std::vector<std::vector<int>> q8 = {{0, 0, 9}};
    std::vector<int> res8 = smallestGreaterThanThreshold(arr8, q8);
    assert(res8.size() == 1 && res8[0] == -1);
}
