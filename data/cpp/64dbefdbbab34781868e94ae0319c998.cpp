Write a C++ function named `swapIntegers` that takes two integer pointers as parameters and swaps the values they point to, without using any standard library swap utilities. The function should modify the original variables through the pointers, and the task requires demonstrating the swap by printing the values before and after the call. The function must handle null pointers gracefully by doing nothing if either pointer is null, and it must work with any integer values including negative numbers and zero.
// The main algorithm is straightforward: dereference both pointers, save the value of the first in a temporary variable, assign the second value to the first location, then assign the temporary value to the second location. This classic swap using a temporary variable ensures no data is lost regardless of the integer values. Edge cases: if either pointer is null, the function should return immediately to avoid undefined behavior from dereferencing null pointers (for a robust standalone function, we add a null check). If both pointers point to the same location, the swap is a no-op but still works correctly—the temporary holds the original value and both assignments preserve it. Time complexity is O(1) since only a fixed number of operations are performed, and space complexity is O(1) because only a single temporary integer is used. The function design matches the original snippet's purpose but improves safety with null checks and uses a descriptive name.
#include <cstddef> // for nullptr

// Swaps the integer values pointed to by a and b.
// If either pointer is null, the function does nothing.
void swapIntegers(int* a, int* b) {
    if (a == nullptr || b == nullptr) {
        return;
    }
    int temp = *a;
    *a = *b;
    *b = temp;
}
#include <cassert>

int main() {
    // Basic swap
    int x = 25;
    int y = 100;
    swapIntegers(&x, &y);
    assert(x == 100);
    assert(y == 25);

    // Swap with negative numbers
    int a = -7;
    int b = 3;
    swapIntegers(&a, &b);
    assert(a == 3);
    assert(b == -7);

    // Swap with zero
    int p = 0;
    int q = 42;
    swapIntegers(&p, &q);
    assert(p == 42);
    assert(q == 0);

    // Swap equal values
    int m = 5;
    int n = 5;
    swapIntegers(&m, &n);
    assert(m == 5);
    assert(n == 5);

    // Null pointer safety (should not crash)
    int r = 1;
    swapIntegers(nullptr, &r);
    assert(r == 1);
    swapIntegers(&r, nullptr);
    assert(r == 1);
    swapIntegers(nullptr, nullptr);

    // Swap self (same address)
    int s = 99;
    swapIntegers(&s, &s);
    assert(s == 99);

    return 0;
}
