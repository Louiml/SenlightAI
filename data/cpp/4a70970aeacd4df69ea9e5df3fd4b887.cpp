// Implement a C++ function `findDislocComponents` that takes a `std::vector<double>` representing dislocation values (triplets of (x, y, z) components for each cell, stored contiguously) and returns a `std::vector<bool>` of length 3 indicating which of the three global components (x, y, z) appear with any non-zero value anywhere in the data. The function should check all entries; if all three components are already detected as non-zero, it can stop early to avoid unnecessary work. The input vector size must be a multiple of 3 (guaranteed by the caller). The return value must have exactly three boolean elements, where `result[0]` corresponds to the x-component, `result[1]` to y, and `result[2]` to z, each set to `true` if any entry for that component across all cells is strictly different from zero, and `false` otherwise. Empty input (zero length) is allowed and should yield `{false, false, false}`.
// The solution iterates over the input vector in steps of 3, examining each cell's three components. For each component index `d` (0, 1, 2), if the value `dislocs[3*i + d]` is non-zero, we mark that component as present. To achieve early termination, we check after each cell whether all three flags are `true`; if so, we break out of the loop. The algorithm runs in O(n) time in the worst case (where n is the number of cells), but with early exit it can perform significantly fewer iterations when all components appear early. It uses O(1) extra space beyond the output vector. Edge cases include an empty input (immediately returns all false), and inputs where only some components are non-zero—the function correctly leaves the absent components as false. The primary subtlety is ensuring we break correctly and that the flags are initialized properly before the loop.
#include <vector>

// Determine which of the three global components (x, y, z) have any non-zero
// dislocation value across all cells. Input is a flat array of triplets:
// dislocs[3*i + d] where i is the cell index and d is 0=x, 1=y, 2=z.
// Returns a vector<bool> of length 3 with true for components that appear
// with a non-zero value anywhere.
std::vector<bool> findDislocComponents(const std::vector<double>& dislocs) {
    std::vector<bool> result(3, false);
    const std::size_t ncell = dislocs.size() / 3;
    for (std::size_t i = 0; i < ncell; ++i) {
        for (int d = 0; d < 3; ++d) {
            if (dislocs[3 * i + d] != 0.0) {
                result[d] = true;
            }
        }
        // Early exit if all three components are already found.
        if (result[0] && result[1] && result[2]) {
            break;
        }
    }
    return result;
}
#include <cassert>
#include <vector>

// Assume the solution function is declared above.

int main() {
    // Empty input yields all false.
    std::vector<double> empty;
    std::vector<bool> res = findDislocComponents(empty);
    assert(res.size() == 3);
    assert(res == std::vector<bool>({false, false, false}));

    // Only x-component non-zero.
    std::vector<double> d1 = {1.0, 0.0, 0.0,  0.0, 0.0, 0.0};
    res = findDislocComponents(d1);
    assert(res == std::vector<bool>({true, false, false}));

    // All three appear, with early termination.
    std::vector<double> d2 = {0.0, 2.5, 0.0,  3.0, 0.0, -1.0,  0.0, 0.0, 0.0};
    res = findDislocComponents(d2);
    assert(res == std::vector<bool>({true, true, true}));

    // Only y component non-zero across multiple cells.
    std::vector<double> d3 = {0.0, 0.1, 0.0,  0.0, -5.0, 0.0};
    res = findDislocComponents(d3);
    assert(res == std::vector<bool>({false, true, false}));

    // Only z component non-zero.
    std::vector<double> d4 = {0.0, 0.0, 7.0,  0.0, 0.0, 0.0};
    res = findDislocComponents(d4);
    assert(res == std::vector<bool>({false, false, true}));

    // Mixed and all present but scattered.
    std::vector<double> d5 = {0.0, 0.0, 0.0,  5.0, 0.0, 0.0,  0.0, 6.0, 0.0,  0.0, 0.0, -9.0};
    res = findDislocComponents(d5);
    assert(res == std::vector<bool>({true, true, true}));

    // Only x and z present (y absent).
    std::vector<double> d6 = {-3.0, 0.0, 0.0,  0.0, 0.0, 2.0};
    res = findDislocComponents(d6);
    assert(res == std::vector<bool>({true, false, true}));

    // Only y and z present.
    std::vector<double> d7 = {0.0, 1.0, 0.0,  0.0, 0.0, 4.0};
    res = findDislocComponents(d7);
    assert(res == std::vector<bool>({false, true, true}));

    return 0;
}
