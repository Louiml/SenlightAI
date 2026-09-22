Write a C++ function that computes the sum of the squares of all odd integers from 1 to a given positive integer `n` (inclusive). The function should take `n` as an input parameter and return the computed sum. For example, if `n = 5`, the odd numbers are 1, 3, 5, and the sum of their squares is 1 + 9 + 25 = 35. Handle the edge case where `n` is 0 or negative by returning 0. Use a loop with a `continue` statement to skip even numbers, and ensure the accumulator is properly initialized.

The solution iterates from 1 to `n` inclusive. For each integer `i`, if `i` is even (`i % 2 == 0`), we use `continue` to skip it and move to the next iteration; otherwise, we add `i * i` to a running sum. The sum accumulator must be initialized to 0 before the loop to avoid undefined behavior from reading an uninitialized variable (as in the original snippet). Edge cases: if `n` is less than 1, the loop does not execute and the function returns 0. The algorithm has a time complexity of O(n) and uses O(1) auxiliary space, making it efficient for moderate inputs. Using `i * i` directly is fine since the input is positive and within typical int range; for very large `n`, integer overflow could occur, but that is outside the scope of this task.

// Compute the sum of squares of all odd integers from 1 to n inclusive.
// Returns 0 if n is less than 1.
int sumOfOddSquares(int n) {
    int sum = 0;
    for (int i = 1; i <= n; ++i) {
        if (i % 2 == 0) {
            continue; // Skip even numbers
        }
        sum += i * i;
    }
    return sum;
}

int main() {
    // Basic cases
    assert(sumOfOddSquares(5) == 35);        // 1 + 9 + 25
    assert(sumOfOddSquares(1) == 1);         // only 1
    assert(sumOfOddSquares(10) == 165);      // 1+9+25+49+81
    // Edge cases with non-positive inputs
    assert(sumOfOddSquares(0) == 0);
    assert(sumOfOddSquares(-3) == 0);
    // Larger but safe value
    assert(sumOfOddSquares(100) == 166650);  // known result
    return 0;
}
