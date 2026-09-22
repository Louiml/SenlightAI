Write a C++ function `computeLatticeAndAtoms` that, given lattice parameters `a`, `cOverA`, and atomic coordinates `x1`, `x2`, `x3`, `y3`, `z3`, constructs the rhombohedral crystal structure described by the AFW ANRL prototype "ABC3_hR10_161_a_a_b". The function should return a struct containing the three lattice vectors `a1`, `a2`, `a3` (as `std::array<double,3>`), the number of atoms (always 10), and a vector of atomic fractional coordinates for all 10 atoms in the exact order and positions specified: two "A" atoms at (x1,x1,x1) and (1/2+x1, 1/2+x1, 1/2+x1); two "B" atoms at (x2,x2,x2) and (1/2+x2, 1/2+x2, 1/2+x2); and six "C" atoms at (x3,y3,z3), (z3,x3,y3), (y3,z3,x3), (1/2+y3,1/2+x3,1/2+z3), (1/2+x3,1/2+z3,1/2+y3), and (1/2+z3,1/2+y3,1/2+x3). The lattice vectors must be computed as:  
a1 = (0.5*a, -a/(2*sqrt(3)), c/3),  
a2 = (0, a/sqrt(3), c/3),  
a3 = (-0.5*a, -a/(2*sqrt(3)), c/3),  
where `c = cOverA * a`. The function must be `const`-correct (no mutation of inputs), and return the structure by value. You may assume all inputs are finite positive doubles for `a` and `cOverA`, and any finite doubles for the coordinates.

#include <cassert>
#include <cmath>

// (Include the solution code above here, or ensure it's available.)

int main() {
    // Test with known simple values.
    const double a = 1.0;
    const double cOverA = 1.0;
    const double x1 = 0.1, x2 = 0.2, x3 = 0.3, y3 = 0.4, z3 = 0.5;

    CrystalStructure s = computeLatticeAndAtoms(a, cOverA, x1, x2, x3, y3, z3);

    // Check number of atoms.
    assert(s.numAtoms == 10);
    assert(s.atoms.size() == 10);

    // Check lattice vectors.
    const double sqrt3 = std::sqrt(3.0);
    const double factor = a / (2.0 * sqrt3);
    assert(std::fabs(s.a1[0] - 0.5) < 1e-9);
    assert(std::fabs(s.a1[1] - (-factor)) < 1e-9);
    assert(std::fabs(s.a1[2] - 1.0/3.0) < 1e-9);
    assert(std::fabs(s.a2[0]) < 1e-9);
    assert(std::fabs(s.a2[1] - 1.0/sqrt3) < 1e-9);
    assert(std::fabs(s.a2[2] - 1.0/3.0) < 1e-9);
    assert(std::fabs(s.a3[0] - (-0.5)) < 1e-9);
    assert(std::fabs(s.a3[1] - (-factor)) < 1e-9);
    assert(std::fabs(s.a3[2] - 1.0/3.0) < 1e-9);

    // Check atomic positions.
    assert(s.atoms[0] == std::array<double,3>{0.1, 0.1, 0.1});
    assert(s.atoms[1] == std::array<double,3>{0.6, 0.6, 0.6});
    assert(s.atoms[2] == std::array<double,3>{0.2, 0.2, 0.2});
    assert(s.atoms[3] == std::array<double,3>{0.7, 0.7, 0.7});
    assert(s.atoms[4] == std::array<double,3>{0.3, 0.4, 0.5});
    assert(s.atoms[5] == std::array<double,3>{0.5, 0.3, 0.4});
    assert(s.atoms[6] == std::array<double,3>{0.4, 0.5, 0.3});
    assert(s.atoms[7] == std::array<double,3>{0.9, 0.8, 1.0});  // 0.5+0.4=0.9, 0.5+0.3=0.8, 0.5+0.5=1.0
    assert(s.atoms[8] == std::array<double,3>{0.8, 1.0, 0.9});
    assert(s.atoms[9] == std::array<double,3>{1.0, 0.9, 0.8});

    // Test different cOverA to ensure c is correctly used.
    CrystalStructure s2 = computeLatticeAndAtoms(2.0, 3.0, 0.0, 0.0, 0.0, 0.0, 0.0);
    assert(std::fabs(s2.a1[2] - 2.0) < 1e-9);  // c/3 = (3*2)/3 = 2
    assert(std::fabs(s2.a2[2] - 2.0) < 1e-9);
    assert(std::fabs(s2.a3[2] - 2.0) < 1e-9);
    // All atoms at (0,0,0) or (0.5,0.5,0.5) pattern.
    assert(s2.atoms[0] == std::array<double,3>{0,0,0});
    assert(s2.atoms[1] == std::array<double,3>{0.5,0.5,0.5});
    assert(s2.atoms[4] == std::array<double,3>{0,0,0});
    assert(s2.atoms[5] == std::array<double,3>{0,0,0});
    assert(s2.atoms[6] == std::array<double,3>{0,0,0});

    return 0;
}

#include <array>
#include <vector>
#include <cmath>

// Structure representing a crystal unit cell with fractional atomic positions.
struct CrystalStructure {
    std::array<double, 3> a1;
    std::array<double, 3> a2;
    std::array<double, 3> a3;
    int numAtoms = 0;
    std::vector<std::array<double, 3>> atoms;  // fractional coordinates
};

// Construct the ABC3_hR10_161_a_a_b rhombohedral crystal given lattice parameters
// and six independent atomic coordinate parameters.
CrystalStructure computeLatticeAndAtoms(
    const double a,
    const double cOverA,
    const double x1,
    const double x2,
    const double x3,
    const double y3,
    const double z3
) {
    const double c = cOverA * a;
    const double sqrt3 = std::sqrt(3.0);
    const double factor = a / (2.0 * sqrt3);

    CrystalStructure result;
    result.a1 = {0.5 * a, -factor, c / 3.0};
    result.a2 = {0.0, a / sqrt3, c / 3.0};
    result.a3 = {-0.5 * a, -factor, c / 3.0};

    // A atoms (2)
    result.atoms.push_back({x1, x1, x1});
    result.atoms.push_back({0.5 + x1, 0.5 + x1, 0.5 + x1});
    // B atoms (2)
    result.atoms.push_back({x2, x2, x2});
    result.atoms.push_back({0.5 + x2, 0.5 + x2, 0.5 + x2});
    // C atoms (6)
    result.atoms.push_back({x3, y3, z3});
    result.atoms.push_back({z3, x3, y3});
    result.atoms.push_back({y3, z3, x3});
    result.atoms.push_back({0.5 + y3, 0.5 + x3, 0.5 + z3});
    result.atoms.push_back({0.5 + x3, 0.5 + z3, 0.5 + y3});
    result.atoms.push_back({0.5 + z3, 0.5 + y3, 0.5 + x3});

    result.numAtoms = static_cast<int>(result.atoms.size());
    return result;
}

// The solution requires defining a plain data structure (e.g., a `struct CrystalStructure`) that holds the three lattice vectors as `std::array<double,3>` and a `std::vector<std::array<double,3>>` of fractional atomic coordinates, plus an `int` for atom count. The function takes six doubles by value or const reference and computes the lattice vectors using the provided formulas. Since the formulas involve `sqrt(3)`, use `std::sqrt` from `<cmath>`. For the atomic positions, create a vector and push back each of the ten positions exactly as listed, being careful with the fractional additions (e.g., `0.5 + x1`, etc.) to avoid integer truncation. There are no edge cases beyond ensuring the vector has exactly 10 elements; the inputs are assumed valid. Time complexity is O(1) because the number of atoms is fixed at 10, and space complexity is O(1) for the returned structure (though the vector allocation is constant-size). The main algorithm is straightforward: compute `c`, then construct each lattice vector component, then populate the positions. Use `const` for input parameters and for the function where possible; the function itself can be non-const because it returns by value, but the parameters should be `const double` or `const double&`.
