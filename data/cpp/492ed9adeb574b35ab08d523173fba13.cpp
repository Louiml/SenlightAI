// Write a C++ function that simulates the probability distribution of a game score after a given number of rounds. Each round, the score changes by one of six possible values: +1 (probability 0.25), +2 (0.125), 0 (0.375), +3 (0.0625), -1 (0.125), or -3 (0.0625). The function should take an integer `rounds` (1 to 20) and return a `std::map<int, double>` where each key is the possible total score after that many rounds, and each value is the probability of that score. The function must handle the probabilities as exact as possible using `double` and should include all reachable scores after `rounds` rounds, starting from score 0 before any rounds. Note that scores can be negative, zero, or positive, and the sum of all probabilities in the returned map must be 1.0 (within floating-point tolerance).
#include <cassert>
#include <cmath>
#include <map>

// Declare the solution function here (or include the header).
std::map<int, double> scoreDistribution(int rounds);

int main() {
    // Round 0: only score 0 with probability 1.
    auto m0 = scoreDistribution(0);
    assert(m0.size() == 1);
    assert(std::abs(m0.at(0) - 1.0) < 1e-9);

    // Round 1: check each possible score and probability.
    auto m1 = scoreDistribution(1);
    assert(std::abs(m1.at(1) - 0.25) < 1e-9);
    assert(std::abs(m1.at(2) - 0.125) < 1e-9);
    assert(std::abs(m1.at(0) - 0.375) < 1e-9);
    assert(std::abs(m1.at(3) - 0.0625) < 1e-9);
    assert(std::abs(m1.at(-1) - 0.125) < 1e-9);
    assert(std::abs(m1.at(-3) - 0.0625) < 1e-9);
    // Sum of probabilities must be 1.
    double sum1 = 0.0;
    for (const auto& p : m1) sum1 += p.second;
    assert(std::abs(sum1 - 1.0) < 1e-9);

    // Round 2: check a few known combinations and sum.
    auto m2 = scoreDistribution(2);
    // Score 2: 1+1 or 2+0? Actually 1+1 (0.25*0.25=0.0625) plus 2+0 (0.125*0.375=0.046875) plus 0+2 (0.375*0.125=0.046875) -> total 0.15625
    assert(std::abs(m2.at(2) - 0.15625) < 1e-9);
    // Score 0: 0+0 (0.375^2=0.140625) plus 1+(-1) (0.25*0.125=0.03125) plus -1+1 (0.125*0.25=0.03125) plus 3+(-3) (0.0625*0.0625=0.00390625) plus -3+3 (0.0625*0.0625=0.00390625) -> total 0.2109375
    assert(std::abs(m2.at(0) - 0.2109375) < 1e-9);
    double sum2 = 0.0;
    for (const auto& p : m2) sum2 += p.second;
    assert(std::abs(sum2 - 1.0) < 1e-9);

    // Round 20: just check sum and that map is non-empty.
    auto m20 = scoreDistribution(20);
    assert(!m20.empty());
    double sum20 = 0.0;
    for (const auto& p : m20) sum20 += p.second;
    assert(std::abs(sum20 - 1.0) < 1e-8);

    // Ensure all probabilities are non-negative and not exceeding 1.
    for (const auto& p : m20) {
        assert(p.second >= -1e-9 && p.second <= 1.0 + 1e-9);
    }

    return 0;
}
#include <map>
#include <utility>

// Return a map from possible total score after `rounds` rounds to its probability.
std::map<int, double> scoreDistribution(int rounds) {
    // Six possible score changes with their probabilities.
    struct Change {
        int score;
        double prob;
    };
    const Change changes[] = {
        {1, 0.25},
        {2, 0.125},
        {0, 0.375},
        {3, 0.0625},
        {-1, 0.125},
        {-3, 0.0625}
    };

    // Start with score 0 with probability 1.
    std::map<int, double> current;
    current[0] = 1.0;

    for (int r = 0; r < rounds; ++r) {
        std::map<int, double> next;
        for (const auto& entry : current) {
            int oldScore = entry.first;
            double oldProb = entry.second;
            for (const auto& ch : changes) {
                int newScore = oldScore + ch.score;
                double newProb = oldProb * ch.prob;
                next[newScore] += newProb;
            }
        }
        current.swap(next);
    }

    return current;
}
// The solution uses dynamic programming over rounds. Maintain a `std::map<int, double>` that stores probabilities for each possible score at the current round. Initialize the map with `{0: 1.0}` before any rounds. For each round from 1 to `rounds`, create a new map. For each existing score and its probability, iterate over the six possible score changes, compute the new score and the new probability (current probability times the change’s probability), and add that probability to the new map at that new score. After processing all current states, swap the new map into the current map. This ensures all possible scores are accumulated. Edge cases: if `rounds` is 0, return the map with only `{0: 1.0}`. Floating-point summation order affects tiny precision, but we do not normalize; the sum should be nearly 1.0 due to the multiplications. Time complexity is O(rounds × S × 6), where S is the number of distinct scores at the current round. In the worst case, S grows roughly linearly with rounds (max score range is [-3*rounds, 3*rounds]), so O(rounds^2) overall. Space complexity is O(S) for the maps.
