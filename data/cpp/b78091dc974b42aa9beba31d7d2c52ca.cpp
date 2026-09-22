// Given an array of student records where each record contains a name (string), roll number (integer), and marks (integer), write a C++ function that takes the number of students and a dynamically allocated array of `Student` objects (or a pointer to the first element), and returns a `std::string` containing all records in the format `"name roll marks"` each on a new line, sorted in descending order by marks. If two students have the same marks, sort them by roll number in ascending order. If the array is empty (n = 0), return an empty string. The function must not modify the original array and must use `const` appropriately.
The main approach is to first copy the input array into a temporary `std::vector<Student>` (since the input is given as a dynamic array, we can copy it to safely sort without modifying the original). Then, use `std::sort` with a custom comparator that compares two `Student` objects: first compare `marks` in descending order (larger marks come first); if marks are equal, compare `roll` in ascending order (smaller roll first). After sorting, iterate through the sorted vector and build an output string by appending each student's `name`, a space, `roll`, a space, `marks`, and a newline character. Handle the empty case (n = 0) by returning an empty string immediately to avoid dereferencing invalid pointers. Time complexity is O(n log n) due to sorting, and space complexity is O(n) for the vector copy and the output string. Edge cases include all students having the same marks (then sort by roll), negative marks, and names with spaces (though simple `cin` in the original snippet reads only a single word, the function should handle arbitrary strings; the test will use single-word names for simplicity).
#include <string>
#include <vector>
#include <algorithm>

struct Student {
    std::string name;
    int roll;
    int marks;
};

// Sort students by marks (descending), then by roll (ascending), and return formatted string.
std::string sortStudentsByMarks(const Student* students, int n) {
    if (n <= 0 || students == nullptr) {
        return "";
    }

    // Copy to a vector to sort without modifying the input array.
    std::vector<Student> vec(students, students + n);

    // Custom comparator: higher marks first; if equal, lower roll first.
    std::sort(vec.begin(), vec.end(), [](const Student& a, const Student& b) {
        if (a.marks != b.marks) {
            return a.marks > b.marks;
        }
        return a.roll < b.roll;
    });

    // Build output string.
    std::string result;
    for (const auto& s : vec) {
        result += s.name + " " + std::to_string(s.roll) + " " + std::to_string(s.marks) + "\n";
    }
    return result;
}
#include <cassert>
#include <iostream>
#include <string>

struct Student; // declaration from solution is not needed here; we define in test
// But we must include the solution's struct definition. Since the solution is provided separately,
// in a real setup you'd include the header. For this test, we re-declare the same struct.
struct Student {
    std::string name;
    int roll;
    int marks;
};

// Prototype of the solution function (already defined above in the solution).
std::string sortStudentsByMarks(const Student* students, int n);

int main() {
    // Test 1: Basic sorting by marks descending.
    Student arr1[] = {{"Alice", 101, 85}, {"Bob", 102, 92}, {"Charlie", 103, 78}};
    assert(sortStudentsByMarks(arr1, 3) == "Bob 102 92\nAlice 101 85\nCharlie 103 78\n");

    // Test 2: Tie in marks, sorted by roll ascending.
    Student arr2[] = {{"A", 3, 90}, {"B", 1, 90}, {"C", 2, 90}};
    assert(sortStudentsByMarks(arr2, 3) == "B 1 90\nC 2 90\nA 3 90\n");

    // Test 3: Empty array.
    Student* arr3 = nullptr;
    assert(sortStudentsByMarks(arr3, 0) == "");

    // Test 4: Single student.
    Student arr4[] = {{"Z", 7, 100}};
    assert(sortStudentsByMarks(arr4, 1) == "Z 7 100\n");

    // Test 5: Negative marks and same roll? roll unique, but marks can be negative.
    Student arr5[] = {{"X", 5, -10}, {"Y", 6, 0}, {"W", 7, -5}};
    assert(sortStudentsByMarks(arr5, 3) == "Y 6 0\nW 7 -5\nX 5 -10\n");

    // Test 6: All same marks, all same roll? Not possible if roll unique, but test same roll? Roll can be duplicate? The problem doesn't forbid. Use different marks.
    // Ensure input is not modified.
    Student arr6[] = {{"A", 1, 50}, {"B", 2, 60}};
    sortStudentsByMarks(arr6, 2);
    assert(arr6[0].name == "A" && arr6[0].roll == 1 && arr6[0].marks == 50);
    assert(arr6[1].name == "B" && arr6[1].roll == 2 && arr6[1].marks == 60);

    // Test 7: Duplicate marks and roll? Test with same marks and different roll already covered.
    // Test 8: Large values.
    Student arr7[] = {{"High", 999999999, 2147483647}, {"Low", 1, -2147483647}};
    assert(sortStudentsByMarks(arr7, 2) == "High 999999999 2147483647\nLow 1 -2147483647\n");

    // Test 9: Multiple same marks, same roll? Not typical, but roll can be same? We'll test same roll same marks but different names.
    Student arr8[] = {{"N", 5, 70}, {"M", 5, 70}};
    // Comparator only uses marks and roll, so order among equal (marks,roll) is unspecified, but both are same.
    std::string result8 = sortStudentsByMarks(arr8, 2);
    // Accept either order.
    assert(result8 == "N 5 70\nM 5 70\n" || result8 == "M 5 70\nN 5 70\n");

    // Test 10: Check output ends with newline even for multiple.
    Student arr9[] = {{"a", 1, 1}, {"b", 2, 2}};
    std::string result9 = sortStudentsByMarks(arr9, 2);
    assert(result9.size() > 0 && result9.back() == '\n');

    std::cout << "All tests passed!\n";
    return 0;
}
