/*
Write a C++ function named `calculateScholarships` that takes a vector of student records (each containing a name, final exam score, class evaluation score, whether the student is a class leader, gender, and number of published papers) and returns a `struct ScholarshipResult` holding the name of the student with the highest total scholarship, the amount of that highest scholarship, and the total scholarship sum across all students. The scholarship rules are: if final exam > 80 and papers > 0, add 8000; if final exam > 85 and class score > 80, add 4000; if final exam > 90, add 2000; if final exam > 85 and gender is 'Y' (female), add 1000; if class score > 80 and is class leader, add 850. The function must handle any number of students (including zero), and if there is a tie for the highest scholarship, choose the student who appears earliest in the input order. Use `const` references for input and avoid any global state. The input may contain names with spaces or special characters—use `std::string` for names.
*/
#include <string>
#include <vector>

struct Student {
    std::string name;
    int finalExam;
    int classScore;
    bool isLeader;
    char gender; // 'Y' for female, 'N' for male
    int papers;
};

struct ScholarshipResult {
    std::string topName;
    int topAmount;
    int totalAmount;
};

// Compute scholarships for all students and return the top earner and total sum.
ScholarshipResult calculateScholarships(const std::vector<Student>& students) {
    ScholarshipResult result{"", 0, 0};
    if (students.empty()) return result;

    int maxScholarship = 0;
    int totalSum = 0;
    size_t bestIndex = 0;

    for (size_t i = 0; i < students.size(); ++i) {
        const Student& s = students[i];
        int scholarship = 0;

        if (s.finalExam > 80 && s.papers > 0) scholarship += 8000;
        if (s.finalExam > 85 && s.classScore > 80) scholarship += 4000;
        if (s.finalExam > 90) scholarship += 2000;
        if (s.finalExam > 85 && s.gender == 'Y') scholarship += 1000;
        if (s.classScore > 80 && s.isLeader) scholarship += 850;

        totalSum += scholarship;

        // Strict greater ensures the first student wins ties.
        if (scholarship > maxScholarship) {
            maxScholarship = scholarship;
            bestIndex = i;
        }
    }

    result.topName = students[bestIndex].name;
    result.topAmount = maxScholarship;
    result.totalAmount = totalSum;
    return result;
}
#include <cassert>

int main() {
    std::vector<Student> students;
    ScholarshipResult r;

    // Empty input
    r = calculateScholarships(students);
    assert(r.topName.empty() && r.topAmount == 0 && r.totalAmount == 0);

    // Single student with all criteria
    students = {{"Alice", 95, 90, true, 'Y', 3}};
    r = calculateScholarships(students);
    assert(r.topName == "Alice" && r.topAmount == 8000+4000+2000+1000+850 && r.totalAmount == r.topAmount);

    // Two students, second has higher scholarship
    students = {{"Bob", 80, 70, false, 'N', 0},
                {"Eve", 91, 85, true, 'Y', 1}};
    r = calculateScholarships(students);
    assert(r.topName == "Eve" && r.topAmount == 8000+4000+2000+1000+850);
    assert(r.totalAmount == 0 + (8000+4000+2000+1000+850));

    // Tie – first student should win
    students = {{"Zoe", 90, 90, true, 'Y', 1},
                {"Max", 90, 90, true, 'Y', 1}};
    r = calculateScholarships(students);
    assert(r.topName == "Zoe" && r.topAmount == 8000+4000+2000+1000+850);

    // Student with zero scholarship
    students = {{"None", 60, 60, false, 'N', 0}};
    r = calculateScholarships(students);
    assert(r.topName == "None" && r.topAmount == 0 && r.totalAmount == 0);

    // Mixed conditions
    students = {{"A", 86, 90, false, 'Y', 0},  // 4000+1000 = 5000
                {"B", 82, 70, true, 'N', 2},   // 8000
                {"C", 91, 70, false, 'N', 0}}; // 2000
    r = calculateScholarships(students);
    assert(r.topName == "B" && r.topAmount == 8000);
    assert(r.totalAmount == 5000 + 8000 + 2000);

    // Multiple students with no qualification except class score and leader
    students = {{"L", 70, 85, true, 'N', 0},
                {"M", 70, 90, false, 'N', 0}};
    r = calculateScholarships(students);
    assert(r.topName == "L" && r.topAmount == 850);
    assert(r.totalAmount == 850 + 0);

    // All possible small checks
    students = {{"T", 90, 80, false, 'N', 0}}; // only 2000
    r = calculateScholarships(students);
    assert(r.topAmount == 2000 && r.topName == "T" && r.totalAmount == 2000);

    return 0;
}
// The solution processes each student exactly once, computing their total scholarship by applying the five independent rules in order, summing the contributions. The highest scholarship and its owner are tracked by initializing with the first student’s data and updating only when a strictly greater total is found, ensuring the earliest student wins ties. The total sum is accumulated across all students. Edge cases include: an empty vector (return an empty name, zero amounts), students who qualify for no scholarships (total 0), and multiple students with equal totals (pick first). Since we only read each student’s fields once and use a constant number of variables, the time complexity is **O(N)** for N students, and space complexity is **O(1)** beyond the input storage (the input vector is not modified, and no additional large structures are used).
