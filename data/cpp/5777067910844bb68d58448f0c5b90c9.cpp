// Write a C++ function named `isShortIdentifier` that takes a `std::string` as input and returns `true` if the string matches the pattern of a "short identifier": it must start with one or more lowercase English letters (`a`–`z`), followed immediately by one or more digits (`0`–`9`), with no other characters allowed. The function must return `false` for empty strings, strings containing uppercase letters, underscores, spaces, or any other symbols, and strings where the letter/digit blocks are missing or out of order (e.g., digits before letters, no digits at all, or no letters at all). Use POSIX extended regular expressions via `<regex.h>` to perform the matching. The function should be safe: if the regex compilation fails, it should return `false` and clean up any allocated resources. The function must not use `std::regex`; it must use the C‑style regex API exactly.
#include <cassert>
#include <string>

// Function declaration (definition provided above)
bool isShortIdentifier(const std::string& str);

int main() {
    assert(isShortIdentifier("a1") == true);
    assert(isShortIdentifier("abc123") == true);
    assert(isShortIdentifier("test0159") == true);
    assert(isShortIdentifier("abc") == false);          // no digits
    assert(isShortIdentifier("123") == false);          // no letters
    assert(isShortIdentifier("123abc") == false);      // digits first
    assert(isShortIdentifier("") == false);             // empty
    assert(isShortIdentifier("Abc123") == false);       // uppercase letter
    assert(isShortIdentifier("abc_123") == false);      // underscore
    assert(isShortIdentifier("abc 123") == false);      // space
    return 0;
}
#include <regex.h>
#include <string>

// Returns true if str matches pattern: one or more lowercase letters followed by one or more digits.
bool isShortIdentifier(const std::string& str) {
    regex_t re;
    const char* pattern = "^[a-z]+[0-9]+$";
    int compile_error = regcomp(&re, pattern, REG_EXTENDED | REG_NOSUB);
    if (compile_error != 0) {
        // Pattern is static and valid; if compilation fails, something is wrong.
        return false;
    }

    int match_result = regexec(&re, str.c_str(), 0, nullptr, 0);
    regfree(&re);

    // regexec returns 0 on match, REG_NOMATCH otherwise.
    return match_result == 0;
}
// The key is to use `regcomp` and `regexec` from `<regex.h>`. First, compile the extended regular expression `^[a-z]+[0-9]+$` with flags `REG_EXTENDED | REG_NOSUB` (the `REG_NOSUB` flag tells `regexec` that we do not need match positions, so we can pass `nullptr` for the match array). If `regcomp` returns a non‑zero error code, compilation failed (e.g., due to invalid pattern), so the function must return `false` immediately. For a valid string, call `regexec` with the C‑string representation; it returns `0` on a match and `REG_NOMATCH` (non‑zero) otherwise. Since the pattern is anchored with `^` and `$`, the entire string must match. Importantly, for an empty string, `[a-z]+` requires at least one letter, so it fails. Strings with uppercase or symbols fail because the character classes are strict. Edge cases like `"abc123"` pass, `"123abc"` fails (digits first), `"abc"` fails (no digits), `"123"` fails (no letters). After the match, always call `regfree` to release the compiled regex’s internal storage, even if `regexec` was never reached due to an early return – but here we only return early on compilation failure after having cleaned up nothing else. Time complexity is linear in string length for the regex scan (though the pattern is simple), and space complexity is constant beyond the input string and regex structure.
