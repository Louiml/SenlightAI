// Write a C++ function `bool isRotation(const std::string& s, const std::string& t)` that returns `true` if string `t` is a rotation of string `s` (i.e., `t` can be obtained by moving some prefix of `s` to its end, possibly zero characters), and `false` otherwise. The function should handle empty strings, strings of different lengths, and cases where one string is a rotation of the other even when characters are repeated (e.g., `s = "abab"` and `t = "baba"`). The comparison must be case-sensitive. You may assume ASCII characters only.

#include <cassert>
#include <string>

bool isRotation(const std::string& s, const std::string& t);

int main() {
    assert(isRotation("abc", "bca") == true);
    assert(isRotation("abc", "abc") == true);   // zero shift
    assert(isRotation("abc", "cab") == true);
    assert(isRotation("abc", "acb") == false);  // not a rotation
    assert(isRotation("", "") == true);         // both empty
    assert(isRotation("ab", "") == false);      // different lengths
    assert(isRotation("abab", "baba") == true); // repeated characters
    assert(isRotation("hello", "elloh") == true);
    assert(isRotation("hello", "olleh") == false); // reverse is not rotation
    assert(isRotation("a", "a") == true);
    return 0;
}

#include <string>

// Returns true if t is a rotation of s.
bool isRotation(const std::string& s, const std::string& t) {
    // Different lengths cannot be rotations (empty case handled here).
    if (s.length() != t.length()) {
        return false;
    }

    // If both are empty, it's trivially true (covered by find on "" in "").
    const std::string doubled = s + s;
    return doubled.find(t) != std::string::npos;
}

// A well-known approach for checking if one string is a rotation of another is to concatenate one string with itself and then search for the other string as a substring. If `t` is a rotation of `s`, then `t` must appear somewhere in `s + s`. For example, if `s = "abc"` and `t = "bca"`, then `s + s = "abcabc"` contains `"bca"`. Conversely, if `t` appears in `s + s` and the lengths are equal, then `t` is indeed a rotation.
//
// **Correctness conditions**: 
// - If lengths differ, return `false` immediately (unless both are empty, which is a rotation with zero shifts).
//
// - If `s` is empty, then `t` must also be empty for it to be a rotation; the substring search on `s + s = ""` would only match an empty `t`. But the length check already handles that.
//
// - Edge case: both strings empty → `""` appears in `"" + "" = ""` → `true`. Both non-empty and equal length → standard rotation check.
//
// **Algorithm**:
// 1. Check if `s.length() != t.length()`. If so, return `false`.
// 2. Create `doubleS = s + s`.
// 3. Use `doubleS.find(t)` (C++ `std::string::find` returns `std::string::npos` if not found).
// 4. Return `doubleS.find(t) != std::string::npos`.
//
// **Time complexity**: Let `n = s.length()`. Constructing `doubleS` takes O(n) time and memory. `std::string::find` typically uses an efficient algorithm (often O(n) in the average case using something like Boyer-Moore or a two-way string search, though worst-case could be O(n^2) depending on the implementation; but for a standard library implementation, the average is O(n)). Overall, this is O(n) average, O(n) auxiliary space for the concatenated string.
//
// **Why this works**: A rotation by `k` positions (where `0 ≤ k < n`) means `t[i] = s[(i + k) % n]` for all `i`. Then `s + s` contains `t` starting at position `k`. Conversely, if `t` occurs in `s + s` at position `p` (with `0 ≤ p < n`), then for all `i`, `t[i] = (s + s)[p + i] = s[(p + i) % n]`, which is exactly a rotation. Since `t` has length `n`, any occurrence must start within the first `n` characters of `s + s`, so the search is valid.
