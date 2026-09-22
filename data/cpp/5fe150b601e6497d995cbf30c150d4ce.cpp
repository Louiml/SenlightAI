You are managing a queue of patients at a clinic using three types of operations, given as integers in a query stream. Patients are assigned consecutive integer IDs starting from 1 in the order they arrive. There are `q` operations total, each starting with an integer `a`:
- If `a == 1`, a new patient arrives; assign the next unused ID (starting from 1) and add them to the back of the queue. No extra input follows.
- If `a == 2`, the next integer `b` is read; this patient `b` has been called and checked in, so they should be marked as "come" (they no longer need to be the front target). However, they may remain physically in the queue until they reach the front.
- If `a == 3`, you must output the ID of the earliest-arrived patient who has **not** been marked as "come". In other words, repeatedly remove patients from the front of the queue as long as they have already been marked "come", then print the ID of the new front (if the queue is empty after removal, you may assume it never becomes empty before output). Write a C++ function `int solveClinic(int q, std::istream& input)` that reads all queries from a given input stream (already positioned at the start) and returns a `std::vector<int>` of all answers produced by type‑3 operations, in order. The first line of input will contain `n` (unused, but present for historical reasons) followed by `q`; you can ignore `n` but must read it. The maximum possible patient ID is at most 500,000, and the queue is guaranteed to have at least one unmarked patient whenever a type‑3 query occurs.

#include <bits/stdc++.h>
#include <cassert>

// The function definition is pasted here (or included from the solution above).
std::vector<int> solveClinic(int q, std::istream& input);

int main() {
    // Test 1: Basic sequence with type‑3 after a couple of arrivals
    {
        std::istringstream iss("5 5\n1 1 3 1 3\n");
        auto ans = solveClinic(5, iss);
        assert(ans == (std::vector<int>{1, 1}));
    }
    // Test 2: Marking patient as come, then type‑3 skips them
    {
        std::istringstream iss("4 6\n1 1 1 2 1 3\n");
        auto ans = solveClinic(6, iss);
        assert(ans == (std::vector<int>{2}));
    }
    // Test 3: Multiple skips after marking several patients
    {
        std::istringstream iss("6 8\n1 1 1 2 2 1 1 3\n");
        auto ans = solveClinic(8, iss);
        assert(ans == (std::vector<int>{4}));
    }
    // Test 4: Mark the first, then ask, then ask again without new arrivals
    {
        std::istringstream iss("4 5\n1 2 1 3 3\n");
        auto ans = solveClinic(5, iss);
        assert(ans == (std::vector<int>{2, 2}));
    }
    // Test 5: No type‑3 queries
    {
        std::istringstream iss("3 3\n1 1 2 1\n");
        auto ans = solveClinic(3, iss);
        assert(ans.empty());
    }
    // Test 6: Mark all current patients, then add one and ask
    {
        std::istringstream iss("5 7\n1 1 2 2 1 2 3\n");
        auto ans = solveClinic(7, iss);
        assert(ans == (std::vector<int>{4}));
    }
    // Test 7: Very large sequence (but small test) to verify consistency
    {
        std::istringstream iss("6 9\n1 1 1 2 3 1 3 2 3\n");
        auto ans = solveClinic(9, iss);
        assert(ans == (std::vector<int>{2, 4, 4}));
    }
    return 0;
}

#include <bits/stdc++.h>

// Reads q operations from the given input stream (first two ints are n and q, ignore n).
// Returns a vector of answers for all type‑3 queries.
std::vector<int> solveClinic(int q, std::istream& input) {
    std::queue<int> callQueue;
    std::bitset<500001> come;  // come[i] == true means patient i has been marked as "come"
    int nextID = 1;
    std::vector<int> answers;

    for (int i = 0; i < q; ++i) {
        int a;
        input >> a;
        if (a == 1) {
            callQueue.push(nextID);
            ++nextID;
        } else if (a == 2) {
            int b;
            input >> b;
            come[b] = true;
        } else {  // a == 3
            while (!callQueue.empty() && come[callQueue.front()]) {
                callQueue.pop();
            }
            answers.push_back(callQueue.front());
        }
    }
    return answers;
}

// The solution uses a queue of integers for patient IDs and a bitset (or boolean array) to track who has been marked "come". We maintain a counter `nextID` that starts at 1 and increments each time a type‑1 query arrives. For type‑1, we push `nextID` into the queue and increment it. For type‑2, we set `come[b] = true`. For type‑3, we repeatedly pop from the front while the front patient is marked "come" (i.e., `come[front] == true`). After clearing those, the front is the first unmarked patient; we record its ID into the answer vector. The while loop in type‑3 is total across all queries because each patient is popped at most once. Hence the algorithm runs in `O(q)` time overall, with `O(maxID)` space for the bitset (here max 500,001). Edge cases: a type‑3 might immediately pop many already‑marked patients; the queue is guaranteed non‑empty after removal. We ignore `n` because it is not used in the logic but must be consumed from input.
