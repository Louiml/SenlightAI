// Write a C++ function `insertArrayInMiddle` that takes two arrays along with their sizes. The first array must always have exactly 2 elements, and the function inserts the entire second array between the two elements of the first array, shifting the second element of the first array to the end. The function should print the resulting combined array in the format `[a, b, c, ...]` (without trailing comma before the closing bracket). Assume the input is valid (first array size is 2, and arrays are large enough to hold the result) and perform the insertion in-place on the first array (which must be sized to accommodate both arrays). The function should not return anything, but its side effect is printing the resulting array to standard output. Handle edge cases such as an empty second array (size 0) gracefully, and ensure the printed output matches the expected format exactly.
// The core algorithm is straightforward: identify the middle index of the first array, which for a size of 2 is index 1 (since `firstArraySize / 2` yields 1). The insertion requires shifting the second element (at index 1) to the right by `secondArraySize` positions to make room for the second array elements. To avoid overwriting data, the shift must be done from the end of the original first array backward. Since the first array is always size 2, we start at the last element (index 1) and shift it to index `1 + secondArraySize`. Then, we copy each element of the second array into the positions starting at the middle index (1). After insertion, the resulting array has `firstArraySize + secondArraySize` elements. The printing loop iterates over all elements except the last, printing each followed by `", "`, then prints the last element followed by `"]"`. Edge cases: if the second array is empty, the shift is zero-based, no elements are copied, and the output becomes `[first[0], first[1]]` — which is correct. The time complexity is O(n) where n is `secondArraySize`, because the shifting and copying each take linear time. Space complexity is O(1) aside from the input arrays, as the operation is done in-place.
#include <iostream>

// Insert the second array into the middle of the first array (which must have size 2)
// and print the resulting combined array in the format [a, b, c, ...].
void insertArrayInMiddle(int firstArray[], int firstArraySize, int secondArray[], int secondArraySize) {
    int middleIndex = firstArraySize / 2;  // For size 2, this is 1

    // Shift the second half of the first array to the right by secondArraySize positions.
    // Start from the end and move backwards to avoid overwriting data.
    for (int i = firstArraySize - 1; i >= middleIndex; --i) {
        firstArray[i + secondArraySize] = firstArray[i];
    }

    // Copy the second array into the freed space starting at the middle index.
    for (int i = 0; i < secondArraySize; ++i) {
        firstArray[middleIndex + i] = secondArray[i];
    }

    // Print the resulting array.
    int totalSize = firstArraySize + secondArraySize;
    std::cout << "Resulting array: [";
    for (int i = 0; i < totalSize - 1; ++i) {
        std::cout << firstArray[i] << ", ";
    }
    // Print the last element (or nothing if totalSize == 0, but with size 2 it's always at least 2)
    std::cout << firstArray[totalSize - 1] << "]";
}
#include <cassert>
#include <sstream>

// The function prints to stdout, so we redirect cout to a string stream for testing.
void testInsert() {
    // Test 1: Basic insertion with positive numbers.
    {
        int first[10] = {1, 2};
        int second[3] = {3, 4, 5};
        std::ostringstream out;
        std::streambuf* old = std::cout.rdbuf(out.rdbuf());
        insertArrayInMiddle(first, 2, second, 3);
        std::cout.rdbuf(old);
        assert(out.str() == "Resulting array: [1, 3, 4, 5, 2]");
    }
    // Test 2: Empty second array.
    {
        int first[10] = {9, 8};
        int second[1] = {0};  // unused but passed as size 0
        std::ostringstream out;
        std::streambuf* old = std::cout.rdbuf(out.rdbuf());
        insertArrayInMiddle(first, 2, second, 0);
        std::cout.rdbuf(old);
        assert(out.str() == "Resulting array: [9, 8]");
    }
    // Test 3: Second array with negative numbers.
    {
        int first[10] = {0, 0};
        int second[2] = {-1, -2};
        std::ostringstream out;
        std::streambuf* old = std::cout.rdbuf(out.rdbuf());
        insertArrayInMiddle(first, 2, second, 2);
        std::cout.rdbuf(old);
        assert(out.str() == "Resulting array: [0, -1, -2, 0]");
    }
    // Test 4: Second array with a single element.
    {
        int first[10] = {7, 11};
        int second[1] = {42};
        std::ostringstream out;
        std::streambuf* old = std::cout.rdbuf(out.rdbuf());
        insertArrayInMiddle(first, 2, second, 1);
        std::cout.rdbuf(old);
        assert(out.str() == "Resulting array: [7, 42, 11]");
    }
    // Test 5: Large second array (20 elements) to ensure shifting works.
    {
        int first[30] = {5, 6};
        int second[20];
        for (int i = 0; i < 20; ++i) second[i] = i + 1;
        std::ostringstream out;
        std::streambuf* old = std::cout.rdbuf(out.rdbuf());
        insertArrayInMiddle(first, 2, second, 20);
        std::cout.rdbuf(old);
        // Expected output: [5, 1, 2, ..., 20, 6] with commas and spaces.
        std::string expected = "Resulting array: [5, ";
        for (int i = 1; i <= 20; ++i) {
            expected += std::to_string(i);
            if (i < 20) expected += ", ";
        }
        expected += ", 6]";
        assert(out.str() == expected);
    }
}

int main() {
    testInsert();
    return 0;
}
