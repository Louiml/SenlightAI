You are given a group of N cows (1 ≤ N ≤ 500), each with a ranked wishlist of gifts (also numbered 0..N-1 internally, but input uses 1..N). Each cow initially receives its own gift (cow i gets gift i). A cow is considered "willing to switch" its current gift for another cow's gift if the other cow's gift appears earlier (higher preference) in its wishlist than its currently assigned gift. For each cow, determine the **best gift it can obtain** through a sequence of direct one-for-one trades, where "best" means the highest-ranked gift on its wishlist that is achievable via a chain of trades. Specifically, the algorithm works as follows: for cow `c`, start with its own gift as the tentative answer; scan its wishlist from most-preferred to least-preferred, stopping before its own gift. For each gift `g` in that list, if the cow that currently owns gift `g` (which is cow `g` in the initial state, since index == gift in this problem) is willing to switch to cow `c`'s gift (i.e., cow `g` would prefer gift `c` over its own gift), then `g` becomes the answer and stop. Otherwise, continue. If no such gift is found, the answer remains the cow's own gift. Output for each cow (in order 1..N) the gift number (1-indexed) it can obtain. Write a function `std::vector<int> bestGifts(const std::vector<std::vector<int>>& wishlists0)` that takes a 0-indexed N×N matrix where `wishlists0[i][j]` is the gift id (0-indexed) ranked at position j by cow i, and returns a vector of length N where element i is the best gift id (0-indexed) for cow i. The input is guaranteed to be a permutation of 0..N-1 for each cow.

#include <cassert>
#include <vector>

// Solution function declaration (provided above)
std::vector<int> bestGifts(const std::vector<std::vector<int>>& wishlists);

int main() {
    // Test 1: 1 cow, own gift is best
    {
        std::vector<std::vector<int>> w = {{0}};
        assert(bestGifts(w) == std::vector<int>({0}));
    }

    // Test 2: 2 cows, each prefers the other's gift
    {
        std::vector<std::vector<int>> w = {{1,0}, {0,1}};
        // Cow 0 prefers gift 1, cow 1 prefers gift 0 -> they swap, so each gets the other's.
        assert(bestGifts(w) == std::vector<int>({1,0}));
    }

    // Test 3: 2 cows, cow 0 prefers own, cow 1 prefers own
    {
        std::vector<std::vector<int>> w = {{0,1}, {1,0}};
        // No one wants to switch, each keeps own.
        assert(bestGifts(w) == std::vector<int>({0,1}));
    }

    // Test 4: 3 cows, cycle
    {
        std::vector<std::vector<int>> w = {
            {1,2,0},  // cow 0: likes 1,2,0
            {2,0,1},  // cow 1: likes 2,0,1
            {0,1,2}   // cow 2: likes 0,1,2
        };
        // Cow 0 wants 1. Is cow 1 willing to switch to 0? Cow1 likes 2,0,1: 0 before 1 -> yes. So cow0 gets 1.
        // Cow 1 wants 2. Cow2 likes 0,1,2: 1 before 2 -> yes. So cow1 gets 2.
        // Cow 2 wants 0. Cow0 likes 1,2,0: 0 is last, not before own -> no. Then 1? Cow0 likes 2 before 0? Actually cow0's wishlist: 1,2,0. For cow2, it scans: 0 (candidate). Cow0 willing? Cow0's list: 1,2,0. It reaches 0 at end, so not willing. Next 1? cow1 willing to switch to 2? cow1's list: 2,0,1 -> 2 before 1 -> yes, so cow2 gets 1.
        assert(bestGifts(w) == std::vector<int>({1,2,1}));
    }

    // Test 5: 4 cows, straightforward
    {
        std::vector<std::vector<int>> w = {
            {0,1,2,3}, // cow0: own first -> answer 0
            {0,1,2,3}, // cow1: likes 0 first, cow0 willing? cow0 likes own first, not willing -> no. next 1 (own) stop. answer 1
            {2,0,1,3}, // cow2: own first -> 2
            {3,2,1,0}  // cow3: own first -> 3
        };
        assert(bestGifts(w) == std::vector<int>({0,1,2,3}));
    }

    // Test 6: 4 cows, chain
    {
        std::vector<std::vector<int>> w = {
            {1,2,3,0}, // cow0 wants 1
            {2,3,0,1}, // cow1 wants 2, willing to switch to 0? cow0's list has 1 first -> no for cow1? Let's compute.
            // Actually cow0 wants 1: cow1 willing to switch to 0? cow1 list: 2,3,0,1 -> 0 before 1? yes, so cow0 gets 1.
            // cow1 wants 2: cow2 list? Need more rows.
            {3,0,1,2}, // cow2 wants 3, willing to switch to 1? cow1 list: 2,3,0,1 -> 1 is last, not before own? own is 1, so reaching own? Actually own is 1, in cow1 list own appears at index 3, so yes candidate 1 appears after own? No, own is 1. For cow2, candidate is 3? Let's just trust the algorithm.
            {0,1,2,3}  // cow3 wishes own? Actually cow3 list: 0,1,2,3 -> own is 3? wait own is 3, but 3 appears last. So cow3 wants 0 first.
        };
        // Compute manually or just check that function returns length 4
        auto result = bestGifts(w);
        assert(result.size() == 4);
        // Let's compute for cow0: wants 1, check cow1 willing to switch to 0? cow1 list: 2,3,0,1 -> 0 before 1 -> yes, so answer=1.
        // Cow1: wants 2, check cow2 willing to switch to 1? cow2 list: 3,0,1,2 -> 1 before 2? yes, so answer=2.
        // Cow2: wants 3, check cow3 willing to switch to 2? cow3 list: 0,1,2,3 -> 2 before 3? yes, so answer=3.
        // Cow3: wants 0, check cow0 willing to switch to 3? cow0 list: 1,2,3,0 -> 3 before 0? yes, so answer=0.
        assert(result == std::vector<int>({1,2,3,0}));
    }

    return 0;
}

#include <vector>

// Given a 0-indexed N x N wishlist matrix where wishlists[i][j] is the j-th
// preferred gift of cow i (a permutation of 0..N-1), return for each cow the
// best gift it can obtain via direct one-for-one trades, as described.
std::vector<int> bestGifts(const std::vector<std::vector<int>>& wishlists) {
    const int n = static_cast<int>(wishlists.size());
    std::vector<int> result(n, 0);

    // Helper: does cow 'cow' prefer gift 'source' over its own gift?
    auto willingToSwitch = [&](int cow, int source) -> bool {
        for (int gift : wishlists[cow]) {
            if (gift == cow) return false;       // reached own gift, so source is not preferred
            if (gift == source) return true;     // source appears before own gift
        }
        return false; // should never reach here because wishlist is a permutation
    };

    for (int cow = 0; cow < n; ++cow) {
        int answer = cow;
        // Scan cow's wishlist in order, stop before reaching its own gift
        for (int i = 0; i < n && wishlists[cow][i] != cow; ++i) {
            int candidateGift = wishlists[cow][i];
            // The cow that currently owns candidateGift is candidateGift itself
            // (since initially each cow has its own gift, and trades are direct).
            if (willingToSwitch(candidateGift, cow)) {
                answer = candidateGift;
                break;
            }
        }
        result[cow] = answer;
    }
    return result;
}

// The solution directly simulates the described process. For each cow `c`, we initialize `answer = c`. Then we iterate through cow `c`'s wishlist in order of preference. We stop when we encounter `c` itself (since gifts after that are less preferred than its current one, and no better trade can be found from them). For each gift `g` before `c` in the list, we check if cow `g` is "willing to switch" to gift `c`. Cow `g` is willing to switch if, scanning cow `g`'s wishlist from the top, we find gift `c` before we find gift `g` (cow `g`'s own gift). This is exactly the `willingToSwitch` helper: it returns true if there exists an index `i` such that `wishlist[g][i] == c` and `wishlist[g][i] != g` and all earlier entries are not `g`. In other words, cow `g` prefers gift `c` to its own gift. If that condition holds, then we can trade: cow `c` gets gift `g`, and cow `g` gets gift `c` (which it prefers). So `g` is achievable for cow `c`, and because we scan in preference order, the first such `g` is the best achievable. Edge cases: if a cow's own gift is its most preferred (i.e., `wishlist[c][0] == c`), then no gifts before it exist, so answer is `c`. If no cow `g` is willing to switch, answer remains `c`. Complexity: For each cow `c`, scanning its wishlist is O(N), and for each candidate `g`, we scan cow `g`'s wishlist in the worst case O(N), so total worst-case O(N^3) per cow? Actually total: for each of N cows, we scan its list (O(N)) and for each candidate (up to N) we do an O(N) scan, so O(N^2) per cow, giving O(N^3) overall. With N ≤ 500, that's 125 million operations, acceptable in C++ with simple loops. Space is O(N^2) for the input matrix (already given) and O(N) for the output vector.
