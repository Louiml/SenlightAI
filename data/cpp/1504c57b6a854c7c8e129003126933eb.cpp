// Write a C++ function that reads a string containing multiple lines of text (each line terminated by `\n`) where a blank line (two consecutive newline characters) marks the end of the input. The function must store all characters from the input into a `std::string` (excluding the terminating blank line) and then return that string. The function should accept the input from a `std::istream` (e.g., `std::cin`) and return the collected text. The input may contain any printable ASCII characters including digits, lowercase/uppercase letters, spaces, and punctuation. The function must handle the case where the input ends immediately with a blank line (resulting in an empty string). Do not rely on any global state; the function must be self-contained and const-correct where applicable.

The main algorithm reads characters one at a time from the input stream using `.get()`. We maintain a `std::string` to store the accumulated characters. While looping, we read a character, append it to the string, then check if the previous character was a newline (`\n`). If we encounter a newline and the very next character is also a newline, that indicates a blank line—the end of input. At that point we stop reading and remove the trailing newline we just appended before the blank line (since the blank line itself should not be part of the output). Important edge cases: (1) If the input starts with a blank line (i.e., two consecutive newlines right away), the function should return an empty string. (2) If the input is non-empty but ends with a single newline followed by another newline (the terminator), the output should not include the last newline before the terminator. (3) We must avoid reading beyond the terminator. The algorithm takes O(n) time where n is the number of characters read (including the terminator), and O(1) extra space besides the returned string. The solution uses `std::istream&` parameter to allow flexibility for testing with `std::stringstream`.

#include <string>
#include <istream>

/**
 * Read text from an input stream until a blank line (two consecutive newlines)
 * is encountered. The blank line itself is not included in the returned string.
 * The function reads from the given stream and returns the accumulated text.
 * If the stream begins with a blank line, an empty string is returned.
 */
std::string readUntilBlankLine(std::istream& input) {
    std::string result;
    char current_char = '\0';
    char previous_char = '\0';

    // Read the first character if available
    if (input.get(current_char)) {
        previous_char = current_char;
        result.push_back(current_char);
    }

    while (input.get(current_char)) {
        // If we just saw a newline and now another newline, stop.
        if (previous_char == '\n' && current_char == '\n') {
            // Remove the first newline of the blank line from result.
            result.pop_back();  // removes the previous '\n' which was the last char before blank line
            break;
        }
        result.push_back(current_char);
        previous_char = current_char;
    }

    return result;
}

#include <iostream>
#include <sstream>
#include <cassert>

// The solution function is declared above; include it or copy here.
std::string readUntilBlankLine(std::istream& input);

int main() {
    // Test 1: Normal multi-line input ending with a blank line.
    {
        std::stringstream ss("Hello world\nThis is line 2\n\n");
        std::string result = readUntilBlankLine(ss);
        assert(result == "Hello world\nThis is line 2\n");
    }

    // Test 2: Input starts with a blank line -> empty result.
    {
        std::stringstream ss("\nRest of text\n");
        std::string result = readUntilBlankLine(ss);
        assert(result == "");
    }

    // Test 3: Single line ending with blank line.
    {
        std::stringstream ss("Only one line\n\n");
        std::string result = readUntilBlankLine(ss);
        assert(result == "Only one line\n");
    }

    // Test 4: No blank line at all (reads until EOF).
    {
        std::stringstream ss("abc\ndef\n");
        std::string result = readUntilBlankLine(ss);
        assert(result == "abc\ndef\n");
    }

    // Test 5: Empty stream (no characters).
    {
        std::stringstream ss("");
        std::string result = readUntilBlankLine(ss);
        assert(result == "");
    }

    // Test 6: Contains digits, uppercase, lowercase, and symbols.
    {
        std::stringstream ss("abc 123 XYZ !@#\n\n");
        std::string result = readUntilBlankLine(ss);
        assert(result == "abc 123 XYZ !@#\n");
    }

    // Test 7: Multiple blank lines - only first blank line stops reading.
    {
        std::stringstream ss("line1\nline2\n\nignored after blank\n");
        std::string result = readUntilBlankLine(ss);
        assert(result == "line1\nline2\n");
    }

    // Test 8: Tabs and spaces before the blank line.
    {
        std::stringstream ss("  spaced\n\ttabbed\n\n");
        std::string result = readUntilBlankLine(ss);
        assert(result == "  spaced\n\ttabbed\n");
    }

    // Test 9: Input that ends exactly at blank line without trailing newline after.
    {
        std::stringstream ss("data\n\n");
        std::string result = readUntilBlankLine(ss);
        assert(result == "data\n");
    }

    // Test 10: Long text with many lines.
    {
        std::stringstream ss("1\n2\n3\n4\n5\n\n");
        std::string result = readUntilBlankLine(ss);
        assert(result == "1\n2\n3\n4\n5\n");
    }

    std::cout << "All tests passed!\n";
    return 0;
}
