// Write a C++ function `determineWinner` that takes six integers representing the number of magical potions in each of six cauldrons (a, b, c, d, e, f) and returns a string `"Ron"` or `"Hermione"` based on a certain rule. The rule states: if (a is zero and b is non-zero and d is non-zero) OR (c is zero and d is non-zero) OR (a * c * e is less than b * d * f), then the winner is `"Ron"`; otherwise it is `"Hermione"`. This problem is inspired by a classic riddle where Ron can win if there's any path that allows him to get "infinite" value from finite starting resources. The function should handle all integer inputs (including negative, zero, and positive values) by evaluating the boolean expression exactly as given, without shortcuts or assumptions about the magnitudes of products.

The solution directly translates the given logical condition into C++ code. The condition `(!a && b && d) || (!c && d) || (a*c*e < b*d*f)` must be evaluated carefully. Important edge cases: when `a` is zero, the first sub-condition checks that both `b` and `d` are non-zero; when `c` is zero, the second sub-condition checks that `d` is non-zero. The third sub-condition compares two products directly, which may overflow for large integer inputs — but since we are only comparing them and the values are within typical 32-bit integers, the product may exceed 32-bit range. To be safe, we should perform the multiplication using `long long` (or `int64_t`) to avoid undefined behavior from integer overflow. The time complexity is O(1) and space complexity is O(1), as we only perform constant-time arithmetic and boolean operations.

#include <cstdint>

// Determine whether Ron or Hermione wins based on the given potion counts.
std::string determineWinner(int a, int b, int c, int d, int e, int f) {
    bool firstCondition = (a == 0) && (b != 0) && (d != 0);
    bool secondCondition = (c == 0) && (d != 0);
    
    // Use int64_t to avoid overflow in multiplication comparison.
    int64_t leftProduct = static_cast<int64_t>(a) * c * e;
    int64_t rightProduct = static_cast<int64_t>(b) * d * f;
    bool thirdCondition = leftProduct < rightProduct;
    
    if (firstCondition || secondCondition || thirdCondition) {
        return "Ron";
    } else {
        return "Hermione";
    }
}

#include <cassert>
#include <string>
#include "determineWinner.h"

int main() {
    // Basic cases from the original snippet
    assert(determineWinner(0, 1, 0, 1, 1, 1) == "Ron"); // !a&&b&&d true (a=0,b!=0,d!=0)
    assert(determineWinner(0, 0, 1, 1, 1, 1) == "Hermione"); // a=0 but b=0, c!=0, product 0<1? false
    
    // Condition 2: c=0 and d!=0
    assert(determineWinner(1, 1, 0, 1, 1, 1) == "Ron");
    assert(determineWinner(1, 1, 0, 0, 1, 1) == "Hermione"); // d=0 so condition false, product 0<0? false
    
    // Condition 3: product comparison
    assert(determineWinner(2, 3, 4, 1, 1, 1) == "Hermione"); // 2*4*1=8 < 3*1*1=3? false
    assert(determineWinner(1, 1, 1, 2, 3, 4) == "Ron"); // 1*1*3=3 < 1*2*4=8? true
    
    // Mixed conditions with zeros and negatives
    assert(determineWinner(0, 5, 1, 0, -1, 1) == "Hermione"); // a=0,b!=0,d=0 -> false; c!=0; product 0*-1*1=0 < 5*0*1=0? false
    assert(determineWinner(-1, -2, -3, -4, -5, -6) == "Hermione"); // products: (-1*-3*-5)=-15 < (-2*-4*-6)=-48? false (since -15 > -48)
    assert(determineWinner(-1, 1, 1, 1, 1, -1) == "Hermione"); // products: -1*1*1=-1 < 1*1*-1=-1? false
    
    // Large values to check overflow safety
    assert(determineWinner(100000, 1, 100000, 1, 100000, 1) == "Hermione"); // 1e15 < 1? false
    assert(determineWinner(1, 100000, 1, 100000, 1, 100000) == "Ron"); // 1 < 1e15? true
    
    // Zero products edge cases
    assert(determineWinner(0, 0, 0, 0, 0, 0) == "Hermione");
    assert(determineWinner(1, 0, 1, 0, 1, 0) == "Hermione");
    
    // Exact equality of products
    assert(determineWinner(2, 2, 3, 3, 4, 4) == "Hermione"); // 2*3*4=24, 2*3*4=24, not less
    
    return 0;
}
