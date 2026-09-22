Write a C++ function `bool containsAllCharacters(const std::string& source, const std::string& target)` that returns `true` if every character appearing in `source` also appears at least once in `target`, and `false` otherwise. Characters are compared case-sensitively (e.g., `'A'` and `'a'` are different). The function must handle empty strings (an empty `source` is considered to trivially contain all of its characters, so return `true`; an empty `target` with a non-empty `source` returns `false` unless `source` is also empty). Duplicate characters in `source` do not affect the result; only the set of distinct characters matters. The function should not modify the input strings.

#include <cassert>
#include <string>

// Declare the function (assume it is defined elsewhere).
bool containsAllCharacters(const std::string& source, const std::string& target);

int main() {
    // Basic positive and negative cases.
    assert(containsAllCharacters("abc", "axbyc") == true);
    assert(containsAllCharacters("abc", "abx") == false);

    // Empty source is trivially true.
    assert(containsAllCharacters("", "anything") == true);
    // Empty target with non-empty source is false.
    assert(containsAllCharacters("a", "") == false);
    // Both empty is true.
    assert(containsAllCharacters("", "") == true);

    // Duplicates in source do not matter.
    assert(containsAllCharacters("aaa", "a") == true);
    // Case-sensitivity: 'A' and 'a' are distinct.
    assert(containsAllCharacters("A", "a") == false);

    // Characters not in ASCII range still work.
    assert(containsAllCharacters("é", "xéy") == true);
    assert(containsAllCharacters("é", "xey") == false);

    // Source longer than target but all characters present.
    assert(containsAllCharacters("hello", "olleh") == true);
    // Missing one character.
    assert(containsAllCharacters("hello", "olhe") == false);

    // Target contains extra characters; that's fine.
    assert(containsAllCharacters("z", "abcdefghijklmnopqrstuvwxyz") == true);

    return 0;
}

#include <string>
#include <vector>

// Returns true if every distinct character in `source` appears in `target`.
// Case-sensitive comparison. Empty `source` returns true.
bool containsAllCharacters(const std::string& source, const std::string& target) {
    // Marker for characters present in `target` (for standard char values 0-255).
    std::vector<bool> inTarget(256, false);
    for (char c : target) {
        inTarget[static_cast<unsigned char>(c)] = true;
    }

    // Build a set of distinct characters in `source` and check each.
    std::vector<bool> seenSource(256, false);
    for (char c : source) {
        unsigned char uc = static_cast<unsigned char>(c);
        if (!seenSource[uc]) {
            seenSource[uc] = true;
            if (!inTarget[uc]) {
                return false;
            }
        }
    }
    return true;
}

// The core problem reduces to checking set inclusion: for each distinct character in `source`, verify it exists in `target`. A straightforward approach is to iterate over each character of `source` and use the `std::string::find` method on `target` to check membership. To avoid redundant searching for duplicate characters, we can precompute the set of distinct characters in `source` (for example, using an array of booleans or a `std::set`). The simplest correct implementation iterates over all characters of `source` and calls `target.find(c)`, which returns `std::string::npos` if not found; if not found, return `false` immediately. Edge cases include both strings empty (returns `true`), `source` empty (returns `true`), and `target` empty with non‑empty `source` (returns `false` because the first character will not be found). Time complexity is \(O(m \cdot n)\) in the worst case where `m` is the size of `source` and `n` is the size of `target`, because each `find` call scans `target` linearly. Space complexity is \(O(1)\) auxiliary, ignoring input storage. For better performance (e.g., \(O(m+n)\) time), one could pre-record all characters of `target` in a boolean array of size 256, but for clarity the simpler approach is acceptable. In the solution below, we use a fixed‑size `bool` array for distinct source characters to avoid repeated checks, achieving \(O(m+n)\) time and \(O(1)\) space (since the alphabet is fixed at 256 for `char`).
