// Write a C++ function `josephusPermutation(int n, int k)` that returns a string representing the Josephus problem elimination order for `n` people numbered 1 through `n`, removing every `k`-th person in a circular queue, with the output formatted exactly as `<a, b, c, ...>`. The function must handle edge cases such as `k == 1` (natural order), `n == 1` (single element), and large values up to at least 1000 for both `n` and `k`, without integer overflow. The returned string must have no trailing spaces, and the comma-space separator must appear only between elements. The solution should not use recursion, and must work in-place with a queue structure.

The core algorithm uses a `std::queue<int>` to store the numbers 1 through `n`. We repeatedly rotate the queue by moving the first `k-1` elements to the back (popping and pushing them), then the front element is the next to be eliminated; we pop it, append it to the result string. This simulates the circular counting. Edge cases: if `k == 1`, the rotation loop does nothing (runs `k-1 = 0` times), so the queue is popped in natural order; if `n == 1`, the loop runs once and outputs `<1>`. For efficiency, we use an `ostringstream` to build the result to avoid repeated string concatenation. Time complexity is O(n·k) because for each of the `n` removals we perform `k-1` queue rotations, and each queue operation is O(1). Space complexity is O(n) for the queue plus O(n) for the output string.

#include <queue>
#include <sstream>
#include <string>

// Return the elimination order for the Josephus problem with n people and step k.
std::string josephusPermutation(int n, int k) {
    std::queue<int> people;
    for (int i = 1; i <= n; ++i) {
        people.push(i);
    }

    std::ostringstream result;
    result << "<";

    bool first = true;
    while (!people.empty()) {
        // Rotate the first k-1 people to the back.
        for (int step = 1; step < k; ++step) {
            int front = people.front();
            people.pop();
            people.push(front);
        }

        // The current front is eliminated.
        int removed = people.front();
        people.pop();

        if (!first) {
            result << ", ";
        }
        result << removed;
        first = false;
    }

    result << ">";
    return result.str();
}

#include <cassert>

int main() {
    // Basic case from the original snippet: n=7, k=3
    assert(josephusPermutation(7, 3) == "<3, 6, 2, 7, 5, 1, 4>");

    // n=1 always outputs single element
    assert(josephusPermutation(1, 5) == "<1>");

    // k=1 removes in natural order
    assert(josephusPermutation(5, 1) == "<1, 2, 3, 4, 5>");

    // k=2 (classic pair elimination)
    assert(josephusPermutation(6, 2) == "<2, 4, 6, 3, 1, 5>");

    // k larger than n (wraps around)
    assert(josephusPermutation(3, 10) == "<3, 2, 1>");

    // n=2, k=3 (wrap multiple times)
    assert(josephusPermutation(2, 3) == "<2, 1>");

    // n=4, k=2
    assert(josephusPermutation(4, 2) == "<2, 4, 3, 1>");

    // n=10, k=3 – known sequence
    assert(josephusPermutation(10, 3) == "<3, 6, 9, 2, 7, 1, 8, 5, 10, 4>");

    // n=8, k=4
    assert(josephusPermutation(8, 4) == "<4, 8, 5, 2, 1, 3, 7, 6>");

    return 0;
}
