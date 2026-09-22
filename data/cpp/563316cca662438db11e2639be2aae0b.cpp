Write a C++ function named `greatestCommonDivisor` that takes two positive integer parameters and returns their greatest common divisor (GCD) using the Euclidean algorithm with a `while` loop. The function must validate that both inputs are positive, but since the task specification requires handling only valid positive integers, the function should assume valid input. However, to be robust, if either parameter is non-positive, return 0 as an error indicator. The solution must be self-contained, use `const` for parameters where appropriate, and avoid any I/O operations inside the function.

// The Euclidean algorithm works by repeatedly replacing the larger number with the remainder of dividing it by the smaller number until the remainder becomes zero. At that point, the non-zero value is the GCD. The algorithm proceeds as follows: given two numbers `a` and `b`, while `b` is not zero, compute `r = a % b`, then set `a = b` and `b = r`. When the loop terminates, `a` holds the GCD. Edge cases include when one number is a multiple of the other (the remainder becomes zero immediately), when both numbers are equal (GCD is the number itself), and when one of the inputs is 1 (GCD is always 1). If either input is zero or negative, the function returns 0 as an invalid result, since the problem specifies positive integers. Time complexity is \(O(\log(\min(a,b)))\) for the Euclidean algorithm in the worst case, and space complexity is \(O(1)\) since only a few integer variables are used.

// Returns the greatest common divisor of two positive integers.
// Returns 0 if either parameter is non-positive.
int greatestCommonDivisor(const int a, const int b) {
    if (a <= 0 || b <= 0) {
        return 0; // Invalid input per specification
    }
    int varA = a;
    int varB = b;
    int remainder;
    while (varB != 0) {
        remainder = varA % varB;
        varA = varB;
        varB = remainder;
    }
    return varA;
}

int main() {
    // Basic positive cases
    assert(greatestCommonDivisor(12, 18) == 6);
    assert(greatestCommonDivisor(100, 10) == 10);
    assert(greatestCommonDivisor(7, 13) == 1);
    // Equal numbers
    assert(greatestCommonDivisor(15, 15) == 15);
    // One is 1
    assert(greatestCommonDivisor(1, 100) == 1);
    // Larger numbers
    assert(greatestCommonDivisor(270, 192) == 6);
    // Invalid inputs
    assert(greatestCommonDivisor(0, 5) == 0);
    assert(greatestCommonDivisor(-4, 8) == 0);
    assert(greatestCommonDivisor(0, 0) == 0);
}
