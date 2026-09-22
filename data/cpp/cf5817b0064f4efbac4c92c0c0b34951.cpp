// Write a C++ function `classifyFaceSize` that simulates the face detection size classification logic from the provided video processing snippet. The function should take as input: a face width and height (both integers), and a character `mode` indicating which detection configuration is active (`'n'` for near, `'m'` for middle, `'f'` for far). For each mode, a face is considered "valid" (detected) if its width and height both fall within a specific inclusive size range: for `'n'`, width and height must be between 30 and 50 pixels; for `'m'`, between 22 and 26 pixels; for `'f'`, between 1 and 15 pixels. The function should return `true` if the face matches the specified mode's size range, and `false` otherwise. If the mode is not one of the three valid characters, return `false` and do nothing else. The function should be `const`-correct (take parameters by value or const reference as appropriate) and should not depend on any OpenCV or external libraries—only standard C++.
The solution is straightforward: check the input mode and apply the corresponding size boundaries. For each mode, verify that both the width and height are within the inclusive minimum and maximum values. Edge cases include: a face with zero or negative dimensions (should always return `false`), a mode character that is lowercase but not one of the three valid ones (e.g., `'x'` or `'z'`), and boundary values (exactly 30 or 50 for near, 22 or 26 for middle, 1 or 15 for far) which should be considered valid. The algorithm runs in constant time \(O(1)\) and uses constant auxiliary space \(O(1)\). No loops or dynamic memory are needed. The only complexity is ensuring the correct comparisons (use `>=` and `<=`) and that the mode check is done first to avoid accidentally treating an invalid mode as valid by falling through to a default case.
#include <cstddef> // for size_t if needed, but not required here

// Determine whether a face of given width and height matches the size range
// for the specified detection mode ('n' near, 'm' middle, 'f' far).
// Returns true if the face fits the mode's inclusive pixel range, false otherwise.
// Unknown modes always return false.
bool classifyFaceSize(int faceWidth, int faceHeight, char mode) {
    // Validate mode and apply the corresponding size constraints
    if (mode == 'n') {
        return (faceWidth >= 30 && faceWidth <= 50) &&
               (faceHeight >= 30 && faceHeight <= 50);
    }
    if (mode == 'm') {
        return (faceWidth >= 22 && faceWidth <= 26) &&
               (faceHeight >= 22 && faceHeight <= 26);
    }
    if (mode == 'f') {
        return (faceWidth >= 1 && faceWidth <= 15) &&
               (faceHeight >= 1 && faceHeight <= 15);
    }
    // Invalid mode: return false
    return false;
}
#include <cassert>

int main() {
    // Near mode: valid boundaries
    assert(classifyFaceSize(30, 30, 'n') == true);
    assert(classifyFaceSize(50, 50, 'n') == true);
    assert(classifyFaceSize(40, 45, 'n') == true);
    // Near mode: invalid sizes
    assert(classifyFaceSize(29, 40, 'n') == false);
    assert(classifyFaceSize(51, 40, 'n') == false);
    assert(classifyFaceSize(40, 29, 'n') == false);
    assert(classifyFaceSize(40, 51, 'n') == false);

    // Middle mode: valid boundaries
    assert(classifyFaceSize(22, 22, 'm') == true);
    assert(classifyFaceSize(26, 26, 'm') == true);
    assert(classifyFaceSize(24, 25, 'm') == true);
    // Middle mode: invalid sizes
    assert(classifyFaceSize(21, 24, 'm') == false);
    assert(classifyFaceSize(27, 24, 'm') == false);
    assert(classifyFaceSize(24, 21, 'm') == false);
    assert(classifyFaceSize(24, 27, 'm') == false);

    // Far mode: valid boundaries
    assert(classifyFaceSize(1, 1, 'f') == true);
    assert(classifyFaceSize(15, 15, 'f') == true);
    assert(classifyFaceSize(10, 5, 'f') == true);
    // Far mode: invalid sizes
    assert(classifyFaceSize(0, 10, 'f') == false);
    assert(classifyFaceSize(16, 10, 'f') == false);
    assert(classifyFaceSize(10, 0, 'f') == false);
    assert(classifyFaceSize(10, 16, 'f') == false);

    // Invalid modes and edge values
    assert(classifyFaceSize(30, 30, 'x') == false);
    assert(classifyFaceSize(30, 30, 'N') == false); // uppercase not accepted
    assert(classifyFaceSize(-5, 40, 'n') == false); // negative dimensions
    assert(classifyFaceSize(30, 30, '\0') == false); // null character

    return 0;
}
