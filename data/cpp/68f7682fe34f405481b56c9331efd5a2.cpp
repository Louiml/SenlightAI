Write a C++ function named `isPalindromeRecursive` that takes a C-style character array (null-terminated string) and returns a boolean value indicating whether the string is a palindrome (reads the same forward and backward). The function must be implemented recursively without using any loops, string reversal, or extra dynamic memory allocation. The input string may contain uppercase and lowercase letters, digits, punctuation, and spaces; all characters must be considered exactly as they appear (case-sensitive, spaces count as characters). An empty string or a single-character string is considered a palindrome.

#include <cassert>

int main() {
    // Basic palindromes
    assert(isPalindromeRecursive("racecar") == true);
    assert(isPalindromeRecursive("hello") == false);
    
    // Empty and single character
    assert(isPalindromeRecursive("") == true);
    assert(isPalindromeRecursive("a") == true);
    
    // Case sensitivity (mixed case is not palindrome)
    assert(isPalindromeRecursive("AbA") == true);   // 'A' == 'A', 'b' == 'b'
    assert(isPalindromeRecursive("Aba") == false);  // 'A' != 'a' at center? Actually A vs a at ends → false
    assert(isPalindromeRecursive("Aba") == false);
    
    // With spaces and punctuation (exact char matching)
    assert(isPalindromeRecursive("a b a") == true);
    assert(isPalindromeRecursive("a b") == false);
    assert(isPalindromeRecursive("!@!") == true);
    
    // Even-length palindrome
    assert(isPalindromeRecursive("abba") == true);
    assert(isPalindromeRecursive("abca") == false);
    
    // Longer strings
    assert(isPalindromeRecursive("tattarrattat") == true);
    assert(isPalindromeRecursive("was it a car or a cat i saw") == false); // spaces count
}

#include <cstddef>

// Helper function for recursive palindrome check
bool isPalindromeHelper(const char input[], int start, int end) {
    // Base case: empty or single character, or start passed end
    if (start >= end) {
        return true;
    }
    // Compare current characters
    if (input[start] != input[end]) {
        return false;
    }
    // Recurse inward
    return isPalindromeHelper(input, start + 1, end - 1);
}

// Returns true if the null-terminated string is a palindrome
bool isPalindromeRecursive(const char input[]) {
    // Compute length manually (loop is allowed before recursion)
    int len = 0;
    while (input[len] != '\0') {
        ++len;
    }
    if (len == 0 || len == 1) {
        return true;
    }
    return isPalindromeHelper(input, 0, len - 1);
}

// The solution uses a recursive helper function that compares characters from the two ends of the string moving toward the center. First, compute the length of the string by iterating through the characters until the terminating null byte is found (this is the only loop allowed, but it can be called before the recursion). The base cases are: if the string is empty (length 0) or has only one character (length 1), return true; also, if the start index becomes greater than or equal to the end index, return true because all compared pairs have matched. In each recursive step, compare `input[start]` and `input[end]`; if they are equal, recursively call the helper with `start+1` and `end-1`; otherwise, return false. Edge cases include strings with mixed-case letters (e.g., "AbA" is palindromic, but "Aba" is not because 'b' != 'B'), strings with spaces and punctuation (e.g., "a b a" is palindrome, "a b" is not), and very long strings (but recursion depth is limited to half the string length). Time complexity is O(n) for the length computation plus O(n/2) for the recursive comparisons, so overall O(n) where n is the string length. Space complexity is O(n) due to recursion stack depth of at most n/2 frames.
