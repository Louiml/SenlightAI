// Write a standalone C++ function `computeStateClosure` that takes a universe of objects represented as integers, a set of initial states (each state is a `std::set<int>`), and a collection of transition rules (each rule is a struct with `std::set<int> lhs` and `std::set<int> rhs`). The function must compute and return the least fixed point of the transition system: starting from the given initial states, repeatedly apply every rule whose `lhs` is a subset of some current state, and add the union of the current state and the rule's `rhs` as a new state, if it was not already present. Continue until no new states are produced. The returned value is a `std::set<std::set<int>>` containing all reachable states (including the initial ones). Rules may be applied multiple times as states grow, and the fixed point must be deterministic regardless of rule order.

// The problem is essentially computing the closure of a set system under monotone set-extension rules. Each rule maps a set to a superset (the union of the input set and its `rhs`). The closure is the smallest family of sets containing the initial sets and closed under all rules. The standard approach is a work-list algorithm: maintain a set `result` of known states, and a queue or list of states that still need to be tested against all rules. For each state popped from the work list, for each rule, check if the rule's `lhs` is a subset of the state; if so, compute the union of the state and the rule's `rhs`. If that union is not already in `result`, insert it and push it to the work list. Because each rule is monotone (it only adds elements), once a state has been processed, applying the same rule to a superset will yield a superset of the previously generated state, but we still need to check all rules for each new state. The algorithm terminates because the universe is finite, so the total number of distinct states is at most `2^n` (in practice much smaller). Time complexity is `O(S * R * (worst-case size of states))` where `S` is the number of reachable states and `R` is the number of rules, since for each state we iterate all rules and perform subset checks and set unions. Space complexity is `O(S * average state size)` for storing all states. Edge cases include empty `lhs` (rule always applies), empty universe, initial states already containing the union, and duplicate states from different rule applications. The implementation must use `const` references for inputs and return by value a `std::set<std::set<int>>`. To make it deterministic, we can process rules in a fixed order (e.g., as given in a `std::vector`).

#include <set>
#include <vector>
#include <algorithm>

// A transition rule: if a state contains every element of lhs,
// then the state can be extended by adding all elements of rhs.
struct TransitionRule {
    std::set<int> lhs;
    std::set<int> rhs;
};

// Compute the least fixed point of the transition system.
// Initial states are given in `initialStates`.
// Returns all reachable states (including initial ones) as a set of sets.
std::set<std::set<int>> computeStateClosure(
    const std::set<std::set<int>>& initialStates,
    const std::vector<TransitionRule>& rules)
{
    std::set<std::set<int>> result;
    std::vector<std::set<int>> worklist;

    // Seed with initial states.
    for (const auto& state : initialStates) {
        if (result.insert(state).second) {
            worklist.push_back(state);
        }
    }

    // Process states until no new states are discovered.
    while (!worklist.empty()) {
        // Take the next state to expand.
        std::set<int> current = std::move(worklist.back());
        worklist.pop_back();

        // Try every rule against this state.
        for (const auto& rule : rules) {
            // Check if the rule's lhs is a subset of current.
            bool applicable = std::includes(
                current.begin(), current.end(),
                rule.lhs.begin(), rule.lhs.end());
            if (!applicable) continue;

            // Build the new state: current union rule.rhs.
            std::set<int> newState = current;
            newState.insert(rule.rhs.begin(), rule.rhs.end());

            // Add if not already present.
            if (result.insert(newState).second) {
                worklist.push_back(std::move(newState));
            }
        }
    }

    return result;
}

#include <cassert>
#include <set>
#include <vector>

// The function and struct from the solution are assumed to be included above.

int main() {
    // Test 1: Simple chain A -> B, B -> C.
    {
        std::set<std::set<int>> init = {{1}};
        std::vector<TransitionRule> rules = {
            {{1}, {2}},
            {{2}, {3}}
        };
        auto result = computeStateClosure(init, rules);
        std::set<std::set<int>> expected = {{1}, {1,2}, {1,2,3}};
        assert(result == expected);
    }

    // Test 2: Rule with empty lhs always applies.
    {
        std::set<std::set<int>> init = {{}};
        std::vector<TransitionRule> rules = {
            {{}, {5}}
        };
        auto result = computeStateClosure(init, rules);
        std::set<std::set<int>> expected = {{}, {5}};
        assert(result == expected);
    }

    // Test 3: No rules, only initial states.
    {
        std::set<std::set<int>> init = {{1,2}, {3}};
        std::vector<TransitionRule> rules;
        auto result = computeStateClosure(init, rules);
        assert(result == init);
    }

    // Test 4: Multiple rules can apply to the same state.
    {
        std::set<std::set<int>> init = {{1}};
        std::vector<TransitionRule> rules = {
            {{1}, {2}},
            {{1}, {3}},
            {{2,3}, {4}}
        };
        auto result = computeStateClosure(init, rules);
        std::set<std::set<int>> expected = {
            {1}, {1,2}, {1,3}, {1,2,3}, {1,2,3,4}
        };
        assert(result == expected);
    }

    // Test 5: Rule that requires two elements.
    {
        std::set<std::set<int>> init = {{1}, {2}};
        std::vector<TransitionRule> rules = {
            {{1,2}, {3}}
        };
        auto result = computeStateClosure(init, rules);
        // The rule cannot apply because no single state contains both 1 and 2.
        std::set<std::set<int>> expected = {{1}, {2}};
        assert(result == expected);
    }

    // Test 6: Cycles and duplicates.
    {
        std::set<std::set<int>> init = {{1}};
        std::vector<TransitionRule> rules = {
            {{1}, {2}},
            {{2}, {1,3}}
        };
        auto result = computeStateClosure(init, rules);
        std::set<std::set<int>> expected = {
            {1}, {1,2}, {1,2,3}
        };
        assert(result == expected);
    }

    // Test 7: Larger universe and many rules.
    {
        std::set<std::set<int>> init = {{0}};
        std::vector<TransitionRule> rules;
        for (int i = 0; i < 10; ++i) {
            rules.push_back({{i}, {i+1}});
        }
        auto result = computeStateClosure(init, rules);
        std::set<std::set<int>> expected;
        std::set<int> cur;
        for (int i = 0; i <= 10; ++i) {
            cur.insert(i);
            expected.insert(cur);
        }
        assert(result == expected);
    }

    return 0;
}
