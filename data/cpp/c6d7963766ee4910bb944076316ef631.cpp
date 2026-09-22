// Write a C++ function named `calculateGridValue` that takes a 2D grid represented as a `std::vector<std::vector<BrickData>>` (where `BrickData` is a struct with an `int index` and an `int property`) and returns the total score computed by iterating over every cell in the grid and applying the following scoring rules based on the `index` field:  
// - If `index == 0` (GenericBrick): add `(property + 1) * 10` points.  
// - If `index == 1` (RichBrick): based on `property` value (0=SteelLeft, 1=BronzeLeft, 2=SilverLeft, 3=GoldLeft), add 50, 100, 200, or 400 points respectively.  
// - If `index == 2` (BonusCoin) or `index == 3` (GiftBrick): add 10 points regardless of property.  
// - If `index == 4` (TenPointsBrick): add `-10 * property + 80` points (note: property is an integer, so this can be negative).  
// - Any other `index` value: ignore the cell (add nothing).  
// The grid may be empty or contain rows of different lengths; the function must handle these cases gracefully. The function must be `const`-correct: it should accept the grid by `const` reference and not modify it. Use appropriate loops or standard algorithms to process all cells.  
// The solution must be self-contained—define the `BrickData` struct in the same file (with public fields) and use only standard library headers. Provide the function declaration and definition, but do not include a `main` function in the solution section.  
// Your function should be efficient for large grids, and the algorithm should be straightforward: traverse each cell once and accumulate the total.

#include <cassert>
#include <vector>

// Include solution here (or assume it is above in the same file)
struct BrickData { int index; int property; };
int calculateGridValue(const std::vector<std::vector<BrickData>>& grid);

int main() {
    // Case 1: Empty grid
    assert(calculateGridValue({}) == 0);

    // Case 2: Grid with rows of different lengths, containing various types
    std::vector<std::vector<BrickData>> grid1 = {
        { {0, 0}, {0, 1}, {4, 3} },   // Generic(0): (0+1)*10=10; Generic(1): 20; Ten(3): -10*3+80=50 => total 80
        { {1, 0}, {1, 2} },            // Rich(Steel): 50; Rich(Silver): 200 => 250
        { {2, 0}, {3, 0} },            // BonusCoin:10, GiftBrick:10 => 20
        { {5, 100} }                   // Unknown index: ignored
    };
    assert(calculateGridValue(grid1) == 80 + 250 + 20);  // 350

    // Case 3: All TenPointsBrick with negative contribution
    std::vector<std::vector<BrickData>> grid2 = {
        { {4, 0}, {4, 1}, {4, 2} }    // 80, 70, 60 => total 210
    };
    assert(calculateGridValue(grid2) == 210);

    // Case 4: Empty rows inside the grid
    std::vector<std::vector<BrickData>> grid3 = { {}, { {0, 0} }, {} };
    assert(calculateGridValue(grid3) == 10);

    // Case 5: RichBrick with invalid property (e.g., 7) should contribute 0
    std::vector<std::vector<BrickData>> grid4 = { { {1, 7} } };
    assert(calculateGridValue(grid4) == 0);

    // Case 6: Large simple grid to test sum of Generic bricks with property=0
    std::vector<std::vector<BrickData>> grid5(3, std::vector<BrickData>(4, {0, 0}));
    assert(calculateGridValue(grid5) == 3 * 4 * 10); // 120

    return 0;
}

#include <vector>

// Data structure representing a single brick or coin in the grid.
struct BrickData {
    int index;      // Type identifier: 0=Generic, 1=Rich, 2=BonusCoin, 3=Gift, 4=TenPoints
    int property;   // Additional data (e.g., remaining hits or sub-type)
};

// Calculate the total score for the entire grid.
// Accepts a 2D vector of BrickData by const reference; does not modify it.
int calculateGridValue(const std::vector<std::vector<BrickData>>& grid) {
    int totalValue = 0;

    // Iterate over each row
    for (const auto& row : grid) {
        // Iterate over each cell in the row
        for (const auto& brick : row) {
            switch (brick.index) {
                case 0: // GenericBrick
                    totalValue += (brick.property + 1) * 10;
                    break;
                case 1: // RichBrick
                    // property: 0=SteelLeft, 1=BronzeLeft, 2=SilverLeft, 3=GoldLeft
                    if (brick.property == 0) totalValue += 50;
                    else if (brick.property == 1) totalValue += 100;
                    else if (brick.property == 2) totalValue += 200;
                    else if (brick.property == 3) totalValue += 400;
                    // For any other property value, no points are added (implicit)
                    break;
                case 2: // BonusCoin
                case 3: // GiftBrick
                    totalValue += 10;
                    break;
                case 4: // TenPointsBrick
                    totalValue += -10 * brick.property + 80;
                    break;
                default:
                    // Unknown index: ignore this cell
                    break;
            }
        }
    }

    return totalValue;
}

// The problem is essentially a nested traversal over a 2D vector. The main algorithm: initialize `int total = 0;` then for each row in the grid, iterate through each cell's `BrickData` and apply the scoring rules via a switch on `index`. For each index case, use `property` as needed. Edge cases: (1) empty grid (no rows) or empty inner vectors (no columns) should return 0 because the loops simply don't execute; (2) rows with different lengths are handled naturally because we iterate over `row.size()`; (3) unknown index values are ignored via a `default` case in the switch. Time complexity is \(O(R \times C)\) where \(R\) is the number of rows and \(C\) is the maximum row length (but actual work is the total number of cells). Space complexity is \(O(1)\) auxiliary, as we only use a single integer. The function must be `const`-correct: accepting `const std::vector<std::vector<BrickData>>&`, and since we don't modify anything, we can make the function itself `const` if it were a member, but as a free function we just use a `const` reference. We can use range-based for loops for clarity, and `const auto&` for iteration to avoid copies.
