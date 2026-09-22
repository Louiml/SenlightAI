// Write a C++ function `int solveCoconuts(int n)` that, given a positive integer `n` representing the total number of coconuts collected by a group of sailors and one monkey on an island, determines the maximum number of sailors (people) that could have been part of the classic "coconut and monkey" puzzle. In the puzzle, the sailors divide the coconuts into equal piles each night, but there is always one leftover coconut given to the monkey, and each sailor hides their share before the final morning division (which leaves no remainder for the monkey). The function must return the largest possible number of sailors such that the process works exactly, or `-1` if no such number exists for that `n`. The input `n` can be up to large values (e.g., up to 10^9), and the function should handle `n` as an `int`. The known solution range for the number of sailors is between 2 and 1000 inclusive. The function should implement an efficient search from the largest possible number of sailors down to the smallest, verifying the constraints for each candidate.

#include <cassert>

int main() {
    // Known classic examples and edge cases.
    assert(solveCoconuts(25) == 3);      // 3 sailors, 25 coconuts
    assert(solveCoconuts(3121) == 5);    // 5 sailors, 3121 coconuts
    assert(solveCoconuts(3) == 2);       // 2 sailors, 3 coconuts
    assert(solveCoconuts(1) == -1);      // too few coconuts
    assert(solveCoconuts(2) == -1);      // no valid division
    assert(solveCoconuts(4) == -1);      // no valid division
    assert(solveCoconuts(7) == -1);      // no valid division
    assert(solveCoconuts(15) == -1);     // no valid division
    assert(solveCoconuts(79) == -1);     // no valid division
    assert(solveCoconuts(23328) == 7);   // 7 sailors, known from puzzle
    return 0;
}

#include <algorithm>

// Determine the largest number of sailors (2..1000) for which the coconut
// puzzle works with n coconuts. Returns -1 if no such number exists.
int solveCoconuts(int n) {
    // Search from the maximum possible sailors down to 2.
    for (int sailors = 1000; sailors >= 2; --sailors) {
        int remaining = n;
        bool valid = true;

        // Simulate each night's division.
        for (int night = 0; night < sailors; ++night) {
            // Need exactly one leftover for the monkey.
            if ((remaining - 1) % sailors != 0) {
                valid = false;
                break;
            }
            // Each sailor takes (remaining-1)/sailors, plus one for monkey.
            remaining = remaining - (remaining - 1) / sailors - 1;
        }

        // After all nights, the morning division must leave no remainder.
        if (valid && remaining % sailors == 0) {
            return sailors;
        }
    }
    return -1;
}

// The classic problem: For `i` sailors, each night, before dividing, there must be one coconut left for the monkey, so the current count `m` must satisfy `(m-1) % i == 0`. Then each sailor takes `(m-1)/i` coconuts, leaving `m - (m-1)/i - 1` for the next night. After `i` nights (each sailor hides a share), in the morning the remaining coconuts are divided equally among all `i` sailors with no remainder, i.e., `m % i == 0` at the end. We brute-force the number of sailors `i` from 1000 down to 2 (since the problem statement says search from 1000 down to 1, but 1 sailor trivially works and is not considered; 1000 is an upper bound). For each candidate `i`, simulate the process on a copy `m = n` for `i` iterations. If at any step `(m-1) % i != 0`, the candidate fails. If all steps pass and the final `m` is divisible by `i`, we found a solution; since we iterate from largest to smallest, we return the first (largest) valid `i`. If no candidate works, return `-1`. Edge cases: `n` may be small; the simulation uses integer arithmetic and repeated subtraction. Time complexity is O(1000 * i) worst-case per call, but since i ≤ 1000, overall O(1e6) operations per call, which is constant. Space complexity is O(1). The problem is well-known: for n=25, answer is 3; for n=3121, answer is 5; for n=1, no solution (since with at least 2 sailors, you need at least 1+2 = 3 coconuts? Actually check: n=1 fails because for i≥2, (1-1)%i=0 but after first night m becomes 0, then final m%i=0 but that would mean 0 coconuts divided among sailors, which is allowed? The original problem typically requires a positive share, but the code allows m=0? In the given snippet, if m=0, then (m-1)%i = -1%i != 0 for positive i, so it fails. So n=1 returns -1). For n=2: try i=2, (2-1)%2=1%2=1 !=0, fail; i down to 2 none, so -1. Test values: n=3: i=2, (3-1)%2=0, m=3 - (2/2) -1 = 3-1-1=1; after second night? Actually for i=2, we do j=0: m=3 → m-1=2, (2%2=0), m=3-(2/2)-1=3-1-1=1; j=1: (1-1)%2=0, m=1-(0/2)-1=0; after loop, m=0, 0%2=0 → valid, so answer 2. So n=3 answer 2. For n=4: i=2, (4-1)%2=1 fail; i=3? (4-1)%3=0, m=4-1-1=2; next (2-1)%3=1%3=1 fail; so -1. The reference solution directly mirrors the given snippet.
