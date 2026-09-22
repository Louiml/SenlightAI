Write a C++ function named `discountedAmount` that takes three integer parameters: `total` (the original price), `discount` (an integer percentage discount, e.g., 10 means 10%), and `x` (the actual paid amount). The function should compute and return the largest multiple of `discount` that is strictly less than or equal to the difference between `total` and `x`. In other words, if `diff = total - x`, return `(diff / discount) * discount` using integer division (truncating toward zero). The function must handle cases where `total < x` (negative difference) correctly, and also handle zero or negative `discount` by returning 0 (since division by zero or negative scaling is undefined for this problem). Assume all inputs are within the range of `int`.
The core idea is to compute the difference `diff = total - x`. Then we divide by `discount` using integer division to find how many full `discount` units fit into `diff`. Multiplying that quotient back by `discount` yields the largest multiple of `discount` that does not exceed `diff`. For positive `diff` and positive `discount`, this works naturally. Edge cases include `discount <= 0`: in that case, the concept of a "multiple" is not meaningful, so we return 0 to avoid division by zero or negative scaling. If `diff` is negative (when `x > total`), integer division in C++ truncates toward zero, so `(-7) / 10` yields `0`, and `0 * 10` yields `0`, which is a valid multiple. If `diff` is between 0 and `discount`, the quotient is 0, returning 0. The algorithm runs in O(1) time and uses O(1) auxiliary space, as it only performs a few arithmetic operations.
#include <algorithm> // for std::max (optional, but good practice)

// Compute the largest multiple of 'discount' that does not exceed (total - x).
// If 'discount' is non-positive, return 0 to avoid division by zero or negative scaling.
int discountedAmount(int total, int discount, int x) {
    if (discount <= 0) {
        return 0;
    }
    const int diff = total - x;
    const int quotient = diff / discount;
    return quotient * discount;
}
int main() {
    // Basic example: total=100, discount=10, x=37 => diff=63 => largest multiple of 10 <=63 is 60
    assert(discountedAmount(100, 10, 37) == 60);

    // Exact multiple: diff=50, discount=10 => 50
    assert(discountedAmount(100, 10, 50) == 50);

    // diff less than discount: diff=5, discount=10 => 0
    assert(discountedAmount(100, 10, 95) == 0);

    // negative diff: total=80, x=100 => diff=-20, integer division -20/10=-2, *10=-20
    assert(discountedAmount(80, 10, 100) == -20);

    // negative diff not multiple: total=85, x=100 => diff=-15, -15/10=-1, *10=-10
    assert(discountedAmount(85, 10, 100) == -10);

    // zero discount: should return 0
    assert(discountedAmount(100, 0, 37) == 0);

    // negative discount: should return 0
    assert(discountedAmount(100, -5, 37) == 0);

    // discount larger than diff: diff=3, discount=7 => 0
    assert(discountedAmount(10, 7, 7) == 0);

    // large values within int: use 2000000000 total, 1 discount, 1999999999 x => diff=1, quotient=1, *1=1
    assert(discountedAmount(2000000000, 1, 1999999999) == 1);

    // diff exactly 0: total=50, x=50, discount=5 => 0
    assert(discountedAmount(50, 5, 50) == 0);
}
