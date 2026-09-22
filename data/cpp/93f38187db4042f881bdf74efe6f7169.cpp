// Write a C++ function that sorts an array of integers, where every element is guaranteed to be either 0, 1, or 2, into ascending order in-place. The function should take a pointer to the first element of the array and its size as parameters. It must run in O(n) time and use O(1) extra space (constant auxiliary memory, not counting the input array itself). The function should not return anything; it modifies the array directly. Edge cases include an array of length 1, an array already sorted, an array with only one distinct value, and an array with all three values in random order.
// The simplest efficient approach is the "Dutch National Flag" algorithm, but an even more straightforward counting sort works perfectly here because there are only three possible values. The main idea is to traverse the array once, counting how many times 0, 1, and 2 appear. Then, rewrite the array by placing all 0s first, then all 1s, then all 2s. This avoids any complex swapping logic and guarantees linear time. The edge cases are naturally handled: for a single-element array, the counters will be 1 for that value and 0 for the others, so the rewrite step just writes that one value back. For an array already sorted or with only one distinct value, the counting and rewrite still work correctly. The time complexity is O(n) for the counting pass plus O(n) for the rewrite pass (since each counter decrements at most n times total), so overall O(n). The auxiliary space is O(1) because we only use three integer counters and an index variable.
#include <cstddef>

// Sort an array of 0s, 1s, and 2s in ascending order in-place.
// The array is modified directly; no return value.
void sort012(int a[], size_t n) {
    int zeroCount = 0;
    int oneCount = 0;
    int twoCount = 0;

    // First pass: count occurrences of each value
    for (size_t i = 0; i < n; ++i) {
        switch (a[i]) {
            case 0: ++zeroCount; break;
            case 1: ++oneCount; break;
            case 2: ++twoCount; break;
        }
    }

    // Second pass: rewrite the array in sorted order
    size_t index = 0;
    while (zeroCount-- > 0) {
        a[index++] = 0;
    }
    while (oneCount-- > 0) {
        a[index++] = 1;
    }
    while (twoCount-- > 0) {
        a[index++] = 2;
    }
}
#include <cassert>
#include <vector>

// Global main function for testing the solution
int main() {
    // Test 1: Typical mixed array
    {
        int arr[] = {0, 2, 1, 2, 0};
        int n = sizeof(arr) / sizeof(arr[0]);
        sort012(arr, n);
        std::vector<int> expected = {0, 0, 1, 2, 2};
        std::vector<int> actual(arr, arr + n);
        assert(actual == expected);
    }

    // Test 2: Single element
    {
        int arr[] = {1};
        int n = sizeof(arr) / sizeof(arr[0]);
        sort012(arr, n);
        std::vector<int> expected = {1};
        std::vector<int> actual(arr, arr + n);
        assert(actual == expected);
    }

    // Test 3: Already sorted
    {
        int arr[] = {0, 0, 1, 1, 2, 2};
        int n = sizeof(arr) / sizeof(arr[0]);
        sort012(arr, n);
        std::vector<int> expected = {0, 0, 1, 1, 2, 2};
        std::vector<int> actual(arr, arr + n);
        assert(actual == expected);
    }

    // Test 4: All zeros
    {
        int arr[] = {0, 0, 0};
        int n = sizeof(arr) / sizeof(arr[0]);
        sort012(arr, n);
        std::vector<int> expected = {0, 0, 0};
        std::vector<int> actual(arr, arr + n);
        assert(actual == expected);
    }

    // Test 5: All twos
    {
        int arr[] = {2, 2, 2, 2};
        int n = sizeof(arr) / sizeof(arr[0]);
        sort012(arr, n);
        std::vector<int> expected = {2, 2, 2, 2};
        std::vector<int> actual(arr, arr + n);
        assert(actual == expected);
    }

    // Test 6: Reverse order
    {
        int arr[] = {2, 1, 0, 2, 1, 0};
        int n = sizeof(arr) / sizeof(arr[0]);
        sort012(arr, n);
        std::vector<int> expected = {0, 0, 1, 1, 2, 2};
        std::vector<int> actual(arr, arr + n);
        assert(actual == expected);
    }

    // Test 7: Larger array with duplicates
    {
        int arr[] = {1, 0, 2, 1, 0, 2, 0, 1, 2, 0};
        int n = sizeof(arr) / sizeof(arr[0]);
        sort012(arr, n);
        std::vector<int> expected = {0, 0, 0, 0, 1, 1, 1, 2, 2, 2};
        std::vector<int> actual(arr, arr + n);
        assert(actual == expected);
    }

    return 0;
}
