/*
Write a C++ function that takes two integers representing the number of correct multiple-choice questions (x) and incorrect questions (y) for a student, and returns the student's total score. Each correct question earns 10 points, and each incorrect question loses 90 points (i.e., subtracts 90 from the total). The score can be negative—there is no lower bound. The function should compute and return the total score as an integer. You may assume the inputs are non-negative integers within the typical `int` range.
*/

#include <cstdint>

// Compute the total score from the number of correct (x) and incorrect (y) answers.
// Each correct answer awards 10 points, each incorrect answer subtracts 90 points.
int computeScore(int x, int y) {
    // Multiply x by 10 and y by 90, then subtract y's contribution.
    // Using int is fine for typical inputs; could use long long for extra safety.
    return (x * 10) - (y * 90);
}

#include <cassert>

int computeScore(int x, int y); // declaration for the test

int main() {
    // Basic cases
    assert(computeScore(0, 0) == 0);
    assert(computeScore(1, 0) == 10);
    assert(computeScore(0, 1) == -90);
    assert(computeScore(2, 1) == 20 - 90); // -70
    assert(computeScore(3, 2) == 30 - 180); // -150
    
    // Larger values
    assert(computeScore(10, 5) == 100 - 450); // -350
    assert(computeScore(100, 0) == 1000);
    assert(computeScore(0, 100) == -9000);
    
    // Mixed values
    assert(computeScore(90, 10) == 900 - 900); // 0
    assert(computeScore(5, 7) == 50 - 630); // -580
    
    return 0;
}

// The task is straightforward arithmetic: the total score is `10 * x - 90 * y`. Since each correct answer adds 10 and each incorrect answer subtracts 90, the formula directly follows from the problem statement. There are no edge cases beyond handling negative results (which is fine with `int`) and ensuring the multiplication does not overflow—since inputs are non-negative and within typical `int` range, `10*x` and `90*y` are safe for 32-bit integers as long as x and y are each below about 200 million. The time complexity is O(1), and the space complexity is O(1). No special handling for zero values is needed; if both are zero, the result is zero.
