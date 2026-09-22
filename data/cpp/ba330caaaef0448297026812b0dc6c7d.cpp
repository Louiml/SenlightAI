Write a C++ function named `printAsteriskLine` that takes an integer parameter `count` and returns a `std::string` containing exactly `count` asterisks (`*`) followed by a newline character (`\n`). The function should work correctly for any non-negative integer value of `count`, including zero (where the result is just `"\n"`). The task is to mimic the behavior of the given snippet's loop without producing any output directly inside the function—the function must *build and return* the string instead. You do not need to handle negative `count` values (undefined behavior is acceptable if passed), but the code should be robust for `count = 0` and large values.

// The main algorithm is straightforward: create an empty `std::string`, then append `count` occurrences of `'*'` using a loop, and finally append a newline character `'\n'`. The edge cases include `count = 0` (the loop runs zero times, returning just `"\n"`) and large `count` values (the loop scales linearly). Time complexity is \(O(\text{count})\), because each asterisk requires one append operation. Space complexity is also \(O(\text{count})\) because the resulting string must store all the characters. The function does not print anything; it returns a string, so the caller decides how to output it. This separation mirrors the provided snippet’s behavior (which prints exactly `n` asterisks and a newline) but in a reusable, testable form.

#include <string>

// Return a string containing 'count' asterisks followed by a newline.
// For count = 0, returns just "\n".
std::string printAsteriskLine(int count) {
    std::string result;
    result.reserve(count + 1); // Reserve space to avoid reallocations
    for (int i = 0; i < count; ++i) {
        result += '*';
    }
    result += '\n';
    return result;
}

#include <cassert>
#include <string>

// The solution function is declared here; in a real test file, include the header.
std::string printAsteriskLine(int count);

int main() {
    // Basic case: n=10 matches the original snippet's output
    assert(printAsteriskLine(10) == "**********\n");
    
    // Zero count: just a newline
    assert(printAsteriskLine(0) == "\n");
    
    // Small counts
    assert(printAsteriskLine(1) == "*\n");
    assert(printAsteriskLine(3) == "***\n");
    
    // Large count (e.g., 100)
    std::string expected100 = std::string(100, '*') + "\n";
    assert(printAsteriskLine(100) == expected100);
    
    // Count of 5: verify length and content
    std::string result5 = printAsteriskLine(5);
    assert(result5.size() == 6); // 5 stars + 1 newline
    assert(result5[0] == '*');
    assert(result5[4] == '*');
    assert(result5[5] == '\n');
    
    // Edge: count = 2, ensure no extra stars
    assert(printAsteriskLine(2) == "**\n");
    
    return 0;
}
