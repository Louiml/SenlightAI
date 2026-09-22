Create a C++ class named `ResultVoter` that maintains a fixed-size collection of up to `MAX_RESULTS = 5` candidate solutions, where each solution is represented as a struct containing a `std::vector<int>` payload and an `unsigned int vote`. The class must provide a method `voteAndGetBest(std::vector<int>& candidate)` that: (1) increments an internal counter each time it is called; (2) if the counter exceeds `MAX_CALLS = 1000`, scales down all existing votes by dividing by 10 (integer division, so votes may become zero) but does not reset them fully; (3) searches the collection for an exact match of the candidate payload. If found, increments that entry's vote by 1; if not found, replaces the entry with the lowest vote (ties broken by the first encountered) with the new candidate and sets its vote to 1; (4) after that, finds the entry with the highest vote (ties broken by the first encountered) and copies that entry's payload into the input `candidate` parameter (overwriting it) so the caller receives the most "popular" solution seen so far. The collection should be initialized with all-zero votes and empty payloads. The class must have a `reset()` public method that performs the same vote scaling as step (2) and sets the counter to zero. All methods should be `const`-correct where appropriate. Provide the class definition and implementation in a single `.hpp` file, without a `main` function.

The core algorithm mimics the given snippet but replaces the fixed-size raw byte buffer with a `std::vector<int>` for payload comparison. The collection is an array of 5 structs. On each call to `voteAndGetBest`, we first increment `m_counter` and check if it exceeds `MAX_CALLS`; if so, we call `scaleVotes()` which loops over all entries and divides each vote by 10 (integer division). Then we search linearly for an exact match of `candidate` using `std::equal` on the vectors. If found, we increment that vote. If not found, we track the minimum vote and its index during the same loop; after the loop, if not found, we replace the min-vote entry (first occurrence if ties) with the new candidate and set vote=1. Finally, we scan again to find the maximum vote entry (first on ties) and assign its payload to `candidate`. Edge cases: When the collection is empty (all payloads empty), a new candidate will be inserted into the first slot (min vote=0 tie). When votes are scaled down to zero, ties are broken by index order, which is deterministic. Complexity: Each call is O(S * L), where S = 5 and L is the average vector length; space is O(S * L) for storage. The constant sizes make it effectively O(L) per call. The `reset()` method is O(S). Note that `std::vector<int>` comparison via `==` works, but we use `std::equal` to be explicit.

#ifndef RESULT_VOTER_HPP
#define RESULT_VOTER_HPP

#include <vector>
#include <algorithm>
#include <cstddef>

class ResultVoter {
public:
    static constexpr size_t MAX_RESULTS = 5;
    static constexpr unsigned int MAX_CALLS = 1000;

    ResultVoter() : m_counter(0) {
        for (size_t i = 0; i < MAX_RESULTS; ++i) {
            m_solutions[i].payload.clear();
            m_solutions[i].vote = 0;
        }
    }

    // Scale down votes by dividing by 10 and reset counter to zero.
    void reset() {
        m_counter = 0;
        for (size_t i = 0; i < MAX_RESULTS; ++i) {
            m_solutions[i].vote /= 10;
        }
    }

    // Vote for the given candidate, then return the best candidate via the parameter.
    void voteAndGetBest(std::vector<int>& candidate) {
        m_counter++;
        if (m_counter > MAX_CALLS) {
            reset();
        }

        size_t minIndex = 0;
        unsigned int minVote = static_cast<unsigned int>(-1); // max
        bool found = false;

        // Search for existing match and track min vote.
        for (size_t i = 0; i < MAX_RESULTS; ++i) {
            if (m_solutions[i].payload.size() == candidate.size() &&
                std::equal(m_solutions[i].payload.begin(), m_solutions[i].payload.end(),
                           candidate.begin())) {
                m_solutions[i].vote += 1;
                found = true;
                break;
            }
            if (m_solutions[i].vote < minVote) {
                minVote = m_solutions[i].vote;
                minIndex = i;
            }
        }

        // If not found, replace the worst solution (lowest vote, first on tie).
        if (!found) {
            m_solutions[minIndex].payload = candidate;
            m_solutions[minIndex].vote = 1;
        }

        // Find best solution (highest vote, first on tie).
        size_t maxIndex = 0;
        unsigned int maxVote = 0;
        for (size_t i = 0; i < MAX_RESULTS; ++i) {
            if (m_solutions[i].vote > maxVote) {
                maxVote = m_solutions[i].vote;
                maxIndex = i;
            }
        }

        // Overwrite candidate with the best solution's payload.
        candidate = m_solutions[maxIndex].payload;
    }

private:
    struct Entry {
        std::vector<int> payload;
        unsigned int vote;
    };

    Entry m_solutions[MAX_RESULTS];
    unsigned int m_counter;
};

#endif // RESULT_VOTER_HPP

#include <cassert>
#include <vector>
#include "ResultVoter.hpp"

int main() {
    ResultVoter voter;

    // Initially empty, first call inserts and returns the same candidate.
    std::vector<int> c1 = {1, 2, 3};
    voter.voteAndGetBest(c1);
    assert(c1 == std::vector<int>({1, 2, 3}));

    // A different candidate replaces the zero-vote entry, but since it's new, it gets vote=1.
    std::vector<int> c2 = {4, 5};
    voter.voteAndGetBest(c2);
    assert(c2 == std::vector<int>({4, 5}));

    // Repeat candidate 1 twice, it should become best (vote=3).
    std::vector<int> c3 = {1, 2, 3};
    voter.voteAndGetBest(c3);
    assert(c3 == std::vector<int>({1, 2, 3}));
    voter.voteAndGetBest(c3);
    assert(c3 == std::vector<int>({1, 2, 3}));

    // Now vote for a new candidate, it should not become best because existing {1,2,3} has higher vote.
    std::vector<int> c4 = {9, 9};
    voter.voteAndGetBest(c4);
    assert(c4 == std::vector<int>({1, 2, 3}));

    // Test reset: votes for {1,2,3} become 0 (3/10=0), others become 0 too.
    voter.reset();
    // Now a new candidate should win because all votes are zero, tie broken by first entry.
    std::vector<int> c5 = {7, 7, 7};
    voter.voteAndGetBest(c5);
    assert(c5 == std::vector<int>({7, 7, 7}));

    // After reset, counter is zero; call many times to trigger auto reset.
    ResultVoter voter2;
    std::vector<int> base = {0};
    for (unsigned int i = 0; i < 1500; ++i) {
        std::vector<int> v = {static_cast<int>(i % 3)};
        voter2.voteAndGetBest(v);
    }
    // After many votes, the most frequent value should be 0,1, or 2, but due to scaling it's deterministic.
    // We just check that the returned vector has size 1 and value is one of 0,1,2.
    std::vector<int> result = {0};
    voter2.voteAndGetBest(result);
    assert(result.size() == 1);
    assert(result[0] == 0 || result[0] == 1 || result[0] == 2);

    return 0;
}
