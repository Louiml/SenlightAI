// Write a C++ function that takes a vector of integers and a positive integer k (where k is less than or equal to the vector's size) and returns a vector of integers representing the number of distinct elements in each contiguous subarray of size k. The function should process the input efficiently by using a sliding window technique. For example, given the array `[1, 2, 1, 3, 4]` and `k = 3`, the output should be `[2, 3, 3]` because the first window `[1, 2, 1]` has 2 distinct values, the second window `[2, 1, 3]` has 3 distinct values, and the third window `[1, 3, 4]` has 3 distinct values. The function must handle duplicate values correctly, and the result vector's size should be exactly `n - k + 1` where `n` is the size of the input array. The input vector is not modified.
#include <cassert>
#include <vector>

int main() {
    // Basic example from the description
    std::vector<int> arr1 = {1, 2, 1, 3, 4};
    std::vector<int> res1 = countDistinctInWindows(arr1, 3);
    assert(res1 == std::vector<int>({2, 3, 3}));

    // All distinct elements
    std::vector<int> arr2 = {5, 6, 7, 8};
    std::vector<int> res2 = countDistinctInWindows(arr2, 2);
    assert(res2 == std::vector<int>({2, 2, 2}));

    // Window size equals array size
    std::vector<int> arr3 = {1, 1, 2, 2};
    std::vector<int> res3 = countDistinctInWindows(arr3, 4);
    assert(res3 == std::vector<int>({2}));

    // Window size of 1
    std::vector<int> arr4 = {10, 20, 10};
    std::vector<int> res4 = countDistinctInWindows(arr4, 1);
    assert(res4 == std::vector<int>({1, 1, 1}));

    // All duplicate elements
    std::vector<int> arr5 = {7, 7, 7, 7};
    std::vector<int> res5 = countDistinctInWindows(arr5, 2);
    assert(res5 == std::vector<int>({1, 1, 1}));

    // Negative numbers and zeros
    std::vector<int> arr6 = {-1, 0, -1, 2};
    std::vector<int> res6 = countDistinctInWindows(arr6, 3);
    assert(res6 == std::vector<int>({2, 3}));

    // Large input with many distinct elements (sanity)
    std::vector<int> arr7;
    for (int i = 0; i < 1000; ++i) arr7.push_back(i % 100);
    std::vector<int> res7 = countDistinctInWindows(arr7, 50);
    assert(res7.size() == 1001 - 50 + 1);
    assert(res7.front() == 50);
    assert(res7.back() == 50);

    // Test that input is not modified (const correctness)
    std::vector<int> original = {1, 2, 1};
    std::vector<int> copy = original;
    countDistinctInWindows(original, 2);
    assert(original == copy);

    return 0;
}
#include <vector>
#include <unordered_map>

// Returns the number of distinct integers in every contiguous subarray of size k.
std::vector<int> countDistinctInWindows(const std::vector<int>& arr, int k) {
    std::vector<int> distinctCounts;
    int n = static_cast<int>(arr.size());
    if (n == 0 || k <= 0 || k > n) {
        return distinctCounts; // Empty result for invalid input
    }

    std::unordered_map<int, int> freq;
    
    // Initialize first window of size k
    for (int i = 0; i < k; ++i) {
        ++freq[arr[i]];
    }
    distinctCounts.push_back(static_cast<int>(freq.size()));

    // Slide the window
    for (int right = k; right < n; ++right) {
        ++freq[arr[right]];                // Add new element
        --freq[arr[right - k]];            // Remove outgoing element
        if (freq[arr[right - k]] == 0) {
            freq.erase(arr[right - k]);    // Eliminate zero-frequency entry
        }
        distinctCounts.push_back(static_cast<int>(freq.size()));
    }

    return distinctCounts;
}
// The solution uses a sliding window combined with a frequency hash map to track the number of occurrences of each element within the current window of size k. First, we initialize the map by inserting the frequencies of the first k elements and record the map's size (which equals the number of distinct elements in that first window) as the first result. Then, for each subsequent window starting at index `i` (from k to n-1), we add the new element at position `i` by incrementing its frequency, and remove the element that slides out at position `i - k` by decrementing its frequency. If the frequency of the outgoing element becomes zero, we erase it from the map so that it no longer contributes to the distinct count. The current size of the map is appended to the result vector after each step. This approach updates the distinct count in constant time per step, giving an overall time complexity of O(n), where n is the length of the array, and an auxiliary space complexity of O(k) because the hash map can hold at most k distinct elements. Edge cases include when k equals the array size, where the result will contain a single element; when the array has duplicates, the frequency map correctly counts distinct values; and when k = 1, each element is its own window, so the result is all ones.
