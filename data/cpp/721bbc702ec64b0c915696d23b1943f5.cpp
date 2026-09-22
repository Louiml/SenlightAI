Write a C++ function named `isAcceptedAnswer` that takes a single string as input and returns `true` if the string is exactly one of the accepted spellings: `"yes"`, `"Yes"`, `"YEs"`, `"YES"`, `"YeS"`, `"yeS"`, `"yEs"`, or `"yES"` (eight case‑sensitive variants); otherwise, return `false`. The function must be case‑sensitive regarding all letters except that only these eight specific combinations are allowed. The input string will contain only alphabetic characters and will have length between 1 and 10. Do not use any external libraries beyond the standard C++ headers. Your function should not print anything; it should only return a boolean.

// The solution is straightforward: compare the given string against a fixed set of exactly eight valid literals. Since the set is small and constant, a simple chain of equality checks is efficient and clear. Use `std::string` comparison (which is case‑sensitive by default) and return `true` if any match occurs, otherwise `false`. Edge cases include empty strings (though the problem guarantees length ≥ 1) and strings with unexpected characters other than the allowed ones—these should all return `false`. Time complexity is O(1) because the number of comparisons is constant regardless of input length (max 10 characters per comparison). Space complexity is O(1) as no extra data structures are used.

#include <string>

// Checks if the input string is one of the eight accepted case variants.
bool isAcceptedAnswer(const std::string& s) {
    return (s == "yes" || s == "Yes" || s == "YEs" || s == "YES" ||
            s == "YeS" || s == "yeS" || s == "yEs" || s == "yES");
}

#include <cassert>
#include <string>

bool isAcceptedAnswer(const std::string& s);

int main() {
    assert(isAcceptedAnswer("yes") == true);
    assert(isAcceptedAnswer("Yes") == true);
    assert(isAcceptedAnswer("YEs") == true);
    assert(isAcceptedAnswer("YES") == true);
    assert(isAcceptedAnswer("YeS") == true);
    assert(isAcceptedAnswer("yeS") == true);
    assert(isAcceptedAnswer("yEs") == true);
    assert(isAcceptedAnswer("yES") == true);
    assert(isAcceptedAnswer("yes ") == false);
    assert(isAcceptedAnswer("no") == false);
    assert(isAcceptedAnswer("YESX") == false);
    assert(isAcceptedAnswer("yess") == false);
    assert(isAcceptedAnswer("Yes") == true);
    return 0;
}
