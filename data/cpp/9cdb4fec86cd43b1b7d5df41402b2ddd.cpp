Write a C++ function named `checkAtCoderString` that takes a non-empty string `s` as input and returns `"AC"` if the string follows the AtCoder (ABC) problem naming convention, and `"WA"` otherwise. The rules are: the string must start with exactly one uppercase `'A'`; the second character must be a lowercase letter (a-z); somewhere strictly between the first and last characters (i.e., positions 2 to len-2, 0-indexed), there must be exactly one uppercase `'C'`; all other characters in the string (except the leading `'A'` and that single `'C'`) must be lowercase letters (a-z), and the last character must also be a lowercase letter. The string length is at least 3.
The solution scans the string while validating each rule. First, we check that `s.front()` is `'A'`; if not, return `"WA"`. Then we verify `s[1]` is a lowercase letter by checking that its ASCII value is between `'a'` and `'z'`; if it is uppercase, return `"WA"`. Next, loop from index 2 to `s.size()-2` (inclusive) because the loop condition `i <= s.size()-2` correctly excludes the last character (index `s.size()-1`). Inside the loop, count occurrences of `'C'`; for any other character that is uppercase, set a flag to false. After the loop, check that the last character is lowercase. Finally, require that exactly one `'C'` was found and no other uppercase letters existed. If all conditions hold, return `"AC"`; otherwise `"WA"`.

Edge cases include: strings where the only `'C'` is at the start or end (should be WA because the single `'C'` must be strictly inside), strings with multiple `'C'`s, strings with other uppercase letters like `'B'` or `'Z'`, and short strings of length 3 where the loop range is empty (then `cnt` remains 0, so WA). The algorithm runs in `O(n)` time and uses `O(1)` extra space.
#include <string>

// Returns "AC" if s follows AtCoder ABC naming rules, otherwise "WA".
std::string checkAtCoderString(const std::string& s) {
    if (s.front() != 'A') {
        return "WA";
    }
    // s[1] must be lowercase
    if (s[1] < 'a' || s[1] > 'z') {
        return "WA";
    }
    
    bool ok = true;
    int cCount = 0;
    
    // Scan from index 2 up to s.size()-2 (inclusive), excluding first two and last char
    for (std::size_t i = 2; i + 1 < s.size(); ++i) {
        if (s[i] == 'C') {
            ++cCount;
        } else if (s[i] >= 'A' && s[i] <= 'Z') {
            ok = false;
        }
    }
    
    // Last character must be lowercase
    if (s.back() < 'a' || s.back() > 'z') {
        return "WA";
    }
    
    if (cCount != 1 || !ok) {
        return "WA";
    }
    
    return "AC";
}
#include <cassert>
#include <string>

// The solution function is declared here (included in the same translation unit)
std::string checkAtCoderString(const std::string& s);

int main() {
    assert(checkAtCoderString("AaaCaa") == "AC");
    assert(checkAtCoderString("abc") == "WA");  // must start with A
    assert(checkAtCoderString("Aac") == "WA");  // must have exactly one C inside
    assert(checkAtCoderString("Aaaaa") == "WA"); // no C
    assert(checkAtCoderString("ACaaa") == "WA"); // C at position 1 (second char)
    assert(checkAtCoderString("AaaCa") == "WA"); // C at last position
    assert(checkAtCoderString("AaaCaaC") == "WA"); // two C's
    assert(checkAtCoderString("AaaBaaa") == "WA"); // other uppercase B
    assert(checkAtCoderString("AaaCaaa") == "AC");
    assert(checkAtCoderString("AbC") == "AC"); // minimal length 3, C at index 2 (inside)
}
