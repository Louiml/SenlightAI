// Write a C++ function named `countConfidentVotes` that takes a non-empty vector of vectors of integers, where each inner vector represents a voter's three binary choices (each being 0 or 1). The function should return the number of voters who chose `1` for at least two of the three choices. The input is guaranteed to be a rectangular matrix (all inner vectors have exactly three elements) and each element is guaranteed to be either 0 or 1. The function must accept the input by const reference and return an integer.
The problem is a straightforward counting problem. For each voter (each inner vector), iterate over its three elements and count how many are equal to 1. If that count is at least 2, increment the overall answer. No special edge cases exist beyond handling a single voter or all zeros/ones; the algorithm simply loops through all voters exactly once. The main algorithm runs in O(n) time, where n is the number of voters, because each voter is processed in constant time (3 elements). Space complexity is O(1) auxiliary, since we only use a few integer variables and do not allocate any additional data structures.
#include <vector>

// Count the number of voters who chose 1 for at least two of three options.
int countConfidentVotes(const std::vector<std::vector<int>>& votes) {
    int confidentCount = 0;
    for (const auto& voter : votes) {
        int ones = 0;
        for (int choice : voter) {
            if (choice == 1) {
                ++ones;
            }
        }
        if (ones >= 2) {
            ++confidentCount;
        }
    }
    return confidentCount;
}
#include <cassert>
#include <vector>

int countConfidentVotes(const std::vector<std::vector<int>>& votes);

int main() {
    // Single voter with all zeros.
    assert(countConfidentVotes({{0, 0, 0}}) == 0);
    // Single voter with exactly two ones.
    assert(countConfidentVotes({{1, 0, 1}}) == 1);
    // Single voter with all ones.
    assert(countConfidentVotes({{1, 1, 1}}) == 1);
    // Multiple voters with mixed results.
    assert(countConfidentVotes({{1, 1, 0}, {0, 0, 0}, {1, 0, 1}, {0, 1, 1}}) == 3);
    // Voter with exactly one one should not count.
    assert(countConfidentVotes({{1, 0, 0}, {0, 1, 0}, {0, 0, 1}}) == 0);
    // Larger test case with five voters.
    assert(countConfidentVotes({{0, 1, 1}, {1, 1, 1}, {0, 0, 0}, {1, 0, 1}, {1, 1, 0}}) == 4);
    // Test with one voter who has two ones but in different positions.
    assert(countConfidentVotes({{0, 1, 1}}) == 1);
    // Edge case: empty vector (guaranteed by spec, but still safe).
    assert(countConfidentVotes({}) == 0);
    
    return 0;
}
