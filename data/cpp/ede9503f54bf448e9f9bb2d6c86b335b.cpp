// Write a C++ function named `calculateStats` that takes five integer marks (each between 0 and 100) as parameters and returns a `std::pair<double, double>` containing the average marks and the percentage (out of 100) respectively. The percentage should be computed as (total marks / 500) * 100. The function must handle invalid inputs (any mark outside 0–100) by returning `{-1.0, -1.0}` to indicate an error. You do not need to read from the console or write output; the function is purely computational. Ensure the average and percentage are computed as `double` values (not integer division), and use `const` for parameters where appropriate.

#include <cassert>
#include <utility>

// Include the solution function here (or #include "solution.h")

int main() {
    // Normal case: all marks 80 -> total 400, avg 80, percentage 80
    auto result1 = calculateStats(80, 80, 80, 80, 80);
    assert(result1.first == 80.0);
    assert(result1.second == 80.0);

    // Mixed marks: 90,85,70,60,95 -> total 400, avg 80, percentage 80
    auto result2 = calculateStats(90, 85, 70, 60, 95);
    assert(result2.first == 80.0);
    assert(result2.second == 80.0);

    // Zero marks -> total 0, avg 0, percentage 0
    auto result3 = calculateStats(0, 0, 0, 0, 0);
    assert(result3.first == 0.0);
    assert(result3.second == 0.0);

    // Max marks -> total 500, avg 100, percentage 100
    auto result4 = calculateStats(100, 100, 100, 100, 100);
    assert(result4.first == 100.0);
    assert(result4.second == 100.0);

    // Non-multiple of 5 total: 23,45,67,89,12 -> total 236, avg 47.2, percentage 47.2
    auto result5 = calculateStats(23, 45, 67, 89, 12);
    assert(result5.first == 47.2);
    assert(result5.second == 47.2);

    // Invalid mark: one is 101 -> error
    auto result6 = calculateStats(50, 60, 70, 80, 101);
    assert(result6.first == -1.0);
    assert(result6.second == -1.0);

    // Invalid mark: negative mark
    auto result7 = calculateStats(-1, 50, 60, 70, 80);
    assert(result7.first == -1.0);
    assert(result7.second == -1.0);

    // Boundary: mark exactly 100 and 0 are valid
    auto result8 = calculateStats(0, 100, 50, 50, 50);
    assert(result8.first == 50.0);
    assert(result8.second == 50.0);

    // Fractional result: 1,2,3,4,5 -> total 15, avg 3, percentage 3
    auto result9 = calculateStats(1, 2, 3, 4, 5);
    assert(result9.first == 3.0);
    assert(result9.second == 3.0);

    // Another fractional: 10,20,30,40,55 -> total 155, avg 31, percentage 31
    auto result10 = calculateStats(10, 20, 30, 40, 55);
    assert(result10.first == 31.0);
    assert(result10.second == 31.0);

    return 0;
}

#include <utility> // for std::pair

// Calculate average and percentage for five subject marks (0-100 each).
// Returns {average, percentage} on success, {-1.0, -1.0} if any mark is invalid.
std::pair<double, double> calculateStats(
    const int mark1,
    const int mark2,
    const int mark3,
    const int mark4,
    const int mark5
) {
    // Validate all marks are within the allowed range.
    if (mark1 < 0 || mark1 > 100 ||
        mark2 < 0 || mark2 > 100 ||
        mark3 < 0 || mark3 > 100 ||
        mark4 < 0 || mark4 > 100 ||
        mark5 < 0 || mark5 > 100) {
        return {-1.0, -1.0};
    }

    const int total = mark1 + mark2 + mark3 + mark4 + mark5;
    const double average = total / 5.0;   // floating-point division
    const double percentage = total / 5.0; // since (total/500)*100 = total/5
    return {average, percentage};
}

// The solution computes the sum of the five marks, then calculates the average by dividing the sum by 5.0 (using a floating-point literal to avoid integer truncation) and the percentage by dividing the sum by 500.0 and multiplying by 100.0 (or equivalently dividing by 5.0, since percentage = (total/500)*100 = total/5). Edge cases: if any mark is outside [0,100], return `{-1.0, -1.0}`. Also handle potential overflow? Since marks are ≤100 and there are 5, sum ≤500, well within int range, so no overflow. Time complexity is O(1) with constant operations; space complexity is O(1).
