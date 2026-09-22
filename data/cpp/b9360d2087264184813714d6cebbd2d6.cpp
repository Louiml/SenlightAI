Write a C++ function `std::string divisionRank(ll n)` that takes a positive integer up to 10^9 (representing a contestant's rating) and returns the string `"Division 1"`, `"Division 2"`, `"Division 3"`, or `"Division 4"` based on the rating: Division 1 if rating ≥ 1900, Division 2 if 1600 ≤ rating < 1900, Division 3 if 1400 ≤ rating < 1600, and Division 4 if rating < 1400. The function must handle ratings that may be negative or zero (though in practice ratings are non‑negative) and return exactly the required strings. The solution should be self‑contained and not rely on global variables.

#include <cassert>
#include <string>

// (declaration of divisionRank is assumed to be available)
// For completeness, include the function here: 
std::string divisionRank(long long rating) {
    if (rating >= 1900) return "Division 1";
    else if (rating >= 1600) return "Division 2";
    else if (rating >= 1400) return "Division 3";
    else return "Division 4";
}

int main() {
    // Boundary cases
    assert(divisionRank(1900) == "Division 1");
    assert(divisionRank(1899) == "Division 2");
    assert(divisionRank(1600) == "Division 2");
    assert(divisionRank(1599) == "Division 3");
    assert(divisionRank(1400) == "Division 3");
    assert(divisionRank(1399) == "Division 4");

    // Typical values
    assert(divisionRank(2500) == "Division 1");
    assert(divisionRank(1700) == "Division 2");
    assert(divisionRank(1500) == "Division 3");
    assert(divisionRank(1200) == "Division 4");

    // Negative and zero (not typical but allowed)
    assert(divisionRank(0) == "Division 4");
    assert(divisionRank(-100) == "Division 4");

    // Large values
    assert(divisionRank(1000000000LL) == "Division 1");

    return 0;
}

#include <string>

// Return the division string for a given rating.
// Division 1: rating >= 1900
// Division 2: 1600 <= rating < 1900
// Division 3: 1400 <= rating < 1600
// Division 4: rating < 1400
std::string divisionRank(long long rating) {
    if (rating >= 1900) {
        return "Division 1";
    } else if (rating >= 1600) {
        return "Division 2";
    } else if (rating >= 1400) {
        return "Division 3";
    } else {
        return "Division 4";
    }
}

// The problem is a straightforward piecewise constant classification. The main algorithm checks the rating against three thresholds in descending order: 1900, 1600, 1400. If the rating is greater than or equal to the highest threshold, we return Division 1; otherwise we move to the next threshold. This is equivalent to a nested conditional or a series of `if`‑`else if` statements. Edge cases include ratings exactly equal to a threshold (e.g., 1900 → Division 1, 1600 → Division 2, 1400 → Division 3) and ratings below 1400 (including negative numbers) → Division 4. The time complexity is O(1) because only a constant number of comparisons are performed. The space complexity is O(1) since we only store the result string (which is small and fixed size). No additional data structures are needed.
