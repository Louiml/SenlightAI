Write a C++ function `countBestStudents(const std::vector<std::string>& answers, int numQuestions)` that takes a vector of strings, each string representing a student’s answers to multiple-choice questions (each character is a digit from `'0'` to `'9'`), and an integer `numQuestions` equal to the length of every string. For each question position, the "best" possible answer is the maximum digit appearing among all students at that position. A student is considered "best" if, for at least one question position, their answer equals that position's maximum digit. Return the number of such best students. The input will always contain at least one student and at least one question, and all strings have the same length.
#include <cassert>
#include <vector>
#include <string>

// Declaration of the function under test (provided elsewhere).
int countBestStudents(const std::vector<std::string>& answers, int numQuestions);

int main() {
    // Basic case: multiple students, multiple questions.
    std::vector<std::string> test1 = {"12", "34", "56"};
    assert(countBestStudents(test1, 2) == 3); // column max: 5 and 6, each student has at least one max? Actually 1≠5,2≠6 no; 3≠5,4≠6 no; 5==5 yes, 6==6 yes → all three? Wait: student1 "12": no match; student2 "34": no; student3 "56": both match → count 1. So fix? Let’s recalc: maxima: col0 max(1,3,5)=5, col1 max(2,4,6)=6. Student1: 1 vs5 no, 2 vs6 no → not best. Student2: 3 vs5 no, 4 vs6 no → not best. Student3: 5==5 yes → best. So expected = 1.
    assert(countBestStudents(test1, 2) == 1);

    // All digits same: every student is best.
    std::vector<std::string> test2 = {"77", "77"};
    assert(countBestStudents(test2, 2) == 2);

    // Single student, single question.
    std::vector<std::string> test3 = {"5"};
    assert(countBestStudents(test3, 1) == 1);

    // Digits from 0 to 9.
    std::vector<std::string> test4 = {"90", "19", "45"};
    // column max: col0 max(9,1,4)=9, col1 max(0,9,5)=9.
    // student1 "90": has 9 at col0 → best; student2 "19": has 9 at col1 → best; student3 "45": no 9 → not best.
    assert(countBestStudents(test4, 2) == 2);

    // One column where max appears in all rows.
    std::vector<std::string> test5 = {"3", "3", "3"};
    assert(countBestStudents(test5, 1) == 3);

    // Multiple students with no matches? Impossible because max is taken from at least one student, so that student will match. But if all have different values, only the one(s) with the max value per column are best.
    std::vector<std::string> test6 = {"01", "10"};
    // column max: col0 max(0,1)=1, col1 max(1,0)=1.
    // "01": has 1 at col1 → best; "10": has 1 at col0 → best → 2
    assert(countBestStudents(test6, 2) == 2);

    // Empty vector should return 0.
    std::vector<std::string> test7;
    assert(countBestStudents(test7, 0) == 0);

    return 0;
}
#include <vector>
#include <string>
#include <algorithm>

// Count students who have at least one answer equal to the column maximum.
// Input: vector of strings (all same length), and the number of questions (columns).
// Returns: number of students that are "best" for at least one question.
int countBestStudents(const std::vector<std::string>& answers, int numQuestions) {
    if (answers.empty() || numQuestions <= 0) return 0;
    
    std::vector<int> maxGrade(numQuestions, 0);
    for (const auto& student : answers) {
        for (int j = 0; j < numQuestions; ++j) {
            maxGrade[j] = std::max(maxGrade[j], student[j] - '0');
        }
    }
    
    int bestCount = 0;
    for (const auto& student : answers) {
        bool isBest = false;
        for (int j = 0; j < numQuestions; ++j) {
            if (student[j] - '0' == maxGrade[j]) {
                isBest = true;
                break;
            }
        }
        if (isBest) ++bestCount;
    }
    return bestCount;
}
// The approach is straightforward: first compute the maximum digit for each column (question position) by iterating through all students and all columns simultaneously. We can maintain a vector `maxGrade` of size `m` initialized to 0, and for each student’s string, update each column’s maximum with the current digit’s value (`s[i][j] - '0'`). After processing all students, we have the per-question maxima. Then, for each student, we check if any of their answers equals the corresponding column maximum; if so, count them. The algorithm runs in O(n*m) time, where n is the number of students and m is the number of questions, and uses O(m) extra space for the maxima array. Edge cases include strings with digits `'0'` (which is fine since initializing `maxGrade` to 0 works because the minimum possible digit is '0'), and cases where all answers in a column are the same, so every student might be best if they match in that column.
