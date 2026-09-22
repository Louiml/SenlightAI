// You are organizing a shipment of three types of items: small, medium, and large. You have three boxes with fixed capacities: box A can hold `a` items, box B can hold `b` items, and box C can hold `c` items. You need to pack `x` small items, `y` medium items, and `z` large items into the three boxes according to these rules: small items must go into box A first, but if they don't all fit, the remaining small items spill into box B. After small items are placed, medium items are placed into whatever space remains in boxes A and B (starting with the remaining space in A, then any leftover in B). After all small and medium items are placed, large items go into box C. Write a C++ function `bool canShip(int x, int y, int z, int a, int b, int c)` that returns `true` if all items can be packed without exceeding any box's capacity, and `false` otherwise. Each item (regardless of type) takes exactly one unit of space in a box.
#include <cassert>

int main() {
    // Test basic fit
    assert(canShip(1, 1, 1, 1, 1, 1) == true);
    // Small items spill into B
    assert(canShip(2, 0, 0, 1, 1, 1) == true);
    // Small items exceed A+B
    assert(canShip(3, 0, 0, 1, 1, 1) == false);
    // Medium items exceed remaining after small
    assert(canShip(1, 2, 0, 1, 1, 1) == false);
    // Medium exactly fill remaining
    assert(canShip(1, 1, 0, 1, 1, 1) == true);
    // Large items exceed C
    assert(canShip(0, 0, 2, 0, 0, 1) == false);
    // Large items exactly fit
    assert(canShip(0, 0, 1, 0, 0, 1) == true);
    // Empty case
    assert(canShip(0, 0, 0, 0, 0, 0) == true);
    // Total capacity test
    assert(canShip(2, 2, 2, 3, 3, 2) == false);
    // Edge with zero capacities
    assert(canShip(0, 1, 0, 0, 1, 0) == true);
    assert(canShip(0, 2, 0, 0, 1, 0) == false);
}
#include <algorithm>

// Returns true if all items can be packed according to the given box capacities and placement rules.
bool canShip(int x, int y, int z, int a, int b, int c) {
    // Small items must fit in A and B combined.
    if (x > a + b) {
        return false;
    }

    // Medium items can use remaining space in A after small items, then remaining in B.
    // Total space available for medium is a + b - x.
    if (y > a + b - x) {
        return false;
    }

    // Large items go only into C.
    if (z > c) {
        return false;
    }

    // Additionally check total capacity (redundant but explicit).
    if (x + y + z > a + b + c) {
        return false;
    }

    return true;
}
// The problem is a simple feasibility check with a greedy assignment because the order of filling is fixed by the rules. First, check if small items fit in box A; if not, the excess must go into box B. So after placing small items, the remaining capacity in A is `a - x` (if non-negative) and the remaining capacity in B is `b - max(0, x - a)`. For medium items, they can use the remainder of A first, then the remainder of B. So the total available space for medium items is `(a - x) + (b - max(0, x - a))` which simplifies to `a + b - x` (provided `x <= a+b`, otherwise we already fail). So if `y > a + b - x`, return false. For large items, they only go in box C, so if `z > c`, return false. But also we need to ensure that the total items don't exceed total capacity, meaning `x + y + z <= a + b + c`; however, this is already implied by the previous checks if we also check `z <= c` and `y <= a+b-x`, because those ensure small+medium fit in A+B and large fits in C, so total <= total. But to be safe and handle edge cases, we can also explicitly check total. Edge cases: if a box has zero capacity and there are items that must go there, return false; if x > a+b, small cannot fit at all (because A and B filled), so return false. The algorithm is O(1) time and O(1) space.
