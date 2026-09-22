// Write a C++ function named `swapIntsByReference` that takes two integer parameters by reference and swaps their values in-place. The function must not return anything (`void`), must not use any standard library swap utilities (like `std::swap`), and must rely solely on a temporary local variable to perform the exchange. After the call, the first argument should contain the original value of the second argument, and the second argument should contain the original value of the first argument. This function is intended for use in contexts where the caller expects the variables’ values to be modified directly by the function.

// The core algorithm is straightforward: to swap two integers using references, we copy the value of the first variable into a temporary local variable, then assign the value of the second variable into the first variable, and finally assign the temporary (original first value) into the second variable. Using references ensures the modifications affect the caller’s original variables, not copies. Edge cases: (a) when both arguments refer to the same variable (aliasing), the algorithm still works correctly because we copy the value first, then assign the same value back, effectively doing nothing; (b) no special handling is needed for negative numbers or extreme integer values because integer assignment handles all valid `int` values. The time complexity is O(1) — constant number of operations regardless of input size. The space complexity is O(1) — only one extra temporary `int` variable is used.

// Swap the values of two integers passed by reference.
// Uses a temporary local variable to hold one value during the exchange.
void swapIntsByReference(int &a, int &b) {
    int temp = a;  // store original value of a
    a = b;         // assign b's value to a
    b = temp;      // assign original a's value to b
}

int main() {
    // Basic swap
    int x = 5, y = 6;
    swapIntsByReference(x, y);
    assert(x == 6);
    assert(y == 5);

    // Swap with negative numbers
    int p = -3, q = -7;
    swapIntsByReference(p, q);
    assert(p == -7);
    assert(q == -3);

    // Swap equal values (no change expected)
    int m = 42, n = 42;
    swapIntsByReference(m, n);
    assert(m == 42);
    assert(n == 42);

    // Swap when both refer to the same variable (aliasing)
    int single = 99;
    swapIntsByReference(single, single);
    assert(single == 99);

    // Swap extremes
    int low = INT_MIN, high = INT_MAX;
    swapIntsByReference(low, high);
    assert(low == INT_MAX);
    assert(high == INT_MIN);
}
