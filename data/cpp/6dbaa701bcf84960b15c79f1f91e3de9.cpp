Write a C++ function named `zeroSmaller` that takes two integer references as parameters and modifies the smaller of the two values to become 0. If the values are equal, neither value should be changed. The function should return `void` (or any suitable return type, but it must modify the original variables through references). Then, in a separate driver program (which you do not need to provide as part of the solution, but will be tested), the function is called with user-provided values, and the modified values are output. The task must handle positive, negative, and zero inputs correctly, and must not modify the arguments if they are equal.
#include <cassert>

// Declare the function to be tested.
void zeroSmaller(int& a, int& b);

int main() {
    int x, y;

    x = 5; y = 3;
    zeroSmaller(x, y);
    assert(x == 5 && y == 0);

    x = -2; y = 7;
    zeroSmaller(x, y);
    assert(x == 0 && y == 7);

    x = 4; y = 4;
    zeroSmaller(x, y);
    assert(x == 4 && y == 4);

    x = 0; y = -10;
    zeroSmaller(x, y);
    assert(x == 0 && y == -10);

    x = -5; y = -5;
    zeroSmaller(x, y);
    assert(x == -5 && y == -5);

    x = 100; y = 0;
    zeroSmaller(x, y);
    assert(x == 0 && y == 0);

    x = -1; y = -2;
    zeroSmaller(x, y);
    assert(x == 0 && y == -2);

    return 0;
}
#include <utility>   // for std::swap (not required but kept optional)
#include <cstddef>   // for size_t (not required here)

// Modifies the smaller of two integer references to become 0.
// If a < b, sets a = 0; if a > b, sets b = 0; if equal, no change.
void zeroSmaller(int& a, int& b) {
    if (a < b) {
        a = 0;
    } else if (a > b) {
        b = 0;
    }
    // If a == b, do nothing.
}
// The core algorithm is a simple conditional comparison. Since the parameters are passed by non-const reference, assignments inside the function directly affect the caller’s variables. The logic: if `a < b`, set `a = 0`; else if `a > b`, set `b = 0`; if `a == b`, do nothing. This handles all integer values including negatives and zeros. Edge cases: (1) equal numbers—must leave both unchanged; (2) negative numbers—comparison works as usual; (3) already-zero values—if one is zero and the other is larger, the smaller zero stays zero and the larger becomes zero, which is fine. Time complexity is O(1) constant time, and space complexity is O(1) auxiliary space (only the two references and no extra storage). The function is side-effect-free except for the intended modification.
