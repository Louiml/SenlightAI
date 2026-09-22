Write a standalone C++ function named `processArray` that takes a reference to a `std::vector<int>` of exactly 5 elements (simulating a dataArray) and an integer `data` (simulating a runtime random value). The function must allocate a dynamic buffer of 10 integers, initialize all elements to 0, and then—if `data` is non-negative and strictly less than 10—set `buffer[data] = 1` and print each buffer element using `std::cout` with a newline. If `data` is negative or >= 10, print the error message `"ERROR: Array index is out of bounds."` to `std::cout` and do not modify the buffer. After the conditional logic, deallocate the buffer with `delete[]` and return nothing (void). The function must be safe against invalid indices and handle all edge cases correctly.
#include <cassert>
#include <iostream>
#include <sstream>
#include <vector>

// Forward declaration of the function to test
void processArray(const std::vector<int>& dataArray);

// Helper to capture stdout
std::string captureOutput(const std::vector<int>& dataArray) {
    std::ostringstream oss;
    std::streambuf* oldCout = std::cout.rdbuf(oss.rdbuf());
    processArray(dataArray);
    std::cout.rdbuf(oldCout);
    return oss.str();
}

int main() {
    // Valid index at position 2 (data=3)
    std::vector<int> arr1 = {0, 0, 3, 0, 0};
    std::string out1 = captureOutput(arr1);
    std::string expected1 = "0\n0\n0\n1\n0\n0\n0\n0\n0\n0\n";
    assert(out1 == expected1);

    // Valid index at lower bound (data=0)
    std::vector<int> arr2 = {0, 0, 0, 0, 0};
    std::string out2 = captureOutput(arr2);
    std::string expected2 = "1\n0\n0\n0\n0\n0\n0\n0\n0\n0\n";
    assert(out2 == expected2);

    // Valid index at upper bound (data=9)
    std::vector<int> arr3 = {0, 0, 9, 0, 0};
    std::string out3 = captureOutput(arr3);
    std::string expected3 = "0\n0\n0\n0\n0\n0\n0\n0\n0\n1\n";
    assert(out3 == expected3);

    // Invalid negative index
    std::vector<int> arr4 = {0, 0, -1, 0, 0};
    std::string out4 = captureOutput(arr4);
    std::string expected4 = "ERROR: Array index is out of bounds.\n0\n0\n0\n0\n0\n0\n0\n0\n0\n0\n";
    assert(out4 == expected4);

    // Invalid index too large (data=10)
    std::vector<int> arr5 = {0, 0, 10, 0, 0};
    std::string out5 = captureOutput(arr5);
    std::string expected5 = "ERROR: Array index is out of bounds.\n0\n0\n0\n0\n0\n0\n0\n0\n0\n0\n";
    assert(out5 == expected5);

    // Invalid index much too large (data=100)
    std::vector<int> arr6 = {0, 0, 100, 0, 0};
    std::string out6 = captureOutput(arr6);
    std::string expected6 = "ERROR: Array index is out of bounds.\n0\n0\n0\n0\n0\n0\n0\n0\n0\n0\n";
    assert(out6 == expected6);

    std::cout << "All tests passed!" << std::endl;
    return 0;
}
#include <iostream>
#include <vector>

// Process a 5-element array using its 3rd element (index 2) as an index.
// Allocate a 10-element buffer, set buffer[data]=1 if valid, print buffer, then free.
void processArray(const std::vector<int>& dataArray) {
    // Extract the index candidate from the 3rd element (index 2)
    int data = dataArray[2];
    
    // Allocate and initialize buffer
    int* buffer = new int[10];
    for (int i = 0; i < 10; ++i) {
        buffer[i] = 0;
    }
    
    // Validate index before direct access
    if (data >= 0 && data < 10) {
        buffer[data] = 1;
    } else {
        std::cout << "ERROR: Array index is out of bounds." << std::endl;
    }
    
    // Print all buffer elements (if error, all zeros are printed)
    for (int i = 0; i < 10; ++i) {
        std::cout << buffer[i] << std::endl;
    }
    
    delete[] buffer;
}
// The core task is straightforward: validate the index before any array access to prevent undefined behavior. The input is constrained to exactly 5 elements for the array (matching the original snippet), but the only value actually used is the 3rd element (`data = dataArray[2]`). The function first allocates a heap buffer of size 10 using `new int[10]`, initializes every element to 0 via a loop, then checks the condition `data >= 0 && data < 10`. If valid, it sets `buffer[data] = 1`; otherwise it prints the error. Finally, it prints all 10 buffer elements (either all zeros, or one 1 at the valid index) and deallocates the buffer. Edge cases include negative `data`, `data` equal to 0 (valid), `data` equal to 9 (valid), and `data` >= 10 (invalid). For a constant buffer size of 10, the time complexity is O(10) = O(1) for initialization and printing, and space complexity is O(10) = O(1) auxiliary (dynamic allocation still counts as constant since size is fixed). The solution avoids any out-of-bounds access by strict validation.
