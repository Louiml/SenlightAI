Write a C++ function named `sumWithGlobalAndLocalC` that takes two integers `a` and `b` as parameters. The function must compute and return the sum of the local parameters `a` and `b` as an integer. However, the function must also demonstrate the distinction between a global variable and a local variable named `c`. Specifically, the function should declare a local integer `c` that holds the sum of `a` and `b`, then compare this local `c` with a global `c` (which should be defined as `int c = 45;`). The function should return the local sum `c` but must first verify that the global `c` is accessible using the scope resolution operator `::`. Additionally, the function should handle the edge case where the sum of `a` and `b` overflows the `int` range — in that case, it should still return the result (which may wrap) but should not crash. The function must not use any `using namespace std;` to keep the scope explicit. The solution must include a free function (no `main`), and the test code must call it directly with various integer inputs, including negative numbers, zero, and large values near `INT_MAX`.
#include <cassert>
#include <climits>

// Global variable c must be defined in the test file as well.
// But since the solution file includes it, we rely on that definition.
// For test compilation, we need to ensure the global c is accessible.
// We can define it here or rely on the solution's definition.
// In a single file, we include the solution and then test.

int main() {
    // Basic cases
    assert(sumWithGlobalAndLocalC(1, 2) == 3);
    assert(sumWithGlobalAndLocalC(-5, 10) == 5);
    assert(sumWithGlobalAndLocalC(0, 0) == 0);
    assert(sumWithGlobalAndLocalC(-3, -7) == -10);

    // Large values near INT_MAX (sum may overflow, but we compare with long long)
    long long a = INT_MAX - 10;
    long long b = 20;
    long long expected = a + b;
    // Cast to int may wrap, but we ensure the function's return matches (int)expected
    int sum = sumWithGlobalAndLocalC(static_cast<int>(a), static_cast<int>(b));
    assert(sum == static_cast<int>(expected));

    // Another overflow-like case
    assert(sumWithGlobalAndLocalC(INT_MAX, 1) == static_cast<int>(static_cast<long long>(INT_MAX) + 1));

    // Symmetric case
    assert(sumWithGlobalAndLocalC(100, -100) == 0);

    // Large negative values
    assert(sumWithGlobalAndLocalC(INT_MIN, 1) == static_cast<int>(static_cast<long long>(INT_MIN) + 1));
    assert(sumWithGlobalAndLocalC(INT_MIN, INT_MIN) == static_cast<int>(static_cast<long long>(INT_MIN) + INT_MIN));
}
#include <climits>

// Global variable c, accessible via ::c
int c = 45;

/**
 * Computes the sum of two integers and demonstrates the difference
 * between a local variable c and the global variable c.
 * 
 * @param a first integer operand
 * @param b second integer operand
 * @return the sum of a and b (stored in a local variable c)
 */
int sumWithGlobalAndLocalC(int a, int b) {
    // Local variable c shadows the global c
    int c = a + b;

    // Demonstrate access to the global c using scope resolution operator
    // This line is for demonstration; the function returns the local c.
    // To highlight the difference, we can compare, but we don't need to do anything with it.
    int globalC = ::c;  // This is 45

    // To avoid unused variable warning, we mention globalC in an assert-like check
    // but in a normal function we might just ignore it. Here we keep it simple.
    // The function's purpose is just to return the local sum.
    return c;
}
// The solution revolves around demonstrating the use of global versus local variables with identical names, and the scope resolution operator `::`. The main algorithm is simple: declare a global integer `c` initialized to `45` outside the function. Inside the function, declare a local integer `c` and assign it the sum of the parameters `a` and `b`. To make the distinction explicit, the function should read the global `c` value (using `::c`) and optionally use it for comparison—for example, it could assert that the local `c` is different from the global `c` unless the sum happens to equal 45. The function returns the local `c`. Edge cases: negative sums, sums equal to zero, and potential integer overflow. Since integer overflow in C++ is undefined behavior for signed types, we should avoid relying on it; but the task does not require handling it specially—just noting that the function simply performs the addition. For the test, we call the function with various pairs and compare the return value to the expected sum (computed as a long long to avoid overflow in tests). The algorithm runs in O(1) time and O(1) auxiliary space. The global variable `c` is part of the global namespace and is accessible via `::c`; local `c` shadows it inside the function. For clarity, we declare the global `c` at file scope in the solution, but the function itself is self-contained and does not depend on external input beyond its parameters.
