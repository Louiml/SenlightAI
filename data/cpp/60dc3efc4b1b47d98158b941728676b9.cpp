// Write a C++ function `computeBranchLengths` that takes three vectors of coordinates (`x`, `y`, `z`) and a vector of "branch" definitions, where each branch is a vector of 1-based indices into the coordinate arrays. The function must return a vector of the total Euclidean length of each branch, computed as the sum of the distances between consecutive points in that branch. Any segment where any coordinate difference is not a valid number (NaN) must be skipped (i.e., not added to the branch length), but the remaining valid segments of that branch should still contribute. The input coordinates are guaranteed to have at least as many elements as the maximum index used; however, individual indices may be out of order or repeated, and branches may be empty. The function must be const-correct and not modify any input. Provide a self-contained implementation with proper headers and comments.

#include <cassert>
#include <cmath>
#include <vector>

// The solution function is assumed to be declared above.

int main() {
    // Simple straight line with two points.
    {
        std::vector<std::vector<int>> branches = {{1, 2}};
        std::vector<double> x = {0.0, 3.0};
        std::vector<double> y = {0.0, 4.0};
        std::vector<double> z = {0.0, 0.0};
        auto result = computeBranchLengths(branches, x, y, z);
        assert(result.size() == 1);
        assert(std::abs(result[0] - 5.0) < 1e-12);
    }
    
    // Three points forming a triangle: lengths 3+4+5? Actually 3-4-5 from (0,0) to (3,0) to (3,4).
    {
        std::vector<std::vector<int>> branches = {{1, 2, 3}};
        std::vector<double> x = {0.0, 3.0, 3.0};
        std::vector<double> y = {0.0, 0.0, 4.0};
        std::vector<double> z = {0.0, 0.0, 0.0};
        auto result = computeBranchLengths(branches, x, y, z);
        assert(result.size() == 1);
        assert(std::abs(result[0] - (3.0 + 4.0)) < 1e-12);
    }
    
    // Empty branch returns 0.
    {
        std::vector<std::vector<int>> branches = {{}};
        std::vector<double> x = {1.0, 2.0};
        std::vector<double> y = {1.0, 2.0};
        std::vector<double> z = {1.0, 2.0};
        auto result = computeBranchLengths(branches, x, y, z);
        assert(result.size() == 1);
        assert(result[0] == 0.0);
    }
    
    // Branch with one point returns 0.
    {
        std::vector<std::vector<int>> branches = {{2}};
        std::vector<double> x = {0.0, 5.0};
        std::vector<double> y = {0.0, 5.0};
        std::vector<double> z = {0.0, 5.0};
        auto result = computeBranchLengths(branches, x, y, z);
        assert(result.size() == 1);
        assert(result[0] == 0.0);
    }
    
    // NaN in one segment should skip that segment but keep the others.
    {
        std::vector<std::vector<int>> branches = {{1, 2, 3, 4}};
        std::vector<double> x = {0.0, 3.0, std::nan(""), 6.0};
        std::vector<double> y = {0.0, 0.0, 0.0, 0.0};
        std::vector<double> z = {0.0, 0.0, 0.0, 0.0};
        // Segments: 1-2 length 3, 2-3 NaN (skip), 3-4 NaN (skip because x[2] is NaN)
        auto result = computeBranchLengths(branches, x, y, z);
        assert(result.size() == 1);
        assert(std::abs(result[0] - 3.0) < 1e-12);
    }
    
    // Multiple branches with repeated indices.
    {
        std::vector<std::vector<int>> branches = {{1, 2, 1}, {2, 3}};
        std::vector<double> x = {0.0, 0.0, 3.0};
        std::vector<double> y = {0.0, 0.0, 0.0};
        std::vector<double> z = {0.0, 0.0, 0.0};
        auto result = computeBranchLengths(branches, x, y, z);
        assert(result.size() == 2);
        // Branch1: 1-2 length 0, 2-1 length 0 => total 0
        // Branch2: 2-3 length 3 => total 3
        assert(result[0] == 0.0);
        assert(std::abs(result[1] - 3.0) < 1e-12);
    }
    
    // Out-of-order indices still work because we just compute distances.
    {
        std::vector<std::vector<int>> branches = {{3, 1}};
        std::vector<double> x = {0.0, 0.0, 4.0};
        std::vector<double> y = {0.0, 0.0, 0.0};
        std::vector<double> z = {0.0, 0.0, 0.0};
        auto result = computeBranchLengths(branches, x, y, z);
        assert(result.size() == 1);
        assert(std::abs(result[0] - 4.0) < 1e-12);
    }
    
    return 0;
}

#include <vector>
#include <cmath>
#include <cstddef>

/**
 * Compute the total Euclidean length of each branch defined by a list of
 * segment lists.
 *
 * @param branches A vector of branches, each branch being a vector of
 *                 1-based indices into the coordinate vectors.
 * @param x, y, z  Coordinate vectors. Must have at least as many elements
 *                 as the maximum index used in branches.
 * @return A vector of branch lengths, one per branch in the input order.
 */
std::vector<double> computeBranchLengths(
    const std::vector<std::vector<int>>& branches,
    const std::vector<double>& x,
    const std::vector<double>& y,
    const std::vector<double>& z) {
    
    std::vector<double> lengths;
    lengths.reserve(branches.size());
    
    for (const auto& branch : branches) {
        double branchLength = 0.0;
        const std::size_t n = branch.size();
        
        // No points or a single point contribute zero length.
        if (n >= 2) {
            for (std::size_t j = 0; j + 1 < n; ++j) {
                // Convert 1-based to 0-based index.
                const int idx0 = branch[j] - 1;
                const int idx1 = branch[j + 1] - 1;
                
                const double dx = x[idx1] - x[idx0];
                const double dy = y[idx1] - y[idx0];
                const double dz = z[idx1] - z[idx0];
                
                // If any coordinate involved is NaN, skip this segment.
                if (std::isnan(dx) || std::isnan(dy) || std::isnan(dz)) {
                    continue;
                }
                
                const double dist = std::sqrt(dx*dx + dy*dy + dz*dz);
                branchLength += dist;
            }
        }
        
        lengths.push_back(branchLength);
    }
    
    return lengths;
}

// The approach is straightforward: iterate over each branch definition in the outer loop. For each branch, retrieve its list of 1-based indices. If the list has fewer than two indices, the branch contributes zero length. Otherwise, for each consecutive pair (i, i+1) in the list, compute the 3D Euclidean distance using the coordinates at those indices (after subtracting 1 to convert to 0-based indexing). If any of the three coordinate values involved is NaN (use `std::isnan`), skip that segment entirely. Otherwise, add the distance to the running sum for that branch. The key edge cases include empty branches (returns 0), branches with a single point (returns 0), repeated indices (distance 0, harmless), and NaN handling per segment so that one bad segment does not invalidate the whole branch. Using `std::isnan` from `<cmath>` is preferred over the R-specific `ISNAN`. The time complexity is O(B * L) where B is the number of branches and L is the average number of indices per branch (i.e., total number of index pairs processed). Space complexity is O(B) for the output vector, plus constant extra space.
