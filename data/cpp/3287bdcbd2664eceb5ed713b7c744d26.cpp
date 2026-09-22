// Write a C++ function that takes an `Eigen::Array4d` (a fixed-size array of 4 doubles) and returns a new `Eigen::Array4d` where each element has been replaced by its error function value, using the `erf()` method available from Eigen's unsupported special functions module. The function must perform the operation element-wise and preserve the input array's values (i.e., it must not modify the input). Test the function with an array containing both positive and negative values, including zero, and verify that the output matches the expected error function values.

The error function `erf(x)` is a standard special function defined as the integral of a Gaussian. Eigen's `Array` class provides an `erf()` member function that applies this operation element-wise. The solution is straightforward: create a copy of the input array and call `erf()` on it, returning the result. Since `Array4d` is a fixed-size type, copying and the element-wise operation are done in constant time (no dynamic allocation). Edge cases include values like 0 (erf(0)=0), negative values (erf(x) is odd, so erf(-x) = -erf(x)), and large magnitude values where erf approaches ±1. The function should be marked `const`-correct: it takes a `const Array4d&` and returns by value, ensuring the input is not modified. Time complexity is O(4) = O(1), space complexity is O(1) for the copy.

#include <Eigen/Core>
#include <unsupported/Eigen/SpecialFunctions>

// Compute the element-wise error function of a 4-element Eigen array.
// Returns a new array; does not modify the input.
Eigen::Array4d errorFunction(const Eigen::Array4d& input) {
    // Create a copy to avoid modifying the caller's data.
    Eigen::Array4d result = input;
    // Apply erf() element-wise.
    result = result.erf();
    return result;
}

#include <cassert>
#include <cmath>
#include <Eigen/Core>
#include <unsupported/Eigen/SpecialFunctions>

// The solution function (declared as in the solution section).
Eigen::Array4d errorFunction(const Eigen::Array4d& input);

int main() {
    // Test with a mix of positive, negative, and zero values.
    Eigen::Array4d input(-0.5, 2.0, 0.0, -7.0);
    Eigen::Array4d output = errorFunction(input);

    // Expected values computed using std::erf (C++11) for verification.
    assert(std::abs(output(0) - std::erf(-0.5)) < 1e-12);
    assert(std::abs(output(1) - std::erf(2.0)) < 1e-12);
    assert(std::abs(output(2) - std::erf(0.0)) < 1e-12); // erf(0) = 0 exactly
    assert(std::abs(output(3) - std::erf(-7.0)) < 1e-12);

    // Verify input is unchanged.
    assert(input(0) == -0.5);
    assert(input(1) == 2.0);
    assert(input(2) == 0.0);
    assert(input(3) == -7.0);

    // Additional edge case: all positive large values.
    Eigen::Array4d large(3.0, 5.0, 10.0, 1e6);
    Eigen::Array4d outputLarge = errorFunction(large);
    assert(std::abs(outputLarge(0) - std::erf(3.0)) < 1e-12);
    assert(std::abs(outputLarge(1) - std::erf(5.0)) < 1e-12);
    assert(std::abs(outputLarge(2) - std::erf(10.0)) < 1e-12);
    assert(std::abs(outputLarge(3) - 1.0) < 1e-12); // erf(1e6) is essentially 1.

    return 0;
}
