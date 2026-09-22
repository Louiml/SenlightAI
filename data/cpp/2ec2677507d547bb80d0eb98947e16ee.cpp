// Write a C++ function named `allSameInRange` that takes a string `s` (containing only ASCII printable characters, length at least 1) and two non-negative integer indices `a` and `b` (0 ≤ a, b < s.length()). The function should return `true` if all characters in the inclusive range from `min(a,b)` to `max(a,b)` are identical, and `false` otherwise. The function must not modify the input string, must handle the case where `a` equals `b` (returning `true`), and must correctly process ranges in either order (e.g., `allSameInRange(s, 3, 1)` is equivalent to `allSameInRange(s, 1, 3)`). This task is inspired by a snippet that reads multiple test cases, but here you should implement a reusable, testable function with `const` correctness.

// The solution is straightforward: normalize the indices by swapping if necessary to ensure `a ≤ b`. Then check the character at position `a`, and iterate from `a+1` to `b` inclusive, comparing each character to the reference. If any mismatch is found, immediately return `false`; otherwise, after the loop, return `true`. Edge cases include: when `a == b` (range of length 1), the loop does not execute and the function correctly returns `true`; when the string length is 1 (only possible single index), the same logic applies. The algorithm runs in O(range length) time, which is at most O(n) where n is the string length, and uses O(1) auxiliary space. No special handling is needed for empty strings because the problem guarantees non-empty input; however, to be robust, you could assume the caller passes valid indices.

#include <string>
#include <algorithm> // for std::min and std::max, but we'll use manual swap

// Returns true if all characters in s[min(a,b)..max(a,b)] are identical.
bool allSameInRange(const std::string& s, int a, int b) {
    // Normalize the range so a <= b
    if (a > b) {
        int temp = a;
        a = b;
        b = temp;
    }

    char first = s[a];
    for (int i = a + 1; i <= b; ++i) {
        if (s[i] != first) {
            return false;
        }
    }
    return true;
}

#include <cassert>
#include <string>

// Declare the function (or include the header where it is defined)
bool allSameInRange(const std::string& s, int a, int b);

int main() {
    std::string s1 = "aaaa";
    assert(allSameInRange(s1, 0, 3) == true);
    assert(allSameInRange(s1, 1, 2) == true);
    assert(allSameInRange(s1, 2, 2) == true);

    std::string s2 = "abca";
    assert(allSameInRange(s2, 0, 0) == true);
    assert(allSameInRange(s2, 0, 1) == false);
    assert(allSameInRange(s2, 1, 2) == false);
    assert(allSameInRange(s2, 2, 3) == false);
    assert(allSameInRange(s2, 3, 0) == false); // reversed order, same as [0,3]

    std::string s3 = "xxxyyy";
    assert(allSameInRange(s3, 0, 2) == true);
    assert(allSameInRange(s3, 3, 5) == true);
    assert(allSameInRange(s3, 0, 5) == false);

    std::string s4 = "z";
    assert(allSameInRange(s4, 0, 0) == true);

    std::string s5 = "aabbaa";
    assert(allSameInRange(s5, 0, 1) == true);
    assert(allSameInRange(s5, 1, 4) == false);
    assert(allSameInRange(s5, 4, 5) == true);

    return 0;
}
