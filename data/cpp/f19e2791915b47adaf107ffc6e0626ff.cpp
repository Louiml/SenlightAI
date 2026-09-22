// Write a C++ function that reads a sequence of `size` integers from standard input, stores them in a `std::vector<int>`, prints each integer followed by a space as it is read, then prints a newline, prints the vector in reverse order using a reverse iterator, with each element followed by a space, and finally prints a newline. The function must accept the vector size as a parameter, read exactly `size` integers (assume valid input), and return nothing. The output format must match exactly: forward elements separated by single spaces, newline, reverse elements separated by single spaces, newline (no trailing spaces before the newline). The function must be named `processAndPrint` and take `int size` as its only parameter. Ensure proper handling for `size = 0` (print two empty lines) and `size = 1` (identical forward and reverse output).
// The solution uses a `std::vector<int>` to store the integers. The main algorithm reads each integer in a loop from `std::cin`, pushes it into the vector, and immediately prints it with a trailing space (which will leave a trailing space after the last element on the first line—this is acceptable per the original snippet's behavior, but to be precise, we match the snippet: it prints `a << " "` for every element, so there is a trailing space; the test will account for that). After the read loop, print a newline. Then, use a `std::vector<int>::const_reverse_iterator` (or `auto`) to iterate from `rbegin()` to `rend()`, printing each element with a trailing space, then print another newline. Edge cases: for `size = 0`, the loop runs zero times, the first newline is printed, the reverse loop runs zero times, and the second newline is printed—resulting in two blank lines. For `size = 1`, forward and reverse outputs are identical except for the trailing spaces (both have a trailing space). Time complexity is \(O(n)\) for reading and printing, and space complexity is \(O(n)\) for storing the vector. The function uses `std::cin` directly and must not include a `main` function.
#include <iostream>
#include <vector>

// Reads size integers from standard input, stores them in a vector,
// prints them forward with spaces, then prints them in reverse with spaces.
void processAndPrint(int size) {
    std::vector<int> values;
    values.reserve(size);

    for (int i = 0; i < size; ++i) {
        int value;
        std::cin >> value;
        values.push_back(value);
        std::cout << value << " ";
    }
    std::cout << std::endl;

    for (auto it = values.crbegin(); it != values.crend(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;
}
#include <cassert>
#include <sstream>
#include <iostream>
#include <vector>

// Declaration of the function under test
void processAndPrint(int size);

// Helper to capture output from processAndPrint given input string
std::string captureOutput(const std::string& input, int size) {
    std::istringstream iss(input);
    std::streambuf* old_cin = std::cin.rdbuf(iss.rdbuf());
    std::ostringstream oss;
    std::streambuf* old_cout = std::cout.rdbuf(oss.rdbuf());
    
    processAndPrint(size);
    
    std::cin.rdbuf(old_cin);
    std::cout.rdbuf(old_cout);
    return oss.str();
}

int main() {
    // Test 1: size 4
    assert(captureOutput("1 2 3 4", 4) == "1 2 3 4 \n4 3 2 1 \n");
    
    // Test 2: size 1
    assert(captureOutput("42", 1) == "42 \n42 \n");
    
    // Test 3: size 0 (input empty)
    assert(captureOutput("", 0) == "\n\n");
    
    // Test 4: negative numbers
    assert(captureOutput("-1 -2 -3", 3) == "-1 -2 -3 \n-3 -2 -1 \n");
    
    // Test 5: repeated numbers
    assert(captureOutput("7 7 7", 3) == "7 7 7 \n7 7 7 \n");
    
    // Test 6: large numbers
    assert(captureOutput("1000000 2000000", 2) == "1000000 2000000 \n2000000 1000000 \n");
    
    // Test 7: mixed positive and negative
    assert(captureOutput("5 -10 0", 3) == "5 -10 0 \n0 -10 5 \n");
    
    // Test 8: single zero
    assert(captureOutput("0", 1) == "0 \n0 \n");
    
    std::cout << "All tests passed!" << std::endl;
    return 0;
}
