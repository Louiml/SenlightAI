Write a C++ function that takes a null-terminated C-style string (`const char*`) and a single `char target`, and returns a `std::vector<std::string>` containing all distinct occurrence prefixes starting from each position where `target` appears in the source string. Each prefix is a substring that starts at the found character and extends to the end of the source string. For example, given the string `"ababa"` and target `'a'`, the function should return `{"ababa", "aba", "a"}`. The function must search the entire string, skipping occurrences that are part of a previously recorded prefix (i.e., each occurrence corresponds to a unique starting index). If `target` is not found, return an empty vector. The function must handle `nullptr` input gracefully (return empty vector) and must not modify the input string. Use only standard library facilities; do not use `std::string` for the input, but you may use `std::string` for the output elements.
The solution uses `std::strchr` repeatedly to find the next occurrence of `target` in the input string. The main algorithm: start with `const char* pos = str;` and in a loop, call `pos = std::strchr(pos, target);` until it returns `nullptr`. For each found position, construct a `std::string` from that pointer using the `std::string(const char*)` constructor (which reads until the null terminator), then append it to the result vector. After recording, increment `pos` by one to move past the current character so the next search starts after it, avoiding an infinite loop on repeated characters. Edge cases: (1) If `str` is `nullptr`, return empty vector immediately to avoid undefined behavior. (2) If `target` is the null character `'\0'`, `std::strchr` would find the terminator; however, to keep the semantics clean, we handle `target == '\0'` by returning an empty vector because a prefix starting at the end would be an empty string (which is arguably valid but not intended). (3) If `target` appears at the very end, the prefix is just a single-character string, which is correct. Time complexity: Each `strchr` call scans from the current position to the next occurrence or the end, so total time is O(n) where n is the length of the string, because each character is examined at most once across all calls. Space complexity: O(k * m) for the output vector, where k is the number of occurrences and m is the average prefix length; in the worst case (string of all same characters) this is O(n^2) due to storing overlapping substrings. The algorithm uses O(1) auxiliary space excluding the output.
#include <vector>
#include <string>
#include <cstring>

// Return all suffixes of `str` starting at each occurrence of `target`.
// If str is nullptr or target is the null character, return an empty vector.
std::vector<std::string> findPrefixes(const char* str, char target) {
    std::vector<std::string> result;
    if (str == nullptr || target == '\0') {
        return result;
    }

    const char* pos = str;
    while ((pos = std::strchr(pos, target)) != nullptr) {
        result.emplace_back(pos); // constructs std::string from C-string
        ++pos; // move past the found character to avoid infinite loop
    }

    return result;
}
#include <cassert>
#include <vector>
#include <string>

// The function under test is declared here (from solution)
std::vector<std::string> findPrefixes(const char* str, char target);

int main() {
    // Normal case with multiple occurrences
    std::vector<std::string> r1 = findPrefixes("ababa", 'a');
    assert((r1 == std::vector<std::string>{"ababa", "aba", "a"}));

    // Target at beginning and end
    std::vector<std::string> r2 = findPrefixes("aXaXa", 'a');
    assert((r2 == std::vector<std::string>{"aXaXa", "aXa", "a"}));

    // No occurrences
    assert(findPrefixes("hello", 'z').empty());

    // Single character source
    std::vector<std::string> r3 = findPrefixes("T", 'T');
    assert((r3 == std::vector<std::string>{"T"}));

    // Occurrences adjacent
    std::vector<std::string> r4 = findPrefixes("aa", 'a');
    assert((r4 == std::vector<std::string>{"aa", "a"}));

    // Empty string (not nullptr)
    assert(findPrefixes("", 'a').empty());

    // nullptr input
    assert(findPrefixes(nullptr, 'a').empty());

    // Null character target is not allowed
    assert(findPrefixes("abc", '\0').empty());

    // Target at end only
    std::vector<std::string> r5 = findPrefixes("hello", 'o');
    assert((r5 == std::vector<std::string>{"o"}));

    // String with only the target
    std::vector<std::string> r6 = findPrefixes("X", 'X');
    assert((r6 == std::vector<std::string>{"X"}));

    // Repeated target across long string
    std::vector<std::string> r7 = findPrefixes("abcabc", 'b');
    assert((r7 == std::vector<std::string>{"bcabc", "bc"}));

    return 0;
}
