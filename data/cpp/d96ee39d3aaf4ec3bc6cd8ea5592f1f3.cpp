Write a C++ function `int assignmentExpressionResult()` that implements the exact logic of the given code snippet but as a reusable function. The function should declare an integer variable `a` initialized to 2, compute an integer variable `b` as `a + 1`, then in an `if` condition assign `a = 3` and compare that assigned value to `b`. If the equality holds, return `a`; otherwise, return `a + 10`. The function must not read from stdin, write to stdout, or depend on any global state. Ensure the behavior matches the snippet exactly, including the side effect of the assignment inside the condition. Handle all integer cases as the snippet does—no special edge cases beyond normal integer arithmetic. The function should be const-correct where possible (e.g., mark local variables `const` only if no mutation occurs; here `a` is mutated, so use non-const). Provide the function in a self-contained header-style format with necessary includes.

The core of this task is to faithfully reproduce the control flow and side-effect semantics of the snippet. The snippet initializes `a = 2` and `b = a + 1 = 3`. The condition `(a = 3) == b` first assigns 3 to `a`, which is a side effect, and then compares the assigned value (3) with `b` (also 3). Since they are equal, the `if` branch executes and prints `a` (which is now 3 after the assignment). If the condition were false, the else branch would print `a + 10`. In the function version, we follow the same sequence: declare `a = 2`, `b = a + 1`, then if `(a = 3) == b` return `a`, else return `a + 10`. Because the assignment always makes `a` equal to 3, and `b` is also 3, the condition is always true, so the function always returns 3. Nevertheless, the implementation must preserve the original structure. Time complexity is O(1) and space complexity is O(1) because there are only two integer variables and no loops or dynamic structures. Edge cases: none beyond integer overflow, but since all values are small constants, overflow is impossible. The `a` variable is mutated, so it cannot be `const`; `b` is never mutated after initialization, so it could be `const`, but making it const is optional and does not affect logic. The function should be declared with `int` return type and no parameters.

#include <cstddef> // not needed but included for completeness if any size_t used; not used here

// Reproduces the logic from the snippet: a=2, b=a+1, then if (a=3)==b returns a, else returns a+10.
int assignmentExpressionResult() {
    int a = 2;
    const int b = a + 1; // b is 3 and never changes

    if ((a = 3) == b) {
        return a; // a is now 3
    } else {
        return a + 10;
    }
}

int main() {
    // The snippet's condition is always true because a is set to 3 and b is 3.
    assert(assignmentExpressionResult() == 3);
    // Also verify the type and that multiple calls return the same result (no state).
    assert(assignmentExpressionResult() == assignmentExpressionResult());
    // Confirm that the result is not influenced by external inputs (it's constant).
    assert(assignmentExpressionResult() == 3);
    // Edge case: ensure the function doesn't rely on return value optimization misbehavior.
    assert(assignmentExpressionResult() + 0 == 3);
    // Static check: the function takes no arguments and returns an int.
    static_assert(std::is_same<decltype(assignmentExpressionResult()), int>::value, "Must return int");
    return 0;
}
