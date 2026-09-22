/*
Write a C++ function that processes a sequence of student records entered interactively, where each record consists of a registration number (RA) and two integer scores (nota1, nota2). The records must be stored in ascending order of RA while preserving all data. The function should read RA values until 0 is entered (which terminates input), insert each new record at the correct sorted position, and return the sorted lists of RAs and their corresponding scores. The function should handle duplicate RAs by inserting new records after existing ones with the same RA (stable insertion). Assume a maximum capacity of 10 records; if more than 10 are entered, ignore additional records. The function signature must be: `std::vector<Student> insertSortedStudents()`; where `Student` is a struct with fields `ra`, `nota1`, `nota2`. The returned vector must contain all valid inserted records in ascending RA order.
*/

#include <vector>
#include <iostream>

struct Student {
    int ra;
    int nota1;
    int nota2;
};

// Reads student records from standard input until RA 0 is entered,
// inserts each record in ascending RA order (stable for duplicates),
// and returns the sorted list. Ignores records beyond capacity 10.
std::vector<Student> insertSortedStudents() {
    const int MAX_CAPACITY = 10;
    std::vector<Student> students;
    students.reserve(MAX_CAPACITY);

    int ra;
    std::cin >> ra;

    while (ra != 0) {
        if (students.size() < MAX_CAPACITY) {
            // Find insertion position: first index with ra >= current (stable)
            int pos = 0;
            while (pos < static_cast<int>(students.size()) && ra > students[pos].ra) {
                ++pos;
            }

            // Read the two scores (only if we are going to insert)
            int nota1, nota2;
            std::cin >> nota1 >> nota2;

            // Insert at pos (shift elements to the right)
            students.insert(students.begin() + pos, Student{ra, nota1, nota2});
        } else {
            // Capacity full: consume scores but ignore record
            int dummy1, dummy2;
            std::cin >> dummy1 >> dummy2;
        }

        std::cin >> ra;
    }

    return students;
}

#include <cassert>
#include <vector>
#include <sstream>
#include <iostream>

// Include the solution function here (or link appropriately)

int main() {
    // Test 1: Basic insertion in order
    {
        std::istringstream input("3 10 20\n1 30 40\n2 50 60\n0\n");
        std::cin.rdbuf(input.rdbuf());
        std::vector<Student> result = insertSortedStudents();
        assert(result.size() == 3);
        assert(result[0].ra == 1 && result[0].nota1 == 30 && result[0].nota2 == 40);
        assert(result[1].ra == 2 && result[1].nota1 == 50 && result[1].nota2 == 60);
        assert(result[2].ra == 3 && result[2].nota1 == 10 && result[2].nota2 == 20);
    }

    // Test 2: Reverse order insertion
    {
        std::istringstream input("5 1 2\n4 3 4\n3 5 6\n0\n");
        std::cin.rdbuf(input.rdbuf());
        std::vector<Student> result = insertSortedStudents();
        assert(result.size() == 3);
        assert(result[0].ra == 3 && result[0].nota1 == 5 && result[0].nota2 == 6);
        assert(result[1].ra == 4 && result[1].nota1 == 3 && result[1].nota2 == 4);
        assert(result[2].ra == 5 && result[2].nota1 == 1 && result[2].nota2 == 2);
    }

    // Test 3: Duplicate RAs (stable order)
    {
        std::istringstream input("2 10 10\n2 20 20\n1 30 30\n0\n");
        std::cin.rdbuf(input.rdbuf());
        std::vector<Student> result = insertSortedStudents();
        assert(result.size() == 3);
        assert(result[0].ra == 1 && result[0].nota1 == 30 && result[0].nota2 == 30);
        assert(result[1].ra == 2 && result[1].nota1 == 10 && result[1].nota2 == 10);
        assert(result[2].ra == 2 && result[2].nota1 == 20 && result[2].nota2 == 20);
    }

    // Test 4: Single record
    {
        std::istringstream input("42 7 8\n0\n");
        std::cin.rdbuf(input.rdbuf());
        std::vector<Student> result = insertSortedStudents();
        assert(result.size() == 1);
        assert(result[0].ra == 42 && result[0].nota1 == 7 && result[0].nota2 == 8);
    }

    // Test 5: Empty input (immediate 0)
    {
        std::istringstream input("0\n");
        std::cin.rdbuf(input.rdbuf());
        std::vector<Student> result = insertSortedStudents();
        assert(result.empty());
    }

    // Test 6: Capacity limit (input 11 records, only first 10 kept)
    {
        std::istringstream input("1 1 1\n2 2 2\n3 3 3\n4 4 4\n5 5 5\n6 6 6\n7 7 7\n8 8 8\n9 9 9\n10 10 10\n11 11 11\n0\n");
        std::cin.rdbuf(input.rdbuf());
        std::vector<Student> result = insertSortedStudents();
        assert(result.size() == 10);
        assert(result[0].ra == 1);
        assert(result[9].ra == 10);
    }

    // Test 7: Mixed arbitrary order
    {
        std::istringstream input("100 5 5\n50 6 6\n75 7 7\n25 8 8\n0\n");
        std::cin.rdbuf(input.rdbuf());
        std::vector<Student> result = insertSortedStudents();
        assert(result.size() == 4);
        assert(result[0].ra == 25);
        assert(result[1].ra == 50);
        assert(result[2].ra == 75);
        assert(result[3].ra == 100);
    }

    // Test 8: Negative RAs? (Not specified, assume positive only, but test with negative works)
    {
        std::istringstream input("-1 1 1\n-3 2 2\n-2 3 3\n0\n");
        std::cin.rdbuf(input.rdbuf());
        std::vector<Student> result = insertSortedStudents();
        assert(result.size() == 3);
        assert(result[0].ra == -3);
        assert(result[1].ra == -2);
        assert(result[2].ra == -1);
    }

    std::cout << "All tests passed!\n";
    return 0;
}

// The core algorithm mirrors insertion sort: for each new RA, we find the correct insertion position by scanning the existing sorted list from the beginning until we find the first element with RA greater than or equal to the new RA (to ensure stability for duplicates). Then we shift all elements from that position to the end one step to the right, and place the new record in the vacated slot. Reading continues until RA 0 is entered. Edge cases: (1) Empty list – insertion position is 0, no shifting; (2) RA larger than all existing – insertion position equals current size, no shifting; (3) Duplicate RA – we insert after existing equal RAs because the condition is `>=`; (4) Capacity limit of 10 – if current size is already 10, ignore further records (do not process them, but still consume input). Complexity: For n inserted records, each insertion takes O(n) time for shifting, so total time O(n²). Space usage is O(n) for storage (up to 10), plus constant overhead for the vector. The solution uses a vector of a custom struct, and we apply `const` correctness where appropriate in helper functions, though the main function itself reads from standard input.
