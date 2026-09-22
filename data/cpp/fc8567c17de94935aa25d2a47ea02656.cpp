/*
Write a C++ function named `letterPercentage` that takes a non-empty string `s` and a single character `letter` as parameters, and returns the integer percentage of characters in `s` that are equal to `letter`. The percentage must be computed by dividing the count of matching characters by the total length of the string, multiplying by 100, and then truncating any fractional part (i.e., using integer division style truncation). The input string may contain uppercase and lowercase letters, digits, spaces, punctuation, and any other printable ASCII characters; the comparison must be case-sensitive. The string is guaranteed to be non-empty, so no division by zero occurs. The function should be `const`-correct, not modify the input string, and handle a string of length 1 correctly (returning either 0 or 100 depending on whether the single character matches). The function should have a descriptive name and be self-contained with only necessary standard headers.
*/

#include <string>

// Returns the integer percentage of characters in s that equal letter.
// The percentage is truncated, not rounded.
int letterPercentage(const std::string& s, char letter) {
    int count = 0;
    for (char c : s) {
        if (c == letter) {
            ++count;
        }
    }
    // s is non-empty, so division is safe. Integer multiplication/division truncates automatically.
    return (count * 100) / static_cast<int>(s.size());
}

#include <cassert>

int main() {
    assert(letterPercentage("hello", 'l') == 40);
    assert(letterPercentage("hello", 'h') == 20);
    assert(letterPercentage("hello", 'z') == 0);
    assert(letterPercentage("a", 'a') == 100);
    assert(letterPercentage("a", 'A') == 0);
    assert(letterPercentage("aaaa", 'a') == 100);
    assert(letterPercentage("abab", 'a') == 50);
    assert(letterPercentage("Hello World", 'o') == 18); // 2/11 = 18.18 -> truncate to 18
    assert(letterPercentage("12345", '3') == 20);
    assert(letterPercentage("!!!", '!') == 100);
}

// The solution is straightforward: iterate through each character of the input string using a range-based for loop, counting how many times the target character appears. Since the string is non-empty, we can safely compute the percentage as `(count * 100) / s.size()` using integer arithmetic, which automatically truncates any fractional part toward zero. Alternatively, using floating-point multiplication then casting to `int` also truncates, but integer arithmetic is more efficient and avoids floating-point precision issues. The main edge case is a string of length 1: if the single character matches, the count is 1, and `(1 * 100) / 1 = 100`; if not, it is `(0 * 100) / 1 = 0`. The comparison is case-sensitive, so `'a'` does not match `'A'`. The time complexity is \(O(n)\), where \(n\) is the length of the string, since we scan each character exactly once. The space complexity is \(O(1)\) as we only use a few integer variables.
