Write a C++ function named `minimumRepeats` that takes two non-empty strings, `a` and `b`, and returns the minimum number of times string `a` must be repeated consecutively so that string `b` becomes a contiguous substring of the resulting repeated string. If it is impossible for `b` to appear as a substring regardless of how many times `a` is repeated, the function should return `-1`. For example, if `a = "abcd"` and `b = "cdabcdab"`, the function should return `3` because repeating `a` three times gives `"abcdabcdabcd"`, which contains `b` starting at index 2. The function must be efficient and handle edge cases such as when `b` is already a substring of `a` (return 1), when `b` is longer than `a`, and when `b` contains characters not present in `a`.
#include <cassert>
#include <string>

int main() {
    // Basic cases
    assert(minimumRepeats("abcd", "cdabcdab") == 3);
    assert(minimumRepeats("a", "aa") == 2);
    assert(minimumRepeats("a", "a") == 1);
    assert(minimumRepeats("abc", "abcabc") == 2);
    
    // b already in a
    assert(minimumRepeats("abcd", "bc") == 1);
    
    // Impossible cases
    assert(minimumRepeats("abc", "def") == -1);
    assert(minimumRepeats("ab", "ba") == -1);
    
    // Longer b requiring many repeats
    assert(minimumRepeats("ab", "ababab") == 3);
    
    // Edge case where b length is a multiple of a length but alignment issue
    assert(minimumRepeats("ab", "ba") == -1);
    assert(minimumRepeats("abc", "cabca") == 2);
    
    // Single character repeated
    assert(minimumRepeats("x", "xxxx") == 4);
    
    // No extra repetition needed beyond length threshold
    assert(minimumRepeats("abc", "bcab") == 2);
    
    return 0;
}
#include <string>

// Returns the minimum number of times 'a' must be repeated so that 'b' becomes a substring.
// Returns -1 if impossible.
int minimumRepeats(const std::string& a, const std::string& b) {
    std::string repeated;
    int count = 0;

    // Build the repeated string until its length is at least the length of b.
    while (repeated.size() < b.size()) {
        repeated += a;
        ++count;
    }

    // Check if b is already a substring in this current repetition.
    if (repeated.find(b) != std::string::npos) {
        return count;
    }

    // Append one more copy of 'a' to cover edge cases where b starts near the end.
    repeated += a;
    ++count;

    if (repeated.find(b) != std::string::npos) {
        return count;
    }

    return -1;
}
// The optimal solution relies on a key insight: if `b` can be found in any number of repetitions of `a`, then it will be found once the length of the repeated string is at least `b.size() + a.size()`. This is because the substring `b` must start somewhere within one copy of `a` and extend at most into the next copy, so the maximum starting point for `b` in the repeated string is `a.size() - 1` if the repetition is long enough. The algorithm first constructs a repeated string by concatenating `a` repeatedly until its length is at least `b.size()`. We then check if `b` is a substring of this string. If not, we append one more copy of `a` (making the total length at least `b.size() + a.size() - 1`, but practically adding one full copy) and check again. If found after the first check, return the number of repetitions used; if found after the second check, return that count plus one; otherwise return -1. A subtle edge case: if `b` has a length that is a multiple of `a`'s length, the first check might still fail due to alignment, but the safety net of adding one more copy always covers all possible starting positions. The time complexity is O(|b| * (|a| + |b|)) in the worst case for the substring search, though typical string search algorithms are linear in practice. Space complexity is O(|a| + |b|) for the constructed repeated string.
