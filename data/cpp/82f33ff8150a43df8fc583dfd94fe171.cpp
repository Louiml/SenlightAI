Write a C++ function that takes an integer array (via pointer) and its size, along with a key integer to search for, and returns `true` if the key exists anywhere in the array, otherwise `false`. The function must be implemented recursively, without using any loops (e.g., `for`, `while`) or standard library search algorithms. The base case should handle the end of the array, and each recursive call should reduce the problem size by one. The function must not modify the array, and must work for any valid array size (including zero, where it should always return `false`). The array elements are unconstrained integers, and duplicates are possible.
The solution uses a classic linear search via recursion. Starting from the first element, check if it equals the key; if yes, return `true` immediately. If not, recursively call the function with the array pointer advanced by one (`arr + 1`) and the size reduced by one (`size - 1`). The base case is when `size == 0`, meaning no elements remain, so return `false`. This approach naturally handles edge cases: an empty array (size 0) returns `false`; a key at the first position returns after one comparison; duplicates are irrelevant because we only need existence. Time complexity is O(n) in the worst case (key absent or at the end), and O(1) if found at the start, but the overall worst-case is O(n). Space complexity is O(n) due to the recursion stack (each call adds a frame), which is noteworthy for large arrays. The function does not modify the array, so the pointer parameter should be declared `const` to enforce read-only access.
// Recursively search for key in arr[0..size-1].
// Returns true if key is present, false otherwise.
bool linearSearchRecursive(const int* arr, int size, int key) {
    // Base case: no elements left to check
    if (size == 0) {
        return false;
    }
    // If first element matches, key is found
    if (arr[0] == key) {
        return true;
    }
    // Otherwise, search the remaining subarray
    return linearSearchRecursive(arr + 1, size - 1, key);
}
int main() {
    // Basic tests
    int arr1[] = {1, 2, 3, 4, 5};
    assert(linearSearchRecursive(arr1, 5, 3) == true);
    assert(linearSearchRecursive(arr1, 5, 6) == false);
    assert(linearSearchRecursive(arr1, 5, 1) == true);  // first element
    assert(linearSearchRecursive(arr1, 5, 5) == true);  // last element

    // Edge case: empty array
    int emptyArr[] = {};
    assert(linearSearchRecursive(emptyArr, 0, 10) == false);

    // Duplicate values
    int dupArr[] = {7, 7, 7, 7};
    assert(linearSearchRecursive(dupArr, 4, 7) == true);
    assert(linearSearchRecursive(dupArr, 4, 9) == false);

    // Negative numbers
    int negArr[] = {-5, -2, 0, 3};
    assert(linearSearchRecursive(negArr, 4, -2) == true);
    assert(linearSearchRecursive(negArr, 4, -8) == false);

    // Single element
    int single[] = {42};
    assert(linearSearchRecursive(single, 1, 42) == true);
    assert(linearSearchRecursive(single, 1, 43) == false);
}
