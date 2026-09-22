// Implement a C++ function `simulateBossFight` that takes four parameters: `n` (number of available abilities), `maxHealth` (initial and maximum health of the boss), `regen` (health regenerated per second, capped at maxHealth), and two vectors `power` and `damage` of length `n`, where the i-th ability has power `power[i]` and damage `damage[i]`. The boss starts at full health. At each integer second t=0,1,2,..., before regeneration, the player may use at most one ability, but only if the boss's current health is at most `maxHealth * power[i] / 100` (i.e., the boss is below the ability's power threshold). Each ability can be used at most once. Using an ability deals its damage instantly, and then the boss regenerates `regen` health (capped at maxHealth). The fight ends when the boss's health becomes ≤ 0 at any moment (after an ability is used, before regeneration is applied). The function must return a vector of pairs `(time, abilityIndex)` in the order abilities are used, where `time` is the second at which the ability is used (0-based). If it is impossible to kill the boss, it returns an empty vector. Use 0-based ability indices. The power vector is given as integers representing percentages (0–100). If no ability is usable at a second and the boss is not dead, the fight is lost. Assume all inputs are valid.

// The problem is a greedy simulation with constraints on when each ability can be used. Sort abilities by decreasing power threshold, because an ability with a higher power threshold is usable earlier (since the condition `hp <= maxHealth * power / 100` becomes true only when hp drops low enough; higher power means the threshold is higher, so it becomes available sooner). At each second, we maintain a max-heap of abilities whose threshold is now met (i.e., current hp ≤ maxHealth * power% / 100). We push all such abilities into the heap before deciding what to use this second. If the heap is empty at the start of a second, we cannot deal damage, and we will never deal damage again unless regeneration increases hp? Actually regeneration only heals, so hp only goes up or stays same (capped), so once no ability is available, it never becomes available later because hp only increases. Therefore, if the heap is empty and the boss is still alive, we output failure (return empty). If the heap is not empty, we use the ability with the highest damage (since using a higher damage now is always optimal because using it later doesn't change availability—it's already available and will remain available because hp will only increase or stay same, but using a stronger ability earlier reduces hp faster, making more abilities available sooner). After using, we subtract damage from hp, then add regen, and cap at maxHealth. Continue until hp ≤ 0. The greedy choice of picking highest damage among currently available abilities is optimal because damage is additive and using a stronger ability earlier cannot hurt: it only reduces hp more, so any ability that becomes available due to a lower hp will become available no later than if we had used a weaker ability. Edge cases: if an ability's power is 100, it is immediately available at full health; if an ability's damage is 0, it should be ignored (but the problem may not give such). If the boss regen keeps hp above the threshold forever, we may get stuck. Time complexity is O(n log n) for sorting and O(n log n) for heap operations, as each ability is pushed and popped at most once. Space O(n).

#include <bits/stdc++.h>

// Simulate the boss fight and return the sequence of ability uses as (time, index) pairs.
std::vector<std::pair<long long, int>> simulateBossFight(
    int n,
    long long maxHealth,
    long long regen,
    const std::vector<long long>& power,
    const std::vector<long long>& damage
) {
    // Sort abilities by descending power threshold.
    std::vector<std::pair<long long, int>> abilities;
    for (int i = 0; i < n; ++i) {
        abilities.emplace_back(power[i], i);
    }
    std::sort(abilities.begin(), abilities.end(),
              [](const auto& a, const auto& b) { return a.first > b.first; });

    long long hp = maxHealth;
    long long totalDamagePerSecond = 0; // Actually we use a heap, not cumulative.
    int ptr = 0;
    long long time = 0;
    std::vector<std::pair<long long, int>> result;

    // Max-heap of available abilities: ordered by damage.
    std::priority_queue<std::pair<long long, int>> available;

    while (hp > 0) {
        // Add all abilities whose power threshold is now met.
        while (ptr < n && hp * 100 <= maxHealth * abilities[ptr].first) {
            int idx = abilities[ptr].second;
            available.emplace(damage[idx], idx);
            ++ptr;
        }

        if (available.empty()) {
            // No ability usable; boss will never die because hp only increases.
            return {};
        }

        // Use the strongest available ability.
        auto use = available.top();
        available.pop();
        result.emplace_back(time, use.second);
        hp -= use.first;

        if (hp <= 0) {
            break;
        }

        // Regeneration after the second.
        hp += regen;
        if (hp > maxHealth) {
            hp = maxHealth;
        }

        ++time;
    }

    return result;
}

#include <cassert>
#include <vector>

int main() {
    // Basic test: two abilities, one immediately usable, damage enough.
    {
        std::vector<long long> power = {100, 50};
        std::vector<long long> damage = {10, 30};
        auto res = simulateBossFight(2, 30, 0, power, damage);
        assert(res.size() == 1);
        assert(res[0].first == 0);
        assert(res[0].second == 1); // ability 1 has damage 30, kills in one hit
    }

    // Need multiple abilities, regen complicates.
    {
        std::vector<long long> power = {100, 50, 50};
        std::vector<long long> damage = {10, 10, 10};
        auto res = simulateBossFight(3, 25, 5, power, damage);
        // At t=0, both 100% ability usable (power=100), and also 50? hp=25, threshold for 50 is 25*100 <= 25*50? no, 2500 <= 1250 false, so only power 100 available.
        // Use damage 10, hp=15, +5 regen => hp=20. t=1: now hp=20, threshold for 50 is 2000 <= 1250? still false. So still only power 100 available? But we already used it. Others not available. Heap empty -> fail.
        // So should fail.
        assert(res.empty());
    }

    // Successful multi-hit with regen.
    {
        std::vector<long long> power = {100, 100, 100};
        std::vector<long long> damage = {8, 8, 8};
        auto res = simulateBossFight(3, 20, 3, power, damage);
        // hp=20, use 8 -> 12, +3 -> 15. t=1 use 8 -> 7, +3 -> 10. t=2 use 8 -> 2, +3 -> 5. t=3? All used, hp=5, heap empty -> fail.
        assert(res.empty());
    }

    // Regeneration never lets hp drop, but damage enough to kill in first hit.
    {
        std::vector<long long> power = {100};
        std::vector<long long> damage = {100};
        auto res = simulateBossFight(1, 50, 1000, power, damage);
        assert(res.size() == 1);
        assert(res[0] == std::make_pair(0LL, 0));
    }

    // Multiple abilities, lower power threshold requires hp drop.
    {
        std::vector<long long> power = {100, 40};
        std::vector<long long> damage = {10, 5};
        auto res = simulateBossFight(2, 20, 0, power, damage);
        // t=0: hp=20, ability0 usable (power=100). ability1 threshold: 20*100 <= 20*40? 2000<=800 false. Use ability0 -> hp=10.
        // t=1: hp=10, threshold for ability1: 10*100 <= 20*40? 1000<=800 false. So no ability1. But we already used ability0, heap empty -> fail.
        assert(res.empty());
    }

    // Case where ability with lower power becomes available after first hit.
    {
        std::vector<long long> power = {100, 50};
        std::vector<long long> damage = {5, 20};
        auto res = simulateBossFight(2, 20, 0, power, damage);
        // t=0: hp=20, ability0 usable (power100) -> hp=15. ability1 threshold: 15*100 <= 20*50? 1500<=1000 false.
        // t=1: still hp=15, no ability1, ability0 used, heap empty -> fail.
        assert(res.empty());
    }

    // A working case: power 100 damages enough to drop hp below threshold.
    {
        std::vector<long long> power = {100, 50};
        std::vector<long long> damage = {10, 15};
        auto res = simulateBossFight(2, 20, 0, power, damage);
        // t=0: hp=20, ability0 usable -> hp=10. Now ability1 threshold: 10*100 <= 20*50? yes 1000<=1000 -> available.
        // t=1: use ability1 (damage 15) -> hp=-5, fight ends.
        assert(res.size() == 2);
        assert(res[0] == std::make_pair(0LL, 0));
        assert(res[1] == std::make_pair(1LL, 1));
    }

    // Regeneration and multiple abilities.
    {
        std::vector<long long> power = {100, 80, 80};
        std::vector<long long> damage = {10, 10, 10};
        auto res = simulateBossFight(3, 30, 5, power, damage);
        // t=0: hp=30, ability0 usable (power100), others not yet. Use d10 -> hp=20, +5 => 25.
        // t=1: hp=25, ability0 used, ability1/2 threshold: 25*100 <= 30*80? 2500 <= 2400 false. So not available. heap empty -> fail.
        assert(res.empty());
    }

    // A successful long fight with regen.
    {
        std::vector<long long> power = {100, 50, 50, 50};
        std::vector<long long> damage = {8, 9, 9, 9};
        auto res = simulateBossFight(4, 30, 2, power, damage);
        // Let's simulate:
        // t=0: hp=30, ability0 (pow100) avail -> use d8 => hp22, +2 =>24.
        // t=1: hp=24, ability0 used, others? threshold 24*100 <= 30*50? 2400<=1500 false. So no. fail.
        assert(res.empty());
    }

    // Use all abilities.
    {
        std::vector<long long> power = {100, 100, 100};
        std::vector<long long> damage = {5, 5, 5};
        auto res = simulateBossFight(3, 15, 0, power, damage);
        // t=0: hp=15, use d5 -> 10.
        // t=1: hp=10, use d5 -> 5.
        // t=2: hp=5, use d5 -> 0, kills.
        assert(res.size() == 3);
        assert(res[0].first == 0);
        assert(res[1].first == 1);
        assert(res[2].first == 2);
    }

    return 0;
}
