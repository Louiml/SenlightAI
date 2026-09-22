Write a C++ function `simulateReduction(int initialValue)` that mimics the behavior of the given snippet. The function should take a positive integer `n` (e.g., `1200`) and simulate a loop that repeatedly: increments a counter `c` (starting at 1), divides `n` by 10 using integer division, multiplies a multiplier `d` by 10 (starting at 9), and prints (or captures) the intermediate values of `n`, `d`, and `c` after each division. The loop continues while `n > 0`. After the loop ends, the function should return the **final value of `c`** (the total number of iterations plus the initial 1), and additionally print the final values of `n`, `d`, and `c` exactly as the snippet does after the loop. For clarity, the function should print each loop iteration's values in the format `n d c` (space-separated), and after the loop prints the final values `n d c` once more. Ensure the function works for any positive integer input, including small values like 1, and edge cases like 0 (if passed, the loop should not execute, and the function should return 1 with final values `0 9 1`).

#include <iostream>
#include <cassert>

int simulateReduction(int);

int main() {
    // Test with 1200: loop runs 4 times (1200->120->12->1->0), c becomes 5
    // Final c = 5
    assert(simulateReduction(1200) == 5);

    // Test with 0: loop does not run, c stays 1
    assert(simulateReduction(0) == 1);

    // Test with single digit 9: one iteration, c becomes 2
    assert(simulateReduction(9) == 2);

    // Test with 10: two iterations (10->1->0), c becomes 3
    assert(simulateReduction(10) == 3);

    // Test with 1000: four iterations, c becomes 5
    assert(simulateReduction(1000) == 5);

    std::cout << "All tests passed.\n";
    return 0;
}

#include <iostream>

// Simulates the given loop, printing intermediate and final values.
// Returns the final counter c.
int simulateReduction(int initialValue) {
    int n = initialValue;
    int c = 1;
    int d = 9;

    while (n > 0) {
        c++;
        n = n / 10;
        d = d * 10;
        std::cout << n << " " << d << " " << c << "\n";
    }

    std::cout << n << " " << d << " " << c << "\n";
    return c;
}

// The solution directly mirrors the provided code. The main algorithm: start with `c = 1`, `d = 9`. While `n > 0`, increment `c` by 1, perform integer division `n = n / 10`, multiply `d = d * 10`, then print the current `n`, `d`, `c` each iteration. After the loop, print the final `n`, `d`, `c` once more. The function should return the final `c` (which equals the number of digits in the original `n` plus 1, because starting `c=1` and each division reduces a digit). Important edge cases: if `n = 0`, the loop does not execute, so final `c` stays 1, `d` stays 9, and `n` stays 0. If `n` is a single digit (e.g., 5), after one iteration `n` becomes 0, `c` becomes 2, `d` becomes 90, then loop exits. Time complexity is \(O(\log_{10} n)\) because each iteration reduces `n` by a factor of 10. Space complexity is \(O(1)\) aside from output. The function must print to standard output; the test code can capture output if desired, or simply assert the return value.
