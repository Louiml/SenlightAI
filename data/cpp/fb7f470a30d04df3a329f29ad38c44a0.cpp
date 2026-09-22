/*
Write a C++ function named `minOperationsToMoveBalls` that takes a binary string `boxes` (containing only '0' and '1' characters, where '1' represents a box containing a ball and '0' represents an empty box). The function should return a vector of integers where the i-th element represents the minimum total number of operations (each operation is moving one ball from its current box to an adjacent box, costing 1 operation per adjacent move) required to move all balls to box `i`. The string can have length from 1 to 1000. Assume the input is always valid (non-empty, only '0'/'1'). For example, if `boxes = "110"`, the output should be `[1, 1, 3]`: for box 0, move the ball from box 1 to box 0 (1 operation), for box 1, the ball already there and the other from box 0 (1 operation), for box 2, move two balls from boxes 0 and 1 (1+2=3 operations). The function must be efficient even for the maximum length.
*/
#include <vector>
#include <string>

// Given a binary string 'boxes', return a vector where ans[i] is the
// minimum operations to gather all balls to box i.
std::vector<int> minOperationsToMoveBalls(const std::string& boxes) {
    int n = static_cast<int>(boxes.size());
    std::vector<int> result(n, 0);

    // Left-to-right pass: accumulate operations from left side.
    int prefixBallCount = 0;
    int leftOperations = 0;
    for (int i = 0; i < n; ++i) {
        result[i] += leftOperations;
        prefixBallCount += (boxes[i] - '0');
        leftOperations += prefixBallCount;
    }

    // Right-to-left pass: accumulate operations from right side.
    int suffixBallCount = 0;
    int rightOperations = 0;
    for (int i = n - 1; i >= 0; --i) {
        result[i] += rightOperations;
        suffixBallCount += (boxes[i] - '0');
        rightOperations += suffixBallCount;
    }

    return result;
}
#include <cassert>
#include <vector>
#include <string>

// Include the solution function here or via header.

int main() {
    // Example from task
    std::vector<int> res1 = minOperationsToMoveBalls("110");
    assert(res1 == std::vector<int>({1, 1, 3}));

    // Single box
    assert(minOperationsToMoveBalls("0") == std::vector<int>({0}));
    assert(minOperationsToMoveBalls("1") == std::vector<int>({0}));

    // No balls
    assert(minOperationsToMoveBalls("000") == std::vector<int>({0, 0, 0}));

    // All balls
    assert(minOperationsToMoveBalls("111") == std::vector<int>({3, 2, 3}));

    // Longer alternating string
    // "101": for box 0, move ball from box 2 (cost 2) -> 2; for box 1, move from box 0 (1) and box 2 (1) -> 2; for box 2, move from box 0 (2) -> 2
    assert(minOperationsToMoveBalls("101") == std::vector<int>({2, 2, 2}));

    // Another case: "001" -> box0 costs 2 (ball from index2), box1 costs 1, box2 costs 0
    assert(minOperationsToMoveBalls("001") == std::vector<int>({2, 1, 0}));

    // Reverse "100" -> box0 costs 0, box1 costs 1, box2 costs 2
    assert(minOperationsToMoveBalls("100") == std::vector<int>({0, 1, 2}));

    // Mixed: "010" -> box0 costs 1, box1 costs 0, box2 costs 1
    assert(minOperationsToMoveBalls("010") == std::vector<int>({1, 0, 1}));

    // Two balls at ends: "10001" -> let's compute: box0: move from 4 (4 ops) -> 4; box1: from 0 (1) + from4 (3)=4; box2: from0(2)+from4(2)=4; box3: from0(3)+from4(1)=4; box4: from0(4)=4
    assert(minOperationsToMoveBalls("10001") == std::vector<int>({4, 4, 4, 4, 4}));

    return 0;
}
// The solution uses two passes to compute the total operations for each position without nested loops. First, a left-to-right pass calculates the cumulative number of operations needed to bring all balls from the left side (including the current box) to that position. For each index `i`, we maintain `leftPrefixSum` (number of balls seen so far to the left including current) and `leftOps` (total operations to move those balls to the current position). At each step, we add the current `leftOps` to the answer for index `i`, then update `leftPrefixSum` by adding whether the current box has a ball (`boxes[i] - '0'`), and then increment `leftOps` by `leftPrefixSum` (because moving all previously seen balls one step to the right takes one operation per ball). The right-to-left pass works symmetrically: it computes the operations to bring balls from the right side to each position and adds to the answer. Edge cases include strings of length 1 (returns `[0]`), strings with no balls (returns all zeros), and strings with all balls. The time complexity is O(n) and space complexity is O(n) for the answer vector (the algorithm itself uses O(1) extra space). The approach avoids O(n²) brute force.
