Create a C++ function that models a simplified 2D grid of cells for charge deposition and field interpolation, inspired by the given Net class. The function should accept the grid dimensions (number of cells in x and y), the physical size of the domain (width and height), and a list of charged particles (each with x, y coordinates and charge). It must compute and return the electric field components at the center of each cell, using the same nearest-node charge assignment and a simple finite-difference-like field calculation as in the original code, but without FFT. Specifically: assign each particle’s charge to its nearest grid node (by rounding the fractional cell index), then for each cell compute Ex and Ey as the negative gradient of the potential, where the potential at each node is the accumulated charge divided by a constant permittivity (use 1.0 for simplicity). The function should return a vector of pairs (Ex, Ey) for all cells in row-major order (x fastest). Handle edge cases where particles lie exactly on the upper boundary (clamp indices to the last node), and ignore any particles outside the domain. The grid has Nx by Ny cells, so there are (Nx+1) by (Ny+1) nodes.

#include <cassert>
#include <cmath>
#include <vector>

// Assume the solution function is defined above.

int main() {
    // Test 1: No particles -> all zero fields.
    {
        std::vector<Particle> particles;
        auto fields = computeCellFields(2, 2, 2.0, 2.0, particles);
        assert(fields.size() == 4);
        for (const auto& f : fields) {
            assert(f.first == 0.0 && f.second == 0.0);
        }
    }

    // Test 2: Single particle at center of a 1x1 grid.
    {
        std::vector<Particle> particles = {{1.0, 1.0, 2.0}};
        auto fields = computeCellFields(1, 1, 2.0, 2.0, particles);
        // Grid: 1 cell, 2x2 nodes. Particle at (1,1) is nearest to node (0.5,0.5) rounded to (1,1)? Actually nx=1, so fractional: 1/2*1=0.5, round to 1 (since 0.5 rounds to 1). Clamped to 1 which is valid (node index 1). So charge at node (1,1). Ex = -(phi[1][1] - phi[0][1])/1 = -(2 - 0) = -2. Ey = -(phi[1][1] - phi[1][0])/1 = -(2 - 0) = -2.
        assert(std::abs(fields[0].first - (-2.0)) < 1e-9);
        assert(std::abs(fields[0].second - (-2.0)) < 1e-9);
    }

    // Test 3: Particle on the upper boundary.
    {
        std::vector<Particle> particles = {{2.0, 1.0, 3.0}}; // x on right boundary, y in middle of height 2.
        auto fields = computeCellFields(2, 2, 2.0, 2.0, particles);
        // Nx=2, Ny=2, nodes 3x3. fx = 2/2*2 = 2, round = 2, clamp to 2. fy = 1/2*2 = 1, round = 1. So charge at node (i=2,j=1). Only cell (i=1,j=0) and (i=1,j=1)? Actually Ex for cell (1,0) uses phi[0][2]-phi[0][1] = 0 - 0 = 0, but phi[1][1]? Wait need to check exact indices. Cell (i=1,j=0): Ex = -(phi[0][2]-phi[0][1]) = 0, Ey = -(phi[1][1]-phi[0][1]) = -(0-0)=0. Cell (i=1,j=1): Ex = -(phi[1][2]-phi[1][1]) = -(0-0)=0, Ey = -(phi[2][1]-phi[1][1]) = -(0-0)=0. Actually all fields are zero because charge is at rightmost node, no forward difference includes it. That's correct per the simple scheme.
        for (const auto& f : fields) {
            assert(f.first == 0.0 && f.second == 0.0);
        }
    }

    // Test 4: Particle at lower-left corner.
    {
        std::vector<Particle> particles = {{0.0, 0.0, 1.0}};
        auto fields = computeCellFields(2, 2, 4.0, 4.0, particles);
        // Node (0,0) gets charge 1. Cell (0,0): Ex = -(phi[0][1]-phi[0][0])/2 = -(0-1)/2 = 0.5? Wait phi[0][0]=1, phi[0][1]=0, so Ex = -(0-1)/2 = 0.5. Ey = -(phi[1][0]-phi[0][0])/2 = -(0-1)/2 = 0.5. Check.
        assert(std::abs(fields[0].first - 0.5) < 1e-9);
        assert(std::abs(fields[0].second - 0.5) < 1e-9);
    }

    // Test 5: Multiple particles summing charges.
    {
        std::vector<Particle> particles = {{0.5, 0.5, 2.0}, {0.5, 0.5, 3.0}};
        // Both at same location, total charge 5 at node (1,1) for a 2x2 grid? Let's compute: width=2, height=2, Nx=2, Ny=2. fx=0.5/2*2=0.5, round=1 (round half up in C++). fy=0.5/2*2=0.5, round=1. So node (1,1) gets 5. Cell (0,0): Ex = -(phi[0][1]-phi[0][0])/1 = -(0-0)=0, Ey = -(phi[1][0]-phi[0][0])/1 = -(0-0)=0. Cell (0,1): Ex = -(phi[1][1]-phi[1][0])/1 = -(5-0)=-5, Ey = -(phi[2][0]-phi[1][0])/1 = -(0-0)=0. Cell (1,0): Ex = -(phi[0][2]-phi[0][1])/1 = 0, Ey = -(phi[1][1]-phi[0][1])/1 = -(5-0)=-5. Cell (1,1): Ex = -(phi[1][2]-phi[1][1])/1 = -(0-5)=5, Ey = -(phi[2][1]-phi[1][1])/1 = -(0-5)=5.
        auto fields = computeCellFields(2, 2, 2.0, 2.0, particles);
        assert(fields.size() == 4);
        assert(std::abs(fields[1].first - (-5.0)) < 1e-9); // cell (0,1) is index 1? Row-major: cells (0,0), (0,1), (1,0), (1,1) in order (x fastest). Yes.
        assert(std::abs(fields[2].second - (-5.0)) < 1e-9); // cell (1,0) index 2.
        assert(std::abs(fields[3].first - 5.0) < 1e-9);
        assert(std::abs(fields[3].second - 5.0) < 1e-9);
    }

    return 0;
}

#include <vector>
#include <utility>
#include <cmath>
#include <algorithm>

struct Particle {
    double x;
    double y;
    double q;
};

// Returns (Ex, Ey) for each cell in row-major order (x varies fastest).
std::vector<std::pair<double, double>> computeCellFields(
    int Nx, int Ny, double width, double height,
    const std::vector<Particle>& particles)
{
    const int nodeCountX = Nx + 1;
    const int nodeCountY = Ny + 1;
    const double cellWidth = width / Nx;
    const double cellHeight = height / Ny;
    const double epsilon = 1.0;

    // Node charges initialized to zero.
    std::vector<std::vector<double>> nodeCharge(nodeCountY, std::vector<double>(nodeCountX, 0.0));

    // Deposit charges to nearest nodes.
    for (const auto& p : particles) {
        // Skip particles outside the domain (allow exactly on boundaries).
        if (p.x < 0.0 || p.x > width || p.y < 0.0 || p.y > height) continue;

        double fx = p.x / width * Nx;
        double fy = p.y / height * Ny;

        int ix = static_cast<int>(std::round(fx));
        int iy = static_cast<int>(std::round(fy));

        // Clamp to valid node indices (for particles on the upper/right edge).
        ix = std::clamp(ix, 0, Nx);
        iy = std::clamp(iy, 0, Ny);

        nodeCharge[iy][ix] += p.q;
    }

    // Compute potentials at nodes.
    std::vector<std::vector<double>> phi(nodeCountY, std::vector<double>(nodeCountX, 0.0));
    for (int j = 0; j < nodeCountY; ++j) {
        for (int i = 0; i < nodeCountX; ++i) {
            phi[j][i] = nodeCharge[j][i] / epsilon;
        }
    }

    // Compute fields at each cell using forward difference.
    std::vector<std::pair<double, double>> cellFields;
    cellFields.reserve(Nx * Ny);
    for (int j = 0; j < Ny; ++j) {
        for (int i = 0; i < Nx; ++i) {
            double Ex = -(phi[j][i+1] - phi[j][i]) / cellWidth;
            double Ey = -(phi[j+1][i] - phi[j][i]) / cellHeight;
            cellFields.emplace_back(Ex, Ey);
        }
    }

    return cellFields;
}

// The solution involves three main steps: constructing the grid, depositing charges to nodes, and computing fields on cells. For each particle, compute the fractional position in cell units: fx = x / width * Nx, fy = y / height * Ny. The nearest node index is round(fx) and round(fy), but clamp to [0, Nx] and [0, Ny] for particles on the upper boundary. Accumulate the particle’s charge into a 2D array of node charges. After all particles are processed, compute the potential at each node as charge / epsilon (epsilon=1). For each cell (i,j), compute Ex = -(phi[i+1][j] - phi[i][j]) / (width/Nx) and Ey = -(phi[i][j+1] - phi[i][j]) / (height/Ny) using forward differences (which matches the physical meaning of field at the cell’s lower-left corner, but we can treat it as the cell center approximation). Edge cases: if a particle lies exactly on the right or top boundary, its rounded index may equal Nx or Ny, which are valid node indices (since we have Nx+1 nodes). Particles outside [0,width] or [0,height] are skipped. Time complexity is O(P + Nx*Ny) where P is the number of particles, and space complexity is O(Nx*Ny) for node charges and cell fields.
