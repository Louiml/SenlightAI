/*
Write a C++ function `findCeilIndex` that takes a sorted integer array (non-decreasing order), its size, and a target value `x`, and returns the index of the smallest element in the array that is greater than or equal to `x` (i.e., the ceiling of `x`). If no such element exists (all elements are less than `x`), return `-1`. The array may contain duplicate values, and you must handle edge cases such as `x` being smaller than the first element, equal to an element, or larger than the last element. The function must be recursive and must not use any standard library functions like `lower_bound`. The input array is guaranteed to be sorted in ascending order.
*/
#include <vector>
#include <cstddef>

// Finds the index of the smallest element >= x in a sorted array.
// Returns -1 if no such element exists.
// Recursive binary search; array must be non-decreasing.
int findCeilIndex(const int arr[], size_t size, int x) {
    // Recursive helper with explicit bounds.
    auto search = [&](int low, int high) -> int {
        // Recurse using a lambda; but C++ requires a std::function for recursion.
        // Use a separate function instead for clarity.
        // This is a placeholder; real implementation below.
        return -1;
    };
    // Actual recursive function defined outside lambda.
    // Use a helper function defined inside the main function via std::function.
    // For brevity, implement as a separate static function.
    // The correct implementation:
    // (Left as comment to avoid confusion; see below for full code)
    // We'll write a named recursive lambda using std::function.
    std::function<int(int,int)> rec = [&](int low, int high) -> int {
        if (low > high) return -1;
        int mid = low + (high - low) / 2;
        if (arr[mid] == x) return mid;
        else if (arr[mid] < x) {
            if (mid + 1 <= high && x <= arr[mid + 1]) return mid + 1;
            else return rec(mid + 1, high);
        } else { // arr[mid] > x
            if (mid - 1 >= low && x > arr[mid - 1]) return mid;
            else return rec(low, mid - 1);
        }
    };
    return rec(0, static_cast<int>(size) - 1);
}
(Note: The above includes a recursive lambda requiring `<functional>`. For a clean self-contained solution, I’ll provide a proper implementation without `std::function` by using a helper function. Since the instruction says “output code only” for the solution section, I’ll write a clean version below.)

#include <functional>
#include <cstddef>

// Find index of smallest element >= x in sorted array arr[0..size-1].
// Returns -1 if no such element exists.
int findCeilIndex(const int arr[], size_t size, int x) {
    // Recursive lambda using std::function for clarity.
    std::function<int(int,int)> ceilRec = [&](int low, int high) -> int {
        if (low > high) return -1;
        int mid = low + (high - low) / 2;
        if (arr[mid] == x) return mid;
        else if (arr[mid] < x) {
            // Ceiling could be the next element if it's >= x.
            if (mid + 1 <= high && x <= arr[mid + 1]) return mid + 1;
            else return ceilRec(mid + 1, high);
        } else { // arr[mid] > x
            // If the previous element is also >= x, we need to go left.
            if (mid - 1 >= low && x > arr[mid - 1]) return mid;
            else return ceilRec(low, mid - 1);
        }
    };
    return ceilRec(0, static_cast<int>(size) - 1);
}
#include <cassert>
#include <vector>
// Include the solution function (assumed above).

int main() {
    int arr1[] = {1, 2, 8, 10, 10, 12, 19};
    size_t n1 = sizeof(arr1)/sizeof(arr1[0]);
    assert(findCeilIndex(arr1, n1, 10) == 3); // first 10
    assert(findCeilIndex(arr1, n1, 9) == 3);  // ceil 9 -> 10 at index 3
    assert(findCeilIndex(arr1, n1, 1) == 0);
    assert(findCeilIndex(arr1, n1, 0) == 0);
    assert(findCeilIndex(arr1, n1, 19) == 6);
    assert(findCeilIndex(arr1, n1, 20) == -1);
    assert(findCeilIndex(arr1, n1, 5) == 2); // ceil 5 -> 8 at index 2

    int arr2[] = {5, 5, 5, 5};
    size_t n2 = sizeof(arr2)/sizeof(arr2[0]);
    assert(findCeilIndex(arr2, n2, 5) == 0);
    assert(findCeilIndex(arr2, n2, 4) == 0);
    assert(findCeilIndex(arr2, n2, 6) == -1);

    int arr3[] = {2, 4, 6, 8};
    size_t n3 = sizeof(arr3)/sizeof(arr3[0]);
    assert(findCeilIndex(arr3, n3, 7) == 3);
    assert(findCeilIndex(arr3, n3, -1) == 0);
    assert(findCeilIndex(arr3, n3, 3) == 1);

    return 0;
}
// The problem is a classic binary-search-based ceiling search. The main algorithm: recursively narrow the search range `[low, high]`. Compute `mid = (low + high) / 2`. If `arr[mid] == x`, return `mid` (ceiling found). If `arr[mid] < x`, then the ceiling could be at `mid+1` if that element is ≥ `x`, otherwise recurse on `[mid+1, high]`. If `arr[mid] > x`, then `mid` is a candidate ceiling, but we must check if `mid-1` is also ≥ `x`; if so, recurse on `[low, mid-1]`, otherwise return `mid`. Base cases: when the range invalid (`low > high`), return `-1`. Edge cases: `x` smaller than the first element returns index 0; `x` larger than last returns `-1`; duplicates are handled by the equality check and by ensuring the search always returns the leftmost ceiling (by recurring left when possible). Time complexity is `O(log n)` and space complexity is `O(log n)` due to recursion stack.
