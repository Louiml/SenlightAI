// Write a C++ function named `countSafeReports` that reads a sequence of lines from an input stream (passed as a `std::istream&` parameter) until the end of the stream. Each line contains one or more integers (levels) separated by whitespace. A report is considered "safe" if it is strictly monotonic (either all increasing or all decreasing) and the absolute difference between every pair of adjacent levels is between 1 and 3 inclusive. A report with only one level is always safe. The function must return the total number of safe reports found. The input may have empty lines—these should be skipped (not counted as reports). The function must not modify the input stream's state beyond normal reading.
The solution reads the stream line by line using `std::getline`. For each non-empty line, parse the integers into a vector. Then, to check safety without mutating the original vector, determine the direction using the first and last elements: if the first element is greater than the last, the series is decreasing. For a decreasing sequence, we can either reverse or check with sign adjustment; simpler: reverse a copy for decreasing case, or check by comparing signs. A robust approach: make a copy, reverse if needed, then iterate from index 1 to end, computing `delta = report[i] - report[i-1]`, and require `1 <= delta <= 3`. If any delta violates, the report is unsafe. Edge cases: empty line (skip), one element (safe), all equal (unsafe because delta = 0), and mixed signs (unsafe because delta will be negative or zero). Time complexity: O(L * N) where L is number of lines and N is average number of integers per line; space O(N) for the temporary vector per line.
#include <sstream>
#include <vector>
#include <string>
#include <cstdint>
#include <algorithm>

// Count how many reports (non-empty lines) are safe.
// A report is safe if it is monotonic (all increasing or all decreasing)
// and every adjacent difference is between 1 and 3 inclusive.
// A report with one level is always safe.
int32_t countSafeReports(std::istream& input) {
    int32_t safeCount = 0;
    std::string line;

    while (std::getline(input, line)) {
        // Skip empty lines
        if (line.empty()) {
            continue;
        }

        // Parse the report levels
        std::istringstream ss(line);
        std::vector<int32_t> report;
        int32_t level;
        while (ss >> level) {
            report.push_back(level);
        }

        // One level is always safe
        if (report.size() <= 1) {
            ++safeCount;
            continue;
        }

        // Work on a copy to allow reversal without changing the original parse
        std::vector<int32_t> sorted = report;
        if (sorted.front() > sorted.back()) {
            std::reverse(sorted.begin(), sorted.end());
        }

        bool isSafe = true;
        for (size_t i = 1; i < sorted.size(); ++i) {
            int32_t delta = sorted[i] - sorted[i - 1];
            if (delta < 1 || delta > 3) {
                isSafe = false;
                break;
            }
        }

        if (isSafe) {
            ++safeCount;
        }
    }

    return safeCount;
}
#include <iostream>
#include <sstream>
#include <cassert>

// Forward declaration (or include the solution header)
int32_t countSafeReports(std::istream& input);

int main() {
    // Test 1: Basic increasing and decreasing safe reports
    std::istringstream input1("7 6 4 2 1\n1 2 7 8 9\n9 7 6 2 1\n1 3 2 4 5\n8 6 4 4 1\n1 3 6 7 9\n");
    assert(countSafeReports(input1) == 2);  // "7 6 4 2 1" and "1 3 6 7 9" are safe

    // Test 2: Single level reports are safe
    std::istringstream input2("5\n3\n");
    assert(countSafeReports(input2) == 2);

    // Test 3: Empty lines are ignored
    std::istringstream input3("\n1 2 3\n\n4 5 6\n");
    assert(countSafeReports(input3) == 2);

    // Test 4: All equal is unsafe
    std::istringstream input4("3 3 3\n");
    assert(countSafeReports(input4) == 0);

    // Test 5: Non-monotonic with correct adjacent differences is unsafe
    std::istringstream input5("1 3 2 4 5\n");
    assert(countSafeReports(input5) == 0);

    // Test 6: Difference of 0 or 4 is unsafe
    std::istringstream input6("1 1 2\n1 5 6\n");
    assert(countSafeReports(input6) == 0);

    // Test 7: Large input, multiple lines, mixed
    std::istringstream input7("1 2 3 4 5\n5 4 3 2 1\n1 2 3 7 8\n8 7 3 2 1\n");
    assert(countSafeReports(input7) == 2); // first two safe, last two unsafe due to delta 4

    // Test 8: Negative numbers allowed, still monotonic
    std::istringstream input8("-3 -2 -1 0\n0 -1 -2 -3\n");
    assert(countSafeReports(input8) == 2);

    std::cout << "All tests passed!" << std::endl;
    return 0;
}
