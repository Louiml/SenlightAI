/*
Write a C++ function that computes the total potential energy and per-energy-group contributions for particles interacting with one or two flat walls (perpendicular to the z-axis), using either a 9-3 Lennard-Jones wall potential, a 10-4 potential, a 12-6 potential, or a user-supplied tabulated force table. The function must take as input the number of walls, the wall type (as an integer code: 0=tabulated, 1=9-3, 2=10-4, 3=12-6), the wall atom-type parameter (an integer), wall density values (only used for 9-3 and 10-4 walls), the wall linear potential cutoff distance, a scaling factor for wall energy groups, the number of atoms, arrays of atom types and z-coordinates, and the per-energy-group exclusion flags. For each atom and each wall, compute the distance to the wall (for wall 0, the distance is the z-coordinate; for wall 1, it is box_height minus the z-coordinate). If the distance is less than or equal to zero, treat it as an error. If the distance is less than the linear-potential cutoff, apply a linear correction to the potential (add `(cutoff - r) * force`) and set the force to the value at the cutoff. Compute the potential and force using the chosen wall model (for tabulated, use cubic interpolation on provided table data; for analytic forms, use the standard formulas with prefactors `Cd` and `Cr` scaled by `wall_density * pi` constants). Accumulate the potential into the energy group bin (group index = atom group * total_groups + (total_groups - nwalls + wall_index)), multiply the force by a lambda scaling factor, and add the z-component of the force to the atom's force vector. Return the derivative of the total potential with respect to lambda (computed by finite differences using lambda = 1 for the final call). The function must be self-contained, use only standard C++ libraries (no external GROMACS headers), and handle all wall types with a default of zero potential/force for unknown types.
*/
#include <vector>
#include <cmath>
#include <stdexcept>
#include <algorithm>

// Structure for tabulated wall data (dispersion and repulsion tables)
struct WallTable {
    std::vector<double> data;  // interleaved: for each n: Vd, Fd, G1, H1, Vr, Fr, G2, H2
    double scale;              // table scale (units: inverse distance)
    int n;                     // number of table entry points
};

/**
 * Compute wall interactions and return dV/dlambda (derivative with respect to lambda).
 * The function assumes the walls are perpendicular to z-axis, at z=0 (wall 0) and z=box_height (wall 1).
 *
 * Parameters:
 *   nwalls - number of walls (1 or 2)
 *   wall_type - 0=table, 1=9-3, 2=10-4, 3=12-6, else zero force
 *   wall_atomtype - atom type index for wall (not used here, kept for interface compatibility)
 *   wall_density - array of density values for each wall (only relevant for 9-3,10-4)
 *   wall_r_linpot - linear potential cutoff distance
 *   lambda - scaling factor for perturbed atoms (0..1)
 *   natoms - number of atoms
 *   atom_types - array of atom type indices for each atom (0-based)
 *   z_coords - array of z-coordinates for each atom
 *   box_height - height of the simulation box in z direction
 *   ngroups - total number of energy groups (including wall groups)
 *   nwall_groups - number of wall groups (should equal nwalls)
 *   energy_group_of_atom - array mapping atom index to its energy group index (0..ngroups-nwalls-1)
 *   group_excl_flags - flat array of size ngroups*ngroups, bit 0 set if excluded (EGP_EXCL)
 *   wall_tables - array of pointers to WallTable for each wall (only used if wall_type==0)
 *   forces_out - output array of size natoms*3, will add z-components
 *   potential_per_group - output array of size ngroups, will accumulate Vlj
 *   return value: dV/dlambda
 */
double compute_walls(int nwalls,
                     int wall_type,
                     const std::vector<int>& wall_atomtype,
                     const std::vector<double>& wall_density,
                     double wall_r_linpot,
                     double lambda,
                     int natoms,
                     const std::vector<int>& atom_types,
                     const std::vector<double>& z_coords,
                     double box_height,
                     int ngroups,
                     int nwall_groups,
                     const std::vector<int>& energy_group_of_atom,
                     const std::vector<int>& group_excl_flags,  // size ngroups*ngroups, bit0=excluded
                     const std::vector<WallTable*>& wall_tables,
                     std::vector<double>& forces_out,           // size natoms*3, z components added
                     std::vector<double>& potential_per_group)  // size ngroups
{
    // Precompute prefactors for 9-3 and 10-4 walls
    std::vector<double> fac_d(nwalls, 0.0), fac_r(nwalls, 0.0);
    for (int w = 0; w < nwalls; ++w) {
        if (wall_type == 1) { // 9-3
            fac_d[w] = wall_density[w] * M_PI / 6.0;
            fac_r[w] = wall_density[w] * M_PI / 45.0;
        } else if (wall_type == 2) { // 10-4
            fac_d[w] = wall_density[w] * M_PI / 2.0;
            fac_r[w] = wall_density[w] * M_PI / 5.0;
        }
    }

    // Wall positions (z=0 for wall 0, z=box_height for wall 1)
    std::vector<double> wall_z(nwalls);
    for (int w = 0; w < nwalls; ++w) {
        wall_z[w] = (w == 0) ? 0.0 : box_height;
    }

    double total_Vlambda = 0.0;
    const double sixth = 1.0 / 6.0;
    const double twelfth = 1.0 / 12.0;

    // Each atom
    for (int i = 0; i < natoms; ++i) {
        int atype = atom_types[i];
        // For each wall
        for (int w = 0; w < nwalls; ++w) {
            // Determine energy group pair index: atom's group and wall's group
            // Wall groups are the last nwalls groups
            int atom_g = energy_group_of_atom[i];
            int wall_g = ngroups - nwalls + w;
            int ggid = atom_g * ngroups + wall_g;
            // Check exclusion
            if (group_excl_flags[ggid] & 1) continue;

            // Distance to wall
            double r;
            if (w == 0) {
                r = z_coords[i];
            } else {
                r = box_height - z_coords[i];
            }
            if (r <= 0.0) {
                // Wall error: atom beyond wall
                throw std::runtime_error("Atom beyond wall");
            }

            // Linear potential handling
            double mr = 0.0;
            if (r < wall_r_linpot) {
                mr = wall_r_linpot - r;
                r = wall_r_linpot;
            }

            // Compute Cd, Cr from fictitious atom type (use simple model: Cd=1, Cr=1 for all types)
            // In a real implementation these would come from a force field; here we use constants
            double Cd = 1.0;
            double Cr = 1.0;

            double V = 0.0, F = 0.0;
            if (wall_type == 0) {
                // Tabulated
                if (w < (int)wall_tables.size() && wall_tables[w] != nullptr) {
                    const WallTable& tab = *wall_tables[w];
                    double rt = r * tab.scale;
                    int n0 = (int)rt;
                    if (n0 >= tab.n) {
                        V = 0.0; F = 0.0;
                    } else {
                        double eps = rt - n0;
                        double eps2 = eps * eps;
                        int nnn = 8 * n0;
                        double Yt = tab.data[nnn];
                        double Ft = tab.data[nnn+1];
                        double Geps = tab.data[nnn+2] * eps;
                        double Heps2 = tab.data[nnn+3] * eps2;
                        double Fp = Ft + Geps + Heps2;
                        double VV = Yt + Fp * eps;
                        double FF = Fp + Geps + 2.0 * Heps2;
                        double Vd = 6 * Cd * VV;
                        double Fd = 6 * Cd * FF;
                        // Repulsion
                        nnn += 4;
                        Yt = tab.data[nnn];
                        Ft = tab.data[nnn+1];
                        Geps = tab.data[nnn+2] * eps;
                        Heps2 = tab.data[nnn+3] * eps2;
                        Fp = Ft + Geps + Heps2;
                        VV = Yt + Fp * eps;
                        FF = Fp + Geps + 2.0 * Heps2;
                        double Vr = 12 * Cr * VV;
                        double Fr = 12 * Cr * FF;
                        V = Vd + Vr;
                        F = -(Fd + Fr) * tab.scale;
                    }
                }
            } else if (wall_type == 1) { // 9-3
                double r1 = 1.0 / r;
                double r2 = r1 * r1;
                double r4 = r2 * r2;
                double Vd = fac_d[w] * Cd * r2 * r1;
                double Vr = fac_r[w] * Cr * r4 * r4 * r1;
                V = Vr - Vd;
                F = (9 * Vr - 3 * Vd) * r1;
            } else if (wall_type == 2) { // 10-4
                double r1 = 1.0 / r;
                double r2 = r1 * r1;
                double r4 = r2 * r2;
                double Vd = fac_d[w] * Cd * r4;
                double Vr = fac_r[w] * Cr * r4 * r4 * r2;
                V = Vr - Vd;
                F = (10 * Vr - 4 * Vd) * r1;
            } else if (wall_type == 3) { // 12-6
                double r1 = 1.0 / r;
                double r2 = r1 * r1;
                double r4 = r2 * r2;
                double Vd = Cd * r4 * r2;
                double Vr = Cr * r4 * r4 * r4;
                V = Vr - Vd;
                F = (12 * Vr - 6 * Vd) * r1;
            }
            // else: default V=0, F=0

            // Linear correction
            if (mr > 0.0) {
                V += mr * F;
            }

            // Scale force by lambda
            F *= lambda;
            // For wall 1, force direction is opposite
            if (w == 1) F = -F;

            // Accumulate potential into group
            potential_per_group[ggid] += V;
            total_Vlambda += V;

            // Add force to atom
            forces_out[i*3 + 2] += F;
        }
    }

    // Return derivative of V with respect to lambda (since V is linear in lambda, derivative = Vlambda / 1)
    return total_Vlambda;
}
#include <cassert>
#include <cmath>
#include <vector>
#include <iostream>

// Include the solution function here (or link)

int main() {
    // Test 1: Single atom with 12-6 wall, one wall, no linear correction
    {
        int natoms = 1;
        std::vector<int> atom_types = {0};
        std::vector<double> z = {3.0};
        int nwalls = 1;
        int wall_type = 3; // 12-6
        std::vector<int> wall_atype = {0};
        std::vector<double> wall_density = {1.0}; // not used for 12-6
        double linpot = 0.0;
        double lambda = 1.0;
        int ngroups = 2; // one normal group, one wall group
        int nwall_groups = 1;
        std::vector<int> energy_group = {0};
        std::vector<int> excl(ngroups*ngroups, 0);
        std::vector<WallTable*> tables(nwalls, nullptr);
        std::vector<double> forces(natoms*3, 0.0);
        std::vector<double> pot(ngroups, 0.0);
        double box_height = 10.0;

        double dVdl = compute_walls(nwalls, wall_type, wall_atype, wall_density,
                                    linpot, lambda, natoms, atom_types, z, box_height,
                                    ngroups, nwall_groups, energy_group, excl, tables,
                                    forces, pot);

        // For Cd=1, Cr=1 at r=3: V = 1/r^12 - 1/r^6 = 1/531441 - 1/729 ≈ -0.00137174
        // F = (12/r^13 - 6/r^7) = 12/1594323 - 6/2187 ≈ 0.000025 - 0.002743 = -0.002718
        assert(std::abs(pot[1] - (-0.00137174)) < 1e-5);
        assert(std::abs(forces[2] - (-0.002718)) < 1e-5);
        assert(std::abs(dVdl - (-0.00137174)) < 1e-5);
    }

    // Test 2: Two walls, atom in middle, 12-6, no linear correction
    {
        int natoms = 1;
        std::vector<int> atom_types = {0};
        std::vector<double> z = {5.0};
        int nwalls = 2;
        int wall_type = 3;
        std::vector<int> wall_atype = {0,0};
        std::vector<double> wall_density = {1.0,1.0};
        double linpot = 0.0;
        double lambda = 1.0;
        int ngroups = 3; // one normal, two wall groups
        int nwall_groups = 2;
        std::vector<int> energy_group = {0};
        std::vector<int> excl(ngroups*ngroups, 0);
        std::vector<WallTable*> tables(nwalls, nullptr);
        std::vector<double> forces(natoms*3, 0.0);
        std::vector<double> pot(ngroups, 0.0);
        double box_height = 10.0;

        double dVdl = compute_walls(nwalls, wall_type, wall_atype, wall_density,
                                    linpot, lambda, natoms, atom_types, z, box_height,
                                    ngroups, nwall_groups, energy_group, excl, tables,
                                    forces, pot);
        // Both walls see r=5.0, V = 1/5^12 - 1/5^6 = 1/244140625 - 1/15625 ≈ -0.000064
        // Force from wall0: (12/5^13 - 6/5^7) ≈ 0.000000196 - 0.000768 = -0.0007676 (Z+ direction)
        // Force from wall1: distance from top = 5.0, same magnitude but direction negative Z
        // So net force ≈ 0 (they cancel)
        assert(std::abs(forces[2]) < 1e-6);
        // Potential total ~ -0.000128
        assert(std::abs(pot[1] + pot[2] - (-0.000128)) < 1e-5);
    }

    // Test 3: Linear potential cutoff
    {
        int natoms = 1;
        std::vector<int> atom_types = {0};
        std::vector<double> z = {0.2};
        int nwalls = 1;
        int wall_type = 3;
        std::vector<int> wall_atype = {0};
        std::vector<double> wall_density = {1.0};
        double linpot = 0.5; // linear region below 0.5
        double lambda = 1.0;
        int ngroups = 2;
        int nwall_groups = 1;
        std::vector<int> energy_group = {0};
        std::vector<int> excl(ngroups*ngroups, 0);
        std::vector<WallTable*> tables(nwalls, nullptr);
        std::vector<double> forces(natoms*3, 0.0);
        std::vector<double> pot(ngroups, 0.0);
        double box_height = 10.0;

        double dVdl = compute_walls(nwalls, wall_type, wall_atype, wall_density,
                                    linpot, lambda, natoms, atom_types, z, box_height,
                                    ngroups, nwall_groups, energy_group, excl, tables,
                                    forces, pot);
        // At r=0.5: V = 1/0.5^12 - 1/0.5^6 = 4096 - 64 = 4032, F = (12/0.5^13 - 6/0.5^7) = 98304 - 768 = 97536
        // Linear correction: mr = 0.5-0.2 = 0.3, so V = 4032 + 0.3*97536 = 33292.8
        assert(std::abs(pot[1] - 33292.8) < 1.0);
        assert(std::abs(forces[2] - 97536) < 1.0);
    }

    // Test 4: Excluded group pair -> no force or potential
    {
        int natoms = 1;
        std::vector<int> atom_types = {0};
        std::vector<double> z = {2.0};
        int nwalls = 1;
        int wall_type = 3;
        std::vector<int> wall_atype = {0};
        std::vector<double> wall_density = {1.0};
        double linpot = 0.0;
        double lambda = 1.0;
        int ngroups = 2;
        int nwall_groups = 1;
        std::vector<int> energy_group = {0};
        std::vector<int> excl(ngroups*ngroups, 0);
        excl[0*ngroups + 1] = 1; // exclude pair (0,1)
        std::vector<WallTable*> tables(nwalls, nullptr);
        std::vector<double> forces(natoms*3, 0.0);
        std::vector<double> pot(ngroups, 0.0);
        double box_height = 10.0;

        double dVdl = compute_walls(nwalls, wall_type, wall_atype, wall_density,
                                    linpot, lambda, natoms, atom_types, z, box_height,
                                    ngroups, nwall_groups, energy_group, excl, tables,
                                    forces, pot);
        assert(pot[1] == 0.0);
        assert(forces[2] == 0.0);
        assert(dVdl == 0.0);
    }

    // Test 5: Unknown wall type -> zero forces
    {
        int natoms = 1;
        std::vector<int> atom_types = {0};
        std::vector<double> z = {2.0};
        int nwalls = 1;
        int wall_type = 99; // unknown
        std::vector<int> wall_atype = {0};
        std::vector<double> wall_density = {1.0};
        double linpot = 0.0;
        double lambda = 1.0;
        int ngroups = 2;
        int nwall_groups = 1;
        std::vector<int> energy_group = {0};
        std::vector<int> excl(ngroups*ngroups, 0);
        std::vector<WallTable*> tables(nwalls, nullptr);
        std::vector<double> forces(natoms*3, 0.0);
        std::vector<double> pot(ngroups, 0.0);
        double box_height = 10.0;

        double dVdl = compute_walls(nwalls, wall_type, wall_atype, wall_density,
                                    linpot, lambda, natoms, atom_types, z, box_height,
                                    ngroups, nwall_groups, energy_group, excl, tables,
                                    forces, pot);
        assert(pot[1] == 0.0);
        assert(forces[2] == 0.0);
        assert(dVdl == 0.0);
    }

    std::cout << "All tests passed!" << std::endl;
    return 0;
}
// The solution requires looping over each atom, each wall (0 and 1 if present), and computing the distance from the wall. For wall 0, the distance is simply the atom's z-coordinate; for wall 1, it is `box_height - z`. The distance is clamped to the linear-potential cutoff: if `r < cutoff`, set `r = cutoff` and remember the difference `mr = cutoff - r`; otherwise `mr = 0`. The potential `V` and force `F` (in the direction away from the wall) are computed based on wall type. For analytic potentials, define `r1 = 1/r`, `r2 = r1*r1`, `r4 = r2*r2`. Then:
// - 9-3: `Vd = (wall_density*pi/6) * Cd * r1^3`, `Vr = (wall_density*pi/45) * Cr * r1^9`, `V = Vr - Vd`, `F = (9*Vr - 3*Vd) * r1`
// - 10-4: `Vd = (wall_density*pi/2) * Cd * r1^4`, `Vr = (wall_density*pi/5) * Cr * r1^10`, `V = Vr - Vd`, `F = (10*Vr - 4*Vd) * r1`
// - 12-6: `Vd = Cd * r1^6`, `Vr = Cr * r1^12`, `V = Vr - Vd`, `F = (12*Vr - 6*Vd) * r1`
// For tabulated walls, use cubic interpolation on the provided table (which contains 8 values per entry: Vd, Fd, G, H, Vr, Fr, G, H) with scaling factors `Cd` and `Cr`. The force is then `F = -(6*Cd*Fd_interp + 12*Cr*Fr_interp) * table_scale`. After computing `V` and `F`, if `mr > 0`, add `mr * F` to `V`. The force is multiplied by `lambda` (the scaling factor) and by a sign: for wall 1, the force direction is reversed (since distance is measured from the opposite side). The potential is added to the energy group bin, and the total `Vlambda` is accumulated. Finally, the function returns the derivative of the total potential with respect to lambda; since the function is called once with `lambda_final` = 1, the derivative is simply the total `Vlambda` (the difference between lambda=0 and lambda=1 is `Vlambda` when the potential is linear in lambda). Edge cases: zero distance (error, return 0), atoms beyond the wall (negative distance, treat as error), and exclusions (skip). Time complexity is O(atoms * walls), space is O(1) auxiliary aside from input arrays.
