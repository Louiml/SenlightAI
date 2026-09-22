// Write a C++ function that demonstrates the construction of various probability distributions from the Boost.Math library, specifically focusing on the negative binomial, binomial, beta, gamma, and normal distributions. The function should accept a template parameter `RealType` (defaulting to `double`) and a boolean flag `useHighPrecision` (defaulting to `false`). When `useHighPrecision` is `false`, the function should construct and return (as a `std::string`) a comma-separated list of the success probability `p` (i.e., `pdf(x)` at a fixed `x = 1.5`) for each distribution, using `RealType` as the underlying type where possible. When `useHighPrecision` is `true`, it should additionally demonstrate the correct and incorrect ways to pass a high-precision value (using `boost::multiprecision::cpp_dec_float_50`) to the negative binomial's `pdf`, and return a formatted report that includes the high-precision `pdf` value computed correctly (from a string literal) and a note about accidental truncation. The function must handle edge cases like integer arguments (which are implicitly converted) and avoid naming clashes by using explicit `using` declarations. The final output string should be well-formatted with each distribution's name and corresponding `pdf` value, and it must compile with minimal includes.

The core idea is to use Boost.Math's distribution classes with different template types and constructors to showcase flexibility. For each distribution, we construct an instance with appropriate parameters (e.g., negative binomial with success count 8 and success fraction 0.25), then call the `pdf` function at a fixed point `x = 1.5` (or for high precision, a carefully constructed `cpp_dec_float_50` value). The main algorithm involves: (1) including necessary Boost headers; (2) defining a template function that takes `RealType` and a boolean; (3) inside the function, creating distribution objects using `boost::math::negative_binomial_distribution<RealType>`, `boost::math::binomial_distribution<RealType>`, `boost::math::beta_distribution<RealType>`, `boost::math::gamma_distribution<RealType>`, and `boost::math::normal_distribution<RealType>`; (4) computing `pdf` at `static_cast<RealType>(1.5)` and converting to string via `std::ostringstream` with appropriate precision (e.g., `std::numeric_limits<RealType>::digits10`); (5) for the high-precision case, also compute `pdf` for a `cpp_dec_float_50` value constructed from a string and from a double literal to demonstrate truncation, then append a note. Edge cases: integer arguments to constructors (e.g., `negative_binomial mydist(5, 0.4)`) are handled via implicit conversion; when `RealType` is `float`, double literals like `0.25` are truncated (but that's acceptable); the beta and gamma distributions must use `beta_distribution` and `gamma_distribution` directly because convenience typedefs are not provided. Time complexity is O(1) per distribution as we only evaluate a single point; space complexity is O(1) for temporary objects and O(L) for the output string, where L is the total length of the formatted results. The main challenge is ensuring correct type conversions and avoiding ambiguity with `std` names, which we resolve by explicit `using boost::math::...` statements.

#include <string>
#include <sstream>
#include <iomanip>
#include <limits>
#include <boost/math/distributions/negative_binomial.hpp>
#include <boost/math/distributions/binomial.hpp>
#include <boost/math/distributions/beta.hpp>
#include <boost/math/distributions/gamma.hpp>
#include <boost/math/distributions/normal.hpp>
#include <boost/multiprecision/cpp_dec_float.hpp>

// Construct various distributions and return a formatted report of their pdf values at x=1.5.
// If useHighPrecision is true, also demonstrate correct/incorrect high-precision handling.
template <typename RealType = double>
std::string distributionConstructionDemo(bool useHighPrecision = false) {
    std::ostringstream out;
    out << std::setprecision(std::numeric_limits<RealType>::digits10);

    // Use explicit using declarations to avoid ambiguity and shorten names.
    using boost::math::negative_binomial_distribution;
    using boost::math::binomial_distribution;
    using boost::math::beta_distribution;
    using boost::math::gamma_distribution;
    using boost::math::normal_distribution;

    // Negative binomial with double arguments.
    negative_binomial_distribution<RealType> negBinom(8.0, 0.25);
    out << "negative_binomial pdf: " << pdf(negBinom, static_cast<RealType>(1.5)) << "\n";

    // Negative binomial with integer and double arguments (implicit conversion).
    negative_binomial_distribution<RealType> negBinomInt(5, 0.4);
    out << "negative_binomial_int pdf: " << pdf(negBinomInt, static_cast<RealType>(1.5)) << "\n";

    // Binomial distribution.
    binomial_distribution<RealType> binom(1, 0.5);
    out << "binomial pdf: " << pdf(binom, static_cast<RealType>(1.5)) << "\n";

    // Beta distribution (no convenience typedef).
    beta_distribution<RealType> betaDist(1, 0.5);
    out << "beta pdf: " << pdf(betaDist, static_cast<RealType>(1.5)) << "\n";

    // Gamma distribution (no convenience typedef).
    gamma_distribution<RealType> gammaDist(1, 0.5);
    out << "gamma pdf: " << pdf(gammaDist, static_cast<RealType>(1.5)) << "\n";

    // Normal distribution with defaults.
    normal_distribution<RealType> norm;
    out << "normal pdf: " << pdf(norm, static_cast<RealType>(1.5)) << "\n";

    if (useHighPrecision) {
        // High precision demonstration with cpp_dec_float_50.
        using boost::multiprecision::cpp_dec_float_50;
        negative_binomial_distribution<cpp_dec_float_50> highPrecNegBinom(8, cpp_dec_float_50("0.25"));
        cpp_dec_float_50 xHigh("1.23456789012345678901234567890");

        out << "high_precision_correct: " << pdf(highPrecNegBinom, xHigh) << "\n";
        // Show the truncated version (from double) for comparison.
        out << "high_precision_truncated: " << pdf(highPrecNegBinom, static_cast<cpp_dec_float_50>(1.23456789012345678901234567890)) << "\n";
    }

    return out.str();
}

#include <cassert>
#include <string>

int main() {
    // Test double precision output contains expected distribution names.
    std::string resultDouble = distributionConstructionDemo<double>(false);
    assert(resultDouble.find("negative_binomial pdf:") != std::string::npos);
    assert(resultDouble.find("binomial pdf:") != std::string::npos);
    assert(resultDouble.find("beta pdf:") != std::string::npos);
    assert(resultDouble.find("gamma pdf:") != std::string::npos);
    assert(resultDouble.find("normal pdf:") != std::string::npos);

    // Test that high precision mode includes both correct and truncated entries.
    std::string resultHigh = distributionConstructionDemo<double>(true);
    assert(resultHigh.find("high_precision_correct:") != std::string::npos);
    assert(resultHigh.find("high_precision_truncated:") != std::string::npos);

    // Test float precision still works.
    std::string resultFloat = distributionConstructionDemo<float>(false);
    assert(resultFloat.find("negative_binomial pdf:") != std::string::npos);

    // Verify that the output for double and float differ in precision (at least in representation).
    assert(resultDouble != resultFloat);

    // Verify the high-precision correct value matches the expected known value (from documentation).
    // The exact value is 0.00012630010495970320103876754721976419438231705359935
    size_t pos = resultHigh.find("high_precision_correct:") + 23; // skip label
    std::string correctValue = resultHigh.substr(pos, resultHigh.find('\n', pos) - pos);
    assert(correctValue == "0.00012630010495970320103876754721976419438231705359935");

    // Verify the truncated value is different from the correct one.
    size_t posTrunc = resultHigh.find("high_precision_truncated:") + 25;
    std::string truncatedValue = resultHigh.substr(posTrunc, resultHigh.find('\n', posTrunc) - posTrunc);
    assert(correctValue != truncatedValue);
}
