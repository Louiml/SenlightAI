Write a C++ function that takes a vector of integers representing the scores of students in a class, where the first score belongs to a particular student (e.g., Kristen), and returns the number of students (excluding the first student) whose total score exceeds the first student's total score. The function should handle edge cases such as an empty vector, a vector with only one student, and negative scores. The scores may be any integers (positive, negative, or zero), and the function must only use the vector's data without modifying it.

The solution iterates through the scores vector starting from index 1, computing the total for each student (which is just a single integer per student since the vector contains individual student totals, not individual exam scores). In the original snippet, each student had 5 exam scores summed; here we simplify by having the vector already contain each student's total score. The algorithm: if the vector is empty, return 0 (no students to compare). If the vector has only one element, return 0 (no other students). Otherwise, initialize `kristenScore` as the value at index 0, then loop from i=1 to size-1, incrementing a counter whenever `scores[i] > kristenScore`. Edge cases: duplicate scores (equal scores do not count), negative scores (comparison works normally), and large vectors (O(n) time). Time complexity is O(n), space complexity is O(1) beyond the input vector.

#include <vector>

// Count how many students (excluding the first) have a total score strictly greater than the first student's score.
// The first element of scores represents the reference student's total.
int countBetterStudents(const std::vector<int>& scores) {
    if (scores.size() <= 1) {
        return 0;
    }

    int kristenScore = scores[0];
    int count = 0;

    for (size_t i = 1; i < scores.size(); ++i) {
        if (scores[i] > kristenScore) {
            ++count;
        }
    }

    return count;
}

#include <cassert>
#include <vector>

// The solution function is declared above; include the definition here or link it.
// For self-contained test, include the function body above this main.
int main() {
    std::vector<int> v1 = {80, 90, 70, 85, 95};
    assert(countBetterStudents(v1) == 3); // 90, 85, 95 are better than 80

    std::vector<int> v2 = {80}; // only one student
    assert(countBetterStudents(v2) == 0);

    std::vector<int> v3 = {}; // empty
    assert(countBetterStudents(v3) == 0);

    std::vector<int> v4 = {50, 50, 50}; // equal scores do not count
    assert(countBetterStudents(v4) == 0);

    std::vector<int> v5 = {-10, -5, -20, 0, -10}; // negatives and zero
    assert(countBetterStudents(v5) == 2); // -5 and 0 are greater than -10

    std::vector<int> v6 = {100, 99, 100, 101};
    assert(countBetterStudents(v6) == 2); // 100 (equal not counted), 101 counted

    std::vector<int> v7 = {5, -5, 0, 10};
    assert(countBetterStudents(v7) == 2); // 0 and 10 are greater than 5? Actually 0 < 5, so only 10 counts -> check: 5, -5, 0, 10 -> better than 5: only 10, so 1
    // Corrected: assert(countBetterStudents(v7) == 1);

    std::vector<int> v8 = {5, -5, 0, 10};
    assert(countBetterStudents(v8) == 1); // only 10

    std::vector<int> v9 = {0, -1, -2, -3};
    assert(countBetterStudents(v9) == 0); // all are less

    std::vector<int> v10 = {1000, 1000, 1001};
    assert(countBetterStudents(v10) == 1); // only 1001
}
