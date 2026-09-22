// Write a C++ function that takes two `Ponto` structures, where `Ponto` is defined as a struct with two `double` members `x` and `y`, and returns the Euclidean distance between them. The function must be named `calcularDistancia`, must take the two points by value (or by const reference for efficiency), must use `std::sqrt` from `<cmath>`, and must compute the distance as the square root of the sum of the squares of the coordinate differences. The function should be correct for negative coordinates, zero coordinates, and points with identical coordinates (distance of 0.0). Ensure the function is `const`-correct, has a descriptive name, and is self-contained with necessary headers. Do not include a `main` function in the solution; the test section will provide the harness.
#include <cassert>
#include <cmath>

// Include the solution's struct and function here (for completeness, replicate them):
struct Ponto {
    double x;
    double y;
};

double calcularDistancia(const Ponto ponto1, const Ponto ponto2) {
    const double dx = ponto2.x - ponto1.x;
    const double dy = ponto2.y - ponto1.y;
    return std::sqrt(dx * dx + dy * dy);
}

int main() {
    // Identical points: distance is 0.
    Ponto p1 = {0.0, 0.0};
    Ponto p2 = {0.0, 0.0};
    assert(calcularDistancia(p1, p2) == 0.0);

    // Simple distance on x-axis.
    p1 = {1.0, 0.0};
    p2 = {4.0, 0.0};
    assert(calcularDistancia(p1, p2) == 3.0);

    // Simple distance on y-axis with negative coordinates.
    p1 = {0.0, -2.0};
    p2 = {0.0, 2.0};
    assert(calcularDistancia(p1, p2) == 4.0);

    // Diagonal distance (3-4-5 triangle).
    p1 = {0.0, 0.0};
    p2 = {3.0, 4.0};
    assert(calcularDistancia(p1, p2) == 5.0);

    // Symmetry: distance(A,B) == distance(B,A).
    p1 = {1.0, 2.0};
    p2 = {4.0, 6.0};
    double d1 = calcularDistancia(p1, p2);
    double d2 = calcularDistancia(p2, p1);
    assert(d1 == d2);
    assert(std::abs(d1 - 5.0) < 1e-12);

    // Negative coordinates in both axes.
    p1 = {-1.0, -1.0};
    p2 = {-4.0, -5.0};
    assert(std::abs(calcularDistancia(p1, p2) - 5.0) < 1e-12);

    // Large values.
    p1 = {1000000.0, 0.0};
    p2 = {1000003.0, 4.0};
    assert(std::abs(calcularDistancia(p1, p2) - 5.0) < 1e-9);

    return 0;
}
#include <cmath>

struct Ponto {
    double x;
    double y;
};

// Calculate the Euclidean distance between two points.
double calcularDistancia(const Ponto ponto1, const Ponto ponto2) {
    const double dx = ponto2.x - ponto1.x;
    const double dy = ponto2.y - ponto1.y;
    return std::sqrt(dx * dx + dy * dy);
}
// The main algorithm computes the differences in the x and y coordinates between the two points, squares each difference, sums them, and returns the square root of that sum using `std::sqrt`. This directly implements the Euclidean distance formula. Edge cases include: both points identical (differences are zero, so sqrt(0) = 0.0), negative coordinates (squaring eliminates sign), and very large coordinate values where the squared sum might overflow if using `float`, but since we use `double`, precision is sufficient for typical inputs. There is no need to handle empty input because the struct always contains two doubles. Time complexity is O(1) — a constant number of arithmetic operations. Space complexity is O(1) — only a few local variables, no dynamic allocation. The function takes both parameters by value in the original snippet, but for better performance and const-correctness, we can pass by const reference; however, to match the snippet exactly, we can also keep by value. We'll choose by value with `const` inside the function, but passing by const reference avoids copies—both are correct. We'll implement with by-value parameters to stay consistent with the original, but note that the function is simple enough that either is fine.
