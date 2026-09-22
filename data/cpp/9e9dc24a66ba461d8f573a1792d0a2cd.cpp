// Given a `Komplex` class with a real part and an imaginary part (both stored as `double`), a natural ordering can be defined by the magnitude (Euclidean norm) of the complex number, and if magnitudes are equal, by the real part, and if those are also equal, by the imaginary part. Write a standalone C++ function named `sortKomplexVector` that takes a `std::vector<Komplex>` by reference and sorts it in place according to this custom ordering. You must not modify the `Komplex` class itself (no operator overloads for `<` or `==` inside the class), but you can use any standard library tools (e.g., `std::sort` with a lambda) to achieve the ordering. The function should handle vectors of any size, including empty vectors, and must preserve the const-correctness of the input type (i.e., the function modifies the vector, so it takes a non-const reference). Also, for completeness, provide a free function `komplexMagnitude` that returns the magnitude (a `double`) of a given `Komplex` value, and ensure the sorting uses that magnitude consistently. The `Komplex` class is assumed to have public `double re` and `double im` members, and a constructor `Komplex(double re, double im)`.
The solution involves defining a comparator that determines ordering based on three hierarchical keys: first, the magnitude (computed as `sqrt(re*re + im*im)`), then the real part, then the imaginary part. Since `std::sort` requires a strict weak ordering, we must ensure the comparator returns `true` only when the first element should come before the second, and it must be transitive and antisymmetric. Using `std::sort` with a lambda that calls `komplexMagnitude` for both elements and compares them, falling back to `re` and then `im` if magnitudes are equal, yields a deterministic order. Edge cases include empty vectors (sort does nothing), vectors with one element (no change), and vectors with duplicate or equal magnitudes but different real/imaginary parts; the fallback comparisons handle these correctly. Also, floating-point comparisons are used directly; for typical test inputs, exact equality is safe, but in general one might want epsilon tolerance—however, for this task with simple values, direct comparison is acceptable. Time complexity is O(n log n) due to sorting, and space complexity is O(1) auxiliary (assuming the sort is in-place, though `std::sort` may use O(log n) recursion stack), not counting the storage for the vector. The function modifies the input vector in place, so it takes a non-const reference.
#include <vector>
#include <cmath>
#include <algorithm>

// Assuming a Komplex class is defined elsewhere with public double re, im
// and a constructor Komplex(double re, double im). For completeness, here is a minimal definition.
// In a real task, this would be provided externally.

// Returns the magnitude (Euclidean norm) of the complex number.
double komplexMagnitude(const Komplex& k) {
    return std::sqrt(k.re * k.re + k.im * k.im);
}

// Sorts the given vector of Komplex in place according to:
// 1. magnitude ascending
// 2. real part ascending
// 3. imaginary part ascending
void sortKomplexVector(std::vector<Komplex>& v) {
    std::sort(v.begin(), v.end(),
        [](const Komplex& a, const Komplex& b) {
            double magA = komplexMagnitude(a);
            double magB = komplexMagnitude(b);
            if (magA != magB) {
                return magA < magB;
            }
            if (a.re != b.re) {
                return a.re < b.re;
            }
            return a.im < b.im;
        });
}
*Note: The above code assumes `Komplex` is defined elsewhere. If the task requires a self-contained snippet, the class definition can be added. But per the instruction, we only output the free function and necessary headers, without a main. The `Komplex` class is external.*
#include <cassert>
#include <vector>
#include <cmath>

// Minimal Komplex class for testing (matches assumed external definition).
struct Komplex {
    double re;
    double im;
    Komplex(double r, double i) : re(r), im(i) {}
};

// Include the solution functions here (or link to them).
// For brevity, copy the solution code above into this test file.

int main() {
    // Empty vector
    std::vector<Komplex> v0;
    sortKomplexVector(v0);
    assert(v0.empty());

    // Single element
    std::vector<Komplex> v1 = { Komplex{1.0, 0.0} };
    sortKomplexVector(v1);
    assert(v1.size() == 1 && v1[0].re == 1.0 && v1[0].im == 0.0);

    // Multiple elements, different magnitudes
    std::vector<Komplex> v2 = { Komplex{3.0, -4.0}, Komplex{1.0, 2.0}, Komplex{-5.0, -6.0} };
    sortKomplexVector(v2);
    assert(v2[0].re == 1.0 && v2[0].im == 2.0);  // magnitude sqrt(5) ~ 2.236
    assert(v2[1].re == 3.0 && v2[1].im == -4.0); // magnitude 5
    assert(v2[2].re == -5.0 && v2[2].im == -6.0); // magnitude sqrt(61) ~ 7.81

    // Equal magnitudes, different real parts
    std::vector<Komplex> v3 = { Komplex{3.0, -4.0}, Komplex{0.0, 5.0}, Komplex{-3.0, 4.0} };
    sortKomplexVector(v3);
    // magnitudes: 5, 5, 5 all
    // real parts: -3, 0, 3 -> sorted order
    assert(v3[0].re == -3.0 && v3[0].im == 4.0);
    assert(v3[1].re == 0.0 && v3[1].im == 5.0);
    assert(v3[2].re == 3.0 && v3[2].im == -4.0);

    // Equal magnitude and real, different imaginary
    std::vector<Komplex> v4 = { Komplex{1.0, -2.0}, Komplex{1.0, 0.0}, Komplex{1.0, 2.0} };
    sortKomplexVector(v4);
    assert(v4[0].re == 1.0 && v4[0].im == -2.0);
    assert(v4[1].re == 1.0 && v4[1].im == 0.0);
    assert(v4[2].re == 1.0 && v4[2].im == 2.0);

    // Duplicate elements
    std::vector<Komplex> v5 = { Komplex{0.0, 0.0}, Komplex{0.0, 0.0}, Komplex{0.0, 0.0} };
    sortKomplexVector(v5);
    assert(v5.size() == 3);
    for (const auto& k : v5) assert(k.re == 0.0 && k.im == 0.0);

    // Negative reals and large magnitudes
    std::vector<Komplex> v6 = { Komplex{-10.0, 0.0}, Komplex{10.0, 0.0}, Komplex{0.0, -10.0} };
    sortKomplexVector(v6);
    assert(std::fabs(v6[0].re) == 10.0 && std::fabs(v6[0].im) == 0.0); // any with mag 10
    assert(std::fabs(v6[1].re) == 0.0 && std::fabs(v6[1].im) == 10.0);
    assert(std::fabs(v6[2].re) == 10.0 && std::fabs(v6[2].im) == 0.0);
    // Actually need to check ordering: all magnitudes = 10, then sort by re: -10, 0, 10
    assert(v6[0].re == -10.0 && v6[0].im == 0.0);
    assert(v6[1].re == 0.0 && v6[1].im == -10.0);
    assert(v6[2].re == 10.0 && v6[2].im == 0.0);

    return 0;
}
