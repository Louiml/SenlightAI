/*
Write a C++ function named `compress` that takes a string as input and returns a new string containing only every third character, starting with the first character (i.e., characters at indices 0, 3, 6, 9, ...). The function must preserve the original relative order of the selected characters. The input string may be empty, contain only one character, or contain any number of characters. The function should work for all ASCII strings, including those with spaces and special characters, and must not modify the input. Provide the function with appropriate `const` correctness for the parameter. The function signature should be `std::string compress(const std::string& x);`. Do not include a `main` function in your solution; only provide the function. The test harness will call the function directly with various inputs.
*/

#include <string>

// Return a string consisting of every third character from the input,
// starting with the first character (indices 0, 3, 6, ...).
std::string compress(const std::string& x) {
    std::string result;
    for (std::size_t i = 0; i < x.size(); i += 3) {
        result.push_back(x[i]);
    }
    return result;
}

#include <cassert>
#include <string>

// Assume compress is declared as above (include the header or paste the solution above).

int main() {
    assert(compress("ABCDEFGHIJKLMN") == "ADGJM");
    assert(compress("123456") == "14");
    assert(compress("HelloWorld") == "HloWl");
    assert(compress("BNK48") == "B4");
    assert(compress("COMPROG261102") == "COR22");
    assert(compress("A") == "A");
    assert(compress("AB") == "A");
    assert(compress("ABC") == "A");
    assert(compress("ABCD") == "AD");
    assert(compress("") == "");
    assert(compress("xy z") == "xz"); // space at index 2 is skipped, 'z' at index 3
    assert(compress("a!c@e#g") == "a@#"); // indices 0,3,6
    return 0;
}

// The core algorithm is straightforward: iterate over the input string using an index starting at 0, append the character at the current index to a result string, then increment the index by 3 each time until the index exceeds the string length. Because the step is fixed, the result will contain characters at positions 0, 3, 6, ... . Edge cases include: an empty string (which returns an empty string), a single character (returns that character), strings of length 2 or 3 (returns only the first character), and strings where the last selected index is exactly at the last character or slightly before. No special handling is needed for repeated characters or whitespace because they are just characters. Time complexity is \(O(n/3) = O(n)\), where \(n\) is the input length, since we visit approximately one third of the characters. Space complexity is \(O(n/3)\) for the result string, which is the standard output size; the auxiliary space (excluding the returned string) is \(O(1)\).
