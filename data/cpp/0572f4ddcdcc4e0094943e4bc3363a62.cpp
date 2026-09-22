Write a C++ function named `generateSquareRootTable` that takes a positive integer `maxNumber` and a `std::string` output filename. The function creates a tab-separated text file with two columns: "Number" and "Square Root", listing integers from 1 to `maxNumber` inclusive, where the square root is displayed with exactly 4 digits after the decimal point. The file must include a header row and a dashed separator line ("-------------------") below it. If the file cannot be created or `maxNumber` is less than 1, the function returns `false` and writes an appropriate error message to `std::cerr`; otherwise, it writes the data and returns `true`. The function should not print anything to the console on success.
The solution first validates the input: if `maxNumber < 1`, return `false` immediately to avoid creating an empty or invalid table. Then attempt to open the output file using `std::ofstream`. If the file stream fails (e.g., directory does not exist or permission denied), return `false` with an error message. On success, set fixed notation and precision to 4 decimal places for the square root values. Write the header and separator line. Then loop from `i = 1` to `maxNumber` inclusive, writing `i` and `std::sqrt(i)` separated by a tab, each on its own line. Close the file and return `true`. Edge cases include very large `maxNumber` (performance is linear in `maxNumber`, and each line uses constant space, so overall time is O(n) and extra space O(1)), and the case `maxNumber == 1` which produces exactly one data row. The use of `std::fixed` and `std::setprecision(4)` ensures consistent formatting regardless of locale or system. Complexity: O(n) time, O(1) auxiliary space.
#include <fstream>
#include <iostream>
#include <cmath>
#include <iomanip>
#include <string>

// Generate a tab-separated square root table file for numbers 1 to maxNumber.
// Returns true on success, false on invalid argument or file creation failure.
bool generateSquareRootTable(int maxNumber, const std::string& filename) {
    if (maxNumber < 1) {
        std::cerr << "Error: maxNumber must be at least 1." << std::endl;
        return false;
    }

    std::ofstream outFile(filename);
    if (!outFile) {
        std::cerr << "Error: Failed to create file " << filename << std::endl;
        return false;
    }

    outFile << std::fixed << std::setprecision(4);

    outFile << "Number\tSquare Root\n";
    outFile << "-------------------\n";

    for (int i = 1; i <= maxNumber; ++i) {
        outFile << i << "\t" << std::sqrt(i) << "\n";
    }

    outFile.close();
    return true;
}
#include <cassert>
#include <fstream>
#include <sstream>
#include <string>

// The solution function is declared above; here we test it.
// For brevity, the function definition is assumed present in the same translation unit.
// We will use a temporary file name for testing.

int main() {
    // Test 1: Valid creation with maxNumber = 5
    const std::string testFile1 = "test_sqrt_1.txt";
    assert(generateSquareRootTable(5, testFile1) == true);
    std::ifstream in1(testFile1);
    assert(in1.is_open());
    std::string line1;
    std::getline(in1, line1);
    assert(line1 == "Number\tSquare Root");
    std::getline(in1, line1);
    assert(line1 == "-------------------");
    std::getline(in1, line1);
    // First data line should be "1\t1.0000"
    assert(line1 == "1\t1.0000");
    std::getline(in1, line1);
    assert(line1 == "2\t1.4142");
    in1.close();
    remove(testFile1.c_str());

    // Test 2: maxNumber = 1 (single row)
    const std::string testFile2 = "test_sqrt_2.txt";
    assert(generateSquareRootTable(1, testFile2) == true);
    std::ifstream in2(testFile2);
    std::string line;
    std::getline(in2, line); // header
    std::getline(in2, line); // separator
    std::getline(in2, line);
    assert(line == "1\t1.0000");
    std::getline(in2, line); // should be empty (EOF)
    assert(line.empty());
    in2.close();
    remove(testFile2.c_str());

    // Test 3: Invalid maxNumber
    assert(generateSquareRootTable(0, "unused.txt") == false);
    assert(generateSquareRootTable(-3, "unused.txt") == false);

    // Test 4: File creation failure (invalid path on most systems)
    assert(generateSquareRootTable(2, "/nonexistent_dir/test.txt") == false);
}
