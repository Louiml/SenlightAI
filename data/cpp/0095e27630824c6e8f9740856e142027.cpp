/*
Write a C++ function named `qualityPoints` that takes an integer average score (0–100) and returns a quality point value according to this academic grading scale: 90–100 → 4, 80–89 → 3, 70–79 → 2, 60–69 → 1, and below 60 → 0. The function must handle any integer input, including values outside the 0–100 range, by applying the same thresholds (e.g., 150 returns 4, −5 returns 0). In a separate program, prompt the user for the number of students, then for each student prompt for their average score and immediately print the corresponding quality points; after processing all students, the program should terminate. The grading logic must be implemented entirely inside the free function `qualityPoints` (no other helper functions), and the function must be pure—no input/output inside it. Ensure correct handling for edge cases exactly at boundary values (60, 70, 80, 90) and for negative or >100 averages.
*/
#include <iostream>

// Return quality points (0-4) based on the given average score.
int qualityPoints(int avg) {
    if (avg >= 90) {
        return 4;
    } else if (avg >= 80) {
        return 3;
    } else if (avg >= 70) {
        return 2;
    } else if (avg >= 60) {
        return 1;
    } else {
        return 0;
    }
}
int main() {
    // Boundary and regular cases
    assert(qualityPoints(100) == 4);
    assert(qualityPoints(90) == 4);
    assert(qualityPoints(89) == 3);
    assert(qualityPoints(80) == 3);
    assert(qualityPoints(79) == 2);
    assert(qualityPoints(70) == 2);
    assert(qualityPoints(69) == 1);
    assert(qualityPoints(60) == 1);
    assert(qualityPoints(59) == 0);
    assert(qualityPoints(0) == 0);
    // Out-of-range cases
    assert(qualityPoints(150) == 4);
    assert(qualityPoints(-10) == 0);
    return 0;
}
// The solution is straightforward: define `qualityPoints(int avg)` using a sequence of if-else if checks that compare against the lower bound of each grade tier. Start with the highest threshold (90) and work downward: if `avg >= 90` return 4; else if `avg >= 80` return 3; else if `avg >= 70` return 2; else if `avg >= 60` return 1; else return 0. This cascade naturally handles all integer values, including negatives (returning 0) and values above 100 (returning 4). The boundary values 90, 80, 70, 60 each correctly belong to the higher tier because the condition uses `>=`. The main function reads the number of students, loops that many times reading an average and printing the result immediately. Since we call the function per student, time complexity is O(n) for n students and O(1) auxiliary space. No special edge cases beyond boundary checks are needed, but the function should be `const`-correct (though an integer parameter is passed by value, so no `const` annotation is required—the parameter itself is already a copy). The implementation is independent and self-contained.
