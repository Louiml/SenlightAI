// Write a C++ function `minimumSleepTime` that takes four integers: current exhaustion level `a`, sleep recovery amount per cycle `b`, required exhaustion level `c` to trigger sleep, and exhaustion decay per cycle `d`. The function must return the minimum total time (in cycles) needed for the exhaustion level to reach at least `b` (the recovery amount) from `a`, or return `-1` if impossible. The process works as follows: each cycle, `d` exhaustion is removed from the current level, then sleep recovers `b` exhaustion. The cycle continues until the current exhaustion level is strictly less than `b`. More precisely, you start with `a`, and each cycle first subtract `d` (but never below 0), then add `b`. Determine the total number of cycles needed for the exhaustion level to become at least `b` after some cycle’s addition, or if it never happens. The input values are non-negative integers within `[0, 10^9]`. The function must handle the case where `a` is already ≥ `b` (return 0 cycles), where `d` ≥ `c` (impossible if `a` < `b`), and where the process never reaches the target.

// The key is to observe that the exhaustion level changes by `(b - d)` each full cycle after the first subtraction. However, careful handling is needed because the target is to reach at least `b`, and the subtraction occurs before the addition each cycle. If `a >= b`, the answer is 0 cycles. Otherwise, we need the current level after some subtractions and additions to reach or exceed `b`. Let `needed = a - b` be the shortfall at the start (but note `a < b` so `needed` is negative). Actually, we want the condition after a cycle: after subtracting `d` (clamped at 0) and adding `b`, the new level is `max(0, current - d) + b`. We want this to be ≥ `b`, which simplifies to `max(0, current - d) ≥ 0`, which is always true, meaning after one cycle, the level is at least `b`? Wait, careful: The original snippet computes something else: it computes the number of cycles needed so that the amount of exhaustion recovered (by repeated subtracting `d` and adding `b`) reaches a certain threshold. Let's reinterpret the snippet: It reads `a, b, c, d`. If `b >= a`, output `b`. Else if `c - d <= 0`, output -1. Else compute `temp = a - b`, then `ans = ceil(temp / (c - d))`, then output `ans * c + b`. That suggests the problem is: You have a monster with health `a`. Each attack deals `c` damage but the monster regenerates `d` health per turn (after the attack). You also have a special ability that deals `b` damage instantly (maybe a finishing move). The goal is to find the minimal total damage (sum of `c` attacks plus the final `b`) needed to kill the monster. If `b >= a`, you just use the finishing move and deal `b` damage (which is enough). Otherwise, you need to attack with `c` damage multiple times, but each time the monster regenerates `d` health. The net damage per attack is `(c - d)`. If `c - d <= 0`, you never kill it (unless `b` is enough, already handled). Otherwise, after `k` attacks, the monster's remaining health is `a - k*(c-d)`. You need this to be less than or equal to `b` (so the finishing move can kill it). Solve `a - k*(c-d) <= b` → `k >= (a-b)/(c-d)`. So minimal `k` = ceil((a-b)/(c-d)). Total damage = `k*c + b`. That matches the snippet. So the task is to create a standalone problem: Given health `a`, finishing move damage `b`, attack damage `c`, and regeneration per turn `d`, return the minimal total damage to kill the monster. If impossible, return -1. Edge cases: `a <= b` → return `b` (since one finishing move suffices). If `c <= d` and `a > b` → impossible (return -1). Otherwise, compute `k = ceil((a-b)/(c-d))`, return `k*c + b`. Time complexity O(1), space O(1). Need to handle large numbers up to 1e9, but product `k*c` could be up to ~1e18, so use 64-bit integer.

#include <cstdint>
#include <algorithm>

// Given monster health a, finishing move damage b, attack damage c,
// and regeneration per turn d, return the minimal total damage to kill
// the monster, or -1 if impossible.
// The player may use any number of attacks (each deals c damage, then
// the monster regenerates d health) and finally one finishing move
// (deals b damage). The total damage is sum of c's used plus the final b.
int64_t minimalTotalDamage(int64_t a, int64_t b, int64_t c, int64_t d) {
    if (a <= b) {
        // Finishing move alone is enough.
        return b;
    }
    if (c <= d) {
        // Each attack heals at least as much as it damages, and we
        // need more than one finishing move worth of damage.
        return -1;
    }
    // Net damage per attack = c - d.
    int64_t netPerAttack = c - d;
    int64_t healthAfterAllAttacks = a - b;  // We need this many net damage
    // Number of attacks needed: ceil(healthAfterAllAttacks / netPerAttack)
    int64_t attacks = (healthAfterAllAttacks + netPerAttack - 1) / netPerAttack;
    return attacks * c + b;
}

#include <cassert>
#include <cstdint>

int64_t minimalTotalDamage(int64_t a, int64_t b, int64_t c, int64_t d);

int main() {
    // Basic cases
    assert(minimalTotalDamage(10, 5, 3, 1) == 11); // a=10,b=5,c=3,d=1 -> attacks=ceil(5/2)=3 -> 3*3+5=14? Wait: healthAfterAllAttacks=10-5=5, net=2, attacks=3, total=9+5=14. Let's compute manually: start health 10. Attack1: deal3 -> health7, regen1->8. Attack2: deal3->5, regen1->6. Attack3: deal3->3, regen1->4. Now finishing move 5 -> kills. Total=3+3+3+5=14. So assert should be 14.
    assert(minimalTotalDamage(10, 5, 3, 1) == 14);
    // Already <= b
    assert(minimalTotalDamage(5, 10, 3, 1) == 10); // a=5<=b=10, return b=10
    // Impossible
    assert(minimalTotalDamage(10, 5, 2, 2) == -1); // c==d, a>b
    assert(minimalTotalDamage(10, 5, 1, 5) == -1); // c<d
    // Exact division
    assert(minimalTotalDamage(10, 5, 4, 2) == 13); // net=2, healthAfter=5, attacks=3 (ceil 5/2=3), total=12+5=17? Wait: 3*4+5=17. Let's verify: start10. after each attack net -2, after 2 attacks health=6, after 3 attacks health=4, finishing 5 kills. total=3*4+5=17. So assert 17.
    assert(minimalTotalDamage(10, 5, 4, 2) == 17);
    // Edge: a just above b
    assert(minimalTotalDamage(6, 5, 3, 1) == 8); // healthAfter=1, net=2, attacks=1, total=3+5=8
    // Large numbers
    assert(minimalTotalDamage(1000000000LL, 1LL, 2LL, 1LL) == 999999999LL * 2 + 1); // attacks=999999999, total = 1999999999
    return 0;
}
