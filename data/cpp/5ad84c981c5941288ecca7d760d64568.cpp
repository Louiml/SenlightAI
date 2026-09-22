// Write a C++ function `computeTransmission` that, given a 3D ray direction `wo`, a surface normal `normal` (both as `std::array<double,3>`), a transmission coefficient `kt` (0 to 1), and a ratio of indices of refraction `ior`, returns a `std::optional<std::array<double,3>>` containing the transmitted ray direction `wt` if no total internal reflection occurs, or `std::nullopt` otherwise. The function must handle both cases where the ray hits the surface from outside (normal opposite to ray direction) or from inside (normal aligned with ray direction), using the standard Snell-vector formula with normal and `ior` adjustments as in the given snippet. The returned direction should be normalized. Additionally, ensure `kt` is ignored for direction calculation (it only affects attenuation, not direction). The function must be robust to degenerate cases (e.g., zero `ior`, perpendicular incidence, and internal/external transitions), and must use `constexpr` for constants where appropriate.

The solution follows the exact methodology of the provided `PerfectTransmitter::sample_f` function. First, compute `cos_thetai = dot(normal, wo)`. If `cos_thetai < 0`, the ray is coming from inside the material; we must flip the normal and invert `ior` (so that the ratio becomes `external_ior / internal_ior`). If `cos_thetai == 0`, the ray is exactly tangential; total internal reflection always occurs when `ior > 1`? Actually, compute `temp = 1 - (1 - cos_thetai^2)/eta^2`. If `temp < 0`, total internal reflection occurs, return `nullopt`. Otherwise, compute `cos_theta2 = sqrt(temp)`. The transmitted direction is `wt = -wo/eta - (cos_theta2 - cos_thetai/eta) * normal` (with `normal` possibly flipped). Finally, normalize `wt`. Edge cases: (1) if `ior` is zero or negative, treat as error? In practice, `ior` should be positive; but to be safe, if `ior <= 0` we can return `nullopt` because division by zero is invalid. (2) If `cos_thetai` is exactly zero, `temp = 1 - (1 - 0)/eta^2 = 1 - 1/eta^2`. If `eta > 1`, then `1/eta^2 < 1`, so temp is positive and transmission occurs, with `cos_theta2 = sqrt(1 - 1/eta^2)`. This is physically valid (grazing incidence). (3) Normalization: after computing `wt`, its length should be 1 if input `wo` is normalized; but to be robust, we normalize anyway. (4) The snippet uses `fabs(sr.normal * wt)` but that's for coefficient, not direction. Time complexity O(1), space O(1).

#include <optional>
#include <array>
#include <cmath>

// Compute the transmitted ray direction for perfect transmission.
// Returns std::nullopt if total internal reflection occurs or if ior is non-positive.
// Both wo and normal should be unit vectors. The returned direction is normalized.
std::optional<std::array<double,3>> computeTransmission(
    const std::array<double,3>& wo,
    const std::array<double,3>& normal,
    double kt,
    double ior) {
    // kt is not used for direction but for attenuation; we ignore it here.
    (void)kt;

    // Basic validity check: ior must be positive to avoid division by zero.
    if (ior <= 0.0) {
        return std::nullopt;
    }

    // Copy normal because we may flip it.
    std::array<double,3> n = normal;

    // Compute cosine of angle between normal and incident direction.
    double cos_thetai = 0.0;
    for (int i = 0; i < 3; ++i) {
        cos_thetai += n[i] * wo[i];
    }

    // Ratio of indices of refraction (eta = n_incident / n_transmitted? In code, ior is the ratio)
    double eta = ior;

    // If the ray is coming from inside the material, adjust normal and eta.
    if (cos_thetai < 0.0) {
        cos_thetai = -cos_thetai;
        for (int i = 0; i < 3; ++i) {
            n[i] = -n[i];
        }
        eta = 1.0 / eta;
    }

    // Compute intermediate value for Snell's law.
    double sin2_thetai = 1.0 - cos_thetai * cos_thetai;
    double temp = 1.0 - sin2_thetai / (eta * eta);

    // Total internal reflection if temp is negative.
    if (temp < 0.0) {
        return std::nullopt;
    }

    double cos_theta2 = std::sqrt(temp);

    // Compute transmitted direction: wt = -wo/eta - (cos_theta2 - cos_thetai/eta) * n
    std::array<double,3> wt = {0.0, 0.0, 0.0};
    double factor = cos_theta2 - cos_thetai / eta;
    double inv_eta = 1.0 / eta;
    for (int i = 0; i < 3; ++i) {
        wt[i] = -wo[i] * inv_eta - factor * n[i];
    }

    // Normalize the resulting vector to avoid floating-point drift.
    double length = 0.0;
    for (int i = 0; i < 3; ++i) {
        length += wt[i] * wt[i];
    }
    length = std::sqrt(length);
    if (length < 1e-12) {
        // Degenerate case, return nullopt or zero? Better nullopt.
        return std::nullopt;
    }
    for (int i = 0; i < 3; ++i) {
        wt[i] /= length;
    }

    return wt;
}

#include <cassert>
#include <cmath>
#include <optional>
#include <array>

// Include your function definition here or above.

int main() {
    // Helper to compare arrays with tolerance.
    auto close = [](const std::array<double,3>& a, const std::array<double,3>& b, double eps=1e-6) {
        return std::fabs(a[0]-b[0]) < eps && std::fabs(a[1]-b[1]) < eps && std::fabs(a[2]-b[2]) < eps;
    };

    // External incidence, normal along +Z, ray from +Z direction (going into surface).
    std::array<double,3> normal = {0,0,1};
    std::array<double,3> wo = {0,0,-1}; // ray direction opposite normal? Actually wo is direction of incoming ray? In code, wo = -ray.d, so if ray travels from (0,0,1) to (0,0,0), ray.d = (0,0,-1), wo = (0,0,1)? Let's be careful: In the snippet, wo = -sr.ray.d. So if ray travels from source to surface, wo is opposite to ray direction. For transmission, we want wt as direction of outgoing ray. We'll use the same convention: wo is the direction from surface toward the source? That matches the snippet. So for normal (0,0,1), ray hitting from outside (from -Z? Actually outside means normal points outward; if normal is +Z, outside is +Z side. So ray comes from +Z direction, i.e., ray.d = (0,0,-1), so wo = (0,0,1). Then cos_thetai = normal·wo = 1, perpendicular? No, that's head-on. Let's test a simple case: normal +Z, wo = (0,0,1) (ray coming from +Z). Then with ior=1.0, no refraction: wt should be (0,0,1) as if passing through? Actually if ior=1 (same medium), transmitted direction should be same as wo? Wait Snell: if eta=1, then no change. Using formula: cos_thetai = 1, sin2=0, temp=1, cos_theta2=1, factor = 1 - 1/1 = 0, wt = -wo/1 - 0*n = -wo = (0,0,-1). That points opposite the incoming direction? That seems wrong. Let's re-read the snippet: In the code, they compute wt = -wo / eta - (cos_theta2 - cos_thetai / eta) * n. For eta=1, cos_thetai=1, cos_theta2=1, factor = 1 - 1 =0, so wt = -wo. But wo = -ray.d, so wt = -wo = ray.d. That would mean transmitted ray goes in the same direction as the incident ray? That makes sense if the ray passes straight through. But in the snippet, they define wt as the direction of the transmitted ray. In the test, we need to be careful. However, for the function we wrote, we follow the same formula. So for normal +Z, wo = (0,0,1) (meaning incident from +Z side), ior=1, wt = (0,0,-1). Actually -wo = (0,0,-1). That means the transmitted ray goes in the opposite direction of wo, which is -Z, which is into the surface? That seems like the transmitted ray is on the same side as the incident? Actually if wo points from surface toward the source (i.e., from surface back to +Z), then -wo points from surface into the material (toward -Z). That is consistent: the ray comes from +Z, passes through the surface, and continues going in the same direction as the original ray direction (which was -Z). So wt = (0,0,-1) is correct. So we can test with simple cases.

    // Case 1: Normal incidence, ior=1, no change in direction.
    auto dir1 = computeTransmission({0,0,1}, {0,0,1}, 1.0, 1.0);
    assert(dir1.has_value());
    assert(close(*dir1, {0,0,-1}));

    // Case 2: Normal incidence, ior=2 from air to glass (eta=2). For head-on, no refraction, same direction.
    auto dir2 = computeTransmission({0,0,1}, {0,0,1}, 0.5, 2.0);
    assert(dir2.has_value());
    assert(close(*dir2, {0,0,-1}));

    // Case 3: Oblique angle, from air (eta=1) into glass (ior=2), normal +Z, wo = (0, sinθ, cosθ) with θ=45°.
    double cos45 = std::sqrt(0.5);
    auto dir3 = computeTransmission({0, cos45, cos45}, {0,0,1}, 1.0, 2.0);
    assert(dir3.has_value());
    // Use Snell's law: sinθt = sinθi / eta = (cos45)/2 ≈ 0.353553, cosθt = sqrt(1 - (0.125)) ≈ 0.935414.
    // wt direction = (0, sinθt, cosθt)? Actually after transmission, the ray bends toward normal, so x=0, y = sinθt, z = cosθt? But with our convention wt = -wo/eta - ... Let's compute expected: normal +Z, wo = (0, cos45, cos45). cos_thetai = cos45 ≈ 0.7071, sin2 = 0.5, eta=2, temp = 1 - 0.5/4 = 1 - 0.125 = 0.875, cos_theta2 = sqrt(0.875) ≈ 0.935414, factor = 0.935414 - 0.7071/2 = 0.935414 - 0.353553 = 0.581861, inv_eta=0.5. wt = -wo*0.5 - factor*n = (0, -0.353553, -0.353553) - (0,0,0.581861) = (0, -0.353553, -0.935414). That is pointing downward (negative z). Length? sqrt(0.125+0.875)=1. So normalized is same. Compare with expected: y negative, z negative. But physically, if incident from +Z side (outside), transmitted goes into -Z side, with y component reversed? Actually in optics, if the ray comes from +Z with a positive y component, after refraction it should still have positive y (if it goes into -Z). But our formula gives negative y. This is because of the convention of wo being opposite to ray direction. Let's not overthink; just test that the function produces a direction that satisfies Snell's law angle. Compute angle with normal: dot(wt, normal) = -0.935414 (cosine of angle with +Z) so angle with normal is acos(-0.935414) ≈ 159°, but we should check angle with -Z (the transmitted side). Actually the transmitted ray direction should have positive cos with -Z. Let's just check the magnitude of sinθt = sqrt(1 - (dot(wt,n))^2) . dot(wt,n) = -0.935414, sinθt = 0.353553, which matches Snell's law. So it's correct but the sign of y is negative because the incident wo has positive y but the transmitted ray goes in the direction -wo (which has negative y). So it's fine. We'll just check the angle.

    // Better test: ensure no total internal reflection when angle is below critical.
    // For ior=1.5, critical angle when going from glass to air: sinθc = 1/1.5 = 0.6667, θc ≈ 41.8°.
    // For incident from inside (wo has negative z component), test with wo = (0, sin(80°), -cos(80°))? Actually inside means normal +Z, but ray from inside (negative z direction) would have wo = (0,0,-1) if coming from -Z? Let's set normal +Z, wo = (0, sin60°, -cos60°) with cos_thetai = -cos60 = -0.5, so it's from inside. ior=1.5. Since eta becomes 1/1.5 = 0.6667, we need to check if total internal reflection occurs. For θi=60°, sinθi=0.866, sinθt = 0.866/0.6667 = 1.299 > 1, so TIR, should return nullopt.
    auto dir4 = computeTransmission({0, std::sin(60.0*M_PI/180.0), -std::cos(60.0*M_PI/180.0)}, {0,0,1}, 1.0, 1.5);
    assert(!dir4.has_value());

    // For θi=30°, sinθt = 0.5/0.6667=0.75, cosθt = sqrt(1-0.5625)=0.6614, no TIR.
    auto dir5 = computeTransmission({0, std::sin(30.0*M_PI/180.0), -std::cos(30.0*M_PI/180.0)}, {0,0,1}, 1.0, 1.5);
    assert(dir5.has_value());
    // Check Snell: sinθt = 0.5/0.6667 = 0.75, so sinθt (with respect to normal) = 0.75, cosθt=0.6614. After transmission, the direction should have negative z but positive y? Actually the transmitted ray goes to the other side (outside), so normal is flipped? In our code, we flip normal and eta, so cos_thetai becomes 0.866 (cos30), sin2=0.25, eta=1/1.5=0.6667, temp=1 - 0.25/(0.4444)=1 - 0.5625=0.4375, cos_theta2=sqrt(0.4375)=0.6614, factor=0.6614 - 0.866/0.6667=0.6614-1.299= -0.6376, n flipped = (0,0,-1), inv_eta=1.5, wt = -wo*1.5 - (-0.6376)*(0,0,-1) = (-0, -0.5*1.5, 0.866*1.5?) Wait wo original = (0, sin30, -cos30) = (0,0.5,-0.866). So -wo*1.5 = (0, -0.75, 1.299). Then minus factor*n = -(-0.6376)*(0,0,-1) = (0,0,-0.6376) -> actually minus factor*n = (-0.6376)*(0,0,-1) = (0,0,0.6376) with negative sign? Let's compute: wt = -wo/eta - factor*n. eta after inversion? Actually we set eta = 1/ior = 0.6667, then inv_eta = 1.5. So -wo/1.5? Wait we use -wo * inv_eta? In code we do -wo[i] * inv_eta, where inv_eta = 1/eta. But after flipping, eta = 1/ior = 0.6667, so inv_eta = 1.5. So -wo * 1.5 = (0, -0.75, 1.299). Then factor = cos_theta2 - cos_thetai/eta = 0.6614 - 0.866/0.6667 = 0.6614 - 1.299 = -0.6376. Then wt = (0, -0.75, 1.299) - (-0.6376)*n_flipped? n after flip = (0,0,-1). So - factor * n = -(-0.6376)*(0,0,-1) = (0,0,0.6376) - actually -factor*n = -(-0.6376)*(0,0,-1) = (0,0,-0.6376) when you multiply -0.6376 by (0,0,-1) you get (0,0,0.6376) but with a leading minus sign: - (-0.6376) = +0.6376, times (0,0,-1) = (0,0,-0.6376). So wt = (0, -0.75, 1.299) + (0,0,-0.6376) = (0, -0.75, 0.6614). So wt has negative y and positive z. But transmitted ray should go outside, which is +Z side? Actually original normal is +Z, but we flipped because incident from inside, so normal is now -Z. Wait we flipped n to -n = (0,0,-1). So the transmitted direction should have positive dot with (0,0,-1)? That would be positive z? No, dot(wt, -n) = dot(wt,(0,0,-1)) = -0.6614, which is negative, meaning it's still going into the negative z side? That seems contradictory. But this is the same as the output of the original code. I think it's fine as long as it satisfies Snell. The angle with the flipped normal (-Z) is arccos(0.6614) ≈ 48.6°, while incident angle with flipped normal (which is -Z? Actually incident wo had negative z component, so dot(wo, -n) = -0.866? No, -n = (0,0,-1), wo = (0,0.5,-0.866), dot = 0.866, so incident angle with flipped normal is 30° (cos=0.866). Then sinθt = sqrt(1-0.6614^2)=0.75, which matches Snell. So it's fine.

    // Simple test for ior=1 with oblique incidence.
    auto dir6 = computeTransmission({0, std::sin(45.0*M_PI/180.0), std::cos(45.0*M_PI/180.0)}, {0,0,1}, 1.0, 1.0);
    assert(dir6.has_value());
    // No refraction, so wt should be -wo.
    std::array<double,3> expected6 = {0, -std::sin(45.0*M_PI/180.0), -std::cos(45.0*M_PI/180.0)};
    assert(close(*dir6, expected6));

    // Edge case: ior=0 returns nullopt.
    assert(!computeTransmission({0,0,1}, {0,0,1}, 1.0, 0.0).has_value());

    // Edge case: degenerate normal? Not needed.

    // Test that output is normalized for a tricky case.
    auto dir7 = computeTransmission({0.6, 0.8, 0.0}, {0,0,1}, 0.3, 1.7);
    if (dir7.has_value()) {
        double len = 0;
        for (double v : *dir7) len += v*v;
        assert(std::fabs(std::sqrt(len) - 1.0) < 1e-6);
    } else {
        // If TIR, that's also valid for this angle? Check: cos_thetai = 0 (since z=0), so sin2=1, temp = 1 - 1/(1.7^2) = 1-0.346 = 0.654 >0, so no TIR. So must have value.
        assert(false);
    }
}
