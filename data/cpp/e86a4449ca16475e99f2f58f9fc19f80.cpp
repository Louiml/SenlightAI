// Given a vector of integers representing the current "path" of an agent, where each connector in the path may be marked with a boolean flag `isToDestroy`, write a C++ function `bool isPathValid(const std::vector<Connector>& path)` that returns `false` if any connector in the path has `isToDestroy == true`; otherwise, return `true`. The function should treat an empty path as valid. The `Connector` struct is defined as: `struct Connector { int id; bool isToDestroy; };`. The function must not modify the input path and must work for any number of connectors, including zero.

// The solution is straightforward: iterate through all elements of the input vector and check if any `Connector` has `isToDestroy` set to `true`. If found, immediately return `false` (short-circuit evaluation). If the loop completes without finding any such connector, return `true`. The most important edge case is an empty vector, which should return `true` because there is nothing to destroy. Also, we must handle the case where all connectors are safe (`isToDestroy == false`) and where exactly one or more are flagged. The time complexity is O(n), where n is the number of connectors in the path, because we potentially have to examine every element. The space complexity is O(1) since we only use a constant amount of extra memory (loop variable and return flag). The function is `const`-correct by taking the vector by `const&` and not mutating anything.

#include <vector>

struct Connector {
    int id;
    bool isToDestroy;
};

// Returns false if any connector in the path has isToDestroy == true, otherwise true.
bool isPathValid(const std::vector<Connector>& path) {
    for (const Connector& con : path) {
        if (con.isToDestroy) {
            return false; // Path is invalid as soon as we find a destroyable connector.
        }
    }
    return true; // No destroyable connectors found (including empty path).
}

#include <cassert>

int main() {
    // Empty path is valid.
    std::vector<Connector> empty;
    assert(isPathValid(empty) == true);

    // Single connector safe.
    std::vector<Connector> oneSafe = {{1, false}};
    assert(isPathValid(oneSafe) == true);

    // Single connector to destroy.
    std::vector<Connector> oneDestroy = {{2, true}};
    assert(isPathValid(oneDestroy) == false);

    // Multiple safe connectors.
    std::vector<Connector> multiSafe = {{1, false}, {2, false}, {3, false}};
    assert(isPathValid(multiSafe) == true);

    // One destroy flag in the middle.
    std::vector<Connector> middleDestroy = {{1, false}, {2, true}, {3, false}};
    assert(isPathValid(middleDestroy) == false);

    // Multiple destroy flags.
    std::vector<Connector> multiDestroy = {{1, true}, {2, false}, {3, true}};
    assert(isPathValid(multiDestroy) == false);

    // All destroy flags.
    std::vector<Connector> allDestroy = {{1, true}, {2, true}};
    assert(isPathValid(allDestroy) == false);

    // Ensure the function does not modify input (implicitly checked by const).
    std::vector<Connector> original = {{1, false}, {2, true}};
    isPathValid(original);
    assert(original[0].isToDestroy == false);
    assert(original[1].isToDestroy == true);

    return 0;
}
