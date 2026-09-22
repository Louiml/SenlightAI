// Create a C++ function that accepts an integer `N` (number of people) and a 2D array `people` where each row contains two integers representing a person's weight and height. The function must compute the rank of each person using the rule: a person's rank is 1 plus the number of other people who are strictly heavier AND strictly taller. Return the ranks as a `std::vector<int>` in the same order as the input. The input array is guaranteed to have exactly `N` rows and 2 columns. You must handle cases where `N` is 0 (return empty vector) and where multiple people share identical or partially overlapping attributes.

#include <cassert>
#include <vector>

// The function declaration is assumed to be in scope.
std::vector<int> computeRanks(const std::vector<std::vector<int>>& people);

int main() {
    // Single person
    assert((computeRanks({{70, 170}}) == std::vector<int>{1}));
    
    // Two people, one dominates
    assert((computeRanks({{60, 160}, {80, 180}}) == std::vector<int>{2, 1}));
    
    // Two people, no one dominates (different heavier/taller)
    assert((computeRanks({{80, 160}, {60, 180}}) == std::vector<int>{1, 1}));
    
    // Duplicate people
    assert((computeRanks({{70, 170}, {70, 170}, {80, 180}}) == std::vector<int>{2, 2, 1}));
    
    // All same
    assert((computeRanks({{70, 170}, {70, 170}}) == std::vector<int>{1, 1}));
    
    // Larger test
    std::vector<std::vector<int>> people = {{55, 185}, {58, 183}, {88, 186}, {60, 175}, {46, 155}};
    std::vector<int> expected = {3, 2, 1, 1, 1}; // careful: check each
    // Let's compute manually:
    // p0 (55,185): p2 (88,186) dominates -> count=1; p1? 58>55 but 183<185 so not both; p3? 60>55 but 175<185; p4? 46<55 and 155<185. So rank=2.
    // p1 (58,183): p0? 55<58 and 185>183 -> not both; p2 dominates (88>58, 186>183) count=1; p3? 60>58 but 175<183; p4? 46<58, 155<183. So rank=2.
    // p2 (88,186): no one dominates -> rank=1.
    // p3 (60,175): p2 dominates (88>60,186>175) count=1; p0? 55<60 but 185>175; p1? 58<60 but 183>175; p4? 46<60,155<175. So rank=2.
    // p4 (46,155): p0,p1,p2,p3 all have greater weight and height? p0 (55>46,185>155) yes; p1 (58>46,183>155) yes; p2 (88>46,186>155) yes; p3 (60>46,175>155) yes. So rank=5.
    // Correct expected: {2,2,1,2,5}
    assert((computeRanks(people) == std::vector<int>{2, 2, 1, 2, 5}));
    
    // Empty input
    assert(computeRanks({}).empty());
    
    return 0;
}

#include <vector>

// Compute the rank of each person among N people.
// Each person is represented as {weight, height}.
// Rank = 1 + number of people who are strictly heavier AND taller.
// Returns a vector of ranks in the same order as input.
std::vector<int> computeRanks(const std::vector<std::vector<int>>& people) {
    const int n = static_cast<int>(people.size());
    std::vector<int> ranks(n, 1);  // start with rank 1 for all

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (i == j) continue;
            if (people[i][0] < people[j][0] && people[i][1] < people[j][1]) {
                ranks[i]++;  // one more person is heavier and taller
            }
        }
    }
    return ranks;
}

// The core idea is to compare every pair of people to count, for each person `i`, how many other people `j` have both a greater weight and a greater height than person `i`. The final rank is that count plus one (so the largest/most dominant person gets rank 1, and the smallest/least dominant gets rank at most `N`). The main algorithm uses two nested loops over all people, with an inner condition checking strict inequality on both dimensions. Edge cases: if `N` is 0, return empty vector; if there are duplicates (same weight and height), they will not count each other because strict inequality fails; if a person is equal in one dimension but greater in the other, they also do not count because both must be strictly greater. Time complexity is \(O(N^2)\) due to the double loop, and space complexity is \(O(N)\) for the output ranks. The function should take a `const` reference to the 2D container to avoid copying.
