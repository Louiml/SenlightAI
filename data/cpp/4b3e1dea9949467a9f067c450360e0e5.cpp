/*
Write a C++ function that simulates a simple counter system based on a sequence of operation strings. The input consists of a positive integer `n` followed by `n` lines, where each line is one of four possible operation strings: `"++X"`, `"X++"`, `"--X"`, or `"X--"`. The `++` operations increment a counter by 1, and the `--` operations decrement it by 1. The counter starts at 0. Your function should read `n` and then the `n` operation strings from standard input, process them in order, and return the final value of the counter as an `int`. You may assume that the operation strings are exactly as specified, with no extra whitespace beyond the line breaks. Duplicate operations are allowed, and `n` can be as large as 10^5.
*/
#include <iostream>
#include <string>

// Processes a sequence of ++ or -- operations from standard input and returns the final counter value.
int computeFinalCounterValue() {
    int operationCount;
    std::cin >> operationCount;
    
    int counter = 0;
    std::string operation;
    
    for (int i = 0; i < operationCount; ++i) {
        std::cin >> operation;
        if (operation == "++X" || operation == "X++") {
            ++counter;
        } else if (operation == "--X" || operation == "X--") {
            --counter;
        }
    }
    
    return counter;
}
#include <cassert>
#include <iostream>
#include <sstream>

int main() {
    // Test 1: Basic increments only
    {
        std::istringstream input("3\n++X\nX++\n++X\n");
        std::cin.rdbuf(input.rdbuf());
        assert(computeFinalCounterValue() == 3);
    }
    
    // Test 2: Basic decrements only
    {
        std::istringstream input("2\n--X\nX--\n");
        std::cin.rdbuf(input.rdbuf());
        assert(computeFinalCounterValue() == -2);
    }
    
    // Test 3: Mixed operations
    {
        std::istringstream input("5\n++X\nX--\nX++\n--X\n++X\n");
        std::cin.rdbuf(input.rdbuf());
        assert(computeFinalCounterValue() == 1);
    }
    
    // Test 4: Single operation
    {
        std::istringstream input("1\nX++\n");
        std::cin.rdbuf(input.rdbuf());
        assert(computeFinalCounterValue() == 1);
    }
    
    // Test 5: Zero operations (edge case)
    {
        std::istringstream input("0\n");
        std::cin.rdbuf(input.rdbuf());
        assert(computeFinalCounterValue() == 0);
    }
    
    // Test 6: All decrements, larger value
    {
        std::istringstream input("4\n--X\nX--\n--X\nX--\n");
        std::cin.rdbuf(input.rdbuf());
        assert(computeFinalCounterValue() == -4);
    }
    
    // Test 7: Alternating operations
    {
        std::istringstream input("6\n++X\n--X\n++X\n--X\n++X\n--X\n");
        std::cin.rdbuf(input.rdbuf());
        assert(computeFinalCounterValue() == 0);
    }
    
    // Test 8: Ensure correct handling of large sequence (simulate 1000 increments)
    {
        std::string input = "1000\n";
        for (int i = 0; i < 1000; ++i) input += "++X\n";
        std::istringstream stream(input);
        std::cin.rdbuf(stream.rdbuf());
        assert(computeFinalCounterValue() == 1000);
    }
    
    // Test 9: Ensure correct handling of large negative sequence
    {
        std::string input = "1000\n";
        for (int i = 0; i < 1000; ++i) input += "X--\n";
        std::istringstream stream(input);
        std::cin.rdbuf(stream.rdbuf());
        assert(computeFinalCounterValue() == -1000);
    }
    
    // Test 10: Mixed random small sequence
    {
        std::istringstream input("7\n++X\nX--\n--X\nX++\n++X\nX--\n--X\n");
        std::cin.rdbuf(input.rdbuf());
        assert(computeFinalCounterValue() == -1);
    }
    
    std::cout << "All tests passed!" << std::endl;
    return 0;
}
// The solution is straightforward: initialize a counter `count` to 0. For each of the `n` input strings, check if the string contains a `'+'` (which indicates an increment operation — valid for both `"++X"` and `"X++"`) or a `'-'` (which indicates a decrement operation — valid for both `"--X"` and `"X--"`). Since each operation string is guaranteed to contain exactly two identical symbols (either two pluses or two minuses), checking the first character or any character works. For simplicity, we can check if the string equals `"++X"` or `"X++"` for increment, else decrement. After processing all strings, return the final counter value. Edge cases: if `n` is 0, the function returns 0; if all operations are increments, the counter grows positively, and if all are decrements, it grows negatively. The time complexity is O(n) because we process each string once; space complexity is O(1) because we only store the counter and the current string.
