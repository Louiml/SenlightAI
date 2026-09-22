/*
Write a C++ function named `reverseArrayInput` that reads a single positive integer `n` from standard input, followed by `n` integer values, and prints those values in reverse order, each value preceded by a single space, and ending with a newline. The function must handle the case where `n` may be zero (in which case it should print only a newline), and it must not store more than 100 integers (assume input constraints guarantee `n <= 100`). The output format must match exactly: for each reversed element, output a space then the number (e.g., for input `3 1 2 3`, output ` 3 2 1`). The function should be self-contained and not rely on global variables.
*/

#include <iostream>

// Reads n integers from standard input and prints them in reverse order,
// each preceded by a single space, followed by a newline.
void reverseArrayInput() {
    int n = 0;
    std::cin >> n;

    const int maxSize = 100;
    int arr[maxSize] = {0};  // Initialize to zero (though not strictly needed)

    for (int i = 0; i < n; ++i) {
        std::cin >> arr[i];
    }

    for (int i = n - 1; i >= 0; --i) {
        std::cout << " " << arr[i];
    }
    std::cout << '\n';
}

#include <cassert>
#include <sstream>
#include <iostream>

// Declare the solution function (normally this would be in a header)
void reverseArrayInput();

// Test harness: redirect cin to a stringstream and capture cout
int main() {
    // Test 1: Basic reverse with 3 elements
    {
        std::istringstream input("3 1 2 3\n");
        std::streambuf* origCin = std::cin.rdbuf(input.rdbuf());
        std::ostringstream output;
        std::streambuf* origCout = std::cout.rdbuf(output.rdbuf());

        reverseArrayInput();

        std::cin.rdbuf(origCin);
        std::cout.rdbuf(origCout);
        assert(output.str() == " 3 2 1\n");
    }

    // Test 2: Single element
    {
        std::istringstream input("1 42\n");
        std::streambuf* origCin = std::cin.rdbuf(input.rdbuf());
        std::ostringstream output;
        std::streambuf* origCout = std::cout.rdbuf(output.rdbuf());

        reverseArrayInput();

        std::cin.rdbuf(origCin);
        std::cout.rdbuf(origCout);
        assert(output.str() == " 42\n");
    }

    // Test 3: Zero elements (n=0)
    {
        std::istringstream input("0\n");
        std::streambuf* origCin = std::cin.rdbuf(input.rdbuf());
        std::ostringstream output;
        std::streambuf* origCout = std::cout.rdbuf(output.rdbuf());

        reverseArrayInput();

        std::cin.rdbuf(origCin);
        std::cout.rdbuf(origCout);
        assert(output.str() == "\n");
    }

    // Test 4: Duplicate values
    {
        std::istringstream input("4 5 5 9 5\n");
        std::streambuf* origCin = std::cin.rdbuf(input.rdbuf());
        std::ostringstream output;
        std::streambuf* origCout = std::cout.rdbuf(output.rdbuf());

        reverseArrayInput();

        std::cin.rdbuf(origCin);
        std::cout.rdbuf(origCout);
        assert(output.str() == " 5 9 5 5\n");
    }

    // Test 5: Negative numbers
    {
        std::istringstream input("3 -100 0 7\n");
        std::streambuf* origCin = std::cin.rdbuf(input.rdbuf());
        std::ostringstream output;
        std::streambuf* origCout = std::cout.rdbuf(output.rdbuf());

        reverseArrayInput();

        std::cin.rdbuf(origCin);
        std::cout.rdbuf(origCout);
        assert(output.str() == " 7 0 -100\n");
    }

    // Test 6: Large n (up to 100) to verify no overflow issue
    {
        std::string data = "100";
        for (int i = 0; i < 100; ++i) {
            data += " " + std::to_string(i);
        }
        data += "\n";
        std::istringstream input(data);
        std::streambuf* origCin = std::cin.rdbuf(input.rdbuf());
        std::ostringstream output;
        std::streambuf* origCout = std::cout.rdbuf(output.rdbuf());

        reverseArrayInput();

        std::cin.rdbuf(origCin);
        std::cout.rdbuf(origCout);

        std::string expected = "";
        for (int i = 99; i >= 0; --i) {
            expected += " " + std::to_string(i);
        }
        expected += "\n";
        assert(output.str() == expected);
    }

    return 0;
}

// The task is straightforward: read the count `n`, then read `n` integers into a fixed-size array (or directly into a container), then iterate from the last element to the first and print each with a leading space. The main algorithm uses a simple loop: after reading all `n` values, loop `i` from `n-1` down to `0` and print `" " << a[i]`. For `n = 0`, the loop body never executes, and the function prints just a newline, matching the expected format. Edge cases include `n = 1` where only one value is printed with a leading space, and duplicate values which require no special handling. The input is guaranteed to have exactly `n` valid integers. Time complexity is O(n) for reading and O(n) for output, so overall O(n). Space complexity is O(n) because we store at most 100 integers in a fixed-size array (or O(1) if we chose to process directly, but the standard approach uses an array). The fixed-size array is acceptable given the constraint `n <= 100`.
