Write a C++ function `std::vector<unsigned int> generateGridIndices(int gridWidth, int gridHeight)` that, given a grid width and height (each representing the number of cells along the horizontal and vertical axes, not the number of vertices), produces the index buffer data for a triangle list rendering of the grid. The grid must contain `(gridWidth + 1) * (gridHeight + 1)` vertices, with vertex `(row, column)` stored at index `row * (gridWidth + 1) + column`. For each cell, the function must emit six indices forming two triangles in the following order: triangle 1 uses vertices (top-left, bottom-right, top-right), and triangle 2 uses (top-left, bottom-left, bottom-right). The function must return a vector of indices where every three consecutive entries form one triangle, and the size must be `gridWidth * gridHeight * 6`. If either dimension is zero or negative, return an empty vector. The function should not depend on any external state or graphics APIs; it must be pure and deterministic.

The core algorithm is a double loop over each cell in the grid. For every cell at row `r` (0 to gridHeight-1) and column `c` (0 to gridWidth-1), we compute the four vertex indices:
- `topLeft = r * (gridWidth+1) + c`
- `topRight = topLeft + 1`
- `bottomLeft = (r+1) * (gridWidth+1) + c`
- `bottomRight = bottomLeft + 1`

Then append six indices in the specified order: `{topLeft, bottomRight, topRight, topLeft, bottomLeft, bottomRight}`. This consistent winding order ensures all triangles face the same direction (counter-clockwise when viewed from the positive Y-axis if row increases downward). Edge cases: when either input dimension is non-positive, the grid has no cells, so return an empty vector, avoiding any attempt to allocate or index. Complexity is O(gridWidth * gridHeight) time and O(gridWidth * gridHeight * 6) space for the output, which is optimal since the output itself has that size. Memory is proportional to the number of triangles, not the number of vertices, which is standard for indexed rendering.

#include <vector>

// Generate triangle-list indices for a grid of cells.
// Each cell is split into two triangles.
// Returns empty vector for non-positive dimensions.
std::vector<unsigned int> generateGridIndices(int gridWidth, int gridHeight) {
    if (gridWidth <= 0 || gridHeight <= 0) {
        return {};
    }

    const unsigned int verticesPerRow = static_cast<unsigned int>(gridWidth) + 1u;
    const unsigned int totalCells = static_cast<unsigned int>(gridWidth) * static_cast<unsigned int>(gridHeight);
    std::vector<unsigned int> indices;
    indices.reserve(totalCells * 6u);

    for (int r = 0; r < gridHeight; ++r) {
        for (int c = 0; c < gridWidth; ++c) {
            const unsigned int topLeft = static_cast<unsigned int>(r) * verticesPerRow + static_cast<unsigned int>(c);
            const unsigned int topRight = topLeft + 1u;
            const unsigned int bottomLeft = topLeft + verticesPerRow;
            const unsigned int bottomRight = bottomLeft + 1u;

            // Triangle 1: top-left, bottom-right, top-right
            indices.push_back(topLeft);
            indices.push_back(bottomRight);
            indices.push_back(topRight);

            // Triangle 2: top-left, bottom-left, bottom-right
            indices.push_back(topLeft);
            indices.push_back(bottomLeft);
            indices.push_back(bottomRight);
        }
    }

    return indices;
}

#include <cassert>
#include <vector>

// The solution function is assumed to be defined above.

int main() {
    // 1x1 grid should produce two triangles using all four unique vertices.
    {
        auto idx = generateGridIndices(1, 1);
        assert(idx.size() == 6);
        std::vector<unsigned int> expected = {0, 3, 1, 0, 2, 3};
        assert(idx == expected);
    }

    // 2x1 grid: two cells horizontally, six vertices total.
    {
        auto idx = generateGridIndices(2, 1);
        assert(idx.size() == 12);
        // Cell 0: vertices 0,3,1 and 0,2,3
        assert(idx[0] == 0 && idx[1] == 3 && idx[2] == 1);
        assert(idx[3] == 0 && idx[4] == 2 && idx[5] == 3);
        // Cell 1: vertices 1,4,2 and 1,3,4
        assert(idx[6] == 1 && idx[7] == 4 && idx[8] == 2);
        assert(idx[9] == 1 && idx[10] == 3 && idx[11] == 4);
    }

    // 1x2 grid: two cells vertically, six vertices total.
    {
        auto idx = generateGridIndices(1, 2);
        assert(idx.size() == 12);
        // Cell 0 (row 0, col 0): vertices 0,3,1 and 0,2,3
        assert(idx[0] == 0 && idx[1] == 3 && idx[2] == 1);
        assert(idx[3] == 0 && idx[4] == 2 && idx[5] == 3);
        // Cell 1 (row 1, col 0): vertices 2,5,3 and 2,4,5
        assert(idx[6] == 2 && idx[7] == 5 && idx[8] == 3);
        assert(idx[9] == 2 && idx[10] == 4 && idx[11] == 5);
    }

    // 2x2 grid: 4 cells, 24 indices, all vertices in [0,8]
    {
        auto idx = generateGridIndices(2, 2);
        assert(idx.size() == 24);
        for (unsigned int index : idx) {
            assert(index < 9); // vertices per side = 3, total = 3*3=9
        }
    }

    // Zero and negative dimensions should return empty.
    assert(generateGridIndices(0, 5).empty());
    assert(generateGridIndices(5, 0).empty());
    assert(generateGridIndices(-3, 4).empty());
    assert(generateGridIndices(4, -2).empty());

    // Large grid: check size correctness and that indices stay within range.
    {
        auto idx = generateGridIndices(50, 30);
        assert(idx.size() == static_cast<size_t>(50 * 30 * 6));
        unsigned int maxVertex = (50 + 1) * (30 + 1) - 1;
        for (unsigned int index : idx) {
            assert(index <= maxVertex);
        }
    }

    return 0;
}
