// Write a C++ function that takes a vector of course intervals, where each course is represented as `{duration, lastDay}`, and returns the maximum number of courses that can be taken without overlapping conflicts, given that each course must be completed by its last day (the course starts immediately at the current time and takes `duration` consecutive days). The function must handle courses with any positive integer durations and deadlines, and should return the maximal count of non-conflicting courses that fit within their deadlines, assuming courses can be scheduled in any order but each taken course occupies time sequentially from day 0.

// The problem is a classic "course scheduling with deadlines" greedy problem. The key insight is: to maximize the number of courses taken, we should always prioritize courses with earlier deadlines, and among those, we should keep the schedule as short as possible. The algorithm sorts all courses by their deadline (and then by duration as a tie-breaker) so that we always consider the most urgent deadlines first. We maintain a max-heap (priority queue) of the durations of courses we've selected so far, and a running total time `currentTime`. For each course in sorted order, we tentatively add it by incrementing `currentTime` by its duration and pushing the duration into the heap. If `currentTime` exceeds the course's deadline, we must drop the course with the longest duration from our selection (which is at the top of the max-heap) to make the schedule feasible, and subtract its duration from `currentTime`. This greedy removal of the longest course guarantees we lose the least progress toward the goal of maximizing the count. After processing all courses, the heap size equals the maximum number of courses that can be scheduled. Edge cases: if a single course's duration exceeds its own deadline, it will be added and then immediately removed (since `currentTime` will exceed `lim`), so such courses never count. If courses have identical deadlines, their durations are compared to keep the tightest fits. Time complexity is O(n log n) due to sorting and heap operations, where n is the number of courses; space complexity is O(n) for the heap.

#include <vector>
#include <queue>
#include <algorithm>

int maxCourses(std::vector<std::vector<int>>& courses) {
    // Sort by deadline, then by duration to ensure deterministic behavior
    std::sort(courses.begin(), courses.end(), [](const std::vector<int>& a, const std::vector<int>& b) {
        if (a[1] != b[1]) return a[1] < b[1];
        return a[0] < b[0];
    });

    std::priority_queue<int> maxHeap; // stores durations of selected courses
    int currentTime = 0;

    for (const auto& course : courses) {
        int duration = course[0];
        int deadline = course[1];

        currentTime += duration;
        maxHeap.push(duration);

        // If we exceed the deadline, remove the longest course
        if (currentTime > deadline) {
            currentTime -= maxHeap.top();
            maxHeap.pop();
        }
    }

    return static_cast<int>(maxHeap.size());
}

#include <cassert>
#include <vector>
#include <iostream>

// The solution function is declared above (for clarity, we include it here in the test file)
// In a standalone compile, the solution header would be included.

int main() {
    // Example 1: Basic
    std::vector<std::vector<int>> courses1 = {{100, 200}, {200, 1300}, {1000, 1250}, {2000, 3200}};
    assert(maxCourses(courses1) == 3);

    // Example 2: Overlapping deadlines
    std::vector<std::vector<int>> courses2 = {{1, 2}, {2, 3}, {3, 4}};
    assert(maxCourses(courses2) == 3);

    // Example 3: Long course that exceeds its own deadline
    std::vector<std::vector<int>> courses3 = {{5, 3}, {1, 100}, {2, 2}};
    assert(maxCourses(courses3) == 2); // can take {2,2} and {1,100}

    // Example 4: All courses have same deadline
    std::vector<std::vector<int>> courses4 = {{3, 5}, {2, 5}, {1, 5}};
    assert(maxCourses(courses4) == 2); // take shortest two: 1+2 <=5

    // Example 5: Empty vector
    std::vector<std::vector<int>> courses5 = {};
    assert(maxCourses(courses5) == 0);

    // Example 6: Single course that fits
    std::vector<std::vector<int>> courses6 = {{2, 3}};
    assert(maxCourses(courses6) == 1);

    // Example 7: Single course that doesn't fit
    std::vector<std::vector<int>> courses7 = {{4, 3}};
    assert(maxCourses(courses7) == 0);

    // Example 8: Duplicate courses
    std::vector<std::vector<int>> courses8 = {{2, 5}, {2, 5}, {2, 5}};
    assert(maxCourses(courses8) == 2); // can take two (total time 4 <=5)

    // Example 9: Edge case with zero duration? (not expected, but if present)
    // We assume positive durations; skip.

    // Example 10: Mixed
    std::vector<std::vector<int>> courses10 = {{1, 1}, {2, 3}, {3, 6}, {4, 7}};
    assert(maxCourses(courses10) == 3); // e.g., take {1,1},{2,3},{3,6} total 6

    std::cout << "All tests passed!" << std::endl;
    return 0;
}
