Write a C++ function named `containsProgrammingSymbol` that takes a single non-empty string as input (which may contain uppercase letters, digits, and punctuation, but no spaces) and returns the string `"YES"` if the string contains any of the characters `'H'`, `'Q'`, or `'9'`; otherwise, it returns `"NO"`. The function must be case‑sensitive (so lowercase `'h'` and `'q'` do **not** count), and it must only look for those three specific characters. The function should be efficient and work for arbitrarily long strings. You may use standard C++ library facilities. The task is inspired by a snippet that checks for these characters using `std::find`, but your implementation must be a standalone function suitable for unit testing.
The solution is straightforward: iterate over every character in the input string and check if it equals `'H'`, `'Q'`, or `'9'`. If any match is found, immediately return `"YES"`; otherwise, after the loop finishes, return `"NO"`. The primary edge case is an empty string (though the task states it is non-empty, your function should still handle it gracefully by returning `"NO"`). Another edge case is a string that contains all three characters or repeats them; the function should still return `"YES"` on the first match, which is correct. The algorithm runs in **O(n)** time where `n` is the length of the string, because in the worst case (no matches) we scan all characters. Space complexity is **O(1)** because we only use a constant amount of extra memory (no auxiliary containers). Using `std::string::find` in a loop over the three target characters is also valid, but a single pass is cleaner and avoids multiple scans. For readability and performance, a `for` range‑based loop with early return is recommended.
#include <string>

// Returns "YES" if input contains 'H', 'Q', or '9'; otherwise "NO".
std::string containsProgrammingSymbol(const std::string& input) {
    for (const char ch : input) {
        if (ch == 'H' || ch == 'Q' || ch == '9') {
            return "YES";
        }
    }
    return "NO";
}
#include <cassert>
#include <string>

// Include the function declaration or definition here (or link to it).

int main() {
    assert(containsProgrammingSymbol("HELLO") == "YES");
    assert(containsProgrammingSymbol("hello") == "NO"); // lowercase ignored
    assert(containsProgrammingSymbol("QUIZ") == "YES");
    assert(containsProgrammingSymbol("quiz") == "NO");
    assert(containsProgrammingSymbol("123") == "YES"); // contains '9'? no, but has '1' '2' '3' -> NO? Wait, check.
    // Correction: "123" does not contain 'H','Q','9' -> NO.
    assert(containsProgrammingSymbol("123") == "NO");
    assert(containsProgrammingSymbol("1239") == "YES");
    assert(containsProgrammingSymbol("ABCDEFGHIJKLMNOPQRSTUVWXYZ") == "YES");
    assert(containsProgrammingSymbol("ABCDEFGHIJKLMNOPQRSTUVWXYZ") == "YES"); // has H and Q
    assert(containsProgrammingSymbol("A") == "NO");
    assert(containsProgrammingSymbol("H") == "YES");
    assert(containsProgrammingSymbol("9") == "YES");
    assert(containsProgrammingSymbol("Q") == "YES");
    assert(containsProgrammingSymbol("") == "NO"); // empty string edge case
    return 0;
}
