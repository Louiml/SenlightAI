// Given a string `s` consisting of uppercase letters, lowercase letters, and the special characters `'B'` and `'b'`, write a C++ function `processString` that simulates a text-editor-like deletion process. Starting from the end of the string and moving toward the beginning, every occurrence of `'B'` indicates a backspace that deletes the nearest preceding uppercase letter (i.e., any letter in `'A'..'Z'` that has not already been deleted or consumed by an earlier backspace). Similarly, every `'b'` deletes the nearest preceding lowercase letter (`'a'..'z'`). Uppercase backspaces only affect uppercase letters, and lowercase backspaces only affect lowercase letters; they cannot delete each other or delete other backspaces. All characters that are not deleted by any backspace (including the backspace characters themselves if they remain) are kept in their original relative order. The function should return the resulting string after processing all backspaces. For example, if `s = "aBcD"`, processing from right to left: `'D'` is kept, `'c'` is kept, `'B'` deletes the nearest uppercase letter to its left, which is `'a'`? No, `'a'` is lowercase, so `'B'` is not consumed and remains; then `'a'` is kept. The result is `"aBcD"`. If `s = "aAbB"` (note the final `'B'`), the rightmost `'B'` deletes the nearest uppercase to its left (`'A'`), leaving `"ab"`, then the `'b'` deletes the nearest lowercase to its left (`'a'`), leaving `"b"`. The function should handle uppercase only, lowercase only, mixed, and strings with no backspaces. Constraints: string length up to 10^5, and the function should run in O(n) time and O(n) auxiliary space.
#include <cassert>
#include <string>

// The solution function is declared above; include its definition before this main.

int main() {
    // No backspaces.
    assert(processString("HelloWorld") == "HelloWorld");

    // Lowercase backspace deletes preceding lowercase letters.
    assert(processString("abc") == "abc");
    assert(processString("abcb") == "ac"); // 'b' deletes the second 'b', leaving "ac"

    // Uppercase backspace deletes preceding uppercase letters.
    assert(processString("ABC") == "ABC");
    assert(processString("ABCB") == "AC"); // 'B' deletes the second 'B', leaving "AC"

    // Mixed case: backspaces only affect their own case.
    assert(processString("aBcD") == "aBcD"); // 'B' sees only lowercase 'a' before it, so remains.

    // Consecutive backspaces delete multiple letters.
    assert(processString("aab") == "a"); // two 'a's, then 'b' deletes one 'a'
    assert(processString("AABB") == ""); // both 'A's deleted by the two 'B's

    // Backspaces at the start (rightmost) delete nothing and remain.
    assert(processString("Bb") == "Bb");

    // Combined case.
    assert(processString("aAbBcC") == "abc"); // each backspace deletes an uppercase/lowercase.

    // Empty string.
    assert(processString("") == "");

    // Only backspaces, no letters to delete.
    assert(processString("BBbb") == "BBbb");

    return 0;
}
#include <string>
#include <algorithm>

// Process a string with 'B' (uppercase backspace) and 'b' (lowercase backspace).
// Returns the resulting string after deletions from right to left.
std::string processString(const std::string& s) {
    std::string result;
    int uppercaseBackspaces = 0;
    int lowercaseBackspaces = 0;

    // Scan from right to left.
    for (int i = static_cast<int>(s.size()) - 1; i >= 0; --i) {
        char ch = s[i];
        if (ch == 'B') {
            ++uppercaseBackspaces;
        } else if (ch == 'b') {
            ++lowercaseBackspaces;
        } else if (ch >= 'A' && ch <= 'Z' && uppercaseBackspaces > 0) {
            --uppercaseBackspaces;
        } else if (ch >= 'a' && ch <= 'z' && lowercaseBackspaces > 0) {
            --lowercaseBackspaces;
        } else {
            result.push_back(ch);
        }
    }

    // The result was built in reverse order.
    std::reverse(result.begin(), result.end());
    return result;
}
// The main algorithm is a single right-to-left scan using two counters: one for pending uppercase backspaces (`B_cnt`) and one for pending lowercase backspaces (`b_cnt`). When encountering `'B'`, increment `B_cnt`; when encountering `'b'`, increment `b_cnt`. When encountering an uppercase letter (`'A'..'Z'` but not `'B'`), if `B_cnt > 0`, decrement `B_cnt` (this letter is deleted); otherwise, append the character to the result. Similarly, for a lowercase letter (but not `'b'`), if `b_cnt > 0`, decrement `b_cnt`; otherwise, append it. Note that `'B'` and `'b'` themselves are not removed by their own backspaces (they only delete letters), but if a backspace appears and there is no letter to delete, it remains in the output. Edge cases: the string may contain only backspaces (they all remain), or backspaces at the end that delete no letters (they remain), and backspaces that delete letters that appear before them but not after them. The result is built in reverse order (since we scan right-to-left) and then reversed at the end. Time complexity is O(n) because each character is visited once, and the final reverse is O(n). Auxiliary space is O(n) for the result string.
