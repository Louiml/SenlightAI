/*
Write a C++ function named `computeFeatureDistanceStatistics` that takes a vector of `FeatureDistance` objects (each with a parameterization type and internal parameter `p_`) and returns a `struct` containing the minimum distance, maximum distance, and average of the absolute values of the distance derivatives across all objects. The `FeatureDistance` class and its methods are provided as-is from the snippet; your function must use the public interface (`setType`, `setParameter`, `getDistance`, `getDistanceDerivative`, etc.) to compute the required statistics. If the input vector is empty, return a struct with all fields set to `0.0`. The function should be `const`-correct and should not modify the input objects. Assume all distances are non-negative (but handle negative values gracefully if they occur). The primary goal is to practice using polymorphism-like dispatch without inheritance, by calling the appropriate methods that internally switch on the stored type.
*/

#include <vector>
#include <cmath>
#include <cstddef>
#include "rovio/FeatureDistance.hpp"

// Structure to hold computed statistics.
struct FeatureDistanceStats {
    double minDistance;
    double maxDistance;
    double avgAbsDerivative;
};

// Compute min, max, and average absolute derivative over a vector of FeatureDistance objects.
FeatureDistanceStats computeFeatureDistanceStatistics(const std::vector<rovio::FeatureDistance>& features) {
    if (features.empty()) {
        return {0.0, 0.0, 0.0};
    }

    double minDist = features[0].getDistance();
    double maxDist = minDist;
    double sumAbsDeriv = 0.0;

    for (const auto& feat : features) {
        double dist = feat.getDistance();
        if (dist < minDist) minDist = dist;
        if (dist > maxDist) maxDist = dist;
        sumAbsDeriv += std::abs(feat.getDistanceDerivative());
    }

    double avgAbsDeriv = sumAbsDeriv / static_cast<double>(features.size());
    return {minDist, maxDist, avgAbsDeriv};
}

#include <cassert>
#include <cmath>
#include "rovio/FeatureDistance.hpp"

int main() {
    // Test 1: Regular distances, simple case.
    std::vector<rovio::FeatureDistance> regs;
    rovio::FeatureDistance r1(rovio::FeatureDistance::REGULAR);
    r1.setParameter(2.0);
    rovio::FeatureDistance r2(rovio::FeatureDistance::REGULAR);
    r2.setParameter(5.0);
    rovio::FeatureDistance r3(rovio::FeatureDistance::REGULAR);
    r3.setParameter(3.0);
    regs.push_back(r1); regs.push_back(r2); regs.push_back(r3);
    auto stats1 = computeFeatureDistanceStatistics(regs);
    assert(stats1.minDistance == 2.0);
    assert(stats1.maxDistance == 5.0);
    assert(std::abs(stats1.avgAbsDerivative - 1.0) < 1e-9); // derivative is 1 for regular

    // Test 2: Inverse distances.
    std::vector<rovio::FeatureDistance> invs;
    rovio::FeatureDistance i1(rovio::FeatureDistance::INVERSE);
    i1.setParameter(4.0); // distance = 1/4 = 0.25
    rovio::FeatureDistance i2(rovio::FeatureDistance::INVERSE);
    i2.setParameter(2.0); // distance = 0.5
    invs.push_back(i1); invs.push_back(i2);
    auto stats2 = computeFeatureDistanceStatistics(invs);
    assert(std::abs(stats2.minDistance - 0.25) < 1e-9);
    assert(std::abs(stats2.maxDistance - 0.5) < 1e-9);
    // derivatives: for inverse, derivative = -1/(p^2) where p is parameter after set
    // i1 p=4 -> -1/16, i2 p=2 -> -1/4; absolute sum = 1/16 + 1/4 = 5/16; avg = 5/32
    assert(std::abs(stats2.avgAbsDerivative - 5.0/32.0) < 1e-9);

    // Test 3: Log distances.
    std::vector<rovio::FeatureDistance> logs;
    rovio::FeatureDistance l1(rovio::FeatureDistance::LOG);
    l1.setParameter(std::exp(1.0)); // distance = e
    rovio::FeatureDistance l2(rovio::FeatureDistance::LOG);
    l2.setParameter(std::exp(3.0)); // distance = e^3
    logs.push_back(l1); logs.push_back(l2);
    auto stats3 = computeFeatureDistanceStatistics(logs);
    assert(std::abs(stats3.minDistance - std::exp(1.0)) < 1e-9);
    assert(std::abs(stats3.maxDistance - std::exp(3.0)) < 1e-9);
    // derivatives: exp(p) = distance itself, so abs sum = e + e^3; avg = (e+e^3)/2
    assert(std::abs(stats3.avgAbsDerivative - (std::exp(1.0)+std::exp(3.0))/2.0) < 1e-9);

    // Test 4: Hyperbolic distances.
    std::vector<rovio::FeatureDistance> hyps;
    rovio::FeatureDistance h1(rovio::FeatureDistance::HYPERBOLIC);
    h1.setParameter(std::sinh(1.0)); // distance = sinh(asinh(d)) = d
    rovio::FeatureDistance h2(rovio::FeatureDistance::HYPERBOLIC);
    h2.setParameter(std::sinh(2.0)); // distance = sinh(asinh(d)) = d
    hyps.push_back(h1); hyps.push_back(h2);
    auto stats4 = computeFeatureDistanceStatistics(hyps);
    assert(std::abs(stats4.minDistance - std::sinh(1.0)) < 1e-9);
    assert(std::abs(stats4.maxDistance - std::sinh(2.0)) < 1e-9);
    // derivatives: cosh(p) where p = asinh(d), so cosh(asinh(d)) = sqrt(d^2+1)
    double d1 = std::sinh(1.0), d2 = std::sinh(2.0);
    double deriv1 = std::sqrt(d1*d1 + 1.0);
    double deriv2 = std::sqrt(d2*d2 + 1.0);
    assert(std::abs(stats4.avgAbsDerivative - (deriv1+deriv2)/2.0) < 1e-9);

    // Test 5: Empty vector.
    std::vector<rovio::FeatureDistance> empty;
    auto stats5 = computeFeatureDistanceStatistics(empty);
    assert(stats5.minDistance == 0.0);
    assert(stats5.maxDistance == 0.0);
    assert(stats5.avgAbsDerivative == 0.0);

    return 0;
}

// The solution iterates over each `FeatureDistance` object in the input vector. For each object, it calls `getDistance()` to obtain the current distance and `getDistanceDerivative()` to obtain the derivative of distance with respect to the parameter. The minimum and maximum distances are tracked using standard comparisons, initialized from the first element if the vector is non-empty. The sum of absolute values of derivatives is accumulated, then divided by the number of elements to compute the average. Edge cases include an empty vector (return zeros) and potential zero distances or parameters that might cause division-by-zero issues inside the `FeatureDistance` methods; the provided class already handles this via `makeNonZero`. Time complexity is O(n) because each object is processed once. Space complexity is O(1) extra, excluding the input vector itself and the returned struct.
