// Write a C++ function named `getEvenOddDescriptions` that takes no parameters and returns a `std::string` containing two lines: the first line lists all even integers from 0 to 30 inclusive, separated by single spaces, prefixed by the text "Четные числа от 1 до 30: "; the second line lists all odd integers from 1 to 29 inclusive, separated by single spaces, prefixed by the text "Нечетные числа от 1 до 30: ". Use a newline character (`\n`) between the two lines, and no trailing space after the last number in each line. The function must not print anything; it must return the formatted string. The output should be identical regardless of locale settings.
The solution builds the output string incrementally. Start with an empty string. For the even line, append the prefix "Четные числа от 1 до 30: " then loop with `for (int i = 0; i <= 30; i += 2)` and append each number followed by a space, but to avoid a trailing space, append a space before each number except the first one, or use a flag. Simpler: build each number with `std::to_string` and add a space separator only when not the last value (i.e., when `i < 30`). Then append a newline. For the odd line, similarly loop `for (int i = 1; i <= 29; i += 2)`, appending numbers separated by spaces, and add a newline at the end. The result is a single string. Complexity: the loop runs a fixed 16+15=31 iterations, so time is O(1) effectively, space is O(1) for temporary string building (the output length is constant). Edge cases: no parameters, so none; the only requirement is exact formatting with no trailing whitespace. The function must be `const`-correct? Since it takes no parameters, we can mark it `static` or just use a free function. Use `std::ostringstream` for clarity and correct spacing.
#include <string>
#include <sstream>

// Returns a formatted string listing even numbers 0..30 and odd numbers 1..29.
std::string getEvenOddDescriptions() {
    std::ostringstream out;
    
    out << "Четные числа от 1 до 30: ";
    for (int i = 0; i <= 30; i += 2) {
        if (i > 0) out << " ";
        out << i;
    }
    
    out << "\nНечетные числа от 1 до 30: ";
    for (int i = 1; i <= 29; i += 2) {
        if (i > 1) out << " ";
        out << i;
    }
    
    return out.str();
}
#include <cassert>
#include <string>

// The function under test is declared above (in Solution).
// Place this main in a separate file with the function included.

int main() {
    std::string result = getEvenOddDescriptions();
    
    // Expected exact string
    std::string expected = 
        "Четные числа от 1 до 30: 0 2 4 6 8 10 12 14 16 18 20 22 24 26 28 30\n"
        "Нечетные числа от 1 до 30: 1 3 5 7 9 11 13 15 17 19 21 23 25 27 29";
    
    assert(result == expected);
    
    // Verify no trailing spaces or extra newline at end
    assert(result.back() == '9');
    assert(result.find("  ") == std::string::npos); // no double spaces
    assert(result.find(" \n") == std::string::npos); // no space before newline
    
    // Check line count
    int newlines = 0;
    for (char c : result) if (c == '\n') ++newlines;
    assert(newlines == 1);
    
    // Check first character is the start of the even line
    assert(result.substr(0, 25) == "Четные числа от 1 до 30: ");
    
    return 0;
}
