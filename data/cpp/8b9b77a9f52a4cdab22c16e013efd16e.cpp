Write a C++ function that takes a non-empty string as input and returns a new string formed by removing all consecutive duplicate characters, keeping only the first occurrence of each run of identical characters. The function should preserve the original order of remaining characters and handle single-character strings, strings with no duplicates, and strings containing repeated characters at the end. The comparison is case-sensitive, and whitespace characters are treated like any other character.
The solution iterates through the input string once, comparing each character to the next one. If a character is not equal to the next character, it is appended to the result. This effectively skips all characters that have a duplicate immediately following them, keeping only the last character of each run (which is the one that is not followed by an equal character). For example, in "aabbbcc", the characters at indices 0 ('a'), 3 ('b'), and 6 ('c') are appended because each is not followed by an identical character, yielding "abc". Edge cases include: an empty string (though specified non-empty, the function can return an empty string safely), a single character (returns the same character), a string where all characters are identical (returns just one character), and strings with trailing duplicates (handled because the loop checks `buf[i] != buf[i+1]` but only when `i+1` is valid—this is safe because C++ strings are null-terminated and accessing `buf[size]` returns '\0', which won't equal the last character unless the last character is also '\0' which is impossible). The main algorithm runs in O(n) time and O(1) auxiliary space (ignoring the output string), where n is the length of the input string.
#include <string>

// Removes consecutive duplicate characters from a string, keeping only one of each run.
std::string removeConsecutiveDuplicates(const std::string& input) {
    std::string result;
    const std::size_t size = input.size();
    
    for (std::size_t i = 0; i < size; ++i) {
        // Append if current char is not equal to the next one.
        // Accessing input[i+1] is safe because when i == size-1, input[size] is '\0' which is never equal to a printable char.
        if (i == size - 1 || input[i] != input[i + 1]) {
            result += input[i];
        }
    }
    return result;
}
#include <cassert>
#include <string>

// Declare the solution function (assumed to be included from above)
std::string removeConsecutiveDuplicates(const std::string& input);

int main() {
    // Basic cases
    assert(removeConsecutiveDuplicates("aabbcc") == "abc");
    assert(removeConsecutiveDuplicates("aaa") == "a");
    assert(removeConsecutiveDuplicates("abc") == "abc");
    assert(removeConsecutiveDuplicates("a") == "a");
    
    // Repeated at end
    assert(removeConsecutiveDuplicates("aaabbb") == "ab");
    assert(removeConsecutiveDuplicates("b") == "b");
    
    // Whitespace and mixed
    assert(removeConsecutiveDuplicates("  hello  ") == " helo ");
    assert(removeConsecutiveDuplicates("aabbAA") == "abA");
    assert(removeConsecutiveDuplicates("mississippi") == "misisipi");
    
    // Empty string (though not required, safe to handle)
    assert(removeConsecutiveDuplicates("") == "");
    
    return 0;
}
