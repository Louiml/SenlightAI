Write a standalone C++ function `int minimalEscapeCost(const std::vector<std::pair<int,int>>& monsters, int health)` where each pair contains `(damage, plague_value)`. Given `n` monsters, your hero has `health` total HP. You must defeat all monsters, but you may choose to "bribe" some monsters by paying HP equal to their damage, which removes them entirely without taking plague damage. Monsters not bribed will deal their plague damage × damage to you after the battle. You want to minimize the total plague damage taken. Return the minimal plague damage, or `0` if you can bribe all monsters within the given health. The input vector will be non-empty, all damages and plague values are positive integers, and the sum of all damages may be larger than `INT_MAX`/2 but each individual fits in `int`.

This is a greedy optimization problem. The key observation is that bribing a monster costs HP equal to its damage but saves plague damage equal to `damage × plague`. To minimize total plague damage, we should prioritize bribing monsters with the highest plague value first, since they contribute the most plague per HP spent on bribing. More formally, we sort monsters by descending plague value, then iterate through them while we have remaining health. For each monster, if we have enough health to bribe it (pay its damage), we do so and deduct the damage from health; otherwise, we cannot bribe it, and it will deal its full damage × plague to us. After processing all monsters or running out of health, we sum the plague contributions from all unbribed monsters. Edge cases: if total damage ≤ health, we can bribe everything and return 0. If health reaches 0 before all monsters are processed, remaining monsters are all unbribed. Time complexity is O(n log n) due to sorting, space O(1) beyond input storage. The example code snippet used a similar greedy but without sorting, which is incorrect; we must sort by plague descending for optimality.

#include <vector>
#include <algorithm>
#include <numeric>

// Given monsters as (damage, plague), and a health pool, return the minimal
// total plague damage suffered after optimally bribing monsters with HP.
// Bribing a monster costs its damage in HP and avoids its plague damage.
// We always bribe monsters with the highest plague first.
int minimalEscapeCost(const std::vector<std::pair<int,int>>& monsters, int health) {
    // Sort by plague descending; if equal, no matter the order.
    std::vector<std::pair<int,int>> sorted = monsters;
    std::sort(sorted.begin(), sorted.end(),
              [](const auto& a, const auto& b) { return a.second > b.second; });

    long long total_damage = 0;
    for (const auto& m : sorted) {
        total_damage += m.first;
    }
    if (total_damage <= static_cast<long long>(health)) {
        return 0;
    }

    long long remaining_health = health;
    long long total_plague = 0;

    for (const auto& m : sorted) {
        if (remaining_health <= 0) {
            // No more health to bribe, all remaining monsters cause plague.
            total_plague += static_cast<long long>(m.first) * m.second;
        } else if (remaining_health >= m.first) {
            // Enough health to bribe this monster.
            remaining_health -= m.first;
        } else {
            // Not enough health to bribe, so this monster deals plague damage.
            total_plague += static_cast<long long>(m.first) * m.second;
        }
    }

    return static_cast<int>(total_plague);
}

#include <cassert>
#include <vector>

int main() {
    // Case 1: Can bribe everything
    std::vector<std::pair<int,int>> m1 = {{5, 10}, {3, 2}};
    assert(minimalEscapeCost(m1, 8) == 0);

    // Case 2: Bribe highest plague first
    std::vector<std::pair<int,int>> m2 = {{5, 10}, {3, 2}};
    assert(minimalEscapeCost(m2, 5) == 6); // bribe damage 5 plague 10, remaining damage 3 plague 2 -> 6

    // Case 3: No health at all
    std::vector<std::pair<int,int>> m3 = {{2, 5}, {4, 3}};
    assert(minimalEscapeCost(m3, 0) == 2*5 + 4*3);

    // Case 4: Health exactly enough for one specific bribe
    std::vector<std::pair<int,int>> m4 = {{5, 100}, {5, 1}};
    assert(minimalEscapeCost(m4, 5) == 5); // bribe first, second deals 5*1=5

    // Case 5: Duplicate plague values
    std::vector<std::pair<int,int>> m5 = {{2, 7}, {2, 7}, {2, 7}};
    assert(minimalEscapeCost(m5, 4) == 14); // bribe two, one left deals 2*7=14

    // Case 6: Large numbers within int range
    std::vector<std::pair<int,int>> m6 = {{1000000, 1000000}, {1, 1}};
    assert(minimalEscapeCost(m6, 1000000) == 1); // bribe big, small deals 1*1=1

    // Case 7: Health more than total damage but not enough? Actually total damage <= health returns 0
    std::vector<std::pair<int,int>> m7 = {{1, 100}, {1, 100}};
    assert(minimalEscapeCost(m7, 2) == 0);

    // Case 8: One monster only
    std::vector<std::pair<int,int>> m8 = {{3, 4}};
    assert(minimalEscapeCost(m8, 2) == 12); // can't bribe

    // Case 9: Health equal to damage of one but not other
    std::vector<std::pair<int,int>> m9 = {{5, 10}, {1, 100}};
    assert(minimalEscapeCost(m9, 5) == 100); // bribe first (plague 10), second (plague 100) deals 1*100

    // Case 10: All monsters same damage
    std::vector<std::pair<int,int>> m10 = {{2, 5}, {2, 5}, {2, 5}};
    assert(minimalEscapeCost(m10, 1) == 30); // can't bribe any
    assert(minimalEscapeCost(m10, 6) == 0); // bribe all
}
