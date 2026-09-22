/*
You are given `n` rooms numbered from `0` to `n-1`. Initially, you are in room `0`, and all rooms are locked except room `0`. Each room `i` contains a list `rooms[i]` of keys (possibly empty) that allow you to open the corresponding locked room if you have that key. A key is an integer representing a room number, and having a key means you can enter that room (and then collect all keys inside it). Write a C++ function `bool canVisitAllRooms(const std::vector<std::vector<int>>& rooms)` that returns `true` if you can visit every room starting from room `0`, and `false` otherwise. You may collect keys in any order, and each key can be used once. The input is a non-empty vector where each `rooms[i]` is a vector of integers representing keys found in room `i`. Note that room numbers in the key lists are guaranteed to be valid (between `0` and `n-1`). You must implement the solution without modifying the input, and your function should work efficiently for up to, say, 1000 rooms.
*/

#include <vector>

// Determine whether all rooms can be visited starting from room 0.
// rooms[i] contains the keys (room indices) found in room i.
bool canVisitAllRooms(const std::vector<std::vector<int>>& rooms) {
    const int n = static_cast<int>(rooms.size());
    std::vector<bool> visited(n, false);

    // Depth-first search helper using an explicit stack to avoid recursion depth issues.
    std::vector<int> stack;
    stack.push_back(0);
    visited[0] = true;

    while (!stack.empty()) {
        int current = stack.back();
        stack.pop_back();

        for (int key : rooms[current]) {
            if (!visited[key]) {
                visited[key] = true;
                stack.push_back(key);
            }
        }
    }

    for (bool roomVisited : visited) {
        if (!roomVisited) {
            return false;
        }
    }
    return true;
}

#include <cassert>
#include <vector>

// Declaration from the solution (for clarity; in practice include the header).
bool canVisitAllRooms(const std::vector<std::vector<int>>& rooms);

int main() {
    // Example 1: all rooms reachable.
    std::vector<std::vector<int>> rooms1 = {{1}, {2}, {3}, {}};
    assert(canVisitAllRooms(rooms1) == true);

    // Example 2: room 2 locked forever.
    std::vector<std::vector<int>> rooms2 = {{1, 3}, {3, 0, 1}, {2}, {0}};
    assert(canVisitAllRooms(rooms2) == false);

    // Single room with no keys.
    std::vector<std::vector<int>> rooms3 = {{}};
    assert(canVisitAllRooms(rooms3) == true);

    // Starting room has no keys, but there are other rooms.
    std::vector<std::vector<int>> rooms4 = {{}, {0}};
    assert(canVisitAllRooms(rooms4) == false);

    // Cyclic reachable graph.
    std::vector<std::vector<int>> rooms5 = {{1}, {2}, {0}};
    assert(canVisitAllRooms(rooms5) == true);

    // Self-loop and isolated node.
    std::vector<std::vector<int>> rooms6 = {{0}, {2}, {1}};
    assert(canVisitAllRooms(rooms6) == true); // You already have room0's key (self), room1 gives key to room2, room2 gives key to room1.

    // Large disconnected component.
    std::vector<std::vector<int>> rooms7 = {{}, {}, {}};
    assert(canVisitAllRooms(rooms7) == false);

    // Keys that skip ahead.
    std::vector<std::vector<int>> rooms8 = {{2}, {}, {0, 1}};
    assert(canVisitAllRooms(rooms8) == true);

    // Keys not ordered.
    std::vector<std::vector<int>> rooms9 = {{3}, {0}, {1}, {2}};
    assert(canVisitAllRooms(rooms9) == true);

    // Empty top-level (though spec says non-empty, defensive test).
    std::vector<std::vector<int>> rooms10 = {};
    assert(canVisitAllRooms(rooms10) == true); // trivially true for 0 rooms.

    return 0;
}

// The problem reduces to a graph traversal: each room is a node, and each key in room `i` is a directed edge from `i` to that key’s room number. We need to determine if all nodes are reachable from the starting node `0`. The main algorithm is a standard Depth‑First Search (DFS) or Breadth‑First Search (BFS) from room `0`, marking visited rooms. After the traversal, if every room is marked as visited, return `true`; otherwise, return `false`. Important edge cases: (1) an empty key list in some room means no outgoing edges; (2) the starting room may have no keys, so if `n > 1` you cannot reach the others; (3) keys may lead back to already visited rooms (cycles), which must be handled by checking before recursing; (4) the input is guaranteed non‑empty, but you should still handle a single room by returning `true` immediately. Time complexity is `O(n + total_keys)` because every room and every key is processed once. Auxiliary space is `O(n)` for the visited array and recursion stack in the worst case (or explicit stack if BFS is used).
