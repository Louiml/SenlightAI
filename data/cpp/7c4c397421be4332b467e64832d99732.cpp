Write a C++ function named `isPalindrome` that takes a single `std::string` argument and returns a `bool` indicating whether the string is a palindrome. A palindrome reads the same forward and backward (e.g., "racecar", "abba", "a"). The function must handle strings of any length, including empty strings (which should be considered palindromes) and strings with an odd number of characters (where the middle character is ignored in comparison). The function should not modify the input string, and its implementation must use a `std::stack` to compare characters from the front and back halves. The function should be case-sensitive (e.g., "AbA" is not a palindrome) and should treat spaces and punctuation as ordinary characters (e.g., "a b a" is a palindrome because it reads "a b a" forward and backward, ignoring the fact that spaces are present). Provide a standalone solution that compiles and is testable via a separate `main` function in a test harness.

// The algorithm works by pushing the first half of the string’s characters onto a stack, then popping them in reverse order while comparing with the second half. For a string of length `n`, the number of characters to push is `n / 2` (integer division). If `n` is odd, the middle character (at index `n / 2`) is skipped because it does not need comparison. Then, starting from index `n / 2` (for even length) or `n / 2 + 1` (for odd length), each character is compared with the top of the stack; if any mismatch occurs, the function returns `false`. If the loop completes without mismatch, it returns `true`. Edge cases include: empty string (loop doesn’t execute, returns `true`), single character (`n / 2 = 0`, stack empty, but middle skipped? Actually for `n=1`, `n/2=0`, the condition `i == 0 && n%2 != 0` triggers `continue`, so no comparison, returns `true`). Two-character string: push index 0, compare with index 1. The algorithm is `O(n)` time and `O(n)` space (stack holds up to `n/2` elements).

#include <string>
#include <stack>

// Returns true if the input string is a palindrome, otherwise false.
// Uses a stack to compare the first half with the reversed second half.
// Handles empty strings and odd-length strings correctly. Case-sensitive.
bool isPalindrome(const std::string& str) {
    std::stack<char> charStack;
    const std::size_t length = str.length();
    const std::size_t half = length / 2;
    
    // Push first half characters onto stack
    for (std::size_t i = 0; i < half; ++i) {
        charStack.push(str[i]);
    }
    
    // Determine starting index for comparison (skip middle for odd length)
    std::size_t startIndex = (length % 2 == 0) ? half : half + 1;
    
    // Compare each character in the second half with the stack top
    for (std::size_t i = startIndex; i < length; ++i) {
        if (charStack.empty() || charStack.top() != str[i]) {
            return false;
        }
        charStack.pop();
    }
    
    // All comparisons passed, and stack should be empty
    return charStack.empty();
}

#include <cassert>
#include <string>

// Your solution function from above is assumed to be declared here
bool isPalindrome(const std::string& str);

int main() {
    // Basic cases
    assert(isPalindrome("racecar") == true);
    assert(isPalindrome("abba") == true);
    assert(isPalindrome("hello") == false);
    
    // Edge cases: empty and single character
    assert(isPalindrome("") == true);
    assert(isPalindrome("a") == true);
    
    // Odd length with middle character skipped
    assert(isPalindrome("abcba") == true);
    assert(isPalindrome("abcda") == false);
    
    // Case sensitivity and spaces/punctuation
    assert(isPalindrome("AbA") == false);
    assert(isPalindrome("a b a") == true);
    assert(isPalindrome("a!b!a") == true);
    assert(isPalindrome("ab") == false);
    
    return 0;
}
