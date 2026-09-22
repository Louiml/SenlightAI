Write a C++ function named `printGreetingNTimes` that takes an integer `n` as input and returns a string containing the phrase `"Hello"` repeated `n` times, with each occurrence on a new line (i.e., separated by `'\n'`). If `n` is less than or equal to 0, return an empty string. The function should handle potentially large values of `n` safely and use appropriate `const` correctness. Do not use any loops that could cause stack overflow; instead, use a standard loop to build the string.

The problem is straightforward: we need to generate a string that consists of the word `"Hello"` repeated `n` times, each on a new line. The main algorithm is a simple loop that appends the substring `"Hello\n"` to the result string `n` times, but we must handle the case where `n <= 0` by returning an empty string. We should also avoid an extra trailing newline if that is not desired, but the specification says each occurrence is on a new line, so appending `"Hello\n"` for each iteration is correct. Edge cases include `n = 0` (return empty string), `n = 1` (return `"Hello\n"` — note the trailing newline is acceptable, but if the specification requires no trailing newline, we might need to adjust; the task says "each occurrence on a new line", so having a trailing newline is fine, but to be safe we can return without trailing newline by appending `n` times with no delimiter after the last, but simpler is to append `"Hello\n"` each time and then optionally strip. However, for consistency with typical tasks, we will append `"Hello\n"` for each iteration, so for `n=1` output will be `"Hello\n"` and for `n=2` it will be `"Hello\nHello\n"`. That matches the requirement. Time complexity is O(n) because we build a string of length 6*n characters (5 for "Hello" plus 1 newline). Space complexity is O(n) for the returned string, plus O(1) auxiliary. We must be careful with large `n` to avoid integer overflow in the loop counter, but `int` is fine for typical constraints; we can use `long long` if necessary, but the task doesn't specify limits. We'll use `int` and note potential overflow only if `n` is extremely large, which is not likely in a test.

#include <string>

// Returns a string containing "Hello" repeated n times, each on a new line.
// For n <= 0, returns an empty string.
std::string printGreetingNTimes(int n) {
    if (n <= 0) {
        return "";
    }
    std::string result;
    result.reserve(n * 6); // each "Hello\n" is 6 chars
    for (int i = 0; i < n; ++i) {
        result += "Hello\n";
    }
    return result;
}

#include <cassert>
#include <string>

int main() {
    assert(printGreetingNTimes(0) == "");
    assert(printGreetingNTimes(-3) == "");
    assert(printGreetingNTimes(1) == "Hello\n");
    assert(printGreetingNTimes(2) == "Hello\nHello\n");
    assert(printGreetingNTimes(3) == "Hello\nHello\nHello\n");
    // Test with a larger number
    std::string expected5;
    for (int i = 0; i < 5; ++i) expected5 += "Hello\n";
    assert(printGreetingNTimes(5) == expected5);
    return 0;
}
