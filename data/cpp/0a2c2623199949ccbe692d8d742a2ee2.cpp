// Given an integer matrix stored as a flat `std::vector<int64_t>` in row-major order, along with its row count `numRows` and column count `numCols`, write a C++ function `tileMatrixInfo` that returns a `std::map<std::string, std::string>` describing a 3-level tiled loop nest skeleton in the style of the provided LLVM snippet. The function should not actually generate LLVM IR; instead, it should simulate the conceptual structure and produce metadata strings for each loop level (columns, rows, inner) containing: the loop name, the start value (always 0), the bound (numCols for columns, numRows for rows, numInner = numCols/2 for inner), the step (fixed tile size of 4), and the header block name (e.g., `"cols.header"`). The output map keys should be `"columns"`, `"rows"`, and `"inner"`, each mapping to a comma-separated string in the format `"name,start,bound,step,header"`. Additionally, to mimic the loop nesting, return a separate `std::vector<std::string>` `nestOrder` (passed by reference) containing the names in nesting order: `["columns", "rows", "inner"]`. The function must validate that `numRows > 0`, `numCols > 0`, and that the total size equals `numRows * numCols`; otherwise, it should throw `std::invalid_argument`. The inner loop bound is defined as `numCols / 2` (integer division). Use `int64_t` for all numeric values. Ensure all header names follow the pattern `loopName + ".header"`.

// The task abstracts the LLVM loop-creation logic into a pure C++ function that produces descriptive metadata rather than IR. The main algorithm is straightforward: validate inputs, then for each of the three levels (columns, rows, inner) create a metadata string with fixed start=0, step=4, bound as specified, and header name derived from the loop name. The function must also set the `nestOrder` vector to the fixed order. Edge cases: invalid dimensions or size mismatch must throw. The inner bound uses integer division, so if `numCols` is odd, the inner bound is correctly truncated. Time complexity is O(1) constant work, space complexity is O(1) auxiliary besides the output structures. The mapping from the LLVM snippet is direct: the `CreateLoop` parameters `Bound` and `Step` map to our bound and step, the `Name` maps to our loop name, and the header block is `Name + ".header"`. The function is purely descriptive and does not build actual control flow.

#include <cstdint>
#include <map>
#include <stdexcept>
#include <string>
#include <vector>

// Describes a tiled loop nest skeleton for an integer matrix.
// Returns metadata for three nested loops (columns, rows, inner).
// Throws std::invalid_argument on invalid dimensions or size mismatch.
std::map<std::string, std::string> tileMatrixInfo(
    const std::vector<int64_t>& matrix,
    int64_t numRows,
    int64_t numCols,
    std::vector<std::string>& nestOrder) {
    
    const int64_t tileSize = 4;
    
    if (numRows <= 0 || numCols <= 0) {
        throw std::invalid_argument("Matrix dimensions must be positive.");
    }
    if (matrix.size() != static_cast<size_t>(numRows * numCols)) {
        throw std::invalid_argument("Matrix size does not match dimensions.");
    }
    
    nestOrder = {"columns", "rows", "inner"};
    
    std::map<std::string, std::string> result;
    
    const int64_t innerBound = numCols / 2;
    
    result["columns"] = "columns,0," + std::to_string(numCols) + "," +
                        std::to_string(tileSize) + ",cols.header";
    result["rows"] = "rows,0," + std::to_string(numRows) + "," +
                     std::to_string(tileSize) + ",rows.header";
    result["inner"] = "inner,0," + std::to_string(innerBound) + "," +
                      std::to_string(tileSize) + ",inner.header";
    
    return result;
}

#include <cassert>
#include <cstdint>
#include <map>
#include <stdexcept>
#include <string>
#include <vector>

int main() {
    // Helper to call solution
    auto call = [](const std::vector<int64_t>& m, int64_t r, int64_t c,
                   std::vector<std::string>& order) {
        return tileMatrixInfo(m, r, c, order);
    };

    // Normal 3x3 matrix, inner bound = 3/2 = 1
    {
        std::vector<int64_t> m(9, 0);
        std::vector<std::string> order;
        auto info = call(m, 3, 3, order);
        assert(order.size() == 3);
        assert(order[0] == "columns");
        assert(order[1] == "rows");
        assert(order[2] == "inner");
        assert(info.at("columns") == "columns,0,3,4,cols.header");
        assert(info.at("rows") == "rows,0,3,4,rows.header");
        assert(info.at("inner") == "inner,0,1,4,inner.header");
    }

    // Even columns, inner bound = 4/2 = 2
    {
        std::vector<int64_t> m(8, 1);
        std::vector<std::string> order;
        auto info = call(m, 2, 4, order);
        assert(info.at("inner") == "inner,0,2,4,inner.header");
        assert(info.at("columns") == "columns,0,4,4,cols.header");
        assert(info.at("rows") == "rows,0,2,4,rows.header");
    }

    // Single row single column
    {
        std::vector<int64_t> m = {5};
        std::vector<std::string> order;
        auto info = call(m, 1, 1, order);
        assert(info.at("columns") == "columns,0,1,4,cols.header");
        assert(info.at("rows") == "rows,0,1,4,rows.header");
        assert(info.at("inner") == "inner,0,0,4,inner.header");
    }

    // Throws on invalid size mismatch
    {
        std::vector<int64_t> m(5, 1);
        std::vector<std::string> order;
        bool threw = false;
        try {
            call(m, 2, 2, order);
        } catch (const std::invalid_argument&) {
            threw = true;
        }
        assert(threw);
    }

    // Throws on non-positive dimensions
    {
        std::vector<int64_t> m(2, 1);
        std::vector<std::string> order;
        bool threw = false;
        try {
            call(m, 0, 2, order);
        } catch (const std::invalid_argument&) {
            threw = true;
        }
        assert(threw);
    }

    // Large matrix, verify numeric values in strings
    {
        std::vector<int64_t> m(100, 0);
        std::vector<std::string> order;
        auto info = call(m, 10, 10, order);
        assert(info.at("columns") == "columns,0,10,4,cols.header");
        assert(info.at("rows") == "rows,0,10,4,rows.header");
        assert(info.at("inner") == "inner,0,5,4,inner.header");
    }
}
