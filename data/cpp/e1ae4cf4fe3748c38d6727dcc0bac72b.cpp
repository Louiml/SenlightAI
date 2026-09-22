// Write a C++ function named `swap_values` that uses function overloading to swap two variables of the same type for the three data types `int`, `float`, and `char`. Each overload must take two non-const references to the respective type, plus an optional third parameter of type `bool` with a default value of `false`. When the bool parameter is `true`, the function should automatically print a confirmation message to standard output indicating which overload was invoked (e.g., `"Swapped int"`, `"Swapped float"`, `"Swapped char"`) followed by a newline. The function must correctly exchange the values of the two passed variables in-place, using only a temporary local variable of the same type for the swap. The solution must be self-contained, include the necessary headers (`<iostream>`), and provide exactly three overloaded function definitions. No `main` function should be provided in the solution code; the test section will contain a `main` that verifies correctness.

int main() {
    // Int tests
    int x = 5, y = 10;
    swap_values(x, y);
    assert(x == 10 && y == 5);
    
    int p = 1, q = 2;
    swap_values(p, q, true); // prints "Swapped int" once
    assert(p == 2 && q == 1);

    // Float tests
    float a = 3.14f, b = 2.71f;
    swap_values(a, b);
    assert(a == 2.71f && b == 3.14f);

    float c = -1.5f, d = 0.0f;
    swap_values(c, d, true); // prints "Swapped float" once
    assert(c == 0.0f && d == -1.5f);

    // Char tests
    char m = 'A', n = 'Z';
    swap_values(m, n);
    assert(m == 'Z' && n == 'A');

    char u = 'x', v = 'y';
    swap_values(u, v, true); // prints "Swapped char" once
    assert(u == 'y' && v == 'x');

    // Self-swap edge case
    int same = 42;
    swap_values(same, same, false);
    assert(same == 42);

    // Different types must not conflict (compile-time check)
    float f1 = 1.1f, f2 = 2.2f;
    swap_values(f1, f2);
    assert(f1 == 2.2f && f2 == 1.1f);

    return 0;
}

#include <iostream>

// Swaps two ints in-place. If verbose is true, prints confirmation.
void swap_values(int &a, int &b, bool verbose = false) {
    int temp = a;
    a = b;
    b = temp;
    if (verbose) {
        std::cout << "Swapped int\n";
    }
}

// Swaps two floats in-place. If verbose is true, prints confirmation.
void swap_values(float &a, float &b, bool verbose = false) {
    float temp = a;
    a = b;
    b = temp;
    if (verbose) {
        std::cout << "Swapped float\n";
    }
}

// Swaps two chars in-place. If verbose is true, prints confirmation.
void swap_values(char &a, char &b, bool verbose = false) {
    char temp = a;
    a = b;
    b = temp;
    if (verbose) {
        std::cout << "Swapped char\n";
    }
}

// The core algorithm for each overload is a classic three-step swap: copy the value of the first reference into a temporary local variable, then assign the second reference's value to the first reference, and finally assign the temporary's value to the second reference. This works in O(1) time and O(1) auxiliary space per call. The main challenge is ensuring that function overloading correctly resolves based on the argument types: the compiler picks the overload whose parameter types exactly match the argument types after reference binding. Edge cases include passing the same variable to both parameters (the swap self-assigns, resulting in no change but still works correctly), and passing `true` as the third argument to trigger the output—the default value only applies when the third argument is omitted. The confirmation message should be exactly as specified to match test expectations. Since the function modifies via references, `const` correctness is not applicable to the swapped parameters, but the function itself does not modify any global state besides the optional print. Time complexity is constant, and space complexity is constant (only a single temporary variable).
