// Write a C++ function that implements the CPU-side neighbor interaction loop for the RE-squared (Rutgers-Ellipsoid) anisotropic pair potential, but simplified to a generic version that processes a list of atom pairs. Given arrays for atom positions (`double** x`), atom types (`int* type`), a flattened neighbor list (where each atom `i` has `numneigh[i]` neighbors stored contiguously in `firstneigh`), per-type shape parameters (`lshape[i]` = product of the three semi-axes of type `i`), and a per-pair cutoff squared `cutsq[itype][jtype]`, the function must compute and accumulate pairwise forces and energies for all pairs `(i, j)` that satisfy `rsq < cutsq[itype][jtype]`. For simplicity, the interaction is modeled as a Lennard-Jones 12-6 potential with per-type parameters `lj1`, `lj2`, `lj3`, `lj4` and `offset` (arrays indexed by `[itype][jtype]`). The function signature must match: `void compute_cpu_pairs(int inum, double** x, int* type, int* numneigh, int** firstneigh, double** f, double* evdwl_for_atom, const double* lshape, const double* const* cutsq, const double* const* lj1, const double* const* lj2, const double* const* lj3, const double* const* lj4, const double* const* offset, double* virial)` where `evdwl_for_atom` accumulates energy per atom (initialized to zero), and `virial` accumulates the 6 distinct stress components (xx, yy, zz, xy, xz, yz). The potential is spherically symmetric (ignore the ellipsoidal orientation terms) and the force magnitude follows the standard LJ formula: `forcelj = -r2inv * (lj1 * r6inv - lj2) * r6inv`, with energy `one_eng = r6inv * (r6inv * lj3 - lj4) - offset`. The force vector for atom `i` is `fforce = r12 * forcelj`, and the force on `j` is the opposite. Each pair contributes to the virial as `-0.5 * (r12_i * f_i + r12_j * f_j)` summed componentwise, but since `f_j = -f_i`, the virial contribution for component `ab` is `-0.5 * (r12[a]*f[b] + r12[b]*f[a])` for off-diagonals (xy, xz, yz) and `-0.5 * (r12[a]*f[a] + r12[a]*f[a])` for diagonals (which simplifies to `-r12[a]*f[a]`). Assume `inum` is the number of local atoms (indices 0..inum-1), the neighbor list is full (not half-list) and may contain ghost atoms with indices >= inum. Use `const` where appropriate, and ensure the function has no side effects other than accumulating forces, energies, and virial into the provided arrays.

// The core problem is a classic neighbor-list–based pair computation loop, similar to what appears in molecular dynamics force kernels. The main algorithm iterates over all local atoms `i` from 0 to `inum-1`, then for each, iterates over its neighbor indices from the flattened neighbor list `firstneigh[i]` (length `numneigh[i]`). For each neighbor `j`, we compute the displacement vector `r12 = x[j] - x[i]`, its squared magnitude `rsq`, and then check against the per-type cutoff squared `cutsq[itype][jtype]`. If within cutoff, we compute the reduced distance `r2inv = 1/rsq` and `r6inv = r2inv^3`. The force magnitude per unit distance is `forcelj = -r2inv * (lj1[itype][jtype] * r6inv - lj2[itype][jtype]) * r6inv` (note: the formula in the snippet is `r6inv * (lj1 * r6inv - lj2) * -r2inv`, which matches). The force vector on atom `i` is `fforce = r12 * forcelj` (since `forcelj` already includes the negative sign and the division by `rsq`). The energy is `one_eng = r6inv * (r6inv * lj3[itype][jtype] - lj4[itype][jtype]) - offset[itype][jtype]`. For the virial, for each pair we add the symmetrized contribution: for diagonal components `xx`, `yy`, `zz`, the contribution is `-r12[a]*fforce[a]` (since the force on `j` is opposite and the virial formula is `-0.5*(r12[a]*f[a] + (-r12[a])*(-f[a]))`? Actually, careful: the virial for a pair is `-0.5 * (r12_i · f_i + r12_j · f_j)`, where `r12_j = -r12_i` and `f_j = -f_i`, so `r12_j · f_j = r12_i · f_i`, therefore the sum is `2 * r12_i · f_i`, and the virial contribution is `-r12_i · f_i`. For off-diagonals (xy), we compute `-0.5 * (r12[x]*f[y] + r12[y]*f[x])` because both components contribute. Since `f[x] = r12[x]*forcelj` and `f[y] = r12[y]*forcelj`, that becomes `-0.5 * (r12[x]*r12[y]*forcelj + r12[y]*r12[x]*forcelj) = -r12[x]*r12[y]*forcelj`. However, to be general we can just use the explicit symmetric formula. The edge cases include: (1) zero or negative `rsq` (should be avoided by the cutoff check, but if `rsq == 0`, the potential is singular, so we assume the caller never provides such a pair; still, we could guard against it), (2) neighbor lists that contain ghost atoms with index >= inum but still have valid positions and types, (3) per-type arrays indexed from 1 (as in LAMMPS), but we can adapt to 0-based indexing in this standalone version to simplify, and (4) the function must accumulate into pre-zeroed arrays (caller is responsible). The time complexity is `O(inum + total_neighbor_pairs)` where the neighbor list is already built; the loop is linear in the number of pairs. Space complexity is `O(1)` auxiliary (only a few scalar temporaries), not counting the input arrays.

#include <cstddef>

// Compute pairwise Lennard-Jones forces, energies, and virial for a set of atoms.
// inum: number of local atoms (indices 0..inum-1)
// x: atom positions (nlocal+nghost, each a double[3])
// type: atom types (0-based indices)
// numneigh: number of neighbors for each local atom
// firstneigh: flattened neighbor list; for atom i, neighbors are firstneigh[i][0..numneigh[i]-1]
// f: force accumulator (pre-zeroed, size nall x 3)
// evdwl_for_atom: per-atom energy accumulator (pre-zeroed, size nall)
// lshape: per-type shape product (not used in this spherical version, included for signature)
// cutsq, lj1, lj2, lj3, lj4, offset: per-type pair parameters (indexed by type pairs)
// virial: 6-component stress accumulator (pre-zeroed, order: xx, yy, zz, xy, xz, yz)
void compute_cpu_pairs(int inum, double** x, int* type, int* numneigh, int** firstneigh,
                       double** f, double* evdwl_for_atom, const double* lshape,
                       const double* const* cutsq, const double* const* lj1,
                       const double* const* lj2, const double* const* lj3,
                       const double* const* lj4, const double* const* offset,
                       double* virial) {
    for (int i = 0; i < inum; ++i) {
        const int itype = type[i];
        const int jnum = numneigh[i];
        const int* jlist = firstneigh[i];

        // Precompute i's shape? Not needed for spherical, but keep for symmetry.
        // (lshape is unused in this simplified version but present for API)

        for (int jj = 0; jj < jnum; ++jj) {
            const int j = jlist[jj];
            const int jtype = type[j];

            // Displacement vector from i to j
            const double r12[3] = { x[j][0] - x[i][0],
                                    x[j][1] - x[i][1],
                                    x[j][2] - x[i][2] };
            const double rsq = r12[0]*r12[0] + r12[1]*r12[1] + r12[2]*r12[2];

            // Skip pairs beyond cutoff
            if (rsq >= cutsq[itype][jtype]) continue;

            // Reduced distance and r^6
            const double r2inv = 1.0 / rsq;
            const double r6inv = r2inv * r2inv * r2inv;

            // Force magnitude (negative sign already included)
            const double forcelj = -r2inv * (lj1[itype][jtype] * r6inv - lj2[itype][jtype]) * r6inv;

            // Force vector on i
            const double fx = r12[0] * forcelj;
            const double fy = r12[1] * forcelj;
            const double fz = r12[2] * forcelj;

            // Accumulate force on i
            f[i][0] += fx;
            f[i][1] += fy;
            f[i][2] += fz;

            // Accumulate energy per atom
            const double one_eng = r6inv * (r6inv * lj3[itype][jtype] - lj4[itype][jtype]) - offset[itype][jtype];
            evdwl_for_atom[i] += one_eng;

            // Virial contribution: -0.5 * (r12 * f_i + r12_j * f_j) where r12_j = -r12, f_j = -f_i
            // => -0.5 * (r12*f_i + r12*f_i) = -r12*f_i for diagonal, 
            //    -0.5 * (r12[a]*f[b] + r12[b]*f[a]) for off-diagonal.
            virial[0] -= r12[0] * fx;  // xx
            virial[1] -= r12[1] * fy;  // yy
            virial[2] -= r12[2] * fz;  // zz
            virial[3] -= 0.5 * (r12[0] * fy + r12[1] * fx);  // xy
            virial[4] -= 0.5 * (r12[0] * fz + r12[2] * fx);  // xz
            virial[5] -= 0.5 * (r12[1] * fz + r12[2] * fy);  // yz
        }
    }
}

#include <cassert>
#include <cmath>

// Declare the function (in real code, include the header)
void compute_cpu_pairs(int inum, double** x, int* type, int* numneigh, int** firstneigh,
                       double** f, double* evdwl_for_atom, const double* lshape,
                       const double* const* cutsq, const double* const* lj1,
                       const double* const* lj2, const double* const* lj3,
                       const double* const* lj4, const double* const* offset,
                       double* virial);

int main() {
    // Test 1: single pair along x-axis, both type 0
    double x1[3] = {0.0, 0.0, 0.0};
    double x2[3] = {1.0, 0.0, 0.0};
    double* x[2] = {x1, x2};
    int type[2] = {0, 0};
    int numneigh[1] = {1};
    int neigh0[1] = {1};
    int* firstneigh[1] = {neigh0};

    double f[2][3] = {{0,0,0},{0,0,0}};
    double* fptr[2] = {f[0], f[1]};
    double evdwl[2] = {0.0, 0.0};
    double lshape[1] = {1.0};
    double cutsq[1][1] = {{2.0}};
    double lj1[1][1] = {{1.0}};
    double lj2[1][1] = {{0.5}};
    double lj3[1][1] = {{1.0}};
    double lj4[1][1] = {{0.5}};
    double offset[1][1] = {{0.0}};
    double virial[6] = {0,0,0,0,0,0};

    double* cutsq_ptr[1] = {cutsq[0]};
    double* lj1_ptr[1] = {lj1[0]};
    double* lj2_ptr[1] = {lj2[0]};
    double* lj3_ptr[1] = {lj3[0]};
    double* lj4_ptr[1] = {lj4[0]};
    double* offset_ptr[1] = {offset[0]};

    compute_cpu_pairs(1, x, type, numneigh, firstneigh, fptr, evdwl, lshape,
                      cutsq_ptr, lj1_ptr, lj2_ptr, lj3_ptr, lj4_ptr, offset_ptr, virial);

    // rsq = 1, r2inv = 1, r6inv = 1
    // forcelj = -1 * (1*1 - 0.5) * 1 = -0.5
    // fx = 1 * -0.5 = -0.5, fy=fz=0
    // energy = 1*(1*1 - 0.5) - 0 = 0.5
    assert(std::abs(f[0][0] - (-0.5)) < 1e-12);
    assert(std::abs(f[0][1]) < 1e-12);
    assert(std::abs(f[0][2]) < 1e-12);
    assert(std::abs(evdwl[0] - 0.5) < 1e-12);
    // virial xx = -r12[0]*fx = -1*(-0.5) = 0.5
    assert(std::abs(virial[0] - 0.5) < 1e-12);
    assert(std::abs(virial[3]) < 1e-12);

    // Test 2: pair with specific separation r=2.0, all arrays reset
    double x1b[3] = {0.0, 0.0, 0.0};
    double x2b[3] = {2.0, 0.0, 0.0};
    double* xb[2] = {x1b, x2b};
    int typeb[2] = {0, 0};
    int numneighb[1] = {1};
    int neighb[1] = {1};
    int* firstneighb[1] = {neighb};
    double fb[2][3] = {{0,0,0},{0,0,0}};
    double* fptrb[2] = {fb[0], fb[1]};
    double evdwlb[2] = {0.0, 0.0};
    double virialb[6] = {0,0,0,0,0,0};

    compute_cpu_pairs(1, xb, typeb, numneighb, firstneighb, fptrb, evdwlb, lshape,
                      cutsq_ptr, lj1_ptr, lj2_ptr, lj3_ptr, lj4_ptr, offset_ptr, virialb);

    // rsq=4, r2inv=0.25, r6inv=0.25^3=0.015625
    // forcelj = -0.25 * (1*0.015625 - 0.5) * 0.015625 = -0.25 * (-0.484375) * 0.015625 = 0.00189208984375
    double expected_force = -0.25 * (1.0*0.015625 - 0.5) * 0.015625;
    assert(std::abs(fb[0][0] - expected_force) < 1e-12);
    // energy = 0.015625*(0.015625*1 - 0.5) - 0 = 0.015625*(-0.484375) = -0.007568359375
    double expected_energy = 0.015625 * (0.015625*1.0 - 0.5);
    assert(std::abs(evdwlb[0] - expected_energy) < 1e-12);
    // virial xx = -2.0 * expected_force
    assert(std::abs(virialb[0] - (-2.0*expected_force)) < 1e-12);

    // Test 3: two atoms with pair beyond cutoff (rsq=9, cutoff=2.0) -> no force
    double x1c[3] = {0,0,0};
    double x2c[3] = {3,0,0};
    double* xc[2] = {x1c, x2c};
    int typec[2] = {0, 0};
    int numneighc[1] = {1};
    int neighc[1] = {1};
    int* firstneighc[1] = {neighc};
    double fc[2][3] = {{0,0,0},{0,0,0}};
    double* fptrc[2] = {fc[0], fc[1]};
    double evdwl_c[2] = {0.0, 0.0};
    double virialc[6] = {0,0,0,0,0,0};
    compute_cpu_pairs(1, xc, typec, numneighc, firstneighc, fptrc, evdwl_c, lshape,
                      cutsq_ptr, lj1_ptr, lj2_ptr, lj3_ptr, lj4_ptr, offset_ptr, virialc);
    assert(fc[0][0] == 0.0 && fc[0][1] == 0.0 && fc[0][2] == 0.0);
    assert(evdwl_c[0] == 0.0);
    for (int k = 0; k < 6; ++k) assert(virialc[k] == 0.0);

    // Test 4: two pairs for atom 0 with two neighbors, check accumulation
    double x1d[3] = {0,0,0};
    double x2d[3] = {1,0,0};
    double x3d[3] = {0,1,0};
    double* xd[3] = {x1d, x2d, x3d};
    int typed[3] = {0, 0, 0};
    int numneighd[1] = {2};
    int neighd[2] = {1, 2};
    int* firstneighd[1] = {neighd};
    double fd[3][3] = {{0,0,0},{0,0,0},{0,0,0}};
    double* fptrd[3] = {fd[0], fd[1], fd[2]};
    double evdwl_d[3] = {0,0,0};
    double viriald[6] = {0,0,0,0,0,0};
    compute_cpu_pairs(1, xd, typed, numneighd, firstneighd, fptrd, evdwl_d, lshape,
                      cutsq_ptr, lj1_ptr, lj2_ptr, lj3_ptr, lj4_ptr, offset_ptr, viriald);
    // First neighbor along x: force -0.5 in x
    // Second neighbor along y: same magnitude, force -0.5 in y
    assert(std::abs(fd[0][0] - (-0.5)) < 1e-12);
    assert(std::abs(fd[0][1] - (-0.5)) < 1e-12);
    assert(std::abs(fd[0][2] - 0.0) < 1e-12);
    assert(std::abs(evdwl_d[0] - (0.5 + 0.5)) < 1e-12);
    // virial xx = 0.5 (from x pair), yy = 0.5 (from y pair), xy = 0 (both force and r have zero? actually xy from x pair: r12[0]*f[1] = 1*0 = 0, r12[1]*f[0] = 0*-0.5=0; from y pair similarly 0)
    assert(std::abs(viriald[0] - 0.5) < 1e-12);
    assert(std::abs(viriald[1] - 0.5) < 1e-12);
    assert(std::abs(viriald[3]) < 1e-12);

    return 0;
}
