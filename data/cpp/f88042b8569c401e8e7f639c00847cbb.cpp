/*
Write a C++ function that reads a sequence of integers from standard input until the number `42` is encountered (without processing that number) and returns a string of the printed non-42 integers, but only those that appear before the first non-42 number if that first number is odd; if the first non-42 number is even, then the output string should be empty. In other words, the original logic prints all numbers until `42` is reached, but only if the very first number read (before `42`) is odd; if the first number is even, nothing is printed. The function should take no arguments and return a `std::string` containing each printed number followed by a newline (in the order read), or an empty string if nothing is printed. The input may contain any integers, including negative and zero; `42` is the terminator and is never included in the output. The function must use `std::cin` for input and must not read beyond the terminating `42`. If the input ends without a `42`, the behavior is undefined, so you may assume `42` always appears.
*/

#include <string>
#include <sstream>
#include <iostream>

// Reads integers from standard input until 42 is encountered.
// Returns a string with each non-42 integer followed by a newline,
// but only if the first non-42 integer is odd; otherwise returns empty.
std::string readUntil42() {
    std::ostringstream result;
    int num;

    // Read the first number. If it's 42, return empty immediately.
    if (!(std::cin >> num) || num == 42) {
        return "";
    }

    // If the first number is even, consume all inputs until 42 and return empty.
    if (num % 2 == 0) {
        while (std::cin >> num && num != 42) {
            // discard numbers
        }
        return "";
    }

    // First number is odd: append it and continue.
    result << num << "\n";

    // Read subsequent numbers until 42.
    while (std::cin >> num && num != 42) {
        result << num << "\n";
    }

    return result.str();
}

#include <cassert>
#include <sstream>
#include <string>

// Forward declaration of the function under test
std::string readUntil42();

int main() {
    // Test 1: First odd, then several numbers then 42
    std::istringstream input1("5 10 20 42");
    std::cin.rdbuf(input1.rdbuf());
    assert(readUntil42() == "5\n10\n20\n");

    // Test 2: First even, then numbers then 42 -> empty
    std::istringstream input2("4 1 2 3 42");
    std::cin.rdbuf(input2.rdbuf());
    assert(readUntil42() == "");

    // Test 3: First odd, immediately followed by 42
    std::istringstream input3("-7 42");
    std::cin.rdbuf(input3.rdbuf());
    assert(readUntil42() == "-7\n");

    // Test 4: First even (0), then nothing else before 42
    std::istringstream input4("0 42");
    std::cin.rdbuf(input4.rdbuf());
    assert(readUntil42() == "");

    // Test 5: First odd, then 42 immediately (single output)
    std::istringstream input5("9 42");
    std::cin.rdbuf(input5.rdbuf());
    assert(readUntil42() == "9\n");

    // Test 6: First even negative, then odd numbers, then 42
    std::istringstream input6("-2 5 7 42");
    std::cin.rdbuf(input6.rdbuf());
    assert(readUntil42() == "");

    // Test 7: First odd negative, then multiple numbers
    std::istringstream input7("-3 -4 0 5 42");
    std::cin.rdbuf(input7.rdbuf());
    assert(readUntil42() == "-3\n-4\n0\n5\n");

    // Test 8: First odd, then 42 appears immediately after first (already covered)
    // Test 9: Large positive odd first, then 42
    std::istringstream input9("12345 42");
    std::cin.rdbuf(input9.rdbuf());
    assert(readUntil42() == "12345\n");

    // Test 10: First odd 1, then numbers including 42 as terminator
    std::istringstream input10("1 2 3 42 4");
    std::cin.rdbuf(input10.rdbuf());
    assert(readUntil42() == "1\n2\n3\n");

    return 0;
}

// The core idea is to simulate the original `while(scanf(...))` loop but adapt it for a string-returning function. We need to read the first integer specially: if it equals `42`, the loop should terminate immediately and the output is empty. Otherwise, we check whether that first integer is odd (positive or negative; odd means `num % 2 != 0`). If odd, we start building the output string with that first number; if even, we must ignore all subsequent numbers until `42` and return an empty string. After processing the first number (if odd), we continue reading integers in a loop. For each subsequent integer, if it equals `42`, we break and return the accumulated string; otherwise, we append it to the output string with a newline. Edge cases: the first number could be `0` (which is even), negative odd numbers like `-3` are considered odd (since `-3 % 2 != 0` in C++ is `-1`), and a single non-42 number followed by `42` should produce that number if odd. Time complexity is O(k) where k is the number of integers read until `42` (including the terminator), and space complexity is O(k) for the output string.
