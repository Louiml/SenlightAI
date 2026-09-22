// Write a C++ function `size_t count_allowed_steps(const std::vector<int>& transitions, int start_state)` that simulates a deterministic finite automaton with states represented as integers and transitions stored in a flat vector. The vector `transitions` encodes rules as triples `(from_state, symbol, to_state)` one after another; the automaton accepts any integer symbol (0 or 1). The automaton starts in `start_state`, and the function must repeatedly apply the transition for symbol `1` (ignoring symbol `0` entirely) until either no transition exists for the current state with symbol `1`, or a state repeats. The function returns the total number of successful `make_step('1')` operations performed before stopping (i.e., the length of the walk using symbol 1). If a repeat occurs, stop and return the count at the moment the repeated state is about to be revisited (do not include that extra step).

The problem reduces to simulating a directed graph where each state has at most one outgoing edge labeled `1`, because the transition table is deterministic for a given `(state, symbol)` pair. We start at `start_state` and follow the `1`-edges. Track visited states using a `std::unordered_set<int>` or a `std::vector<bool>` (if state IDs are small). At each step, look up the transition for `(current_state, 1)`. If not found, stop and return the count. If found, check if the destination state has already been visited; if yes, stop (without incrementing) and return the current count. Otherwise, move to that state, increment count, and continue. Important edge cases: an empty `transitions` vector (no rules) gives 0; a self-loop leads to immediate repeat; a chain that ends with no `1` transition stops at the last state; a chain that eventually loops back to an earlier state stops before repeating. The lookup can be done by scanning the vector each time, giving `O(T * K)` worst-case where `T` is number of steps and `K` is number of transitions, but we can precompute a map from `(state,1)` to next state to make each step `O(1)`. Since we only care about symbol `1`, precompute a `std::unordered_map<int,int>` for symbol `1` only. The simulation runs at most `V` steps where `V` is the number of distinct states visited, because a repeat halts. Space is `O(V)` for visited set plus `O(K)` for the map. If the transition vector is malformed (odd length), we can ignore extra items or assume well-formed; for safety, we can require length divisible by 3 or just process full triples.

#include <vector>
#include <unordered_map>
#include <unordered_set>

// Simulate a deterministic finite automaton following only symbol '1' transitions.
// transitions encodes triples (from, symbol, to). Returns the number of successful
// '1'-steps before halting (due to missing transition or repeated state).
size_t count_allowed_steps(const std::vector<int>& transitions, int start_state) {
    // Build a map: for each (from_state, symbol=1) -> to_state.
    std::unordered_map<int, std::unordered_map<int,int>> rules;
    for (size_t i = 0; i + 2 < transitions.size(); i += 3) {
        int from = transitions[i];
        int symbol = transitions[i+1];
        int to = transitions[i+2];
        rules[from][symbol] = to;
    }

    std::unordered_set<int> visited;
    int current = start_state;
    size_t step_count = 0;

    while (true) {
        // Check if we've already visited this state? Actually we check before moving.
        // We start with start_state not yet visited; we add it when first entered.
        if (visited.count(current)) {
            // We are about to revisit a state — stop.
            break;
        }
        visited.insert(current);

        // Find transition for symbol 1 from current state.
        auto it_state = rules.find(current);
        if (it_state == rules.end()) {
            break; // no rules for this state at all
        }
        auto it_sym = it_state->second.find(1);
        if (it_sym == it_state->second.end()) {
            break; // no transition for symbol 1
        }

        int next = it_sym->second;
        // If next is already visited, we would move if not for the check; stop before moving.
        if (visited.count(next)) {
            break;
        }
        // Move to next.
        current = next;
        ++step_count;
    }
    return step_count;
}

#include <cassert>
#include <vector>

int main() {
    // No rules: any start gives 0.
    std::vector<int> empty;
    assert(count_allowed_steps(empty, 0) == 0);
    assert(count_allowed_steps(empty, 5) == 0);

    // Single self-loop: (0,1)->0. First step from 0 to 0 would repeat, but we stop before moving.
    std::vector<int> self_loop = {0,1,0};
    assert(count_allowed_steps(self_loop, 0) == 0);

    // Two-state cycle: 0--1-->1 and 1--1-->0. Starting at 0: 0->1 (step1), then 1->0 would revisit 0, stop => 1.
    std::vector<int> cycle = {0,1,1, 1,1,0};
    assert(count_allowed_steps(cycle, 0) == 1);

    // Chain ending in a dead end: 0->1->2, and no transition from 2 with 1. Steps = 2.
    std::vector<int> chain = {0,1,1, 1,1,2};
    assert(count_allowed_steps(chain, 0) == 2);

    // Chain with a repeated state later: 0->1, 1->2, 2->1. Starting at 0: 0->1 (1), 1->2 (2), 2->1 would revisit 1, stop => 2.
    std::vector<int> repeat_chain = {0,1,1, 1,1,2, 2,1,1};
    assert(count_allowed_steps(repeat_chain, 0) == 2);

    // Start not in any rule: no transition, count 0.
    std::vector<int> unrelated = {10,1,11};
    assert(count_allowed_steps(unrelated, 0) == 0);

    // Mixed symbols: ignore symbol 0 completely.
    std::vector<int> mixed = {0,0,99, 0,1,1, 1,0,98, 1,1,2};
    // Only symbol 1 matters: 0->1 (step1), 1->2 (step2), 2 has no 1-transition => 2.
    assert(count_allowed_steps(mixed, 0) == 2);

    // Long path with no repeat.
    std::vector<int> long_path;
    for (int i = 0; i < 100; ++i) {
        long_path.push_back(i);
        long_path.push_back(1);
        long_path.push_back(i+1);
    }
    // Starting at 0, with transitions 0->1,1->2,...,99->100; 100 has no outgoing -> 100 steps.
    assert(count_allowed_steps(long_path, 0) == 100);

    // Start at 50 in the same long path: 50->51 ... 100 => 50 steps.
    assert(count_allowed_steps(long_path, 50) == 50);
}
