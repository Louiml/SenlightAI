// Write a C++ function that reads exactly 7 integers from standard input (one per line, no prompts), stores them in an array in the order entered, and returns a single `std::string` containing the list of the numbers in the original order, formatted exactly as `"Número 1: X\nNúmero 2: Y\n...\nNúmero 7: Z\n"` (using newline separators and the numbering pattern shown). The function must handle any valid integer input, including negative values and zero, and must not print anything itself—the caller will handle output. For simplicity, assume the input always contains exactly 7 valid integers.

// The main algorithm is straightforward: use a fixed-size array of 7 integers, read each value with `std::cin >> array[i]` in a loop, then build a result string by appending each formatted line using `std::to_string` and manual newline characters. No special edge cases exist besides ensuring the loop runs exactly 7 times and that negative numbers are formatted correctly by `std::to_string`. The important detail is returning the string, not printing it, so the function is reusable and testable. Time complexity is \(O(7) = O(1)\) because the input size is fixed, and space complexity is \(O(1)\) for the array plus \(O(\text{output length})\) for the returned string, which is also constant (approx. 100 bytes). The implementation must include `<iostream>`, `<string>`, and `<array>` (or use a raw C array) and must correctly handle `const` only where appropriate—the array is modified during reading, so it cannot be `const` until after fill.

#include <iostream>
#include <string>

// Reads exactly 7 integers from standard input and returns a formatted listing string.
std::string formatSevenNumbers() {
    int numeros[7];

    for (int i = 0; i < 7; ++i) {
        std::cin >> numeros[i];
    }

    std::string result;
    for (int i = 0; i < 7; ++i) {
        result += "Número " + std::to_string(i + 1) + ": " + std::to_string(numeros[i]) + "\n";
    }

    return result;
}

#include <iostream>
#include <sstream>
#include <string>
#include <cassert>

// Include the solution function here (or link to it)
// #include "solution.h"

int main() {
    // Test 1: Basic positive numbers
    {
        std::istringstream input("1\n2\n3\n4\n5\n6\n7\n");
        std::streambuf* oldCin = std::cin.rdbuf(input.rdbuf());
        std::string result = formatSevenNumbers();
        std::cin.rdbuf(oldCin);
        assert(result == "Número 1: 1\nNúmero 2: 2\nNúmero 3: 3\nNúmero 4: 4\nNúmero 5: 5\nNúmero 6: 6\nNúmero 7: 7\n");
    }

    // Test 2: Negative numbers and zero
    {
        std::istringstream input("-5\n0\n-100\n7\n42\n-1\n999\n");
        std::streambuf* oldCin = std::cin.rdbuf(input.rdbuf());
        std::string result = formatSevenNumbers();
        std::cin.rdbuf(oldCin);
        assert(result == "Número 1: -5\nNúmero 2: 0\nNúmero 3: -100\nNúmero 4: 7\nNúmero 5: 42\nNúmero 6: -1\nNúmero 7: 999\n");
    }

    // Test 3: All same numbers
    {
        std::istringstream input("3\n3\n3\n3\n3\n3\n3\n");
        std::streambuf* oldCin = std::cin.rdbuf(input.rdbuf());
        std::string result = formatSevenNumbers();
        std::cin.rdbuf(oldCin);
        assert(result == "Número 1: 3\nNúmero 2: 3\nNúmero 3: 3\nNúmero 4: 3\nNúmero 5: 3\nNúmero 6: 3\nNúmero 7: 3\n");
    }

    // Test 4: Large values
    {
        std::istringstream input("2147483647\n-2147483648\n12345\n-12345\n0\n1\n-1\n");
        std::streambuf* oldCin = std::cin.rdbuf(input.rdbuf());
        std::string result = formatSevenNumbers();
        std::cin.rdbuf(oldCin);
        assert(result == "Número 1: 2147483647\nNúmero 2: -2147483648\nNúmero 3: 12345\nNúmero 4: -12345\nNúmero 5: 0\nNúmero 6: 1\nNúmero 7: -1\n");
    }

    std::cout << "All tests passed!\n";
    return 0;
}
