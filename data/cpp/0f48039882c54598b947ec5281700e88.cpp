/*
Write a C++ function that implements a class `Multiplier` which uses constructor overloading to compute the product of either two or three integers. The class must have two constructors: one taking two `int` parameters and another taking three `int` parameters. Each constructor should store the resulting product in a private member variable `product_`. Additionally, provide a public `const` member function `getProduct()` that returns the stored product. The free function `createMultiplier` (or a similar descriptive name) must be provided that takes an `std::vector<int>` (non-empty, size 2 or 3) and returns the corresponding `Multiplier` object, choosing the appropriate constructor based on the vector's size. The solution must handle only these two valid sizes; if the vector has any other size, the function should throw `std::invalid_argument`. The function and class must be self-contained (include necessary headers) and must not include a `main` function. Ensure member functions are `const` where appropriate.
*/
#include <vector>
#include <stdexcept>

class Multiplier {
private:
    int product_;

public:
    Multiplier(int a, int b) : product_(a * b) {}
    Multiplier(int a, int b, int c) : product_(a * b * c) {}

    int getProduct() const {
        return product_;
    }
};

// Create a Multiplier from a vector of 2 or 3 integers.
Multiplier createMultiplier(const std::vector<int>& values) {
    if (values.size() == 2) {
        return Multiplier(values[0], values[1]);
    } else if (values.size() == 3) {
        return Multiplier(values[0], values[1], values[2]);
    } else {
        throw std::invalid_argument("Multiplier requires exactly 2 or 3 integers");
    }
}
#include <cassert>

int main() {
    // Two numbers
    Multiplier m1 = createMultiplier({2, 5});
    assert(m1.getProduct() == 10);

    // Three numbers
    Multiplier m2 = createMultiplier({2, 5, 6});
    assert(m2.getProduct() == 60);

    // Zero values
    assert(createMultiplier({0, 7}).getProduct() == 0);
    assert(createMultiplier({1, 0, 9}).getProduct() == 0);

    // Negative numbers
    assert(createMultiplier({-3, 4}).getProduct() == -12);
    assert(createMultiplier({-2, -3, 5}).getProduct() == 30);

    // One negative and zero
    assert(createMultiplier({-1, 0, 3}).getProduct() == 0);

    // Large values (int overflow is not handled, but test within range)
    assert(createMultiplier({100, 200}).getProduct() == 20000);
    assert(createMultiplier({10, 10, 10}).getProduct() == 1000);

    // Invalid size throws
    bool threw = false;
    try {
        createMultiplier({1});
    } catch (const std::invalid_argument&) {
        threw = true;
    }
    assert(threw);

    threw = false;
    try {
        createMultiplier({1, 2, 3, 4});
    } catch (const std::invalid_argument&) {
        threw = true;
    }
    assert(threw);
}
// The solution involves two main steps: (1) defining a class with two overloaded constructors, each computing and storing the product; (2) writing a free function that inspects the size of an input vector and constructs the correct object. The class stores only the product (an `int`), not the factors, so memory usage is constant regardless of constructor. Edge cases include zero values (product becomes zero, which is handled naturally) and negative numbers (product sign is correct via standard integer multiplication). The function must validate the vector size — if it is not 2 or 3, it throws an exception to indicate invalid input. Since the vector's size is at most 3, the time complexity is O(1), as only a fixed number of multiplications occur. Space complexity is O(1) for the object and vector storage (ignoring the vector itself, which is given). The constructors are not `const` (they modify the object), but the getter `getProduct()` is marked `const` because it only reads the member. The reference solution uses `#include <vector>`, `#include <stdexcept>`, and `#include <cstddef>` (or similar) to support the required functionality.
