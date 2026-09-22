Implement a C++ function `bilinearInterpolation` that performs bilinear interpolation on a uniform 2D grid of function values. Given two 1D coordinate vectors `x` and `y` (both strictly increasing, with at least 2 points each), a 2D matrix `f` of values `f[i][j]` corresponding to `(x[i], y[j])`, and a query point `(xq, yq)` that lies strictly inside the grid (not on the boundary), return the interpolated value using the standard bilinear formula: `1/((x2-x1)*(y2-y1)) * (f11*(x2-xq)*(y2-yq) + f21*(xq-x1)*(y2-yq) + f12*(x2-xq)*(yq-y1) + f22*(xq-x1)*(yq-y1))`, where `(x1,y1)`, `(x2,y2)` are the grid cell corners and `f11=f[i][j]`, `f21=f[i+1][j]`, `f12=f[i][j+1]`, `f22=f[i+1][j+1]`. Find the correct cell `(i,j)` such that `x[i] <= xq <= x[i+1]` and `y[j] <= yq <= y[j+1]`. If the query point is on the exact boundary between cells, choose the cell with the smaller index. If the point is outside the grid, throw `std::out_of_range`. The function should be `const`-correct and take references to the vectors and the matrix (as `std::vector<std::vector<double>>`), returning a `double`. Use `std::lower_bound` to locate the cell indices efficiently.

The main algorithm involves two steps: locating the grid cell and performing bilinear interpolation. First, for a given query `(xq, yq)`, find the index `i` such that `x[i] <= xq < x[i+1]` and similarly for `j` with `y`. Use `std::lower_bound` on the `x` vector to find the first element not less than `xq`, then adjust to get the cell start index. If `xq` equals an interior grid point, `lower_bound` returns the index of that point, but we want the cell to the left (index-1) to handle boundary conditions. Since the spec says the point is strictly inside the grid (not on the outer boundary), we can safely adjust. A robust approach: compute `i = std::lower_bound(x.begin(), x.end(), xq) - x.begin() - 1;` then clamp to ensure `0 <= i < x.size()-1`. Similarly for `j`. Then compute the interpolation using the four corner values. Edge cases: if the point is exactly on a vertical or horizontal grid line, the formula still works correctly because the weights for degenerate dimensions (where `xq == x1` or `xq == x2`) become zero or one naturally. Time complexity is O(log n) for `lower_bound` plus O(1) for interpolation, so O(log n + log m). Space complexity is O(1) beyond the input. If the point is outside the grid entirely, throw `std::out_of_range`.

#include <vector>
#include <stdexcept>
#include <algorithm>

// Perform bilinear interpolation on a uniform grid.
// x and y are strictly increasing coordinate vectors of size at least 2.
// f is a 2D vector with dimensions x.size() x y.size(), where f[i][j] corresponds to (x[i], y[j]).
// Query point (xq, yq) must lie strictly inside the grid domain.
double bilinearInterpolation(const std::vector<double>& x,
                             const std::vector<double>& y,
                             const std::vector<std::vector<double>>& f,
                             double xq, double yq) {
    // Validate grid dimensions
    if (x.size() < 2 || y.size() < 2 || f.size() != x.size()) {
        throw std::invalid_argument("Grid dimensions are invalid");
    }
    for (const auto& row : f) {
        if (row.size() != y.size()) {
            throw std::invalid_argument("Matrix rows must match y size");
        }
    }

    // Locate cell indices i, j such that x[i] <= xq <= x[i+1], y[j] <= yq <= y[j+1]
    // Use lower_bound to find first element >= xq, then subtract 1 to get the left edge.
    auto itX = std::lower_bound(x.begin(), x.end(), xq);
    std::size_t i = static_cast<std::size_t>(itX - x.begin());
    if (i == 0) {
        // xq is less than all x values -> outside
        throw std::out_of_range("Query x is below the grid range");
    }
    if (i == x.size()) {
        // xq is greater than all x values -> outside
        throw std::out_of_range("Query x is above the grid range");
    }
    // If i points to the last element, it means xq >= last, which is boundary; but spec says strictly inside, so reject.
    if (i == x.size() - 1) {
        throw std::out_of_range("Query x is on the upper boundary");
    }
    // We want the cell where x[i] <= xq < x[i+1]; if xq equals x[i] exactly, this cell is correct.
    // Actually, if xq equals x[i] but i>0, we might want the cell to the left? The spec says use smaller index on exact boundary.
    // Since lower_bound returns first >=, if xq == x[i], the returned iterator is at i. We want i-1 for cell [i-1, i].
    // But if we choose i-1, then x[i-1] <= xq <= x[i] holds. Let's adjust:
    if (itX != x.end() && *itX == xq && i > 0) {
        i--; // Use the cell to the left when on exact grid point (except leftmost boundary)
    }
    // Now ensure i is in valid range [0, x.size()-2]
    if (i >= x.size() - 1) {
        throw std::out_of_range("Query x is on the upper boundary");
    }

    auto itY = std::lower_bound(y.begin(), y.end(), yq);
    std::size_t j = static_cast<std::size_t>(itY - y.begin());
    if (j == 0) {
        throw std::out_of_range("Query y is below the grid range");
    }
    if (j == y.size()) {
        throw std::out_of_range("Query y is above the grid range");
    }
    if (j == y.size() - 1) {
        throw std::out_of_range("Query y is on the upper boundary");
    }
    if (itY != y.end() && *itY == yq && j > 0) {
        j--;
    }
    if (j >= y.size() - 1) {
        throw std::out_of_range("Query y is on the upper boundary");
    }

    // Now perform bilinear interpolation
    double x1 = x[i], x2 = x[i+1];
    double y1 = y[j], y2 = y[j+1];
    double f11 = f[i][j];
    double f21 = f[i+1][j];
    double f12 = f[i][j+1];
    double f22 = f[i+1][j+1];

    double dx = x2 - x1;
    double dy = y2 - y1;
    if (dx == 0.0 || dy == 0.0) {
        throw std::invalid_argument("Grid coordinates must be strictly increasing");
    }

    double result = 1.0 / (dx * dy) * (
        f11 * (x2 - xq) * (y2 - yq) +
        f21 * (xq - x1) * (y2 - yq) +
        f12 * (x2 - xq) * (yq - y1) +
        f22 * (xq - x1) * (yq - y1)
    );
    return result;
}

#include <cassert>
#include <cmath>
#include <vector>
#include <stdexcept>

// Include the function here (or link to it)
// (Assume the function from is defined above.)

int main() {
    // Simple linear function f(x,y) = 2x + 3y on a 3x3 grid
    std::vector<double> x = {0.0, 1.0, 2.0};
    std::vector<double> y = {0.0, 1.0, 2.0};
    std::vector<std::vector<double>> f = {
        {0.0, 3.0, 6.0},
        {2.0, 5.0, 8.0},
        {4.0, 7.0, 10.0}
    }; // f[i][j] = 2*x[i] + 3*y[j]

    // Point at center of cell (0,0): xq=0.5, yq=0.5 should give 0*0.25 + 2*0.25 + 3*0.25 + 5*0.25 = 2.5
    double val = bilinearInterpolation(x, y, f, 0.5, 0.5);
    assert(std::abs(val - 2.5) < 1e-12);

    // Point exactly on grid line x=1.0, y=0.5 (between cells (0,0) and (1,0) but we choose cell (0,0) because boundary rule)
    val = bilinearInterpolation(x, y, f, 1.0, 0.5);
    // In cell (0,0): x1=0, x2=1, y1=0, y2=1
    // f11=0, f21=2, f12=3, f22=5
    // xq=1, yq=0.5 -> (2)*(0.5) + ... compute: 1/(1*1)*(0*(0)*(0.5) + 2*(1)*(0.5) + 3*(0)*(0.5) + 5*(1)*(0.5)) = (1 + 2.5) = 3.5? Actually: 2*1*0.5=1, 5*1*0.5=2.5, total 3.5
    assert(std::abs(val - 3.5) < 1e-12);

    // Point exactly on grid intersection (1,1), choose cell (0,0) because boundary rule
    val = bilinearInterpolation(x, y, f, 1.0, 1.0);
    // cell (0,0): x1=0,x2=1,y1=0,y2=1, xq=1,yq=1 -> 1/(1)*(0 + 2*1*1 + 0 + 5*1*1) = 7? Wait: f21*(1)*(1)=2, f22*(1)*(1)=5, total 7, but actual f(1,1)=2+3=5. But because we choose cell (0,0), the interpolation gives 5? Let's recalc: formula: f11*(1-1)*(1-1)=0, f21*(1-0)*(1-1)=0, f12*(1-1)*(1-0)=0, f22*(1-0)*(1-0)=5. So val=5. That's correct, actually. The boundary rule didn't change the result because on the exact point the weights for non-corner terms vanish. So assert 5.
    assert(std::abs(val - 5.0) < 1e-12);

    // Test outside range
    bool threw = false;
    try {
        bilinearInterpolation(x, y, f, 3.0, 0.5);
    } catch (const std::out_of_range&) {
        threw = true;
    }
    assert(threw);

    // Test non-square grid
    std::vector<double> x2 = {1.0, 2.0, 3.0};
    std::vector<double> y2 = {10.0, 20.0};
    std::vector<std::vector<double>> f2 = {
        {1.0, 2.0},
        {3.0, 4.0},
        {5.0, 6.0}
    }; // f(x,y) = x + 0.1*y? Actually not needed, just test linearity
    val = bilinearInterpolation(x2, y2, f2, 1.5, 15.0);
    // Cell (0,0): x1=1,x2=2,y1=10,y2=20, f11=1,f21=3,f12=2,f22=4
    // Compute: 1/(1*10)*(1*(0.5)*(5) + 3*(0.5)*(5) + 2*(0.5)*(5) + 4*(0.5)*(5)) = 1/10*(0.5*5*(1+3+2+4)) = 0.1*0.5*5*10 = 2.5
    assert(std::abs(val - 2.5) < 1e-12);

    // Test on boundary between cells but not exact point: xq=1.5, yq=10 (y on boundary)
    val = bilinearInterpolation(x2, y2, f2, 1.5, 10.0);
    // Cell (0,0): y1=10,y2=20, yq=10 so weights: (y2-yq)=10, (yq-y1)=0
    // val = 1/(1*10)*(1*(0.5)*10 + 3*(0.5)*10 + 0 + 0) = 1/10*(0.5*10*4) = 2.0? Actually 1*0.5*10=5, 3*0.5*10=15, sum=20, /10 =2.0
    assert(std::abs(val - 2.0) < 1e-12);

    return 0;
}
