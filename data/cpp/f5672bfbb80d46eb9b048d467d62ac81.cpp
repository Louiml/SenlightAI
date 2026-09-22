/*
Write a C++ function named `could_agent_win` that takes four integers: `age` (the agent's age), `health` (current health points), `armor` (current armor points), and `has_medkit` (1 if the agent has a medkit, 0 otherwise). The function should determine whether the agent survives an encounter based on the following rules: If the agent is 18 or younger, they use the medkit if available (reducing armor by 10), and survive if the final armor is at least 10. If the agent is between 19 and 50, they use the medkit if available (reducing armor by 10), and survive if the final armor is at least 14. If the agent is older than 50, they use the medkit if available (reducing armor by 10), and also rely on a secondary shield that costs 14 health points, and survive if the final armor (after medkit reduction) is at least 21. The function should return `true` if the agent survives, `false` otherwise. All inputs are non‑negative. Note that the medkit reduces armor by 10 but does not affect health; the secondary shield directly reduces health by 14.
*/

#include <algorithm> // for std::min, but not needed here; kept for potential future use

// Determines if an agent survives based on age, health, armor, and medkit presence.
// Returns true if the agent survives, false otherwise.
bool could_agent_win(int age, int health, int armor, int has_medkit) {
    // Apply medkit effect: reduces armor by 10 if available.
    if (has_medkit) {
        armor -= 10;
    }

    // Age-based survival thresholds.
    if (age <= 18) {
        return armor >= 10;
    } else if (age <= 50) {
        return armor >= 14;
    } else {
        // For agents over 50, secondary shield costs 14 health.
        // Health is reduced but only armor threshold matters for survival.
        health -= 14; // health is not used further, but kept for completeness
        return armor >= 21;
    }
}

#include <assert.h>

int main() {
    // Age <= 18
    assert(could_agent_win(10, 100, 15, 0) == true);  // no medkit, armor 15 >= 10
    assert(could_agent_win(18, 100, 9, 0) == false);  // no medkit, armor 9 < 10
    assert(could_agent_win(17, 100, 20, 1) == true);  // medkit reduces to 10, passes
    assert(could_agent_win(15, 100, 19, 1) == false); // medkit reduces to 9, fails

    // 19 <= age <= 50
    assert(could_agent_win(30, 100, 14, 0) == true);  // no medkit, armor 14 >= 14
    assert(could_agent_win(50, 100, 13, 0) == false); // no medkit, armor 13 < 14
    assert(could_agent_win(25, 100, 24, 1) == true);  // medkit reduces to 14, passes
    assert(could_agent_win(40, 100, 23, 1) == false); // medkit reduces to 13, fails

    // age > 50
    assert(could_agent_win(60, 100, 21, 0) == true);  // no medkit, armor 21 >= 21
    assert(could_agent_win(70, 100, 20, 0) == false); // no medkit, armor 20 < 21
    assert(could_agent_win(80, 100, 31, 1) == true);  // medkit reduces to 21, passes
    assert(could_agent_win(90, 100, 30, 1) == false); // medkit reduces to 20, fails

    return 0;
}

// The solution is a straightforward conditional logic problem. The main steps: (1) Decide which age bracket applies. (2) Apply the medkit effect if `has_medkit` is 1, subtracting 10 from armor. (3) For the over‑50 bracket, additionally subtract 14 from health, but only if the secondary shield is used—here it's always used as per the rules. (4) Compare the resulting armor value against the threshold for that age bracket and return `true` if armor ≥ threshold. Edge cases: age exactly 18 goes to the first bracket; age exactly 50 goes to the second bracket; older than 50 goes to the third. Health can be reduced to zero or negative, but the rules only require armor threshold for survival, so we do not check health after reduction. Armor can become negative after medkit subtraction; the comparison still works because we compare against a positive threshold. Time complexity is O(1), space complexity O(1).
