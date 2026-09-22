Write a C++ function named `classifyDifficulty` that takes an integer `x` representing a problem's difficulty score and returns a `std::string` describing its category: `"Easy"` if `x` is in the inclusive range [1, 99], `"Medium"` if `x` is in the inclusive range [100, 199], and `"Hard"` if `x` is in the inclusive range [200, 300]. If `x` is outside all these ranges, the function must return `"Unknown"`. The function must be `const`-correct and not modify the input. Provide a self-contained solution with proper headers and comments, and then validate it with `assert` checks in a separate `main` function that tests several boundary and invalid values.

The solution is straightforward: compare the input integer `x` against the three specified ranges using simple conditional statements. The main algorithm is a constant-time decision tree with three checks. Key edge cases include the boundaries exactly at 1, 99, 100, 199, 200, and 300, all of which must be included in their respective ranges as stated. Values below 1, above 300, or in the gaps (e.g., 0, 301, negative values) must return `"Unknown"`. Since the ranges are disjoint and cover only [1,300] in three contiguous intervals, no overlapping conditions are needed. Time complexity is O(1) and space complexity is O(1) (excluding the returned string). The function should be `const`-qualified in the sense it does not modify any state (though it has no member variables, it should be a free function taking `int` by value).

#include <string>

// Returns the difficulty category for a given problem score.
// - "Easy"   if 1 <= x <= 99
// - "Medium" if 100 <= x <= 199
// - "Hard"   if 200 <= x <= 300
// - "Unknown" otherwise
std::string classifyDifficulty(const int x) {
    if (x >= 1 && x <= 99) {
        return "Easy";
    }
    if (x >= 100 && x <= 199) {
        return "Medium";
    }
    if (x >= 200 && x <= 300) {
        return "Hard";
    }
    return "Unknown";
}

#include <cassert>
#include <string>
#include "solution.h" // or just paste the function here if not using header

int main() {
    // Boundary values for Easy
    assert(classifyDifficulty(1) == "Easy");
    assert(classifyDifficulty(50) == "Easy");
    assert(classifyDifficulty(99) == "Easy");
    
    // Boundary values for Medium
    assert(classifyDifficulty(100) == "Medium");
    assert(classifyDifficulty(150) == "Medium");
    assert(classifyDifficulty(199) == "Medium");
    
    // Boundary values for Hard
    assert(classifyDifficulty(200) == "Hard");
    assert(classifyDifficulty(250) == "Hard");
    assert(classifyDifficulty(300) == "Hard");
    
    // Invalid values
    assert(classifyDifficulty(0) == "Unknown");
    assert(classifyDifficulty(-1) == "Unknown");
    assert(classifyDifficulty(301) == "Unknown");
    assert(classifyDifficulty(1000) == "Unknown");
    
    return 0;
}
