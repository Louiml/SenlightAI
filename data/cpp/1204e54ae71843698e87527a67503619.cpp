// Write a C++ function named `isEven` that takes a single integer argument and returns a `bool` value indicating whether the number is even. The function should handle positive, negative, and zero values correctly. In addition, create a separate global integer variable and a reference to it that aliases the original variable, demonstrating that the reference can be used to modify the original value. The task requires you to implement the even-checking logic using the modulo operator, and to clearly distinguish between value passing, references, and pointers in your solution.
#include <cassert>

// Declare the function prototype for the test.
bool isEven(int number);

int main() {
    // Basic positive numbers
    assert(isEven(0) == true);
    assert(isEven(2) == true);
    assert(isEven(7) == false);
    assert(isEven(100) == true);
    
    // Negative numbers
    assert(isEven(-2) == true);
    assert(isEven(-3) == false);
    assert(isEven(-4) == true);
    
    // Large numbers
    assert(isEven(2147483646) == true);
    assert(isEven(2147483647) == false);
    
    // Verify the reference behavior (if included in the solution)
    // This demonstrates that modifying through the reference updates the original.
    extern int GlobalNumber;
    extern int& GlobalRef;
    GlobalNumber = 10;
    assert(GlobalRef == 10);
    GlobalRef = 20;
    assert(GlobalNumber == 20);
    
    return 0;
}
#include <iostream>

// Determine whether an integer is even.
// Returns true if the number is divisible by 2, false otherwise.
// Works for positive, negative, and zero values.
bool isEven(int number) {
    return (number % 2) == 0;
}

// Global variable and its reference alias for demonstration.
int GlobalNumber = 42;
int& GlobalRef = GlobalNumber;  // Reference to GlobalNumber
// The solution is straightforward: an integer is even if it is divisible by 2 with no remainder, which is tested using the modulo operator `%`. For any integer `n`, `n % 2` equals `0` if `n` is even and `1` or `-1` (for negative odd numbers) if it is odd. The function should return `true` when the remainder is zero and `false` otherwise. Zero is even because `0 % 2 == 0`. Edge cases include negative numbers (e.g., `-4 % 2 == 0`, `-3 % 2 == -1`) and large values; the modulo operation works correctly for all `int` values. The reference part of the task is handled by declaring a global integer, then creating a reference alias to it, and demonstrating that both names refer to the same memory location—modifying through one updates the other. Time complexity is O(1) and space complexity is O(1) for the function. The reference variable also uses O(1) space.
