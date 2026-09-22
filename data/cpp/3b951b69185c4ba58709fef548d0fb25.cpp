// Given a fixed array of integers with a known size (e.g., `int arr[5] = {10, 20, 30, 40, 50}`), write a C++ function that copies the array elements into a `std::vector<int>` and returns that vector. The function must accept a pointer to the first element of the array and the array's size as parameters. It should handle arrays of any size (including size 0) and preserve the original order of elements. The function must be `const`-correct, meaning it should not modify the input array, and should be callable with arrays that are declared `const`. The returned vector should contain exactly the same elements in the same sequence as the input array.

The solution uses the standard library's `std::vector` constructor that takes a range defined by two iterators (or pointers): `std::vector<int> v(first, last)`, where `first` points to the first element and `last` points one past the last element. Given a pointer `arr` and a size `n`, we call `std::vector<int>(arr, arr + n)`. If `n` is 0, then `arr + 0` is the same as `arr`, creating an empty vector—this is safe as long as `arr` is a valid pointer (even if it points to a dummy address, pointer arithmetic with 0 is well-defined). No elements are modified in the input array, so `const` correctness is maintained by accepting `const int*` as the parameter type. Time complexity is O(n) because the vector copies all `n` elements. Space complexity is O(n) for the newly created vector, plus a constant amount for the function's local variables. Edge cases include an empty array (size 0) and arrays with duplicate or negative values—none of these require special handling, as the copy preserves all elements unconditionally.

#include <vector>

// Copy array elements into a vector and return it.
std::vector<int> arrayToVector(const int* arr, int size) {
    return std::vector<int>(arr, arr + size);
}

#include <cassert>
#include <vector>

// Assume the solution function is declared above.
std::vector<int> arrayToVector(const int* arr, int size);

int main() {
    int arr1[5] = {10, 20, 30, 40, 50};
    std::vector<int> v1 = arrayToVector(arr1, 5);
    assert(v1.size() == 5);
    assert(v1[0] == 10 && v1[1] == 20 && v1[2] == 30 && v1[3] == 40 && v1[4] == 50);

    int arr2[0] = {};  // empty array
    std::vector<int> v2 = arrayToVector(arr2, 0);
    assert(v2.empty());

    int arr3[3] = {7, -3, 7};
    std::vector<int> v3 = arrayToVector(arr3, 3);
    assert(v3[0] == 7 && v3[1] == -3 && v3[2] == 7);

    const int arr4[2] = {100, 200};
    std::vector<int> v4 = arrayToVector(arr4, 2);
    assert(v4 == (std::vector<int>{100, 200}));

    int arr5[1] = {42};
    std::vector<int> v5 = arrayToVector(arr5, 1);
    assert(v5.size() == 1 && v5[0] == 42);
}
