Write a C++ function that sorts an array of non-negative integers using the bucket sort (counting sort) algorithm. The function should take an array and its size as parameters, and modify the array in-place so that the elements are sorted in ascending order. The input is guaranteed to contain only non-negative integers, and the maximum value in the array will be at most 100,000. You do not need to handle negative numbers or invalid input.

#include <cassert>
#include <vector>

int main() {
    // Test 1: Basic sort
    std::vector<int> a = {5, 2, 9, 1, 5, 6};
    bucketSort(a);
    assert(a == (std::vector<int>{1, 2, 5, 5, 6, 9}));

    // Test 2: Already sorted
    std::vector<int> b = {1, 2, 3, 4};
    bucketSort(b);
    assert(b == (std::vector<int>{1, 2, 3, 4}));

    // Test 3: Reverse sorted
    std::vector<int> c = {4, 3, 2, 1};
    bucketSort(c);
    assert(c == (std::vector<int>{1, 2, 3, 4}));

    // Test 4: All equal elements
    std::vector<int> d = {7, 7, 7, 7};
    bucketSort(d);
    assert(d == (std::vector<int>{7, 7, 7, 7}));

    // Test 5: Single element
    std::vector<int> e = {42};
    bucketSort(e);
    assert(e == (std::vector<int>{42}));

    // Test 6: Empty array (should not crash and remain empty)
    std::vector<int> f = {};
    bucketSort(f);
    assert(f.empty());

    // Test 7: Elements including zero
    std::vector<int> g = {0, 10, 0, 3, 2};
    bucketSort(g);
    assert(g == (std::vector<int>{0, 0, 2, 3, 10}));

    // Test 8: Larger array with duplicates
    std::vector<int> h = {100, 0, 55, 55, 33, 0, 100, 7};
    bucketSort(h);
    assert(h == (std::vector<int>{0, 0, 7, 33, 55, 55, 100, 100}));
}

#include <vector>
#include <algorithm>
#include <cstddef>

// Sorts an array of non-negative integers in-place using counting sort (bucket sort).
// The input array must contain only non-negative integers.
void bucketSort(std::vector<int>& arr) {
    if (arr.empty()) {
        return;
    }

    // Find the maximum element to determine bucket size.
    int maxVal = *std::max_element(arr.begin(), arr.end());

    // Create and initialize the count array.
    std::vector<int> count(maxVal + 1, 0);

    // Count occurrences of each element.
    for (int value : arr) {
        if (value < 0) {
            // The algorithm is designed for non-negative integers.
            // If negative values appear, this is an invalid input.
            return;
        }
        count[value]++;
    }

    // Reconstruct the sorted array.
    int index = 0;
    for (int i = 0; i <= maxVal; ++i) {
        while (count[i] > 0) {
            arr[index++] = i;
            count[i]--;
        }
    }
}

// The bucket sort method used here is essentially counting sort, suitable for sorting integers in a known, bounded range. First, scan the array to find its maximum value. Then create a "bucket" (counting) array of size `max+1` and initialize all entries to zero. Next, iterate through the original array and increment the bucket at index equal to each element's value, thereby counting occurrences of each number. Finally, rebuild the original array by iterating from index 0 to `max`, and for each value with a positive count, place that value into the array as many times as its count. Important edge cases include an empty array (which should do nothing), an array with a single element, and an array where all elements are equal. Time complexity is O(n + max), where n is the array size and max is the maximum value; space complexity is O(max) for the bucket array. This approach is efficient when max is not much larger than n.
