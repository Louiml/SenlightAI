// Write a C++ function named `formTeams` that takes a vector of integers representing the skills of students in a class, where each skill is either 1 (programming), 2 (math), or 3 (physical education), and returns a vector of triples (as a `vector<tuple<int,int,int>>` or `vector<array<int,3>>`) containing the 1-based indices of students such that each triple contains one student from each skill group. The goal is to form the maximum possible number of complete teams, each with exactly one programmer, one mathematician, and one physical education student. If no complete team can be formed, return an empty vector. The function must preserve the order of selection: for each team, list the programming student’s index first, then the math student, then the PE student. The input vector may be empty or contain only some skill types. The function must not modify the input vector and must be const-correct.
#include <cassert>
#include <tuple>
#include <vector>

int main() {
    // Example from the original problem: skills [1,2,3,1,2,3] → 2 teams
    std::vector<int> skills1 = {1,2,3,1,2,3};
    auto teams1 = formTeams(skills1);
    assert(teams1.size() == 2);
    assert(teams1[0] == std::make_tuple(1,2,3));
    assert(teams1[1] == std::make_tuple(4,5,6));

    // Mixed order and duplicates
    std::vector<int> skills2 = {3,1,2,3,1,2,3,1,2};
    auto teams2 = formTeams(skills2);
    assert(teams2.size() == 3);
    assert(teams2[0] == std::make_tuple(2,3,1));
    assert(teams2[1] == std::make_tuple(5,6,4));
    assert(teams2[2] == std::make_tuple(8,9,7));

    // Only one type present → no teams
    std::vector<int> skills3 = {1,1,1,2,2};
    auto teams3 = formTeams(skills3);
    assert(teams3.empty());

    // Empty input
    std::vector<int> skills4 = {};
    auto teams4 = formTeams(skills4);
    assert(teams4.empty());

    // Unequal groups: 1 prog, 2 math, 3 pe → only 1 team
    std::vector<int> skills5 = {1,2,2,3,3,3};
    auto teams5 = formTeams(skills5);
    assert(teams5.size() == 1);
    assert(teams5[0] == std::make_tuple(1,2,4));

    // Verify const-correctness: pass a const reference
    const std::vector<int> skills6 = {1,2,3};
    auto teams6 = formTeams(skills6);
    assert(teams6.size() == 1);
    assert(teams6[0] == std::make_tuple(1,2,3));

    return 0;
}
#include <vector>
#include <tuple>
#include <algorithm>

// Form maximum number of teams of three students with skills 1, 2, 3.
// Returns a vector of triples (programming_index, math_index, pe_index) with 1-based indices.
std::vector<std::tuple<int,int,int>> formTeams(const std::vector<int>& skills) {
    std::vector<int> programming;
    std::vector<int> math;
    std::vector<int> pe;
    
    for (std::size_t i = 0; i < skills.size(); ++i) {
        if (skills[i] == 1) {
            programming.push_back(static_cast<int>(i) + 1);
        } else if (skills[i] == 2) {
            math.push_back(static_cast<int>(i) + 1);
        } else if (skills[i] == 3) {
            pe.push_back(static_cast<int>(i) + 1);
        }
    }
    
    std::size_t w = std::min({programming.size(), math.size(), pe.size()});
    std::vector<std::tuple<int,int,int>> teams;
    teams.reserve(w);
    
    for (std::size_t i = 0; i < w; ++i) {
        teams.emplace_back(programming[i], math[i], pe[i]);
    }
    
    return teams;
}
// The solution groups student indices by their skill value using three separate vectors: `programming`, `math`, and `pe`. Traverse the input vector once, and for each element at position `i` (0-based in the input, but we store 1-based indices), push `i+1` into the corresponding group based on the skill value. After building the three groups, the maximum number of complete teams is limited by the smallest group size, since each team requires one member from each group. Let `w = min(programming.size(), math.size(), pe.size())`. Then create a result vector of `w` triples, where the `k`-th triple contains `programming[k]`, `math[k]`, and `pe[k]` — this naturally pairs the first w students from each group, guaranteeing correctness because each group only contains distinct indices. Edge cases: if any group is empty, `w` is 0 and result is empty. If the input vector has duplicate skill values or unordered skills, grouping handles it. Time complexity is O(n) for a single pass plus O(w) to build result, overall O(n). Space complexity is O(n) to store the three groups plus O(w) for the result, which fits within O(n) total. The approach is straightforward and robust.
