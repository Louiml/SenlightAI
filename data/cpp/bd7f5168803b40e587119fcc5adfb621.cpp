Write a C++ function that reads two integers from standard input (using `cin`) and returns the larger of the two. The function must not print anything to the console; instead, it should return the maximum value as an `int`. The input will always contain exactly two valid integers, and the integers can be negative, zero, or positive. The function should be independent of any `main` function, so it can be called from a test harness. The primary goal is to practice conditional logic (ternary operator or if-else) and input handling, while ensuring the function is reusable and side-effect-free.
// The solution is straightforward: read two integer values from standard input using `cin` into two variables, then compare them and return the larger. The main algorithm is a simple conditional check: if the first integer is greater than the second, return the first; otherwise, return the second. Edge cases include equal values (either can be returned), negative numbers, and integer overflow—though since we only compare and return one of the two inputs, no arithmetic is performed, so overflow is not an issue. Time complexity is \(O(1)\) because only two input operations and one comparison are performed. Space complexity is \(O(1)\) as only two integer variables are used. The function must correctly handle cases where the two inputs are equal, and it must not print anything, ensuring it is testable via assertions.
#include <iostream>

// Reads two integers from standard input and returns the larger one.
int getLargerFromInput() {
    int first, second;
    std::cin >> first >> second;
    return (first > second) ? first : second;
}
#include <cassert>
#include <sstream>
#include <iostream>

// Declare the function to test (assuming it's defined in the same translation unit).
int getLargerFromInput();

int main() {
    // Test 1: First is larger
    {
        std::istringstream input("10 5");
        std::cin.rdbuf(input.rdbuf());
        assert(getLargerFromInput() == 10);
    }
    
    // Test 2: Second is larger
    {
        std::istringstream input("3 8");
        std::cin.rdbuf(input.rdbuf());
        assert(getLargerFromInput() == 8);
    }
    
    // Test 3: Equal numbers
    {
        std::istringstream input("7 7");
        std::cin.rdbuf(input.rdbuf());
        assert(getLargerFromInput() == 7);
    }
    
    // Test 4: Negative numbers, first larger
    {
        std::istringstream input("-2 -5");
        std::cin.rdbuf(input.rdbuf());
        assert(getLargerFromInput() == -2);
    }
    
    // Test 5: Negative numbers, second larger
    {
        std::istringstream input("-9 -1");
        std::cin.rdbuf(input.rdbuf());
        assert(getLargerFromInput() == -1);
    }
    
    // Test 6: Zero and positive
    {
        std::istringstream input("0 4");
        std::cin.rdbuf(input.rdbuf());
        assert(getLargerFromInput() == 4);
    }
    
    // Test 7: Positive and zero
    {
        std::istringstream input("6 0");
        std::cin.rdbuf(input.rdbuf());
        assert(getLargerFromInput() == 6);
    }
    
    // Test 8: Large integers
    {
        std::istringstream input("2147483647 2147483646");
        std::cin.rdbuf(input.rdbuf());
        assert(getLargerFromInput() == 2147483647);
    }
    
    return 0;
}
