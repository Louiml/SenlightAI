/*
Write a C++ function named `averageOfTwo` that takes a constant reference to a class `Sample` (defined as shown below) and returns the arithmetic mean of its two private integer members as a `double`. The class must have a public method `setValues(int x, int y)` to assign values to the private members, and the function must be a friend of the class so that it can access those private members. The function should compute the mean as `(a + b) / 2.0` and return it. Your task is to provide only the free function (no `main`), with proper `const` correctness (the parameter should be `const Sample&`), and include the class definition, any necessary headers, and comments.
*/

#include <iostream>

class Sample {
private:
    int a;
    int b;
public:
    void setValues(int x, int y) {
        a = x;
        b = y;
    }
    friend double averageOfTwo(const Sample& s);
};

// Compute the arithmetic mean of the two private integers in a Sample object.
// Uses friend access to directly retrieve the members. The parameter is const-ref
// to avoid copying and to guarantee no modification. Division by 2.0 ensures
// floating-point result.
double averageOfTwo(const Sample& s) {
    return (static_cast<double>(s.a) + s.b) / 2.0;
}

#include <cassert>

int main() {
    Sample s1;
    s1.setValues(25, 30);
    assert(averageOfTwo(s1) == 27.5);

    Sample s2;
    s2.setValues(-10, 20);
    assert(averageOfTwo(s2) == 5.0);

    Sample s3;
    s3.setValues(0, 0);
    assert(averageOfTwo(s3) == 0.0);

    Sample s4;
    s4.setValues(1, 2);
    assert(averageOfTwo(s4) == 1.5);

    Sample s5;
    s5.setValues(-5, -7);
    assert(averageOfTwo(s5) == -6.0);

    Sample s6;
    s6.setValues(1000000, 2000000);
    assert(averageOfTwo(s6) == 1500000.0);

    Sample s7;
    s7.setValues(2147483647, 2147483647);
    assert(averageOfTwo(s7) == 2147483647.0);

    Sample s8;
    s8.setValues(-2147483647, 2147483647);
    assert(averageOfTwo(s8) == 0.0);

    return 0;
}

// The main challenge is accessing private members of the class from a non-member function. This is solved by declaring the function as a `friend` inside the class, granting it access to the private integers. The function receives a constant reference to the object, which avoids copying and guarantees that the object is not modified. The algorithm is trivial: read `a` and `b` from the passed object (via the friend access), cast or compute their sum, then divide by `2.0` to force floating‑point division and preserve the fractional part. Edge cases include extreme integer values (e.g., `INT_MAX` and `INT_MIN`) where integer overflow could occur if we summed first—but here `2.0` multiplication avoids that; however, to be safe, we could cast one operand to `double` before adding to avoid any potential intermediate overflow (though in this simple case, using `2.0` already ensures double arithmetic). There are no other edge cases since the class always has both members set. Time complexity is O(1) and space complexity is O(1) as only constant memory is used.
