// Write a C++ function `applyFunctionToRange` that takes a reference to an abstract base class `Dng1DFunction` (modeled after the provided snippet's `dng_1d_function`), two real64 bounds `low` and `high`, and an integer `samples` (required to be at least 2). The function must return a `std::vector<real64>` of length `samples` containing the evenly spaced evaluations of the function from `low` to `high` inclusive, with the first element equal to `function.Evaluate(low)` and the last equal to `function.Evaluate(high)`. The abstract base class must define `virtual ~Dng1DFunction() = default`, `virtual bool IsIdentity() const`, and `virtual real64 Evaluate(real64 x) const`. Provide two concrete derived classes: `Dng1DIdentity` (where `Evaluate(x)` returns `x` and `IsIdentity()` returns `true`) and `Dng1DSquare` (where `Evaluate(x)` returns `x*x` and `IsIdentity()` returns `false`). In your solution, implement `applyFunctionToRange` as a free function that uses the abstract interface, and also provide the class definitions. Ensure that the function works for both increasing and decreasing ranges (i.e., `low` can be greater than `high`) and correctly handles floating-point spacing.

The solution involves defining an abstract base class with a virtual destructor, a virtual `IsIdentity()`, and a pure virtual `Evaluate()`. The two derived classes implement the required behaviors: identity returns `x` unchanged, square returns `x*x`. The free function `applyFunctionToRange` first validates that `samples >= 2`; if not, it can either throw or return an empty vector (we'll return empty and document it, but for robustness use an assert or throw). The step size is computed as `(high - low) / (samples - 1)`, and we iterate from `i = 0` to `i < samples`, computing `x = low + step * i` and calling `Evaluate(x)`. To avoid floating-point drift, it is safer to compute `x` as `low + (high - low) * (double(i) / (samples - 1))` because this preserves the exact endpoints even when `low` and `high` have opposite signs or large magnitudes. The function should be `const` correct, and we should accept the function by `const Dng1DFunction&`. The time complexity is O(samples) for evaluations, and space complexity is O(samples) for the output vector. Edge cases: when `samples == 2`, step is `high - low`; when `low == high`, all samples are the same value; negative ranges are handled naturally because the formula works with signed values. We must ensure that the abstract class has a virtual destructor so derived objects are cleaned up correctly, and we can provide a default implementation of `IsIdentity()` returning `false` for convenience, but the base class itself cannot be instantiated due to the pure virtual `Evaluate`.

#include <vector>
#include <cstdint>

using real64 = double;

// Abstract base class for 1D functions on [0,1] in the original snippet, but here generalized.
class Dng1DFunction {
public:
    virtual ~Dng1DFunction() = default;

    // Returns true if the function is the identity function f(x) = x.
    virtual bool IsIdentity() const {
        return false;
    }

    // Evaluate the function at x.
    virtual real64 Evaluate(real64 x) const = 0;
};

// Identity function: f(x) = x.
class Dng1DIdentity : public Dng1DFunction {
public:
    bool IsIdentity() const override {
        return true;
    }

    real64 Evaluate(real64 x) const override {
        return x;
    }
};

// Square function: f(x) = x * x.
class Dng1DSquare : public Dng1DFunction {
public:
    real64 Evaluate(real64 x) const override {
        return x * x;
    }
};

// Returns a vector of 'samples' evenly spaced evaluations of the function
// from 'low' to 'high' inclusive. Requires samples >= 2.
std::vector<real64> applyFunctionToRange(const Dng1DFunction& function,
                                         real64 low,
                                         real64 high,
                                         int samples) {
    // Return empty vector if invalid sample count.
    if (samples < 2) {
        return {};
    }

    std::vector<real64> result;
    result.reserve(samples);

    const real64 denominator = static_cast<real64>(samples - 1);
    for (int i = 0; i < samples; ++i) {
        // Compute x using a formula that exactly hits endpoints.
        const real64 t = static_cast<real64>(i) / denominator;
        const real64 x = low + (high - low) * t;
        result.push_back(function.Evaluate(x));
    }

    return result;
}

#include <cassert>
#include <cmath>
#include <vector>

// The class definitions and applyFunctionToRange are included from the solution.
// (In an actual test file, they would be included or copied here.)

int main() {
    Dng1DIdentity identity;
    Dng1DSquare square;

    // Identity on [0,1] with 5 samples.
    std::vector<real64> idValues = applyFunctionToRange(identity, 0.0, 1.0, 5);
    assert(idValues.size() == 5);
    assert(idValues[0] == 0.0);
    assert(idValues[4] == 1.0);
    assert(std::abs(idValues[2] - 0.5) < 1e-12);

    // Square on [0,1] with 4 samples.
    std::vector<real64> sqValues = applyFunctionToRange(square, 0.0, 1.0, 4);
    assert(sqValues.size() == 4);
    assert(sqValues[0] == 0.0);
    assert(sqValues[1] == (1.0/3.0) * (1.0/3.0));
    assert(sqValues[2] == (2.0/3.0) * (2.0/3.0));
    assert(sqValues[3] == 1.0);

    // Decreasing range with identity.
    std::vector<real64> decValues = applyFunctionToRange(identity, 2.0, -2.0, 5);
    assert(decValues.size() == 5);
    assert(decValues[0] == 2.0);
    assert(decValues[4] == -2.0);
    assert(decValues[2] == 0.0);

    // Square on negative to positive range.
    std::vector<real64> negSq = applyFunctionToRange(square, -1.0, 1.0, 3);
    assert(negSq.size() == 3);
    assert(negSq[0] == 1.0);
    assert(negSq[1] == 0.0);
    assert(negSq[2] == 1.0);

    // Two samples only.
    std::vector<real64> twoSamples = applyFunctionToRange(identity, 5.0, 5.0, 2);
    assert(twoSamples.size() == 2);
    assert(twoSamples[0] == 5.0);
    assert(twoSamples[1] == 5.0);

    // Invalid sample count returns empty.
    assert(applyFunctionToRange(identity, 0.0, 1.0, 1).empty());
    assert(applyFunctionToRange(identity, 0.0, 1.0, 0).empty());

    // Identity flag behavior.
    assert(identity.IsIdentity() == true);
    assert(square.IsIdentity() == false);
}
