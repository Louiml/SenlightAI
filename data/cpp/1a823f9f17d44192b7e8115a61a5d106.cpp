Write a C++ function named `computePeakGroundVelocity` that processes a 3D seismic wave field grid and populates two 2D peak-ground-velocity arrays (horizontal component `pgvh` and total component `pgv`). The function takes as parameters: the grid dimensions `(nx, ny, nz)` (where the actual physical grid excludes a HALO border of size 2 on each side in each dimension), three 1D float arrays `Vx`, `Vy`, `Vz` representing velocity components at each grid point in row-major order with halo padding (total size `(nx+4)*(ny+4)*(nz+4)`), and two 2D output arrays `pgvh` and `pgv` each of size `nx*ny` in row-major order, preinitialized to zero. For each grid cell `(i, j)` at the topmost physical layer `k = nz` (0-indexed physical, corresponding to padded index `k + 2`), the function computes `Vx`, `Vy`, `Vz` from the input arrays at that position, scales each by `1.0 / 300.0` (where `Cv = 300`), then updates `pgvh[i*ny + j]` to the maximum of its current value and `sqrt(Vx*Vx + Vy*Vy)`, and updates `pgv[i*ny + j]` to the maximum of its current value and `sqrt(Vx*Vx + Vy*Vy + Vz*Vz)`. The function should handle any positive `nx`, `ny`, `nz` and assume valid input arrays. Provide implementation with proper `const` correctness where applicable.

// The core algorithm iterates over the 2D plane `(i, j)` for `i` from 0 to `nx-1` and `j` from 0 to `ny-1`. For each such pair, the physical coordinate is `(i+2, j+2, nz+1)` in the padded 3D array (since `HALO=2` and the top physical layer is at `k = nz` which maps to padded index `k+2 = nz+2`, and wait – careful: the code snippet uses `k = _nz_ - HALO - 1` where `_nz_ = nz + 2*HALO` so `k = (nz+4) - 2 - 1 = nz+1`). Indeed, the physical layer index `nz-1` in 0-based physical indexing maps to padded `nz+1`? Let's verify: physical indices range from `HALO` to `HALO+nz-1` inclusive. The top physical layer is `HALO + nz - 1 = 2 + nz - 1 = nz+1`. So `k = nz+1`. Good. Then compute linear index in the 1D padded array: `index = (k * (nx+4) + (i+2)) * (ny+4) + (j+2)`. Access `Vx[index]`, `Vy[index]`, `Vz[index]`. Multiply each by `1.0/300.0`. Compute horizontal magnitude `Vh = sqrt(Vx^2 + Vy^2)`, total magnitude `V = sqrt(Vx^2 + Vy^2 + Vz^2)`. Update output arrays at position `pos = i*ny + j` with max. Edge cases: if input arrays contain negative values, squaring handles that; if all velocities zero, outputs remain zero; if `nx` or `ny` is zero, function does nothing; if `nz` is zero, the top layer index would be `1` (since `nz+1`), but then there is no physical layer – but the problem states to use `k = nz+1` anyway, which would access out-of-bounds if `nz=0`; assume `nz > 0`. Time complexity: O(nx*ny) per call, space O(1) additional. The function only reads input arrays and writes outputs, no allocation.

#include <cmath>
#include <cstddef>

/**
 * Compute peak ground velocity (horizontal and total) at the top surface of a 3D wave field.
 *
 * @param nx, ny, nz  Physical grid dimensions (excluding HALO padding).
 * @param Vx, Vy, Vz  Velocity component arrays with size (nx+4)*(ny+4)*(nz+4).
 * @param pgvh        Output array of size nx*ny, updated with max horizontal velocity.
 * @param pgv         Output array of size nx*ny, updated with max total velocity.
 * @param Cv          Scaling factor (default 300.0).
 */
void computePeakGroundVelocity(
    int nx, int ny, int nz,
    const float* Vx, const float* Vy, const float* Vz,
    float* pgvh, float* pgv,
    double Cv = 300.0)
{
    constexpr int HALO = 2;
    const int padded_x = nx + 2 * HALO;
    const int padded_y = ny + 2 * HALO;
    const int padded_z = nz + 2 * HALO;
    const int k = padded_z - HALO - 1;  // top physical layer in padded indexing

    const double c = 1.0 / Cv;

    for (int i = 0; i < nx; ++i) {
        for (int j = 0; j < ny; ++j) {
            const int padded_i = i + HALO;
            const int padded_j = j + HALO;

            // 1D index in row-major order with halo padding
            const int index = (k * padded_x + padded_i) * padded_y + padded_j;

            const float vx = static_cast<float>(Vx[index] * c);
            const float vy = static_cast<float>(Vy[index] * c);
            const float vz = static_cast<float>(Vz[index] * c);

            const float vh = std::sqrt(vx * vx + vy * vy);
            const float v  = std::sqrt(vx * vx + vy * vy + vz * vz);

            const int pos = i * ny + j;
            if (pgvh[pos] < vh) pgvh[pos] = vh;
            if (pgv[pos]  < v ) pgv[pos]  = v;
        }
    }
}

#include <cassert>
#include <cmath>
#include <vector>

int main() {
    const int nx = 2, ny = 3, nz = 4;
    const int pnx = nx + 4, pny = ny + 4, pnz = nz + 4;
    const int total = pnx * pny * pnz;

    // Initialize velocity arrays to zero
    std::vector<float> Vx(total, 0.0f), Vy(total, 0.0f), Vz(total, 0.0f);

    // Set non-zero velocities at top layer only, position (1,2) physical -> padded (3,4)
    // Padded k = nz+1 = 5
    int i_phys = 1, j_phys = 2;
    int idx = (5 * pnx + (i_phys+2)) * pny + (j_phys+2);
    Vx[idx] = 300.0f;  // after scaling becomes 1.0
    Vy[idx] = 400.0f;  // after scaling becomes 4/3
    Vz[idx] = 0.0f;    // total equals horizontal

    std::vector<float> pgvh(nx*ny, 0.0f), pgv(nx*ny, 0.0f);

    computePeakGroundVelocity(nx, ny, nz, Vx.data(), Vy.data(), Vz.data(), pgvh.data(), pgv.data());

    // Expected: at pos = i_phys*ny + j_phys = 1*3+2=5
    // vx=1.0, vy=4/3, vh = sqrt(1 + 16/9) = sqrt(25/9) = 5/3
    float expected_vh = 5.0f / 3.0f;
    float expected_v  = expected_vh; // vz=0
    assert(std::fabs(pgvh[5] - expected_vh) < 1e-6);
    assert(std::fabs(pgv[5]  - expected_v ) < 1e-6);

    // Other positions remain zero
    for (int i = 0; i < nx*ny; ++i) {
        if (i != 5) {
            assert(pgvh[i] == 0.0f);
            assert(pgv[i]  == 0.0f);
        }
    }

    // Test with negative velocities and total > horizontal
    Vx[idx] = -300.0f;
    Vy[idx] = 0.0f;
    Vz[idx] = 400.0f;
    pgvh.assign(nx*ny, 0.0f);
    pgv.assign(nx*ny, 0.0f);
    computePeakGroundVelocity(nx, ny, nz, Vx.data(), Vy.data(), Vz.data(), pgvh.data(), pgv.data());

    // vx = -1.0, vz = 4/3 -> vh = 1.0, v = sqrt(1 + 16/9) = 5/3
    assert(std::fabs(pgvh[5] - 1.0f) < 1e-6);
    assert(std::fabs(pgv[5]  - expected_v) < 1e-6);

    // Test with multiple calls accumulating maxima (simulate multiple timesteps)
    // Set another position to a larger velocity
    int i2 = 0, j2 = 0;
    int idx2 = (5 * pnx + (i2+2)) * pny + (j2+2);
    Vx[idx2] = 600.0f; // scaled 2.0
    Vy[idx2] = 0.0f;
    Vz[idx2] = 0.0f;
    // Call again without resetting outputs
    computePeakGroundVelocity(nx, ny, nz, Vx.data(), Vy.data(), Vz.data(), pgvh.data(), pgv.data());

    // At pos 5, previous max vh=1.0 now becomes max(1.0, 1.0)=1.0, v=max(5/3,5/3)=5/3
    // At pos 0, new vh=2.0, v=2.0
    assert(std::fabs(pgvh[5] - 1.0f) < 1e-6);
    assert(std::fabs(pgv[5]  - expected_v) < 1e-6);
    assert(std::fabs(pgvh[0] - 2.0f) < 1e-6);
    assert(std::fabs(pgv[0]  - 2.0f) < 1e-6);

    // Edge case: nx=1, ny=1, nz=1
    int nx1=1, ny1=1, nz1=1;
    int p1 = (nx1+4)*(ny1+4)*(nz1+4);
    std::vector<float> vx1(p1, 0.0f), vy1(p1, 0.0f), vz1(p1, 0.0f);
    // top layer k = nz+1 = 2, padded i=2, j=2
    int idx1 = (2 * (nx1+4) + 2) * (ny1+4) + 2;
    vx1[idx1] = 150.0f; // scaled 0.5
    std::vector<float> out1(1, 0.0f), out2(1, 0.0f);
    computePeakGroundVelocity(nx1, ny1, nz1, vx1.data(), vy1.data(), vz1.data(), out1.data(), out2.data());
    assert(std::fabs(out1[0] - 0.5f) < 1e-6);
    assert(std::fabs(out2[0] - 0.5f) < 1e-6);

    return 0;
}
