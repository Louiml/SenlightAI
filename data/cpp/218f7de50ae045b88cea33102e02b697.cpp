Write a C++ function named `collectUniqueLevels` that processes a sequence of "events" represented as a vector of integers, where positive integers denote entering a new scope (pushing that integer onto a stack), zero denotes exiting the innermost scope (popping the stack), and negative integers denote a variable reference occurring at the current scope depth (the absolute value is the variable's unique ID), and returns the number of distinct scope levels at which at least one variable reference occurred. For example, given `{1, -10, 2, -20, 0, -30, 0, -40}`, scope levels are: level 0 (outside any scope) has variable 40, level 1 (after pushing 1) has variable 10, level 2 (after pushing 2) has variables 20 and 30, so the set of distinct levels with references is `{0,1,2}` → return 3. The input is guaranteed to be well-formed: every `0` corresponds to a matching prior push, stack never becomes empty on pop, and the vector is non-empty. The function must handle nested scopes and ignore pushes/pops that contain no variable references. It should only count the number of distinct levels (not the total references), and each level is an integer ≥ 0.
#include <cassert>
#include <vector>

int main() {
    // Basic single reference at root.
    assert(collectUniqueLevels({-1}) == 1);
    // Push and reference inside.
    assert(collectUniqueLevels({1, -2, 0}) == 1);
    // Two nested scopes, references at both.
    assert(collectUniqueLevels({1, -2, 2, -3, 0, 0}) == 2);
    // Reference at every level including root.
    assert(collectUniqueLevels({-10, 1, -20, 2, -30, 0, -40, 0}) == 3);
    // Duplicate references at same level only count once.
    assert(collectUniqueLevels({1, -5, -6, -7, 0}) == 1);
    // Multiple scopes at same depth? Depth increments, so each push gives unique depth.
    // Three separate scopes at depths 1, 2, 3.
    assert(collectUniqueLevels({1, -1, 0, 2, -2, 0, 3, -3, 0}) == 3);
    // Mixed: root ref, nested ref, then another root ref after pop.
    assert(collectUniqueLevels({-1, 1, -2, 0, -3}) == 2);
    // Empty scope with no refs.
    assert(collectUniqueLevels({1, 0}) == 0);
    // Deep nesting, refs only at bottom.
    assert(collectUniqueLevels({1, 2, 3, -99, 0, 0, 0}) == 1);
}
#include <vector>
#include <unordered_set>

// Count distinct scope levels where at least one variable reference (-id) occurs.
// Positive: push a scope, 0: pop a scope, negative: variable reference at current depth.
int collectUniqueLevels(const std::vector<int>& events) {
    int currentLevel = 0;
    std::unordered_set<int> levelsWithRefs;
    for (int event : events) {
        if (event > 0) {
            // Enter a new scope.
            ++currentLevel;
        } else if (event == 0) {
            // Exit current scope.
            --currentLevel;
        } else {
            // Negative number: variable reference at current scope.
            levelsWithRefs.insert(currentLevel);
        }
    }
    return static_cast<int>(levelsWithRefs.size());
}
// The solution mimics a stack-based tree traversal. We iterate through the events in order. Each positive integer is a push: we increment a `currentLevel` counter (starting at 0 for the root). Each zero is a pop: decrement `currentLevel`. Each negative integer is a variable reference: we record `currentLevel` into a `std::set<int>` (or similar unique container). After processing all events, the answer is the size of that set. 
//
// Important edge cases: references can occur at level 0 (before any push, after all pops). Nested scopes with same level numbers: since levels are absolute counts of active pushes, they are unique per depth. Duplicate references at same level are ignored due to the set. The stack depth is maintained by counting pushes/pops, not by an actual stack of values, since we only need the depth. 
//
// Time complexity: O(n) for iteration plus O(log d) per insertion into set, where d is number of distinct levels ≤ n, so O(n log n) worst-case; but since d ≤ n, it's fine. Space: O(n) for the set if all levels distinct (worst case), plus O(1) auxiliary. For a typical implementation using unordered_set, average O(1) per insertion, total O(n) time and O(d) space.
//
// The function should be `int collectUniqueLevels(const std::vector<int>& events)`.
