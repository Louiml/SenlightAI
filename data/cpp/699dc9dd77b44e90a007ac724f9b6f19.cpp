Write a standalone C++ function named `scopeAndGlobalValue` that simulates the behavior of the original code snippet involving variable shadowing and global/static interactions. The function must accept an integer `inputValue` and return an integer result computed according to these rules: first, add the current value of a global integer `globalK` (initialized to 1) to `inputValue` to get `m`; second, update `globalK` by adding `m` to it; third, compute a local character `charK` initialized to `'B'` (but it does not affect the returned value); fourth, return the product of `m` and `globalK` (after the update). Additionally, the function must be designed so that repeated calls with the same `inputValue` produce results that depend on the evolving `globalK`, thereby testing persistent global state. Ensure the function is `const`-correct where appropriate, includes necessary headers, and is fully self-contained except for a separate user-provided `main` for testing. Do not include a `main` function in your solution.
The solution needs to replicate the side effects and variable scoping seen in the snippet: a global integer `globalK` starts at 1 and is modified each time the function is called. Inside the function, `m` is computed as `inputValue + globalK` (using the current global value). Then `globalK` is updated to `globalK + m`. A local `char` named `charK` shadows nothing in the actual arithmetic but demonstrates the idea of scoping—though it is not used in the return. The function returns `m * globalK` (after the update) to produce a meaningful numeric result that depends on both the input and the evolving global state. Edge cases: the function must handle negative inputs, zero, and large integers without overflow (assuming standard `int` range; we can mention this). Time complexity is O(1) per call, space complexity O(1) (only a few integer variables). The key is to declare `globalK` as a global variable (outside the function) so its value persists between calls, which is critical for the task. Since we are asked to output only the solution function (no `main`), we declare the global variable in the solution block. The test code will call the function multiple times and assert expected values based on sequential calls.
#include <cstddef> // not needed, but for completeness
// Global variable that persists across calls, simulating the original global k.
int globalK = 1;

// Simulates the original code's behavior, returning m * globalK after updating globalK.
int scopeAndGlobalValue(int inputValue) {
    // m starts as inputValue + current globalK (as in the snippet: m += k)
    int m = inputValue + globalK;
    // Update globalK by adding m (as in the snippet: k += m)
    globalK += m;
    // Local char to demonstrate shadowing (not used in computation)
    char charK = 'B'; // unused, but present for clarity
    // Return product of m and the updated globalK
    return m * globalK;
}
#include <cassert>

int main() {
    // Reset globalK to 1 for deterministic testing
    globalK = 1;

    // Call 1: input 4, globalK starts 1 -> m=5, globalK becomes 6, return 5*6=30
    assert(scopeAndGlobalValue(4) == 30);
    // Call 2: input 4, globalK now 6 -> m=10, globalK becomes 16, return 10*16=160
    assert(scopeAndGlobalValue(4) == 160);
    // Call 3: input 0, globalK now 16 -> m=16, globalK becomes 32, return 16*32=512
    assert(scopeAndGlobalValue(0) == 512);
    // Call 4: input -2, globalK now 32 -> m=30, globalK becomes 62, return 30*62=1860
    assert(scopeAndGlobalValue(-2) == 1860);
    // Call 5: input 1, globalK now 62 -> m=63, globalK becomes 125, return 63*125=7875
    assert(scopeAndGlobalValue(1) == 7875);

    // Test with negative initial value when globalK is reset
    globalK = 1;
    // input -5: m=-4, globalK becomes -3, return -4*(-3)=12
    assert(scopeAndGlobalValue(-5) == 12);
    // input 10: m=7 (since globalK=-3), globalK becomes 4, return 7*4=28
    assert(scopeAndGlobalValue(10) == 28);

    // Test with zero input when globalK is reset
    globalK = 1;
    // input 0: m=1, globalK becomes 2, return 1*2=2
    assert(scopeAndGlobalValue(0) == 2);
    // input 0 again: m=2, globalK becomes 4, return 2*4=8
    assert(scopeAndGlobalValue(0) == 8);
}
