/*
Write a C++ function that processes multiple game rounds, where for each round it receives the number of shots, a list of target heights (positive integers), and a string of moves ('S' for shoot, 'J' for jump). The function should count how many shots hit the target: a shot hits if the move is 'S' and the target height is 1 or 2; a move 'J' hits if the target height is greater than 2 (since jumping over a low target counts as a successful dodge, but jumping over a high target means you still hit it). The function takes the number of rounds, and for each round takes the number of shots, the vector of heights, and a string of moves, and returns a vector of integers containing the hit count for each round. The input is guaranteed to have exactly T characters in the moves string for each round.
*/
#include <string>
#include <vector>

// Count successful hits per round.
// heights: vector of vectors, each inner vector contains target heights for a round.
// moves: vector of strings, each string contains 'S' or 'J' for the corresponding round.
// Returns a vector with the number of hits for each round.
std::vector<int> countHits(const std::vector<std::vector<int>>& heights,
                           const std::vector<std::string>& moves) {
    std::vector<int> results;
    results.reserve(heights.size());
    
    for (std::size_t round = 0; round < heights.size(); ++round) {
        const auto& h = heights[round];
        const std::string& m = moves[round];
        int hitCount = 0;
        
        // Both h and m are guaranteed to have the same length T.
        for (std::size_t i = 0; i < h.size(); ++i) {
            if (m[i] == 'S') {
                if (h[i] == 1 || h[i] == 2) {
                    ++hitCount;
                }
            } else if (m[i] == 'J') {
                if (h[i] > 2) {
                    ++hitCount;
                }
            }
            // Any other move character is ignored (but input guarantees only S or J).
        }
        results.push_back(hitCount);
    }
    return results;
}
#include <cassert>
#include <vector>
#include <string>

// The solution function is declared here (for completeness in test, but in real usage it's separate)
std::vector<int> countHits(const std::vector<std::vector<int>>& heights,
                           const std::vector<std::string>& moves);

int main() {
    // Single round, T=3
    std::vector<std::vector<int>> h1 = {{1, 3, 2}};
    std::vector<std::string> m1 = {"S", "J", "S"};
    // h[0]=1 + 'S' -> hit; h[1]=3 + 'J' -> hit; h[2]=2 + 'S' -> hit => 3
    assert(countHits(h1, m1) == std::vector<int>({3}));

    // Two rounds
    std::vector<std::vector<int>> h2 = {{1, 2, 3, 4}, {5, 1, 2}};
    std::vector<std::string> m2 = {"S", "J", "S", "J", "S", "J", "S"};
    // Round1: shot0 S/1 hit, shot1 J/2 no hit (2 not >2), shot2 S/3 no hit, shot3 J/4 hit => 2
    // Round2: shot0 S/5 no, shot1 J/1 no, shot2 S/2 hit => 1
    assert(countHits(h2, m2) == std::vector<int>({2, 1}));

    // Edge case: empty round
    std::vector<std::vector<int>> h3 = {{}};
    std::vector<std::string> m3 = {""};
    assert(countHits(h3, m3) == std::vector<int>({0}));

    // All misses: S with high target, J with low target
    std::vector<std::vector<int>> h4 = {{3, 1}};
    std::vector<std::string> m4 = {"S", "J"};
    assert(countHits(h4, m4) == std::vector<int>({0}));

    // Mixed with repeated values
    std::vector<std::vector<int>> h5 = {{2, 2, 5, 1, 3}};
    std::vector<std::string> m5 = {"S", "J", "S", "J", "J"};
    // S/2 hit, J/2 no hit, S/5 no, J/1 no, J/3 hit => 2
    assert(countHits(h5, m5) == std::vector<int>({2}));

    // Multiple rounds with different lengths
    std::vector<std::vector<int>> h6 = {{1, 4}, {2}, {10, 11, 12}};
    std::vector<std::string> m6 = {"S", "J", "S", "J", "S", "J"}; // lengths: 2,1,3
    // Round1: S/1 hit, J/4 hit => 2
    // Round2: S/2 hit => 1
    // Round3: J/10 hit, S/11 no, J/12 hit => 2
    assert(countHits(h6, m6) == std::vector<int>({2, 1, 2}));

    return 0;
}
// The solution iterates through each round, and within each round loops over each shot index `i`. For each index, it checks the move character at position `i` and the corresponding height in the vector. If the move is 'S' and height is 1 or 2, increment count; if the move is 'J' and height > 2, increment count. All other combinations (e.g., 'S' with height 3, 'J' with height 1) are not counted. Edge cases: empty rounds (T=0) should produce count 0; the moves string must be exactly length T; heights are positive integers, but the logic works for any integer because we compare >2 and ==1 or ==2. Time complexity: O(total number of shots across all rounds) since each shot is processed once. Space complexity: O(number of rounds) for the output vector, plus O(T) for storing the heights per round if we copy them, but we can process directly if given as a vector. In this implementation we accept the heights as a `const std::vector<int>&` and moves as `const std::string&` for each round, but since the function signature must handle multiple rounds, we accept vectors of vectors and strings.
