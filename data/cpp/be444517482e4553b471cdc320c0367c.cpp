/*
Given a rectangular grid of characters representing a robot's environment, along with a starting position (row, column), a hit probability for correctly sensing a color, and a blur factor, write a C++ function that returns a 2D vector of floats representing the robot's initial belief distribution over all cells. Each belief must be uniform initially, meaning every cell gets an equal probability of `1.0 / (rows * cols)`. However, the function must also handle the edge case where the grid is empty or has zero rows/columns by returning an empty 2D vector. The input grid is a `std::vector<std::vector<char>>`, and you must not modify the input. The function should be named `initialize_beliefs` and must be a free function independent of any class.
*/

#include <vector>

/**
 * Initialize a uniform belief distribution over a rectangular grid.
 *
 * @param grid A rectangular grid of characters (rows x cols).
 * @return A 2D vector of floats with equal probability for each cell,
 *         or an empty vector if the grid is empty or has zero rows/columns.
 */
std::vector<std::vector<float>> initialize_beliefs(const std::vector<std::vector<char>>& grid) {
    if (grid.empty() || grid[0].empty()) {
        return {};
    }

    const size_t rows = grid.size();
    const size_t cols = grid[0].size();
    const float uniform_probability = 1.0f / static_cast<float>(rows * cols);

    // Construct a rows x cols vector filled with the uniform probability.
    return std::vector<std::vector<float>>(rows, std::vector<float>(cols, uniform_probability));
}

#include <cassert>
#include <vector>

// The solution function is assumed to be declared above.
std::vector<std::vector<float>> initialize_beliefs(const std::vector<std::vector<char>>& grid);

int main() {
    // Normal 2x2 grid
    std::vector<std::vector<char>> grid1 = {{'r', 'g'}, {'g', 'r'}};
    auto beliefs1 = initialize_beliefs(grid1);
    assert(beliefs1.size() == 2);
    assert(beliefs1[0].size() == 2);
    assert(beliefs1[0][0] == 0.25f);
    assert(beliefs1[1][1] == 0.25f);

    // 1x3 grid
    std::vector<std::vector<char>> grid2 = {{'a', 'b', 'c'}};
    auto beliefs2 = initialize_beliefs(grid2);
    assert(beliefs2.size() == 1);
    assert(beliefs2[0].size() == 3);
    assert(beliefs2[0][2] == 1.0f / 3.0f);

    // Empty grid
    std::vector<std::vector<char>> grid3 = {};
    auto beliefs3 = initialize_beliefs(grid3);
    assert(beliefs3.empty());

    // Grid with zero columns
    std::vector<std::vector<char>> grid4 = {{}, {}};
    auto beliefs4 = initialize_beliefs(grid4);
    assert(beliefs4.empty());

    // 3x2 grid, check sum of all elements ≈ 1.0
    std::vector<std::vector<char>> grid5 = {{'x','y'}, {'y','x'}, {'x','x'}};
    auto beliefs5 = initialize_beliefs(grid5);
    float sum = 0.0f;
    for (const auto& row : beliefs5) {
        for (float val : row) sum += val;
    }
    assert(sum > 0.999f && sum < 1.001f);

    // Verify input unchanged
    assert(grid1[0][0] == 'r');
    assert(grid5.size() == 3);
}

// The task is straightforward: compute the uniform prior probabilities for a robot starting with no information about its location. The main steps are:
// 1. Determine the number of rows and columns from the input grid. If either dimension is zero, return an empty `std::vector<std::vector<float>>`.
// 2. Compute the total number of cells (`rows * cols`). Since probabilities must sum to 1, each cell gets `1.0 / total_cells`.
// 3. Construct a 2D vector with `rows` rows, each containing `cols` copies of the uniform value.
// 4. The function is `const` correct because the input grid is passed by `const` reference and we never modify it. The returned vector is a new object.
//
// Edge cases: 
// - Empty grid (`{}`) or grid with zero rows (e.g., `{{}}`) → return empty.
// - Very large grids: multiplication could overflow, but for typical tests it's fine; we can use `double` internally or just rely on `float`. The reference uses `float`, and `1.0 / total` is a double division cast to float implicitly—fine for this exercise.
// - The function should handle non-rectangular grids? The problem statement says "rectangular grid", so we assume all rows have equal length. If not, we use the first row's size for width, but that's undefined behavior; we'll document that assumption.
//
// Time complexity: O(rows * cols) to fill the vector. Space complexity: O(rows * cols) for the returned vector. No extra auxiliary space beyond the output.
