/*
Write a C++ function named `polygonName` that takes an integer representing the number of sides of a regular polygon and returns a `std::string` with the English name of that polygon. The function must support side counts from 3 to 10 inclusive, returning the lowercase names: "triangle", "quadrilateral" (use "square" since the snippet uses "quadrado" but for regular polygons with 4 sides it is a square), "pentagon", "hexagon", "heptagon", "octagon", "nonagon", and "decagon". If the input is outside the range 3–10, the function must return an empty string `""` to signal an error. The function should be standalone, const-correct, and not rely on any global state.
*/
#include <string>
#include <array>

// Return the English name of a regular polygon with the given number of sides.
// Supports sides from 3 to 10 inclusive. Returns an empty string for invalid inputs.
std::string polygonName(int sides) {
    const std::array<std::string, 8> names = {
        "triangle", // 3 sides
        "square",   // 4 sides
        "pentagon", // 5 sides
        "hexagon",  // 6 sides
        "heptagon", // 7 sides
        "octagon",  // 8 sides
        "nonagon",  // 9 sides
        "decagon"   // 10 sides
    };

    if (sides < 3 || sides > 10) {
        return "";
    }

    return names[sides - 3];
}
#include <cassert>
#include <string>

// The solution function is declared above; include it in the same translation unit.
std::string polygonName(int sides); // Declaration for clarity

int main() {
    // Test valid edge cases and interior values.
    assert(polygonName(3) == "triangle");
    assert(polygonName(4) == "square");
    assert(polygonName(5) == "pentagon");
    assert(polygonName(6) == "hexagon");
    assert(polygonName(7) == "heptagon");
    assert(polygonName(8) == "octagon");
    assert(polygonName(9) == "nonagon");
    assert(polygonName(10) == "decagon");

    // Test invalid inputs.
    assert(polygonName(2) == "");
    assert(polygonName(11) == "");
    assert(polygonName(0) == "");
    assert(polygonName(-3) == "");
}
// The solution maps the number of sides to a polygon name using a fixed lookup structure. The simplest approach is to use a `switch` statement or a `std::array<std::string, 8>` indexed by `(sides - 3)`. Since the valid range is small and contiguous, an array lookup is efficient and avoids repetitive branching. The function first checks if the input is within the valid range; if not, it returns an empty string immediately. Otherwise, it computes the index as `sides - 3` and returns the corresponding name from the pre-initialized array. Time complexity is O(1) for the range check and array access. Space complexity is O(1) since the array is fixed-size constant. Edge cases: exactly 3 and 10 are valid boundaries; any value below 3 or above 10 results in an empty string. The use of `const` for the name array and for the parameter ensures correctness and clarity.
