/*
Write a C++ function that takes a vector of required skill names and a vector of people, where each person is represented by a vector of skill names they possess, and returns a vector of the indices of the smallest possible team (by number of people) such that every required skill is covered by at least one team member. If multiple minimal teams exist, return any one of them. The function should handle cases where a person has skills not in the required list (ignore them), where some required skills may be missing entirely (in which case return an empty vector, as it's impossible), and where there are up to 16 required skills and up to 60 people. The returned indices should be the original positions in the people vector, sorted in ascending order.
*/
#include <vector>
#include <string>
#include <unordered_map>
#include <algorithm>

// Returns indices of minimal team covering all required skills, or empty if impossible.
std::vector<int> minimalCoveringTeam(
    const std::vector<std::string>& req_skills,
    const std::vector<std::vector<std::string>>& people
) {
    int n_skills = static_cast<int>(req_skills.size());
    int n_people = static_cast<int>(people.size());
    if (n_skills == 0) return {};

    // Map each required skill to a bit position.
    std::unordered_map<std::string, int> skill_to_bit;
    for (int s = 0; s < n_skills; ++s) {
        skill_to_bit[req_skills[s]] = s;
    }

    // Convert each person's skills to a bitmask.
    std::vector<int> person_mask(n_people, 0);
    for (int i = 0; i < n_people; ++i) {
        for (const std::string& skill : people[i]) {
            auto it = skill_to_bit.find(skill);
            if (it != skill_to_bit.end()) {
                person_mask[i] |= (1 << it->second);
            }
        }
    }

    int total_mask = (1 << n_skills) - 1;
    // Sentinel: a large number meaning "unreachable". Using (1LL << n_people) - 1 is fine,
    // but we'll use a dedicated large value for clarity.
    const long long UNREACHABLE = (n_people >= 63) ? (1LL << 62) : ((1LL << n_people) - 1);
    std::vector<long long> dp(1 << n_skills, UNREACHABLE);
    dp[0] = 0; // empty team covers nothing

    for (int i = 0; i < n_people; ++i) {
        int skills = person_mask[i];
        if (skills == 0) continue; // ignore useless people
        for (int mask = total_mask; mask >= 0; --mask) {
            if (dp[mask] == UNREACHABLE) continue; // skip unreachable states
            int new_mask = mask | skills;
            long long candidate_team = dp[mask] | (1LL << i);
            // Check if candidate team is smaller than current best for new_mask.
            // Since we compare popcounts, we need a popcount function.
            int current_size = __builtin_popcountll(dp[new_mask]);
            int candidate_size = __builtin_popcountll(candidate_team);
            if (current_size > candidate_size ||
                (current_size == candidate_size && dp[new_mask] == UNREACHABLE)) {
                dp[new_mask] = candidate_team;
            }
        }
    }

    if (dp[total_mask] == UNREACHABLE) return {};

    long long team_bits = dp[total_mask];
    std::vector<int> result;
    for (int i = 0; i < n_people; ++i) {
        if (team_bits & (1LL << i)) {
            result.push_back(i);
        }
    }
    return result;
}
#include <cassert>
#include <vector>
#include <string>

// The solution function is assumed to be defined above (include it or copy here).

int main() {
    // Example 1: Basic
    std::vector<std::string> skills1 = {"java", "nodejs", "reactjs"};
    std::vector<std::vector<std::string>> people1 = {
        {"java"},
        {"nodejs"},
        {"nodejs", "reactjs"}
    };
    std::vector<int> ans1 = minimalCoveringTeam(skills1, people1);
    // Possible minimal teams: {0,2} or {1,2}. Both size 2.
    assert((ans1 == std::vector<int>{0, 2}) || (ans1 == std::vector<int>{1, 2}));

    // Example 2: One person covers all
    std::vector<std::string> skills2 = {"algorithms", "math", "java", "reactjs", "csharp", "aws"};
    std::vector<std::vector<std::string>> people2 = {
        {"algorithms", "math", "java"},
        {"algorithms", "math", "reactjs"},
        {"java", "csharp", "aws"},
        {"reactjs", "csharp"},
        {"csharp", "math"},
        {"aws", "java"}
    };
    // A minimal team could be {1,2} (size 2) or {0,3,5} (size 3) – so expected size 2.
    std::vector<int> ans2 = minimalCoveringTeam(skills2, people2);
    assert(ans2.size() == 2);

    // Example 3: Impossible (missing skill)
    std::vector<std::string> skills3 = {"a", "b", "c"};
    std::vector<std::vector<std::string>> people3 = {
        {"a", "b"},
        {"b", "c"},
        {"a"}
    };
    std::vector<int> ans3 = minimalCoveringTeam(skills3, people3);
    assert(ans3.empty());

    // Example 4: No required skills → empty team
    std::vector<std::string> skills4 = {};
    std::vector<std::vector<std::string>> people4 = {{"anything"}};
    std::vector<int> ans4 = minimalCoveringTeam(skills4, people4);
    assert(ans4.empty()); // Actually our function returns empty for n_skills==0, but that's ambiguous – test logic: we check size 0.

    // Example 5: Skill with extra irrelevant skills
    std::vector<std::string> skills5 = {"x", "y"};
    std::vector<std::vector<std::string>> people5 = {
        {"x", "z"},
        {"y"},
        {"z"}
    };
    std::vector<int> ans5 = minimalCoveringTeam(skills5, people5);
    assert((ans5 == std::vector<int>{0, 1}));

    // Example 6: Duplicate skills within a person are ignored (by mask OR)
    std::vector<std::string> skills6 = {"p", "q"};
    std::vector<std::vector<std::string>> people6 = {
        {"p", "p", "q"},
        {"q"}
    };
    std::vector<int> ans6 = minimalCoveringTeam(skills6, people6);
    assert(ans6 == std::vector<int>{0});

    // Example 7: Large number of people, small skills – ensure no crash
    std::vector<std::string> skills7 = {"skill0"};
    std::vector<std::vector<std::string>> people7(50);
    for (int i = 0; i < 50; ++i) people7[i] = {"skill" + std::to_string(i % 5)};
    // Only skill0 required, but no one has it? Actually skill0 is a key, but people have skill0..skill4? Let's fix: make everyone have skill0.
    for (int i = 0; i < 50; ++i) people7[i].push_back("skill0");
    std::vector<int> ans7 = minimalCoveringTeam(skills7, people7);
    assert(ans7.size() == 1);

    // Example 8: Multiple people with identical skills – pick one
    std::vector<std::string> skills8 = {"a"};
    std::vector<std::vector<std::string>> people8 = {{"a"}, {"a"}, {"a"}};
    std::vector<int> ans8 = minimalCoveringTeam(skills8, people8);
    assert(ans8.size() == 1 && (ans8[0] == 0 || ans8[0] == 1 || ans8[0] == 2));

    // Example 9: Edge case with n_skills = 16 (max bits)
    std::vector<std::string> skills9;
    for (int i = 0; i < 16; ++i) skills9.push_back("s" + std::to_string(i));
    std::vector<std::vector<std::string>> people9 = {
        {"s0", "s1", "s2"},
        {"s3", "s4", "s5"},
        {"s6", "s7"},
        {"s8", "s9", "s10", "s11"},
        {"s12", "s13", "s14", "s15"}
    };
    std::vector<int> ans9 = minimalCoveringTeam(skills9, people9);
    assert(ans9.size() == 5); // must pick all

    return 0;
}
// The problem is a classic set-cover optimization, but with a twist: we need the minimum number of people, not minimum cost. Since the required skills count is small (n ≤ 16), we can represent the set of skills each person covers as a bitmask of length n. The state space is all possible subsets of skills (2^n), and we use dynamic programming to find the minimum people needed to cover each mask. We initialize dp[mask] as a bitmask of selected people (stored as a 64-bit integer, since up to 60 people). dp[0] = 0 (empty team). For each person i, for each mask from total_mask down to 0 (to avoid reusing the same person in the same iteration), we compute new_mask = mask | person_skills[i]. If the new team size (popcount of dp[new_mask]) is greater than popcount(dp[mask] | (1<<i)), we update dp[new_mask] to that new team. After processing all people, if dp[total_mask] still equals the sentinel value (all bits set, meaning impossible), return an empty vector. Otherwise, extract the indices from the bitmask. Edge cases: people with no relevant skills are skipped; if some required skill is never covered, dp[total_mask] remains sentinel. Time complexity: O(n_people * 2^n_skills) for the DP loop, plus O(n_people) for reading and O(n_people) for output. Space: O(2^n_skills) for dp.
