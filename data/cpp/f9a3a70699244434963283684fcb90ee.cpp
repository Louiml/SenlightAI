/*
Write a C++ function named `findArrayMaximum` that is a templated function capable of determining the maximum value in any contiguous array of elements of the same type, where the type supports the `>` comparison operator (e.g., `int`, `float`, `double`). The function must accept a fixed-size C-style array (not a `std::array` or `std::vector`) along with its size, and return the maximum element. The function should handle arrays with at least one element (no need to handle empty arrays), and must be `const`-correct (i.e., it should not modify the input array). In addition, write a helper function `printMaxIntFloat` that demonstrates the function by creating an integer array `{5, 2, 9, 1, 7}` and a float array `{2.5, 3.8, 1.9, 6.2, 4.1}`, calling `findArrayMaximum` for each, and printing the results in the exact format:
`Maximum in integer array: 9`
`Maximum in float array: 6.2`
Use `std::cout` and include the necessary headers. The solution must not use any global variables or non-const global state.
*/
#include <iostream>

// Templated function to find the maximum value in a fixed-size C-style array.
// The type T must support the > operator.
template <typename T>
T findArrayMaximum(int size, const T arr[]) {
    // Assume array is non-empty (size >= 1).
    T maxValue = arr[0];
    for (int i = 1; i < size; ++i) {
        if (arr[i] > maxValue) {
            maxValue = arr[i];
        }
    }
    return maxValue;
}

// Helper function demonstrating usage for int and float arrays.
void printMaxIntFloat() {
    const int intArray[] = {5, 2, 9, 1, 7};
    const float floatArray[] = {2.5f, 3.8f, 1.9f, 6.2f, 4.1f};
    int intSize = sizeof(intArray) / sizeof(intArray[0]);
    int floatSize = sizeof(floatArray) / sizeof(floatArray[0]);

    int intMax = findArrayMaximum(intSize, intArray);
    float floatMax = findArrayMaximum(floatSize, floatArray);

    std::cout << "Maximum in integer array: " << intMax << std::endl;
    std::cout << "Maximum in float array: " << floatMax << std::endl;
}
#include <cassert>
#include <sstream>
#include <iostream>

// Include the solution function here (or copy it above)
template <typename T>
T findArrayMaximum(int size, const T arr[]) {
    T maxValue = arr[0];
    for (int i = 1; i < size; ++i) {
        if (arr[i] > maxValue) maxValue = arr[i];
    }
    return maxValue;
}

void printMaxIntFloat() {
    const int intArray[] = {5, 2, 9, 1, 7};
    const float floatArray[] = {2.5f, 3.8f, 1.9f, 6.2f, 4.1f};
    int intSize = sizeof(intArray) / sizeof(intArray[0]);
    int floatSize = sizeof(floatArray) / sizeof(floatArray[0]);
    int intMax = findArrayMaximum(intSize, intArray);
    float floatMax = findArrayMaximum(floatSize, floatArray);
    std::cout << "Maximum in integer array: " << intMax << std::endl;
    std::cout << "Maximum in float array: " << floatMax << std::endl;
}

int main() {
    // Test findArrayMaximum with integer arrays
    int a1[] = {5, 2, 9, 1, 7};
    assert(findArrayMaximum(5, a1) == 9);
    int a2[] = {42};
    assert(findArrayMaximum(1, a2) == 42);
    int a3[] = {-5, -10, -3, -8};
    assert(findArrayMaximum(4, a3) == -3);
    int a4[] = {7, 7, 7, 7};
    assert(findArrayMaximum(4, a4) == 7);

    // Test with float arrays (use approximate comparison for floating point)
    float f1[] = {2.5f, 3.8f, 1.9f, 6.2f, 4.1f};
    assert(findArrayMaximum(5, f1) == 6.2f);
    float f2[] = {0.0f, -1.5f, -2.5f};
    assert(findArrayMaximum(3, f2) == 0.0f);
    float f3[] = {3.3f, 3.3f};
    assert(findArrayMaximum(2, f3) == 3.3f);

    // Test double precision as well
    double d1[] = {1.1, 9.9, 2.2};
    assert(findArrayMaximum(3, d1) == 9.9);

    // Verify the helper function produces the expected output format by capturing cout
    std::stringstream buffer;
    std::streambuf* oldCout = std::cout.rdbuf(buffer.rdbuf());
    printMaxIntFloat();
    std::cout.rdbuf(oldCout);
    std::string expected = "Maximum in integer array: 9\nMaximum in float array: 6.2\n";
    assert(buffer.str() == expected);

    return 0;
}
// The solution involves defining a function template `template<typename T> T findArrayMaximum(int size, const T arr[])`. The algorithm initializes a local variable `maxValue` with the first element of the array (`arr[0]`), then iterates from index 1 to `size-1`, comparing each element with the current `maxValue` and updating it if a larger element is found. Because the function template works with any type supporting `>`, it is generic and reusable for `int`, `float`, `double`, etc. Edge cases: if the array has size 0, the function would be invalid (but the task guarantees at least one element, so no special handling is needed); if all elements are equal, the function correctly returns that value; negative numbers are handled naturally because comparisons work on the numeric values. Time complexity is O(n) because we scan the array once. Space complexity is O(1) auxiliary, as we only store a single variable plus loop counters. The function is `const`-correct by taking the array as `const T arr[]`, preventing modification. The helper `printMaxIntFloat` calls the template with explicit template arguments (or lets the compiler deduce them) and prints using `std::cout`, ensuring the output matches the specified format exactly (including newline characters).
