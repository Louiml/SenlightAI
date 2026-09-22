/*
Write a standalone C++ function named `terrainMovementCost` that, given a 21×80 grid of terrain characters represented as a 2D array of `char` tiles (e.g., `'#'` for path, `'M'` for PokeMart, `'C'` for PokeCenter, `':'` for tall grass, `'.'` for short grass, `'~'` for water, `'%'` for mountain, `'^'` for forest), a `TerrainCosts` struct containing movement costs for each terrain type for a specific NPC type (hiker, rival, or player), and integer coordinates `(x, y)`, returns the movement cost to enter the cell at those coordinates. The function must enforce these rules: if the target is on the map border (x==0 or x==79 or y==0 or y==20), the cost is the `border` field if the border terrain is not `'#'`; if the border terrain is `'#'`, the cost is the `gate` field. For interior cells (1≤x≤78 and 1≤y≤19), the cost is determined by the terrain character using the corresponding fields in the struct: `'#'`→path, `'M'`→PMart, `'C'`→PCntr, `':'`→TGrass, `'.'`→SGrass, `'~'`→Water, `'%'`→Mtn, and any other character (including `'^'`)→Forest. The function must be `const`-correct and not modify the input grid or the costs struct. The TerrainCosts struct should be defined as having fields: `int path, PMart, PCntr, TGrass, SGrass, Mtn, Forest, Water, Gate, Border;` and an `enum NPCType { none, hiker, rival } npc;` field. The function should be usable with the provided initialization patterns for Hiker, Rival, and Player from the original code.
*/
#include <cstdint> // for INT16_MAX

enum NPCType { none, hiker, rival };

struct TerrainCosts {
    int path;
    int PMart;
    int PCntr;
    int TGrass;
    int SGrass;
    int Mtn;
    int Forest;
    int Water;
    int Gate;
    int Border;
    NPCType npc;
};

// Return the movement cost to enter cell (x, y) in a 21x80 map.
// Border cells (x==0 or x==79 or y==0 or y==20) are treated specially.
int terrainMovementCost(const char grid[21][80], const TerrainCosts& costs, int x, int y) {
    // Check if the coordinates are on the map border.
    if (x == 0 || x == 79 || y == 0 || y == 20) {
        // On the border, if the terrain is '#', it's a gate; otherwise, it's a border.
        if (grid[y][x] == '#') {
            return costs.Gate;
        } else {
            return costs.Border;
        }
    }

    // Interior cell: map terrain character to its cost.
    switch (grid[y][x]) {
        case '#': return costs.path;
        case 'M': return costs.PMart;
        case 'C': return costs.PCntr;
        case ':': return costs.TGrass;
        case '.': return costs.SGrass;
        case '~': return costs.Water;
        case '%': return costs.Mtn;
        default:  return costs.Forest; // Includes '^' and any other character.
    }
}
#include <cassert>
#include <cstdint>

// The solution's struct, enum, and function are assumed to be defined above.
// For testing, we define a helper to initialize a TerrainCosts for hiker.
TerrainCosts makeHikerCosts() {
    TerrainCosts c;
    c.path = 10;
    c.PMart = 50;
    c.PCntr = 50;
    c.TGrass = 15;
    c.SGrass = 10;
    c.Mtn = 15;
    c.Forest = 15;
    c.Water = INT16_MAX;
    c.Gate = INT16_MAX;
    c.Border = INT16_MAX;
    c.npc = hiker;
    return c;
}

int main() {
    // Create a simple 21x80 grid for testing.
    char grid[21][80];
    for (int y = 0; y < 21; ++y) {
        for (int x = 0; x < 80; ++x) {
            grid[y][x] = '.'; // default short grass
        }
    }
    // Set some special terrains.
    grid[0][0] = '#';       // border gate corner
    grid[0][1] = '%';       // border mountain
    grid[1][1] = '#';       // interior path
    grid[1][2] = 'M';       // interior PokeMart
    grid[1][3] = 'C';       // interior PokeCenter
    grid[1][4] = ':';       // interior tall grass
    grid[1][5] = '~';       // interior water
    grid[1][6] = '%';       // interior mountain
    grid[1][7] = '^';       // interior forest
    grid[20][5] = '.';      // border non-gate
    grid[20][6] = '#';      // border gate

    TerrainCosts hiker = makeHikerCosts();

    // Border tests.
    assert(terrainMovementCost(grid, hiker, 0, 0) == INT16_MAX);      // gate
    assert(terrainMovementCost(grid, hiker, 0, 1) == INT16_MAX);      // border (mountain)
    assert(terrainMovementCost(grid, hiker, 20, 5) == INT16_MAX);     // border (short grass)
    assert(terrainMovementCost(grid, hiker, 20, 6) == INT16_MAX);     // gate

    // Interior tests.
    assert(terrainMovementCost(grid, hiker, 1, 1) == 10);  // path '#'
    assert(terrainMovementCost(grid, hiker, 1, 2) == 50);  // 'M'
    assert(terrainMovementCost(grid, hiker, 1, 3) == 50);  // 'C'
    assert(terrainMovementCost(grid, hiker, 1, 4) == 15);  // ':'
    assert(terrainMovementCost(grid, hiker, 1, 5) == INT16_MAX); // '~'
    assert(terrainMovementCost(grid, hiker, 1, 6) == 15);  // '%'
    assert(terrainMovementCost(grid, hiker, 1, 7) == 15);  // '^' → Forest

    // Interior default (short grass '.').
    assert(terrainMovementCost(grid, hiker, 5, 5) == 10);

    // Const-correctness check: function should accept a const grid reference implicitly.
    const char constGrid[21][80] = {};
    // Uncommenting the next line would compile error if function didn't accept const.
    // (It does, because the parameter is const char[21][80].)
    // assert(terrainMovementCost(constGrid, hiker, 1, 1) == 15); // constGrid all nulls → default Forest? Actually '\0' is not a known char → Forest.

    return 0;
}
// The solution requires a straightforward mapping from terrain character and position to a cost value. The key decision point is the border check: any cell on the outer boundary (row 0, row 20, column 0, or column 79) is treated specially. For such cells, if the terrain is the gate symbol `'#'`, the cost is the `gate` field; otherwise, it is the `border` field. For interior cells, we ignore the `border` and `gate` fields entirely and map each terrain character to its corresponding cost field. The mapping for interior cells is deterministic: `'#'`→path, `'M'`→PMart, `'C'`→PCntr, `':'`→TGrass, `'.'`→SGrass, `'~'`→Water, `'%'`→Mtn, and everything else (notably `'^'` and any unexpected characters) →Forest. Edge cases include: coordinates that are outside the valid range (though the function assumes valid input, it’s safe to clamp or return `INT16_MAX` if out of bounds—but for simplicity we assume valid coordinates). The original code uses `strcmp` against string literals, but since we are using a `char` grid, we compare `char` values directly. Time complexity is O(1) per call, and space complexity is O(1) auxiliary. The function should be declared as `int terrainMovementCost(const char grid[21][80], const TerrainCosts& costs, int x, int y)`.
