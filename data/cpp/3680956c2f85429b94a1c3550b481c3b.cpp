// You are simulating a dynamic linked-list-like sequence of `len` positions numbered from 1 to `len`. Initially the sequence has length `n`, and a "current position" is at index `k` (1-based). You are given `m` and `t` as fixed parameters (read from input but not directly used in the logic), and then `t` operations. Each operation is given by two integers `a` and `b`. If `a == 0`, it is a "cut" operation: remove the first `b` positions (respectively remove the last `len - b` positions if `b` is not less than the current position). Specifically, if `b < k`, then the cut happens before the current position, so both the length and current position decrease by `b`; otherwise the cut happens after (or at) the current position, so only the length is reduced to `b` (the current position remains unchanged). If `a == 1`, it is an "insert" operation: add a new position at index `b` (1-based). If `b <= current position`, the current position shifts right by 1 (increments) and the length increases by 1; otherwise only the length increases by 1 (current position unchanged). After each operation, output the new `len` and current position separated by a space. Write a standalone C++ function `simulateSequence(int initial_length, int initial_position, const vector<pair<int,int>>& operations)` that returns a vector of strings (one per operation) representing the `len` and `current position` after each operation, in order. The function must handle up to `10^5` operations and initial length up to `10^9`, so use only integer arithmetic without simulating the actual list.
// The key insight is that we never need to store the actual sequence; we only need to maintain two integers: `len` (current length) and `pos` (current position, 1-based). For each operation:  
// - If `a == 0`: a "cut" at position `b` (0 means remove first `b` elements, so `b` is the number of leading elements to remove; the original code interprets `b` as the new length if `b >= pos`, or the number removed if `b < pos`). Precisely, if `b < pos`, then both `pos` and `len` reduce by `b`. If `b >= pos`, then `len` becomes `b` and `pos` stays the same (since the cut is after the current position). Note: `b` is a 0-based index in the original problem? In the given snippet, the input `b` is used as follows: if `a==0` and `b < k`, then `k -= b; len -= b;` else `len = b;`. That means `b` is interpreted as "remove first `b` elements" when `b < k`, or "set the length to b" when `b >= k`. The problem statement says "cut" at position `b` (1-based?) but the snippet treats `b` as a length (0-based? Actually the input in the original problem from Codeforces likely has `b` as a 1-based index, but the snippet uses `b` as the new length directly when `b >= k`, and as the number of leading elements to remove otherwise. For our task we replicate exactly the logic from the snippet: given `a` and `b`, if `a==0` and `b < pos`, we subtract `b` from both; if `a==0` and `b >= pos`, we set `len = b`. If `a==1`, we increment `len` by 1 always, and if `b <= pos` we increment `pos` by 1.  
// - The operation `a` is either 0 or 1.  
// Edge cases: `b` may be 0 for `a==0`; then `b < pos` is always true (since pos >=1), so we subtract 0 from both, which is a no-op. For `a==1` and `b` can be any integer from 1 to `len` (inclusive) presumably; if `b` is larger than `pos` we do the else branch. The original code does not validate `b`'s range, but we assume valid input.  
// Time complexity: O(t) for t operations. Space complexity: O(t) for the output vector, O(1) auxiliary.  
// The function must be pure, taking the initial length and position and the list of operations.
#include <vector>
#include <string>
#include <utility>

// Simulates a dynamic sequence with cut and insert operations.
// Returns a vector of strings, each representing "len pos" after each operation.
std::vector<std::string> simulateSequence(
    int initial_length,
    int initial_position,
    const std::vector<std::pair<int, int>>& operations
) {
    int len = initial_length;
    int pos = initial_position;
    std::vector<std::string> results;
    results.reserve(operations.size());

    for (const auto& op : operations) {
        int a = op.first;
        int b = op.second;
        if (a == 0) {
            if (b < pos) {
                pos -= b;
                len -= b;
            } else {
                len = b;
            }
        } else { // a == 1
            if (b <= pos) {
                pos++;
                len++;
            } else {
                len++;
            }
        }
        results.push_back(std::to_string(len) + " " + std::to_string(pos));
    }
    return results;
}
#include <vector>
#include <string>
#include <cassert>
#include <iostream>

// Declaration of the solution function (provided in the solution section).
std::vector<std::string> simulateSequence(
    int initial_length,
    int initial_position,
    const std::vector<std::pair<int, int>>& operations
);

int main() {
    // Test 1: from the original snippet example (not given, but simple)
    {
        std::vector<std::pair<int,int>> ops = {{0,1}};
        auto res = simulateSequence(5, 3, ops);
        assert(res.size() == 1);
        assert(res[0] == "4 2");
    }
    // Test 2: cut at position after current position
    {
        std::vector<std::pair<int,int>> ops = {{0,4}};
        auto res = simulateSequence(5, 3, ops);
        assert(res[0] == "4 3");
    }
    // Test 3: insert before current position
    {
        std::vector<std::pair<int,int>> ops = {{1,1}};
        auto res = simulateSequence(5, 3, ops);
        assert(res[0] == "6 4");
    }
    // Test 4: insert after current position
    {
        std::vector<std::pair<int,int>> ops = {{1,4}};
        auto res = simulateSequence(5, 3, ops);
        assert(res[0] == "6 3");
    }
    // Test 5: cut at position equal to current position (b == pos)
    {
        std::vector<std::pair<int,int>> ops = {{0,3}};
        auto res = simulateSequence(5, 3, ops);
        assert(res[0] == "3 3");
    }
    // Test 6: multiple operations
    {
        std::vector<std::pair<int,int>> ops = {{1,2}, {0,1}, {0,4}};
        auto res = simulateSequence(5, 3, ops);
        // After 1,2: len=6, pos=4 (since 2<=3)
        // After 0,1: b=1 < pos=4 -> len=5, pos=3
        // After 0,4: b=4 >= pos=3 -> len=4, pos=3
        assert(res[0] == "6 4");
        assert(res[1] == "5 3");
        assert(res[2] == "4 3");
    }
    // Test 7: initial position at 1, cut before it
    {
        std::vector<std::pair<int,int>> ops = {{0,0}};
        auto res = simulateSequence(10, 1, ops);
        assert(res[0] == "10 1");
    }
    // Test 8: large values
    {
        std::vector<std::pair<int,int>> ops = {{1,1000000000}, {0,1}};
        auto res = simulateSequence(1000000000, 1000000000, ops);
        // After insert at b=1e9 (<= pos=1e9) -> len=1000000001, pos=1000000001
        // After cut b=1 (which is < pos) -> len=1000000000, pos=1000000000
        assert(res[0] == "1000000001 1000000001");
        assert(res[1] == "1000000000 1000000000");
    }
    // Test 9: no operations
    {
        auto res = simulateSequence(5, 2, {});
        assert(res.empty());
    }
    // Test 10: edge case where b is large and a==0 sets len directly
    {
        std::vector<std::pair<int,int>> ops = {{0,100}};
        auto res = simulateSequence(50, 20, ops);
        assert(res[0] == "100 20");
    }
    std::cout << "All tests passed.\n";
    return 0;
}
