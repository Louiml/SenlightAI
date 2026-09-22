// Write a C++ function named `compareStudents` that accepts two student records, where a student record contains an integer `num`, a `std::string` `name`, and an integer `score`. The function must return a `std::string` that lists the record (in the format `"num name score"`) of the student with the higher score first (if scores are equal, list the first student's record first, then the second student's record), separated by a newline character (`'\n'`). The input is guaranteed to contain only ASCII characters, non‑empty names, and scores between 0 and 100 inclusive. The function should be const‑correct: it must not modify the passed student objects and should accept them by `const` reference.

The algorithm is straightforward: compare the two scores. If `a.score >= b.score`, then output `a` first followed by `b`; otherwise output `b` first followed by `a`. This ensures that when scores are equal, the first student appears first (because the condition uses `>=`). The output is constructed by concatenating the fields with spaces and a newline between the two records. Use `std::to_string` for the numeric fields. Edge cases: equal scores (handled by `>=`), zero score, and maximum score—none require special logic. Time complexity is O(1) because only two records are compared and string building is constant with respect to name length (assuming names are short but still O(name length) for copying; overall it's O(1) in terms of number of records). Space complexity is O(1) auxiliary (excluding the returned string).

#include <string>

// Return a string with the two student records, higher score first.
// If scores are equal, the first student is listed first.
std::string compareStudents(const Student& first, const Student& second) {
    // Build both formatted lines.
    std::string line1 = std::to_string(first.num) + " " + first.name + " " + std::to_string(first.score);
    std::string line2 = std::to_string(second.num) + " " + second.name + " " + std::to_string(second.score);
    
    // Higher score first; if equal, first student first.
    if (first.score >= second.score) {
        return line1 + "\n" + line2;
    } else {
        return line2 + "\n" + line1;
    }
}
*Note: Since the task requires a self‑contained free function, the `Student` struct must be defined before the function. The solution above assumes a global `struct Student { int num; std::string name; int score; };` is provided. In the test code below, we include the struct definition.*

#include <cassert>
#include <string>

// Student record definition (matching the original snippet's structure).
struct Student {
    int num;
    std::string name;
    int score;
};

// The solution function (copied here for completeness, but typically in a header).
std::string compareStudents(const Student& first, const Student& second) {
    std::string line1 = std::to_string(first.num) + " " + first.name + " " + std::to_string(first.score);
    std::string line2 = std::to_string(second.num) + " " + second.name + " " + std::to_string(second.score);
    if (first.score >= second.score) {
        return line1 + "\n" + line2;
    } else {
        return line2 + "\n" + line1;
    }
}

int main() {
    // Test 1: first has higher score.
    Student a{1, "Alice", 90};
    Student b{2, "Bob", 80};
    assert(compareStudents(a, b) == "1 Alice 90\n2 Bob 80");

    // Test 2: second has higher score.
    Student c{3, "Carol", 70};
    Student d{4, "Dave", 95};
    assert(compareStudents(c, d) == "4 Dave 95\n3 Carol 70");

    // Test 3: equal scores -> first listed first.
    Student e{5, "Eve", 85};
    Student f{6, "Frank", 85};
    assert(compareStudents(e, f) == "5 Eve 85\n6 Frank 85");

    // Test 4: first score zero, second score zero (equal).
    Student g{7, "Grace", 0};
    Student h{8, "Heidi", 0};
    assert(compareStudents(g, h) == "7 Grace 0\n8 Heidi 0");

    // Test 5: maximum scores.
    Student i{9, "Ivan", 100};
    Student j{10, "Judy", 100};
    assert(compareStudents(i, j) == "9 Ivan 100\n10 Judy 100");

    // Test 6: names with spaces (ensures formatting handles them).
    Student k{11, "Kate Smith", 77};
    Student l{12, "Leo Brown", 88};
    assert(compareStudents(k, l) == "12 Leo Brown 88\n11 Kate Smith 77");

    // Test 7: first score lower, second score higher.
    Student m{13, "Mallory", 50};
    Student n{14, "Nick", 99};
    assert(compareStudents(m, n) == "14 Nick 99\n13 Mallory 50");

    // Test 8: first score higher, second score lower.
    Student o{15, "Olivia", 66};
    Student p{16, "Paul", 33};
    assert(compareStudents(o, p) == "15 Olivia 66\n16 Paul 33");

    // Test 9: second score lower, equal names (not a problem).
    Student q{17, "Quincy", 45};
    Student r{18, "Quincy", 45};
    assert(compareStudents(q, r) == "17 Quincy 45\n18 Quincy 45");

    // Test 10: one student has score 0, other has score 100.
    Student s{19, "Sara", 0};
    Student t{20, "Tom", 100};
    assert(compareStudents(s, t) == "20 Tom 100\n19 Sara 0");

    return 0;
}
