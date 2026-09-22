/*
Write a C++ function that takes a string `input` as its only parameter and returns a new string containing a triangular pattern of the input's characters, where the first line contains the first character, the second line contains the first two characters, and so on, up to a final line containing the entire input. Each line in the returned string must end with a newline character (`'\n'`), and the function should handle empty strings by returning an empty string. The function must preserve the original character order and not add any extra spaces or characters.
*/

#include <string>

// Returns a triangular pattern of the input string, one prefix per line, each line ending with '\n'.
std::string buildTriangle(const std::string& input) {
    if (input.empty()) {
        return "";
    }

    std::string result;
    const std::size_t length = input.length();

    for (std::size_t i = 0; i < length; ++i) {
        result += input.substr(0, i + 1);
        result += '\n';
    }

    return result;
}

#include <cassert>
#include <string>

// Include the function definition here or via a header.
std::string buildTriangle(const std::string& input);

int main() {
    assert(buildTriangle("") == "");
    assert(buildTriangle("a") == "a\n");
    assert(buildTriangle("ab") == "a\nab\n");
    assert(buildTriangle("abc") == "a\nab\nabc\n");
    assert(buildTriangle("hello") == "h\nhe\nhel\nhell\nhello\n");
    assert(buildTriangle("A1") == "A\nA1\n");
    assert(buildTriangle("xyz!") == "x\nxy\nxyz\nxyz!\n");
    assert(buildTriangle("12345") == "1\n12\n123\n1234\n12345\n");
    // Ensure no extra trailing characters or missing newlines
    std::string result = buildTriangle("test");
    assert(result.length() == 10); // 1+2+3+4 = 10 chars + 4 newlines = 14? Wait: 't'=1, 'te'=2, 'tes'=3, 'test'=4 => 10 chars + 4 newlines = 14, but check logic: actually 1+2+3+4 = 10 characters and 4 newlines => 14 total length.
    // Let's correct: length = 10 characters + 4 newlines = 14.
    assert(result.length() == 14);
    assert(result.back() == '\n');
    return 0;
}

// The solution iterates over each index `i` from 0 to `length-1` and for each index appends a substring from position 0 to `i` (inclusive) to the result string, followed by a newline. The substring can be obtained using `input.substr(0, i+1)` or by building it character by character with a nested loop. The main edge case is an empty input, which should return an empty string without producing any newline. Another edge case is a single-character input, which produces exactly one line containing that character plus a newline. The algorithm runs in \(O(n^2)\) time because the total number of characters output is the sum of the first `n` integers, which is \(n(n+1)/2\). The space complexity is \(O(n^2)\) for the returned string itself, plus \(O(1)\) auxiliary space if we use a string builder approach with `+=` (since the string grows internally).
