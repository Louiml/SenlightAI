// Write a C++ function named `buildShortestPathStack` that takes an array of `STACK` nodes (where index 0 is the head), an array of `VERTEX` structures, and a target vertex index `t` (1-based). The `VERTEX` structure contains fields `pi` (the predecessor vertex index, 1-based, or 0 for none) and `key` (a float, where `FLT_MAX` indicates unreachable). The function should reconstruct the shortest path from the implicit source to vertex `t` by following `pi` pointers backward from `t`, pushing each visited vertex (1-based) onto the stack (linked list) in reverse order so that the resulting linked list starting from `S[0].next` represents the path from source to `t`. If the target is unreachable (`pi == 0` and `key == FLT_MAX`), the function should return 1 and leave the stack unchanged. On success, return 0. The function must handle the case where the target is the source itself (termination condition when `pi == 0` but `key` is not `FLT_MAX`), and must avoid infinite loops if the `pi` chain contains a cycle (e.g., by limiting iterations to the number of vertices, or by detecting a repeated index).
#include <cassert>
#include <cfloat>
#include <cstddef>

// Structure definitions.
struct STACK {
    int vertex;
    STACK* next;
};

struct VERTEX {
    int pi;
    float key;
};

// Function under test (declared here, but in the real solution it's defined above).
int buildShortestPathStack(STACK S[], VERTEX V[], int t, int numVertices);

int main() {
    // Test 1: Simple path: 1 -> 2 -> 3, target = 3.
    {
        VERTEX V[3] = {{0, 0.0f}, {1, 5.0f}, {2, 10.0f}}; // V[0]: source, V[1]: pred 1, V[2]: pred 2
        STACK S[1] = {{0, NULL}};
        int result = buildShortestPathStack(S, V, 3, 3);
        assert(result == 0);
        // Expected path: 1 -> 2 -> 3
        assert(S[0].next != NULL && S[0].next->vertex == 1);
        assert(S[0].next->next != NULL && S[0].next->next->vertex == 2);
        assert(S[0].next->next->next != NULL && S[0].next->next->next->vertex == 3);
        assert(S[0].next->next->next->next == NULL);
    }

    // Test 2: Target is source itself.
    {
        VERTEX V[1] = {{0, 0.0f}};
        STACK S[1] = {{0, NULL}};
        int result = buildShortestPathStack(S, V, 1, 1);
        assert(result == 0);
        assert(S[0].next != NULL && S[0].next->vertex == 1);
        assert(S[0].next->next == NULL);
    }

    // Test 3: Unreachable target (pi = 0, key = FLT_MAX).
    {
        VERTEX V[2] = {{0, 0.0f}, {0, FLT_MAX}};
        STACK S[1] = {{0, NULL}};
        int result = buildShortestPathStack(S, V, 2, 2);
        assert(result == 1);
        assert(S[0].next == NULL); // stack unchanged
    }

    // Test 4: Longer path with guard: 4 vertices, target = 4.
    {
        VERTEX V[4] = {{0, 0.0f}, {1, 2.0f}, {2, 4.0f}, {3, 6.0f}};
        STACK S[1] = {{0, NULL}};
        int result = buildShortestPathStack(S, V, 4, 4);
        assert(result == 0);
        int expected[] = {1, 2, 3, 4};
        STACK* cur = S[0].next;
        for (int i = 0; i < 4; ++i) {
            assert(cur != NULL && cur->vertex == expected[i]);
            cur = cur->next;
        }
        assert(cur == NULL);
    }

    // Test 5: Cycle detection: pi chain of length > numVertices won't happen, but we test with a cycle (1->2, 2->1) for target=2 with numVertices=2.
    {
        VERTEX V[2] = {{2, 1.0f}, {1, 2.0f}}; // V[0] pred 2, V[1] pred 1 -> cycle
        STACK S[1] = {{0, NULL}};
        int result = buildShortestPathStack(S, V, 2, 2);
        assert(result == 1); // should fail due to cycle guard
        assert(S[0].next == NULL); // or contains some nodes, but function returns 1; we only care about return code.
    }

    return 0;
}
#include <cfloat>   // for FLT_MAX
#include <cstddef>  // for NULL

// Assume these structures are defined externally as per the snippet.
// For self-containedness, we redefine them here with the same fields.
struct STACK {
    int vertex;          // 1-based vertex number
    STACK* next;
};

struct VERTEX {
    int pi;              // predecessor vertex index (1-based), or 0 if none
    float key;           // distance value; FLT_MAX means unreachable
};

// Reconstruct the shortest path from the implicit source to target t (1-based).
// Pushes vertices onto the linked list S (S[0] is the head). Returns 0 on success,
// 1 if t is unreachable or the pi chain is invalid (cycle).
int buildShortestPathStack(STACK S[], VERTEX V[], int t, int numVertices) {
    // Check for unreachable target.
    if (V[t-1].pi == 0 && V[t-1].key == FLT_MAX) {
        return 1;
    }

    int current = t;  // 1-based index
    int guard = 0;    // limit iterations to numVertices to avoid infinite loops.

    // Follow the predecessor chain until we hit pi == 0 (which is the source).
    // Note: The source itself may have pi == 0, so we push it as well.
    while (true) {
        // Push current vertex onto the front of the stack.
        STACK* node = new STACK();
        node->vertex = current;
        node->next = S[0].next;
        S[0].next = node;

        // If we've reached a vertex with no predecessor (source), stop.
        if (V[current-1].pi == 0) {
            break;
        }

        // Move to the predecessor.
        current = V[current-1].pi;

        // Guard against cycles: if we've processed more than numVertices, fail.
        if (++guard > numVertices) {
            return 1;  // Corrupted cycle in pi chain.
        }
    }

    return 0;
}
// The solution mirrors typical path reconstruction in Dijkstra's or BFS. Start from the target vertex `t` (1-based). Check if it is unreachable: if `V[t-1].pi == 0` and `V[t-1].key == FLT_MAX`, return 1. Otherwise, traverse backward using `pi` pointers. Since `pi` stores the predecessor's vertex index (1-based) or 0 for none (which means source or root), we can follow until `pi == 0`. However, there is a subtlety: if the source itself has `pi == 0` but `key` not `FLT_MAX`, we still need to include the source in the path. To avoid cycles, we can use an iteration counter set to the total number of vertices (or a visited array) — if we exceed it, return 1 (indicating a corrupted input). As we backtrack, we push each vertex onto the front of the linked list (i.e., insert at the head `S[0].next`), so that the final list is in forward order from source to target. The function returns 0 on success. Time complexity is O(number of vertices on the path), which is at most O(n) where n is the total number of vertices. Space complexity is O(path length) for the new stack nodes, plus O(1) auxiliary. Edge cases include: target is source (just push source), unreachable target (return 1), and cyclic `pi` chain (guard with iteration limit).
