Write a C++ function named `canParticipate` that takes a single integer `n` representing the number of participants in a contest. The function must return `true` if `n` is greater than or equal to 10, and `false` otherwise. The problem is inspired by a simple decision rule used in competitive programming problems where large test inputs require a different handling logic than small ones. Your function should be const-correct, use no global variables, and be self-contained with only necessary headers. Consider edge cases such as zero and negative numbers—these should all return `false`.

// The problem is a straightforward conditional check. The main algorithm is: if the input integer `n` is at least 10, return `true`; otherwise return `false`. Edge cases include `n` being 0, negative, or exactly 9—all should return `false` because they do not satisfy `n >= 10`. Also, `n = 10` itself returns `true`. The integer type is `long long` to safely handle any reasonable input size without overflow (though the range is small, using `long long` is safe). Time complexity is O(1) because only a single comparison is performed, and space complexity is O(1) since no extra data structures are used.

#include <cstdint>

// Returns true if the number of participants is at least 10.
bool canParticipate(long long n) {
    return n >= 10;
}

#include <cassert>

int main() {
    // Basic valid cases
    assert(canParticipate(10) == true);
    assert(canParticipate(11) == true);
    assert(canParticipate(1000000) == true);
    
    // Boundary and invalid cases
    assert(canParticipate(9) == false);
    assert(canParticipate(0) == false);
    assert(canParticipate(-5) == false);
    
    // Edge case: exactly the threshold
    assert(canParticipate(10LL) == true);
    assert(canParticipate(10LL - 1) == false);
    
    // Large value
    assert(canParticipate(1000000000000LL) == true);
    return 0;
}
