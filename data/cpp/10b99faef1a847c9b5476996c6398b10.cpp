Write a C++ function named `computeExpression` that takes three integer parameters `initialA`, `initialB`, and `incrementA`, and returns an integer result computed as follows: first, set `a` to `initialA` and `b` to `initialB`; then add `incrementA` to `a`; finally, return `a - b`. The function should perform no input/output operations and must be usable in a test harness. Ensure proper handling of integer overflow (wrap-around is acceptable as per C++ standard for signed integers? Actually, signed overflow is undefined behavior—so specify that inputs will be small enough to avoid overflow, or use `long long` for safety). For simplicity, assume all inputs are between `-1000` and `1000`, so no overflow occurs. The function must be `const`-correct with respect to parameters (i.e., parameters are passed by value, so no const needed, but apply `const` to any local variables that don't change). Provide the function without a `main` function.
The solution is straightforward arithmetic. The algorithm: (1) assign `a = initialA`, `b = initialB`; (2) `a = a + incrementA`; (3) return `a - b`. No loops, no recursion, no edge cases beyond ensuring that the types are large enough. Given the constraint that inputs are within [-1000, 1000], the maximum absolute result occurs when `initialA=1000`, `incrementA=1000` → `a=2000`, and `initialB=-1000` → result = 2000 - (-1000) = 3000, which fits in `int`. Time complexity is O(1), space complexity O(1). The function is pure and deterministic. No I/O is performed inside the function.
// Returns (initialA + incrementA) - initialB.
int computeExpression(const int initialA, const int initialB, const int incrementA) {
    const int a = initialA + incrementA;  // a = initialA + incrementA
    const int b = initialB;               // b = initialB
    return a - b;                         // result = a - b
}
int main() {
    assert(computeExpression(5, 2, 1) == 4);   // (5+1) - 2 = 4
    assert(computeExpression(0, 0, 0) == 0);   // 0 - 0 = 0
    assert(computeExpression(10, 10, 5) == 5); // (10+5) - 10 = 5
    assert(computeExpression(-3, 7, 2) == -8); // (-3+2) - 7 = -8
    assert(computeExpression(1000, -1000, 1000) == 3000); // (1000+1000) - (-1000) = 3000
    assert(computeExpression(-1000, 1000, -1000) == -3000); // (-1000-1000) - 1000 = -3000
    assert(computeExpression(1, 1, 0) == 0);   // (1+0) - 1 = 0
    assert(computeExpression(7, 3, 4) == 8);   // (7+4) - 3 = 8
    assert(computeExpression(50, 25, -10) == 15); // (50-10) - 25 = 15
    assert(computeExpression(100, 200, 100) == 0); // (100+100) - 200 = 0
}
