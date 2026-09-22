Write a C++ function named `findClosestEnemyKart` that, given a vector of `KartInfo` structures (where each structure contains a unique `id`, a `team_id`, an `eliminated` flag, a boolean `is_player`, and a `distance_from_ai` float), returns the `id` of the closest enemy kart (i.e., a kart with a different `team_id` than the AI's own team) that is not eliminated. The function should take as parameters the vector of karts, the AI's own kart id, and a boolean `consider_difficulty` that, when true, applies these rules: if the AI is on "easy" difficulty, skip player karts unless they are the only non-eliminated karts remaining; if the AI is on "best" difficulty, skip non-player (AI) karts entirely. If no enemy kart is found, return `-1`. The AI's own kart id is guaranteed to exist in the vector, and the vector is guaranteed to have at least one element.

The solution iterates through all karts, skipping the AI's own kart and any kart on the same team. For each remaining kart, if it is eliminated, skip it. If `consider_difficulty` is true and the difficulty is "easy", skip player karts only if there is at least one other non-player, non-eliminated enemy kart remaining (i.e., the total number of non-eliminated karts minus the number of non-eliminated players is greater than 1). If the difficulty is "best", skip AI karts entirely. For each candidate kart, compare its `distance_from_ai` with the current minimum; update the closest kart id and distance if a smaller distance is found. Edge cases: if all enemy karts are eliminated or filtered out, return `-1`; if multiple karts have the same distance, the first encountered (by vector index) is chosen. Complexity: \(O(n)\) time for a vector of \(n\) karts, and \(O(1)\) auxiliary space.

#include <vector>
#include <limits>

struct KartInfo {
    int id;
    int team_id;
    bool eliminated;
    bool is_player;
    float distance_from_ai;
};

// Find the closest enemy kart (different team) that is not eliminated.
// consider_difficulty applies filtering rules for easy and best difficulty.
// Returns the id of the closest enemy kart, or -1 if none found.
int findClosestEnemyKart(const std::vector<KartInfo>& karts, int ai_kart_id, int ai_team_id, const std::string& difficulty, bool consider_difficulty) {
    float min_distance = std::numeric_limits<float>::max();
    int closest_id = -1;
    int total_active = 0;
    int active_players = 0;

    // First count active karts (non-eliminated) and active players for easy difficulty logic.
    if (consider_difficulty && difficulty == "easy") {
        for (const auto& kart : karts) {
            if (!kart.eliminated) {
                total_active++;
                if (kart.is_player) active_players++;
            }
        }
        // If there are fewer than 2 active AI karts (i.e., only players or one AI plus players),
        // we need to skip the player-skipping rule. The condition in the original code is:
        // skip players if (total_active - active_players) > 1, meaning at least 2 active AI karts.
    }

    for (const auto& kart : karts) {
        // Skip the AI's own kart
        if (kart.id == ai_kart_id) continue;
        // Skip same team
        if (kart.team_id == ai_team_id) continue;
        // Skip eliminated karts
        if (kart.eliminated) continue;

        // Apply difficulty filtering
        if (consider_difficulty) {
            if (difficulty == "easy") {
                // Skip player karts only if there are at least 2 active AI karts (i.e., total_active - active_players > 1)
                if (kart.is_player && (total_active - active_players) > 1) continue;
            } else if (difficulty == "best") {
                // Skip AI karts (non-players)
                if (!kart.is_player) continue;
            }
        }

        // Update closest if this kart is nearer
        if (kart.distance_from_ai < min_distance) {
            min_distance = kart.distance_from_ai;
            closest_id = kart.id;
        }
    }

    return closest_id;
}

#include <cassert>
#include <string>
#include <vector>

// (KartInfo struct and function defined above)

int main() {
    // Karts: AI is id=0, team=1. Enemy team=2.
    std::vector<KartInfo> karts = {
        {0, 1, false, false, 0.0f}, // AI itself
        {1, 2, false, false, 5.0f}, // AI enemy, distance 5
        {2, 2, false, true, 3.0f},  // player enemy, distance 3
        {3, 2, true, false, 1.0f},  // eliminated enemy
        {4, 1, false, false, 2.0f}  // teammate
    };

    // Without difficulty filtering: closest enemy is id=2 (distance 3)
    assert(findClosestEnemyKart(karts, 0, 1, "medium", false) == 2);

    // Easy difficulty: skip player unless only they are left. Here we have 2 active non-player karts (AI itself and kart 1), so total_active=3 (karts 0,1,2) and active_players=2 (karts 1 and 2? Actually kart 1 is player enemy, kart 2 is AI enemy wait – kart 1 is not player, kart 2 is player). Let's fix: total_active = 3, active_players = 1. Then total_active - active_players = 2 > 1, so skip players → only kart 1 remains, distance 5.
    assert(findClosestEnemyKart(karts, 0, 1, "easy", true) == 1);

    // Best difficulty: skip non-player (AI) karts, so only player enemy kart 2 remains, distance 3.
    assert(findClosestEnemyKart(karts, 0, 1, "best", true) == 2);

    // All enemies eliminated → return -1
    std::vector<KartInfo> no_enemy = {
        {0, 1, false, false, 0.0f},
        {1, 2, true, false, 5.0f}
    };
    assert(findClosestEnemyKart(no_enemy, 0, 1, "medium", false) == -1);

    // Only the AI and a single player enemy, easy difficulty: total_active - active_players = 1, so do not skip player.
    std::vector<KartInfo> only_player = {
        {0, 1, false, false, 0.0f},
        {1, 2, false, true, 4.0f}
    };
    assert(findClosestEnemyKart(only_player, 0, 1, "easy", true) == 1);

    return 0;
}
