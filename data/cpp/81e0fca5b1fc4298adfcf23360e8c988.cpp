Write a C++ function that takes an integer rating `R` (1 ≤ R ≤ 5000) and returns a string representing the contest class based on the following rules: if R < 1200, return `"ABC"`; if 1200 ≤ R < 2800, return `"ARC"`; otherwise (R ≥ 2800), return `"AGC"`. The function should handle the exact boundary values correctly and be robust for all valid inputs within the given range.

#include <cassert>
#include <string>

// The solution function (include above or in same file)
std::string contestClass(int rating);

int main() {
    // Boundary tests
    assert(contestClass(1) == "ABC");
    assert(contestClass(1199) == "ABC");
    assert(contestClass(1200) == "ARC");
    assert(contestClass(2799) == "ARC");
    assert(contestClass(2800) == "AGC");
    
    // Representative interior tests
    assert(contestClass(500) == "ABC");
    assert(contestClass(2000) == "ARC");
    assert(contestClass(3500) == "AGC");
    assert(contestClass(5000) == "AGC");
    
    return 0;
}

#include <string>

// Return the contest class based on the given rating.
std::string contestClass(int rating) {
    if (rating < 1200) {
        return "ABC";
    } else if (rating < 2800) {
        return "ARC";
    } else {
        return "AGC";
    }
}

// The solution is straightforward: compare the input rating against the two critical thresholds (1200 and 2800). Since the ranges are contiguous and cover all possible inputs in the domain, we can use a simple if-else chain. The first check handles ratings below 1200, the second handles ratings from 1200 up to but not including 2800, and the final else captures ratings 2800 and above. Edge cases include exactly 1200 (should give "ARC") and exactly 2800 (should give "AGC"). No special handling for negative values is needed since the domain restricts to positive integers starting at 1. Time complexity is O(1) because there is a constant number of comparisons, and space complexity is O(1) since only a fixed-size string is returned (the strings are string literals or constants).
