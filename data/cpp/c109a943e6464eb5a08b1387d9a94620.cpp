// Write a C++ function named `countShapeTypes` that takes a `std::vector<figur*>` containing pointers to dynamically allocated objects of the polymorphic hierarchy `figur`, `kreis`, `viereck`, and `dreieck` (as defined below), and returns a `std::tuple<int,int,int>` representing the number of `kreis`, `viereck`, and `dreieck` instances, respectively. The function must not modify the vector, must not delete the pointers, and must correctly count objects using `dynamic_cast` (not `std::type_info`). Assume the vector may contain null pointers, which should be ignored entirely. The result tuple should contain counts in the order (kreis, viereck, dreieck). Provide the class definitions exactly as in the snippet, but renamed to English equivalents (e.g., `shape`, `circle`, `square`, `triangle`).

#include <cassert>
#include <vector>

int main() {
    // Test empty vector
    std::vector<shape*> v1;
    auto result1 = countShapeTypes(v1);
    assert(result1 == std::make_tuple(0, 0, 0));

    // Test only circles
    std::vector<shape*> v2;
    v2.push_back(new circle());
    v2.push_back(new circle());
    auto result2 = countShapeTypes(v2);
    assert(result2 == std::make_tuple(2, 0, 0));
    for (shape* p : v2) delete p;

    // Test mixed types with null pointers
    std::vector<shape*> v3;
    v3.push_back(new square());
    v3.push_back(nullptr);
    v3.push_back(new triangle());
    v3.push_back(new circle());
    v3.push_back(nullptr);
    v3.push_back(new square());
    auto result3 = countShapeTypes(v3);
    assert(result3 == std::make_tuple(1, 2, 1));
    for (shape* p : v3) delete p;

    // Test all null pointers
    std::vector<shape*> v4 = {nullptr, nullptr, nullptr};
    auto result4 = countShapeTypes(v4);
    assert(result4 == std::make_tuple(0, 0, 0));

    // Test single triangle
    std::vector<shape*> v5;
    v5.push_back(new triangle());
    auto result5 = countShapeTypes(v5);
    assert(result5 == std::make_tuple(0, 0, 1));
    delete v5[0];

    return 0;
}

#include <tuple>
#include <vector>

// Base class
class shape {
public:
    virtual ~shape() {}
};

// Derived classes
class circle : public shape {};
class square : public shape {};
class triangle : public shape {};

// Count the number of each derived type in the vector. Ignores null pointers.
// Returns a tuple of (circle_count, square_count, triangle_count).
std::tuple<int, int, int> countShapeTypes(const std::vector<shape*>& shapes) {
    int circles = 0;
    int squares = 0;
    int triangles = 0;
    
    for (const shape* s : shapes) {
        if (s == nullptr) continue;  // Ignore null pointers
        if (dynamic_cast<const circle*>(s) != nullptr) {
            ++circles;
        } else if (dynamic_cast<const square*>(s) != nullptr) {
            ++squares;
        } else if (dynamic_cast<const triangle*>(s) != nullptr) {
            ++triangles;
        }
    }
    
    return std::make_tuple(circles, squares, triangles);
}

// The solution iterates through the vector once. For each non-null pointer, we use `dynamic_cast` to test if it points to a `circle`, `square`, or `triangle`. Since the classes form a simple hierarchy without virtual functions other than the destructor, `dynamic_cast` will succeed only for the exact derived type (and also for base class pointers, but we have only derived instances). Null pointers are skipped to avoid undefined behavior when dereferencing. The three counters are initialized to zero. Edge cases include an empty vector, a vector with only null pointers, and vectors with all three types. Time complexity is O(n) for n elements, each requiring constant-time checks. Space complexity is O(1) auxiliary, not counting the input vector or the returned tuple.
