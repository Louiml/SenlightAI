/*
Write a C++ function that takes two positive integers and returns a `std::pair<int, int>` containing their Highest Common Factor (HCF, also known as GCD) and Least Common Multiple (LCM), in that order. The function must compute the HCF using a simple loop that checks all integers from 1 up to the smaller of the two inputs (or the larger, as in the original snippet, but for efficiency use the smaller), and then compute the LCM as the product of the two numbers divided by their HCF. The function must handle the case where either input is 0 gracefully (returning HCF = 0, LCM = 0), and must not assume any particular ordering of the inputs. The solution should be self-contained, use `const` where appropriate, and be testable with `assert` statements.
*/

#include <utility> // for std::pair

// Computes the HCF and LCM of two positive integers.
// Returns {0,0} if either input is 0.
std::pair<int, int> computeHcfAndLcm(int a, int b) {
    if (a == 0 || b == 0) {
        return {0, 0};
    }

    const int limit = (a < b) ? a : b; // only need to check up to the smaller
    int hcf = 1;

    for (int i = 1; i <= limit; ++i) {
        if (a % i == 0 && b % i == 0) {
            hcf = i; // because i increases, last found is the largest
        }
    }

    const int lcm = (a * b) / hcf;
    return {hcf, lcm};
}

#include <cassert>
#include <utility>

// The solution function is declared above; here we test it.

int main() {
    // Basic cases
    std::pair<int, int> result1 = computeHcfAndLcm(12, 18);
    assert(result1.first == 6);
    assert(result1.second == 36);

    std::pair<int, int> result2 = computeHcfAndLcm(7, 13);
    assert(result2.first == 1);
    assert(result2.second == 91);

    std::pair<int, int> result3 = computeHcfAndLcm(4, 4);
    assert(result3.first == 4);
    assert(result3.second == 4);

    // One input is 1
    std::pair<int, int> result4 = computeHcfAndLcm(1, 100);
    assert(result4.first == 1);
    assert(result4.second == 100);

    // Multiples
    std::pair<int, int> result5 = computeHcfAndLcm(4, 12);
    assert(result5.first == 4);
    assert(result5.second == 12);

    // One input is 0
    std::pair<int, int> result6 = computeHcfAndLcm(0, 5);
    assert(result6.first == 0);
    assert(result6.second == 0);

    // Large numbers (still fits in int)
    std::pair<int, int> result7 = computeHcfAndLcm(100000, 100000);
    assert(result7.first == 100000);
    assert(result7.second == 100000);

    // Reversed order should give same result
    std::pair<int, int> result8 = computeHcfAndLcm(18, 12);
    assert(result8.first == result1.first);
    assert(result8.second == result1.second);

    return 0;
}

// The core algorithm is straightforward:  
// 1. If either input is 0, return `{0, 0}` because HCF(0, anything) is 0 (by convention) and LCM would be undefined (we return 0).  
// 2. Otherwise, determine the smaller of the two inputs (call it `limit`). The HCF is the largest integer `i` in `[1, limit]` that divides both inputs exactly. Start with `hcf = 1` and loop `i` from 1 to `limit`, updating `hcf` whenever `i` divides both `a` and `b`. This is inefficient but matches the spirit of the original snippet.  
// 3. The LCM is then `(a * b) / hcf`. Since `hcf` divides both, this product division is exact.  
// 4. Edge cases:  
//    - One input is 0 → return `{0,0}`.  
//    - Both inputs equal → HCF = that value, LCM = that value.  
//    - One input is 1 → HCF = 1, LCM = the other input.  
//    - Inputs that are multiples (e.g., 4 and 12) → HCF = 4, LCM = 12.  
// 5. Complexity: The loop runs `min(a,b)` times, so O(min(a,b)) time, O(1) auxiliary space.
