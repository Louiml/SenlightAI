Write a C++ function `bool canSplitIntoTwoEvenGroups(int n)` that determines whether a group of `n` items can be split into two non-empty groups of equal even size. The function should return `true` if possible and `false` otherwise. The input `n` is a positive integer. For example, `n = 4` is possible because each group would have 2 items, but `n = 2` is not possible because each group would have only 1 item (which is odd), and `n = 6` is possible (each group has 3 items). The function must handle all positive integers.
// The problem reduces to checking a simple condition: we need to split `n` into two equal non-empty positive integers, each of which must be even. This means:  
// 1. `n` must be even, because the two equal parts sum to `n`.  
// 2. Each part must be even, which means each part is divisible by 2. Since the parts are equal and sum to `n`, each part is `n/2`. So we require `n/2` to be even, which is equivalent to `n` being divisible by 4.  
// 3. Additionally, the parts must be non-empty, so `n/2 > 0`, which holds for all `n >= 2`. For `n = 0` or negative values (though the problem says positive), we should return false.  
//
// The edge cases:  
// - `n = 2`: `n` is even, but `n/2 = 1` (odd), so not possible.  
// - `n = 4`: `n/2 = 2` (even), possible.  
// - `n = 6`: `n/2 = 3` (odd), not possible.  
// - `n = 8`: possible.  
// So the condition is simply `n % 4 == 0`.  
//
// Time complexity: \(O(1)\). Space complexity: \(O(1)\).
#include <cstdbool>

// Returns true if n items can be split into two non-empty groups of equal even size.
bool canSplitIntoTwoEvenGroups(int n) {
    // Each group must have size n/2, which must be even and non-zero.
    return n > 0 && (n % 4 == 0);
}
#include <cassert>

int main() {
    assert(canSplitIntoTwoEvenGroups(4) == true);   // 2 and 2
    assert(canSplitIntoTwoEvenGroups(8) == true);   // 4 and 4
    assert(canSplitIntoTwoEvenGroups(12) == true);  // 6 and 6
    assert(canSplitIntoTwoEvenGroups(2) == false);  // 1 and 1 (odd)
    assert(canSplitIntoTwoEvenGroups(6) == false);  // 3 and 3 (odd)
    assert(canSplitIntoTwoEvenGroups(10) == false); // 5 and 5 (odd)
    assert(canSplitIntoTwoEvenGroups(1) == false);  // cannot split into two positive groups
    assert(canSplitIntoTwoEvenGroups(0) == false);  // invalid input
}
