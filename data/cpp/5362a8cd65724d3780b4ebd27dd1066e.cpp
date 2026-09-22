// Write a C++ function named `helloSubsequence` that takes a single `std::string` as input and returns a `bool` indicating whether the string contains the characters 'h', 'e', 'l', 'l', 'o' as a subsequence in that exact order (not necessarily consecutively). The function should return `true` if such a subsequence exists, and `false` otherwise. The input string may contain any printable ASCII characters, including uppercase letters, digits, punctuation, and spaces. The function must be case-sensitive and must handle empty strings.

#include <cassert>
#include <string>

// Assume the solution function is declared above.
bool helloSubsequence(const std::string& input);

int main() {
    // Basic true cases
    assert(helloSubsequence("hello") == true);
    assert(helloSubsequence("hlelo") == true);
    assert(helloSubsequence("h e l l o") == true);
    assert(helloSubsequence("xhyexllxlo") == true);
    assert(helloSubsequence("Hello") == false); // case-sensitive, 'H' is not 'h'

    // Basic false cases
    assert(helloSubsequence("") == false);
    assert(helloSubsequence("helo") == false); // missing second 'l'
    assert(helloSubsequence("olleh") == false); // reversed order
    assert(helloSubsequence("hxexlx") == false); // no 'o' after 'l's
    assert(helloSubsequence("hhheeelllooo") == true); // extra duplicates fine
    assert(helloSubsequence("ahbcefd") == false); // incomplete

    return 0;
}

#include <string>

// Returns true if the input string contains "hello" as a subsequence.
bool helloSubsequence(const std::string& input) {
    const std::string target = "hello";
    size_t targetIndex = 0;

    for (char ch : input) {
        if (ch == target[targetIndex]) {
            ++targetIndex;
            if (targetIndex == target.size()) {
                return true;
            }
        }
    }
    return false;
}

// The solution uses a simple scanning approach: maintain an index into the target word "hello". Iterate through each character of the input string. Whenever the current character matches the character at the current target position, advance the target index. If the target index reaches 5 (the length of "hello"), we have found the complete subsequence and can return `true` immediately. If the loop finishes without reaching the end of the target word, return `false`. 
//
// Edge cases include an empty input string (immediately `false`), a string shorter than 5 characters (cannot contain the subsequence), duplicate characters (e.g., many 'l's before an 'o' are fine), and letters appearing out of order. Since we only advance on exact character matches, case sensitivity is naturally handled. 
//
// Time complexity is \(O(n)\) where \(n\) is the length of the input string, because we process each character once. Space complexity is \(O(1)\) because we only store a few integer indices and the fixed target string constant.
