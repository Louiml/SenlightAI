/*
Create a C++ class called `Term` that represents a single algebraic term of the form `coefficient * x^exponent`, where the coefficient is a `double` and the exponent is an `int`. Your class must have a parameterized constructor, setter and getter methods for both fields, and a function-call operator `operator()` that evaluates the term at a given value of `x` using `std::pow`. Additionally, implement the following comparison operators that compare terms by exponent only (except `!=`, which compares both coefficient and exponent): `operator==`, `operator<`, `operator>`, and `operator!=`. Also implement a compound multiplication assignment `operator*=` that scales the coefficient by a given factor. Finally, provide both stream extraction operators (`operator>>` for `istream` and `ifstream`) that read a coefficient and an exponent from the stream, and a stream insertion operator (`operator<<`) that outputs the term in a readable human format, handling special cases where coefficient is 1 (omit the "1") and exponent is 1 (omit the "^1"), and when exponent is 0 (output just the coefficient). Your solution must be standalone, include all necessary headers, and the class must be defined entirely in one code block.
*/
#include <iostream>
#include <fstream>
#include <cmath>

class Term {
private:
    double coefficient;
    int exponent;

public:
    // Constructor
    Term(double coeff = 0.0, int expn = 0) {
        setCoefficient(coeff);
        setExponent(expn);
    }

    // Setters
    void setCoefficient(double newCoeff) { coefficient = newCoeff; }
    void setExponent(int newExpn) { exponent = newExpn; }

    // Getters
    double getCoefficient() const { return coefficient; }
    int getExponent() const { return exponent; }

    // Evaluate term at x
    double operator()(double x) const {
        return coefficient * std::pow(x, exponent);
    }

    // Scale coefficient
    Term& operator*=(double factor) {
        coefficient *= factor;
        return *this;
    }

    // Comparison operators (by exponent, except !=)
    bool operator==(const Term& other) const {
        return exponent == other.exponent;
    }

    bool operator<(const Term& other) const {
        return exponent < other.exponent;
    }

    bool operator>(const Term& other) const {
        return exponent > other.exponent;
    }

    bool operator!=(const Term& other) const {
        return coefficient != other.coefficient && exponent != other.exponent;
    }

    // Stream extraction (istream)
    friend std::istream& operator>>(std::istream& input, Term& t) {
        double coeff;
        int expn;
        input >> coeff >> expn;
        t.setCoefficient(coeff);
        t.setExponent(expn);
        return input;
    }

    // Stream extraction (ifstream)
    friend std::ifstream& operator>>(std::ifstream& inFile, Term& t) {
        double coeff;
        int expn;
        inFile >> coeff >> expn;
        t.setCoefficient(coeff);
        t.setExponent(expn);
        return inFile;
    }

    // Stream insertion
    friend std::ostream& operator<<(std::ostream& out, const Term& t) {
        double c = t.getCoefficient();
        int e = t.getExponent();

        if (c == 1 && e == 1) {
            out << "x";
        } else if (c == 1) {
            out << "x^" << e;
        } else if (e == 1) {
            out << c << "x";
        } else if (e == 0) {
            out << c;
        } else {
            out << c << "x^" << e;
        }
        return out;
    }
};
#include <cassert>
#include <sstream>

int main() {
    // Constructor and getters
    Term t1(2.5, 3);
    assert(t1.getCoefficient() == 2.5);
    assert(t1.getExponent() == 3);

    // Evaluation
    assert(t1(2.0) == 2.5 * 8.0);  // 2.5 * 2^3 = 20.0
    assert(t1(0.0) == 0.0);        // 2.5 * 0^3 = 0

    Term t2(1.0, 0);
    assert(t2(5.0) == 1.0);        // 1 * 5^0 = 1

    // Multiplication assignment
    t1 *= 2.0;
    assert(t1.getCoefficient() == 5.0);

    // Comparisons (by exponent)
    Term t3(3.0, 2);
    Term t4(7.0, 3);
    assert(t3 == t3);
    assert(!(t3 == t4));
    assert(t3 < t4);
    assert(t4 > t3);
    assert(t3 != t4);
    assert(!(t3 != t3));  // same term is equal

    // Stream extraction (istream)
    std::istringstream iss("4.5 2");
    Term t5;
    iss >> t5;
    assert(t5.getCoefficient() == 4.5);
    assert(t5.getExponent() == 2);

    // Stream extraction (ifstream) - simulate with istringstream via a file-like stream
    // We can't use ifstream easily in unit test, but the function is identical in logic.
    // We'll test the insertion operator instead.

    // Stream insertion
    std::ostringstream oss;
    oss << t1;      // t1 has coefficient 5.0, exponent 3
    assert(oss.str() == "5x^3");

    oss.str("");
    Term t6(1.0, 1);
    oss << t6;
    assert(oss.str() == "x");

    oss.str("");
    Term t7(1.0, 4);
    oss << t7;
    assert(oss.str() == "x^4");

    oss.str("");
    Term t8(3.5, 1);
    oss << t8;
    assert(oss.str() == "3.5x");

    oss.str("");
    Term t9(2.0, 0);
    oss << t9;
    assert(oss.str() == "2");

    // Default constructor
    Term t10;
    assert(t10.getCoefficient() == 0.0);
    assert(t10.getExponent() == 0);

    return 0;
}
// The solution requires defining a `Term` class with two private members (`double coefficient`, `int exponent`). The constructor delegates to the setters to ensure any future validation is centralized. The evaluation operator uses `std::pow` with the coefficient multiplied by the result. Comparison operators are straightforward: equality and ordering are based on the exponent, while inequality checks that both coefficient and exponent differ. The `operator*=` modifies the coefficient in place. For stream input, both `ifstream` and `istream` overloads are identical in logic—they read two values and assign them via setters, returning the stream to allow chaining. For output, the insertion operator uses conditional logic: if the coefficient is 1 and exponent is 1, print "x"; if coefficient is 1, print "x^exp"; if exponent is 1, print "coeffx"; if exponent is 0, print just the coefficient; otherwise print "coeffx^exp". The complexity is O(1) for all operations except `operator()` which is O(log exponent) due to `pow` typically using exponentiation by squaring, and O(1) for stream I/O in terms of the Term object itself (excluding underlying I/O). Space is O(1).
