// Write a C++ function named `monthsToPayOffLoan` that takes two integer parameters: `loanAmount` and `monthlyPayment`. The function must return the number of months needed to pay off the loan as an integer, computed as integer division (floor) of `loanAmount / monthlyPayment`. Assume both inputs are positive integers and `monthlyPayment` is non-zero. The function must handle the case where `loanAmount` is exactly divisible by `monthlyPayment` correctly, and it should not perform any rounding—only truncation toward zero. Your solution must be a self-contained free function (no `main`), with proper `const` correctness where applicable, and must not use any external libraries beyond standard headers.
// The core algorithm is straightforward: divide the loan amount by the monthly payment using integer division, which truncates any fractional part toward zero. This yields the number of full months needed, assuming payments are made in whole-month increments and no interest is considered. Edge cases: (1) If `loanAmount` is less than `monthlyPayment`, the result is 0 because the floor of a fraction less than 1 is 0—this would theoretically mean the loan is paid in less than a month, but the problem asks for months, so we return 0 (note: in the original snippet, this would produce 0, which is acceptable). (2) If `loanAmount` equals `monthlyPayment`, the result is 1 because the division is exact. (3) If `monthlyPayment` is larger than `loanAmount`, we must ensure we don't divide by zero—but the assumption states it's non-zero, so no special handling is needed. Time complexity is \(O(1)\) because only a single division operation is performed. Space complexity is \(O(1)\) as no additional data structures are used.
#include <cstdint> // for int64_t to avoid overflow? But problem says int, so we'll use int

// Compute the number of whole months to pay off a loan with integer division.
// Both parameters are assumed positive integers, and monthlyPayment is non-zero.
int monthsToPayOffLoan(int loanAmount, int monthlyPayment) {
    // Integer division truncates toward zero; since both are positive, this is floor.
    return loanAmount / monthlyPayment;
}
int main() {
    // Basic cases
    assert(monthsToPayOffLoan(1000, 100) == 10);
    assert(monthsToPayOffLoan(1000, 300) == 3);  // 3 full months, remainder 100
    assert(monthsToPayOffLoan(1000, 1000) == 1);
    assert(monthsToPayOffLoan(1000, 2000) == 0); // Loan less than payment
    
    // Edge cases
    assert(monthsToPayOffLoan(1, 1) == 1);
    assert(monthsToPayOffLoan(0, 5) == 0); // Zero loan amount
    assert(monthsToPayOffLoan(500, 7) == 71); // 500/7 = 71.428..., truncated to 71
    
    // Larger values
    assert(monthsToPayOffLoan(1000000, 123) == 8130); // 1000000/123 = 8130.08..., truncated to 8130
    
    // Exact divisibility
    assert(monthsToPayOffLoan(1200, 400) == 3);
    
    // Very small payment
    assert(monthsToPayOffLoan(10, 1) == 10);
}
