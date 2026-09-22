// Design a C++ function that simulates the core behavior of the GOAP planner's `CurrentPlan` execution and action chaining, but operates on plain integers instead of actions. Given a vector of non-negative integers representing action costs (where `0` means the action is already complete), and a positive integer `speed` representing how much cost is reduced per update, write a function `simulatePlanExecution` that returns a vector of the number of update ticks each action required to finish (i.e., the iteration index at which it becomes complete), assuming actions are processed sequentially. When an action's remaining cost becomes zero or negative after an update, it is considered complete and the next action begins on the next update tick. If all actions complete, the function stops. The returned vector must have the same length as the input, with each entry being the 1-based tick number when that action completed. For example, with costs `{10, 5}` and speed `3`, action 0 needs 4 ticks (10→7→4→1→negative), action 1 starts on tick 5 and needs 2 ticks (5→2→negative), so returns `{4, 6}`. If an action has cost 0, it completes on the current tick (before any update). The function must handle empty input and return an empty vector.

#include <cassert>
#include <vector>

// The solution function is declared here for completeness.
std::vector<int> simulatePlanExecution(const std::vector<int>& costs, int speed);

int main() {
    // Basic sequential case.
    assert(simulatePlanExecution({10, 5}, 3) == std::vector<int>({4, 6}));

    // Zero-cost action completes immediately.
    assert(simulatePlanExecution({0, 3}, 1) == std::vector<int>({1, 4}));

    // Single action that needs multiple ticks.
    assert(simulatePlanExecution({7}, 2) == std::vector<int>({4}));

    // Empty input returns empty.
    assert(simulatePlanExecution({}, 5) == std::vector<int>());

    // Multiple zero-cost actions all complete on tick 1.
    assert(simulatePlanExecution({0, 0, 0}, 10) == std::vector<int>({1, 1, 1}));

    // Large costs and speed=1.
    assert(simulatePlanExecution({2, 1}, 1) == std::vector<int>({2, 3}));

    // Speed larger than cost, action completes in one tick.
    assert(simulatePlanExecution({1, 10}, 5) == std::vector<int>({1, 3}));
}

#include <vector>
#include <cstddef>

// Simulate sequential action execution with cost reduction per tick.
// Returns the 1-based tick index when each action completes.
std::vector<int> simulatePlanExecution(const std::vector<int>& costs, int speed) {
    std::vector<int> completionTicks;
    if (costs.empty()) {
        return completionTicks;
    }

    completionTicks.reserve(costs.size());
    int currentTick = 1;
    std::size_t actionIndex = 0;
    int remainingCost = costs[actionIndex];

    while (actionIndex < costs.size()) {
        // If the current action is already complete (cost <= 0), record and move on.
        if (remainingCost <= 0) {
            completionTicks.push_back(currentTick);
            ++actionIndex;
            if (actionIndex < costs.size()) {
                remainingCost = costs[actionIndex];
                // Note: we do not increment currentTick here, because the next action starts on the same tick.
                // But if the next action has cost 0, it will be handled in the next loop iteration.
                continue;
            } else {
                break;
            }
        }

        // Apply the update: reduce remaining cost by speed.
        remainingCost -= speed;

        // If the action is now complete after this update, record the tick.
        if (remainingCost <= 0) {
            completionTicks.push_back(currentTick);
            ++actionIndex;
            if (actionIndex < costs.size()) {
                remainingCost = costs[actionIndex];
            } else {
                break;
            }
        }

        // Move to the next tick unless we've finished all actions.
        if (actionIndex < costs.size()) {
            ++currentTick;
        }
    }

    return completionTicks;
}

// The main algorithm is a sequential simulation. We track the current tick counter (starting at 1), the current action index, and the remaining cost of the current action. For each tick, we first check if the current action's remaining cost is already <= 0. If so, we record the current tick for that action, move to the next action, and if no more actions exist we stop. Otherwise (if remaining cost > 0), we reduce the remaining cost by `speed`. After the reduction, if the remaining cost becomes <= 0, we record the current tick for that action and advance to the next action (if any). We continue until all actions are processed. Important edge cases: an action with cost 0 should complete immediately at the start of its turn without decrementing; an empty input returns an empty vector; the speed is positive so no risk of infinite loop; if an action has a very large cost, the loop will run many ticks, but that's linear in the total number of ticks. Time complexity is O(total_ticks) where total_ticks is the sum over actions of ceil(cost / speed) (with special handling for zero-cost actions). Space complexity is O(n) for the output vector, where n is the number of actions.
