/*
Given \(n\) students, each with a set of \(5\) distinct-identifier courses they are considering, where a course is represented by a value of \(1\) (considering) or \(0\) (not considering), write a C++ function `bool canFormMajor(int n, const std::vector<std::array<int,5>>& students)` that returns `true` if there exist exactly two courses from the five such that every student is considering at least one of them, and each of the two chosen courses is considered by at least \(n/2\) students (integer division, i.e., floor). The function should handle any positive \(n\) and return `false` otherwise.
*/

#include <vector>
#include <array>

// Return true if there exist two courses such that every student considers at
// least one, and each course is considered by at least n/2 (floor) students.
bool canFormMajor(int n, const std::vector<std::array<int, 5>>& students) {
    // Try every unordered pair of courses (i, j).
    for (int i = 0; i < 5; ++i) {
        for (int j = i + 1; j < 5; ++j) {
            int count_i = 0;
            int count_j = 0;
            bool all_covered = true;
            for (int s = 0; s < n; ++s) {
                const auto& row = students[s];
                if (row[i] == 0 && row[j] == 0) {
                    all_covered = false;
                    break;
                }
                if (row[i] == 1) ++count_i;
                if (row[j] == 1) ++count_j;
            }
            int threshold = n / 2;
            if (all_covered && count_i >= threshold && count_j >= threshold) {
                return true;
            }
        }
    }
    return false;
}

#include <cassert>
#include <vector>
#include <array>

// The solution function is declared above (assume included).
int main() {
    // Case 1: n=1, student considers course 0 only. Pair (0,1) works.
    {
        int n = 1;
        std::vector<std::array<int,5>> s = {{{1,0,0,0,0}}};
        assert(canFormMajor(n, s) == true);
    }
    // Case 2: n=2, both students consider course 0 and 1 respectively, but each only one.
    {
        int n = 2;
        std::vector<std::array<int,5>> s = {{{1,0,0,0,0},{0,1,0,0,0}}};
        assert(canFormMajor(n, s) == true);
    }
    // Case 3: n=2, one student considers none of the five (impossible per spec? but we handle).
    {
        int n = 2;
        std::vector<std::array<int,5>> s = {{{0,0,0,0,0},{1,1,0,0,0}}};
        assert(canFormMajor(n, s) == false);
    }
    // Case 4: n=2, both consider only course 0, so pair (0,1) has count1=2, count2=0 -> fails.
    {
        int n = 2;
        std::vector<std::array<int,5>> s = {{{1,0,0,0,0},{1,0,0,0,0}}};
        assert(canFormMajor(n, s) == false);
    }
    // Case 5: n=3, students consider (0,1), (0,2), (1,2). Pair (0,1) gives counts 2 and 2, threshold 1.
    {
        int n = 3;
        std::vector<std::array<int,5>> s = {{{1,1,0,0,0},{1,0,1,0,0},{0,1,1,0,0}}};
        assert(canFormMajor(n, s) == true);
    }
    // Case 6: n=3, all consider only course 0. Need second course with at least 1, none.
    {
        int n = 3;
        std::vector<std::array<int,5>> s = {{{1,0,0,0,0},{1,0,0,0,0},{1,0,0,0,0}}};
        assert(canFormMajor(n, s) == false);
    }
    // Case 7: n=4, half are {0,1}, half are {2,3}. Pair (0,2) gives counts 2 and 2, threshold 2.
    {
        int n = 4;
        std::vector<std::array<int,5>> s = {{{1,1,0,0,0},{1,1,0,0,0},{0,0,1,1,0},{0,0,1,1,0}}};
        assert(canFormMajor(n, s) == true);
    }
    // Case 8: n=4, all consider {0,1}. Pair (0,1) gives counts 4 and 4, threshold 2.
    {
        int n = 4;
        std::vector<std::array<int,5>> s = {{{1,1,0,0,0},{1,1,0,0,0},{1,1,0,0,0},{1,1,0,0,0}}};
        assert(canFormMajor(n, s) == true);
    }
    // Case 9: n=5, each student considers exactly one distinct course among first three, but not enough overlap.
    // Pair (0,1): counts 2 and 2, threshold 2, but student considering course 2 doesn't have 0 or1 -> fail.
    {
        int n = 5;
        std::vector<std::array<int,5>> s = {{{1,0,0,0,0},{1,0,0,0,0},{0,1,0,0,0},{0,1,0,0,0},{0,0,1,0,0}}};
        assert(canFormMajor(n, s) == false);
    }
    // Case 10: n=5, one student considers all five, others consider none? Actually all must consider each chosen at least threshold.
    // Pair (0,1): count0=1, count1=1, threshold 2 -> fail anyway.
    {
        int n = 5;
        std::vector<std::array<int,5>> s = {{{1,1,1,1,1},{0,0,0,0,0},{0,0,0,0,0},{0,0,0,0,0},{0,0,0,0,0}}};
        assert(canFormMajor(n, s) == false);
    }
    return 0;
}

// The solution iterates over all \(\binom{5}{2} = 10\) pairs of courses. For each pair, we simulate counting how many students consider the first course and how many consider the second. However, we must also ensure that no student is missing both courses; if a student has neither, that pair is immediately invalid. For a valid pair, we require both counts to be at least \(n/2\) (floor division). Since there are only 5 courses, the loop is constant-size, and each pair requires a single pass over all students, giving \(O(10n) = O(n)\) time. Space is \(O(n)\) for storage of the input (or \(O(1)\) if processed streaming, but we accept the vector). Edge cases: \(n=1\) means \(n/2 = 0\), so any pair where the single student considers at least one course is valid, even if the counts are 1 and 0. Also, if \(n\) is odd, \(n/2\) is floor, e.g., for \(n=3\), threshold is 1, so counts of 1 and 1 are enough.
