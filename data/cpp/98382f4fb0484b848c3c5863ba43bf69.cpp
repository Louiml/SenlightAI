// Write a C++ function named `analyzeToys` that takes a dynamic array of pointers to the abstract base class `Igrachka`, the number of toys in the array, and a pointer to one additional toy (also of type `Igrachka`), and returns a `float` value representing the absolute difference between the total mass of all toys in the array and the mass of the additional toy. In the given problem context, each toy is either a `Topka` (sphere) or a `Kocka` (box/cube), both inherit from `Igrachka` and `Forma`. The `Igrachka` interface requires methods `getVolumen()`, `getMasa()`, and `getPlostina()`. The `Forma` base class stores a color (as `char` array) and a density (integer). The function must work with any array of concrete toy objects and the additional toy, and it must compute the difference correctly regardless of toy type. The comparison is: if the total mass of the array exceeds the mass of the additional toy, it returns the positive difference; otherwise, it returns the negative difference (i.e., just the arithmetic difference `totalMass - extraMass`). Assume the input is always valid (non-empty array, correct types, positive dimensions and density). For clarity, the function should not modify any of the objects.
#include <cassert>
#include <cstring>

// The concrete classes (must be defined before main, but for test we include them here)
// Since the task expects the function to work with the given classes, we provide minimal definitions.
class Forma {
protected:
    char boja[100];
    int gustina;
public:
    Forma(const char *boja = "", int gustina = 0) {
        if (boja) {
            std::strcpy(this->boja, boja);
        } else {
            this->boja[0] = '\0';
        }
        this->gustina = gustina;
    }
    virtual ~Forma() {}
};

class Topka : public Igrachka, public Forma {
private:
    int radius;
public:
    Topka(const char *boja = "", int gustina = 0, int radius = 0)
        : Forma(boja, gustina), radius(radius) {}
    float getVolumen() override {
        return 4.0f / 3 * 3.14f * radius * radius * radius;
    }
    float getMasa() override {
        return getVolumen() * gustina;
    }
    float getPlostina() override {
        return 4.0f * 3.14f * radius * radius;
    }
};

class Kocka : public Igrachka, public Forma {
private:
    int height, width, depth;
public:
    Kocka(const char *boja = "", int gustina = 0, int h = 0, int w = 0, int d = 0)
        : Forma(boja, gustina), height(h), width(w), depth(d) {}
    float getVolumen() override {
        return static_cast<float>(height) * width * depth;
    }
    float getMasa() override {
        return getVolumen() * gustina;
    }
    float getPlostina() override {
        if (height == width && width == depth) {
            return 6.0f * height * height;
        } else {
            return 2.0f * (height * depth + height * width + width * depth);
        }
    }
};

int main() {
    // Test 1: Two spheres, one cube. Total mass of array vs extra mass.
    Igrachka *toys[2];
    toys[0] = new Topka("red", 10, 3);   // volume = 4/3*3.14*27 ≈ 113.04, mass = 1130.4
    toys[1] = new Kocka("blue", 5, 2, 3, 4); // volume = 24, mass = 120
    Igrachka *extra = new Topka("green", 2, 5); // volume ≈ 523.33, mass ≈ 1046.67
    // Total mass = 1130.4 + 120 = 1250.4, extra mass ≈ 1046.67, difference ≈ 203.73
    float result = analyzeToys(toys, 2, extra);
    assert(result > 203.0f && result < 204.0f);

    // Test 2: Single toy with equal mass as extra (should be zero)
    Igrachka *one[1];
    one[0] = new Topka("yellow", 3, 2); // volume ≈ 33.49, mass = 100.48
    Igrachka *extra2 = new Topka("black", 3, 2); // same mass
    assert(std::abs(analyzeToys(one, 1, extra2)) < 0.001f);

    // Test 3: Negative result (extra heavier than total)
    Igrachka *light[1];
    light[0] = new Topka("white", 1, 1); // mass ≈ 4.19
    Igrachka *heavy = new Kocka("dark", 100, 10, 10, 10); // mass = 100000
    float diff = analyzeToys(light, 1, heavy);
    assert(diff < -99990.0f && diff > -100000.0f);

    // Clean up
    for (int i = 0; i < 2; ++i) delete toys[i];
    delete extra;
    delete one[0];
    delete extra2;
    delete light[0];
    delete heavy;

    return 0;
}
#include <cmath> // for fabs if needed, but we return float difference directly

// Abstract base classes (must be defined before use, but in the task they are given)
class Igrachka {
public:
    virtual float getVolumen() = 0;
    virtual float getMasa() = 0;
    virtual float getPlostina() = 0;
    virtual ~Igrachka() {}
};

// Note: The function does not use any member variables of the concrete classes directly.
// It only relies on the public virtual interface.

// Compute the difference between total mass of all toys in the array and the mass of an extra toy.
// Returns (totalMass - extraMass) which may be negative if extra is heavier.
float analyzeToys(Igrachka **toys, int n, Igrachka *extraToy) {
    if (toys == nullptr || n <= 0 || extraToy == nullptr) {
        return 0.0f; // defensive, but input is guaranteed valid
    }

    float totalMass = 0.0f;
    for (int i = 0; i < n; ++i) {
        totalMass += toys[i]->getMasa();
    }

    float extraMass = extraToy->getMasa();

    return totalMass - extraMass;
}
// The solution requires iterating over all toys in the array and summing their masses by calling `getMasa()` on each polymorphic pointer. Then compute the mass of the additional toy similarly. The result is simply `totalMass - extraMass` (which may be negative if the extra toy is heavier). The main challenge is ensuring the abstract interface is correctly used; we do not need to know the concrete types during the summing loop because the virtual methods dispatch correctly. Edge cases: if the array contains only one toy, the summation still works; if the extra toy has the same mass as the total, the result is zero. The time complexity is O(n) because we iterate over the array once, and each call to `getMasa()` is O(1) (it calls `getVolumen()` which computes a formula using constant-time operations). Space complexity is O(1) beyond the input pointers, as we only use a few local variables.
