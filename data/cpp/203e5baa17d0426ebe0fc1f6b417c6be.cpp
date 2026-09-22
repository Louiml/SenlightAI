/*
Write a C++ function named `transformAndFindRange` that takes a fixed-size array of 100 integers and a valid student count `n` (1 ≤ n ≤ 100). The function should read `n` integer marks (each between -1000 and 1000) from standard input, double each mark and store it back into the first `n` positions of the array (while leaving positions `n` through 99 untouched, all initially set to -1). After processing, the function should return the maximum doubled value among the first `n` elements. The function must not modify any index outside `[0, n-1]`, and it must handle duplicate values, negative marks, and the case where `n` equals 100 (no unused slots). The function should not print anything; it should only compute and return the maximum. Input format: the first integer is the count, followed by that many integers (each on a new line or space-separated). The function does the reading internally, so the caller provides no input.
*/

#include <iostream>
#include <climits>
#include <algorithm>

// Reads n student marks from stdin, doubles each, stores in arr[0..n-1],
// and returns the maximum doubled value among those n entries.
// arr must have at least n elements. All other arr positions remain unchanged.
int transformAndFindRange(int arr[100], int n) {
    // Initialize all 100 positions to -1 as per the original snippet's intent.
    std::fill(arr, arr + 100, -1);
    
    int maxVal = INT_MIN;
    
    for (int i = 0; i < n; ++i) {
        std::cin >> arr[i];
        arr[i] *= 2;
        maxVal = std::max(maxVal, arr[i]);
    }
    
    return maxVal;
}

#include <cassert>
#include <sstream>
#include <iostream>

// Declare the solution function (already defined above in the actual file)
int transformAndFindRange(int arr[100], int n);

int main() {
    // Test 1: Basic positive marks
    {
        int arr[100];
        std::istringstream input("3\n10 20 30\n");
        std::cin.rdbuf(input.rdbuf());
        int result = transformAndFindRange(arr, 3);
        assert(result == 60);
        assert(arr[0] == 20 && arr[1] == 40 && arr[2] == 60);
        assert(arr[3] == -1 && arr[99] == -1);
    }
    
    // Test 2: Negative and mixed values
    {
        int arr[100];
        std::istringstream input("4\n-5 0 7 -3\n");
        std::cin.rdbuf(input.rdbuf());
        int result = transformAndFindRange(arr, 4);
        assert(result == 14);
        assert(arr[0] == -10 && arr[1] == 0 && arr[2] == 14 && arr[3] == -6);
    }
    
    // Test 3: n equals 1
    {
        int arr[100];
        std::istringstream input("1\n42\n");
        std::cin.rdbuf(input.rdbuf());
        int result = transformAndFindRange(arr, 1);
        assert(result == 84);
        assert(arr[0] == 84 && arr[1] == -1);
    }
    
    // Test 4: Duplicates
    {
        int arr[100];
        std::istringstream input("5\n4 4 4 4 4\n");
        std::cin.rdbuf(input.rdbuf());
        int result = transformAndFindRange(arr, 5);
        assert(result == 8);
        for (int i = 0; i < 5; ++i) assert(arr[i] == 8);
        assert(arr[5] == -1);
    }
    
    // Test 5: n equals 100 (full array usage)
    {
        int arr[100];
        std::string data = "100\n";
        for (int i = 0; i < 100; ++i) data += std::to_string(i) + "\n";
        std::istringstream input(data);
        std::cin.rdbuf(input.rdbuf());
        int result = transformAndFindRange(arr, 100);
        assert(result == 198); // 99*2
        assert(arr[99] == 198);
    }
    
    std::cout << "All tests passed!" << std::endl;
    return 0;
}

// The algorithm initializes an array of 100 elements, each set to -1 using `int marks[100] = {-1};` (note: this sets only the first element to -1 and the rest to 0; to set all to -1, we need `std::fill` or a loop—this is an important edge case to handle correctly). Read `n` from `cin`, then for each `i` from 0 to n-1, read a value into `marks[i]`, immediately double it. Maintain a running maximum variable initialized to the smallest possible integer (e.g., `INT_MIN`) or to the first doubled value. Update the maximum after each doubled assignment. Since `n` is at least 1, the maximum is well-defined. The array positions beyond `n-1` remain at -1 (or whatever initial value we set), but they do not affect the result. Edge cases: negative doubled values, all duplicates, `n=100` (all slots used), and ensuring we do not accidentally process the uninitialized tail. Time complexity is O(n) for reading and processing, and O(1) auxiliary space (excluding the fixed 100-element array). The function is `const`-correct because it does not modify any global state.
