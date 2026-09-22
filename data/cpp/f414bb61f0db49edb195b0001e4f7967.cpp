Write a C++ function that processes the academic records of exactly 5 students, where each student is identified by a unique roll number and has marks for 5 subjects. The function should take as input five roll numbers and five arrays/containers of 5 integers each (the marks per subject for each student), and return the roll number of the student with the highest **aggregate** (sum of all 5 subject marks). If there is a tie for the highest aggregate, return the roll number of the student who appears first in the input order. You must use a structured approach (e.g., arrays or `std::array`) rather than 25 separate scalar variables, and the function must not print anything — it must only return the winning roll number as an integer.

The core task is to compute the sum of marks for each of the 5 students and then find the maximum aggregate, tracking the roll number of the student who achieves it. The main algorithm: initialize the first student’s aggregate as the current maximum and their roll number as the current winner. Then, for each subsequent student, compute their aggregate and if it is strictly greater than the current maximum, update both the maximum and the winner. Because ties are broken by input order (first occurrence wins), we only update on strict `>` comparison, not `>=`. Edge cases: all marks are zero or negative — works fine since we start with the first student’s sum; duplicates of the highest aggregate — handled by no update on equality; input must always contain exactly 5 students × 5 marks (we assume valid input). Time complexity: O(1) since we process exactly 25 numbers (5×5). Space complexity: O(5) for the arrays (if passed by reference) or O(1) extra if we compute sums on the fly; but since we need to store the roll numbers and marks, using `std::array` of size 5 for each is O(5) which is constant. The solution emphasizes `const` correctness by taking the marks arrays as `const` references and the roll numbers as `const` references.

#include <array>
#include <cstddef>

// Returns the roll number of the student with the highest total marks.
// Ties are broken by the first occurrence in the input order.
int highest_aggregate(const std::array<int, 5>& roll_numbers,
                      const std::array<std::array<int, 5>, 5>& marks) {
    // Compute the aggregate for the first student
    int best_roll = roll_numbers[0];
    int best_sum = 0;
    for (int mark : marks[0]) {
        best_sum += mark;
    }

    // Compare with the remaining students
    for (std::size_t i = 1; i < 5; ++i) {
        int current_sum = 0;
        for (int mark : marks[i]) {
            current_sum += mark;
        }
        // Strict '>' ensures first occurrence wins on ties
        if (current_sum > best_sum) {
            best_sum = current_sum;
            best_roll = roll_numbers[i];
        }
    }
    return best_roll;
}

#include <cassert>
#include <array>

// The function under test is declared above; include the solution code before this main.

int main() {
    // Test 1: Simple case with distinct aggregates
    std::array<int, 5> rolls1 = {101, 102, 103, 104, 105};
    std::array<std::array<int, 5>, 5> marks1 = {{
        {10, 20, 30, 40, 50},  // sum = 150
        {1, 2, 3, 4, 5},       // sum = 15
        {5, 5, 5, 5, 5},       // sum = 25
        {20, 20, 20, 20, 20},  // sum = 100
        {0, 0, 0, 0, 0}        // sum = 0
    }};
    assert(highest_aggregate(rolls1, marks1) == 101);

    // Test 2: Tie breaking — first student has the same highest sum as later one
    std::array<int, 5> rolls2 = {1, 2, 3, 4, 5};
    std::array<std::array<int, 5>, 5> marks2 = {{
        {1, 1, 1, 1, 1},  // sum = 5
        {5, 5, 5, 5, 5},  // sum = 25
        {2, 2, 2, 2, 2},  // sum = 10
        {3, 3, 3, 3, 3},  // sum = 15
        {5, 5, 5, 5, 5}   // sum = 25 (tie with roll 2)
    }};
    // Since roll 2 appears before roll 5, roll 2 wins.
    assert(highest_aggregate(rolls2, marks2) == 2);

    // Test 3: All sums equal — first roll number wins
    std::array<int, 5> rolls3 = {111, 222, 333, 444, 555};
    std::array<std::array<int, 5>, 5> marks3 = {{
        {1, 2, 3, 4, 0},  // sum = 10
        {4, 3, 2, 1, 0},  // sum = 10
        {2, 2, 2, 2, 2},  // sum = 10
        {0, 0, 0, 0, 10}, // sum = 10
        {5, 5, 0, 0, 0}   // sum = 10
    }};
    assert(highest_aggregate(rolls3, marks3) == 111);

    // Test 4: Negative marks — highest aggregate is the least negative
    std::array<int, 5> rolls4 = {10, 20, 30, 40, 50};
    std::array<std::array<int, 5>, 5> marks4 = {{
        {-1, -1, -1, -1, -1}, // sum = -5
        {0, 0, 0, 0, 0},      // sum = 0
        {-2, -2, -2, -2, -2}, // sum = -10
        {2, 2, 2, 2, 2},      // sum = 10
        {1, 1, 1, 1, 1}       // sum = 5
    }};
    assert(highest_aggregate(rolls4, marks4) == 40);

    // Test 5: Last student has the highest aggregate
    std::array<int, 5> rolls5 = {1, 2, 3, 4, 999};
    std::array<std::array<int, 5>, 5> marks5 = {{
        {1, 1, 1, 1, 1},   // sum = 5
        {2, 2, 2, 2, 2},   // sum = 10
        {3, 3, 3, 3, 3},   // sum = 15
        {4, 4, 4, 4, 4},   // sum = 20
        {10, 10, 10, 10, 10} // sum = 50
    }};
    assert(highest_aggregate(rolls5, marks5) == 999);

    // Test 6: Zero marks for all students — first roll wins
    std::array<int, 5> rolls6 = {7, 8, 9, 10, 11};
    std::array<std::array<int, 5>, 5> marks6 = {{
        {0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0}
    }};
    assert(highest_aggregate(rolls6, marks6) == 7);

    // Test 7: Large numbers (potential overflow is not an issue within int range)
    std::array<int, 5> rolls7 = {1, 2, 3, 4, 5};
    std::array<std::array<int, 5>, 5> marks7 = {{
        {100000, 100000, 100000, 100000, 100000}, // sum = 500000
        {99999, 99999, 99999, 99999, 99999},     // sum = 499995
        {100, 100, 100, 100, 100},               // sum = 500
        {0, 0, 0, 0, 0},                         // sum = 0
        {-50000, -50000, -50000, -50000, -50000} // sum = -250000
    }};
    assert(highest_aggregate(rolls7, marks7) == 1);

    return 0;
}
