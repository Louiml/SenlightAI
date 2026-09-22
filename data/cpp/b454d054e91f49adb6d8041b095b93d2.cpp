Write a C++ function `std::string fixupName(const std::string& input)` that transforms a name by repeatedly replacing any occurrence of the substring `"__"` (two consecutive underscores) with a single dot `"."`, then after all such replacements are complete, replace every remaining underscore `"_"` with a hyphen `"-"`. For example, `"foo__bar_baz"` becomes `"foo.bar-baz"` (first `"__"` → `"."`, then remaining `"_"` → `"-"`). The function should handle empty strings, strings without underscores, and multiple overlapping or adjacent double underscores correctly (e.g., `"a___b"` should become `"a.-b"` because the first two underscores become a dot, leaving one underscore that later becomes a hyphen). Assume the input contains only ASCII printable characters. The function must not modify the input and must return a new string.
The solution operates in two distinct phases. First, process the input character by character to detect the substring `"__"`. Since the replacement is a single dot, and the replacement can create new `"__"` sequences only if the original had three or more consecutive underscores (e.g., `"___"` → first two become `.` leaving `._` which does not contain `"__"`), a simple linear scan from left to right works: when we see `'_'` and the next character is also `'_'`, append `'.'` and skip two characters; otherwise, append the current character and advance one. This correctly handles overlapping cases because once two underscores are consumed, the next check starts after them. After this pass, we have a string where no `"__"` remains. Then, perform a second pass over that intermediate string, replacing every `'_'` with `'-'`. This second pass can be done with `std::replace`. Edge cases: empty input returns empty; input with only single underscores becomes hyphens; input with three or more underscores produces a dot followed by hyphens for the remaining underscores. Time complexity is O(n) in two passes, and space complexity is O(n) for the intermediate string and the final result (though the final result can reuse the intermediate string if we modify it in place, but returning a new string is simpler and acceptable).
#include <string>
#include <algorithm>

// Transform a name: replace all "__" substrings with "." first,
// then replace every remaining '_' with '-'.
std::string fixupName(const std::string& input) {
    std::string intermediate;
    intermediate.reserve(input.size());

    // First pass: replace "__" with "."
    for (size_t i = 0; i < input.size(); ) {
        if (input[i] == '_' && i + 1 < input.size() && input[i + 1] == '_') {
            intermediate.push_back('.');
            i += 2;
        } else {
            intermediate.push_back(input[i]);
            ++i;
        }
    }

    // Second pass: replace every remaining '_' with '-'
    std::replace(intermediate.begin(), intermediate.end(), '_', '-');

    return intermediate;
}
int main() {
    // Basic cases
    assert(fixupName("simple_name") == "simple-name");
    assert(fixupName("nochange") == "nochange");
    assert(fixupName("") == "");

    // Double underscores become dots
    assert(fixupName("foo__bar") == "foo.bar");
    assert(fixupName("a__b__c") == "a.b.c");

    // Mixed underscores
    assert(fixupName("foo__bar_baz") == "foo.bar-baz");
    assert(fixupName("_foo_") == "-foo-");

    // Triple underscores: first two become dot, third becomes hyphen
    assert(fixupName("a___b") == "a.-b");

    // Four underscores: two dots
    assert(fixupName("x____y") == "x..y");

    // Leading and trailing double underscores
    assert(fixupName("__start_end__") == ".start-end.");

    // Multiple and adjacent patterns
    assert(fixupName("ab__cd__ef_gh") == "ab.cd.ef-gh");

    // Long string with many replacements
    assert(fixupName("one__two___three____four_____five") == "one.two.-three..four...-five");
}
