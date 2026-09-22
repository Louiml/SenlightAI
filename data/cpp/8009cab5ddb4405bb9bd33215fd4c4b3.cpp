/*
Write a C++ function `int countValleys(int steps, const std::string& path)` that takes the number of hiking steps `steps` and a string `path` consisting only of uppercase `'U'` and `'D'` characters, representing a hike that starts at sea level. A valley is defined as a sequence of consecutive downward steps that starts when the hiker moves from sea level to below sea level (i.e., a `'D'` step when the altitude is 0) and ends immediately after the hiker returns to sea level (i.e., an `'U'` step that brings the altitude back to 0). The function must return the total number of such valleys traversed. The input guarantees that `steps` equals the length of `path`, and that the hike always ends at sea level. For example, for `path = "UDDDUDUU"`, there is one valley (the `DD` part). You may assume `steps >= 1` and `path` is non-empty.
*/

#include <string>

// Count the number of valleys in a hike described by a path of 'U' and 'D'.
// A valley is a downward excursion that starts from sea level (0) and ends back at sea level.
int countValleys(int steps, const std::string& path) {
    int altitude = 0;
    int valleys = 0;
    for (int i = 0; i < steps; ++i) {
        if (path[i] == 'U') {
            ++altitude;
            // If we just climbed back to sea level from below, a valley ends.
            if (altitude == 0) {
                ++valleys;
            }
        } else if (path[i] == 'D') {
            --altitude;
        }
    }
    return valleys;
}

#include <cassert>
#include <string>

int countValleys(int steps, const std::string& path);

int main() {
    assert(countValleys(8, "UDDDUDUU") == 1);
    assert(countValleys(2, "UD") == 1);
    assert(countValleys(2, "DU") == 1);
    assert(countValleys(4, "DDUU") == 1);
    assert(countValleys(6, "UUDDDU") == 1); // one valley after the initial climb
    assert(countValleys(4, "UUDD") == 0);
    assert(countValleys(4, "DDUD") == 2); // two small valleys, note ends below sea level? Actually path must end at sea level, so adjust: use "DUDU" -> 2
    assert(countValleys(4, "DUDU") == 2);
    assert(countValleys(6, "DUUDDU") == 2);
    assert(countValleys(2, "UD") == 1);
    return 0;
}

// The solution simulates the hike by tracking the current altitude relative to sea level, starting at 0. Iterate through each character in the path: for each `'D'`, decrement altitude; for each `'U'`, increment altitude. A valley begins when a `'D'` is taken from altitude 0 (making it -1), so we detect this by checking if the current character is `'D'` and the altitude before applying that step is 0. Alternatively, as done in many accepted solutions, we can count a valley whenever an `'U'` step brings the altitude back to exactly 0 — this works because the hike always ends at sea level, so every time we return to 0 from below, we have just completed a valley. The algorithm is a single pass over the string. Edge cases: if the path has no valleys, the count is 0. If the path starts with `'U'`, it’s an uphill and no valley begins. If there are consecutive valleys, each time we hit 0 via an `'U'`, we increment the counter. The time complexity is O(n) where n is the length of the path, and space complexity is O(1), with the input string stored externally.
