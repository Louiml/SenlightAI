Write a C++ function `int applyOperator(int A, char op, int B)` that performs the arithmetic operation indicated by `op` on two integer operands. If `op` is `'+'`, return `A + B`; if `op` is `'-'`, return `A - B`. For any other character (e.g., `'*'`, `'/'`, `'%'`, or an invalid symbol), return `0` as a fallback. The function must be const-correct and should not read from or write to any external stream; it only receives values and returns a computed result. The operands may be any integers, including negative values and extremes like `INT_MIN` and `INT_MAX`; the function must rely on normal integer arithmetic (which may overflow arbitrarily, as C++ does not define overflow behavior for signed integers—assume the tests avoid overflow).

// The solution is a direct conditional dispatch: compare `op` against the two supported characters using an `if-else` chain (or a `switch` statement). Since only addition and subtraction are required, no complex parsing or state is needed. The main edge case is the fallback behavior: any operator other than `'+'` or `'-'` must return `0`. Negative operands work naturally with `+` and `-` in C++. The time complexity is \(O(1)\) because only constant comparisons and a single arithmetic operation are performed. The space complexity is \(O(1)\) since no additional data structures are used. Const-correctness is applied by passing `A` and `B` by value (they are small integers) and making the function itself `const` is not applicable to free functions, but we mark parameters as `const` to emphasize they are not modified (though by-value parameters are inherently local). The function has no side effects.

#include <cstdint> // not strictly needed, but for clarity

// Perform addition or subtraction based on the given operator.
// Returns A + B for '+', A - B for '-', and 0 for any other operator.
int applyOperator(const int A, const char op, const int B) {
    if (op == '+') {
        return A + B;
    }
    if (op == '-') {
        return A - B;
    }
    // Unsupported operator fallback
    return 0;
}

#include <cassert>

int applyOperator(const int A, const char op, const int B);

int main() {
    // Basic addition and subtraction
    assert(applyOperator(5, '+', 3) == 8);
    assert(applyOperator(5, '-', 3) == 2);
    
    // Negative operands
    assert(applyOperator(-5, '+', 3) == -2);
    assert(applyOperator(-5, '-', 3) == -8);
    assert(applyOperator(5, '+', -3) == 2);
    assert(applyOperator(5, '-', -3) == 8);
    
    // Unsupported operators fallback to 0
    assert(applyOperator(10, '*', 4) == 0);
    assert(applyOperator(10, '/', 4) == 0);
    assert(applyOperator(10, '%', 4) == 0);
    assert(applyOperator(10, 'x', 4) == 0);
    
    // Edge: zero operands
    assert(applyOperator(0, '+', 0) == 0);
    assert(applyOperator(0, '-', 0) == 0);
    
    // Large values (assuming no overflow in these tests)
    assert(applyOperator(1000000, '+', 2000000) == 3000000);
    assert(applyOperator(1000000, '-', -2000000) == 3000000);
    
    return 0;
}
