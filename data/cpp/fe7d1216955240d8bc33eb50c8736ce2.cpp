Write a C++ function named `updateAndReport` that takes two integer pointers as parameters. The function must replace the value pointed to by the first pointer with the sum of the two original pointed values, replace the value pointed to by the second pointer with the absolute difference of the two original pointed values, and then return a string in the format `"a=<newA>, b=<newB>"` (without quotes), where `<newA>` and `<newB>` are the updated integer values. The function should handle cases where the absolute difference of two negative numbers could overflow; in such cases, assume the input values are within the range `[-10^6, 10^6]` so that no overflow occurs. The function must not modify any other global state and must be safe to call multiple times.

The solution first dereferences both pointers to capture the original values (`origA` and `origB`). Then compute `newA = origA + origB`. For `newB`, compute the absolute difference using `abs(origA - origB)`. Since the input range is bounded to ±10^6, the subtraction and addition cannot exceed 2×10^6, which fits comfortably in a 32-bit `int` (max ~2.1×10^9). We then assign the new values to the pointed locations and construct a string using `std::to_string`. Edge case: if either pointer is null, the function would dereference a null pointer and crash, so the caller must ensure valid pointers (the task implies non-null pointers). Another edge case: if the original values are equal, the absolute difference is zero, which is fine. The algorithm runs in O(1) time and O(1) auxiliary space (the returned string uses O(log N) space for digits, but that is negligible and constant since N is bounded by 10^6).

#include <string>
#include <cstdlib>

// Updates the values pointed to by a and b to (a+b) and abs(a-b) respectively.
// Returns a string describing the new values in the format "a=<newA>, b=<newB>".
std::string updateAndReport(int* a, int* b) {
    const int originalA = *a;
    const int originalB = *b;
    const int newA = originalA + originalB;
    const int newB = std::abs(originalA - originalB);
    *a = newA;
    *b = newB;
    return "a=" + std::to_string(newA) + ", b=" + std::to_string(newB);
}

#include <cassert>
#include <string>

// Assume updateAndReport is defined as above.

int main() {
    int a1 = 3, b1 = 4;
    assert(updateAndReport(&a1, &b1) == "a=7, b=1");
    assert(a1 == 7 && b1 == 1);

    int a2 = -5, b2 = 2;
    assert(updateAndReport(&a2, &b2) == "a=-3, b=7");
    assert(a2 == -3 && b2 == 7);

    int a3 = -10, b3 = -20;
    assert(updateAndReport(&a3, &b3) == "a=-30, b=10");
    assert(a3 == -30 && b3 == 10);

    int a4 = 0, b4 = 0;
    assert(updateAndReport(&a4, &b4) == "a=0, b=0");
    assert(a4 == 0 && b4 == 0);

    int a5 = 1000000, b5 = -1000000;
    assert(updateAndReport(&a5, &b5) == "a=0, b=2000000");
    assert(a5 == 0 && b5 == 2000000);

    int a6 = 7, b6 = 7;
    assert(updateAndReport(&a6, &b6) == "a=14, b=0");
    assert(a6 == 14 && b6 == 0);

    int a7 = -1, b7 = 1;
    assert(updateAndReport(&a7, &b7) == "a=0, b=2");
    assert(a7 == 0 && b7 == 2);

    int a8 = 123, b8 = -456;
    assert(updateAndReport(&a8, &b8) == "a=-333, b=579");
    assert(a8 == -333 && b8 == 579);
}
