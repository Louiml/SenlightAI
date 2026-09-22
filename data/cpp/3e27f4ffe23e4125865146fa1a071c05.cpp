/*
Write a C++ function that computes the total volume of a given deformable 8-node hexahedral (brick) element. The function takes the coordinates of the 8 nodes as a `std::array<std::array<double,3>,8>` (or equivalent), evaluates the Jacobian determinant at each of 8 Gauss points (using 2-point Gauss-Legendre quadrature in each direction, with quadrature points at ±1/√3 and equal weights of 1.0), and returns the integrated volume as the sum over all Gauss points of (weight × Jacobian determinant). Use standard trilinear shape functions for the hexahedron: N_i(ξ,η,ζ) = (1/8)(1+ξξ_i)(1+ηη_i)(1+ζζ_i), where (ξ_i,η_i,ζ_i) are the local coordinates of node i (±1 in each direction). The Jacobian matrix at a Gauss point is the 3×3 matrix J where J(row, col) = Σ_i (∂N_i/∂local_col) × x_coord_of_node_i(row), and the determinant is computed accordingly. The volume is the sum of |det(J)| × w_gauss for all 8 Gauss points. The function must be robust to node ordering (assume standard OpenSees brick node ordering: bottom face nodes 1-4 counterclockwise, top face nodes 5-8 directly above), and should handle degenerate or inverted elements by taking the absolute value of each determinant contribution.
*/

#include <array>
#include <cmath>

// Compute the volume of an 8-node hexahedral brick element using
// 2x2x2 Gauss-Legendre quadrature with trilinear shape functions.
// Node ordering: bottom face nodes 0-3 counterclockwise, top face nodes 4-7 above.
double brickVolume(const std::array<std::array<double,3>,8>& nodes) {
    const double one_over_root3 = 1.0 / std::sqrt(3.0);
    
    // Local coordinates of the 8 Gauss points (ξ, η, ζ) in {-1/√3, +1/√3}^3
    const double gp[2] = { -one_over_root3, one_over_root3 };
    
    // Local coordinates of the 8 nodes: (ξ_i, η_i, ζ_i) each in {-1, +1}
    // Standard ordering: bottom face (z=-1), then top face (z=+1)
    const double nodeLocal[8][3] = {
        {-1, -1, -1}, {1, -1, -1}, {1, 1, -1}, {-1, 1, -1},
        {-1, -1,  1}, {1, -1,  1}, {1, 1,  1}, {-1, 1,  1}
    };
    
    double totalVolume = 0.0;
    
    // Loop over all 8 Gauss points
    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 2; ++j) {
            for (int k = 0; k < 2; ++k) {
                double xi = gp[i];
                double eta = gp[j];
                double zeta = gp[k];
                
                // Shape function values and derivatives at this Gauss point
                double N[8];
                double dN_dxi[8];
                double dN_deta[8];
                double dN_dzeta[8];
                
                for (int n = 0; n < 8; ++n) {
                    double xi_n = nodeLocal[n][0];
                    double eta_n = nodeLocal[n][1];
                    double zeta_n = nodeLocal[n][2];
                    
                    N[n] = 0.125 * (1.0 + xi * xi_n) * (1.0 + eta * eta_n) * (1.0 + zeta * zeta_n);
                    dN_dxi[n] = 0.125 * xi_n * (1.0 + eta * eta_n) * (1.0 + zeta * zeta_n);
                    dN_deta[n] = 0.125 * (1.0 + xi * xi_n) * eta_n * (1.0 + zeta * zeta_n);
                    dN_dzeta[n] = 0.125 * (1.0 + xi * xi_n) * (1.0 + eta * eta_n) * zeta_n;
                }
                
                // Build Jacobian matrix: J[global_row][local_col]
                // global rows: x, y, z; local cols: ξ, η, ζ
                double J[3][3] = {{0.0}};
                for (int r = 0; r < 3; ++r) { // global coordinate index (0=x,1=y,2=z)
                    for (int c = 0; c < 3; ++c) { // local coordinate index (0=ξ,1=η,2=ζ)
                        double sum = 0.0;
                        for (int n = 0; n < 8; ++n) {
                            double dN;
                            if (c == 0) dN = dN_dxi[n];
                            else if (c == 1) dN = dN_deta[n];
                            else dN = dN_dzeta[n];
                            sum += dN * nodes[n][r];
                        }
                        J[r][c] = sum;
                    }
                }
                
                // Compute determinant of 3x3 matrix J
                double detJ = 
                    J[0][0] * (J[1][1]*J[2][2] - J[1][2]*J[2][1]) -
                    J[0][1] * (J[1][0]*J[2][2] - J[1][2]*J[2][0]) +
                    J[0][2] * (J[1][0]*J[2][1] - J[1][1]*J[2][0]);
                
                // Weight is 1.0 for each Gauss point in 2-point rule
                totalVolume += std::fabs(detJ);
            }
        }
    }
    
    return totalVolume;
}

#include <cassert>
#include <cmath>
#include <array>

// Forward declaration of the solution function
double brickVolume(const std::array<std::array<double,3>,8>& nodes);

int main() {
    // Test 1: Unit cube (nodes: bottom face at z=0, top at z=1)
    std::array<std::array<double,3>,8> cube = {{
        {0,0,0}, {1,0,0}, {1,1,0}, {0,1,0},
        {0,0,1}, {1,0,1}, {1,1,1}, {0,1,1}
    }};
    double vol = brickVolume(cube);
    assert(std::fabs(vol - 1.0) < 1e-12);

    // Test 2: Rectangular box 2x3x4 = 24
    std::array<std::array<double,3>,8> box = {{
        {0,0,0}, {2,0,0}, {2,3,0}, {0,3,0},
        {0,0,4}, {2,0,4}, {2,3,4}, {0,3,4}
    }};
    vol = brickVolume(box);
    assert(std::fabs(vol - 24.0) < 1e-10);

    // Test 3: Sheared parallelepiped (det of edge vectors = 12)
    // Edge vectors: a=(2,0,0), b=(1,3,0), c=(0,0,2)
    std::array<std::array<double,3>,8> parallelepiped = {{
        {0,0,0}, {2,0,0}, {3,3,0}, {1,3,0},
        {0,0,2}, {2,0,2}, {3,3,2}, {1,3,2}
    }};
    vol = brickVolume(parallelepiped);
    assert(std::fabs(vol - 12.0) < 1e-10);

    // Test 4: Inverted element (opposite node ordering) should still give positive volume
    std::array<std::array<double,3>,8> inverted = {{
        {1,1,1}, {0,1,1}, {0,0,1}, {1,0,1},
        {1,1,0}, {0,1,0}, {0,0,0}, {1,0,0}
    }};
    vol = brickVolume(inverted);
    assert(std::fabs(vol - 1.0) < 1e-12);

    // Test 5: Degenerate element (all nodes in a plane => zero volume)
    std::array<std::array<double,3>,8> flat = {{
        {0,0,0}, {1,0,0}, {1,1,0}, {0,1,0},
        {0,0,0}, {1,0,0}, {1,1,0}, {0,1,0}
    }};
    vol = brickVolume(flat);
    assert(vol < 1e-12);

    // Test 6: Very small element (edge length 0.001)
    double s = 0.001;
    std::array<std::array<double,3>,8> small = {{
        {0,0,0}, {s,0,0}, {s,s,0}, {0,s,0},
        {0,0,s}, {s,0,s}, {s,s,s}, {0,s,s}
    }};
    vol = brickVolume(small);
    assert(std::fabs(vol - s*s*s) < 1e-12);

    // Test 7: Non-aligned skewed element, volume = 5
    std::array<std::array<double,3>,8> skewed = {{
        {1,2,0}, {3,2,0}, {4,5,0}, {2,5,0},
        {1,2,5}, {3,2,5}, {4,5,5}, {2,5,5}
    }};
    vol = brickVolume(skewed);
    assert(std::fabs(vol - 25.0) < 1e-10);

    return 0;
}

// The solution uses isoparametric mapping from a reference cube [-1,1]³ to the physical brick. For each of the 8 Gauss points (all combinations of ξ,η,ζ = ±1/√3), we evaluate the 8 shape functions and their derivatives with respect to the local coordinates (ξ,η,ζ). The derivatives are simple: for node i, ∂N_i/∂ξ = (ξ_i/8)(1+ηη_i)(1+ζζ_i), and analogously for η and ζ. The Jacobian matrix is then assembled: J(r,c) = Σ_i (∂N_i/∂ξ_c) × node_i_coord(r) for r,c ∈ {0,1,2} mapping global x,y,z to local ξ,η,ζ. The determinant of this 3×3 matrix is computed using the standard formula. The volume element at each Gauss point is w_gauss × |det(J)|, where each of the 2-point quadrature weights is 1.0, so the total volume is the sum of the 8 absolute determinant values. Edge cases: the element may be undistorted (unit cube gives volume 1.0), inverted (negative determinant, handled by absolute value), or degenerate (zero determinant for a collapsed face, handled naturally). The algorithm is O(1) with respect to input size (fixed 8 nodes, 8 Gauss points), requiring O(1) auxiliary space for the Jacobian and intermediate values. The use of absolute value ensures positive volume even for incorrectly ordered nodes. Precision is double, so floating-point comparisons in tests should use approximate equality where appropriate.
