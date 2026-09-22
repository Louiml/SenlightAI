// Write a C++ function named `makeBravaisLattice` that takes three arguments: an integer `ibrav`, a vector of three doubles `axes` (representing the lattice parameters a, b, c), and a vector of three doubles `angles` (representing the cosine of the lattice angles alpha, beta, gamma). The function must return a 3x3 matrix (as an array of three arrays of three doubles) representing the Bravais lattice basis vectors according to the conventions used in the Vipster code snippet. The supported values of `ibrav` are: 1 (cubic), 2 (fcc), 3 (bcc), -3 (bcc alternative), 4 (hexagonal/trigonal, c-axis from `axes[2]`), 5 (trigonal with 3-fold axis along z, from `angles[0]`), -5 (trigonal with 3-fold axis along <111>, from `angles[0]`), 6 (tetragonal, c-axis from `axes[2]`), 7 (bct, c-axis from `axes[2]`), 8 (orthorhombic, b and c from `axes[1]` and `axes[2]`), 9 and -9 (bco variants), 91 (bco A-type), 10 (fco), 11 (body-centered orthorhombic), 12 and -12 (monoclinic with unique axis z or y, using `angles[0]` or `angles[1]`), 13 and -13 (bcm variants), and 14 (triclinic, using all angles). For cases where `axes[1]` or `axes[2]` are required but zero, or where an angle cosine has absolute value >= 1, throw a `std::invalid_argument` with a meaningful message. For any other `ibrav`, throw `std::invalid_argument` saying "unknown ibrav". You may use `std::sqrt` and `std::pow` from `<cmath>`. Your function must be self-contained (no external library besides standard headers) and must handle all edge cases correctly.

// The solution directly implements the matrix construction rules from the snippet. The main algorithm is a switch on `ibrav` that returns a hardcoded or computed 3x3 matrix. For most cases, the matrix is constant or depends on `axes[2]` (for hexagonal, tetragonal, bct, orthorhombic variants) or on `axes[1]` and `axes[2]` (for orthorhombic/monoclinic/triclinic cases). The trigonal cases (5 and -5) compute intermediate variables `tx`, `ty`, `tz`, `u`, `v` from `angles[0]`. The monoclinic and triclinic cases use `angles[0]`, `angles[1]`, `angles[2]` to compute the basis vectors with the correct trigonometric relations. The implementation uses `std::array<std::array<double,3>,3>` for the return type. Edge cases include throwing when required axes components are zero (using `float_comp`-like check: since the snippet uses a helper `float_comp`, we approximate with a tolerance check `std::abs(x) < 1e-12`). Also, angle cosines must have absolute value strictly less than 1 to avoid negative square roots; the snippet checks `std::abs(angles[i]) >= 1` and throws, which we replicate. Time complexity is O(1) since it's a constant number of operations; space complexity O(1) for the returned matrix. The main challenge is correctly transcribing the formulas, especially for cases 12, -12, 13, -13, and 14, paying attention to indexing (angles[0] = cos alpha, angles[1] = cos beta, angles[2] = cos gamma). Also note that in the snippet, for case 14, the formula uses `axes[1]` and `axes[2]` and `singam` (sin gamma) correctly. All operations are straightforward mathematical computations.

#include <array>
#include <cmath>
#include <stdexcept>

using Vec = std::array<double, 3>;
using Mat = std::array<std::array<double, 3>, 3>;

// Helper to check if a double is essentially zero
inline bool isZero(double x) {
    return std::abs(x) < 1e-12;
}

// Build a Bravais lattice matrix from ibrav, axes (a,b,c), and angles (cos alpha, cos beta, cos gamma)
Mat makeBravaisLattice(int ibrav, const Vec& axes, const Vec& angles) {
    switch (ibrav) {
        case 1: // cubic
            return {{{{1, 0, 0}}, {{0, 1, 0}}, {{0, 0, 1}}}};
        case 2: // fcc
            return {{{{-0.5, 0.0, 0.5}}, {{0.0, 0.5, 0.5}}, {{-0.5, 0.5, 0.0}}}};
        case 3: // bcc
            return {{{{ 0.5, 0.5, 0.5}}, {{-0.5, 0.5, 0.5}}, {{-0.5, -0.5, 0.5}}}};
        case -3: // bcc alternative
            return {{{{-0.5, 0.5, 0.5}}, {{0.5, -0.5, 0.5}}, {{0.5, 0.5, -0.5}}}};
        case 4: // hexagonal/trigonal
            if (isZero(axes[2])) throw std::invalid_argument("makeBravais: axes[2] not provided");
            return {{{{1.0, 0.0, 0.0}}, {{-0.5, std::sqrt(3.0)*0.5, 0.0}}, {{0.0, 0.0, axes[2]}}}};
        case 5: // trigonal, 3fold axis z
            if (std::abs(angles[0]) >= 1) throw std::invalid_argument("makeBravais: angles[0] is invalid");
        {
            double tx = std::sqrt((1.0 - angles[0]) / 2.0);
            double ty = std::sqrt((1.0 - angles[0]) / 6.0);
            double tz = std::sqrt((1.0 + 2.0 * angles[0]) / 3.0);
            return {{{{tx, -ty, tz}}, {{0, 2*ty, tz}}, {{-tx, -ty, tz}}}};
        }
        case -5: // trigonal, 3fold axis <111>
            if (std::abs(angles[0]) >= 1) throw std::invalid_argument("makeBravais: angles[0] is invalid");
        {
            double ty = std::sqrt((1.0 - angles[0]) / 6.0);
            double tz = std::sqrt((1.0 + 2.0 * angles[0]) / 3.0);
            double u = (tz - 2.0 * std::sqrt(2.0) * ty) / std::sqrt(3.0);
            double v = (tz + std::sqrt(2.0) * ty) / std::sqrt(3.0);
            return {{{{u, v, v}}, {{v, u, v}}, {{v, v, u}}}};
        }
        case 6: // tetragonal
            if (isZero(axes[2])) throw std::invalid_argument("makeBravais: axes[2] not provided");
            return {{{{1, 0, 0}}, {{0, 1, 0}}, {{0, 0, axes[2]}}}};
        case 7: // bct
            if (isZero(axes[2])) throw std::invalid_argument("makeBravais: axes[2] not provided");
            return {{{{ 0.5, -0.5, axes[2]*0.5}},
                     {{ 0.5,  0.5, axes[2]*0.5}},
                     {{-0.5, -0.5, axes[2]*0.5}}}};
        case 8: // orthorhombic
            if (isZero(axes[1])) throw std::invalid_argument("makeBravais: axes[1] not provided");
            if (isZero(axes[2])) throw std::invalid_argument("makeBravais: axes[2] not provided");
            return {{{{1, 0, 0}}, {{0, axes[1], 0}}, {{0, 0, axes[2]}}}};
        case 9: // bco
            if (isZero(axes[1])) throw std::invalid_argument("makeBravais: axes[1] not provided");
            if (isZero(axes[2])) throw std::invalid_argument("makeBravais: axes[2] not provided");
            return {{{{ 0.5, axes[1]*0.5, 0}},
                     {{-0.5, axes[1]*0.5, 0}},
                     {{ 0.0, 0.0, axes[2]}}}};
        case -9: // bco alternative
            if (isZero(axes[1])) throw std::invalid_argument("makeBravais: axes[1] not provided");
            if (isZero(axes[2])) throw std::invalid_argument("makeBravais: axes[2] not provided");
            return {{{{ 0.5, -axes[1]*0.5, 0}},
                     {{ 0.5,  axes[1]*0.5, 0}},
                     {{ 0.0, 0.0, axes[2]}}}};
        case 91: // bco A type
            if (isZero(axes[1])) throw std::invalid_argument("makeBravais: axes[1] not provided");
            if (isZero(axes[2])) throw std::invalid_argument("makeBravais: axes[2] not provided");
            return {{{{ 0.5, 0.0, 0.0}},
                     {{ 0.0, axes[1]*0.5, -axes[2]*0.5}},
                     {{ 0.0, axes[1]*0.5,  axes[2]*0.5}}}};
        case 10: // fco
            if (isZero(axes[1])) throw std::invalid_argument("makeBravais: axes[1] not provided");
            if (isZero(axes[2])) throw std::invalid_argument("makeBravais: axes[2] not provided");
            return {{{{ 0.5, 0.0, axes[2]*0.5}},
                     {{ 0.5, axes[1]*0.5, 0.0}},
                     {{ 0.0, axes[1]*0.5, axes[2]*0.5}}}};
        case 11: // body-centered orthorhombic
            if (isZero(axes[1])) throw std::invalid_argument("makeBravais: axes[1] not provided");
            if (isZero(axes[2])) throw std::invalid_argument("makeBravais: axes[2] not provided");
            return {{{{ 0.5,  axes[1]*0.5, axes[2]*0.5}},
                     {{-0.5,  axes[1]*0.5, axes[2]*0.5}},
                     {{-0.5, -axes[1]*0.5, axes[2]*0.5}}}};
        case 12: // monoclinic unique axis z
            if (isZero(axes[1])) throw std::invalid_argument("makeBravais: axes[1] not provided");
            if (isZero(axes[2])) throw std::invalid_argument("makeBravais: axes[2] not provided");
            if (std::abs(angles[0]) >= 1) throw std::invalid_argument("makeBravais: angles[0] is invalid");
            return {{{{1, 0, 0}},
                     {{axes[1]*angles[0], axes[1]*std::sqrt(1.0 - std::pow(angles[1], 2)), 0}},
                     {{0, 0, axes[2]}}}};
        case -12: // monoclinic unique axis y
            if (isZero(axes[1])) throw std::invalid_argument("makeBravais: axes[1] not provided");
            if (isZero(axes[2])) throw std::invalid_argument("makeBravais: axes[2] not provided");
            if (std::abs(angles[1]) >= 1) throw std::invalid_argument("makeBravais: angles[1] is invalid");
            return {{{{1, 0, 0}},
                     {{0, axes[1], 0}},
                     {{axes[2]*angles[1], 0, axes[2]*std::sqrt(1.0 - std::pow(angles[1], 2))}}}};
        case 13: // bcm unique axis z
            if (isZero(axes[1])) throw std::invalid_argument("makeBravais: axes[1] not provided");
            if (isZero(axes[2])) throw std::invalid_argument("makeBravais: axes[2] not provided");
            if (std::abs(angles[0]) >= 1) throw std::invalid_argument("makeBravais: angles[0] is invalid");
            return {{{{0.5, 0, -0.5 * axes[2]}},
                     {{axes[1]*angles[1], 0, axes[1]*std::sqrt(1.0 - std::pow(angles[1], 2))}},
                     {{0.5, 0, 0.5 * axes[2]}}}};
        case -13: // bcm unique axis y
            if (isZero(axes[1])) throw std::invalid_argument("makeBravais: axes[1] not provided");
            if (isZero(axes[2])) throw std::invalid_argument("makeBravais: axes[2] not provided");
            if (std::abs(angles[1]) >= 1) throw std::invalid_argument("makeBravais: angles[1] is invalid");
            return {{{{0.5, -0.5 * axes[1], 0}},
                     {{0.5,  0.5 * axes[1], 0}},
                     {{axes[2]*angles[2], 0, axes[2]*std::sqrt(1.0 - std::pow(angles[2], 2))}}}};
        case 14: // triclinic
            if (isZero(axes[1])) throw std::invalid_argument("makeBravais: axes[1] not provided");
            if (isZero(axes[2])) throw std::invalid_argument("makeBravais: axes[2] not provided");
            if (std::abs(angles[0]) >= 1) throw std::invalid_argument("makeBravais: angles[0] is invalid");
            if (std::abs(angles[1]) >= 1) throw std::invalid_argument("makeBravais: angles[1] is invalid");
            if (std::abs(angles[2]) >= 1) throw std::invalid_argument("makeBravais: angles[2] is invalid");
        {
            double singam = std::sqrt(1.0 - std::pow(angles[2], 2));
            return {{{{1, 0, 0}},
                     {{axes[1]*angles[2], axes[1]*singam, 0}},
                     {{axes[2]*angles[1], axes[2]*(angles[0] - angles[1]*angles[2])/singam,
                       axes[2]*std::sqrt(1 + 2*angles[0]*angles[1]*angles[2] -
                                          std::pow(angles[0],2) - std::pow(angles[1],2) -
                                          std::pow(angles[2],2))/singam}}}};
        }
        default:
            throw std::invalid_argument("makeBravais unknown ibrav");
    }
}

#include <cassert>
#include <cmath>
#include <stdexcept>

// Assume makeBravaisLattice and Vec/Mat types are defined above

int main() {
    // Helper to compare matrices with tolerance
    auto close = [](double a, double b) { return std::abs(a - b) < 1e-9; };
    auto checkMat = [&](const Mat& m, const Mat& ref) {
        for (int i = 0; i < 3; ++i)
            for (int j = 0; j < 3; ++j)
                assert(close(m[i][j], ref[i][j]));
    };

    // Cubic
    Mat m1 = makeBravaisLattice(1, {0,0,0}, {0,0,0});
    checkMat(m1, {{{1,0,0},{0,1,0},{0,0,1}}});

    // fcc
    Mat m2 = makeBravaisLattice(2, {0,0,0}, {0,0,0});
    checkMat(m2, {{{-0.5,0,0.5},{0,0.5,0.5},{-0.5,0.5,0}}});

    // bcc
    Mat m3 = makeBravaisLattice(3, {0,0,0}, {0,0,0});
    checkMat(m3, {{{0.5,0.5,0.5},{-0.5,0.5,0.5},{-0.5,-0.5,0.5}}});

    // Hexagonal with c=2
    Mat m4 = makeBravaisLattice(4, {0,0,2.0}, {0,0,0});
    checkMat(m4, {{{1,0,0},{-0.5,std::sqrt(3)*0.5,0},{0,0,2.0}}});

    // Trigonal case 5 with cos alpha = 0.5
    Mat m5 = makeBravaisLattice(5, {0,0,0}, {0.5,0,0});
    double tx = std::sqrt(0.25), ty = std::sqrt(0.25/3.0), tz = std::sqrt(2.0/3.0);
    checkMat(m5, {{{tx,-ty,tz},{0,2*ty,tz},{-tx,-ty,tz}}});

    // Tetragonal with c=3
    Mat m6 = makeBravaisLattice(6, {0,0,3.0}, {0,0,0});
    checkMat(m6, {{{1,0,0},{0,1,0},{0,0,3.0}}});

    // Orthorhombic with b=2, c=3
    Mat m8 = makeBravaisLattice(8, {0,2.0,3.0}, {0,0,0});
    checkMat(m8, {{{1,0,0},{0,2.0,0},{0,0,3.0}}});

    // Monoclinic case 12: a=1, b=2, c=3, cos alpha=0.2, cos beta=0.3
    Mat m12 = makeBravaisLattice(12, {0,2.0,3.0}, {0.2,0.3,0});
    double sqrt_term = std::sqrt(1.0 - 0.3*0.3);
    checkMat(m12, {{{1,0,0},{2.0*0.2,2.0*sqrt_term,0},{0,0,3.0}}});

    // Triclinic case 14: a=1, b=2, c=3, cos alpha=0.2, cos beta=0.3, cos gamma=0.4
    Mat m14 = makeBravaisLattice(14, {0,2.0,3.0}, {0.2,0.3,0.4});
    double sg = std::sqrt(1.0 - 0.4*0.4);
    double third = 3.0 * (0.2 - 0.3*0.4) / sg;
    double thirdz = 3.0 * std::sqrt(1 + 2*0.2*0.3*0.4 - 0.2*0.2 - 0.3*0.3 - 0.4*0.4) / sg;
    checkMat(m14, {{{1,0,0},{2.0*0.4,2.0*sg,0},{3.0*0.3, third, thirdz}}});

    // Error cases
    bool threw = false;
    try { makeBravaisLattice(4, {0,0,0}, {0,0,0}); } catch (const std::invalid_argument&) { threw = true; }
    assert(threw);

    threw = false;
    try { makeBravaisLattice(5, {0,0,0}, {1.0,0,0}); } catch (const std::invalid_argument&) { threw = true; }
    assert(threw);

    threw = false;
    try { makeBravaisLattice(99, {0,0,0}, {0,0,0}); } catch (const std::invalid_argument&) { threw = true; }
    assert(threw);

    return 0;
}
