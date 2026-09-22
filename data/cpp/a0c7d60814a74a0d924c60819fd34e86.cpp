/*
Create a C++ function `canPassExam` that takes an integer `n` representing a student's score on a 200-point exam, and returns `true` if the student passes the exam (score is at least 127) and `false` otherwise. The function should be pure (no I/O), handle negative scores and scores above 200 gracefully (just compare directly), and the solution should be reusable without a main function. The task is to implement the logic cleanly with proper const correctness and no side effects.
*/
#include <cstdbool>

// Return true if the exam score is at least the passing threshold of 127.
bool canPassExam(const int score) {
    // Direct comparison against the passing threshold.
    return score >= 127;
}
#include <cassert>

int main() {
    // Basic passing and failing cases.
    assert(canPassExam(127) == true);
    assert(canPassExam(126) == false);
    assert(canPassExam(200) == true);
    assert(canPassExam(0) == false);

    // Edge cases: negative and very large values.
    assert(canPassExam(-100) == false);
    assert(canPassExam(1000) == true);

    // Boundary exactly at threshold.
    assert(canPassExam(127) == true);
    assert(canPassExam(128) == true);

    // Additional sanity check with extreme low.
    assert(canPassExam(-1) == false);

    return 0;
}
// The solution is straightforward: compare the input integer `n` against the threshold of 127. If `n >= 127`, return `true`, else return `false`. Edge cases include negative scores (should return `false`), exactly 127 (should return `true`), and scores above 200 (should return `true`). No additional data structures or algorithms are needed. Time complexity is O(1) since only a single comparison is performed. Space complexity is O(1) as no auxiliary storage is used. The function should be marked `const`-correct by taking the argument by value (since it's a primitive) and not modifying anything.
