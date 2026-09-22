// You are given a simplified model of a container yard with multiple stacks. Each stack is identified by a pair `(bay, stack)` and can hold a maximum of `MAX_HEIGHT` containers. You must write a C++ function that, given a `ContainerYard` object with methods `GetNumberOfBays()`, `GetNumberOfStacks()`, `GetStackHeight(Location)`, `GetTopContainerId(Location)`, `GetContainerId(Location, height)`, `HasMaximumHeight(Location)`, `Relocate(from, to)`, `Retrieve(location)`, `Empty()`, `GetNextContainerId()`, and `GetNumberOfMoves()`, simulates the "unrestricted rollout heuristic" that retrieves containers in the order they appear at the front of the queue (starting from container ID 1 up to N). The algorithm: while the yard is not empty, take the next container ID that must be retrieved. If it is not on top of its current stack, consider every possible empty or non-full stack (excluding the source stack) as a candidate destination. For each candidate, simulate moving the blocking containers out of the way (using a priority-based relocation rule: if the top container of the destination stack is smaller than the top of the source stack, look for an alternative stack whose *lowest* container ID is greater than the source's top container ID, and preferably with the smallest such lowest ID; if none found, stop relocating). Then relocate the source's top container to the candidate, run a simple heuristic that recursively (but in simulation) retrieves all remaining containers using the same unrestricted rollout method, and count the total moves. Choose the candidate that minimizes the total moves. After selecting the best candidate, apply the same blocking relocation steps to the actual yard, then relocate the target container to that candidate, and finally retrieve the target. Repeat until all containers are retrieved. Implement a function `void rolloutUnrestrictedSolve(ContainerYard& yard)` that performs this algorithm. Assume `Location` is a struct with `int bay, stack` and supports `==` and default constructor setting `(-1,-1)`. The container IDs are positive integers starting from 1. Your solution must not modify the original algorithm's logic; only translate the given snippet into a clean, independent function. The `ContainerYard` class will be provided externally, but you may assume its interface as described.

#include <cassert>
#include <vector>

// Minimal mock ContainerYard for testing (real class is provided separately).
class ContainerYard {
public:
    int bays, stacks, maxHeight;
    std::vector<std::vector<int>> grid;
    std::vector<int> toRetrieve;
    int moves;

    ContainerYard(int b, int s, int mh) : bays(b), stacks(s), maxHeight(mh), grid(b*s, std::vector<int>()), moves(0) {}

    int GetNumberOfBays() const { return bays; }
    int GetNumberOfStacks() const { return stacks; }
    int GetStackHeight(const Location& loc) const { return grid[loc.bay * stacks + loc.stack].size(); }
    int GetTopContainerId(const Location& loc) const { return grid[loc.bay * stacks + loc.stack].back(); }
    int GetContainerId(const Location& loc, int height) const { return grid[loc.bay * stacks + loc.stack][height]; }
    bool HasMaximumHeight(const Location& loc) const { return GetStackHeight(loc) >= maxHeight; }
    void Relocate(const Location& from, const Location& to) {
        int cid = grid[from.bay * stacks + from.stack].back();
        grid[from.bay * stacks + from.stack].pop_back();
        grid[to.bay * stacks + to.stack].push_back(cid);
        ++moves;
    }
    void Retrieve(const Location& loc) {
        grid[loc.bay * stacks + loc.stack].pop_back();
        ++moves;
    }
    bool Empty() const {
        for (auto& s : grid) if (!s.empty()) return false;
        return true;
    }
    int GetNextContainerId() const {
        static int next = 1;
        return next;
    }
    int GetNumberOfMoves() const { return moves; }
    ContainerYard(const ContainerYard& other) : bays(other.bays), stacks(other.stacks), maxHeight(other.maxHeight), grid(other.grid), moves(other.moves) {}
};

// Test helper to set initial yard
void setInitial(ContainerYard& yard, const std::vector<std::vector<int>>& stacks) {
    yard.grid.clear();
    for (auto& s : stacks) {
        yard.grid.push_back(s);
    }
}

int main() {
    // Test 1: Single container, already on top
    ContainerYard y1(1, 1, 10);
    setInitial(y1, {{1}});
    rolloutUnrestrictedSolve(y1);
    assert(y1.Empty());
    assert(y1.GetNumberOfMoves() == 1);

    // Test 2: Two containers in same stack, retrieve order 1 then 2
    ContainerYard y2(1, 2, 10);
    setInitial(y2, {{2,1}, {}}); // stack0 has 2 on bottom, 1 on top
    rolloutUnrestrictedSolve(y2);
    assert(y2.Empty());

    // Test 3: Simple blocking case
    ContainerYard y3(1, 2, 10);
    setInitial(y3, {{1,2}, {}}); // need to retrieve 1 first, but 2 is on top
    rolloutUnrestrictedSolve(y3);
    assert(y3.Empty());

    // Test 4: All in one stack in reverse order
    ContainerYard y4(1, 2, 10);
    setInitial(y4, {{3,2,1}, {}});
    rolloutUnrestrictedSolve(y4);
    assert(y4.Empty());

    // Test 5: Multiple stacks
    ContainerYard y5(2, 2, 10);
    setInitial(y5, {{2}, {1}, {}, {3}});
    rolloutUnrestrictedSolve(y5);
    assert(y5.Empty());

    // Test 6: Empty yard
    ContainerYard y6(1, 1, 10);
    setInitial(y6, {{}});
    rolloutUnrestrictedSolve(y6);
    assert(y6.Empty());

    return 0;
}

#include <climits>
#include <memory>
#include <vector>

struct Location {
    int bay;
    int stack;
    Location() : bay(-1), stack(-1) {}
    Location(int b, int s) : bay(b), stack(s) {}
    bool operator==(const Location& other) const {
        return bay == other.bay && stack == other.stack;
    }
    bool operator!=(const Location& other) const {
        return !(*this == other);
    }
};

// Forward declaration for the class that will be provided externally.
class ContainerYard {
public:
    int GetNumberOfBays() const;
    int GetNumberOfStacks() const;
    int GetStackHeight(const Location& loc) const;
    int GetTopContainerId(const Location& loc) const;
    int GetContainerId(const Location& loc, int height) const;
    bool HasMaximumHeight(const Location& loc) const;
    void Relocate(const Location& from, const Location& to);
    void Retrieve(const Location& loc);
    bool Empty() const;
    int GetNextContainerId() const;
    int GetNumberOfMoves() const;
    ContainerYard(const ContainerYard& other);
};

// Helper to perform the blocking container clearing step.
void clearBlockingContainers(ContainerYard& yard, const Location& source, const Location& destination) {
    const int bays = yard.GetNumberOfBays();
    const int stacks = yard.GetNumberOfStacks();
    while (yard.GetStackHeight(destination) != 0 &&
           yard.GetTopContainerId(destination) < yard.GetTopContainerId(source)) {
        Location selectedLow(-1, -1);
        int lowestIndex = INT_MAX;
        for (int bay = 0; bay < bays; ++bay) {
            for (int stack = 0; stack < stacks; ++stack) {
                Location loc(bay, stack);
                if (!yard.HasMaximumHeight(loc) && loc != source && loc != destination) {
                    int stackLowest = INT_MAX;
                    for (int h = 0; h < yard.GetStackHeight(loc); ++h) {
                        int cid = yard.GetContainerId(loc, h);
                        if (stackLowest > cid) stackLowest = cid;
                    }
                    if (stackLowest > yard.GetTopContainerId(destination)) {
                        if (lowestIndex > stackLowest) {
                            lowestIndex = stackLowest;
                            selectedLow = loc;
                        } else if (stackLowest == INT_MAX) {
                            lowestIndex = 0;
                            selectedLow = loc;
                        }
                    }
                }
            }
        }
        if (selectedLow != Location(-1, -1)) {
            yard.Relocate(destination, selectedLow);
        } else {
            break;
        }
    }
}

// Recursive rollout simulation: solves a copy of the yard and returns total moves.
int simulateAndSolve(ContainerYard& yard) {
    const int bays = yard.GetNumberOfBays();
    const int stacks = yard.GetNumberOfStacks();
    while (!yard.Empty()) {
        const int targetId = yard.GetNextContainerId();
        Location targetLoc(-1, -1);
        // Find the location of targetId (simplified: search all stacks).
        for (int bay = 0; bay < bays; ++bay) {
            for (int stack = 0; stack < stacks; ++stack) {
                Location loc(bay, stack);
                for (int h = 0; h < yard.GetStackHeight(loc); ++h) {
                    if (yard.GetContainerId(loc, h) == targetId) {
                        targetLoc = loc;
                        break;
                    }
                }
                if (targetLoc != Location(-1, -1)) break;
            }
            if (targetLoc != Location(-1, -1)) break;
        }
        if (targetLoc == Location(-1, -1)) return INT_MAX;

        while (yard.GetTopContainerId(targetLoc) != targetId) {
            Location bestDest(-1, -1);
            int bestMoves = INT_MAX;
            for (int bay = 0; bay < bays; ++bay) {
                for (int stack = 0; stack < stacks; ++stack) {
                    Location dest(bay, stack);
                    if (!yard.HasMaximumHeight(dest) && dest != targetLoc) {
                        ContainerYard copy = yard;
                        clearBlockingContainers(copy, targetLoc, dest);
                        copy.Relocate(targetLoc, dest);
                        int moves = simulateAndSolve(copy);
                        if (moves < bestMoves) {
                            bestMoves = moves;
                            bestDest = dest;
                        }
                    }
                }
            }
            if (bestDest == Location(-1, -1)) return INT_MAX; // No feasible move
            clearBlockingContainers(yard, targetLoc, bestDest);
            yard.Relocate(targetLoc, bestDest);
        }
        yard.Retrieve(targetLoc);
    }
    return yard.GetNumberOfMoves();
}

// Main entry point: solves the container yard.
void rolloutUnrestrictedSolve(ContainerYard& yard) {
    const int bays = yard.GetNumberOfBays();
    const int stacks = yard.GetNumberOfStacks();
    while (!yard.Empty()) {
        const int targetId = yard.GetNextContainerId();
        Location targetLoc(-1, -1);
        for (int bay = 0; bay < bays; ++bay) {
            for (int stack = 0; stack < stacks; ++stack) {
                Location loc(bay, stack);
                for (int h = 0; h < yard.GetStackHeight(loc); ++h) {
                    if (yard.GetContainerId(loc, h) == targetId) {
                        targetLoc = loc;
                        break;
                    }
                }
                if (targetLoc != Location(-1, -1)) break;
            }
            if (targetLoc != Location(-1, -1)) break;
        }
        while (yard.GetTopContainerId(targetLoc) != targetId) {
            Location bestDest(-1, -1);
            int bestMoves = INT_MAX;
            for (int bay = 0; bay < bays; ++bay) {
                for (int stack = 0; stack < stacks; ++stack) {
                    Location dest(bay, stack);
                    if (!yard.HasMaximumHeight(dest) && dest != targetLoc) {
                        ContainerYard copy = yard;
                        clearBlockingContainers(copy, targetLoc, dest);
                        copy.Relocate(targetLoc, dest);
                        int moves = simulateAndSolve(copy);
                        if (moves < bestMoves) {
                            bestMoves = moves;
                            bestDest = dest;
                        }
                    }
                }
            }
            // Apply best move to the actual yard
            clearBlockingContainers(yard, targetLoc, bestDest);
            yard.Relocate(targetLoc, bestDest);
        }
        yard.Retrieve(targetLoc);
    }
}

// The algorithm is a greedy search with one-step lookahead. For each container that needs to be retrieved, if it is buried, we evaluate all possible stacks (excluding the source) that are not at maximum height as a temporary destination for the top container blocking the target. For each candidate, we simulate the effect of first clearing the blocking containers above the target using a specific relocation rule: repeatedly check if the top of the candidate stack (after relocations) is less than the top of the source stack; if so, find a third stack (not source, not candidate) that is not full and whose smallest container ID is greater than the candidate's top container ID, choosing the one with the smallest such smallest ID (or an empty stack if none found). If such a stack exists, relocate the candidate's top container there; otherwise stop. After clearing, move the source's top container to the candidate. Then recursively solve the entire remaining yard using the same rollout strategy (but in a copy), and count total moves. Choose the candidate with the minimum total moves. Apply the same clearing steps to the actual yard, perform the relocation, and then retrieve the target container. Important edge cases: when the source stack is empty or the target is already on top, skip relocation; when no candidate exists (all other stacks are full), but the problem guarantees a feasible solution; when the clearing search finds no valid third stack, break and proceed with the current top. Time complexity is exponential in the worst case because the recursive simulation can branch on every move, but in practice for small yard sizes (e.g., up to a few dozen containers) it is feasible. Space complexity is proportional to the recursion depth, which is at most the number of containers, plus copies of the yard.
