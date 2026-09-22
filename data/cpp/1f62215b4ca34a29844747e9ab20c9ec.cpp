// Write a standalone C++ function named `findNearestTreeStep` that simulates the movement logic of a dwarf in a grid-based environment. The function takes the current row `r`, current column `c`, a const reference to a `Dwarf` object (which provides a `look(row, col)` method returning `EMPTY`, `PINE_TREE`, or `APPLE_TREE`), and an output stream reference for logging. The function must return a `std::pair<int,int>` representing the next cell the dwarf should walk to. The dwarf scans outward in concentric squares (starting from Manhattan distance 2) looking for the first tree in the four cardinal directions (right, left, down, up) at the current range. For each direction, if a tree is found at range `k` and the cell at range `k-1` in that same direction is `EMPTY`, the function should return that empty cell (the cell immediately before the tree). If that intermediate cell is not empty, or if no tree is found at that range, the function recurses with range+1. If the range would make the scanned cell go out of bounds (row/col < 0 or >= 40), the function should clamp the search to a small random walk (use `rand()` modulo 40 for both coordinates) and return that random cell. You may assume the grid is at most 40x40, and that the `Dwarf` class has `look(int, int)` returning an enum-like integer, and constants `EMPTY`, `PINE_TREE`, `APPLE_TREE` are defined globally. Provide a robust implementation that handles edge cases like immediate trees next to the dwarf (though the function will only be called when no adjacent tree exists).
// The main algorithm is a recursive depth-first search expanding the Manhattan distance range from the dwarf's current position. To avoid revisiting the same range repeatedly, we check the four cardinal directions at each increment. The base condition ensures we don't go out of bounds; if we would, we fall back to a random walk to avoid infinite recursion. The critical edge case is when a tree is found but the cell just before it is occupied: we must continue expanding the range rather than skipping over the obstacle. Also, if the dwarf is near the border, the range check prevents invalid look calls. The recursive depth is at most 40 (since max grid dimension), so time complexity is O(40) per call in the worst case (each look is O(1)), and space complexity is O(40) for recursion stack. The random fallback uses `rand()` which is fine for a simulation. The function does not modify the dwarf state; it only returns the next position. The `Dwarf` object is passed by const reference to avoid side effects, and the output stream is used for logging during debugging.
#include <utility>
#include <cstdlib>
#include <iostream>

// Forward declaration of assumed constants and Dwarf interface
enum { EMPTY = 0, PINE_TREE = 1, APPLE_TREE = 2 };
class Dwarf {
public:
    int look(int r, int c) const; // returns one of EMPTY/PINE_TREE/APPLE_TREE
};

// Returns the next cell to walk to, either the empty cell before a found tree
// or a random valid cell if out of bounds fallback is needed.
std::pair<int,int> findNearestTreeStep(const Dwarf& dwarf, int r, int c, int range, std::ostream& log) {
    const int MAX = 40; // maximum grid dimension

    // Out-of-bounds check for the current range: if any coordinate would be out of bounds,
    // fall back to a random walk within the 0..39 grid.
    if (r + range >= MAX || r - range < 0 || c + range >= MAX || c - range < 0) {
        log << "Out of bounds at range " << range << ", random walk fallback\n";
        return { rand() % MAX, rand() % MAX };
    }

    // Check right: (r, c+range)
    if (dwarf.look(r, c+range) == PINE_TREE || dwarf.look(r, c+range) == APPLE_TREE) {
        int intermediate = c + range - 1;
        if (dwarf.look(r, intermediate) == EMPTY) {
            log << "Found tree to right at range " << range << "\n";
            return { r, intermediate };
        } else {
            log << "Obstacle before right tree, expanding\n";
            return findNearestTreeStep(dwarf, r, c, range+1, log);
        }
    }

    // Check left: (r, c-range)
    if (dwarf.look(r, c-range) == PINE_TREE || dwarf.look(r, c-range) == APPLE_TREE) {
        int intermediate = c - range + 1;
        if (dwarf.look(r, intermediate) == EMPTY) {
            log << "Found tree to left at range " << range << "\n";
            return { r, intermediate };
        } else {
            log << "Obstacle before left tree, expanding\n";
            return findNearestTreeStep(dwarf, r, c, range+1, log);
        }
    }

    // Check down: (r+range, c)
    if (dwarf.look(r+range, c) == PINE_TREE || dwarf.look(r+range, c) == APPLE_TREE) {
        int intermediate = r + range - 1;
        if (dwarf.look(intermediate, c) == EMPTY) {
            log << "Found tree below at range " << range << "\n";
            return { intermediate, c };
        } else {
            log << "Obstacle before below tree, expanding\n";
            return findNearestTreeStep(dwarf, r, c, range+1, log);
        }
    }

    // Check up: (r-range, c)
    if (dwarf.look(r-range, c) == PINE_TREE || dwarf.look(r-range, c) == APPLE_TREE) {
        int intermediate = r - range + 1;
        if (dwarf.look(intermediate, c) == EMPTY) {
            log << "Found tree above at range " << range << "\n";
            return { intermediate, c };
        } else {
            log << "Obstacle before above tree, expanding\n";
            return findNearestTreeStep(dwarf, r, c, range+1, log);
        }
    }

    // No tree at this range, expand further
    return findNearestTreeStep(dwarf, r, c, range+1, log);
}
#include <cassert>
#include <iostream>
#include <utility>
#include <cstdlib>
#include <sstream>

// Mock Dwarf for testing: provides a look method from a 2D grid.
class TestDwarf : public Dwarf {
    int grid[40][40];
public:
    TestDwarf() {
        std::srand(42); // fixed seed for deterministic random fallback
        for (int i = 0; i < 40; ++i)
            for (int j = 0; j < 40; ++j)
                grid[i][j] = EMPTY;
    }
    void set(int r, int c, int val) { grid[r][c] = val; }
    int look(int r, int c) const override {
        if (r < 0 || r >= 40 || c < 0 || c >= 40) return EMPTY; // out of bounds treated as empty
        return grid[r][c];
    }
};

int main() {
    // Helper to call the solution function with a trivial log
    auto call = [](TestDwarf& d, int r, int c, int range) {
        std::ostringstream log;
        return findNearestTreeStep(d, r, c, range, log);
    };

    // Test 1: Tree to the right at distance 2, empty intermediate
    TestDwarf d1;
    d1.set(10, 12, PINE_TREE);
    auto p1 = call(d1, 10, 10, 2);
    assert(p1.first == 10 && p1.second == 11);

    // Test 2: Tree to the left at distance 3, empty intermediate
    TestDwarf d2;
    d2.set(5, 2, APPLE_TREE);
    auto p2 = call(d2, 5, 5, 2);
    assert(p2.first == 5 && p2.second == 3); // intermediate at c-2+1 = 4? Actually range starts at 2, but tree at c-3? Let's adjust: set tree at distance 3, so first found at range 3, intermediate c-2
    // Let's redo: set tree at (5,2), start (5,5). Range 2: look left at 3? No, left at c-2=3 is empty. Range 3: left at c-3=2 is tree, intermediate c-2=3 is empty. So expected (5,3)
    // Already covered by setting tree at (5,2) and checking range 2: left at 3 is empty, range 3: left at 2 is tree, intermediate 3 is empty. So p2 should be (5,3). Let's rewrite to avoid confusion.
    // Actually let's use a simpler case: tree at (5,2) from (5,5), range 2 gives left at 3 empty, range 3 gives left at 2 tree, intermediate 3 empty => return (5,3). But we started range 2, so first call range=2, not found, recursion range=3, found. So p2 should be (5,3).
    assert(p2.first == 5 && p2.second == 3);

    // Test 3: Tree directly above at distance 2
    TestDwarf d3;
    d3.set(8, 10, APPLE_TREE);
    auto p3 = call(d3, 10, 10, 2);
    assert(p3.first == 9 && p3.second == 10);

    // Test 4: No tree in range -> eventually out of bounds -> random fallback
    TestDwarf d4;
    auto p4 = call(d4, 0, 0, 2);
    // Since all empty, recursion will hit out of bounds at range 2 when checking r-range<0, will random walk. Need to ensure return is within [0,39]
    assert(p4.first >= 0 && p4.first < 40 && p4.second >= 0 && p4.second < 40);

    // Test 5: Obstacle before tree forces range expansion
    TestDwarf d5;
    d5.set(10, 12, PINE_TREE); // tree at range 2 to right
    d5.set(10, 11, PINE_TREE); // obstacle at intermediate
    auto p5 = call(d5, 10, 10, 2);
    // At range 2, right tree found but intermediate occupied, so expand to range 3.
    // Place tree at range 3? Not done. So will expand until out of bounds and fallback.
    // Since no other tree, it will eventually random fallback.
    assert(p5.first >= 0 && p5.first < 40 && p5.second >= 0 && p5.second < 40);

    // Test 6: Tree below at distance 4 with empty intermediate
    TestDwarf d6;
    d6.set(14, 10, PINE_TREE);
    auto p6 = call(d6, 10, 10, 2);
    // Range 2: no tree, range 3: no tree, range 4: down at (14,10) tree, intermediate (13,10) empty
    assert(p6.first == 13 && p6.second == 10);

    // Test 7: Tree to right at distance 2 but intermediate is a tree => expand
    // Already covered by test 5, but let's also check that expansion finds a tree further.
    TestDwarf d7;
    d7.set(10, 12, PINE_TREE); // range 2
    d7.set(10, 11, PINE_TREE); // obstacle
    d7.set(10, 13, APPLE_TREE); // range 3
    auto p7 = call(d7, 10, 10, 2);
    // Range 2: right tree but intermediate occupied -> recurse range 3: right at (10,13) tree, intermediate (10,12) is PINE_TREE (not empty), so again obstacle, recurse range 4... eventually out of bounds? Not, since range 4 right is 14 which is in bounds, but no tree there, then left/down/up none, expand range 5... eventually out of bounds at range 5? r=10, c=10, range 5 -> c+5=15 in bounds, r-5=5 in bounds, all in bounds until range 30? Actually max 40, so could go far. But no more trees, so will hit out of bounds when range>30? Actually when range+10>=40 => range>=30, so fallback. So we just check it returns valid coordinates.
    assert(p7.first >= 0 && p7.first < 40 && p7.second >= 0 && p7.second < 40);

    // Test 8: Tree to left at distance 2 with empty intermediate
    TestDwarf d8;
    d8.set(10, 8, APPLE_TREE);
    auto p8 = call(d8, 10, 10, 2);
    assert(p8.first == 10 && p8.second == 9);

    // Test 9: Tree to up at distance 3 with empty intermediate
    TestDwarf d9;
    d9.set(7, 10, PINE_TREE);
    auto p9 = call(d9, 10, 10, 2);
    // Range 2: up at 8 empty, range 3: up at 7 tree, intermediate 8 empty -> (8,10)
    assert(p9.first == 8 && p9.second == 10);

    // Test 10: Multiple trees, ensures first found (right priority) is chosen
    TestDwarf d10;
    d10.set(10, 12, PINE_TREE); // right at range 2
    d10.set(10, 8, APPLE_TREE); // left at range 2
    auto p10 = call(d10, 10, 10, 2);
    assert(p10.first == 10 && p10.second == 11); // right priority because checked first

    std::cout << "All tests passed!\n";
    return 0;
}
