// Write a C++ function named `classifyPoints` that takes no parameters, reads an integer `t` from standard input, then reads `t` pairs of integers `(a, b)` from standard input, and returns a `std::string` containing five lines exactly in the following format: `"Q1: {count_Q1}\nQ2: {count_Q2}\nQ3: {count_Q3}\nQ4: {count_Q4}\nAXIS: {count_axis}"` (with the actual counts replacing placeholders). A point is in Q1 if both coordinates are positive, Q2 if `a < 0 && b > 0`, Q3 if both are negative, Q4 if `a > 0 && b < 0`, and on the axis if either coordinate is zero. The function should handle the input from `cin` exactly once and output nothing to stdout itself (only return the string). Assume `t` is a non-negative integer, and each pair consists of any integer values within the `int` range. Ensure the returned string uses `endl`-equivalent newline characters (i.e., `"\n"`).

#include <cassert>
#include <sstream>
#include <string>

// Declare the function (normally from header, but here we include the implementation).
// For testing, we need to redirect cin. We'll use a helper that sets cin to a stringstream.
// Since the solution function reads from cin, we must manipulate the stream buffer.

// Include the solution implementation here (for testing purposes, we copy it).
std::string classifyPoints() {
    int t;
    std::cin >> t;
    
    int q1 = 0, q2 = 0, q3 = 0, q4 = 0, axis = 0;
    
    for (int i = 0; i < t; ++i) {
        int a, b;
        std::cin >> a >> b;
        
        if (a == 0 || b == 0) {
            ++axis;
        } else if (a > 0 && b > 0) {
            ++q1;
        } else if (a < 0 && b > 0) {
            ++q2;
        } else if (a < 0 && b < 0) {
            ++q3;
        } else {
            ++q4;
        }
    }
    
    return "Q1: " + std::to_string(q1) + "\n" +
           "Q2: " + std::to_string(q2) + "\n" +
           "Q3: " + std::to_string(q3) + "\n" +
           "Q4: " + std::to_string(q4) + "\n" +
           "AXIS: " + std::to_string(axis);
}

// Helper to run classifyPoints with a given input string.
std::string runWithInput(const std::string& input) {
    std::istringstream iss(input);
    std::streambuf* old_buffer = std::cin.rdbuf(iss.rdbuf());
    std::string result = classifyPoints();
    std::cin.rdbuf(old_buffer);
    return result;
}

int main() {
    assert(runWithInput("0\n") == "Q1: 0\nQ2: 0\nQ3: 0\nQ4: 0\nAXIS: 0");
    assert(runWithInput("1\n1 1\n") == "Q1: 1\nQ2: 0\nQ3: 0\nQ4: 0\nAXIS: 0");
    assert(runWithInput("1\n-1 1\n") == "Q1: 0\nQ2: 1\nQ3: 0\nQ4: 0\nAXIS: 0");
    assert(runWithInput("1\n-1 -1\n") == "Q1: 0\nQ2: 0\nQ3: 1\nQ4: 0\nAXIS: 0");
    assert(runWithInput("1\n1 -1\n") == "Q1: 0\nQ2: 0\nQ3: 0\nQ4: 1\nAXIS: 0");
    assert(runWithInput("1\n0 5\n") == "Q1: 0\nQ2: 0\nQ3: 0\nQ4: 0\nAXIS: 1");
    assert(runWithInput("1\n3 0\n") == "Q1: 0\nQ2: 0\nQ3: 0\nQ4: 0\nAXIS: 1");
    assert(runWithInput("5\n1 1\n-1 1\n-1 -1\n1 -1\n0 0\n") == "Q1: 1\nQ2: 1\nQ3: 1\nQ4: 1\nAXIS: 1");
    assert(runWithInput("7\n1 2\n-3 4\n-5 -6\n7 -8\n0 1\n2 0\n0 0\n") == "Q1: 1\nQ2: 1\nQ3: 1\nQ4: 1\nAXIS: 3");
    return 0;
}

#include <string>
#include <iostream>

// Reads t points from standard input and returns a string with quadrant/axis counts.
std::string classifyPoints() {
    int t;
    std::cin >> t;
    
    int q1 = 0, q2 = 0, q3 = 0, q4 = 0, axis = 0;
    
    for (int i = 0; i < t; ++i) {
        int a, b;
        std::cin >> a >> b;
        
        if (a == 0 || b == 0) {
            ++axis;
        } else if (a > 0 && b > 0) {
            ++q1;
        } else if (a < 0 && b > 0) {
            ++q2;
        } else if (a < 0 && b < 0) {
            ++q3;
        } else { // a > 0 && b < 0
            ++q4;
        }
    }
    
    return "Q1: " + std::to_string(q1) + "\n" +
           "Q2: " + std::to_string(q2) + "\n" +
           "Q3: " + std::to_string(q3) + "\n" +
           "Q4: " + std::to_string(q4) + "\n" +
           "AXIS: " + std::to_string(axis);
}

// The solution reads the total number of points `t` first. Then, for each iteration, it reads a pair of integers. The classification uses simple conditional statements: if either coordinate is zero, increment `axis`; otherwise, compare signs to determine the quadrant. The key edge case is when `a` or `b` is exactly zero, which must be counted only in `axis` and not in any quadrant (even if the other coordinate is non-zero). Another edge case is when `t` is 0, meaning no pairs are read, and all counts remain zero. The time complexity is \(O(t)\) because we process each point exactly once, and the space complexity is \(O(1)\) since we only use a fixed number of integer counters and a string for output (the output string size is constant). The function must not print anything; it should build the result string using `std::to_string` for each count and concatenate with newline characters.
