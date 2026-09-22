// Write a standalone C++ function `compute2ndDerivative` that computes the second derivative \( \frac{\partial^2 f}{\partial x \, \partial y} \) for a scalar field \( f \) discretized on a uniform Cartesian grid. The function receives a 2D `std::vector<std::vector<double>>` representing field values at grid nodes (where `field[i][j]` is the value at node \((x_i, y_j)\)), a grid spacing `dx` for the x-direction, a grid spacing `dy` for the y-direction, and node indices `i` and `j` (with `0 <= i < field.size()` and `0 <= j < field[0].size()`). The function returns the mixed partial derivative using central differences:  
// \[
// \frac{\partial^2 f}{\partial x \partial y}(i,j) = \frac{f(i+1,j+1) - f(i-1,j+1) - f(i+1,j-1) + f(i-1,j-1)}{4 \, dx \, dy}.
// \]  
// Assume the grid has at least 3 rows and 3 columns, and the requested indices are strictly interior (i.e., `1 <= i <= size_x-2`, `1 <= j <= size_y-2`), so no boundary handling is needed. The function must be `const`-correct, take the field by `const` reference, and be implemented as a free function named `computeMixedSecondDerivative`.

The solution directly implements the central difference formula for the mixed partial derivative. Given a uniform grid with spacing `dx` in x and `dy` in y, the cross-derivative at an interior point can be approximated by evaluating the function at the four corners of a cell of size \(2dx \times 2dy\) centered at \((i,j)\). The formula uses a 4-point stencil: \(f(i+1,j+1)\), \(f(i-1,j+1)\), \(f(i+1,j-1)\), \(f(i-1,j-1)\). The denominator is \(4 \, dx \, dy\) because the stencil spans two grid spacings in each direction. Edge cases: because the problem guarantees interior indices and grid sizes ≥3×3, no boundary checks are required. If `dx` or `dy` are zero, division would be undefined, but typical inputs have positive spacings. Time complexity is \(O(1)\) since only four constant-time array accesses and arithmetic operations are performed; space complexity is \(O(1)\) as no extra storage is allocated.

#include <vector>
#include <stdexcept>

// Compute the mixed partial derivative ∂²f/∂x∂y at interior node (i,j) using central differences.
double computeMixedSecondDerivative(
    const std::vector<std::vector<double>>& field,
    double dx,
    double dy,
    int i,
    int j
) {
    if (dx <= 0.0 || dy <= 0.0) {
        throw std::invalid_argument("Grid spacings must be positive");
    }
    if (field.size() < 3 || field[0].size() < 3) {
        throw std::invalid_argument("Grid must have at least 3 rows and 3 columns");
    }
    if (i < 1 || i >= static_cast<int>(field.size()) - 1 ||
        j < 1 || j >= static_cast<int>(field[0].size()) - 1) {
        throw std::out_of_range("Indices must be strictly interior");
    }

    return (field[i + 1][j + 1] - field[i - 1][j + 1] -
            field[i + 1][j - 1] + field[i - 1][j - 1]) / (4.0 * dx * dy);
}

#include <cassert>
#include <vector>

// The solution function is assumed to be declared above; here we test it.
int main() {
    // Simple linear field f(x,y) = 3*x + 5*y, with dx=1, dy=1 => mixed derivative = 0.
    std::vector<std::vector<double>> linear = {
        {0, 1, 2},
        {3, 4, 5},
        {6, 7, 8}
    };
    // For field values = 3*i + 5*j (using index spacing 1), any interior: derivative = 0.
    assert(computeMixedSecondDerivative(linear, 1.0, 1.0, 1, 1) == 0.0);

    // Quadratic field f(x,y) = x*y, with dx=1, dy=1, values = i*j.
    std::vector<std::vector<double>> bilinear = {
        {0, 0, 0},
        {0, 1, 2},
        {0, 2, 4}
    };
    // ∂²(xy)/∂x∂y = 1. Central difference with dx=dy=1 yields 1.0.
    assert(computeMixedSecondDerivative(bilinear, 1.0, 1.0, 1, 1) == 1.0);

    // Non-unit spacing: f(x,y) = x²*y, dx=2, dy=0.5. At interior point corresponding to i=1,j=1 with actual x=2, y=0.5, derivative = 2*x = 4? Actually ∂²(x²y)/∂x∂y = 2x. For x=2 (i=1, dx=2), derivative = 4.
    // Build grid with dx=2, dy=0.5, indices i (0..2) -> x = i*2, j (0..2) -> y = j*0.5.
    std::vector<std::vector<double>> nonUniform = {
        {0.0, 0.0, 0.0},            // x=0
        {0.0, 1.0, 4.0},            // x=2: y=0 ->0, y=0.5 -> (4)*(0.5)=2? Wait x²*y = (2²)*(0.5)=2, but I wrote 1? Let's compute correctly.
    };
    // Let's construct correctly: values f = (i*dx)^2 * (j*dy). dx=2, dy=0.5.
    // i=0,1,2; j=0,1,2.
    // f(i,j) = (2i)^2 * (0.5j) = 4i² * 0.5j = 2 i² j.
    std::vector<std::vector<double>> grid = {
        {0.0, 0.0, 0.0},  // i=0
        {0.0, 2.0, 4.0},  // i=1: 2*1*1=2, 2*1*2=4
        {0.0, 8.0, 16.0}  // i=2: 2*4*1=8, 2*4*2=16
    };
    // At i=1,j=1: expected mixed derivative = 2*x = 2*(2) = 4? Actually ∂²(x²y)/∂x∂y = 2x, x=2 => 4.
    assert(computeMixedSecondDerivative(grid, 2.0, 0.5, 1, 1) == 4.0);

    // Symmetry: swapping x and y roles yields same? Not needed.

    // Test that it throws for invalid interior index.
    bool threw = false;
    try {
        computeMixedSecondDerivative(grid, 2.0, 0.5, 0, 1);
    } catch (const std::out_of_range&) {
        threw = true;
    }
    assert(threw);
}
