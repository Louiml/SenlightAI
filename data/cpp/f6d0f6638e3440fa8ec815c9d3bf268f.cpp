Write a C++ function `bool isPalindrome(const std::string& s)` that determines whether a string is a valid palindrome after ignoring non-alphanumeric characters and case. A valid palindrome reads the same forward and backward when only letters and digits are considered, with uppercase and lowercase letters treated as equivalent. The input string may contain spaces, punctuation, digits, and letters. An empty string and a string with only one character are considered palindromes. The function should not modify the input and must handle arbitrary Unicode? (For this task, assume ASCII characters only.) Return `true` if the filtered string is a palindrome, otherwise `false`.

#include <cassert>

int main() {
    assert(isPalindrome("") == true);
    assert(isPalindrome("a") == true);
    assert(isPalindrome("A man, a plan, a canal: Panama") == true);
    assert(isPalindrome("race a car") == false);
    assert(isPalindrome("0P") == false);
    assert(isPalindrome("ab_a") == true);
    assert(isPalindrome(".,") == true);
    assert(isPalindrome("a b c d c b a") == true);
    assert(isPalindrome("1a2") == false);
    assert(isPalindrome("12321") == true);
    return 0;
}

#include <string>
#include <cctype>

// Determine if a string is a valid palindrome after ignoring non-alphanumeric characters and case.
bool isPalindrome(const std::string& s) {
    int left = 0;
    int right = static_cast<int>(s.size()) - 1;
    
    while (left < right) {
        // Skip non-alphanumeric characters from the left.
        while (left < right && !std::isalnum(static_cast<unsigned char>(s[left]))) {
            ++left;
        }
        // Skip non-alphanumeric characters from the right.
        while (left < right && !std::isalnum(static_cast<unsigned char>(s[right]))) {
            --right;
        }
        
        if (left < right) {
            if (std::tolower(static_cast<unsigned char>(s[left])) != 
                std::tolower(static_cast<unsigned char>(s[right]))) {
                return false;
            }
            ++left;
            --right;
        }
    }
    
    return true;
}

// The algorithm uses two iterators, one from the beginning (`left`) and one from the end (`right`) of the string. At each step, advance `left` forward while it points to a non-alphanumeric character, and advance `right` backward while it points to a non-alphanumeric character. Once both point to alphanumeric characters, compare them case-insensitively using `std::tolower`. If they differ, return `false` immediately. If they match, move both iterators inward and continue. Stop when the iterators cross (when `left > right`). Edge cases include empty strings, strings with only one character, strings with no alphanumeric characters (e.g., `"!!!..."`), and strings where all characters are alphanumeric. The time complexity is O(n) since each character is examined at most twice (once by each iterator), and the auxiliary space is O(1) because only a constant number of variables are used.
