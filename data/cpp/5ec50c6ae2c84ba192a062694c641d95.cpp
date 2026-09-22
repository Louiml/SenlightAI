/*
Write a standalone C++ function named `compareNumbersReversed` that reads two three-digit positive integers from standard input (each digit guaranteed to be non-zero, so no digit is `'0'`), compares them digit-by-digit starting from the hundreds place (index 0) down to the units place (index 2), and determines which number is “larger” according to this reversed lexicographic comparison (i.e., compare hundreds first, then tens, then units). The function must then output the digits of the larger number in reverse order (units, tens, hundreds) as a string without any separators. If the two numbers are identical digit-wise (which can happen since all digits are non‑zero), output the reversed digits of the first number. The function should take no parameters, read from `cin`, and return the resulting string. The main program already exists in the provided snippet; your task is to implement the function to replace the logic in the main body. The function must handle exactly two three-digit inputs, each with all digits in the range `'1'`–`'9'`. The output must be produced by a separate `main` that calls this function and prints the returned string, but in your solution you should only provide the function itself.
*/

#include <string>

// Reads two three-digit numbers (each digit '1'..'9'), compares them
// digit by digit from the most significant digit (hundreds) to least
// significant (units), and returns the reversed digits of the larger.
// If equal, returns the reversed digits of the first number.
std::string compareNumbersReversed() {
    std::string first, second;
    std::cin >> first >> second;

    // Determine which string is "larger" by comparing from index 0.
    std::string larger = first;  // default if equal
    for (int i = 0; i < 3; ++i) {
        if (first[i] > second[i]) {
            larger = first;
            break;
        } else if (first[i] < second[i]) {
            larger = second;
            break;
        }
        // If equal, continue to next digit; if all equal, larger stays first.
    }

    // Build the reversed string: units, tens, hundreds.
    std::string result;
    result.reserve(3);
    for (int i = 2; i >= 0; --i) {
        result.push_back(larger[i]);
    }
    return result;
}

#include <cassert>
#include <sstream>

// Declaration of the function under test (must match the solution).
std::string compareNumbersReversed();

int main() {
    // Redirect cin to test various inputs.
    std::string input;
    std::string expected;

    // Test 1: first number larger at hundreds.
    input = "912 899\n";
    std::istringstream iss1(input);
    std::cin.rdbuf(iss1.rdbuf());
    expected = "219";  // 912 reversed
    assert(compareNumbersReversed() == expected);

    // Test 2: second number larger at tens.
    input = "129 189\n";
    std::istringstream iss2(input);
    std::cin.rdbuf(iss2.rdbuf());
    expected = "981";  // 189 reversed
    assert(compareNumbersReversed() == expected);

    // Test 3: second number larger at units.
    input = "123 124\n";
    std::istringstream iss3(input);
    std::cin.rdbuf(iss3.rdbuf());
    expected = "421";  // 124 reversed
    assert(compareNumbersReversed() == expected);

    // Test 4: identical numbers (choose first).
    input = "555 555\n";
    std::istringstream iss4(input);
    std::cin.rdbuf(iss4.rdbuf());
    expected = "555";
    assert(compareNumbersReversed() == expected);

    // Test 5: first larger at hundreds and units differ.
    input = "999 100\n";
    std::istringstream iss5(input);
    std::cin.rdbuf(iss5.rdbuf());
    expected = "999";  // 999 reversed
    assert(compareNumbersReversed() == expected);

    // Test 6: second larger at hundreds.
    input = "211 912\n";
    std::istringstream iss6(input);
    std::cin.rdbuf(iss6.rdbuf());
    expected = "219";  // 912 reversed
    assert(compareNumbersReversed() == expected);

    // Test 7: all digits non-zero, check random case.
    input = "731 739\n";
    std::istringstream iss7(input);
    std::cin.rdbuf(iss7.rdbuf());
    expected = "937";  // 739 reversed
    assert(compareNumbersReversed() == expected);

    // Test 8: second larger at tens even if hundreds are equal.
    input = "123 153\n";
    std::istringstream iss8(input);
    std::cin.rdbuf(iss8.rdbuf());
    expected = "351";  // 153 reversed
    assert(compareNumbersReversed() == expected);

    return 0;
}

// The core idea is to read both strings (each of length 3) and then simulate the loop in the original snippet. We iterate from index 0 (hundreds) to index 2 (units) and break at the first position where the characters differ; the number with the greater character at that position is chosen. If no difference is found, the two numbers are identical, and we arbitrarily choose the first one. After determining the “larger” string, we construct the output by iterating from index 2 down to 0 and appending each character to a result string. Edge cases include identical numbers (all characters equal) and a difference at the first character (in which case the rest is irrelevant). The time complexity is O(3) = O(1) because the strings are fixed length, and space complexity is O(1) beyond the input and output strings.
