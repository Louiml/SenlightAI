// Write a C++ function named `evaluateExpression` that takes a single character operator and two integer operands, performs the corresponding arithmetic operation, and returns the integer result. The supported operations are addition (`+`), subtraction (`-`), multiplication (`*`), and integer division (`/`). If the operator is `?`, the function should immediately return `0` and signal that the sequence is terminated by setting an output reference parameter `bool& running` to `false`. For any other invalid operator, the function should return `0` and set `running` to `false` as well. Division by zero should return `0` and set `running` to `false`. The function must be `const`-correct (parameters are passed by value or const ref, and the function itself is not modifying anything external). The function signature should be `int evaluateExpression(int x, int y, char op, bool& running)`. Do not include a `main` function; provide only the function implementation in the solution section. In the test section, write `assert` statements that call this function directly with various inputs and check the results and the `running` flag.

The solution is straightforward: check the operator character and perform the corresponding integer operation. For `+`, `-`, and `*`, the operation is direct. For `/`, check that `y` is not zero before performing integer division (since integer division truncates toward zero in C++). For `?` and any unrecognized character, set `running` to `false` and return `0`. For the valid operations, set `running` to `true` and return the computed value. Edge cases include division by zero, invalid operator characters, and the special termination operator `?`. Time complexity is O(1) per call, and space complexity is O(1). The `running` flag is an output parameter passed by reference to update the caller's loop control.

#include <cstddef> // for size_t if needed, but not required here

// Perform integer arithmetic based on operator op.
// If op is '?', or an invalid operator, or division by zero, set running to false and return 0.
// Otherwise, set running to true and return the result.
int evaluateExpression(int x, int y, char op, bool& running) {
    switch (op) {
        case '+':
            running = true;
            return x + y;
        case '-':
            running = true;
            return x - y;
        case '*':
            running = true;
            return x * y;
        case '/':
            if (y == 0) {
                running = false;
                return 0;
            }
            running = true;
            return x / y;
        case '?':
            running = false;
            return 0;
        default:
            running = false;
            return 0;
    }
}

#include <cassert>

int main() {
    bool running = true;

    // Valid operations
    assert(evaluateExpression(5, 3, '+', running) == 8 && running == true);
    assert(evaluateExpression(5, 3, '-', running) == 2 && running == true);
    assert(evaluateExpression(5, 3, '*', running) == 15 && running == true);
    assert(evaluateExpression(5, 3, '/', running) == 1 && running == true);
    assert(evaluateExpression(-5, 2, '/', running) == -2 && running == true);
    assert(evaluateExpression(0, 3, '/', running) == 0 && running == true);

    // Termination operator
    running = true;
    assert(evaluateExpression(1, 2, '?', running) == 0 && running == false);

    // Division by zero
    running = true;
    assert(evaluateExpression(5, 0, '/', running) == 0 && running == false);

    // Invalid operator
    running = true;
    assert(evaluateExpression(5, 3, '%', running) == 0 && running == false);

    // Large numbers
    assert(evaluateExpression(1000000, 1000000, '*', running) == 1000000000000 && running == true);

    // Negative division truncation toward zero
    assert(evaluateExpression(-7, 2, '/', running) == -3 && running == true);

    return 0;
}
