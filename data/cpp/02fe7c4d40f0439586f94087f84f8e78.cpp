// Given an array of integers and its size, write a C++ function that returns a `std::string` summarizing the array contents in the exact format: "Element at index i is value" for each index, one per line, prefixed by the header "Array contents:". For example, for the array {1,2,3} with size 3, the output string should be exactly "Array contents:\nElement at index 0 is 1\nElement at index 1 is 2\nElement at index 2 is 3". The function should handle an empty array (size 0) by returning only the header "Array contents:" without any additional lines. The input array must not be modified, and the function must be `const`-correct (use `const int*` or `const std::vector<int>&`). Use `std::to_string` to convert integers to strings and `\n` for newlines. This task tests your ability to iterate over arrays, format output as a string, and respect const-correctness — no console output is allowed inside the function itself; everything must be returned as a single string.

// The solution involves three main steps:
// 1. Initialize an empty `std::string` result.
// 2. Append the header `"Array contents:\n"` to the result.
// 3. Iterate from index 0 up to `size-1`, and for each index `i`, append `"Element at index " + std::to_string(i) + " is " + std::to_string(arr[i]) + "\n"` to the result.
//
// Edge cases:
// - **Empty array (size 0)**: The loop iterates zero times, so the result is just `"Array contents:\n"`. This is handled naturally by the loop condition.
// - **Negative values**: `std::to_string` handles negative numbers correctly (e.g., `std::to_string(-5)` returns `"-5"`).
// - **Size mismatch**: The function trusts the provided `size` parameter; it does not check for array bounds since the caller is responsible for passing a valid size. This is standard for C-style arrays.
//
// Time complexity: \(O(n)\) where \(n\) is the size, because we perform constant work per element (one string concatenation). Space complexity: \(O(n)\) for the resulting string, plus \(O(1)\) auxiliary space for the loop variable and temporary strings.
//
// The main algorithm is a simple linear scan, which cannot be improved asymptotically because we must visit every element to include it in the output.

#include <string>

// Returns a formatted summary of the array contents.
// The output format is:
// "Array contents:\nElement at index i is value\n" for each i from 0 to size-1.
// For an empty array, only "Array contents:\n" is returned.
// The input array is not modified (const-correct).
std::string formatArray(const int* arr, int size) {
    std::string result = "Array contents:\n";
    for (int i = 0; i < size; ++i) {
        result += "Element at index " + std::to_string(i) + " is " + std::to_string(arr[i]) + "\n";
    }
    return result;
}

#include <cassert>
#include <string>

// The solution function is declared here (assuming it's in the same file or included).
std::string formatArray(const int* arr, int size);

int main() {
    // Test 1: Normal array
    int arr1[] = {1, 2, 3};
    std::string expected1 = "Array contents:\nElement at index 0 is 1\nElement at index 1 is 2\nElement at index 2 is 3\n";
    assert(formatArray(arr1, 3) == expected1);

    // Test 2: Empty array
    int* arr2 = nullptr; // size 0, pointer can be null since we don't dereference
    std::string expected2 = "Array contents:\n";
    assert(formatArray(arr2, 0) == expected2);

    // Test 3: Single element
    int arr3[] = {42};
    std::string expected3 = "Array contents:\nElement at index 0 is 42\n";
    assert(formatArray(arr3, 1) == expected3);

    // Test 4: Negative and zero values
    int arr4[] = {-5, 0, 7};
    std::string expected4 = "Array contents:\nElement at index 0 is -5\nElement at index 1 is 0\nElement at index 2 is 7\n";
    assert(formatArray(arr4, 3) == expected4);

    // Test 5: Larger array with duplicate values
    int arr5[] = {10, 10, 20, 30, 10};
    std::string expected5 = "Array contents:\nElement at index 0 is 10\nElement at index 1 is 10\nElement at index 2 is 20\nElement at index 3 is 30\nElement at index 4 is 10\n";
    assert(formatArray(arr5, 5) == expected5);

    return 0;
}
