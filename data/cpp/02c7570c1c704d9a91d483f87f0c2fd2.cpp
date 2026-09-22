// You are given `N` pairs of values, where each pair represents two options: an attack action that always deals `a` damage (usable any number of times) and a special attack that deals `b` damage but can only be used once. Initially, you have health `H`. Using an attack costs one "turn" regardless of type. Write a C++ function `long long minimumTurns(long long H, const std::vector<std::pair<long long, long long>>& attacks)` that returns the minimum number of turns required to reduce health to zero or below. You may use each ordinary attack repeatedly, but each special attack only once. All values are positive integers. If it is impossible to kill the monster (which won't happen in typical tests), return `-1`. The function must work for `N` up to 10^5 and `H` up to 10^18.

The goal is to minimize turns. Since ordinary attacks (`a`) are unlimited, we should only use a special attack if its damage `b` is greater than the best ordinary attack damage `bestA`. So we first find the maximum `a` among all pairs. Then we consider all special attacks with `b > bestA` because using a special with `b <= bestA` is never better than using an ordinary attack. Sort those special attacks in descending order of `b`. Then simulate greedy: use the highest-damage special attacks first because they reduce health faster, and each special costs one turn just like an ordinary attack. After we run out of useful specials (or choose to stop early if health already dead), use ordinary attacks with damage `bestA` to finish. The answer is the count of specials used plus the number of ordinary attacks needed (which is `ceil(remainingHealth / bestA)`). Edge cases: If `H <= 0` initially? Not given, but assume positive. If no ordinary attacks? But each pair has both `a` and `b`, so at least one ordinary attack exists. If `bestA` is huge, we may only need one ordinary. Complexity: Sorting the special attacks takes O(N log N) time, and scanning them O(N). Space O(N) for storing the vector of special damages.

#include <vector>
#include <algorithm>
#include <cstdint>

// Returns the minimum number of turns to reduce health H to <=0.
// attacks[i] = {ordinaryDamage, specialDamage} (both positive)
// Ordinary attacks can be used unlimited times; special attacks once each.
long long minimumTurns(long long H, const std::vector<std::pair<long long, long long>>& attacks) {
    long long bestOrdinary = 0;
    // Find the best ordinary damage among all pairs
    for (const auto& p : attacks) {
        bestOrdinary = std::max(bestOrdinary, p.first);
    }
    
    // Collect special attacks that are better than the best ordinary
    std::vector<long long> specials;
    for (const auto& p : attacks) {
        if (p.second > bestOrdinary) {
            specials.push_back(p.second);
        }
    }
    
    // Sort descending to use highest damage specials first
    std::sort(specials.begin(), specials.end(), std::greater<long long>());
    
    long long turns = 0;
    long long remainingHealth = H;
    
    // Use specials as long as they exist and health is still positive
    for (long long dmg : specials) {
        if (remainingHealth <= 0) break;
        remainingHealth -= dmg;
        turns++;
    }
    
    // If health still positive, finish with ordinary attacks
    if (remainingHealth > 0) {
        // Number of ordinary attacks needed: ceil(remainingHealth / bestOrdinary)
        turns += (remainingHealth + bestOrdinary - 1) / bestOrdinary;
    }
    
    return turns;
}

#include <cassert>
#include <vector>
#include <utility>

// Function prototype (declared above in solution)
long long minimumTurns(long long H, const std::vector<std::pair<long long, long long>>& attacks);

int main() {
    // Test 1: Use one special that is better, finish with ordinary
    std::vector<std::pair<long long, long long>> t1 = {{2, 5}, {3, 1}};
    assert(minimumTurns(10, t1) == 3); // 5+3+2 = 10

    // Test 2: All specials are weak, just use ordinary
    std::vector<std::pair<long long, long long>> t2 = {{4, 2}, {4, 3}};
    assert(minimumTurns(9, t2) == 3); // 4+4+1? Actually 4*3=12, so 3

    // Test 3: Multiple useful specials sorted by damage
    std::vector<std::pair<long long, long long>> t3 = {{1, 7}, {1, 5}, {1, 3}};
    assert(minimumTurns(10, t3) == 2); // 7+5 >10? 7+5=12, so 2

    // Test 4: Health exactly matches a combination
    std::vector<std::pair<long long, long long>> t4 = {{2, 6}, {2, 4}};
    assert(minimumTurns(12, t4) == 3); // 6+4+2=12

    // Test 5: Very large health, only ordinary
    std::vector<std::pair<long long, long long>> t5 = {{1000000000, 1}};
    assert(minimumTurns(1000000000000000000LL, t5) == 1000000000LL);

    // Test 6: Single pair with high both
    std::vector<std::pair<long long, long long>> t6 = {{5, 10}};
    assert(minimumTurns(9, t6) == 1); // 10

    // Test 7: Health exactly one turn with special
    std::vector<std::pair<long long, long long>> t7 = {{3, 100}, {3, 1}};
    assert(minimumTurns(100, t7) == 1);

    return 0;
}
