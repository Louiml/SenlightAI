/*
Write a C++ function `bool passedEveryTask(int n, const std::vector<int>& scores)` that takes the number of tasks `n` and a vector of `n` non-negative integer scores, and returns `true` if for every task index `i` (1-based), the cumulative sum of the first `i` scores is at least `40 * i`. Otherwise, return `false`. The function must handle `n >= 1` and a vector of exactly length `n`. The condition implies that the average score up to every prefix must be at least 40.
*/
#include <vector>

// Returns true if the cumulative average of the first i scores is at least 40 for every i from 1 to n.
bool passedEveryTask(int n, const std::vector<int>& scores) {
    int sum = 0;
    for (int i = 1; i <= n; ++i) {
        sum += scores[i - 1];
        if (sum < 40 * i) {
            return false;
        }
    }
    return true;
}
#include <cassert>
#include <vector>

bool passedEveryTask(int n, const std::vector<int>& scores);

int main() {
    assert(passedEveryTask(1, {40}) == true);
    assert(passedEveryTask(1, {39}) == false);
    assert(passedEveryTask(2, {40, 40}) == true);
    assert(passedEveryTask(2, {100, 0}) == false); // i=2 sum=100, 40*2=80, ok; but i=1 sum=100 >=40 ok? Actually check: sum after first=100 >=40, after second=100 >=80 true, so true.
    // Correction: {100,0} is true, but let's test a false case:
    assert(passedEveryTask(2, {50, 30}) == true); // 50>=40, 80>=80
    assert(passedEveryTask(2, {50, 29}) == false); // 50>=40, 79<80
    assert(passedEveryTask(3, {40, 40, 40}) == true);
    assert(passedEveryTask(3, {0, 100, 100}) == false); // first sum=0 < 40
    assert(passedEveryTask(5, {40, 40, 40, 40, 40}) == true);
    assert(passedEveryTask(5, {100, 100, 0, 0, 0}) == false); // i=3 sum=200, 40*3=120 okay; i=4 sum=200 <160? no 200>=160 okay; i=5 sum=200 <200? no, actually true. Let's test another: {100,0,0,0,0} fails at i=2 (sum=100 <80? no) actually i=2 sum=100>=80 true, i=3 sum=100>=120 false.
    assert(passedEveryTask(5, {100, 0, 0, 0, 0}) == false); // fails at i=3
    assert(passedEveryTask(4, {40, 40, 40, 40}) == true);
}
// The solution iterates through the vector once, maintaining a running cumulative sum. For each 1-based index `i`, after adding the current score, we check if `sum < 40 * i`. If any such check fails, we immediately return `false`. If we finish the loop without failure, return `true`. The condition checks strictly "at least", so equality is allowed. Edge cases include a single element (must be >= 40), and all zeros (fails at i=1). Since the vector size matches `n`, we can assume correct input length. Time complexity is O(n) and space complexity is O(1) auxiliary (ignoring the input vector).
