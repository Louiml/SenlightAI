Write a standalone C++ function that models the behavior of the identity mapping for a grid of points in `dim`-dimensional space, where each point may have `codim` components (with `codim >= dim`). The function should take a vector of input points (each represented as `std::array<double, codim>` where the last `codim - dim` components are always zero on input) and produce three output vectors: the mapped values (identical to input for the first `dim` components, zero for the remaining components), the gradients (which for an identity mapping are the identity matrix of size `dim x dim` at every point, stored as `std::array<std::array<double, dim>, dim>`), and the Hessians (all zeros, represented as `std::array<std::array<std::array<double, dim>, dim>, dim>`). The function must handle empty input gracefully (all output vectors empty). The signature should be: `void identity_mapping_evaluate(const std::vector<std::array<double, codim>>& points, std::vector<std::array<double, codim>>& values, std::vector<std::array<std::array<double, dim>, dim>>& gradients, std::vector<std::array<std::array<std::array<double, dim>, dim>, dim>>& hessians)`, templated on `int dim` and `int codim` with compile-time static assertions that `codim >= dim` and `dim >= 1`. The function must not use any external libraries beyond standard headers and must be self-contained.

#include <cassert>
#include <vector>
#include <array>
#include <cstddef>

// Include the solution function here (or link accordingly)
// (Declared above)

int main() {
    // Test 1: dim=2, codim=3, multiple points
    {
        constexpr int dim = 2;
        constexpr int codim = 3;
        std::vector<std::array<double, codim>> points = {
            {1.0, 2.0, 0.0},
            {-3.0, 4.5, 0.0},
            {0.0, 0.0, 0.0}
        };
        std::vector<std::array<double, codim>> values;
        std::vector<std::array<std::array<double, dim>, dim>> gradients;
        std::vector<std::array<std::array<std::array<double, dim>, dim>, dim>> hessians;
        
        identity_mapping_evaluate<dim, codim>(points, values, gradients, hessians);
        
        assert(values.size() == 3);
        assert(gradients.size() == 3);
        assert(hessians.size() == 3);
        
        // Check values
        assert(values[0][0] == 1.0 && values[0][1] == 2.0 && values[0][2] == 0.0);
        assert(values[1][0] == -3.0 && values[1][1] == 4.5 && values[1][2] == 0.0);
        assert(values[2][0] == 0.0 && values[2][1] == 0.0 && values[2][2] == 0.0);
        
        // Check gradients (identity matrix for each point)
        for (const auto& g : gradients) {
            for (int i = 0; i < dim; ++i)
                for (int j = 0; j < dim; ++j)
                    assert(g[i][j] == (i == j ? 1.0 : 0.0));
        }
        
        // Check hessians (all zeros for each point)
        for (const auto& h : hessians) {
            for (int i = 0; i < dim; ++i)
                for (int j = 0; j < dim; ++j)
                    for (int k = 0; k < dim; ++k)
                        assert(h[i][j][k] == 0.0);
        }
    }
    
    // Test 2: dim=3, codim=3 (no extra dimensions)
    {
        constexpr int dim = 3;
        constexpr int codim = 3;
        std::vector<std::array<double, codim>> points = {
            {1.0, 2.0, 3.0},
            {-1.0, -2.0, -3.0}
        };
        std::vector<std::array<double, codim>> values;
        std::vector<std::array<std::array<double, dim>, dim>> gradients;
        std::vector<std::array<std::array<std::array<double, dim>, dim>, dim>> hessians;
        
        identity_mapping_evaluate<dim, codim>(points, values, gradients, hessians);
        
        assert(values.size() == 2);
        assert(values[0][0] == 1.0 && values[0][1] == 2.0 && values[0][2] == 3.0);
        assert(values[1][0] == -1.0 && values[1][1] == -2.0 && values[1][2] == -3.0);
        
        for (const auto& g : gradients) {
            assert(g[0][0] == 1.0 && g[0][1] == 0.0 && g[0][2] == 0.0);
            assert(g[1][0] == 0.0 && g[1][1] == 1.0 && g[1][2] == 0.0);
            assert(g[2][0] == 0.0 && g[2][1] == 0.0 && g[2][2] == 1.0);
        }
        
        for (const auto& h : hessians) {
            for (int i = 0; i < dim; ++i)
                for (int j = 0; j < dim; ++j)
                    for (int k = 0; k < dim; ++k)
                        assert(h[i][j][k] == 0.0);
        }
    }
    
    // Test 3: Empty input
    {
        constexpr int dim = 1;
        constexpr int codim = 2;
        std::vector<std::array<double, codim>> points;
        std::vector<std::array<double, codim>> values;
        std::vector<std::array<std::array<double, dim>, dim>> gradients;
        std::vector<std::array<std::array<std::array<double, dim>, dim>, dim>> hessians;
        
        identity_mapping_evaluate<dim, codim>(points, values, gradients, hessians);
        
        assert(values.empty());
        assert(gradients.empty());
        assert(hessians.empty());
    }
    
    // Test 4: dim=1, codim=4, single point
    {
        constexpr int dim = 1;
        constexpr int codim = 4;
        std::vector<std::array<double, codim>> points = {{7.5, 0.0, 0.0, 0.0}};
        std::vector<std::array<double, codim>> values;
        std::vector<std::array<std::array<double, dim>, dim>> gradients;
        std::vector<std::array<std::array<std::array<double, dim>, dim>, dim>> hessians;
        
        identity_mapping_evaluate<dim, codim>(points, values, gradients, hessians);
        
        assert(values.size() == 1);
        assert(values[0][0] == 7.5);
        assert(values[0][1] == 0.0 && values[0][2] == 0.0 && values[0][3] == 0.0);
        assert(gradients[0][0][0] == 1.0);
        assert(hessians[0][0][0][0] == 0.0);
    }
    
    return 0;
}

#include <vector>
#include <array>
#include <cstddef>
#include <type_traits>

/**
 * @brief Evaluates an identity mapping on a set of points.
 * 
 * For each input point (with `dim` coordinates embedded in `codim`-dimensional space),
 * produces:
 * - values: identical to input for first `dim` components, zero for remaining `codim-dim`
 * - gradients: dim x dim identity matrix at each point
 * - hessians: dim x dim x dim zero tensor at each point
 * 
 * @tparam dim    Dimension of the domain (must be >= 1)
 * @tparam codim  Dimension of the codomain (must be >= dim)
 */
template<int dim, int codim>
void identity_mapping_evaluate(
    const std::vector<std::array<double, codim>>& points,
    std::vector<std::array<double, codim>>& values,
    std::vector<std::array<std::array<double, dim>, dim>>& gradients,
    std::vector<std::array<std::array<std::array<double, dim>, dim>, dim>>& hessians)
{
    static_assert(dim >= 1, "dim must be at least 1");
    static_assert(codim >= dim, "codim must be >= dim");

    const std::size_t num_points = points.size();

    // Resize output vectors to match input size
    values.resize(num_points);
    gradients.resize(num_points);
    hessians.resize(num_points);

    if (num_points == 0) {
        return; // Nothing to do for empty input
    }

    // Precompute constant identity matrix for gradients
    std::array<std::array<double, dim>, dim> identity{};
    for (int i = 0; i < dim; ++i) {
        identity[i][i] = 1.0;
    }

    // Precompute constant zero tensor for hessians (already zero-initialized by default)
    std::array<std::array<std::array<double, dim>, dim>, dim> zero_hessian{};
    // (All elements default-initialized to 0.0)

    for (std::size_t i = 0; i < num_points; ++i) {
        const auto& x = points[i];

        // Values: copy first dim components, zero the rest
        for (int k = 0; k < dim; ++k) {
            values[i][k] = x[k];
        }
        for (int k = dim; k < codim; ++k) {
            values[i][k] = 0.0;
        }

        // Gradients: identity matrix
        gradients[i] = identity;

        // Hessians: all zeros
        hessians[i] = zero_hessian;
    }
}

// The solution directly implements the mathematical definition of an identity mapping: for each input point `x`, the mapped value is `F(x) = x` for the first `dim` components (since the input's first `dim` components are the actual coordinates) and zero for the remaining `codim - dim` components (embedding into higher-dimensional space). The gradient of the identity mapping is constant and equal to the `dim x dim` identity matrix for every point, because the derivative of `F(x) = x` with respect to `x` is the identity. The Hessian (second derivative) is identically zero for all points. The algorithm is straightforward: first, resize all output vectors to match the input size (or clear them if input is empty). For each point, copy the first `dim` components into the corresponding value output, set the remaining components to zero (which also works correctly if `codim == dim`). Then fill the gradient vector with an identity matrix (a precomputed constant) and the Hessian vector with all zeros. Important edge cases: empty input (must not crash, so check `points.empty()` and clear outputs), and when `codim == dim`, the zero-fill loop for the value components is skipped (but the code should handle it correctly). Time complexity is `O(n * max(dim, codim, dim^2, dim^3))`, where `n` is the number of points, but effectively linear in `n` with a constant factor depending on the fixed dimensions. Space complexity is `O(n * (codim + dim^2 + dim^3))` for the output vectors, plus `O(1)` auxiliary for the constant matrices.
