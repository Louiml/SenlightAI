/*
Write a C++ function named `swapElements` that takes a vector of strings `data`, a 2D vector of pairs `operations` (where each inner vector contains exactly two integers `X` and `Y` representing zero-based indices), and returns a new vector of strings after applying all the swap operations sequentially. The function must handle out-of-range indices gracefully by skipping any swap where either index is invalid (i.e., less than 0 or greater than or equal to the vector size). The vector may contain duplicate strings, and the number of operations can be zero. The function must not modify the original vector, and must return a copy with all swaps applied in the order given.
*/

#include <vector>
#include <string>
#include <algorithm>

// Applies a sequence of (X, Y) index swaps to a copy of the input vector.
// Returns the new vector after all valid swaps, leaving the original unchanged.
std::vector<std::string> swapElements(const std::vector<std::string>& data,
                                      const std::vector<std::pair<int, int>>& operations) {
    std::vector<std::string> result = data;  // copy original
    for (const auto& op : operations) {
        int x = op.first;
        int y = op.second;
        // Validate indices: ignore if either is out of bounds
        if (x >= 0 && x < static_cast<int>(result.size()) &&
            y >= 0 && y < static_cast<int>(result.size())) {
            std::swap(result[x], result[y]);
        }
    }
    return result;
}

#include <cassert>
#include <vector>
#include <string>

// The swapElements function is assumed to be declared above (or included here).

int main() {
    // Basic swap
    {
        std::vector<std::string> v = {"a", "b", "c"};
        std::vector<std::pair<int, int>> ops = {{0, 2}};
        auto res = swapElements(v, ops);
        assert(res == std::vector<std::string>({"c", "b", "a"}));
        assert(v == std::vector<std::string>({"a", "b", "c"})); // original unchanged
    }
    // Multiple sequential swaps
    {
        std::vector<std::string> v = {"x", "y", "z", "w"};
        std::vector<std::pair<int, int>> ops = {{0, 1}, {2, 3}, {1, 2}};
        auto res = swapElements(v, ops);
        assert(res == std::vector<std::string>({"y", "z", "x", "w"}));
    }
    // Out-of-range indices are skipped
    {
        std::vector<std::string> v = {"one", "two"};
        std::vector<std::pair<int, int>> ops = {{0, 5}, {-1, 1}, {1, 1}, {2, 0}};
        auto res = swapElements(v, ops);
        assert(res == std::vector<std::string>({"one", "two"})); // all invalid or self-swap
    }
    // No operations
    {
        std::vector<std::string> v = {"q"};
        std::vector<std::pair<int, int>> ops = {};
        auto res = swapElements(v, ops);
        assert(res == std::vector<std::string>({"q"}));
    }
    // Empty vector
    {
        std::vector<std::string> v = {};
        std::vector<std::pair<int, int>> ops = {{0, 1}};
        auto res = swapElements(v, ops);
        assert(res.empty());
    }
    // Duplicate strings
    {
        std::vector<std::string> v = {"a", "a", "b"};
        std::vector<std::pair<int, int>> ops = {{0, 2}};
        auto res = swapElements(v, ops);
        assert(res == std::vector<std::string>({"b", "a", "a"}));
    }
    return 0;
}

// The solution begins by making a copy of the input vector so that the original remains unchanged. For each operation in the operations vector, we first validate that both indices `X` and `Y` are within the valid range `[0, data.size())`. If either index is out of bounds, we skip that operation entirely. If both are valid, we perform a standard swap using `std::swap` on the vector elements at those positions. Since operations are applied sequentially, the order matters; each swap modifies the current state of the copied vector. Edge cases include: empty vector (all operations should be skipped), operations with negative indices or indices equal to the size, and operations that swap an element with itself (which is harmless). Time complexity is O(N + K) where N is the size of the input vector (for copying) and K is the number of operations (since each swap is O(1)). Space complexity is O(N) for the copy, excluding the input and output containers.
