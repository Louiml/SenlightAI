/*
Write a C++ function named `canTransform` that takes three strings `a`, `b`, and `c` of equal length (all non-empty, same size) and returns `true` if for every index `i`, at least one of the following holds: `a[i] == c[i]` or `b[i] == c[i]`. In other words, the string `c` must match the corresponding character from either `a` or `b` at every position. Return `false` otherwise. The function should be case-sensitive and handle any printable ASCII characters. You do not need to validate input lengths; assume they are equal.
*/

#include <string>

// Returns true if for every index i, c[i] equals either a[i] or b[i].
bool canTransform(const std::string& a, const std::string& b, const std::string& c) {
    for (std::size_t i = 0; i < a.size(); ++i) {
        if (a[i] != c[i] && b[i] != c[i]) {
            return false;
        }
    }
    return true;
}

#include <cassert>

int main() {
    // Basic cases
    assert(canTransform("abc", "def", "abc") == true);
    assert(canTransform("abc", "def", "dec") == true);
    assert(canTransform("abc", "def", "xyz") == false);

    // Single character strings
    assert(canTransform("a", "b", "a") == true);
    assert(canTransform("a", "b", "b") == true);
    assert(canTransform("a", "b", "c") == false);

    // All identical
    assert(canTransform("hello", "hello", "hello") == true);

    // Mixed matching positions
    assert(canTransform("cat", "dog", "cot") == true); // c matches a, o matches neither? actually o != 'c' and o != 'd' -> false
    assert(canTransform("cat", "dog", "cag") == true); // c from a, a from a, g from b
    assert(canTransform("cat", "dog", "dot") == false); // t != c/d, o != a/d? -> false

    // Longer string with edge at last index
    assert(canTransform("abcde", "fghij", "abcij") == true);
    assert(canTransform("abcde", "fghij", "abcxy") == false);

    // Case sensitivity
    assert(canTransform("A", "a", "A") == true);
    assert(canTransform("A", "a", "a") == true);
    assert(canTransform("A", "a", "b") == false);
}

// The solution is straightforward: iterate through each character index of the strings once. For each index `i`, check if `a[i] == c[i]` or `b[i] == c[i]`. If neither condition holds at any position, the transformation is impossible, so return `false`. If all positions satisfy at least one condition, return `true`. Edge cases include empty strings (though task states non-empty) — the loop would naturally do nothing and return `true`, which is acceptable. Another edge case is when all three strings are identical — then every index trivially satisfies the condition. Time complexity is O(n) where n is the length of the strings, and space complexity is O(1) since only a few integer variables are used.
