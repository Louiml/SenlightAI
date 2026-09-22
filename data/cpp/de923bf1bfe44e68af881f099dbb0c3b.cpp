Write a C++ function named `categorizeByMax` that reads a series of test cases from an input stream, where each test case begins with an integer `p` (the number of elements in that test case, with `p >= 0`), followed by `p` integers. For each test case, the function must find the maximum value among the `p` integers (if `p == 0`, treat the maximum as `0`), and then output a category label on a single line: `1` if the maximum is less than `10`, `2` if the maximum is at least `10` but less than `20`, and `3` if the maximum is at least `20` (i.e., `>= 20`). The function should process the entire stream until end-of-input (EOF), and for each test case, print the result to `std::cout` followed by a newline. The function must not use global variables or rely on any input prompts. The input may contain extra whitespace, and the number of test cases is not fixed. The function signature should be: `void categorizeByMax(std::istream& in = std::cin, std::ostream& out = std::cout);` (default arguments allowed). Ensure the function handles large integers (within typical `int` range) and test cases with `p = 0` gracefully. The function must be self-contained, include all necessary headers, and use `const` correctness where applicable (e.g., for parameters that are not modified). Do not include a `main` function in the solution; only provide the free function.

#include <sstream>
#include <cassert>

int main() {
    // Test case 1: Basic inputs with varying categories
    std::istringstream input1("3 1 2 3\n2 5 15\n4 20 21 22 23\n1 10\n");
    std::ostringstream output1;
    categorizeByMax(input1, output1);
    assert(output1.str() == "1\n2\n3\n2\n");

    // Test case 2: p == 0 and negative numbers
    std::istringstream input2("0\n3 -5 -2 -1\n1 -100\n");
    std::ostringstream output2;
    categorizeByMax(input2, output2);
    assert(output2.str() == "1\n1\n1\n");

    // Test case 3: Edge boundaries: 9, 10, 19, 20
    std::istringstream input3("1 9\n1 10\n1 19\n1 20\n");
    std::ostringstream output3;
    categorizeByMax(input3, output3);
    assert(output3.str() == "1\n2\n2\n3\n");

    // Test case 4: Multiple test cases with extra whitespace and empty lines
    std::istringstream input4("\n 2  100  200 \n\n1 5 \n 0 \n");
    std::ostringstream output4;
    categorizeByMax(input4, output4);
    assert(output4.str() == "3\n1\n1\n");

    // Test case 5: All categories together
    std::istringstream input5("3 0 0 0\n3 10 10 10\n2 20 30\n");
    std::ostringstream output5;
    categorizeByMax(input5, output5);
    assert(output5.str() == "1\n2\n3\n");

    // Test case 6: Large p values (simulate by using a loop to build input)
    std::istringstream input6("5 1 2 3 4 5\n4 100 0 -100 200\n");
    std::ostringstream output6;
    categorizeByMax(input6, output6);
    assert(output6.str() == "1\n3\n");

    // Test case 7: Empty stream (no test cases)
    std::istringstream input7("");
    std::ostringstream output7;
    categorizeByMax(input7, output7);
    assert(output7.str().empty());

    // Test case 8: Single element per test case with max boundaries
    std::istringstream input8("1 11\n1 20\n1 9\n1 0\n");
    std::ostringstream output8;
    categorizeByMax(input8, output8);
    assert(output8.str() == "2\n3\n1\n1\n");

    // Test case 9: Mixed p values and duplicates
    std::istringstream input9("3 5 5 5\n2 19 19\n3 20 20 20\n");
    std::ostringstream output9;
    categorizeByMax(input9, output9);
    assert(output9.str() == "1\n2\n3\n");

    return 0;
}

#include <iostream>
#include <istream>
#include <ostream>

// Reads test cases from `in` until EOF. Each test case starts with an integer
// p (number of elements), followed by p integers. Outputs a category (1, 2, or 3)
// based on the maximum value among those p integers, per the rules:
//   max < 10  -> 1
//   10 <= max < 20 -> 2
//   max >= 20 -> 3
// If p == 0, max is treated as 0.
void categorizeByMax(std::istream& in = std::cin, std::ostream& out = std::cout) {
    int p;
    while (in >> p) {
        int maxElem = 0; // default for p == 0 or if all numbers are negative, max stays 0 unless a larger value appears
        for (int i = 0; i < p; ++i) {
            int n;
            in >> n;
            if (n > maxElem) {
                maxElem = n;
            }
        }
        if (maxElem < 10) {
            out << 1 << '\n';
        } else if (maxElem < 20) {
            out << 2 << '\n';
        } else {
            out << 3 << '\n';
        }
    }
}

// The main algorithm is straightforward: for each test case, read the count `p`, then read exactly `p` integers, tracking the maximum value encountered. Initialize `maxElem` to `0` when `p` is `0`, or to the first integer when `p > 0` (or simply initialize to `0` and update if any value is greater). After reading all integers, compare `maxElem` with thresholds: if `maxElem < 10` output `1`, else if `maxElem < 20` output `2`, else output `3`. The loop continues until EOF is reached on the input stream. Edge cases: `p == 0` means no integers follow, so the max should be considered `0`, which falls into category `1` (since `0 < 10`). Negative integers: if all values are negative, the maximum will be the least negative (closest to zero) or if `p` is `0`, max is `0`. The thresholds are based on the maximum value, so negative numbers will always produce category `1` unless the max is at least `10`. The time complexity is \(O(\text{total number of integers across all test cases})\), and space complexity is \(O(1)\) beyond the streaming buffers, since we only store the current max and a few loop variables.
