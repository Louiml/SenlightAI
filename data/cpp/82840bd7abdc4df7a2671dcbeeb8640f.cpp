Write a C++ function named `dnfSort` that takes a non-constant array of integers and its size `n` as parameters, and sorts the array in-place using the Dutch National Flag algorithm. The array is guaranteed to contain only the values 0, 1, and 2 (and may contain duplicates). The function must rearrange the elements so that all 0s come first, followed by all 1s, and then all 2s. The algorithm must only use a single pass over the array (i.e., loop while a moving index is within bounds) and must not use any extra data structures. The function should be `const`-correct for any read-only helper logic—although the array itself is modified—and should not return a value. Handle the edge case where `n` is 0 or 1 gracefully (e.g., do nothing). The solution must be self-contained, include only necessary headers, and be ready for integration into a larger program.
#include <cassert>
#include <iostream>

// Declaration of the solution function (must match the provided implementation).
void dnfSort(int arr[], int n);

// Helper to check that an array is sorted according to 0-1-2 order.
bool isSorted012(const int arr[], int n) {
    int last = 0;
    for (int i = 0; i < n; ++i) {
        if (arr[i] < last) return false;
        last = arr[i];
    }
    return true;
}

int main() {
    // Test 1: Typical mixed array
    int a1[] = {0, 2, 2, 1, 1, 1, 0, 2};
    dnfSort(a1, 8);
    assert(isSorted012(a1, 8));

    // Test 2: Already sorted
    int a2[] = {0, 0, 1, 1, 2, 2};
    dnfSort(a2, 6);
    assert(isSorted012(a2, 6));

    // Test 3: Reverse order
    int a3[] = {2, 2, 1, 1, 0, 0};
    dnfSort(a3, 6);
    assert(isSorted012(a3, 6));

    // Test 4: All zeros only
    int a4[] = {0, 0, 0};
    dnfSort(a4, 3);
    assert(isSorted012(a4, 3));

    // Test 5: All ones only
    int a5[] = {1, 1, 1};
    dnfSort(a5, 3);
    assert(isSorted012(a5, 3));

    // Test 6: All twos only
    int a6[] = {2, 2, 2};
    dnfSort(a6, 3);
    assert(isSorted012(a6, 3));

    // Test 7: Single element
    int a7[] = {1};
    dnfSort(a7, 1);
    assert(isSorted012(a7, 1));

    // Test 8: Empty array
    int a8[] = {};
    dnfSort(a8, 0);
    assert(isSorted012(a8, 0));

    // Test 9: Large test with repeated values
    int a9[] = {2, 0, 1, 2, 0, 1, 2, 0, 1, 2, 0, 1};
    dnfSort(a9, 12);
    assert(isSorted012(a9, 12));

    // Test 10: Two elements
    int a10[] = {2, 0};
    dnfSort(a10, 2);
    assert(isSorted012(a10, 2));

    std::cout << "All tests passed." << std::endl;
    return 0;
}
#include <utility>  // for std::swap

// Sorts an array containing only 0, 1, and 2 in-place using the Dutch National Flag algorithm.
// Parameters:
//   arr - array of integers (only values 0, 1, or 2 allowed)
//   n   - number of elements in arr
// Postcondition: all 0s appear before all 1s before all 2s.
void dnfSort(int arr[], int n) {
    int low = 0;    // next position for a 0
    int mid = 0;    // current element being examined
    int high = n - 1; // next position for a 2 (from the right)

    while (mid <= high) {
        if (arr[mid] == 0) {
            std::swap(arr[low], arr[mid]);
            ++low;
            ++mid;
        } else if (arr[mid] == 1) {
            ++mid;
        } else { // arr[mid] == 2
            std::swap(arr[mid], arr[high]);
            --high;
            // do not increment mid; the value swapped from high is unexamined
        }
    }
}
// The Dutch National Flag algorithm uses three pointers: `low`, `mid`, and `high`. `low` marks the boundary where all elements before it are 0, `mid` is the current element being examined, and `high` marks the boundary where all elements after it are 2. Initially, `low = 0`, `mid = 0`, and `high = n-1`. The loop runs while `mid <= high`. At each step, inspect `a[mid]`:
// - If it is 0, swap `a[low]` and `a[mid]`, then increment both `low` and `mid` (because after the swap, both positions are correctly placed).
// - If it is 1, just increment `mid` (1 is in the correct section).
// - If it is 2, swap `a[mid]` and `a[high]`, then decrement `high` (but do **not** increment `mid`, because the new value at `mid` after the swap is unexamined).
// This single-pass approach correctly partitions the array. Edge cases: empty array (`n=0`) and single-element array (`n=1`) require no action—the loop condition `mid <= high` fails initially or immediately, so it works without special handling. Complexity: Time is O(n) because each element is examined at most once (each swap places an element correctly), and space is O(1) auxiliary (only three indices and a temporary swap variable). The algorithm is stable? No, but stability is not required for this problem.
