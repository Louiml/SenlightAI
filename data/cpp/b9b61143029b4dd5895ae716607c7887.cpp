// Write a C++ function named `findNearestK` that takes a sorted array of integers, its size, a target integer, and a positive integer `k`, and returns a `std::vector<int>` containing the `k` closest values to the target from the array. The result must be sorted in ascending order. You may assume the array is already sorted in non-decreasing order and that `k` is at least 1 and no greater than the array size. If two values are equally close to the target, the smaller value must be chosen first. If the target lies between two array elements, preference is given to the closer one regardless of whether it's to the left or right; if the target is exactly between two elements (i.e., the distances are identical), choose the smaller element. The function must handle cases where the target is smaller than all elements or larger than all elements. Do not modify the input array. Example: for `arr = {12, 16, 22, 30, 35, 36, 37, 43, 49, 50, 53}`, `size=11`, `target=43`, `k=4`, the function returns `{36, 37, 43, 49}`. Note: the original snippet's logic is buggy and incomplete; your implementation must correctly produce exactly `k` elements.
// The key is to first locate the position of the target within the sorted array using binary search to reduce search space. The target may not be present; we need the closest element's index as a starting anchor. Then we expand outward from that anchor using a two-pointer or sliding window technique: maintain a window of indices `[low, high]` that initially contains the closest element (the anchor) only. Expand by comparing the distances of the next left candidate (`arr[low-1]`) and next right candidate (`arr[high+1]`) to the target; choose the one with smaller distance, and if distances are equal, choose the smaller value (i.e., the left candidate). Continue expanding until the window has exactly `k` elements. Edge cases: if the target is less than the first element, the anchor is index 0; if greater than the last element, anchor is size-1. The binary search should return the index of the closest element, not necessarily an exact match. After expansion, collect the elements from `low+1` to `high-1` (inclusive of the chosen elements) and return them as a sorted vector. Time complexity is O(log n + k) for the binary search and expansion, and space complexity is O(k) for the result vector.
#include <vector>
#include <cstdlib>

// Return k closest integers to target from the sorted array arr of size n.
std::vector<int> findNearestK(const int arr[], int n, int target, int k) {
    // Handle empty array (though n>=k>=1 by specification)
    if (n <= 0) return {};
    if (k <= 0) return {};

    // Binary search for the closest element index (or exact match)
    int left = 0, right = n - 1;
    int closest = 0;
    while (left <= right) {
        int mid = left + (right - left) / 2;
        if (arr[mid] == target) {
            closest = mid;
            break;
        } else if (arr[mid] < target) {
            closest = mid; // candidate
            left = mid + 1;
        } else {
            closest = mid; // candidate
            right = mid - 1;
        }
    }
    // After loop, closest may be last checked mid; but ensure it's truly the closest:
    // We can refine by checking neighbors
    // Find actual closest index among left, right, and closest
    int candidate = closest;
    if (left < n && (std::abs(arr[left] - target) < std::abs(arr[candidate] - target) ||
        (std::abs(arr[left] - target) == std::abs(arr[candidate] - target) && arr[left] < arr[candidate]))) {
        candidate = left;
    }
    if (right >= 0 && (std::abs(arr[right] - target) < std::abs(arr[candidate] - target) ||
        (std::abs(arr[right] - target) == std::abs(arr[candidate] - target) && arr[right] < arr[candidate]))) {
        candidate = right;
    }

    // Expand window around candidate until we have k elements
    int low = candidate - 1;
    int high = candidate + 1;
    int count = 1;
    while (count < k) {
        bool chooseLeft = false;
        if (low < 0) {
            // Only right available
            chooseLeft = false;
        } else if (high >= n) {
            // Only left available
            chooseLeft = true;
        } else {
            int distL = std::abs(arr[low] - target);
            int distR = std::abs(arr[high] - target);
            if (distL < distR) {
                chooseLeft = true;
            } else if (distL > distR) {
                chooseLeft = false;
            } else {
                // equal distance, choose smaller value (left)
                chooseLeft = (arr[low] < arr[high]);
            }
        }
        if (chooseLeft) {
            low--;
        } else {
            high++;
        }
        count++;
    }

    // Build result: elements from low+1 to high-1 inclusive
    std::vector<int> result;
    for (int i = low + 1; i < high; ++i) {
        result.push_back(arr[i]);
    }
    return result;
}
#include <cassert>
#include <vector>
#include <iostream>

// Solution declaration (inline for brevity)
std::vector<int> findNearestK(const int arr[], int n, int target, int k);

int main() {
    // Example from prompt
    int arr1[] = {12, 16, 22, 30, 35, 36, 37, 43, 49, 50, 53};
    int n1 = 11;
    std::vector<int> res1 = findNearestK(arr1, n1, 43, 4);
    assert(res1 == std::vector<int>({36, 37, 43, 49}));

    // Target smaller than all
    int arr2[] = {1, 2, 3, 4, 5};
    std::vector<int> res2 = findNearestK(arr2, 5, 0, 3);
    assert(res2 == std::vector<int>({1, 2, 3}));

    // Target larger than all
    int arr3[] = {10, 20, 30};
    std::vector<int> res3 = findNearestK(arr3, 3, 100, 2);
    assert(res3 == std::vector<int>({20, 30}));

    // Target exactly in middle, equal distances
    int arr4[] = {1, 3, 5, 7};
    std::vector<int> res4 = findNearestK(arr4, 4, 4, 3);
    // distances: 1->3, 3->1, 5->1, 7->3 => pick 3,5,1 => sorted {1,3,5}
    assert(res4 == std::vector<int>({1, 3, 5}));

    // Duplicate values
    int arr5[] = {2, 2, 2, 2};
    std::vector<int> res5 = findNearestK(arr5, 4, 2, 2);
    assert(res5 == std::vector<int>({2, 2}));

    // k = 1
    int arr6[] = {5, 10, 15};
    std::vector<int> res6 = findNearestK(arr6, 3, 12, 1);
    assert(res6 == std::vector<int>({10}));

    // k = n
    int arr7[] = {1, 2, 3};
    std::vector<int> res7 = findNearestK(arr7, 3, 0, 3);
    assert(res7 == std::vector<int>({1, 2, 3}));

    // Larger array with target not present
    int arr8[] = {1, 4, 6, 8, 9, 10, 12};
    std::vector<int> res8 = findNearestK(arr8, 7, 7, 4);
    // distances: 4->3, 6->1, 8->1, 9->2, 10->3 => pick 6,8,9,4 => sorted {4,6,8,9}
    assert(res8 == std::vector<int>({4, 6, 8, 9}));

    // All distinct and equal distance tie
    int arr9[] = {1, 5, 9};
    std::vector<int> res9 = findNearestK(arr9, 3, 5, 2);
    assert(res9 == std::vector<int>({1, 5})); // distances 4 and 4, choose smaller 1

    // Negative numbers
    int arr10[] = {-10, -5, 0, 5, 10};
    std::vector<int> res10 = findNearestK(arr10, 5, -3, 3);
    // distances: -5->2, 0->3, -10->7, 5->8 => pick -5,0,-10 => sorted {-10,-5,0}
    assert(res10 == std::vector<int>({-10, -5, 0}));

    std::cout << "All assertions passed!" << std::endl;
    return 0;
}
