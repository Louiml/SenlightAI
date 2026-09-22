/*
Write a C++ function that takes a vector of integers `arr`, a positive integer `k`, and an integer `threshold`, and returns the number of contiguous subarrays of length exactly `k` whose average value is greater than or equal to `threshold`. The function must use a sliding window technique (not brute-force) and must handle cases where `k` is larger than the array size (in which case the answer is 0), negative numbers, and zero-length edge cases. The function should be named `countSubarraysWithAvgAtLeast` and accept the vector by const reference to avoid copying.
*/
#include <vector>

// Count contiguous subarrays of length exactly k whose average >= threshold.
// Uses efficient sliding window technique. Returns 0 if k is non-positive or k > arr.size().
int countSubarraysWithAvgAtLeast(const std::vector<int>& arr, int k, int threshold) {
    if (k <= 0 || arr.size() < static_cast<size_t>(k)) {
        return 0;
    }
    
    int count = 0;
    int window_sum = 0;
    int left = 0;
    
    for (int right = 0; right < static_cast<int>(arr.size()); ++right) {
        window_sum += arr[right];
        
        // Shrink window from left if it exceeds size k
        if (right - left + 1 > k) {
            window_sum -= arr[left];
            ++left;
        }
        
        // When window is exactly size k, check the average condition
        // Compare sum >= threshold * k to avoid integer truncation issues
        if (right - left + 1 == k) {
            if (window_sum >= threshold * k) {
                ++count;
            }
        }
    }
    
    return count;
}
#include <cassert>
#include <vector>

int countSubarraysWithAvgAtLeast(const std::vector<int>& arr, int k, int threshold);

int main() {
    // Provided example: arr = {11,13,17,23,29,31,7,5,2,3}, k=3, threshold=5
    // Subarrays of length 3:
    // [11,13,17] avg=13.67 >=5 -> yes
    // [13,17,23] avg=17.67 >=5 -> yes
    // [17,23,29] avg=23 >=5 -> yes
    // [23,29,31] avg=27.67 >=5 -> yes
    // [29,31,7] avg=22.33 >=5 -> yes
    // [31,7,5] avg=14.33 >=5 -> yes
    // [7,5,2] avg=4.67 <5 -> no
    // [5,2,3] avg=3.33 <5 -> no
    // Total: 6
    std::vector<int> arr1 = {11,13,17,23,29,31,7,5,2,3};
    assert(countSubarraysWithAvgAtLeast(arr1, 3, 5) == 6);

    // Single element, k=1, threshold=5
    std::vector<int> arr2 = {10};
    assert(countSubarraysWithAvgAtLeast(arr2, 1, 5) == 1);

    // Single element below threshold
    std::vector<int> arr3 = {4};
    assert(countSubarraysWithAvgAtLeast(arr3, 1, 5) == 0);

    // k larger than array size -> 0
    std::vector<int> arr4 = {1,2,3};
    assert(countSubarraysWithAvgAtLeast(arr4, 5, 1) == 0);

    // Negative numbers and threshold
    std::vector<int> arr5 = {-3, -1, 2, 4};
    // k=2, threshold=0: subarrays: [-3,-1] avg=-2 <0; [-1,2] avg=0.5 >=0; [2,4] avg=3 >=0 -> 2
    assert(countSubarraysWithAvgAtLeast(arr5, 2, 0) == 2);

    // All equal, k=2, threshold=5
    std::vector<int> arr6 = {5,5,5,5};
    // subarrays of length 2: all avg=5 -> 3
    assert(countSubarraysWithAvgAtLeast(arr6, 2, 5) == 3);

    // k=1, threshold very low (negative) -> all qualify
    std::vector<int> arr7 = {-5, 10, 3};
    assert(countSubarraysWithAvgAtLeast(arr7, 1, -100) == 3);

    // Empty vector
    std::vector<int> arr8;
    assert(countSubarraysWithAvgAtLeast(arr8, 2, 1) == 0);
}
// The optimal solution uses a fixed-size sliding window. We maintain two indices `left` and `right` representing the current window `[left, right]`. We expand `right` from 0 to `n-1`, adding `arr[right]` to a running `sum`. Whenever the window size exceeds `k`, we remove `arr[left]` from the sum and increment `left` to keep the window exactly size `k`. When the window size equals `k`, we check if `sum / k >= threshold`. Because integer division truncates, it is safer to compare `sum >= threshold * k` to avoid precision issues with negative numbers and truncation. For example, if `sum = 14`, `k = 3`, `threshold = 5`, `sum / 3 = 4` (integer) but `5*3=15`, `14 >= 15` is false; however, mathematically 14/3 ≈ 4.67 < 5, so the multiplication check correctly yields false. Conversely, if `sum = 15`, `sum / 3 = 5` (≥5) and `15 >= 15` true. Edge cases: if `k <= 0` (but problem states positive, still handle defensively), if `k > n`, the loop never reaches window size `k`, so count remains 0. If `threshold` is negative, the condition often trivially true. Time complexity is O(n) because each element is added and removed at most once. Space complexity is O(1) extra (excluding input vector itself).
