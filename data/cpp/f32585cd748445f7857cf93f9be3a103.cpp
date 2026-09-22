Write a C++ function named `multiplyPolynomials` that accepts two vectors of polynomial terms, where each term is a struct with an integer exponent `exp` and a double coefficient `coe`. The polynomials are sorted by exponent in ascending order, and terms with a coefficient of zero are not present. The function should return a vector of terms representing the product of the two polynomials, with terms combined by adding coefficients for equal exponents. The result must be sorted by exponent in ascending order, and only terms with a nonzero coefficient (absolute value greater than or equal to 0.005, then rounded to one decimal place for output) should be included. For example, if a combined coefficient is 0.00001, it should be omitted. The function should be robust to empty inputs and handle large exponents and negative coefficients correctly.
#include <cassert>
#include <vector>
#include <cmath>

// Term struct and multiplyPolynomials declaration here...

int main() {
    // (1 + 2x) * (3 + 4x) = 3 + 10x + 8x^2
    std::vector<Term> a{{0,1},{1,2}};
    std::vector<Term> b{{0,3},{1,4}};
    auto r = multiplyPolynomials(a,b);
    assert(r.size()==3);
    assert(r[0].exp==0 && std::abs(r[0].coe - 3.0) < 1e-9);
    assert(r[1].exp==1 && std::abs(r[1].coe - 10.0) < 1e-9);
    assert(r[2].exp==2 && std::abs(r[2].coe - 8.0) < 1e-9);

    // (2x^3) * (5x^2) = 10x^5
    std::vector<Term> c{{3,2}};
    std::vector<Term> d{{2,5}};
    auto r2 = multiplyPolynomials(c,d);
    assert(r2.size()==1);
    assert(r2[0].exp==5 && std::abs(r2[0].coe - 10.0) < 1e-9);

    // (x - 1) * (x + 1) = x^2 - 1
    std::vector<Term> e{{0,-1},{1,1}};
    std::vector<Term> f{{0,1},{1,1}};
    auto r3 = multiplyPolynomials(e,f);
    assert(r3.size()==2);
    assert(r3[0].exp==0 && std::abs(r3[0].coe + 1.0) < 1e-9);
    assert(r3[1].exp==2 && std::abs(r3[1].coe - 1.0) < 1e-9);

    // Zero polynomial * anything = empty
    std::vector<Term> g; // empty
    auto r4 = multiplyPolynomials(g, a);
    assert(r4.empty());

    // Coefficients that cancel exactly
    std::vector<Term> h{{0,1},{1,1}};
    std::vector<Term> i{{0,1},{1,-1}};
    auto r5 = multiplyPolynomials(h,i); // (1+x)*(1-x)=1 - x^2
    assert(r5.size()==2);
    assert(r5[0].exp==0 && std::abs(r5[0].coe - 1.0) < 1e-9);
    assert(r5[1].exp==2 && std::abs(r5[1].coe + 1.0) < 1e-9);

    // Check rounding: (0.1x) * (0.2x) = 0.02x^2 -> rounds to 0.0, should be omitted
    std::vector<Term> j{{1,0.1}};
    std::vector<Term> k{{1,0.2}};
    auto r6 = multiplyPolynomials(j,k);
    assert(r6.empty()); // 0.02 rounds to 0.0, so omitted
}
#include <vector>
#include <map>
#include <cmath>
#include <algorithm>

struct Term {
    int exp;
    double coe;
};

// Multiply two polynomials, combine like terms, filter near-zero coeffs, round to 1 decimal.
// Inputs are sorted by exp ascending, no zero coeff terms.
std::vector<Term> multiplyPolynomials(const std::vector<Term>& poly1,
                                      const std::vector<Term>& poly2) {
    std::map<int, double> acc; // exponent -> accumulated coefficient

    for (const auto& t1 : poly1) {
        for (const auto& t2 : poly2) {
            int newExp = t1.exp + t2.exp;
            double newCoe = t1.coe * t2.coe;
            acc[newExp] += newCoe;
        }
    }

    std::vector<Term> result;
    const double threshold = 1e-6; // to avoid floating point noise

    for (const auto& [exp, coe] : acc) {
        // Round to one decimal place
        double rounded = std::round(coe * 10.0) / 10.0;
        if (std::abs(rounded) > threshold) {
            result.push_back({exp, rounded});
        }
    }

    return result;
}
// The main algorithm is a straightforward convolution: for every term in polynomial A and every term in polynomial B, create a product term with exponent = expA + expB and coefficient = coeA * coeB. Use a dictionary (e.g., `std::map<int, double>`) to accumulate coefficients for each exponent, since map automatically sorts keys. After accumulating all products, iterate through the map and filter out terms whose absolute coefficient is less than 0.005 (since the expected output uses one decimal place, but we want to avoid zero coefficients; the threshold should be small, e.g., 1e-6 to avoid floating errors, but the original code used 1e-2 – we can use 1e-6 for correctness). Then round each coefficient to one decimal place using `round(coeff * 10) / 10` and include if the rounded value is not zero. The result is a vector of terms sorted by exponent ascending. Time complexity is O(m*n + k log k) where m,n are input sizes and k is the number of distinct exponent sums, but since map insertion is O(log k), the total is O(m*n log(m*n)). Space is O(m*n) in the worst case for the map. Edge cases: if either input is empty, return an empty vector; if all terms cancel out, return an empty vector; handle negative coefficients and exponents.
