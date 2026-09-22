Write a C++ function named `hasCircularPath` that takes a single `std::string` argument `moves` (containing only uppercase characters `'U'`, `'D'`, `'L'`, and `'R'`), and returns a `bool` indicating whether applying those moves from the origin `(0,0)` returns to the origin. The available moves are: `'U'` moves up (increase y by 1), `'D'` moves down (decrease y by 1), `'L'` moves left (decrease x by 1), and `'R'` moves right (increase x by 1). The function must be `const`-correct (i.e., the input string is not modified) and work correctly for empty strings (return `true`), strings of any length, and handle any sequence of moves. Provide a solution that checks both coordinates independently and returns `true` only if the final position is exactly `(0,0)`. The function must avoid using `std::map` or any complex data structures; a simple loop with integer tracking is sufficient.
// The solution maintains two integer coordinates `x` and `y`, initially both `0`. For each character in the input string, update the coordinates according to the mapping: `'U'` increments `y`, `'D'` decrements `y`, `'L'` decrements `x`, `'R'` increments `x`. After processing all characters, return `(x == 0 && y == 0)`. This is a straightforward linear scan. The only edge case is the empty string: since no moves are applied, the position remains `(0,0)`, so the function returns `true`. The algorithm runs in `O(n)` time where `n` is the length of the input string, and uses `O(1)` auxiliary space. There are no tricky cases regarding overflow because the number of moves is bounded by the string length, and integer coordinates are used; for typical constraints (string length up to 10^5 or 10^6) this is safe. The solution does not require any special handling for invalid characters, as the problem statement guarantees the input only contains the four specified characters.
#include <string>

// Determines if a sequence of moves returns to the origin (0,0).
// 'U' = up (y+1), 'D' = down (y-1), 'L' = left (x-1), 'R' = right (x+1).
bool hasCircularPath(const std::string& moves) {
    int x = 0;
    int y = 0;
    for (char c : moves) {
        switch (c) {
            case 'U': ++y; break;
            case 'D': --y; break;
            case 'L': --x; break;
            case 'R': ++x; break;
            // No default needed; input guaranteed valid.
        }
    }
    return (x == 0 && y == 0);
}
#include <cassert>

int main() {
    assert(hasCircularPath("UD") == true);
    assert(hasCircularPath("LR") == true);
    assert(hasCircularPath("ULDR") == true);
    assert(hasCircularPath("") == true);
    assert(hasCircularPath("U") == false);
    assert(hasCircularPath("D") == false);
    assert(hasCircularPath("L") == false);
    assert(hasCircularPath("R") == false);
    assert(hasCircularPath("UDLR") == true);
    assert(hasCircularPath("UUDDLLRR") == true);
    assert(hasCircularPath("URDL") == true);
    assert(hasCircularPath("UUU") == false);
    assert(hasCircularPath("RLR") == false);
    return 0;
}
