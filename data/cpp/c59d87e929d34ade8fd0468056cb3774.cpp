Write a C++ function named `sortedArrayInsertNumber` that takes a pointer to a dynamically allocated, sorted array of integers, its current length, and a new integer value to insert. The function must insert the value into its correct sorted position, preserving the sorted order of the array, and return a pointer to the resized array. If the input pointer is null or the length is negative, the function must return `nullptr`. The function should use `realloc` (via `std::realloc` from `<cstdlib>` or `realloc` from `<malloc.h>`) to resize the array by one element, shifting existing elements as needed. The array is modified in place, and the caller is responsible for freeing the returned pointer. The task requires implementing two helper functions: one to find the correct insertion index (by scanning for the first element greater than the new value) and another to shift elements and insert the value.
// The main algorithm performs the insertion in three steps. First, validate inputs: if `Arr` is null or `len` is negative, return `nullptr`. Because the input is sorted ascending, the correct insertion position can be found by scanning from the left until we find an element greater than `num`. If no such element exists, the position is at the end (`len`). If the first element is already greater than `num`, the position is 0. This linear scan takes O(n) time in the worst case. After finding the position, resize the array with `realloc` to `(len + 1)` integers. Then, starting from the last index of the new array, shift each element one position to the right until reaching the insertion position, and finally place `num` at that position. The shift also takes O(n) time. The total time complexity is O(n), and space complexity is O(1) additional memory (aside from the reallocation, which may copy data internally but does not allocate extra permanent memory). Edge cases include inserting at the beginning (all elements shift right), inserting at the end (no shift needed), inserting into an empty array (length 0 but must have valid non-null pointer from a prior allocation, or realloc with size 1 works), and duplicate values—the function should insert after existing equal elements to maintain stability, which this algorithm handles because it stops shifting when it finds the first strictly greater element.
#include <cstdlib> // for realloc

// Helper: find the insertion index in a sorted array.
// Returns 0 if num is smaller than all elements, len if larger than all,
// otherwise the index of the first element greater than num.
int findInsertionPos(const int* arr, int len, int num) {
    for (int i = 0; i < len; ++i) {
        if (arr[i] > num) {
            return i;
        }
    }
    return len;
}

// Helper: shift elements from position pos to the right by one and insert num.
void insertAtPos(int* arr, int len, int num, int pos) {
    // len is the new length after resize; start from last index.
    for (int i = len - 1; i > pos; --i) {
        arr[i] = arr[i - 1];
    }
    arr[pos] = num;
}

// Insert num into sorted array Arr of length len. Returns the new array pointer.
// Returns nullptr on invalid inputs. Uses realloc to resize.
int* sortedArrayInsertNumber(int* arr, int len, int num) {
    if (arr == nullptr || len < 0) {
        return nullptr;
    }

    // Resize to accommodate one more element.
    int* resized = static_cast<int*>(realloc(arr, (len + 1) * sizeof(int)));
    if (resized == nullptr) {
        return nullptr; // realloc failure; original arr is still valid but we return null.
    }

    int pos = findInsertionPos(resized, len, num);
    insertAtPos(resized, len + 1, num, pos);

    return resized;
}
#include <cassert>
#include <cstdlib>

int main() {
    // Test 1: insert in the middle
    int* arr1 = static_cast<int*>(malloc(3 * sizeof(int)));
    arr1[0] = 2; arr1[1] = 4; arr1[2] = 6;
    int* res1 = sortedArrayInsertNumber(arr1, 3, 5);
    assert(res1 != nullptr);
    assert(res1[0] == 2 && res1[1] == 4 && res1[2] == 5 && res1[3] == 6);
    free(res1);

    // Test 2: insert at beginning
    int* arr2 = static_cast<int*>(malloc(2 * sizeof(int)));
    arr2[0] = 3; arr2[1] = 8;
    int* res2 = sortedArrayInsertNumber(arr2, 2, 1);
    assert(res2 != nullptr);
    assert(res2[0] == 1 && res2[1] == 3 && res2[2] == 8);
    free(res2);

    // Test 3: insert at end
    int* arr3 = static_cast<int*>(malloc(2 * sizeof(int)));
    arr3[0] = 1; arr3[1] = 2;
    int* res3 = sortedArrayInsertNumber(arr3, 2, 9);
    assert(res3 != nullptr);
    assert(res3[0] == 1 && res3[1] == 2 && res3[2] == 9);
    free(res3);

    // Test 4: duplicate values - insert after existing equal
    int* arr4 = static_cast<int*>(malloc(2 * sizeof(int)));
    arr4[0] = 4; arr4[1] = 4;
    int* res4 = sortedArrayInsertNumber(arr4, 2, 4);
    assert(res4 != nullptr);
    assert(res4[0] == 4 && res4[1] == 4 && res4[2] == 4);
    free(res4);

    // Test 5: invalid inputs
    assert(sortedArrayInsertNumber(nullptr, 5, 10) == nullptr);
    int valid[1] = {1};
    assert(sortedArrayInsertNumber(valid, -1, 10) == nullptr);

    // Test 6: empty array (len=0, but valid pointer from malloc(0) is implementation-defined; use a 1-element allocation to be safe)
    int* arr6 = static_cast<int*>(malloc(1 * sizeof(int))); // allocate one to avoid issues
    // Treat as empty by passing len=0; realloc will still work.
    int* res6 = sortedArrayInsertNumber(arr6, 0, 42);
    assert(res6 != nullptr);
    assert(res6[0] == 42);
    free(res6);

    return 0;
}
