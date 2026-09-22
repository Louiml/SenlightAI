Write a C++ function `bool isEligibleForTeaDiscount(bool isStudent, int numberOfCups)` that determines whether a customer qualifies for a tea subscription discount. The discount applies if the customer is a student OR has purchased more than 15 cups of tea. The function should take a boolean indicating student status (true = student, false = not) and an integer representing the number of cups purchased. The function must return `true` if eligible for the discount, and `false` otherwise. Handle edge cases such as zero or negative cup counts (which should not be eligible unless the person is a student), and ensure the logic correctly treats exactly 15 cups as not eligible (must be strictly greater than 15). The function should be `const` correct and not modify its inputs.

#include <cassert>

// Declaration of the solution function (assumed to be included from elsewhere)
bool isEligibleForTeaDiscount(bool isStudent, int numberOfCups);

int main() {
    // Student, any cup count -> eligible
    assert(isEligibleForTeaDiscount(true, 0) == true);
    assert(isEligibleForTeaDiscount(true, 15) == true);
    assert(isEligibleForTeaDiscount(true, -5) == true);

    // Not student, cup count <= 15 -> not eligible
    assert(isEligibleForTeaDiscount(false, 15) == false);
    assert(isEligibleForTeaDiscount(false, 0) == false);
    assert(isEligibleForTeaDiscount(false, -3) == false);

    // Not student, cup count > 15 -> eligible
    assert(isEligibleForTeaDiscount(false, 16) == true);
    assert(isEligibleForTeaDiscount(false, 100) == true);

    // Boundary test: exactly 15 cups and not a student should be false
    assert(isEligibleForTeaDiscount(false, 15) == false);

    return 0;
}

#include <cstdbool>

// Determine if a customer qualifies for the tea subscription discount.
// The discount applies if the customer is a student OR has purchased more than 15 cups.
bool isEligibleForTeaDiscount(bool isStudent, int numberOfCups) {
    return isStudent || numberOfCups > 15;
}

// The solution is straightforward: apply the logical OR (`||`) operator between the student status and the condition `numberOfCups > 15`. This directly implements the discount rule. The main edge case is the boundary at exactly 15 cups—since the condition is strictly greater than 15, a customer with 15 cups and not a student is not eligible. Negative or zero cup counts also do not satisfy `> 15`, so they are handled naturally. No additional validation is needed since the inputs are assumed to be provided by the caller. The time complexity is \(O(1)\) constant time, and the space complexity is \(O(1)\) as only primitive types are used. The function is `const` correct because it does not modify any input parameters.
