// Write a C++ function `describeInitialization` that accepts two integers `a1` and `a2` and returns a `std::string` describing how the member variables `a` and `b` of a class (simulating the constructor behavior in the snippet) are initialized under three distinct rules. The function should simulate the following constructor behaviors in order: (1) `a` is initialized directly from `a1`, and `b` is assigned `a2` (the active snippet behavior, printing values); (2) `a` is initialized from `a1`, and `b` is initialized as `a1 + 7`; (3) `a` is initialized from `a1`, and `b` is initialized as `a + a1` (using the updated `a`). For each of the three rules, the function must build a multi-line string that contains the exact lines printed by the snippet's constructor, i.e., `Value of a is <a>` and `The value of b is <b>`, where `<a>` and `<b>` are the actual integers after initialization. If `a2` is negative, the function should only simulate rule (1) and append a final line `Negative a2 given`; otherwise, it simulates all three rules. Return the concatenated string for all applicable rules, each pair of lines separated by a newline, and no trailing whitespace. The function must be `const`-correct, take arguments by value, and use only standard library facilities. The output must exactly match the order and format described.
// The task requires simulating three distinct initialization orders from the provided snippet. The key subtlety is rule (3) where `b` is initialized as `a + a1` after `a` has already been set to `a1`, so `b = a1 + a1 = 2 * a1`. For rule (2), `b = a1 + 7` independent of `a`. Rule (1) simply assigns `b = a2`. Since the snippet prints values during construction, the function must format these as strings. The function will build a `std::ostringstream` for efficient concatenation. For each applicable rule, compute `a` and `b` integers first, then append the two lines. Edge cases: `a2` negative means only rule (1) plus the extra line; otherwise, all three rules. No trailing newline: join rule outputs with `\n` between each rule's two-line block, but no extra at the end. Use `std::to_string` for integer conversion. Time complexity is O(1) (constant number of operations) because at most three rules are processed, each taking constant time. Space complexity is O(length of output) which is O(1) in practice since output has at most 7 lines; but technically linear in number of digits, but that's negligible. The function must be pure, without side effects (no printing), returning a string. The `main` test function will call it with various inputs and verify via `assert`.
#include <string>
#include <sstream>

// Simulates constructor initialization rules and returns the printed lines as a string.
std::string describeInitialization(int a1, int a2) {
    std::ostringstream out;
    bool first = true;

    // Helper to append a rule's output.
    auto appendRule = [&](int a_value, int b_value) {
        if (!first) {
            out << "\n";
        }
        first = false;
        out << "Value of a is " << a_value << "\n";
        out << "The value of b is " << b_value;
    };

    // Rule 1: a = a1, b = a2 (always present)
    appendRule(a1, a2);

    if (a2 < 0) {
        out << "\nNegative a2 given";
    } else {
        // Rule 2: a = a1, b = a1 + 7
        appendRule(a1, a1 + 7);
        // Rule 3: a = a1, b = a + a1 (where a has been set to a1)
        appendRule(a1, a1 + a1);
    }

    return out.str();
}
#include <cassert>
#include <string>

int main() {
    // Positive a2: all three rules
    assert(describeInitialization(5, 10) ==
           "Value of a is 5\nThe value of b is 10\n"
           "Value of a is 5\nThe value of b is 12\n"
           "Value of a is 5\nThe value of b is 10");

    // a2 = 0: not negative, so all three rules
    assert(describeInitialization(3, 0) ==
           "Value of a is 3\nThe value of b is 0\n"
           "Value of a is 3\nThe value of b is 10\n"
           "Value of a is 3\nThe value of b is 6");

    // Negative a2: only rule 1 plus extra line
    assert(describeInitialization(7, -4) ==
           "Value of a is 7\nThe value of b is -4\nNegative a2 given");

    // Negative a1 but positive a2
    assert(describeInitialization(-2, 6) ==
           "Value of a is -2\nThe value of b is 6\n"
           "Value of a is -2\nThe value of b is 5\n"
           "Value of a is -2\nThe value of b is -4");

    // a1 = 0, a2 = 0
    assert(describeInitialization(0, 0) ==
           "Value of a is 0\nThe value of b is 0\n"
           "Value of a is 0\nThe value of b is 7\n"
           "Value of a is 0\nThe value of b is 0");
}
