Write a C++ function `double hydrogenBondEnergy(const std::vector<Atom>& atoms, int donorIndex, int acceptorIndex, int hydrogenIndex, double epsilon, double sigma, double cutInner, double cutOuter, double cutAngleDegrees, int angularPower)` that computes the Dreiding hydrogen-bond interaction energy between a donor atom, an acceptor atom, and a hydrogen atom covalently bonded to the donor. The atoms are represented by a simple `struct Atom { double x, y, z; int type; };` placed in a 3D Cartesian coordinate system (no periodic boundary conditions). The energy is computed as a Lennard-Jones-type kernel multiplied by an angular term governed by the angle at the hydrogen atom: \(E = \epsilon_{\text{eff}} \cdot \left( \left(\frac{\sigma}{r_{DA}}\right)^{12} - \left(\frac{\sigma}{r_{DA}}\right)^{6}\right) \cdot \cos^{p}(\theta)\), where \(r_{DA}\) is the donor–acceptor distance, \(\theta\) is the angle \(D\!-\!H\!-\!A\) (with vertex at the hydrogen), \(p\) is a positive integer angular power, and the overall interaction is smoothly switched off between an inner cutoff \(r_{\text{inner}}\) and an outer cutoff \(r_{\text{outer}}\) using a cubic switching function. The function must return 0.0 if the angle \(\theta\) is outside the allowed angular cutoff (between \(0\) and \(2\pi - \theta_{\text{cut}}\), or equivalently, if \(\theta < \theta_{\text{cut}}\) or \(\theta > 180^\circ - \theta_{\text{cut}}\), where the maximum possible angle is \(180^\circ\)). The switching function is defined as \(S(r) = \frac{(r_{\text{outer}}^2 - r^2)^2 (r_{\text{outer}}^2 + 2r^2 - 3r_{\text{inner}}^2)}{(r_{\text{outer}}^2 - r_{\text{inner}}^2)^3}\) for \(r_{\text{inner}}^2 < r^2 < r_{\text{outer}}^2\), \(S=1\) for \(r^2 \le r_{\text{inner}}^2\), and \(S=0\) for \(r^2 \ge r_{\text{outer}}^2\). The effective epsilon used in the energy is \(5\epsilon\), and the effective sigma used in the distance term is \(\sigma\). The angular power \(p\) is applied as \(\cos^p(\theta)\). The function must handle edge cases: if any distance is zero, return 0.0; if the cosine is slightly outside \([-1,1]\), clamp it.
// The main algorithm computes the donor–acceptor distance squared, the donor–hydrogen and acceptor–hydrogen vectors to determine the angle at the hydrogen atom, and then applies the energy formula with the switching function. First, we compute the vectors from the hydrogen to the donor and from the hydrogen to the acceptor. The cosine of the angle is the dot product of these two vectors divided by their magnitudes. If the cosine is out of the valid range, clamp it to \([-1,1]\). Compute the angle in radians. If the angle is less than the angular cutoff (converted from degrees to radians) or greater than \(180^\circ - \theta_{\text{cut}}\), the angular condition is not satisfied and we return 0.0. Otherwise, compute the donor–acceptor distance squared and the radial switching factor based on the inner and outer cutoffs. The energy is then computed as \(5\epsilon \cdot \left[ \left(\frac{\sigma}{r}\right)^{12} - \left(\frac{\sigma}{r}\right)^{6} \right] \cdot \cos^p(\theta) \cdot S(r)\). Edge cases: if the donor–acceptor distance is outside the outer cutoff, early return 0.0. If any denominator (e.g., donor–hydrogen or acceptor–hydrogen distance) is zero, avoid division by zero and return 0.0. If the angular power \(p=0\), \(\cos^0\) is 1 for all angles, so the angular term disappears. The time complexity is O(1) since it performs a fixed number of arithmetic operations. The space complexity is O(1) beyond the input vector.
#include <cmath>
#include <vector>
#include <algorithm>

struct Atom {
    double x, y, z;
    int type;
};

// Compute the Dreiding hydrogen-bond energy for a donor-acceptor-hydrogen triple.
double hydrogenBondEnergy(const std::vector<Atom>& atoms,
                          int donorIndex,
                          int acceptorIndex,
                          int hydrogenIndex,
                          double epsilon,
                          double sigma,
                          double cutInner,
                          double cutOuter,
                          double cutAngleDegrees,
                          int angularPower) {
    const double PI = 3.14159265358979323846;
    const double SMALL = 0.001;

    // Validate indices
    if (donorIndex < 0 || donorIndex >= static_cast<int>(atoms.size()) ||
        acceptorIndex < 0 || acceptorIndex >= static_cast<int>(atoms.size()) ||
        hydrogenIndex < 0 || hydrogenIndex >= static_cast<int>(atoms.size()))
        return 0.0;

    const Atom& donor = atoms[donorIndex];
    const Atom& acceptor = atoms[acceptorIndex];
    const Atom& hydrogen = atoms[hydrogenIndex];

    // Vectors from hydrogen to donor and hydrogen to acceptor
    double dhx = donor.x - hydrogen.x;
    double dhy = donor.y - hydrogen.y;
    double dhz = donor.z - hydrogen.z;
    double ahx = acceptor.x - hydrogen.x;
    double ahy = acceptor.y - hydrogen.y;
    double ahz = acceptor.z - hydrogen.z;

    double rDH2 = dhx*dhx + dhy*dhy + dhz*dhz;
    double rAH2 = ahx*ahx + ahy*ahy + ahz*ahz;
    if (rDH2 < SMALL*SMALL || rAH2 < SMALL*SMALL)
        return 0.0;
    double rDH = std::sqrt(rDH2);
    double rAH = std::sqrt(rAH2);

    // Angle cosine at hydrogen
    double cosTheta = (dhx*ahx + dhy*ahy + dhz*ahz) / (rDH * rAH);
    cosTheta = std::max(-1.0, std::min(1.0, cosTheta));
    double theta = std::acos(cosTheta);

    // Angular cutoff: reject if angle is too small or too large (i.e., not in the allowed band)
    double cutAngleRad = cutAngleDegrees * PI / 180.0;
    double upperAngle = PI - cutAngleRad; // since max angle is 180 degrees
    if (theta < cutAngleRad || theta > upperAngle)
        return 0.0;

    // Donor-acceptor distance
    double dax = donor.x - acceptor.x;
    double day = donor.y - acceptor.y;
    double daz = donor.z - acceptor.z;
    double rsq = dax*dax + day*day + daz*daz;
    double rOuterSq = cutOuter * cutOuter;
    if (rsq >= rOuterSq)
        return 0.0;

    double rInnerSq = cutInner * cutInner;

    // Radial switching function
    double switchFactor = 1.0;
    if (rsq > rInnerSq) {
        double denom = (rOuterSq - rInnerSq);
        denom = denom * denom * denom;
        double term = (rOuterSq - rsq);
        switchFactor = term * term * (rOuterSq + 2.0*rsq - 3.0*rInnerSq) / denom;
    }

    // Lennard-Jones kernel using sigma and r (donor-acceptor distance)
    double r = std::sqrt(rsq);
    double sigmaOverR = sigma / r;
    double sigmaOverR12 = std::pow(sigmaOverR, 12);
    double sigmaOverR6 = std::pow(sigmaOverR, 6);
    double ljKernel = sigmaOverR12 - sigmaOverR6;

    // Angular term: cos^p(theta)
    double angularTerm = std::pow(cosTheta, angularPower);

    double energy = 5.0 * epsilon * ljKernel * angularTerm * switchFactor;
    return energy;
}
#include <cassert>
#include <cmath>
#include <vector>

// (Atom struct and hydrogenBondEnergy function should be included here)

int main() {
    // Simple linear configuration: D--H--A with 180-degree angle
    std::vector<Atom> atoms = {
        {0.0, 0.0, 0.0, 1},   // donor
        {2.0, 0.0, 0.0, 2},   // hydrogen
        {4.0, 0.0, 0.0, 3}    // acceptor
    };
    // Donor-acceptor distance = 4, sigma=1, epsilon=1, no angular cutoff (0 degrees), p=2
    // Energy = 5*1*[(1/4)^12 - (1/4)^6]*cos^2(180°)=5*[1/16777216 - 1/4096]*(1)
    double expected = 5.0 * (std::pow(0.25, 12) - std::pow(0.25, 6));
    assert(std::fabs(hydrogenBondEnergy(atoms, 0, 2, 1, 1.0, 1.0, 0.0, 10.0, 0.0, 2) - expected) < 1e-9);

    // Angle 90 degrees: cos=0, energy should be 0 for p>0
    std::vector<Atom> atoms90 = {
        {0.0, 0.0, 0.0, 1},   // donor
        {1.0, 0.0, 0.0, 2},   // hydrogen
        {1.0, 1.0, 0.0, 3}    // acceptor (perpendicular)
    };
    // Donor-acceptor distance = sqrt(2), cos(90°)=0
    assert(std::fabs(hydrogenBondEnergy(atoms90, 0, 2, 1, 1.0, 1.0, 0.0, 5.0, 0.0, 2)) < 1e-9);

    // p=0: angular term is 1, angle anti-bonding (cos=-1) still gives full angular factor
    // Use same 180-degree configuration, p=0
    double expected_p0 = 5.0 * (std::pow(0.25, 12) - std::pow(0.25, 6));
    assert(std::fabs(hydrogenBondEnergy(atoms, 0, 2, 1, 1.0, 1.0, 0.0, 10.0, 0.0, 0) - expected_p0) < 1e-9);

    // Angular cutoff: 150 degrees means theta must be between 150° and 180°-150°=30°? Actually 180-150=30, so valid range [30°,150°]. Our 180° is out of range → 0
    assert(std::fabs(hydrogenBondEnergy(atoms, 0, 2, 1, 1.0, 1.0, 0.0, 10.0, 150.0, 2)) < 1e-9);

    // Inner and outer cutoff: place acceptor far enough that rsq > outer^2
    std::vector<Atom> atomsFar = {
        {0.0, 0.0, 0.0, 1},
        {1.0, 0.0, 0.0, 2},
        {20.0, 0.0, 0.0, 3}
    };
    assert(std::fabs(hydrogenBondEnergy(atomsFar, 0, 2, 1, 1.0, 1.0, 0.0, 5.0, 0.0, 2)) < 1e-9);

    // Invalid index returns 0
    assert(std::fabs(hydrogenBondEnergy(atoms, 5, 2, 1, 1.0, 1.0, 0.0, 10.0, 0.0, 2)) < 1e-9);

    // Zero hydrogen-donor distance (degenerate) should return 0
    std::vector<Atom> atomsDegenerate = {
        {0.0, 0.0, 0.0, 1},   // donor
        {0.0, 0.0, 0.0, 2},   // hydrogen at same position
        {2.0, 0.0, 0.0, 3}
    };
    assert(std::fabs(hydrogenBondEnergy(atomsDegenerate, 0, 2, 1, 1.0, 1.0, 0.0, 10.0, 0.0, 2)) < 1e-9);

    // Verify switching at inner boundary: for r = cutInner exactly, switch=1
    // Set donor at (0,0,0), acceptor at (2,0,0), so r=2. Use cutInner=2, cutOuter=4.
    // Then energy should equal unsmoothed value.
    double unsmoothed = 5.0 * (std::pow(0.5, 12) - std::pow(0.5, 6)) * 1.0; // cos(180°)=1
    assert(std::fabs(hydrogenBondEnergy(atoms, 0, 2, 1, 1.0, 2.0, 2.0, 4.0, 0.0, 1) - unsmoothed) < 1e-9);

    // Verify switching at outer boundary: r slightly less than cutOuter gives small factor
    // Use cutInner=0, cutOuter=4, r=3.9 for donor-acceptor distance (using donor at 0, acceptor at 3.9, hydrogen at 1)
    std::vector<Atom> atomsOuter = {
        {0.0, 0.0, 0.0, 1},
        {1.0, 0.0, 0.0, 2},
        {3.9, 0.0, 0.0, 3}
    };
    double rsq = 3.9*3.9;
    double rOutSq = 16.0;
    double rInSq = 0.0;
    double denom = (rOutSq - rInSq) * (rOutSq - rInSq) * (rOutSq - rInSq);
    double switchVal = ((rOutSq - rsq)*(rOutSq - rsq) * (rOutSq + 2.0*rsq - 3.0*rInSq)) / denom;
    double expectedOuter = 5.0 * (std::pow(1.0/3.9, 12) - std::pow(1.0/3.9, 6)) * switchVal;
    assert(std::fabs(hydrogenBondEnergy(atomsOuter, 0, 2, 1, 1.0, 1.0, 0.0, 4.0, 0.0, 1) - expectedOuter) < 1e-9);

    return 0;
}
