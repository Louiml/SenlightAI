Write a C++ function `bool returnsToStart(const std::string& path)` that takes a string of directions consisting only of the uppercase letters `'N'`, `'S'`, `'E'`, and `'W'` (representing one unit of movement north, south, east, and west, respectively). The function should return `true` if, after following the entire sequence of moves starting from the origin `(0,0)`, you end up back at the origin; otherwise, return `false`. Empty strings are allowed and should return `true` (since you never leave the origin). The function must handle strings of arbitrary length, including very long ones, and must be case-sensitive (only uppercase letters are valid). You do not need to validate the input; assume it contains only the four specified characters.
The problem reduces to tracking the net displacement. Maintain two integer counters: one for the vertical axis (`a` for north/south) and one for the horizontal axis (`b` for east/west). For each character, increment or decrement the appropriate counter: `'N'` increases `a`, `'S'` decreases `a`, `'E'` increases `b`, `'W'` decreases `b`. After processing all characters, the path returns to the origin if and only if both counters are zero. This works for any sequence, including duplicates and symmetric moves. Empty strings trivially return `true`. Edge cases: a single `'N'` returns `false`, a pair `"NS"` returns `true`, `"NSEW"` returns `true`, and an unbalanced string like `"NN"` returns `false`. Time complexity is \(O(n)\) for a string of length \(n\), and auxiliary space complexity is \(O(1)\) because we only use two integer variables. No extra data structures or recursion are required.
#include <string>

// Return true if the sequence of moves returns to the starting point (0,0).
bool returnsToStart(const std::string& path) {
    int vertical = 0;   // + for N, - for S
    int horizontal = 0; // + for E, - for W

    for (char move : path) {
        if (move == 'N') {
            ++vertical;
        } else if (move == 'S') {
            --vertical;
        } else if (move == 'E') {
            ++horizontal;
        } else if (move == 'W') {
            --horizontal;
        }
    }

    return (vertical == 0) && (horizontal == 0);
}
#include <cassert>
#include <string>

// Declare the function from the solution (already included).
bool returnsToStart(const std::string& path);

int main() {
    assert(returnsToStart("") == true);
    assert(returnsToStart("N") == false);
    assert(returnsToStart("NS") == true);
    assert(returnsToStart("NSEW") == true);
    assert(returnsToStart("NNSSEESS") == true);
    assert(returnsToStart("NNN") == false);
    assert(returnsToStart("EENNWWSS") == true);
    assert(returnsToStart("EWNS") == true);
    assert(returnsToStart("NENE") == false);
    assert(returnsToStart("WWWW") == false);
    return 0;
}
