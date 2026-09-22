Write a C++ function that reads a single character from an input stream (for example, `std::cin`) and returns the integer value of its ASCII code. The function should accept a `std::istream&` parameter to allow flexibility in testing with different input sources (e.g., `std::cin`, `std::istringstream`). The function must handle any printable ASCII character (including spaces, digits, letters, punctuation) and return an `int` between 0 and 127. The function should not print anything itself; it must only return the ASCII value. The input is guaranteed to contain exactly one character, possibly with surrounding whitespace which should be ignored (i.e., use the stream’s default `>>` extraction for a `char`, which skips leading whitespace). Assume the character is from the standard ASCII range (0–127); no error handling is required for out-of-range inputs.

// The core task is straightforward: use the stream extraction operator `>>` on a `char` variable, which automatically skips any leading whitespace (spaces, tabs, newlines) and reads the next non-whitespace character. After reading, simply cast that `char` to `int` (e.g., via `static_cast<int>`) to obtain its ASCII value. This works because `char` is an integral type in C++, and implicit or explicit conversion to `int` yields the character's numeric code in the execution character set (ASCII on virtually all modern systems). Edge cases include: (1) the character might be a space itself, but since `>>` skips leading whitespace, to read a space as the target character, the input stream would need to contain a space followed by another character; however, the problem guarantees exactly one character, so if the input is just a space, `>>` would fail—but the problem statement says "exactly one character" meaning a non-whitespace character, so this is not a concern. (2) The ASCII value is always non-negative for standard characters, so returning `int` is safe. Time complexity is O(1) since only one extraction is performed. Space complexity is O(1), using a single `char` variable.

#include <istream>

// Reads a single non-whitespace character from the given input stream
// and returns its ASCII integer value.
int charToAscii(std::istream& input) {
    char ch;
    input >> ch;  // Skips leading whitespace and reads the next character
    return static_cast<int>(ch);
}

#include <cassert>
#include <sstream>

// Global main function for testing the charToAscii function.
int main() {
    std::istringstream input1("A");
    assert(charToAscii(input1) == 65);

    std::istringstream input2("z");
    assert(charToAscii(input2) == 122);

    std::istringstream input3("5");
    assert(charToAscii(input3) == 53);

    std::istringstream input4(" ");
    // Note: A single space is skipped, so extraction fails, but the problem
    // guarantees exactly one character; this test handles the common case.
    // For a space, we would need to read raw, so we test with a real char.
    std::istringstream input4b("! ");
    assert(charToAscii(input4b) == 33); // Reads '!' then ignores the space

    std::istringstream input5("\n");
    // Newline is whitespace; the stream fails to read a char. We don't test this.
    // Instead test with a typical case:
    std::istringstream input6("\tH");
    assert(charToAscii(input6) == 72); // 'H' after tab

    std::istringstream input7("~");
    assert(charToAscii(input7) == 126);

    std::istringstream input8("0");
    assert(charToAscii(input8) == 48);

    std::istringstream input9(" ");
    // To handle a space character, use a different approach, but for this task
    // we rely on non-whitespace characters. So we test with ' ' via noskipws concept.
    // In practice, input >> ch skips spaces, so we don't assert on a sole space.
    // Instead, test another typical character:
    std::istringstream input10("a");
    assert(charToAscii(input10) == 97);

    // All tests passed
    return 0;
}
