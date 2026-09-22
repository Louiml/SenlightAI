// You are given `k` projects that can be completed, each with a `profit` and a `required capital`. You start with an initial capital `w`. In each of the first `k` project selections (if possible), you may pick any one project whose required capital is less than or equal to your current capital. Completing a project adds its profit to your capital. Your goal is to maximize your total capital after performing at most `k` projects. Write a C++ function `int maximizeCapital(int k, int initialCapital, const std::vector<int>& profits, const std::vector<int>& capital)` that returns the maximum possible final capital. The vectors have equal length (`n`), with `0 <= n <= 10^5`, `1 <= k <= 10^5`, `0 <= initialCapital, capital[i] <= 10^9`, and `0 <= profits[i] <= 10^4`. You may only start a project if your current capital is at least its required capital, and you cannot start the same project twice. If at any point no project is affordable, stop early.
#include <cassert>
#include <vector>

int main() {
    // Example 1: Basic scenario
    {
        std::vector<int> profits = {1, 2, 3};
        std::vector<int> capital = {0, 1, 1};
        assert(maximizeCapital(2, 0, profits, capital) == 4);
    }
    // Example 2: Cannot start any project
    {
        std::vector<int> profits = {10, 20};
        std::vector<int> capital = {5, 5};
        assert(maximizeCapital(3, 0, profits, capital) == 0);
    }
    // Example 3: k larger than n, all affordable
    {
        std::vector<int> profits = {5, 3, 2};
        std::vector<int> capital = {0, 0, 0};
        assert(maximizeCapital(10, 1, profits, capital) == 11);
    }
    // Example 4: Need to choose order wisely
    {
        std::vector<int> profits = {10, 2, 1};
        std::vector<int> capital = {0, 1, 2};
        assert(maximizeCapital(3, 0, profits, capital) == 13);
    }
    // Example 5: Empty vectors
    {
        std::vector<int> profits;
        std::vector<int> capital;
        assert(maximizeCapital(5, 100, profits, capital) == 100);
    }
    // Example 6: Large capital requirement, no progress
    {
        std::vector<int> profits = {100};
        std::vector<int> capital = {10};
        assert(maximizeCapital(1, 5, profits, capital) == 5);
    }
    // Example 7: Duplicates
    {
        std::vector<int> profits = {2, 2, 2};
        std::vector<int> capital = {0, 1, 1};
        assert(maximizeCapital(3, 0, profits, capital) == 6);
    }
    // Example 8: k = 0
    {
        std::vector<int> profits = {1, 2};
        std::vector<int> capital = {0, 0};
        assert(maximizeCapital(0, 5, profits, capital) == 5);
    }
    return 0;
}
#include <vector>
#include <queue>
#include <utility>

// Maximize final capital after at most k projects.
int maximizeCapital(int k, int initialCapital, const std::vector<int>& profits, const std::vector<int>& capital) {
    const int n = static_cast<int>(profits.size());
    // Min-heap: (required capital, project profit)
    std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, std::greater<std::pair<int, int>>> minCapital;
    // Max-heap: project profits for affordable projects
    std::priority_queue<int> maxProfits;

    // Push all projects into min-heap sorted by capital requirement
    for (int i = 0; i < n; ++i) {
        minCapital.push({capital[i], profits[i]});
    }

    int currentCapital = initialCapital;

    for (int step = 0; step < k; ++step) {
        // Move all projects we can currently afford into max-heap
        while (!minCapital.empty() && minCapital.top().first <= currentCapital) {
            maxProfits.push(minCapital.top().second);
            minCapital.pop();
        }

        if (maxProfits.empty()) {
            break;  // No affordable project left
        }

        // Take the most profitable affordable project
        currentCapital += maxProfits.top();
        maxProfits.pop();
    }

    return currentCapital;
}
// The problem is a classic greedy selection with two priority queues. First, group all projects by their capital requirement and profit. We sort projects by capital in ascending order, but more efficiently we can push all pairs `(capital[i], profits[i])` into a min-heap keyed by capital. Then we repeatedly, for each of the `k` steps, move every project whose required capital is ≤ current capital from the min-heap into a max-heap of profits. From that max-heap, we take the largest profit and add it to current capital. If the max-heap is empty (meaning no affordable project exists), we break early because no further progress is possible. Important edge cases: (1) `k` can be larger than `n`, but the loop will naturally stop when all projects are used or none are affordable. (2) Initial capital might already allow all projects, so the heap operations still work. (3) Duplicates in capital and profit are handled by the heaps. Time complexity: O(n log n + k log n) because each project is pushed once into the min-heap, popped once into the max-heap, and each top/pop on either heap is O(log n). Space complexity: O(n) for the two heaps.
