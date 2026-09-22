Write a C++ function named `splitEvenOddIndices` that takes a non-empty string of lowercase English letters and returns a string formed by first printing all characters at even indices (0, 2, 4, …) in their original order, followed by a single space, followed by all characters at odd indices (1, 3, 5, …) in their original order. The input will contain only letters (no spaces or digits), and the string length can be up to 10,000 characters. The function must handle both even and odd string lengths correctly, and for a string of length 1, the odd-index part should be empty (so the result is the single even character plus a trailing space). The output should preserve the exact original characters without sorting or modifying them.
// The solution is straightforward: iterate through the input string twice. In the first pass, collect every character where `index % 2 == 0` (i.e., 0, 2, 4, …) into a result string. In the second pass, collect every character where `index % 2 == 1` (i.e., 1, 3, 5, …) into the same result string but after appending a single space delimiter. For a string of length `n`, this approach always works regardless of whether `n` is even or odd. Edge cases: when `n == 1`, the first loop adds the single character, then we add a space, and the second loop adds nothing—producing e.g., `"a "` (with trailing space). When `n == 0` is not allowed per the task specification (non-empty), but if it were, we would produce `" "`; we can optionally guard against empty input by returning an empty string, though the specification says non-empty. The time complexity is O(n) because each character is visited exactly once across both loops. The auxiliary space complexity is O(n) for the returned string, plus O(1) for loop counters and the delimiter constant. No special library functions beyond standard string operations are required.
#include <string>

// Return a string with characters at even indices first, then a space, then characters at odd indices.
// Precondition: input is non-empty and contains only lowercase letters.
std::string splitEvenOddIndices(const std::string& input) {
    std::string result;
    const std::size_t n = input.size();

    // Append characters at even indices (0, 2, 4, ...)
    for (std::size_t i = 0; i < n; i += 2) {
        result.push_back(input[i]);
    }

    // Append a space delimiter
    result.push_back(' ');

    // Append characters at odd indices (1, 3, 5, ...)
    for (std::size_t i = 1; i < n; i += 2) {
        result.push_back(input[i]);
    }

    return result;
}
#include <cassert>
#include <string>

// Function declaration matching the solution
std::string splitEvenOddIndices(const std::string& input);

int main() {
    // Basic case: even length
    assert(splitEvenOddIndices("abcd") == "ac bd");

    // Odd length
    assert(splitEvenOddIndices("abcde") == "ace bd");

    // Single character (odd part empty, trailing space)
    assert(splitEvenOddIndices("z") == "z ");

    // Two characters
    assert(splitEvenOddIndices("ab") == "a b");

    // All same characters
    assert(splitEvenOddIndices("aaaa") == "aa aa");

    // Longer mixed string
    assert(splitEvenOddIndices("hello") == "hlo el");

    // String of length 3
    assert(splitEvenOddIndices("xyz") == "xz y");

    // String of length 6
    assert(splitEvenOddIndices("abcdef") == "ace bdf");

    // Repeating pattern
    assert(splitEvenOddIndices("abcabc") == "acac bb");

    // String with length 7
    assert(splitEvenOddIndices("abcdefg") == "aceg bdf");

    return 0;
}
