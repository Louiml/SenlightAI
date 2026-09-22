Write a C++ function that, given an array of integers and its size, inserts into the array—at the position immediately after every occurrence of a specified key value—a new given value. The array may contain duplicate keys; every occurrence must be followed by the new value. The function should return the final size of the modified array (after all insertions). The input array is provided as a raw pointer to a dynamically allocated block with enough capacity to accommodate the insertions, and the function must achieve the insertions in-place without using any auxiliary container. Assume that the key value is guaranteed to appear at least once, and the array always has sufficient capacity for the worst-case total number of insertions.

// The core challenge is to perform all insertions in-place without shifting elements backwards incorrectly. A straightforward left-to-right modification would overwrite existing elements as we insert, corrupting subsequent data. The correct approach is to:
// 1. First, count the number of occurrences of the key in the original array.
// 2. Starting from the end of the original array and moving backwards, copy each element to its final position (which is shifted right by the number of insertions already made after it), and when we encounter the key, also place the new value immediately after the key in its final position.
// 3. The final size is original size plus the number of occurrences of the key. Time complexity is O(n) because we traverse the array twice (once to count, once to shift/insert). Space complexity is O(1) auxiliary, since we only use a few integer variables. Edge cases: the key may appear multiple times consecutively—the algorithm naturally handles this because each occurrence creates a new gap. The key may be at the very end; the final shifted position is still placed correctly. The array must have enough size; we assume capacity is provided.

#include <cstddef>

// Insert `newValue` immediately after every occurrence of `key` in the array `arr`.
// `originalSize` is the number of elements currently in the array.
// `capacity` is the maximum number of elements the array can hold (must be >= originalSize + occurrences).
// Returns the new size after all insertions.
std::size_t insertAfterEveryKey(int* arr, std::size_t originalSize, std::size_t capacity, int key, int newValue) {
    // Count occurrences of key
    std::size_t occurrences = 0;
    for (std::size_t i = 0; i < originalSize; ++i) {
        if (arr[i] == key) {
            ++occurrences;
        }
    }
    
    // If no key found (though problem guarantees at least one), return original size
    if (occurrences == 0) {
        return originalSize;
    }
    
    // New size after all insertions
    std::size_t newSize = originalSize + occurrences;
    // Ensure capacity is sufficient
    if (newSize > capacity) {
        // In a real program we might throw or handle; here we simply return original size
        // since the specification says capacity is sufficient.
        return originalSize;
    }
    
    // Work from the end of the original array backwards.
    std::size_t writeIndex = newSize - 1; // last position in final array
    // Iterate from last original element down to first
    for (std::size_t readIndex = originalSize; readIndex-- > 0; ) {
        int current = arr[readIndex];
        // Place the element at its final position
        arr[writeIndex] = current;
        --writeIndex;
        // If this element is the key, also insert the new value after it
        // (i.e., since we are moving backwards, the new value goes before the current element in the final array)
        if (current == key) {
            arr[writeIndex] = newValue;
            --writeIndex;
        }
    }
    
    return newSize;
}

#include <cassert>

int main() {
    // Test 1: Single key in middle
    int arr1[10] = {1, 2, 3, 4};
    std::size_t n1 = insertAfterEveryKey(arr1, 4, 10, 2, 99);
    assert(n1 == 5);
    assert(arr1[0] == 1 && arr1[1] == 2 && arr1[2] == 99 && arr1[3] == 3 && arr1[4] == 4);

    // Test 2: Multiple keys, including consecutive
    int arr2[10] = {5, 5, 6, 5};
    std::size_t n2 = insertAfterEveryKey(arr2, 4, 10, 5, 0);
    assert(n2 == 7);
    assert(arr2[0] == 5 && arr2[1] == 0 && arr2[2] == 5 && arr2[3] == 0 && arr2[4] == 6 && arr2[5] == 5 && arr2[6] == 0);

    // Test 3: Key at beginning and end
    int arr3[10] = {7, 8, 7};
    std::size_t n3 = insertAfterEveryKey(arr3, 3, 10, 7, -1);
    assert(n3 == 5);
    assert(arr3[0] == 7 && arr3[1] == -1 && arr3[2] == 8 && arr3[3] == 7 && arr3[4] == -1);

    // Test 4: All elements are key
    int arr4[10] = {3, 3};
    std::size_t n4 = insertAfterEveryKey(arr4, 2, 10, 3, 42);
    assert(n4 == 4);
    assert(arr4[0] == 3 && arr4[1] == 42 && arr4[2] == 3 && arr4[3] == 42);

    // Test 5: Key appears once but is at the very end
    int arr5[10] = {10, 20, 30, 40, 5};
    std::size_t n5 = insertAfterEveryKey(arr5, 5, 10, 5, 77);
    assert(n5 == 6);
    assert(arr5[0] == 10 && arr5[1] == 20 && arr5[2] == 30 && arr5[3] == 40 && arr5[4] == 5 && arr5[5] == 77);

    // Test 6: Existing duplicate values not key
    int arr6[10] = {1, 2, 2, 3};
    std::size_t n6 = insertAfterEveryKey(arr6, 4, 10, 2, 8);
    assert(n6 == 6);
    assert(arr6[0] == 1 && arr6[1] == 2 && arr6[2] == 8 && arr6[3] == 2 && arr6[4] == 8 && arr6[5] == 3);

    return 0;
}
