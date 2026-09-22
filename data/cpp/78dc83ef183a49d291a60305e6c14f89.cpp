// Write a C++ function named `towerOfHanoiSteps` that takes an integer `n` (number of disks) and returns a `std::string` containing the sequence of moves required to transfer all disks from rod `'a'` (source) to rod `'c'` (destination) using rod `'b'` (auxiliary). Each move must be on its own line in the format "source destination" (e.g., "a c"). The function should handle values of `n` from 0 to 20 inclusive. When `n == 0`, return an empty string. The solution must be recursive and follow the classic Tower of Hanoi rules: move only one disk at a time, never place a larger disk on a smaller one, and use the auxiliary rod optimally. The returned string must match the exact format expected by the original problem (no extra whitespace, each move separated by a newline). The function should be declared with proper `const` correctness where applicable and be callable in a standalone test harness.
The Tower of Hanoi puzzle has a well-known recursive solution. To move `n` disks from source rod `s` to destination rod `d` using auxiliary rod `a`, we first recursively move `n-1` disks from `s` to `a` (using `d` as auxiliary), then move the largest disk from `s` to `d` directly, and finally recursively move the `n-1` disks from `a` to `d` (using `s` as auxiliary). The base case occurs when `n == 0`, where no moves are needed. For `n > 0`, at each recursive step we append the current move (source and destination separated by a space) followed by a newline to the result string. Edge case: `n == 0` returns an empty string, and `n == 1` returns exactly "a c". The algorithm runs in `O(2^n - 1)` time because the number of moves is `2^n - 1`, and uses `O(n)` space due to the recursion stack depth. The implementation must build the string efficiently (using `std::string` concatenation is acceptable for `n <= 20` because the total output size is at most `(2^20 - 1) * 4` characters ≈ 4 MB, still reasonable). Alternatively, use an `ostringstream` for cleaner appending. The function should be free of side effects—it only constructs and returns the string.
#include <string>

// Returns a string containing the moves to solve Tower of Hanoi for n disks.
// Moves are formatted as "source destination" per line, with source 'a',
// auxiliary 'b', destination 'c'. Returns empty string for n == 0.
std::string towerOfHanoiSteps(int n, char source = 'a', char auxiliary = 'b', char destination = 'c') {
    std::string result;

    if (n > 0) {
        // Move n-1 disks from source to auxiliary using destination as temp.
        result += towerOfHanoiSteps(n - 1, source, destination, auxiliary);

        // Move the largest disk from source to destination.
        result += source;
        result += ' ';
        result += destination;
        result += '\n';

        // Move n-1 disks from auxiliary to destination using source as temp.
        result += towerOfHanoiSteps(n - 1, auxiliary, source, destination);
    }

    return result;
}
#include <cassert>
#include <string>

int main() {
    // n = 0: no moves.
    assert(towerOfHanoiSteps(0) == "");

    // n = 1: single move from a to c.
    assert(towerOfHanoiSteps(1) == "a c\n");

    // n = 2: sample from problem.
    assert(towerOfHanoiSteps(2) == "a b\na c\nb c\n");

    // n = 3: sample from problem.
    assert(towerOfHanoiSteps(3) == "a c\na b\nc b\na c\nb a\nb c\na c\n");

    // n = 4: verify number of lines (2^4 - 1 = 15).
    std::string four = towerOfHanoiSteps(4);
    int lineCount = 0;
    for (char ch : four) {
        if (ch == '\n') ++lineCount;
    }
    assert(lineCount == 15);

    // Check the last move of n=4 is "a c".
    assert(four.substr(four.size() - 4) == "a c\n");

    // n = 5: verify count is 31.
    std::string five = towerOfHanoiSteps(5);
    int lineCount5 = 0;
    for (char ch : five) {
        if (ch == '\n') ++lineCount5;
    }
    assert(lineCount5 == 31);

    // Small sanity: n=2 first character is 'a', last part is "b c\n".
    assert(towerOfHanoiSteps(2).front() == 'a');
    assert(towerOfHanoiSteps(2).substr(5) == "b c\n");

    // Ensure function works with explicit rod names (default arguments are fine).
    assert(towerOfHanoiSteps(1, 'x', 'y', 'z') == "x z\n");
}
