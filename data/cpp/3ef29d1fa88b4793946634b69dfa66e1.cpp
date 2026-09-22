/*
Write a C++ function that takes two `Complex` numbers (represented by a class with integer real and imaginary parts) and returns a new `Complex` object representing their sum. The class must have a default constructor, a parameterized constructor `Complex(int rl, int im)`, and a `const` member function `getdata()` that prints the sum in the format `"the sum is"` followed by a newline and `"real+imaginary"` (without spaces). The addition operator must be implemented as a non-member friend function. The function should work correctly for positive, negative, and zero real/imaginary components, and must not modify the input objects.
*/
#include <iostream>

class Complex {
private:
    int real;
    int imaginary;

public:
    // Default constructor initializes to zero
    Complex() : real(0), imaginary(0) {}

    // Parameterized constructor
    Complex(int rl, int im) : real(rl), imaginary(im) {}

    // Const member function to print the sum
    void getdata() const {
        std::cout << "the sum is" << std::endl << real << "+" << imaginary;
    }

    // Friend function declaration
    friend Complex operator+(const Complex& c1, const Complex& c2);
};

// Friend function definition (non-member)
Complex operator+(const Complex& c1, const Complex& c2) {
    Complex temp;
    temp.real = c1.real + c2.real;
    temp.imaginary = c1.imaginary + c2.imaginary;
    return temp;
}
#include <cassert>
#include <sstream>

// Redirect cout to capture output for testing
void test_getdata(const Complex& c, const std::string& expected) {
    std::ostringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());
    c.getdata();
    std::cout.rdbuf(old);
    assert(buffer.str() == expected);
}

int main() {
    // Test addition with positive values
    Complex c1(10, 20);
    Complex c2(30, 40);
    Complex sum1 = c1 + c2;
    test_getdata(sum1, "the sum is\n40+60");

    // Test with negative components
    Complex c3(-5, -7);
    Complex c4(2, 3);
    Complex sum2 = c3 + c4;
    test_getdata(sum2, "the sum is\n-3+-4");

    // Test with zeros
    Complex c5(0, 0);
    Complex c6(0, 0);
    Complex sum3 = c5 + c6;
    test_getdata(sum3, "the sum is\n0+0");

    // Test mixed signs
    Complex c7(-10, 15);
    Complex c8(20, -25);
    Complex sum4 = c7 + c8;
    test_getdata(sum4, "the sum is\n10+-10");

    // Test that original objects are unchanged
    assert(c1.getdata() == nullptr); // Placeholder, but we actually check via output capture
    // Direct check of members is not possible (private), so we verify via getdata
    test_getdata(c1, "the sum is\n10+20");
    test_getdata(c2, "the sum is\n30+40");

    return 0;
}
// The solution involves defining a `Complex` class with private integer members `real` and `imaginary`. The default constructor initializes them to zero (to ensure safe use when creating temporary objects), and the parameterized constructor assigns the given values. The `getdata()` method is marked `const` because it only reads the object's state. The friend function `operator+(const Complex& c1, const Complex& c2)` creates a temporary `Complex` object, sets its members to the sum of the corresponding members of `c1` and `c2`, and returns it by value. Taking constant references as parameters avoids unnecessary copying and guarantees the originals are not modified. Edge cases include negative components (e.g., `-3 + -5` prints as `-3+-5`), zeros, and the addition of two objects with mixed signs. Time complexity is O(1) since only two additions and a return are performed; space complexity is O(1) for the temporary object.
