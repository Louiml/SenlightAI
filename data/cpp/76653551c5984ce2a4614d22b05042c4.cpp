/*
Design a C++ function that takes a vector of `Shape*` pointers (where `Shape` is an abstract base class with a pure virtual `area()` method) and returns the sum of the areas of all shapes. The shapes are `Circle` (area = 3.14 * radius * radius) and `Square` (area = side * side). The function must accept a `const std::vector<Shape*>&` and compute the total area as a `double`. Handle the case of an empty vector (return 0.0) and ensure that each shape's area is computed using virtual dispatch correctly. The function should be named `sumAreas` and must not modify the shapes.
*/

#include <vector>
#include <cmath> // not strictly needed, but for potential future use

class Shape {
public:
    virtual ~Shape() = default;
    virtual double area() const = 0;
};

class Circle : public Shape {
private:
    double radius;
public:
    Circle(double r) : radius(r) {}
    double area() const override { return 3.14 * radius * radius; }
};

class Square : public Shape {
private:
    double side;
public:
    Square(double s) : side(s) {}
    double area() const override { return side * side; }
};

// Compute the total area of all shapes in the vector.
// Accepts a const reference to avoid copying and const-correctness.
// Assumes all pointers are non-null, but skips null pointers defensively.
double sumAreas(const std::vector<Shape*>& shapes) {
    double total = 0.0;
    for (const Shape* shape : shapes) {
        if (shape != nullptr) {
            total += shape->area();
        }
    }
    return total;
}

#include <cassert>
#include <vector>

int main() {
    std::vector<Shape*> shapes;

    // Empty vector
    assert(sumAreas(shapes) == 0.0);

    // Single square with side 5
    Square s1(5.0);
    shapes.push_back(&s1);
    assert(sumAreas(shapes) == 25.0);

    // Add a circle with radius 2
    Circle c1(2.0);
    shapes.push_back(&c1);
    // 25 + 3.14*4 = 25 + 12.56 = 37.56
    assert(sumAreas(shapes) == 37.56);

    // Add another square with side 3
    Square s2(3.0);
    shapes.push_back(&s2);
    // 37.56 + 9 = 46.56
    assert(sumAreas(shapes) == 46.56);

    // Add a null pointer (should be ignored)
    shapes.push_back(nullptr);
    assert(sumAreas(shapes) == 46.56);

    // Test const correctness: the function does not modify shapes
    const std::vector<Shape*>& const_shapes = shapes;
    assert(sumAreas(const_shapes) == 46.56);

    return 0;
}

// The solution involves iterating over the vector of pointers and summing the result of calling the virtual `area()` method on each shape. Since `Shape` is abstract, we cannot instantiate it, but we can hold pointers to derived objects. The algorithm is a simple linear scan: initialize `total` to 0.0, then for each pointer, add `shape->area()`. We must ensure the function is `const`-correct by declaring it to accept a `const` reference to the vector and by not modifying the shapes. For an empty vector, the loop does not execute, so `total` remains 0.0. Time complexity is O(n) where n is the number of shapes, and space complexity is O(1) beyond the input. Edge cases include a null pointer in the vector (though the task specification assumes valid pointers, a robust implementation could check for null and skip). The areas are computed using the derived class implementations, so no need for dynamic_cast. Use `double` for precision.
