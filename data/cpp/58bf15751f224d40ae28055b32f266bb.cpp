/*
You are given a simplified model of an engineer's "nest" in a game: a set of sentry hints and teleporter hints, each with a name, and an optional owning object that may be considered "abandoned" (no owner). Write a C++ function `std::vector<std::string> find_stale_nests(const std::vector<NestHint>& hints)` that takes a flat list of hints (each having a `name`, a `type` (`"sentry"` or `"teleporter"`), and a boolean `has_owner`), groups them by name into nests, and returns the names of all nests that are "stale". A nest is stale if it contains at least one sentry hint **and** that sentry hint has no owner, **or** it contains at least one teleporter hint and that teleporter hint has no owner. But there is an important rule: a nest is only considered stale if it has at least one hint of either type with no owner, **and** the nest itself has at least one hint of each type? No—re-read: In the original code, a nest is stale if **any** of its sentry hints or teleporter hints has no owner. So for this task, define: a nest (group of hints with the same name) is stale if there exists any hint in that group whose `has_owner` is `false`. If a name appears in more than one hint, they all belong to the same nest. If a nest has no hints at all (shouldn't happen, but ignore), it is not stale. Return the names in the order they first appear in the input list. If no stale nests, return an empty vector. The function must be `const`‑correct, take the input by `const std::vector<NestHint>&`, and may assume `NestHint` is a simple struct with `std::string name; std::string type; bool has_owner;`. Provide a reference implementation with no `main`.
*/

#include <string>
#include <vector>
#include <unordered_map>

struct NestHint {
    std::string name;
    std::string type;  // "sentry" or "teleporter" (not used directly in logic)
    bool has_owner;
};

// Return names of all nests that contain at least one hint with no owner,
// in the order the names first appear in the input list.
std::vector<std::string> find_stale_nests(const std::vector<NestHint>& hints) {
    std::unordered_map<std::string, bool> stale;
    std::vector<std::string> order;
    order.reserve(hints.size());
    stale.reserve(hints.size());

    for (const NestHint& hint : hints) {
        auto it = stale.find(hint.name);
        if (it == stale.end()) {
            stale.emplace(hint.name, false);
            order.push_back(hint.name);
        }
        if (!hint.has_owner) {
            stale[hint.name] = true;
        }
    }

    std::vector<std::string> result;
    for (const std::string& name : order) {
        if (stale.at(name)) {
            result.push_back(name);
        }
    }
    return result;
}

#include <cassert>
#include <string>
#include <vector>

// NestHint definition and function declaration appear here in a real test.
// For brevity, we assume they are included from the solution.

int main() {
    // Empty input yields empty output
    assert(find_stale_nests({}).empty());

    // Basic case: one stale nest, one fresh nest
    std::vector<NestHint> hints1 = {
        {"A", "sentry", true},
        {"B", "teleporter", false},
        {"A", "teleporter", true}
    };
    std::vector<std::string> res1 = find_stale_nests(hints1);
    assert(res1.size() == 1 && res1[0] == "B");

    // Same name with one hint lacking owner makes the whole nest stale
    std::vector<NestHint> hints2 = {
        {"X", "sentry", true},
        {"X", "teleporter", false},
        {"Y", "sentry", true}
    };
    std::vector<std::string> res2 = find_stale_nests(hints2);
    assert(res2.size() == 1 && res2[0] == "X");

    // Order of first appearance is preserved
    std::vector<NestHint> hints3 = {
        {"Z", "sentry", false},
        {"W", "teleporter", true},
        {"Z", "teleporter", true},
        {"W", "sentry", false}
    };
    std::vector<std::string> res3 = find_stale_nests(hints3);
    assert(res3.size() == 2 && res3[0] == "Z" && res3[1] == "W");

    // All hints have owners => no stale nests
    std::vector<NestHint> hints4 = {
        {"A", "sentry", true},
        {"A", "teleporter", true},
        {"B", "sentry", true}
    };
    assert(find_stale_nests(hints4).empty());

    // Multiple stale hints of same name should not duplicate the name
    std::vector<NestHint> hints5 = {
        {"N", "sentry", false},
        {"N", "teleporter", false}
    };
    std::vector<std::string> res5 = find_stale_nests(hints5);
    assert(res5.size() == 1 && res5[0] == "N");

    // Duplicate names with all owners but later a no‑owner hint
    std::vector<NestHint> hints6 = {
        {"P", "sentry", true},
        {"P", "teleporter", true},
        {"P", "sentry", false}
    };
    std::vector<std::string> res6 = find_stale_nests(hints6);
    assert(res6.size() == 1 && res6[0] == "P");
}

// The core algorithm involves grouping hints by their `name` while preserving the order of first appearance. The simplest approach is to iterate through the input vector and maintain a map from each name to a boolean indicating whether any hint with that name has `has_owner == false`. Also maintain an ordered list of unique names (in order of first occurrence) so we can return the stale names in that order. For each hint, if its name is not yet in the map, add it to the ordered list and set its flag to false. Then, if `!hint.has_owner`, set the flag to true for that name. After processing all hints, iterate over the ordered list and collect names whose flag is true. Edge cases: an empty input returns an empty vector; multiple hints with same name are handled by the flag; hints with `has_owner == true` do not affect staleness; duplicates in names do not cause double counting. Time complexity: O(n) for iterating through all hints plus O(u) for final collection where u is number of unique names, so O(n) overall. Space complexity: O(u) for the map and list, at worst O(n). No other special cases.
