// Write a C++ function that computes the factorial of a non-negative integer `n` using recursion. The function must return the factorial as an `int`, handle the base case `n == 0` correctly, and assume the input is small enough that the result fits within the range of `int` (i.e., `n ≤ 12` for typical 32-bit `int`). The function should be named `factorialRecursive` and take a single `int` parameter. It must use the "divide and conquer" recursive pattern: compute the factorial of `n-1` first, then multiply by `n`. Do not use loops, and do not modify the input parameter. Provide only the function definition; no `main` function is needed for the solution.

// The solution follows the standard recursive definition of factorial: `factorial(0) = 1`, and for `n > 0`, `factorial(n) = n * factorial(n-1)`. The recursion reduces the problem size by 1 at each step until reaching the base case. Important edge cases: `n == 0` returns 1 (the multiplicative identity). For `n == 1`, the recursion returns `1 * factorial(0) = 1`, which is correct. Negative inputs are not handled because the problem specifies non-negative integers; if a negative value is passed, the recursion would never reach a base case because `n-1` becomes more negative, leading to infinite recursion and stack overflow. We assume valid input per the specification. The time complexity is `O(n)` because the function makes `n+1` recursive calls. The space complexity is `O(n)` due to the call stack depth of `n+1` frames. For `n` up to 12, the result fits in a 32-bit `int` (12! = 479001600). For larger n, overflow would occur, but that is outside the scope of this task.

// Compute the factorial of a non-negative integer n using recursion.
// Assumes n is non-negative and the result fits in an int (n <= 12 for typical int).
int factorialRecursive(int n) {
    // Base case: 0! = 1
    if (n == 0) {
        return 1;
    }
    // Recursive case: n! = n * (n-1)!
    int smallerFactorial = factorialRecursive(n - 1);
    int result = n * smallerFactorial;
    return result;
}

int main() {
    // Base case
    assert(factorialRecursive(0) == 1);
    // Small values
    assert(factorialRecursive(1) == 1);
    assert(factorialRecursive(2) == 2);
    assert(factorialRecursive(3) == 6);
    assert(factorialRecursive(4) == 24);
    assert(factorialRecursive(5) == 120);
    assert(factorialRecursive(6) == 720);
    assert(factorialRecursive(7) == 5040);
    assert(factorialRecursive(8) == 40320);
    // Larger value within int range
    assert(factorialRecursive(10) == 3628800);
    assert(factorialRecursive(12) == 479001600);
    return 0;
}
