Write a C++ function `mineralFormation` that takes a 2D vector of integers (each element is either 0 or 1) representing a cross‑section of a cave, where 1 denotes rock and 0 denotes air. The function must return a string: `"stalactites"` if there is at least one 1 in the top row (ceiling), `"stalagmites"` if there is at least one 1 in the bottom row (floor), `"both"` if both conditions are true, and `"neither"` if no 1 appears in either the top or bottom row (though the problem states this case will not occur, handle it gracefully). The input is guaranteed to be non‑empty, rectangular, and to contain only 0s and 1s.
The solution is straightforward: examine the first row (index 0) for any `1`s, and examine the last row (index `rows-1`) for any `1`s. Use two boolean flags initialized to `false`; iterate over the columns of the top row and set `topHasRock` if any cell equals 1, and similarly for the bottom row. After scanning both rows, combine the flags: if both are true return `"both"`; else if only the top is true return `"stalactites"`; else if only the bottom is true return `"stalagmites"`; else return `"neither"`. Time complexity is O(cols) because we iterate through each row once (two separate loops over the columns, each O(cols)). Space complexity is O(1) beyond the input storage; we only use a few boolean variables and the returned string. Edge cases: a single‑row cave means top and bottom are the same row—if it contains any 1, both flags will be set, returning `"both"`; if it contains no 1, returns `"neither"`. Empty rows are not possible per constraints.
#include <string>
#include <vector>

// Determine whether cave formation is stalactites, stalagmites, both, or neither.
// A stalactite is present if the top row has any 1s; a stalagmite if the bottom row has any 1s.
std::string mineralFormation(const std::vector<std::vector<int>>& cave) {
    if (cave.empty() || cave[0].empty()) {
        return "neither";
    }

    bool topHasRock = false;
    bool bottomHasRock = false;
    size_t rows = cave.size();
    size_t cols = cave[0].size();

    // Check top row (ceiling)
    for (size_t col = 0; col < cols; ++col) {
        if (cave[0][col] == 1) {
            topHasRock = true;
            break;
        }
    }

    // Check bottom row (floor)
    for (size_t col = 0; col < cols; ++col) {
        if (cave[rows - 1][col] == 1) {
            bottomHasRock = true;
            break;
        }
    }

    if (topHasRock && bottomHasRock) {
        return "both";
    } else if (topHasRock) {
        return "stalactites";
    } else if (bottomHasRock) {
        return "stalagmites";
    } else {
        return "neither";
    }
}
#include <cassert>
#include <vector>
#include <string>

// (Function definition here, as above)

int main() {
    // Example 1: stalactites
    std::vector<std::vector<int>> cave1 = {
        {0, 1, 0, 1},
        {0, 1, 0, 1},
        {0, 0, 0, 1},
        {0, 0, 0, 0}
    };
    assert(mineralFormation(cave1) == "stalactites");

    // Example 2: stalagmites
    std::vector<std::vector<int>> cave2 = {
        {0, 0, 0, 0},
        {0, 1, 0, 1},
        {0, 1, 1, 1},
        {0, 1, 1, 1}
    };
    assert(mineralFormation(cave2) == "stalagmites");

    // Example 3: both
    std::vector<std::vector<int>> cave3 = {
        {1, 0, 1, 0},
        {1, 1, 0, 1},
        {0, 1, 1, 1},
        {0, 1, 1, 1}
    };
    assert(mineralFormation(cave3) == "both");

    // Single row with 1s -> both top and bottom are the same row
    std::vector<std::vector<int>> singleRow = {{1, 0, 1}};
    assert(mineralFormation(singleRow) == "both");

    // Single row with no 1s -> neither (edge case not in problem spec but robust)
    std::vector<std::vector<int>> singleEmpty = {{0, 0, 0}};
    assert(mineralFormation(singleEmpty) == "neither");

    // Large rectangular matrix with only bottom rocks
    std::vector<std::vector<int>> wide = {
        {0, 0},
        {0, 0},
        {0, 0},
        {1, 1}
    };
    assert(mineralFormation(wide) == "stalagmites");

    // Matrix with only top rocks
    std::vector<std::vector<int>> tall = {
        {1, 0},
        {0, 0},
        {0, 0}
    };
    assert(mineralFormation(tall) == "stalactites");

    // Both top and bottom, but middle rows have different patterns
    std::vector<std::vector<int>> mixed = {
        {1, 0},
        {0, 0},
        {0, 1}
    };
    assert(mineralFormation(mixed) == "both");
}
