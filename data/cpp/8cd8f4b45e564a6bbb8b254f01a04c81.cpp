/*
Write a C++ function `countSmaller` that takes a vector of integers and returns a vector where each element at index `i` contains the number of elements to the right of `nums[i]` that are strictly smaller than `nums[i]`. The function must use a divide-and-conquer merge-sort-based approach with a pair structure to track the original indices, handling duplicate values correctly (a smaller equal value counts only if it appears strictly to the right and is strictly less). The input vector may be empty, contain negative numbers, duplicates, and up to 10^5 elements. The solution must be self-contained, avoid global state, and use appropriate `const` correctness where applicable.
*/

#include <vector>
#include <utility>

// Count strictly smaller elements to the right of each position.
// Uses a merge-sort-based approach with index tracking via pairs.
std::vector<int> countSmaller(std::vector<int>& nums) {
    int n = static_cast<int>(nums.size());
    if (n == 0) return {};
    
    std::vector<std::pair<int,int>> pairs;
    pairs.reserve(n);
    for (int i = 0; i < n; ++i) {
        pairs.emplace_back(nums[i], i);
    }
    
    std::vector<int> result(n, 0);
    
    // Recursive merge sort helper
    auto mergeSort = [&](auto&& self, int low, int high) -> void {
        if (low >= high) return;
        int mid = (low + high) / 2;
        self(self, low, mid);
        self(self, mid + 1, high);
        
        // Merge step
        int left = low;
        int right = mid + 1;
        int idx = 0;
        int smallerCount = 0;
        std::vector<std::pair<int,int>> merged(high - low + 1);
        
        while (left <= mid && right <= high) {
            if (pairs[left].first <= pairs[right].first) {
                // Left element is not greater than right, so add the count
                // of right elements already placed (all strictly smaller)
                result[pairs[left].second] += smallerCount;
                merged[idx++] = pairs[left++];
            } else {
                // Right element is strictly smaller than left, increment count
                ++smallerCount;
                merged[idx++] = pairs[right++];
            }
        }
        while (left <= mid) {
            result[pairs[left].second] += smallerCount;
            merged[idx++] = pairs[left++];
        }
        while (right <= high) {
            merged[idx++] = pairs[right++];
        }
        
        for (int i = 0; i < (high - low + 1); ++i) {
            pairs[low + i] = merged[i];
        }
    };
    
    mergeSort(mergeSort, 0, n - 1);
    return result;
}

#include <cassert>
#include <vector>

// Include the solution function here (or link it)
std::vector<int> countSmaller(std::vector<int>& nums);

int main() {
    // Basic cases
    std::vector<int> v1 = {5, 2, 6, 1};
    std::vector<int> r1 = countSmaller(v1);
    assert(r1 == std::vector<int>({2, 1, 1, 0}));

    // Duplicate values
    std::vector<int> v2 = {1, 1, 1};
    std::vector<int> r2 = countSmaller(v2);
    assert(r2 == std::vector<int>({0, 0, 0}));

    // Descending order
    std::vector<int> v3 = {3, 2, 1};
    std::vector<int> r3 = countSmaller(v3);
    assert(r3 == std::vector<int>({2, 1, 0}));

    // Ascending order
    std::vector<int> v4 = {1, 2, 3};
    std::vector<int> r4 = countSmaller(v4);
    assert(r4 == std::vector<int>({0, 0, 0}));

    // Single element
    std::vector<int> v5 = {7};
    std::vector<int> r5 = countSmaller(v5);
    assert(r5 == std::vector<int>({0}));

    // Empty input
    std::vector<int> v6 = {};
    std::vector<int> r6 = countSmaller(v6);
    assert(r6.empty());

    // Negative numbers
    std::vector<int> v7 = {-1, -2, -3, -4};
    std::vector<int> r7 = countSmaller(v7);
    assert(r7 == std::vector<int>({3, 2, 1, 0}));

    // Mixed values with duplicates
    std::vector<int> v8 = {2, 2, 1, 0};
    std::vector<int> r8 = countSmaller(v8);
    assert(r8 == std::vector<int>({2, 2, 1, 0}));

    // Larger test
    std::vector<int> v9 = {10, 9, 8, 7, 6, 5, 4, 3, 2, 1};
    std::vector<int> r9 = countSmaller(v9);
    std::vector<int> expected9 = {9, 8, 7, 6, 5, 4, 3, 2, 1, 0};
    assert(r9 == expected9);

    return 0;
}

// The task is a classic "count smaller numbers after self" problem, solvable efficiently using a modified merge sort. The algorithm works by maintaining an array of pairs `(value, original_index)`. During the merge step, we compare elements from the left and right halves. When we place an element from the left half into the merged array, we know that all elements already moved from the right half are strictly smaller than it (because the right half is sorted and we only move from right to left when the right element is strictly smaller). Therefore, we add the count of moved right elements to the answer at the left element's original index. For duplicate values, we use `<=` when comparing left and right: if left value equals right value, we place the left element first and do not increment the count, ensuring that equal values on the right are not counted as "smaller" (only strictly smaller counts). Edge cases include an empty input (return empty vector) and single-element input (answer is 0). The merge sort recursion runs in O(n log n) time and uses O(n) auxiliary space for the temporary merge array and the solution vector. The function must be carefully implemented to handle the `mid` pointer correctly (it is passed as `mid+1` in the recursive call, so the merge function receives the start index of the right half). Furthermore, the indices must be tracked via pairs to correctly associate each value with its original position after sorting.
