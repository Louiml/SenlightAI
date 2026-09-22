Write a standalone C++ function `removeAllOccurrences(std::string& s, const std::string& part)` that repeatedly removes every occurrence of the substring `part` from `s` until no occurrences remain. The removal must be done in-place, meaning the original string object `s` is modified directly. The function should handle cases where `part` is empty or longer than `s`, and should correctly process overlapping occurrences (e.g., removing "aa" from "aaaa" should result in an empty string, not "aa"). The function must not use any additional dynamic allocations beyond those inherent in string operations, and it should be implemented recursively or iteratively. Provide only the function definition, no `main`.
The solution repeatedly searches for the first occurrence of `part` in `s` using `std::string::find`. If found, it constructs the new string by concatenating the substring before the occurrence and the substring after the occurrence (skipping `part`'s length), then assigns this back to `s`. The process is repeated recursively (or iteratively) until `find` returns `std::string::npos`. Key edge cases: (1) If `part` is empty, `find` returns position 0, and the concatenation logic would create an infinite loop because removing an empty string does not change `s`; therefore, the function should return immediately if `part` is empty. (2) Overlapping occurrences like removing "aa" from "aaaa": after removing the first occurrence at index 0, the new string becomes "aa" (from indices 2-3), then the recursive call removes that "aa", yielding an empty string. (3) If `part` is longer than `s`, `find` returns `npos` and the function does nothing. The time complexity is O(n * m) in the worst case, where n is the length of `s` and m is the length of `part`, because each removal involves scanning `s` and copying substrings; the number of removals can be O(n/m). Space complexity is O(n) due to the recursive call stack and temporary string constructions in the worst case (or O(1) extra if iterative and using `erase`), but here we use recursion with string concatenation, which is O(n) auxiliary space.
#include <string>

// Remove all occurrences of 'part' from 's' repeatedly until none remain.
// If 'part' is empty, do nothing to avoid infinite recursion.
void removeAllOccurrences(std::string& s, const std::string& part) {
    if (part.empty()) {
        return;
    }
    const std::size_t found = s.find(part);
    if (found != std::string::npos) {
        s.erase(found, part.size());
        removeAllOccurrences(s, part);
    }
}
#include <cassert>
#include <string>

// Declaration of the function under test.
void removeAllOccurrences(std::string& s, const std::string& part);

int main() {
    std::string s1 = "hello world hello";
    removeAllOccurrences(s1, "hello");
    assert(s1 == " world ");

    std::string s2 = "aaaa";
    removeAllOccurrences(s2, "aa");
    assert(s2.empty());

    std::string s3 = "abcabcabc";
    removeAllOccurrences(s3, "abc");
    assert(s3.empty());

    std::string s4 = "mississippi";
    removeAllOccurrences(s4, "iss");
    assert(s4 == "mippi");

    std::string s5 = "nochange";
    removeAllOccurrences(s5, "xyz");
    assert(s5 == "nochange");

    std::string s6 = "keep";
    removeAllOccurrences(s6, "");
    assert(s6 == "keep");

    std::string s7 = "a";
    removeAllOccurrences(s7, "bb");
    assert(s7 == "a");

    std::string s8 = "ababa";
    removeAllOccurrences(s8, "aba");
    assert(s8 == "ba");

    std::string s9 = "x";
    removeAllOccurrences(s9, "x");
    assert(s9.empty());

    std::string s10 = "aabbaa";
    removeAllOccurrences(s10, "a");
    assert(s10 == "bb");

    return 0;
}
