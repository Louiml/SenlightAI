Write a C++ function named `classifyStudents` that takes no parameters and returns a `std::string`. Inside the function, simulate reading exactly 10 students' scores, where each student has exactly 4 floating-point scores between 0 and 10 (assume the input is always valid in this range). For each student, compute the average of the 4 scores. Then, build and return a string that contains, for each student whose average is greater than or equal to 7.0, a line in the exact format: `"Aluno X com Media: Y.YY\n"`, where `X` is the student number (1-based) and `Y.YY` is the average formatted with exactly two decimal places and a dot as the decimal separator. Students with an average below 7.0 are omitted. Preserve the natural order of student numbers. The function must not read from standard input or write to standard output; instead, it will be tested by a harness that provides a mock input mechanism (not required for your implementation—just use a hardcoded test data table inside the function for demonstration purposes, as the function will be called directly with no arguments). For the sake of a self-contained test, use a fixed table of 10 students' scores (e.g., student 1 has scores 8.0, 7.5, 9.0, 8.5; student 2 has 5.0, 6.0, 5.5, 6.5; etc.). The function should be deterministic and return the same string every call.
#include <cassert>
#include <string>
#include <iostream>

// Forward declaration of the function under test (if not included from header).
std::string classifyStudents();

int main() {
    // Test 1: Verify exact output for the predefined data.
    std::string expected = 
        "Aluno 1 com Media: 8.25\n"
        "Aluno 3 com Media: 7.00\n"
        "Aluno 5 com Media: 10.00\n"
        "Aluno 6 com Media: 7.00\n"
        "Aluno 7 com Media: 7.50\n"
        "Aluno 9 com Media: 7.50\n";
    assert(classifyStudents() == expected);

    // Test 2: Check that the output contains the correct number of lines.
    std::string result = classifyStudents();
    int lines = 0;
    for (char c : result) {
        if (c == '\n') ++lines;
    }
    assert(lines == 6);

    // Test 3: Ensure each line starts with "Aluno " and contains " com Media: ".
    size_t pos = 0;
    int count = 0;
    while ((pos = result.find("Aluno ", pos)) != std::string::npos) {
        ++count;
        pos += 6;
    }
    assert(count == 6);

    // Test 4: Check that formatting uses exactly two decimal places.
    assert(result.find("8.25") != std::string::npos);
    assert(result.find("7.00") != std::string::npos);
    assert(result.find("10.00") != std::string::npos);

    // Test 5: Verify that students with averages below 7.0 are absent.
    assert(result.find("Aluno 2") == std::string::npos);
    assert(result.find("Aluno 4") == std::string::npos);
    assert(result.find("Aluno 8") == std::string::npos);
    assert(result.find("Aluno 10") == std::string::npos);

    // Test 6: Verify the exact order of students.
    size_t first = result.find("Aluno 1");
    size_t third = result.find("Aluno 3");
    size_t fifth = result.find("Aluno 5");
    size_t sixth = result.find("Aluno 6");
    size_t seventh = result.find("Aluno 7");
    size_t ninth = result.find("Aluno 9");
    assert(first < third && third < fifth && fifth < sixth && sixth < seventh && seventh < ninth);

    std::cout << "All tests passed.\n";
    return 0;
}
#include <string>
#include <sstream>
#include <iomanip>
#include <vector>

// Returns a string listing students (1-based) with average >= 7.0
// Format: "Aluno X com Media: Y.YY\n" for each qualifying student.
std::string classifyStudents() {
    // Predefined scores for 10 students, each with 4 scores.
    const std::vector<std::vector<float>> scores = {
        {8.0f, 7.5f, 9.0f, 8.5f},  // student 1
        {5.0f, 6.0f, 5.5f, 6.5f},  // student 2
        {7.0f, 7.0f, 7.0f, 7.0f},  // student 3
        {4.0f, 3.0f, 2.0f, 1.0f},  // student 4
        {10.0f, 10.0f, 10.0f, 10.0f}, // student 5
        {6.0f, 8.0f, 6.0f, 8.0f},  // student 6
        {9.0f, 8.0f, 7.0f, 6.0f},  // student 7
        {2.0f, 2.0f, 2.0f, 2.0f},  // student 8
        {7.5f, 7.5f, 7.5f, 7.5f},  // student 9
        {0.0f, 0.0f, 0.0f, 0.0f}   // student 10
    };
    
    std::ostringstream result;
    for (int i = 0; i < 10; ++i) {
        float sum = 0.0f;
        for (int j = 0; j < 4; ++j) {
            sum += scores[i][j];
        }
        float average = sum / 4.0f;
        if (average >= 7.0f) {
            result << "Aluno " << (i + 1) << " com Media: "
                   << std::fixed << std::setprecision(2) << average << "\n";
        }
    }
    return result.str();
}
// The main algorithm is straightforward: iterate over exactly 10 students. For each student, we compute the sum of the 4 scores and divide by 4 to get the average. We then check if the average is >= 7.0. If yes, we append a formatted line to a `std::ostringstream` or a `std::string` using `std::fixed` and `std::setprecision(2)` to ensure exactly two decimal places with a dot separator. The student number is 1-based (i from 0 to 9, print i+1). The function does not accept any input; instead, it uses a pre-defined array of 10x4 scores. Edge cases: all students below 7.0 results in an empty string; averages exactly 7.0 are included; formatting must be consistent regardless of the platform (use `std::ostringstream` and `std::fixed`). Time complexity is O(10) = O(1) because the number of students is fixed; space complexity is O(1) for computation and O(n) for the returned string, but constant in practice.
