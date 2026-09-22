/*
Write a C++ function named `towerOfHanoi` that simulates the classic Tower of Hanoi puzzle. The function should take an integer `n` (the number of disks, `n >= 0`) and three character parameters representing the source, auxiliary, and destination pegs. It must print the entire sequence of moves required to transfer all disks from the source peg to the destination peg, following the rules: only one disk can be moved at a time, each move takes the top disk from one peg and places it on another, and a larger disk may never be placed on top of a smaller disk. The output for each move must be formatted as `SourcePeg ---> DestinationPeg` on its own line. The function should handle the base case where `n == 0` by producing no output. Ensure the function is `const`-correct and uses descriptive parameter names.
*/

#include <iostream>

// Print the sequence of moves to solve the Tower of Hanoi puzzle.
// n: number of disks, source: source peg, auxiliary: auxiliary peg, destination: destination peg.
void towerOfHanoi(const int n, const char source, const char auxiliary, const char destination) {
    if (n > 0) {
        towerOfHanoi(n - 1, source, destination, auxiliary);
        std::cout << source << " ---> " << destination << '\n';
        towerOfHanoi(n - 1, auxiliary, source, destination);
    }
}

#include <cassert>
#include <sstream>
#include <string>

// Helper to capture output from towerOfHanoi
std::string captureOutput(int n, char a, char b, char c) {
    std::ostringstream oss;
    std::streambuf* old = std::cout.rdbuf(oss.rdbuf());
    towerOfHanoi(n, a, b, c);
    std::cout.rdbuf(old);
    return oss.str();
}

int main() {
    // Test with 0 disks: no output
    assert(captureOutput(0, 'A', 'B', 'C') == "");

    // Test with 1 disk: single move
    assert(captureOutput(1, 'A', 'B', 'C') == "A ---> C\n");

    // Test with 2 disks: known sequence
    assert(captureOutput(2, 'A', 'B', 'C') == "A ---> B\nA ---> C\nB ---> C\n");

    // Test with 3 disks: known sequence
    assert(captureOutput(3, 'A', 'B', 'C') == "A ---> C\nA ---> B\nC ---> B\nA ---> C\nB ---> A\nB ---> C\nA ---> C\n");

    // Test with different peg letters
    assert(captureOutput(1, 'X', 'Y', 'Z') == "X ---> Z\n");

    // Test that the number of moves for n=4 is 15 lines
    std::string out4 = captureOutput(4, 'A', 'B', 'C');
    int moves = 0;
    for (char ch : out4) if (ch == '\n') moves++;
    assert(moves == 15);

    return 0;
}

// The solution relies on the recursive nature of the Tower of Hanoi problem. To move `n` disks from source to destination using an auxiliary peg, we first recursively move the top `n-1` disks from source to auxiliary (using destination as the temporary peg), then move the largest disk directly from source to destination, and finally recursively move the `n-1` disks from auxiliary to destination (using source as the temporary peg). The base case occurs when `n` equals 0, at which point no moves are needed and the recursion terminates. Edge cases include `n = 0` (no output) and `n = 1` (single direct move). The function modifies no external state and works purely via recursion and console output. The time complexity is \(O(2^n - 1)\) moves, which is optimal, and the space complexity is \(O(n)\) due to the recursion stack depth.
