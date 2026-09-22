/*
Write a C++ function named `arrayElementCount` that takes a reference to a C-style array of integers (using a template parameter for the array size) and returns the number of elements in that array as a `std::size_t`. The function must not use `sizeof` directly on the array parameter (since it decays to a pointer), but instead use the template size deduction to compute the element count. Additionally, write a second free function `printArrayStats` that takes the same array reference and prints the array size in bytes and the element count to `std::cout` in the format: `Size in bytes: <n>`, then newline, then `Number of elements: <m>`. Ensure both functions are `const`-correct (they should accept `const int (&arr)[N]` so they do not modify the array). The solution must be self-contained with necessary headers, but no `main` function in the solution code.
*/
#include <cstddef>
#include <iostream>

// Returns the number of elements in a C-style array of ints.
// Uses template parameter N to deduce the array size without decaying.
template<std::size_t N>
std::size_t arrayElementCount(const int (&arr)[N]) {
    (void)arr; // Mark unused to avoid warnings, but arr is not needed for count
    return N;
}

// Prints the total byte size and the element count of the given int array.
template<std::size_t N>
void printArrayStats(const int (&arr)[N]) {
    std::size_t bytes = sizeof(arr); // arr is a reference to the full array, so sizeof gives total bytes
    std::size_t count = arrayElementCount(arr);
    std::cout << "Size in bytes: " << bytes << '\n';
    std::cout << "Number of elements: " << count << '\n';
}
#include <cassert>
#include <cstddef>

// Declare the function template (must match the solution's signature)
template<std::size_t N>
std::size_t arrayElementCount(const int (&arr)[N]);

int main() {
    int arr1[] = {32, 90, 6, 56, 12, 5, 78};
    assert(arrayElementCount(arr1) == 7);
    
    int arr2[] = {1};
    assert(arrayElementCount(arr2) == 1);
    
    int arr3[] = {0, 0, 0, 0};
    assert(arrayElementCount(arr3) == 4);
    
    const int arr4[] = {10, 20, 30, 40, 50, 60, 70, 80, 90, 100};
    assert(arrayElementCount(arr4) == 10);
    
    int arr5[] = {42, -17, 0, 99, -3};
    assert(arrayElementCount(arr5) == 5);
    
    assert((arrayElementCount<int, 7>(arr1)) == 7); // explicit template argument check
    
    return 0;
}
// The core challenge is to avoid the pitfall shown in the snippet where `sizeof(MyArr)` returns the total bytes (e.g., 28 for 7 integers on a typical 4-byte int system), not the element count. To correctly compute the element count, we divide the total byte size by the size of a single element. However, when passing an array to a function, it decays to a pointer, so `sizeof` on the parameter would give the pointer size. The solution is to use a template function with a non-type parameter for the array size `N`: `template<std::size_t N> std::size_t arrayElementCount(const int (&arr)[N])`. Inside the function, `N` is already the element count, so we can return `N` directly. For `printArrayStats`, we call `arrayElementCount(arr)` to get the count, and compute the size in bytes as `sizeof(arr)` (since the reference preserves the full array type). Edge cases: an empty array would have `N=0` but C++ does not allow zero-size standard arrays, so this is not a concern; all arrays have at least one element. The functions are `const`-correct because they only read the array. Time complexity is O(1) for both, and space complexity O(1) (no extra storage). The only tricky part is ensuring the template parameter is deduced correctly from the array reference, which works for any size.
