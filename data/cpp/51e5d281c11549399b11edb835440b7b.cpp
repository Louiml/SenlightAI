Write a C++ function that takes a string `s` containing lowercase English letters and returns a new string where the characters are rearranged according to the following rule: process the original string from left to right, and for each character at an even index (0-based), append it to the end of the result string that is being built; for each character at an odd index, prepend it to the beginning of the result string. However, the rule changes depending on the parity of the total length of the input string: if the length of `s` is even, apply the rule "even index → append to end, odd index → prepend to beginning"; if the length of `s` is odd, apply the opposite rule: "even index → prepend to beginning, odd index → append to end". Return the final resulting string.

#include <cassert>
#include <string>

// The solution function is declared above (or included from the provided header).
// We declare it here for completeness.
std::string rearrangeString(const std::string& s);

int main() {
    // Test for even length: "abcdef" (n=6)
    // Process:
    // i=0 (even) -> append 'a' -> "a"
    // i=1 (odd)  -> prepend 'b' -> "ba"
    // i=2 (even) -> append 'c' -> "bac"
    // i=3 (odd)  -> prepend 'd' -> "dbac"
    // i=4 (even) -> append 'e' -> "dbace"
    // i=5 (odd)  -> prepend 'f' -> "fdbace"
    // Expected: "fdbace"
    assert(rearrangeString("abcdef") == "fdbace");
    
    // Test for odd length: "abcde" (n=5)
    // i=0 (even) -> prepend 'a' -> "a"
    // i=1 (odd)  -> append 'b' -> "ab"
    // i=2 (even) -> prepend 'c' -> "cab"
    // i=3 (odd)  -> append 'd' -> "cabd"
    // i=4 (even) -> prepend 'e' -> "ecabd"
    // Expected: "ecabd"
    assert(rearrangeString("abcde") == "ecabd");
    
    // Test single character
    assert(rearrangeString("x") == "x");
    
    // Test empty string (should return empty)
    assert(rearrangeString("") == "");
    
    // Test even length with repeated characters
    // "abab" (n=4)
    // i=0 (even) append 'a' -> "a"
    // i=1 (odd)  prepend 'b' -> "ba"
    // i=2 (even) append 'a' -> "baa"
    // i=3 (odd)  prepend 'b' -> "bbaa"
    // Expected: "bbaa"
    assert(rearrangeString("abab") == "bbaa");
    
    // Test odd length with repeated characters
    // "zzz" (n=3)
    // i=0 (even) prepend 'z' -> "z"
    // i=1 (odd)  append 'z' -> "zz"
    // i=2 (even) prepend 'z' -> "zzz"
    // Expected: "zzz"
    assert(rearrangeString("zzz") == "zzz");
    
    // Additional even test "hello" (n=5 odd) -> let's verify manually:
    // "hello": indices 0:h (prepend), 1:e (append), 2:l (prepend), 3:l (append), 4:o (prepend)
    // Start empty, i=0: prepend 'h' -> "h"
    // i=1: append 'e' -> "he"
    // i=2: prepend 'l' -> "lhe"
    // i=3: append 'l' -> "lhel"
    // i=4: prepend 'o' -> "olhel"
    assert(rearrangeString("hello") == "olhel");
    
    // Test another even length "aabb" (n=4)
    // i=0: append 'a' -> "a"
    // i=1: prepend 'a' -> "aa"
    // i=2: append 'b' -> "aab"
    // i=3: prepend 'b' -> "baab"
    assert(rearrangeString("aabb") == "baab");
    
    return 0;
}

#include <string>
#include <deque>

// Rearrange characters of s according to the parity-based rule.
// If s.size() is even: even indices -> append to end, odd -> prepend to front.
// If s.size() is odd: even indices -> prepend to front, odd -> append to end.
// Returns the resulting string.
std::string rearrangeString(const std::string& s) {
    std::deque<char> buffer;
    const size_t n = s.size();
    const bool evenLength = (n % 2 == 0);
    
    for (size_t i = 0; i < n; ++i) {
        const bool evenIndex = (i % 2 == 0);
        // Decide whether to push to front or back.
        // For even length: evenIndex -> push_back, oddIndex -> push_front.
        // For odd length: evenIndex -> push_front, oddIndex -> push_back.
        if (evenLength) {
            if (evenIndex) {
                buffer.push_back(s[i]);
            } else {
                buffer.push_front(s[i]);
            }
        } else {
            if (evenIndex) {
                buffer.push_front(s[i]);
            } else {
                buffer.push_back(s[i]);
            }
        }
    }
    
    return std::string(buffer.begin(), buffer.end());
}

// The key observation is that the problem describes a deterministic rearrangement of characters based on their original index and the parity of the total string length. The solution is straightforward: iterate over the input string once, and for each index `i`, decide whether to add that character to the front or back of a result string. The decision depends on two booleans: the parity of `i` (even/odd) and the parity of `n` (the total length). Specifically, if `n` is even, then even indices are appended to the back and odd indices are prepended to the front. If `n` is odd, the opposite is true: even indices are prepended to the front and odd indices are appended to the back. This can be implemented using a `std::string` and either `result = s[i] + result` (prepend) or `result += s[i]` (append). The simulation is direct and requires no further data structures. Edge cases include empty string (though the problem statement likely assumes non-empty; the code should handle it gracefully by returning an empty string), and strings of length 1, which trivially return the same character because either rule leaves the result unchanged. Time complexity is O(n) because each character is processed once and prepending to a `std::string` takes O(k) where k is the current length, but since each prepend shifts the existing content, the total cost is O(n^2) if we use `s[i] + result` naively? Actually, in practice `std::string` prepending via `+` creates a temporary and then copies, so it is O(n) per operation, leading to O(n^2) total. However, we can avoid this by using a `std::deque<char>` or by building a `std::string` of fixed size and placing characters at computed positions, which would be O(n). For a teaching problem, we can accept O(n^2) but note that a more efficient approach uses a `std::deque` or precomputed positions. In the reference solution below, I will use a `std::deque<char>` to achieve O(n) time and O(n) space. The space complexity is O(n) for the result. The solution must handle the case where the input length is even or odd correctly.
