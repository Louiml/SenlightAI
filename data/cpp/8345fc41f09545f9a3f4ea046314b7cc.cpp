// Create a C++ standalone function that takes two `Distance` objects, each storing a non-negative floating-point distance internally, and returns an integer: `1` if the first distance is strictly greater than the second, `0` if they are equal or the first is smaller. The function must be a friend of the `Distance` class so it can access the private `m` member directly. The `Distance` class should have a private `float m` data member and a public setter `getdata(float)` to initialize it. The function should be named `compareDistance` and must enforce `const` correctness on its parameters (take `const Distance&` arguments). The comparison must handle floating-point precision appropriately—since distances are non-negative and set via a setter, a simple `>` comparison is acceptable without epsilon tolerance, but your function should still use `const` references to avoid copying.
The main task is to implement a friend function that compares two `Distance` objects. The class design includes a `private float m` member and a `public getdata` setter. The friend function needs `const` correctness: parameters should be `const Distance&` to prevent modification and avoid unnecessary copies. The algorithm is trivial: extract `d1.m` and `d2.m` (accessible because the function is a friend) and perform `d1.m > d2.m`, returning `1` if true, else `0`. Edge cases: equal distances should return `0`, and any negative value should not occur, but if it did, the function still behaves correctly since `>` handles signed floats. Since the comparison is a single operation, time complexity is O(1), and space complexity is O(1) — no extra data structures are needed. No floating-point equality is used, so no epsilon issues arise for the `>` operator. The function is standalone and does not need a `main`.
#include <iostream>

class Distance {
private:
    float m;
public:
    void getdata(float x) { m = x; }
    friend int compareDistance(const Distance& d1, const Distance& d2);
};

// Compare two Distance objects: return 1 if d1 > d2, else 0.
int compareDistance(const Distance& d1, const Distance& d2) {
    return d1.m > d2.m ? 1 : 0;
}
#include <cassert>

int main() {
    Distance a, b, c;
    a.getdata(10.5f);
    b.getdata(20.0f);
    c.getdata(10.5f);

    // a (10.5) is not greater than b (20.0)
    assert(compareDistance(a, b) == 0);
    // b (20.0) is greater than a (10.5)
    assert(compareDistance(b, a) == 1);
    // Equal distances should return 0 (not greater)
    assert(compareDistance(a, c) == 0);
    // Zero case: both zero -> not greater
    Distance d, e;
    d.getdata(0.0f);
    e.getdata(0.0f);
    assert(compareDistance(d, e) == 0);
    // Large vs small
    Distance f, g;
    f.getdata(999.99f);
    g.getdata(0.01f);
    assert(compareDistance(f, g) == 1);
    assert(compareDistance(g, f) == 0);

    return 0;
}
