// Write a C++ function `template<typename T> T totalSquaredMagnitude(const std::vector<std::complex<T>>& vec)` that computes the sum of squared magnitudes of all elements in a vector of complex numbers. The function must handle an empty vector gracefully by returning `T(0)`, work for any floating-point type `T` (e.g., `float`, `double`, `long double`), and use `std::norm` from `<complex>` to compute the squared magnitude of each element. The function must be `const`-correct (accept a `const` reference) and include the necessary `<complex>` and `<vector>` headers. This is inspired by the `mag2` template function in the provided snippet, but your implementation should be simpler and directly use standard library facilities without any external dependencies. Ensure the function is standalone and can be used in any C++ program without modifications.
#include <cassert>
#include <complex>
#include <vector>

// The solution function declaration is included here for completeness.
template <typename T>
T totalSquaredMagnitude(const std::vector<std::complex<T>>& vec);

int main() {
    // Test with empty vector
    std::vector<std::complex<double>> empty;
    assert(totalSquaredMagnitude(empty) == 0.0);

    // Test with single element: (3.0, 4.0) -> |z|^2 = 9 + 16 = 25
    std::vector<std::complex<double>> single = {std::complex<double>(3.0, 4.0)};
    assert(totalSquaredMagnitude(single) == 25.0);

    // Test with multiple elements: (1,1) and (2,-2) -> 1+1 + 4+4 = 10
    std::vector<std::complex<double>> multi = {
        std::complex<double>(1.0, 1.0),
        std::complex<double>(2.0, -2.0)
    };
    assert(totalSquaredMagnitude(multi) == 10.0);

    // Test with float precision and fractional values
    std::vector<std::complex<float>> floatVec = {
        std::complex<float>(0.5f, 0.5f),  // |z|^2 = 0.25 + 0.25 = 0.5
        std::complex<float>(1.0f, 0.0f)   // |z|^2 = 1.0
    };
    assert(totalSquaredMagnitude(floatVec) == 1.5f);

    // Test with long double and negative components
    std::vector<std::complex<long double>> longDoubleVec = {
        std::complex<long double>(-2.0L, 0.0L), // |z|^2 = 4
        std::complex<long double>(0.0L, -3.0L)  // |z|^2 = 9
    };
    assert(totalSquaredMagnitude(longDoubleVec) == 13.0L);

    // Test with zeros
    std::vector<std::complex<double>> zeros(3, std::complex<double>(0.0, 0.0));
    assert(totalSquaredMagnitude(zeros) == 0.0);

    return 0;
}
#include <vector>
#include <complex>

/**
 * Computes the sum of squared magnitudes of all elements in a vector of complex numbers.
 *
 * @param vec A const reference to a vector of std::complex<T>.
 * @return The sum of |z|^2 for all z in vec, as type T. Returns T(0) for an empty vector.
 */
template <typename T>
T totalSquaredMagnitude(const std::vector<std::complex<T>>& vec) {
    T result = T(0);
    for (const auto& element : vec) {
        result += std::norm(element); // std::norm(z) returns |z|^2 as a real value of type T
    }
    return result;
}
// The solution is straightforward: iterate over each element of the input vector, compute its squared magnitude using `std::norm` (which returns the sum of squares of real and imaginary parts), and accumulate the results into a variable of type `T` initialized to `T(0)`. The time complexity is \(O(n)\) where \(n\) is the vector size, and the space complexity is \(O(1)\) because only a single accumulator variable is used. Edge cases: an empty vector returns `T(0)` because the loop never executes and the accumulator remains zero. Since `std::norm` is used, there is no need to manually compute `real*real + imag*imag`, which improves clarity and avoids potential precision issues. The function template works for any type `T` that is supported by `std::complex<T>` (i.e., `float`, `double`, `long double`). The function is declared as `const`-correct by taking a `const std::vector<std::complex<T>>&` parameter, so it can be called on const vectors and does not modify the input. No special handling for NaN or infinity is required.
