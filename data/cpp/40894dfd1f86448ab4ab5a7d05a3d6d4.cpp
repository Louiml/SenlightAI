Write a C++ function named `canLegsMatch` that takes three integer parameters: `noOfAdults`, `noOfKids`, and `noOfLegs`. It should return `true` if the total number of legs from all adults and kids (each adult has 2 legs, each kid has 2 legs) exactly equals the given `noOfLegs`, and `false` otherwise. The input values are non-negative integers. The function must be pure (no I/O), const-correct, and efficient.
The solution is straightforward: compute `totalLegs = (noOfAdults + noOfKids) * 2` (or equivalently `noOfAdults * 2 + noOfKids * 2`), then compare with `noOfLegs`. Edge cases include: zero adults/kids (total legs must be 0 to match), large values that could overflow if not using a wider type (but since parameters are `int`, the sum could overflow if both are very large; to be safe, use `long long` for the multiplication, or compare directly without storing). Time complexity is O(1) and space complexity is O(1). No special handling for negative numbers is required per the task description, but if they appeared, the comparison would still work logically (though such inputs are invalid per spec).
#include <cstddef> // for std::size_t (not strictly needed, but good practice)

// Returns true if the total number of legs (2 per adult, 2 per kid) equals noOfLegs.
bool canLegsMatch(int noOfAdults, int noOfKids, int noOfLegs) {
    // Use long long to avoid potential overflow from int multiplication/addition.
    const long long totalLegs = 2LL * noOfAdults + 2LL * noOfKids;
    return totalLegs == noOfLegs;
}
#include <cassert>

int main() {
    assert(canLegsMatch(2, 3, 10) == true);   // 4 + 6 = 10
    assert(canLegsMatch(1, 1, 4) == true);    // 2 + 2 = 4
    assert(canLegsMatch(0, 0, 0) == true);    // zero legs
    assert(canLegsMatch(5, 0, 10) == true);   // only adults
    assert(canLegsMatch(0, 7, 14) == true);   // only kids
    assert(canLegsMatch(2, 3, 11) == false);  // 10 != 11
    assert(canLegsMatch(1, 0, 3) == false);   // 2 != 3
    assert(canLegsMatch(0, 0, 1) == false);   // 0 != 1
    assert(canLegsMatch(1000000000, 1000000000, 4000000000LL) == true); // Overflow-safe (but int noOfLegs can't hold 4e9; adjust test to use long long? Since parameter is int, use a smaller large test)
    // The above line would not compile because noOfLegs is int; use a valid int:
    assert(canLegsMatch(1000000000, 0, 2000000000) == true); // 2e9 fits in int? 2e9 is within int range (2,147,483,647) yes.
    assert(canLegsMatch(1000000000, 1000000000, 2000000000) == false); // 4e9 > int max, but we compare with int 2e9, so false.
    return 0;
}
