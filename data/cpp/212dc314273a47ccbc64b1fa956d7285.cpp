/*
Write a C++ function `int countRooms(int n, int k, const std::vector<std::pair<int,int>>& students)` that determines the minimum number of rooms needed for a school camp. Each student is represented by a pair `(sex, grade)` where `sex` is `0` for female and `1` for male, and `grade` is an integer from `1` to `6`. Rooms are assigned separately by sex and grade: every room holds at most `k` students, and all students of the same sex and same grade must be in the same room(s), but no mixing of different sexes or grades is allowed. Given the number of students `n`, maximum room capacity `k`, and a list of student pairs, return the total number of rooms required. The input is guaranteed to be valid (grades between 1 and 6, sex 0 or 1), and `k` is a positive integer.
*/

#include <vector>
#include <utility>

// Returns the total number of rooms needed given maximum capacity k.
// students is a vector of (sex, grade) pairs, where sex is 0 (female) or 1 (male).
int countRooms(int n, int k, const std::vector<std::pair<int,int>>& students) {
    // 12 counters: index = 2*(grade-1) + sex, covering grades 1..6 and both sexes.
    int counts[12] = {0};
    
    for (const auto& s : students) {
        int sex = s.first;      // 0 or 1
        int grade = s.second;   // 1..6
        ++counts[2 * (grade - 1) + sex];
    }
    
    int totalRooms = 0;
    for (int i = 0; i < 12; ++i) {
        if (counts[i] > 0) {
            // Ceiling division: counts[i] / k rounded up.
            totalRooms += (counts[i] + k - 1) / k;
        }
    }
    return totalRooms;
}

#include <cassert>
#include <vector>
#include <utility>

// Declaration of the function under test (include the solution above).
int countRooms(int n, int k, const std::vector<std::pair<int,int>>& students);

int main() {
    // Example: 5 students, k=2, various sex/grade groups.
    std::vector<std::pair<int,int>> s1 = {{0,1},{0,1},{0,1},{1,1},{1,2}};
    assert(countRooms(5, 2, s1) == 4); // 3 females grade1 -> 2 rooms, 1 male grade1 -> 1 room, 1 male grade2 -> 1 room

    // All students same group, exact multiple of k.
    std::vector<std::pair<int,int>> s2 = {{1,3},{1,3},{1,3},{1,3}};
    assert(countRooms(4, 2, s2) == 2);

    // No students.
    std::vector<std::pair<int,int>> s3 = {};
    assert(countRooms(0, 5, s3) == 0);

    // Single student in a group.
    std::vector<std::pair<int,int>> s4 = {{0,6}};
    assert(countRooms(1, 3, s4) == 1);

    // Large k, many groups.
    std::vector<std::pair<int,int>> s5 = {{0,1},{0,2},{1,3},{1,4},{0,5},{1,6}};
    assert(countRooms(6, 10, s5) == 6); // each group needs its own room

    // Mixed counts with capacity exactly 1.
    std::vector<std::pair<int,int>> s6 = {{0,1},{0,1},{1,1}};
    assert(countRooms(3, 1, s6) == 3);

    // Boundary: 6 females and 6 males all in grade 1, k=6.
    std::vector<std::pair<int,int>> s7;
    for (int i = 0; i < 6; ++i) s7.push_back({0,1});
    for (int i = 0; i < 6; ++i) s7.push_back({1,1});
    assert(countRooms(12, 6, s7) == 2);

    // Non-divisible group size.
    std::vector<std::pair<int,int>> s8 = {{1,2},{1,2},{1,2},{1,2},{1,2}};
    assert(countRooms(5, 3, s8) == 2); // 5 students / 3 capacity -> 2 rooms

    return 0;
}

// The solution counts, for each of the 12 possible sex–grade combinations (2 sexes × 6 grades), how many students belong to that combination. Then for each nonempty combination, the number of rooms needed is the ceiling of that count divided by `k`. Since the capacity is uniform, this can be computed as `(count + k - 1) / k` using integer arithmetic, which avoids branching. Edge cases include having zero students in a combination (skip), a count exactly divisible by `k` (the formula gives the exact quotient), and a count less than `k` (gives 1). The algorithm processes all students in \(O(n)\) time and uses constant extra space \(O(1)\) because only 12 counters are maintained. No sorting or other preprocessing is required, and the result fits in an `int` given typical constraints.
