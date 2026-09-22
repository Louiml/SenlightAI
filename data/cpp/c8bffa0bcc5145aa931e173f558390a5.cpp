/*
Write a C++ function named `determineCategory` that takes two floating-point values representing prices (with default arguments of `0.0f` for both) and returns a `std::string`. The function should compute the sum of the two prices. If the sum is less than or equal to `100.76f`, return `"Orange"`. Otherwise, return the string `"Blue\nGreen"` (with a newline character between the words). The function must be const-correct where applicable (e.g., parameters passed by value are already const, but you should not modify them). Handle edge cases such as negative prices, zero, and very large values (within float range). Do not use any I/O inside the function — it should purely compute and return the result.
*/
#include <string>

// Returns "Orange" if the sum of two prices is <= 100.76, otherwise "Blue\nGreen".
std::string determineCategory(float price1 = 0.0f, float price2 = 0.0f) {
    const float threshold = 100.76f;
    const float total = price1 + price2;

    if (total <= threshold) {
        return "Orange";
    }
    return "Blue\nGreen";
}
#include <cassert>
#include <string>

// Free function declaration (solution from above)
std::string determineCategory(float price1, float price2);

int main() {
    // Basic cases
    assert(determineCategory(50.0f, 50.0f) == "Orange");      // 100.0 <= 100.76
    assert(determineCategory(100.0f, 0.76f) == "Orange");    // 100.76 exactly
    assert(determineCategory(100.0f, 1.0f) == "Blue\nGreen"); // 101.0 > 100.76
    // Edge: zero and negative
    assert(determineCategory(0.0f, 0.0f) == "Orange");
    assert(determineCategory(-100.0f, -50.0f) == "Orange"); // sum -150
    // Edge: large values
    assert(determineCategory(1e38f, 1e38f) == "Blue\nGreen"); // overflow to inf
    // Default arguments
    assert(determineCategory() == "Orange");
    assert(determineCategory(100.0f) == "Orange"); // second default 0
    assert(determineCategory(200.0f) == "Blue\nGreen");
    // Boundary: just above threshold
    assert(determineCategory(100.76f, 0.0001f) == "Blue\nGreen");
    return 0;
}
// The core algorithm is straightforward: accept two `float` parameters, sum them, and compare the result against a constant threshold `100.76f`. Since the sum is a simple arithmetic operation, there are no loops or recursion. Edge cases include:  
// - When both prices are zero, the sum is `0.0f`, which is ≤ threshold, so return `"Orange"`.  
// - When prices are negative, the sum could be negative or lower than threshold, still returning `"Orange"`.  
// - When one price is very large (e.g., `FLT_MAX`), the sum might overflow to infinity — in that case, the comparison with `100.76f` will still correctly evaluate to false (since infinity > threshold), returning `"Blue\nGreen"`.  
// - Floating-point precision: the threshold `100.76f` is not exactly representable in binary floating point, but using the same literal in both the function and test avoids issues.  
// Time complexity is O(1) and space complexity is O(1) (only the returned string allocation is needed). No external data structures are used.
