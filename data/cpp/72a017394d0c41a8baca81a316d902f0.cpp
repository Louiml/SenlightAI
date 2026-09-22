// Write a C++ function `std::string rgbLights(const std::string& sequence)` that takes a string consisting only of lowercase letters `'r'`, `'g'`, `'b'` (representing unlit colored lights) and uppercase letters `'R'`, `'G'`, `'B'` (representing attempts to turn on the corresponding colored light), in any order. A light can be turned on only after at least one of its lowercase counterpart has appeared earlier in the sequence. The function should return `"YES"` if, after processing the entire sequence from left to right, all three colors (red, green, blue) have been turned on successfully, and `"NO"` otherwise. For example, in `"rRgG bB"` (no spaces in real input) all three turn on, but in `"RrGgBb"` none turn on because uppercase appears before the lowercase for each color. The input string length is between 1 and 10 inclusive, and only these six characters appear. You must implement the function and test it.
#include <cassert>
#include <string>

// The solution function is declared above (or included from a header).

int main() {
    // All three turned on correctly
    assert(rgbLights("rRgGbB") == "YES");
    // Mixed order but valid
    assert(rgbLights("rbgGRB") == "YES");
    // Uppercase before lowercase: none turned on
    assert(rgbLights("RrGgBb") == "NO");
    // Only one color turned on
    assert(rgbLights("rRbBgG") == "NO"); // Wait: rR is fine, bB fine, but gG? g appears then G later? Actually string "rRbBgG" -> r seen, R turns on red; b seen, B turns on blue; g seen, G turns on green -> YES. Let's change to "rRbb" -> only red and no blue because B never appears.
    assert(rgbLights("rRbb") == "NO"); // red on, but green and blue not
    assert(rgbLights("rg") == "NO"); // no uppercase at all
    // Uppercase appears before any lowercase of same color
    assert(rgbLights("Rr") == "NO"); // R before r, so not turned on
    // Multiple uppercase of same color: only count once
    assert(rgbLights("rRRRRgGbB") == "YES"); // red turns on at first R after r, extra R don't matter
    // All lowercase only, no uppercase
    assert(rgbLights("rgb") == "NO");
    // Minimal valid
    assert(rgbLights("rRgGbB") == "YES");

    return 0;
}
#include <string>

// Determines if all three colored lights can be turned on given the sequence.
// A light can be turned on only after its lowercase letter appears earlier.
std::string rgbLights(const std::string& sequence) {
    int redSeen = 0, greenSeen = 0, blueSeen = 0;
    bool redOn = false, greenOn = false, blueOn = false;
    int turnedOn = 0;

    for (const char ch : sequence) {
        if (ch == 'r') {
            ++redSeen;
        } else if (ch == 'g') {
            ++greenSeen;
        } else if (ch == 'b') {
            ++blueSeen;
        } else if (ch == 'R') {
            if (redSeen >= 1 && !redOn) {
                redOn = true;
                ++turnedOn;
            }
        } else if (ch == 'G') {
            if (greenSeen >= 1 && !greenOn) {
                greenOn = true;
                ++turnedOn;
            }
        } else if (ch == 'B') {
            if (blueSeen >= 1 && !blueOn) {
                blueOn = true;
                ++turnedOn;
            }
        }
    }

    return (turnedOn == 3) ? "YES" : "NO";
}
// The solution processes the string character by character, maintaining three counters for how many lowercase letters of each color have been seen. Initialize three counters `redSeen`, `greenSeen`, `blueSeen` to zero, and a counter `turnedOn` to zero. For each character:
// - If it is lowercase `'r'`, increment `redSeen`; `'g'` increments `greenSeen`; `'b'` increments `blueSeen`.
// - If it is uppercase `'R'`, check if `redSeen >= 1` (meaning at least one lowercase `r` appeared earlier). If yes, increment `turnedOn` and mark that red has been turned on (to avoid counting the same color twice). Similarly for `'G'` and `'B'`. Since each uppercase letter appears at most once per color in a valid problem (but could appear multiple times), we need to ensure we only count the first successful turn-on per color. Use boolean flags `redOn`, `greenOn`, `blueOn` initialized to `false`. When encountering an uppercase letter and the corresponding lowercase has appeared and the flag is false, set the flag to true and increment `turnedOn`. At the end, return `"YES"` if `turnedOn == 3`, else `"NO"`. Edge cases: uppercase before any lowercase → not counted; multiple uppercase of same color → only first counts; no lowercase at all → never turns on. Time complexity O(n) where n is string length, space O(1).
