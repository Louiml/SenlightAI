/*
Write a C++ function named `classifyGlucoseLevel` that takes an integer representing a person's blood glucose level (in mg/dL) and returns a string classification according to the following medical ranges: if the value is less than 70, return `"Hypoglycemia"`; if the value is between 70 and 180 inclusive, return `"Normal"`; if the value is greater than 180, return `"Hyperglycemia"`. Note that the given snippet had a logical error (it printed "Hyperghlycemia" for the normal range and never handled values exactly between 70 and 180 correctly) — your implementation must correct this by using the boundaries as specified above. Assume the input is any valid integer (negative, zero, or positive) and handle all cases without crashing.
*/
#include <string>

// Classify a blood glucose level (mg/dL) into a medical category.
// Returns "Hypoglycemia" if < 70, "Normal" if 70 <= level <= 180,
// and "Hyperglycemia" if > 180.
std::string classifyGlucoseLevel(int glucose) {
    if (glucose < 70) {
        return "Hypoglycemia";
    } else if (glucose <= 180) {
        return "Normal";
    } else {
        return "Hyperglycemia";
    }
}
#include <cassert>
#include <string>

// Assume the solution function is declared above or included here.
// (For standalone test, the function definition must be visible.)
std::string classifyGlucoseLevel(int glucose); // declaration for clarity

int main() {
    // Below 70
    assert(classifyGlucoseLevel(0) == "Hypoglycemia");
    assert(classifyGlucoseLevel(69) == "Hypoglycemia");
    assert(classifyGlucoseLevel(-100) == "Hypoglycemia");

    // Normal range (70 to 180 inclusive)
    assert(classifyGlucoseLevel(70) == "Normal");
    assert(classifyGlucoseLevel(100) == "Normal");
    assert(classifyGlucoseLevel(180) == "Normal");

    // Above 180
    assert(classifyGlucoseLevel(181) == "Hyperglycemia");
    assert(classifyGlucoseLevel(300) == "Hyperglycemia");
    assert(classifyGlucoseLevel(2000) == "Hyperglycemia");

    // Boundary sanity
    assert(classifyGlucoseLevel(69) != "Normal");
    assert(classifyGlucoseLevel(180) != "Hyperglycemia");
    return 0;
}
// The solution is a straightforward conditional classification. The function receives an integer `glucose` and compares it against the two thresholds: 70 and 180. Since the conditions are mutually exclusive and cover all integers, we use an if-else chain. The first check `glucose < 70` catches all values below the lower bound. The second check `glucose <= 180` catches all values from 70 up to and including 180 (because if the first condition is false, we know `glucose >= 70`). The final `else` catches all values greater than 180. No special edge cases exist beyond ensuring the comparisons use `<=` for the upper bound (not `<`) and that the lower bound is exclusive (i.e., `< 70`, not `<= 70`). The time complexity is O(1) constant, and space complexity is O(1) for the returned string (the string content is fixed length).
