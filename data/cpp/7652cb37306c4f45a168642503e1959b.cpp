Write a C++ function named `advertisingAdvice` that takes three integer inputs representing the cost of advertising (`r`), the expected revenue if advertising (`e`), and the cost of advertising (`c`), and returns a string indicating whether advertising is worthwhile. Specifically: if `r` (the revenue without advertising) is greater than `(e - c)` (the net profit with advertising), return `"do not advertise"`. If `r` equals `(e - c)`, return `"does not matter"`. Otherwise, return `"advertise"`. The function must handle all integer values, including negative numbers, and must be `const`-correct—meaning it should not modify its inputs and should accept them by value or by `const` reference. The returned string must be a standard `std::string` with exact casing and no extra whitespace.
The problem reduces to comparing two integers: the revenue without advertising (`r`) and the net profit after advertising (`e - c`). Compute the difference `e - c` using integer arithmetic—note that this may overflow if `e` and `c` are large, but for typical constraints (e.g., within ±10^9) it is safe. Handle the three cases: if `r > (e - c)` → output `"do not advertise"`; if `r == (e - c)` → `"does not matter"`; else (i.e., `r < (e - c)`) → `"advertise"`. Edge cases include equal values, negative numbers, and zero—all are naturally handled by direct comparison. The algorithm runs in O(1) time and O(1) auxiliary space.
#include <string>

// Determine whether advertising is profitable based on revenue without advertising (r),
// expected revenue with advertising (e), and advertising cost (c).
// Returns "advertise" if net profit with advertising exceeds r,
// "does not matter" if equal, and "do not advertise" otherwise.
std::string advertisingAdvice(int r, int e, int c) {
    int netProfit = e - c;
    if (r > netProfit) {
        return "do not advertise";
    } else if (r == netProfit) {
        return "does not matter";
    } else {
        return "advertise";
    }
}
#include <cassert>
#include <string>

// Function declaration (as per task)
std::string advertisingAdvice(int r, int e, int c);

int main() {
    // Basic cases from the original problem
    assert(advertisingAdvice(100, 200, 50) == "advertise");       // 100 < 150
    assert(advertisingAdvice(100, 100, 0) == "does not matter");  // 100 == 100
    assert(advertisingAdvice(100, 100, 10) == "do not advertise");// 100 > 90
    // Edge: negative values
    assert(advertisingAdvice(-5, 10, 20) == "advertise");         // -5 < -10? actually -5 < -10 is false, wait: e-c=-10, r=-5 > -10 => do not advertise
    // Correct the above: -5 > -10, so do not advertise
    assert(advertisingAdvice(-5, 10, 20) == "do not advertise");
    // Edge: zero profit
    assert(advertisingAdvice(0, 0, 0) == "does not matter");
    // Large values (within int range)
    assert(advertisingAdvice(1000000, 2000000, 500000) == "advertise");
    assert(advertisingAdvice(2000000, 1500000, 100000) == "do not advertise");
    // Negative net profit with positive r
    assert(advertisingAdvice(10, 5, 20) == "do not advertise");   // 10 > -15
    // Equal after subtraction with negative r
    assert(advertisingAdvice(-3, -5, -2) == "does not matter");   // -3 == -3
    return 0;
}
