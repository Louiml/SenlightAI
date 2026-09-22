// Write a C++ function `bool isReachableFromStartToEnd(const std::vector<int>& positions, int startIndex, int targetValue)` that, given a 1-indexed array `positions` where `positions[i]` represents the next index to jump to from position `i` (using 1-based indexing), and given a starting index `startIndex` and a target value `targetValue`, determines whether starting from position `startIndex` and repeatedly jumping to `positions[currentIndex]` you will eventually land on an index whose value equals `targetValue`. The array length is `n` (with positions in range `[1, n]`), and indices are 1-based. Return `true` if reachable, `false` otherwise. You may assume the input is valid (positions vector size is at least 1, startIndex is between 1 and n, and targetValue appears at least once in the array). Handle cycles gracefully (if stuck in a cycle without reaching target, return `false`). Example: positions = {2, 3, 1}, startIndex = 1, targetValue = 1 → starting at index 1 (value 2), jump to index 2 (value 3), jump to index 3 (value 1) → reachable, so return true. But if targetValue = 4 and it doesn't exist, return false.
The problem resembles following a functional graph where each node has exactly one outgoing edge (to another valid index). Starting from the given `startIndex`, we simulate the jumps. However, note that the target is a value, not an index. The key is to recognize that as we jump, we check the value at the current index. If that value equals `targetValue`, we return `true`. Since the graph is finite and each step moves to a valid index, the path will eventually either hit the target value or enter a cycle. To avoid infinite loops, we track visited indices (or use a boolean visited array of size n+1, where index 0 is unused). Once we revisit an index, we are in a cycle and since we have not found the target yet, we return `false`. The function signature uses 1-based indexing for `startIndex` and positions vector (where positions[0] is ignored). Edge cases: if `startIndex` itself holds the target value, return true immediately. If the target value is not present anywhere, the function will still simulate but will eventually cycle and return false; that's fine but could be optimized with a pre-check, but not necessary. Time complexity is O(n) because we visit at most n distinct nodes before either finding target or hitting a cycle. Space complexity O(n) for the visited array (or O(1) if we modify input, but we prefer const-correctness so O(n) auxiliary).
#include <vector>
#include <unordered_set>

// Determines whether starting from a 1-based index 'startIndex' and repeatedly
// following positions[current] (values are 1-based indices), we ever land on a
// node whose value equals 'targetValue'. Returns true if reachable, false otherwise.
bool isReachableFromStartToEnd(const std::vector<int>& positions, int startIndex, int targetValue) {
    int n = static_cast<int>(positions.size()) - 1; // positions[0] is unused
    std::unordered_set<int> visited;
    int current = startIndex;
    
    while (current >= 1 && current <= n && visited.find(current) == visited.end()) {
        if (positions[current] == targetValue) {
            return true;
        }
        visited.insert(current);
        current = positions[current];
    }
    return false;
}
#include <cassert>
#include <vector>

int main() {
    // Simple chain: 1->2->3->1, target at end
    std::vector<int> pos1 = {0, 2, 3, 1};
    assert(isReachableFromStartToEnd(pos1, 1, 1) == true);
    assert(isReachableFromStartToEnd(pos1, 1, 3) == true);
    assert(isReachableFromStartToEnd(pos1, 2, 3) == true);
    assert(isReachableFromStartToEnd(pos1, 1, 4) == false); // target not present
    
    // Self-loop with target at start
    std::vector<int> pos2 = {0, 1, 1};
    assert(isReachableFromStartToEnd(pos2, 1, 1) == true);
    assert(isReachableFromStartToEnd(pos2, 2, 1) == true); // jump to 1 then find 1
    assert(isReachableFromStartToEnd(pos2, 2, 2) == false); // never reaches value 2
    
    // Larger cycle, target inside
    std::vector<int> pos3 = {0, 2, 3, 4, 2};
    assert(isReachableFromStartToEnd(pos3, 1, 2) == true); // at index 1 value is 2
    assert(isReachableFromStartToEnd(pos3, 3, 3) == true); // at index 3 value is 4? wait check
    // Let's recalc: pos3[3]=4, so starting at 3, current=3, value=4, then jump to 4, value=2, jump to 2, value=3 -> next jump to 3 (cycle). target=3 is not immediate at start, but later we hit index 2 whose value is 3? Actually when current=2, positions[2]=3, equals target=3, so true.
    assert(isReachableFromStartToEnd(pos3, 3, 3) == true);
    assert(isReachableFromStartToEnd(pos3, 3, 2) == true); // eventually hit index 4 value=2
    
    // Single node
    std::vector<int> pos4 = {0, 1};
    assert(isReachableFromStartToEnd(pos4, 1, 1) == true);
    assert(isReachableFromStartToEnd(pos4, 1, 0) == false);
    
    // Target not present but cycles
    std::vector<int> pos5 = {0, 2, 1};
    assert(isReachableFromStartToEnd(pos5, 1, 5) == false);
}
