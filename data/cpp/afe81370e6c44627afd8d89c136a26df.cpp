Write a C++ function that takes a non-empty string and two integer parameters (a start position and a substring to insert), and returns a new string where the substring is inserted at the given position after first erasing exactly three characters starting from that same position (if those characters exist). If fewer than three characters remain from the start position, erase only the remaining characters. The function must preserve the original input string (i.e., not modify it) and handle edge cases such as insertion at the end of the string, insertion beyond the string length (insert at the end), and negative start positions (treat as 0). The returned string should be the modified result.

// The core operation is: given position `pos` and substring `sub`, first erase `min(3, str.size() - pos)` characters starting at `pos` (if `pos >= str.size()`, erase nothing), then insert `sub` at the original `pos` (clamped to `[0, str.size()]`, because after erasing, the size shrinks but the insertion index is still the original position, which may now be beyond the new size; in that case insert at the end). To avoid modifying the input, we copy the input into a local `std::string`. We clamp `pos` to `[0, str.size()]` before erasing: if `pos < 0` set to 0; if `pos > str.size()` set to `str.size()`. Then compute `eraseCount = min(3, (int)str.size() - pos)`, call `erase(pos, eraseCount)`, then `insert(pos, sub)` (if `pos` > new size, `std::string::insert` will throw; so we clamp `pos` to the new size before inserting). Edge cases: empty input? The task says non-empty, but we can still handle gracefully. Complexity: O(n + m) where n is original length and m is substring length, due to copying and shifting. Time is linear, space is O(n + m) for the returned string.

#include <string>
#include <algorithm>

// Insert a substring at a given position after erasing up to three characters
// starting at that same position. The original string is not modified.
std::string insertAfterEraseThree(const std::string& input, int pos, const std::string& sub) {
    std::string result = input;
    if (result.empty()) {
        // If empty, just insert at the beginning
        return sub;
    }

    // Clamp pos to [0, size()]
    size_t p = static_cast<size_t>(std::clamp(pos, 0, static_cast<int>(result.size())));

    // Erase up to three characters (or fewer if not enough remain)
    size_t eraseCount = std::min<size_t>(3, result.size() - p);
    result.erase(p, eraseCount);

    // After erasing, the insertion position may be beyond the new size.
    // Insert at the end if p > result.size()
    p = std::min(p, result.size());
    result.insert(p, sub);

    return result;
}

#include <cassert>
#include <string>

// (Declaration of the function from the solution is assumed above)

int main() {
    // Basic case from snippet: original "qazxs wedcvf rtgbnhyujm kiooopl"
    // find "wed" at position 6, insert "lol" after erasing 3 chars at position 6
    std::string s = "qazxs wedcvf rtgbnhyujm kiooopl";
    std::string expected = "qazxs lolecvf rtgbnhyujm kiooopl";
    assert(insertAfterEraseThree(s, 6, "lol") == expected);

    // Erase fewer than 3 if near end
    assert(insertAfterEraseThree("abc", 1, "X") == "aXc"); // erase "bc" (2 chars), then insert X

    // Insert at end (pos == size)
    assert(insertAfterEraseThree("hello", 5, "!") == "hello!");

    // Pos beyond size -> insert at end
    assert(insertAfterEraseThree("hey", 10, "Z") == "heyZ");

    // Negative pos -> treat as 0
    assert(insertAfterEraseThree("abcd", -3, "YY") == "YYd"); // erase "abc" (3 chars at pos 0), insert YY

    // Exactly 3 chars erased
    assert(insertAfterEraseThree("abcdef", 2, "12") == "ab12f");

    // Empty substring
    assert(insertAfterEraseThree("xyz", 0, "") == ""); // erase all three, insert nothing

    // Original input unchanged
    std::string orig = "qazxs wedcvf";
    insertAfterEraseThree(orig, 6, "lol");
    assert(orig == "qazxs wedcvf");
}
