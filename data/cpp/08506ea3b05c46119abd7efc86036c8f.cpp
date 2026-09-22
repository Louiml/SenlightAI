Write a C++ function named `josephusName` that takes a vector of pairs, where each pair consists of a student's name (a 4-character string like "Amy" or "Bob") and an integer count, and returns the name of the last remaining student if students are arranged in a circle and eliminated using the Josephus-like rule: starting with the first student, count off the given number (the count value of the current student), remove that many students (i.e., skip count-1 students then eliminate the next one), and continue until only one student remains. The input guarantees at least one student and all names are exactly 4 characters. The counts can be any positive integers. Return the surviving student's name as a string.
// The problem is a classic Josephus elimination with variable step sizes. The main approach is to use a circular queue (or list) simulating the process exactly as described. Start from the front of the queue. For each round, take the front student (the "counter"), remove them, then move the next `(count-1)` students from the front to the back (this simulates skipping them), and finally remove the student now at the front (the one to be eliminated). Repeat until only one student remains in the queue. Edge cases: if only one student exists, return immediately. If a count is 1, that means the counter is eliminated immediately (skip 0, remove the front). If a count is large, the modulo operation is implicitly handled by cycling through the queue via repeated moves. Time complexity is O(total_steps), which in the worst case (e.g., all counts are large) could be O(n * max_count) — but since we shift elements via list operations that are O(1) each, and we perform at most total count steps across all eliminations, the overall complexity is O(total_sum_of_counts) which is acceptable for typical inputs. Space complexity is O(n) for storing the students. We must be careful with string handling since names are exactly 4 characters (could include a null terminator when converting to std::string).
#include <string>
#include <vector>
#include <list>
#include <utility>

// Simulates the Josephus-like elimination and returns the last remaining student's name.
std::string josephusName(const std::vector<std::pair<std::string, int>>& students) {
    std::list<std::pair<std::string, int>> circle(students.begin(), students.end());

    while (circle.size() > 1) {
        // The current student is the one who counts.
        auto current = circle.front();
        circle.pop_front();

        int steps = current.second; // number of students to skip before elimination
        // Move (steps - 1) students from front to back.
        for (int i = 1; i < steps; ++i) {
            circle.push_back(circle.front());
            circle.pop_front();
        }

        // The student at the front is now eliminated.
        circle.pop_front();
    }

    return circle.front().first;
}
#include <cassert>
#include <string>
#include <vector>
#include <utility>

// The solution function is assumed to be defined above (or included here).
// For the test, we include the declaration.
std::string josephusName(const std::vector<std::pair<std::string, int>>& students);

int main() {
    // Single student
    assert(josephusName({{"Amy", 5}}) == "Amy");

    // Simple case with n=2
    assert(josephusName({{"Amy", 1}, {"Bob", 2}}) == "Bob");
    // Explanation: Amy counts 1, eliminates herself, Bob remains.

    // Case from the original snippet: n=3, names "Amy" (3), "Bob" (2), "Cal" (4)
    std::vector<std::pair<std::string, int>> test1 = {{"Amy", 3}, {"Bob", 2}, {"Cal", 4}};
    assert(josephusName(test1) == "Bob");

    // Larger circle with variable counts
    std::vector<std::pair<std::string, int>> test2 = {{"Amy", 2}, {"Bob", 3}, {"Cal", 1}, {"Dan", 2}, {"Eve", 4}};
    assert(josephusName(test2) == "Eve");

    // Count of 1 means immediate elimination of the counter
    std::vector<std::pair<std::string, int>> test3 = {{"Amy", 1}, {"Bob", 1}, {"Cal", 1}};
    assert(josephusName(test3) == "Cal");

    // Large counts wrap around naturally
    std::vector<std::pair<std::string, int>> test4 = {{"Amy", 10}, {"Bob", 10}, {"Cal", 10}};
    assert(josephusName(test4) == "Cal");

    // Two students, count=2 on the first student: skip 1, eliminate the second, then first remains
    std::vector<std::pair<std::string, int>> test5 = {{"Amy", 2}, {"Bob", 2}};
    assert(josephusName(test5) == "Amy");

    return 0;
}
