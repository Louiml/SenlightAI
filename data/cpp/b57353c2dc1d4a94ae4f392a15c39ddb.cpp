Write a C++ function `longestCollatzSequence(int limit)` that computes the starting number under a given positive integer `limit` which produces the longest Collatz sequence (using the standard rule: if current term is even, divide by 2; if odd, multiply by 3 and add 1; stop when the term reaches 1). If multiple starting numbers produce sequences of the same maximum length, return the smallest starting number. The function should return the starting integer itself (not the sequence length). The input `limit` is guaranteed to be at least 1, but the starting number must be in the range [1, limit). Handle edge cases like `limit = 2` (only candidate is 1) or `limit` being large (up to 1,000,000) efficiently using memoization to avoid recomputing lengths for already-seen terms. Do not use global variables; pass all needed state through the function's parameters.

// The Collatz sequence for a starting number `n` follows: if `n` is even, next term is `n/2`; if odd, next term is `3n+1`; the sequence ends when the term becomes 1. Naively computing the length for each starting number from 1 to `limit-1` can be extremely inefficient because many sequences share intermediate terms (for example, all sequences eventually go through some common terms). We can use a memoization map (e.g., `std::unordered_map<long long, int>`) that stores, for each seen term, the number of steps needed to reach 1 from that term. When computing the length for a starting number `i`, we walk through the sequence, recording the path temporarily, and as soon as we hit a term that is already in the memo, we add its stored length to the length of the current path and then populate the memo for all previous terms on the path with the correct total steps. This reduces the average complexity to roughly O(n) per test overall, though each term may be processed a constant number of times. Important edge cases: `limit=1` means no valid starting number (since we need at least one), but the problem statement says limit is at least 1; if limit <= 2, the only starting number is 1 (since starting numbers are less than limit). Also, numbers can grow beyond 32-bit during the sequence (e.g., 27 reaches 9232), so we must use a 64-bit integer for the current term while iterating. The memo map should use `long long` as key. Time complexity: O(limit) average, but with memoization, each distinct term in any sequence up to the given limit is computed once, so O(limit) or slightly more, but practically linear. Space complexity: O(k) where k is the number of distinct terms encountered, which is on the order of the maximum sequence length times the number of starts, but bounded by a few times `limit` in practice.

#include <bits/stdc++.h>

// Compute the starting number less than `limit` that produces the longest Collatz sequence.
// If multiple starting numbers have the same maximum length, return the smallest one.
// `limit` must be at least 2 (since starting numbers are in [1, limit-1]).
int longestCollatzSequence(int limit) {
    if (limit <= 2) {
        return 1;
    }

    // Memoization: maps a term to the number of steps to reach 1 from that term.
    std::unordered_map<long long, int> memo;
    memo[1] = 0; // 1 requires 0 steps to reach 1

    int bestStart = 1;
    int bestLength = 0;

    for (int i = 1; i < limit; ++i) {
        long long current = i;
        int steps = 0;
        // Keep track of the path we traverse so we can later populate memo.
        std::vector<long long> path;
        path.reserve(100);

        // Walk the sequence until we hit a known term.
        while (memo.find(current) == memo.end()) {
            path.push_back(current);
            if (current % 2 == 0) {
                current /= 2;
            } else {
                current = 3 * current + 1;
            }
            ++steps;
        }

        // Now `current` is in memo, we know its length.
        int totalSteps = steps + memo[current];

        // Populate memo for every term on the path.
        // The last term on the path (path.back()) has length totalSteps,
        // the one before has totalSteps - 1, etc.
        for (size_t j = 0; j < path.size(); ++j) {
            // The number of steps from path[j] to 1 is (totalSteps - j).
            memo[path[j]] = totalSteps - static_cast<int>(j);
        }

        if (totalSteps > bestLength) {
            bestLength = totalSteps;
            bestStart = i;
        }
    }

    return bestStart;
}

#include <bits/stdc++.h>
using namespace std;

// Declaration of the tested function (assumed to be provided in the solution).
int longestCollatzSequence(int limit);

int main() {
    // limit = 2: only candidate is 1, length 0.
    assert(longestCollatzSequence(2) == 1);

    // limit = 3: candidates 1 (0 steps) and 2 (1 step), so 2 wins.
    assert(longestCollatzSequence(3) == 2);

    // limit = 10: known that 9 has length 19 (longest under 10).
    assert(longestCollatzSequence(10) == 9);

    // limit = 14: 9 (length 19) vs 13 (length 9), so 9.
    assert(longestCollatzSequence(14) == 9);

    // limit = 20: 18 has length 20, but 19 has length 20 too, smallest is 18.
    assert(longestCollatzSequence(20) == 18);

    // limit = 100: known answer is 97.
    assert(longestCollatzSequence(100) == 97);

    // limit = 1000: known answer is 871.
    assert(longestCollatzSequence(1000) == 871);

    // limit = 100000: known answer is 77031.
    assert(longestCollatzSequence(100000) == 77031);

    // Edge: limit = 1? Not allowed per spec, but function would return 1 (no loop).
    assert(longestCollatzSequence(1) == 1);

    cout << "All tests passed!" << endl;
    return 0;
}
