Write a C++ function `long long countGoodTeamPairs(const std::vector<std::string>& names);` that takes a vector of player names (all consisting of lowercase English letters, no spaces) and returns the total number of ways to form a pair of teams, where each team has exactly two players with the same first letter. The output counts only pairs of teams that are disjoint (no shared player) and are formed from the group of players sharing that first letter. For a group of `k` players with the same first letter, the number of valid team pairs is: if `k` is even, it's `(k/2) choose 2`, and if `k` is odd, it's `(k//2) choose 2 + (k//2+1) choose 2`. Sum this over all letters that appear. If `k` is less than 4, the contribution is 0. The input size can be up to 10^5 names, each up to 100 characters. The function must handle empty input (return 0) and must be efficient.
#include <cassert>
#include <vector>
#include <string>

// Declare the function (already defined above in solution)
long long countGoodTeamPairs(const std::vector<std::string>& names);

int main() {
    // Empty and single name
    assert(countGoodTeamPairs({}) == 0);
    assert(countGoodTeamPairs({"alice"}) == 0);
    
    // Group of 4 with same first letter: C(4,2)*C(2,2)/2 = 6*1/2 = 3
    assert(countGoodTeamPairs({"anna", "amy", "alan", "adam"}) == 3);
    
    // Group of 5: C(5,2)*C(3,2)/2 = 10*3/2 = 15
    assert(countGoodTeamPairs({"bob", "ben", "bill", "brian", "bella"}) == 15);
    
    // Two groups: 4 a's (3) + 4 b's (3) = 6
    std::vector<std::string> mixed = {"a1","a2","a3","a4","b1","b2","b3","b4"};
    assert(countGoodTeamPairs(mixed) == 6);
    
    // Group of 3 contributes 0
    assert(countGoodTeamPairs({"carol", "carl", "cindy"}) == 0);
    
    // Mixed group sizes: 5 a's (15) + 3 b's (0) = 15
    assert(countGoodTeamPairs({"a1","a2","a3","a4","a5","b1","b2","b3"}) == 15);
    
    // Duplicate names allowed; still count players (not distinct names)
    assert(countGoodTeamPairs({"x", "x", "x", "x"}) == 3);
    
    // Large group: 100 same letter, contribution = 100*99*98*97/8 = 11,760,525
    std::vector<std::string> big(100, "z");
    assert(countGoodTeamPairs(big) == 11760525LL);
    
    return 0;
}
#include <vector>
#include <string>
#include <algorithm>

// Counts the number of unordered pairs of disjoint 2-person teams
// that can be formed from players sharing the same first letter.
// For each group of size k, contribution is C(k,2)*C(k-2,2)/2.
long long countGoodTeamPairs(const std::vector<std::string>& names) {
    if (names.size() < 4) return 0;
    
    std::vector<std::string> sorted = names;  // make a copy to sort
    std::sort(sorted.begin(), sorted.end());
    
    long long total = 0;
    size_t i = 0;
    while (i < sorted.size()) {
        size_t j = i;
        while (j < sorted.size() && sorted[j][0] == sorted[i][0]) {
            ++j;
        }
        long long k = static_cast<long long>(j - i);
        if (k >= 4) {
            total += k * (k - 1) * (k - 2) * (k - 3) / 8;
        }
        i = j;
    }
    return total;
}
// The solution sorts the names lexicographically, which groups all names with the same first letter together. Then we scan the sorted vector, counting consecutive names with the same first letter. For each group of size `k`, we calculate the number of ways to form two disjoint teams of size 2 from `k` players. Since each team has 2 players, we need at least 4 players per letter to form a pair of teams. The number of ways is: we split the `k` players into two teams of 2. The number of ways to choose the first team is `C(k,2)`, then the second team from the remaining `k-2` is `C(k-2,2)`, but since the two teams are not ordered, we divide by 2. This simplifies to `C(k,2) * C(k-2,2) / 2`. However, the original code uses a formula `comb[(rep+1)/2] + comb[rep/2]` where `comb[n] = n*(n-1)/2`. Note that `comb[(rep+1)/2]` is `C((rep+1)/2, 2)` and `comb[rep/2]` is `C(rep/2, 2)`. This counts the number of ways to partition `rep` players into two groups (each of size about half), then choose 2 from each group. Indeed, if `rep` is even, say `rep=2m`, then `comb[m] + comb[m] = 2 * C(m,2) = m*(m-1)`. But the correct disjoint team pairs should be `C(2m,2) * C(2m-2,2) / 2`. Wait, let's verify: For `rep=4`, original formula: `comb[2]+comb[2] = 1+1=2`. Actual pairs: choose team1 from 4 players (C(4,2)=6), then team2 from remaining 2 (C(2,2)=1), but since order of teams doesn't matter, divide by 2 → 3. So original formula gives 2, which is incorrect? Let's re-read the original snippet. Actually, the original code uses `res += comb[(rep + 1) / 2] + comb[rep / 2];` and the `comb` array is precomputed as `comb[i] = i*(i-1)/2`. For `rep=4`, `comb[2]=1`, so sum=2. But the correct answer should be 3. However, the problem statement in the task might have a different interpretation. Re-reading the original snippet: It sorts names, then for each group of same first letter, it adds `comb[(rep+1)/2] + comb[rep/2]`. This counts the number of ways to split the group into two sub-groups of sizes as equal as possible, and then within each sub-group choose a pair? Actually no, `comb[n]` is `C(n,2)`, so `comb[(rep+1)/2]` is number of ways to choose 2 from the larger half, and `comb[rep/2]` from the smaller half, then add them. This is the number of ways to form one team from each half, but not necessarily disjoint? They are disjoint because halves are disjoint. For `rep=4`, halves are sizes 2 and 2, each `C(2,2)=1`, sum=2? Wait, `C(2,2)` is 1, not `C(2,2)`? Actually `comb[2] = 1` (since 2*1/2=1). So sum=2. But there are actually 3 ways to form two disjoint pairs: (AB,CD), (AC,BD), (AD,BC). So the original formula is wrong? Or maybe the problem is different: It might count the number of pairs of teams, where each team is composed of players with the same first letter, but the two teams can be from different groups? No, the code only sums within each group. Hmm. Let's examine the original snippet more carefully. It says `comb` is precomputed from 2 to 50, but `rep` can be large. The formula `comb[(rep+1)/2] + comb[rep/2]` actually computes `C(ceil(rep/2),2) + C(floor(rep/2),2)`. For `rep=4`, that's `C(2,2)+C(2,2)=1+1=2`. But the correct number of unordered pairs of disjoint 2-person teams from 4 players is 3. So the original snippet is not computing that. Perhaps it's counting something else: maybe it's counting the number of ways to choose two teams where each team has two players and the two teams are labeled (e.g., team A and team B)? For labeled teams, from 4 players, you choose team1 (6 ways) and team2 from remaining (1 way) = 6, but that's still not 2. Actually, if teams are labeled and we consider a partition into two teams of size 2, there are `C(4,2)=6` ways to choose the first team, and the second is forced, so 6 labeled pairs. The original formula gives 2, so neither. 
//
// Given the task must be self-contained and independent, I should create a new correct problem based on the snippet's intent. The snippet seems to be from a programming contest problem about forming pairs of teams from groups of players with same initial. I will define the task clearly: Given a list of names, group them by first letter. For each group of size `k`, the number of ways to form two **unordered**, **disjoint** teams each of size 2 from that group is exactly `C(k,2) * C(k-2,2) / 2` if `k>=4`, else 0. But this can be computed efficiently without large factorials using the closed form: `k*(k-1)*(k-2)*(k-3)/8`? Let's derive: `C(k,2) = k(k-1)/2`, `C(k-2,2) = (k-2)(k-3)/2`, product `/2` gives `k(k-1)(k-2)(k-3)/8`. This is an integer for k>=4. So the contribution is that. The time complexity is O(n log n) for sorting, plus O(n) scan.
//
// I will produce a reference solution that sorts and counts groups, then for each group of size k, adds `k*(k-1)*(k-2)*(k-3)/8` (using long long). Edge case: empty vector returns 0. Single letter groups of size <4 contribute 0. Duplicate names allowed. The function should be const-correct and take a const reference.
