// Write a C++ function named `printThreeNewLines` that takes no arguments and returns nothing, but when called, it outputs three newline characters to the standard output by invoking a helper function named `newLine()` exactly three times. The helper function `newLine()` itself outputs a single newline (`endl`). Your function must not use any direct output statements (like `cout << endl;`) inside its body; it can only call `newLine()` three times. Also, ensure that `newLine()` is defined before `printThreeNewLines` so that the code compiles without forward declarations. The function should be declared `void` and should be placed in a standalone code block (without a `main` function in the solution section). The task tests that calling `printThreeNewLines` indeed produces exactly three newline characters, which can be verified by capturing the output stream or by counting them.
#include <cassert>
#include <iostream>
#include <sstream>

int main() {
    // Redirect cout to a stringstream to capture output.
    std::stringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());

    // Call the solution function.
    printThreeNewLines();

    // Restore cout.
    std::cout.rdbuf(old);

    // The captured output should be exactly three newline characters.
    assert(buffer.str() == "\n\n\n");

    // Also test that it's not four or two.
    assert(buffer.str().size() == 3);

    // Extra sanity check: each character is a newline.
    for (char c : buffer.str()) {
        assert(c == '\n');
    }

    // Edge case: call twice to ensure it works repeatedly.
    std::stringstream buffer2;
    std::streambuf* old2 = std::cout.rdbuf(buffer2.rdbuf());
    printThreeNewLines();
    printThreeNewLines();
    std::cout.rdbuf(old2);
    assert(buffer2.str() == "\n\n\n\n\n\n");

    return 0;
}
#include <iostream>

// Helper that outputs a single newline and flushes.
void newLine()
{
    std::cout << std::endl;
}

// Print exactly three newlines by calling newLine() three times.
void printThreeNewLines()
{
    newLine();
    newLine();
    newLine();
}
// The solution requires defining a helper function `newLine()` that performs `cout << endl;`. Then, the main function `printThreeNewLines` simply calls `newLine()` three times sequentially. There are no parameters, no return values, and no special edge cases other than ensuring that the helper is defined before the caller (or a forward declaration is used, but for simplicity we place it first). The main algorithm is trivial: three function calls. Time complexity is O(1) (constant time, ignoring I/O buffering), and space complexity is O(1). The only subtlety is that `cout << endl` flushes the output buffer each time, which is intentional. In the test, we can redirect `cout` to a `stringstream`, call `printThreeNewLines`, then assert that the captured string contains exactly three newline characters (i.e., `"\n\n\n"`). However, note that `endl` also flushes, but that doesn't affect the captured content. Alternatively, we could directly test by counting newlines in a captured buffer. The test code will restore `cout` after capturing and use `assert` to verify equality.
