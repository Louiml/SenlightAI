/*
Write a standalone C++ function named `linearFitWithUncertainties` that reads paired data points from a text file whose name is passed as a parameter, performs a linear least-squares fit \( y = mx + b \), and returns a `struct FitResult` containing the slope \( m \), intercept \( b \), and their standard uncertainties \( \Delta m \) and \( \Delta b \). The file format is two whitespace-separated numbers per line (x and y), with possibly blank lines and trailing whitespace. The number of points \( N \) must be at least 3; otherwise, throw a `std::runtime_error` with an informative message. Use the standard formulas: means \( \bar{x} = \frac{1}{N}\sum x_i \), \( \bar{y} = \frac{1}{N}\sum y_i \), \( m = \frac{\sum (x_i-\bar{x})(y_i-\bar{y})}{\sum (x_i-\bar{x})^2} \), \( b = \bar{y} - m\bar{x} \), \( s^2 = \frac{1}{N-2}\sum (y_i - mx_i - b)^2 \), \( \Delta m = \sqrt{ \frac{s^2}{\sum (x_i-\bar{x})^2} } \), \( \Delta b = \Delta m \sqrt{ \frac{1}{N}\sum x_i^2 } \). The function must be const-correct and use `double` precision throughout. Do not write a `main` function in the solution code; that will be provided in the test section.
*/
#include <fstream>
#include <stdexcept>
#include <vector>
#include <cmath>
#include <string>

struct FitResult {
    double slope;       // m
    double intercept;   // b
    double slopeErr;    // Δm
    double interceptErr; // Δb
};

// Perform linear least-squares fit y = m*x + b with uncertainties.
// Reads pairs from a text file (two numbers per line, whitespace-separated).
// Throws std::runtime_error on file open failure, insufficient data, or zero variance in x.
FitResult linearFitWithUncertainties(const std::string& filename) {
    std::ifstream fin(filename);
    if (!fin) {
        throw std::runtime_error("Cannot open file: " + filename);
    }

    std::vector<double> xs, ys;
    double x, y;
    while (fin >> x >> y) {
        xs.push_back(x);
        ys.push_back(y);
    }

    const std::size_t N = xs.size();
    if (N < 3) {
        throw std::runtime_error("At least 3 data points required for linear fit");
    }

    // Pass 1: compute means
    double sumX = 0.0, sumY = 0.0;
    for (std::size_t i = 0; i < N; ++i) {
        sumX += xs[i];
        sumY += ys[i];
    }
    const double meanX = sumX / static_cast<double>(N);
    const double meanY = sumY / static_cast<double>(N);

    // Pass 2: centered sums
    double sxx = 0.0, sxy = 0.0, syy = 0.0;
    for (std::size_t i = 0; i < N; ++i) {
        const double dx = xs[i] - meanX;
        const double dy = ys[i] - meanY;
        sxx += dx * dx;
        sxy += dx * dy;
        syy += dy * dy;
    }

    if (sxx == 0.0) {
        throw std::runtime_error("All x values are identical; slope is undefined");
    }

    const double slope = sxy / sxx;
    const double intercept = meanY - slope * meanX;

    // Residual variance
    double residSumSq = 0.0;
    for (std::size_t i = 0; i < N; ++i) {
        const double pred = slope * xs[i] + intercept;
        const double resid = ys[i] - pred;
        residSumSq += resid * resid;
    }
    const double s2 = residSumSq / static_cast<double>(N - 2);

    const double slopeErr = std::sqrt(s2 / sxx);
    // Intercept uncertainty: Δb = Δm * sqrt(Σx^2 / N)
    double sumX2 = 0.0;
    for (std::size_t i = 0; i < N; ++i) {
        sumX2 += xs[i] * xs[i];
    }
    const double interceptErr = slopeErr * std::sqrt(sumX2 / static_cast<double>(N));

    return {slope, intercept, slopeErr, interceptErr};
}
#include <cassert>
#include <cmath>
#include <fstream>
#include <stdexcept>

// Include the solution code here (the struct and function) by copying or referencing.

int main() {
    // Test 1: Perfect linear data y = 2x + 1, no noise
    {
        std::ofstream f("test1.txt");
        f << "0 1\n1 3\n2 5\n3 7\n4 9\n";
        f.close();
        FitResult r = linearFitWithUncertainties("test1.txt");
        assert(std::abs(r.slope - 2.0) < 1e-9);
        assert(std::abs(r.intercept - 1.0) < 1e-9);
        assert(r.slopeErr < 1e-9);
        assert(r.interceptErr < 1e-9);
    }

    // Test 2: Horizontal line y = 5
    {
        std::ofstream f("test2.txt");
        f << "-2 5\n0 5\n2 5\n4 5\n";
        f.close();
        FitResult r = linearFitWithUncertainties("test2.txt");
        assert(std::abs(r.slope) < 1e-9);
        assert(std::abs(r.intercept - 5.0) < 1e-9);
        assert(r.slopeErr < 1e-9);
        assert(r.interceptErr < 1e-9);
    }

    // Test 3: Known noisy data (small sample, check approximate values)
    {
        std::ofstream f("test3.txt");
        f << "1 3\n2 5\n3 7\n4 9\n5 11\n"; // y = 2x + 1 exactly
        f.close();
        FitResult r = linearFitWithUncertainties("test3.txt");
        assert(std::abs(r.slope - 2.0) < 1e-6);
        assert(std::abs(r.intercept - 1.0) < 1e-6);
    }

    // Test 4: Non-existent file throws
    {
        bool threw = false;
        try {
            linearFitWithUncertainties("nonexistent.txt");
        } catch (const std::runtime_error&) {
            threw = true;
        }
        assert(threw);
    }

    // Test 5: Fewer than 3 points throws
    {
        std::ofstream f("test5.txt");
        f << "0 0\n1 1\n";
        f.close();
        bool threw = false;
        try {
            linearFitWithUncertainties("test5.txt");
        } catch (const std::runtime_error&) {
            threw = true;
        }
        assert(threw);
    }

    // Test 6: All x identical throws
    {
        std::ofstream f("test6.txt");
        f << "2 1\n2 3\n2 5\n2 7\n";
        f.close();
        bool threw = false;
        try {
            linearFitWithUncertainties("test6.txt");
        } catch (const std::runtime_error&) {
            threw = true;
        }
        assert(threw);
    }

    // Test 7: Blank lines and extra whitespace
    {
        std::ofstream f("test7.txt");
        f << "  0   1  \n\n  1   3  \n  2   5  \n  3   7  \n";
        f.close();
        FitResult r = linearFitWithUncertainties("test7.txt");
        assert(std::abs(r.slope - 2.0) < 1e-9);
        assert(std::abs(r.intercept - 1.0) < 1e-9);
    }

    // Cleanup test files
    std::remove("test1.txt");
    std::remove("test2.txt");
    std::remove("test3.txt");
    std::remove("test5.txt");
    std::remove("test6.txt");
    std::remove("test7.txt");

    return 0;
}
// The core algorithm computes the five sums needed for ordinary least squares: \( \sum x \), \( \sum y \), \( \sum x^2 \), \( \sum xy \), and \( \sum y^2 \), then converts them to means. Alternatively, a more numerically stable approach uses centered sums: compute means first, then accumulate \( S_{xx} = \sum (x_i-\bar{x})^2 \), \( S_{xy} = \sum (x_i-\bar{x})(y_i-\bar{y}) \), and \( S_{yy} = \sum (y_i-\bar{y})^2 \). For robustness against cancellation, we use the centered-sum method after computing means in one pass, then a second pass for the sums; this is \( O(N) \) time and \( O(1) \) auxiliary space for the computation, plus \( O(N) \) space to store input points. Edge cases: if the file cannot be opened, throw `std::runtime_error`; if fewer than 3 points, throw; if \( S_{xx} \) is zero (all x identical), the fit is undefined, so throw. The uncertainties require \( N>2 \), and the formulas yield \( \Delta b = \Delta m \sqrt{ \frac{\sum x_i^2}{N} } \), which is mathematically equivalent to the common expression. The solution must include the `struct FitResult` definition.
