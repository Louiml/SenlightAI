/*
Write a C++ function `tollCost` that takes two integer parameters: the number of passengers `people` (ranging from 1 to 100) and the base fare `fare` (a positive integer), and returns the total toll cost (an integer). The toll is calculated as follows: if there are at least 3 passengers, the cost is twice the base fare; if exactly 2 passengers, the cost equals the base fare; if only 1 passenger, the cost is 0 (free). The function must handle all valid inputs within the specified ranges and return the correct integer result. The function should be pure (no side effects, no I/O) and use `const` parameters.
*/
// Calculate toll cost based on number of passengers and base fare.
int tollCost(const int people, const int fare) {
    if (people >= 3) {
        return 2 * fare;  // Two times fare for 3 or more passengers
    }
    if (people == 2) {
        return fare;  // Exactly the fare for 2 passengers
    }
    return 0;  // Free for 1 passenger (or any unexpected non-positive)
}
int main() {
    assert(tollCost(3, 10) == 20);   // 3 passengers
    assert(tollCost(5, 7) == 14);    // more than 3
    assert(tollCost(2, 25) == 25);   // exactly 2
    assert(tollCost(1, 100) == 0);   // 1 passenger
    assert(tollCost(100, 1) == 2);   // max passengers, min fare
    assert(tollCost(4, 0) == 0);     // edge case with zero fare (though positive expected)
    assert(tollCost(2, 0) == 0);     // zero fare with 2 passengers
    assert(tollCost(1, 50) == 0);    // repetition of 1 passenger case
    assert(tollCost(3, 1) == 2);     // small fare with 3 passengers
    assert(tollCost(10, 99) == 198); // larger multiplication
}
// The solution is straightforward: use conditional branching based on the value of `people`. Since `people` is always between 1 and 100 (as per the constraint), we can directly check the three cases: `people >= 3` returns `2 * fare`; `people == 2` returns `fare`; and `people == 1` returns `0`. Edge cases: if `people` is 0 or negative (though not in the problem statement), the function could return 0 or handle gracefully; however, given the constraints, we assume valid input. For `fare` being positive, multiplication by 2 is safe. The algorithm uses only constant-time operations, so time complexity is `O(1)` and space complexity is `O(1)`.
