// Write a C++ function named `countSolubleProblems` that takes two parameters: a vector of positive integers representing the difficulty of problems arranged in a row, and a positive integer `k` representing the maximum difficulty a problem can have and still be solvable. The function must return the total number of problems that can be solved if you start from the leftmost problem and solve consecutive problems as long as each problem's difficulty is ≤ `k` until you hit a problem with difficulty > `k` (at which point you stop from the left), and then also start from the rightmost problem and solve consecutive problems as long as each problem's difficulty is ≤ `k` until you hit a problem with difficulty > `k` from the right. However, if the same problem is approached from both sides (i.e., the left and right sweeps overlap), you must not double-count it. In other words, you count the problems solved from the left prefix up to but not including the first too-hard problem, and the problems solved from the right suffix up to but not including the first too-hard problem from the right, but if these two sets have any intersection, the intersection should be counted only once. The function should return the total number of distinct solvable problems.
// The core idea is to simulate the problem-solving process from both ends independently. Start by iterating from index 0 and count consecutive problems whose difficulty is ≤ `k`, stopping immediately at the first difficulty > `k`. Record the last index processed from the left (call it `left_end`). Then iterate from the end of the vector backward, starting at index `size-1` and moving down, counting consecutive problems whose difficulty ≤ `k`, stopping at the first difficulty > `k`, and also stop if the backward index reaches `left_end` (inclusive) to avoid double-counting the intersection. In fact, the simplest correct approach is: first compute `left_count` by scanning forward until a problem > `k` or the end. Then compute `right_count` by scanning backward from the last index, but only while the current index is strictly greater than `left_end` (the last index already counted from the left). Note that if the entire array is ≤ `k`, then `left_count = n` and `right_count` will be 0 because the backward loop starts at `n-1` which is not > `left_end` (since `left_end = n-1`), so total = `n`. Edge cases: an empty vector returns 0; if the first problem is > `k`, then `left_count = 0` and we still check from the right, but if the rightmost is also > `k`, total is 0; if `k` is large enough that all problems are solvable, the answer is the length of the vector. Time complexity is O(n) because we scan the array at most twice, and space complexity is O(1) beyond the input vector.
#include <vector>

int countSolubleProblems(const std::vector<int>& problems, int k) {
    int n = static_cast<int>(problems.size());
    if (n == 0) return 0;

    int left_count = 0;
    int left_end = -1; // last index counted from the left

    for (int i = 0; i < n; ++i) {
        if (problems[i] > k) break;
        ++left_count;
        left_end = i;
    }

    int right_count = 0;
    for (int i = n - 1; i > left_end; --i) {
        if (problems[i] > k) break;
        ++right_count;
    }

    return left_count + right_count;
}
#include <cassert>
#include <vector>

int countSolubleProblems(const std::vector<int>& problems, int k);

int main() {
    // All problems are too hard from the left, but some from the right
    assert(countSolubleProblems({5, 6, 7}, 5) == 1); // left stops at index0, right counts 0? Actually right starts at 7>5 so 0, total 1
    assert(countSolubleProblems({5, 6, 7}, 6) == 2); // left counts 0 and then stops at 6? 5<=6, 6<=6, 7>6 -> left_count=2, right sees 7>6 -> 0, total 2
    assert(countSolubleProblems({1, 2, 3, 4, 5}, 3) == 3); // left counts 1,2,3 then stops -> 3, right counts 5? no 5>3 so 0, total 3
    assert(countSolubleProblems({3, 2, 1}, 3) == 3); // all <=3, left counts all 3, right loop starts at index2 <= left_end? left_end=2, so right_count=0, total 3
    assert(countSolubleProblems({4, 1, 2, 3}, 3) == 3); // left stops immediately (4>3) left_count=0 left_end=-1, right counts 3,2,1 (all <=3) -> 3, total 3
    assert(countSolubleProblems({2, 2, 2}, 2) == 3);
    assert(countSolubleProblems({1, 5, 1}, 1) == 2); // left counts index0 only, right counts index2 only, no overlap, total 2
    assert(countSolubleProblems({9, 9, 9}, 1) == 0);
    assert(countSolubleProblems({}, 5) == 0);
    assert(countSolubleProblems({1}, 1) == 1);
    return 0;
}
