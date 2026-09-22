// Write a C++ function that takes a `std::string` as input and returns a `bool` indicating whether the string is a palindrome (reads the same forwards and backwards). The function should ignore case sensitivity (i.e., treat 'A' and 'a' as equal) and ignore all non-alphabetic characters (spaces, punctuation, digits, etc.). For example, `"A man, a plan, a canal: Panama"` should be considered a palindrome, while `"race a car"` should not. If the string contains only non-alphabetic characters or is empty after filtering, consider it a palindrome. The function must be `const`-correct and use only standard library utilities.

The solution involves a two-pointer technique after preprocessing the input. First, iterate through the original string to build a new string containing only alphabetic characters (using `std::isalpha`), converting each to lowercase (using `std::tolower`). Then compare characters from both ends of the filtered string: initialize two indices `left = 0` and `right = filtered.size() - 1`, and while `left < right`, check if `filtered[left]` equals `filtered[right]`; if any mismatch occurs, return `false`. If the loop completes, return `true`. Edge cases include empty or all-non-alpha input (the filtered string is empty or length 1, which is trivially a palindrome), and mixed-case strings. The time complexity is O(n) for preprocessing and O(n) for comparison, giving overall O(n), where n is the length of the input string. Space complexity is O(n) for the filtered string.

#include <string>
#include <cctype>

// Check if a string is a palindrome ignoring case and non-alphabetic characters.
bool isPalindrome(const std::string& s) {
    std::string filtered;
    filtered.reserve(s.size());

    for (char ch : s) {
        if (std::isalpha(static_cast<unsigned char>(ch))) {
            filtered.push_back(std::tolower(static_cast<unsigned char>(ch)));
        }
    }

    int left = 0;
    int right = static_cast<int>(filtered.size()) - 1;

    while (left < right) {
        if (filtered[left] != filtered[right]) {
            return false;
        }
        ++left;
        --right;
    }

    return true;
}

#include <cassert>

int main() {
    assert(isPalindrome("A man, a plan, a canal: Panama") == true);
    assert(isPalindrome("race a car") == false);
    assert(isPalindrome("") == true);
    assert(isPalindrome("   ,.!?   ") == true);
    assert(isPalindrome("No 'x' in Nixon") == true);
    assert(isPalindrome("123321") == true);  // digits ignored, empty filtered => true
    assert(isPalindrome("AbBa") == true);
    assert(isPalindrome("Hello") == false);
    assert(isPalindrome("Madam, I'm Adam") == true);
    assert(isPalindrome("A") == true);
}
