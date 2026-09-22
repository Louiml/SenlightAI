// Write a C++ function that takes a fixed-capacity integer array (implemented as a raw pointer with a maximum size of 100), the current number of elements in the array, a new integer value, and a zero-based insertion position. The function must insert the new value at the specified position, shifting existing elements to the right, and update the size. If the array is already full (size equals 100) or the position is invalid (less than 0 or greater than current size), the function should perform no modification and return `false`; otherwise it inserts and returns `true`. The function should use `const` where appropriate for parameters not modified and must handle the case where the size is 0 (inserting at position 0 is valid). After insertion, the size must be incremented by one.
// The core algorithm mirrors the insertion step in an array-based list. Starting from the last element (index `size - 1`) and moving backward, each element is copied one position to the right (`arr[i] = arr[i-1]`) until reaching the target position `pos`. This overwrites the position at `pos` with the new value, then increments the size. Edge cases: (1) if `size >= MAX_SIZE`, insertion is impossible; (2) if `pos < 0` or `pos > size`, the position is invalid (note `pos == size` means appending at the end); (3) if the array is empty (`size == 0`), only `pos == 0` is valid, and the loop body must not execute (since `size == 0`, the loop initializer `i = 0` is not greater than `pos = 0`). Time complexity is O(n) for the shifting, where n is the current size; space complexity is O(1) auxiliary. All modifications are in-place.
#include <cstddef> // for size_t, though we use int for simplicity

// Inserts 'num' at 'pos' in array 'arr' of capacity 100.
// 'size' is the current number of elements (modified on success).
// Returns true if insertion succeeded, false otherwise.
bool insertNumber(int arr[], int& size, int num, int pos) {
    constexpr int MAX_SIZE = 100;
    
    // Check for full array or invalid position.
    if (size >= MAX_SIZE || pos < 0 || pos > size) {
        return false;
    }
    
    // Shift elements to the right from the end down to pos.
    for (int i = size; i > pos; --i) {
        arr[i] = arr[i - 1];
    }
    
    // Place the new value and update size.
    arr[pos] = num;
    ++size;
    
    return true;
}
#include <cassert>

// Forward declaration of the solution function (not needed if included above).
bool insertNumber(int arr[], int& size, int num, int pos);

int main() {
    // Test 1: Insert into empty array at position 0.
    int arr1[100];
    int size1 = 0;
    assert(insertNumber(arr1, size1, 42, 0) == true);
    assert(size1 == 1);
    assert(arr1[0] == 42);
    
    // Test 2: Insert in the middle shifts elements.
    int arr2[100] = {1, 2, 4, 5};
    int size2 = 4;
    assert(insertNumber(arr2, size2, 3, 2) == true);
    assert(size2 == 5);
    assert(arr2[0] == 1 && arr2[1] == 2 && arr2[2] == 3 && arr2[3] == 4 && arr2[4] == 5);
    
    // Test 3: Insert at the end (append).
    int arr3[100] = {1, 2, 3};
    int size3 = 3;
    assert(insertNumber(arr3, size3, 9, 3) == true);
    assert(size3 == 4);
    assert(arr3[3] == 9);
    
    // Test 4: Insert at the beginning.
    int arr4[100] = {2, 3, 4};
    int size4 = 3;
    assert(insertNumber(arr4, size4, 1, 0) == true);
    assert(size4 == 4);
    assert(arr4[0] == 1 && arr4[1] == 2 && arr4[2] == 3 && arr4[3] == 4);
    
    // Test 5: Invalid position (negative) – no change.
    int arr5[100] = {5, 6};
    int size5 = 2;
    assert(insertNumber(arr5, size5, 10, -1) == false);
    assert(size5 == 2 && arr5[0] == 5 && arr5[1] == 6);
    
    // Test 6: Invalid position (greater than size) – no change.
    int arr6[100] = {5, 6};
    int size6 = 2;
    assert(insertNumber(arr6, size6, 10, 3) == false);
    assert(size6 == 2 && arr6[0] == 5 && arr6[1] == 6);
    
    // Test 7: Full array – cannot insert.
    int arr7[100];
    int size7 = 100;
    for (int i = 0; i < 100; ++i) arr7[i] = i;
    assert(insertNumber(arr7, size7, 999, 50) == false);
    assert(size7 == 100); // unchanged
    
    // Test 8: Insert at position equal to current size (append) when not full.
    int arr8[100] = {7};
    int size8 = 1;
    assert(insertNumber(arr8, size8, 8, 1) == true);
    assert(size8 == 2 && arr8[0] == 7 && arr8[1] == 8);
    
    return 0;
}
