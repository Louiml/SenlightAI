/*
Write a C++ function named `translatePoint` that takes two `Point` objects (where `Point` is a class with integer `x` and `y` coordinates, a default constructor that sets both to 0, a parameterized constructor, and a public `show()` method that prints the coordinates to stdout) and returns a new `Point` representing the component-wise difference between the first point and the second point (i.e., `result.x = p1.x - p2.x`, `result.y = p1.y - p2.y`). The function must be declared with `const` reference parameters to avoid unnecessary copying, and it must not modify the input points. You are not allowed to overload any operators; instead, use a free function with the exact signature `Point translatePoint(const Point& p1, const Point& p2)`. The function must work correctly for any integer coordinates, including negative values, zeros, and identical points. You may assume the `Point` class is already defined as follows (do not redefine it, but your solution will be tested with this definition):

```cpp
class Point {
private:
    int x_cor;
    int y_cor;
public:
    Point(int x = 0, int y = 0) : x_cor(x), y_cor(y) {}
    void show() const {
        std::cout << "x: " << x_cor << ", y: " << y_cor << std::endl;
    }
    // Add a public getter method for testing purposes:
    int getX() const { return x_cor; }
    int getY() const { return y_cor; }
};
```

Note: Your `translatePoint` function must be able to access the private members of `Point` (since it is a free function, you will need to either use the provided getters `getX()` and `getY()`, or you may declare the function as a friend inside the `Point` class – but since you cannot modify the class, use the getters). Include all necessary headers and write your function so it can be compiled and tested standalone.
*/

#include <iostream>  // for cout (used by Point::show, not required here but included for completeness)

// Assume Point class is defined as given in the task (with getX() and getY()).

// Return a Point representing the component-wise difference p1 - p2.
Point translatePoint(const Point& p1, const Point& p2) {
    return Point(p1.getX() - p2.getX(), p1.getY() - p2.getY());
}

#include <cassert>
#include <iostream>

// Assume Point class definition and translatePoint function are included above.

int main() {
    // Test 1: Basic subtraction
    Point p1(11, 5), p2(9, 7);
    Point r1 = translatePoint(p1, p2);
    assert(r1.getX() == 2 && r1.getY() == -2);

    // Test 2: Identical points
    Point p3(5, 80), p4(5, 80);
    Point r2 = translatePoint(p3, p4);
    assert(r2.getX() == 0 && r2.getY() == 0);

    // Test 3: Negative differences
    Point p5(1, -3), p6(-4, 2);
    Point r3 = translatePoint(p5, p6);
    assert(r3.getX() == 5 && r3.getY() == -5);

    // Test 4: Zero coordinates
    Point p7(0, 0), p8(0, 0);
    Point r4 = translatePoint(p7, p8);
    assert(r4.getX() == 0 && r4.getY() == 0);

    // Test 5: Larger values
    Point p9(1000, -1000), p10(1, 1);
    Point r5 = translatePoint(p9, p10);
    assert(r5.getX() == 999 && r5.getY() == -1001);

    // Test 6: Verify input points are not modified (const correctness)
    assert(p1.getX() == 11 && p1.getY() == 5);
    assert(p2.getX() == 9 && p2.getY() == 7);

    std::cout << "All tests passed!" << std::endl;
    return 0;
}

// The solution is straightforward: create a new `Point` object whose `x` coordinate is the difference of the two input `x` coordinates (p1.getX() - p2.getX()) and whose `y` coordinate is the difference of the two input `y` coordinates (p1.getY() - p2.getY()). Since the parameters are `const` references, we only read from them and never mutate them. The function uses the public getters `getX()` and `getY()` to access the private data members. Edge cases include identical points (the result is (0,0)), negative coordinate differences (which are handled naturally by integer subtraction), and zeros. The time complexity is O(1) since only a few arithmetic operations are performed, and space complexity is O(1) because only one new `Point` object is created (which is returned by value, so no dynamic allocation). No error handling is needed because all inputs are valid `Point` objects.
