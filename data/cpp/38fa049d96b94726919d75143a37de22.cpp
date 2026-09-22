Write a standalone C++ function named `computeFinalGrade` that takes as parameters two floating-point values for each of two subjects (four values total: `subject1Note1`, `subject1Note2`, `subject2Note1`, `subject2Note2`) and one floating-point value for a final integrative project (`projectNote`). The function must compute the unweighted average of each subject's two notes, then combine these two averages with the project note using weights of 20%, 30%, and 50% respectively, and return the resulting final grade as a `float`. The function must not read from or write to standard input/output; it must be pure and deterministic. Assume all input values are valid non-negative floating-point numbers (no validation is required), but the function must correctly handle edge cases such as all zeros or identical values. Provide a separate test harness with `assert` statements that verify the function's correctness for typical, boundary, and edge cases.

#include <cassert>
#include <cmath>

// Global main for testing the solution function.
int main() {
    // Typical case: subject1 avg = (8+9)/2 = 8.5, subject2 avg = (7+6)/2 = 6.5, project=8
    // Final = 8.5*0.2 + 6.5*0.3 + 8*0.5 = 1.7 + 1.95 + 4.0 = 7.65
    float result = computeFinalGrade(8.0f, 9.0f, 7.0f, 6.0f, 8.0f);
    assert(std::fabs(result - 7.65f) < 1e-6);

    // All zeros -> zero
    assert(computeFinalGrade(0.0f, 0.0f, 0.0f, 0.0f, 0.0f) == 0.0f);

    // All same value v=10 -> final should be 10.0
    assert(computeFinalGrade(10.0f, 10.0f, 10.0f, 10.0f, 10.0f) == 10.0f);

    // Max grades: each subject avg=10, project=10 -> final = 10*(0.2+0.3+0.5)=10
    assert(computeFinalGrade(10.0f, 10.0f, 10.0f, 10.0f, 10.0f) == 10.0f);

    // Uneven notes: subject1 avg=(0+10)/2=5, subject2 avg=(4+6)/2=5, project=0
    // Final = 5*0.2 + 5*0.3 + 0*0.5 = 1.0 + 1.5 + 0 = 2.5
    result = computeFinalGrade(0.0f, 10.0f, 4.0f, 6.0f, 0.0f);
    assert(std::fabs(result - 2.5f) < 1e-6);

    return 0;
}

#include <cstddef> // not needed, but harmless; not required

// Computes the final grade from two subjects (each with two notes) and a project note.
// Weights: subject1 average 20%, subject2 average 30%, project 50%.
// Returns the weighted sum as a float.
float computeFinalGrade(float subject1Note1, float subject1Note2,
                        float subject2Note1, float subject2Note2,
                        float projectNote) {
    const float subject1Avg = (subject1Note1 + subject1Note2) / 2.0f;
    const float subject2Avg = (subject2Note1 + subject2Note2) / 2.0f;

    const float finalGrade = subject1Avg * 0.20f +
                             subject2Avg * 0.30f +
                             projectNote * 0.50f;
    return finalGrade;
}

// The solution is straightforward arithmetic. For each subject, compute the average: `(note1 + note2) / 2`. Then compute the weighted sum: `subject1Avg * 0.20f + subject2Avg * 0.30f + projectNote * 0.50f`. This is the final grade. No special cases exist beyond ensuring the arithmetic uses `float` literals (e.g., `0.20f`) to avoid double-to-float promotion warnings or precision surprises, though the result will still be implicitly converted. Complexity is O(1) time and O(1) space because only a few primitive operations are performed. Edge cases: if all inputs are zero, the result is zero; if all inputs are the same value `v`, each subject average is `v`, and the final is `v * (0.20 + 0.30 + 0.50) = v * 1.0 = v`, which is correctly handled. No loops, no dynamic memory, and no input parsing are involved.
