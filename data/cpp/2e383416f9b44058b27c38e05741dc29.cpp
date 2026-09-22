// Create a C++ function that, given two type parameters `T` and `U`, returns a `bool` indicating whether `T` is exactly the same type as `U` (i.e., no implicit conversions, no references decayed, no top-level cv-qualifiers removed — the types must match exactly). The function should work for fundamental types, pointers, references, and `void`. This must be implemented using template metaprogramming, specifically by leveraging partial specialization and a helper trait to compare types exactly. The function should be named `typesAreIdentical` and should be usable at compile time (i.e., `static_assert`-friendly). Provide a standalone implementation without any external libraries like `<type_traits>`.

We need a compile-time exact type comparison. C++’s own `std::is_same` does exactly this, but we must reimplement it. The core idea is a primary template `struct areSame` that defaults to `false`, and a partial specialization for when both type parameters are the same, which sets it to `true`. The specialization must use the same template parameter name twice in the specialization’s parameter list (e.g., `template<typename T> struct areSame<T,T>`). This makes the compiler match only when the two types are identical, including all qualifiers and referenceness. For example, `areSame<int, const int>` does not match the specialization because `int` and `const int` are different types; similarly `areSame<int&, int>` does not match. The `typesAreIdentical` function is simply a `constexpr` function returning `areSame<T,U>::value`. Edge cases: `void` vs `void` matches, `int` vs `int&` does not, `const int` vs `int` does not, `int*` vs `int` does not, and `int* const` vs `int*` does not (top-level cv-qualifiers are part of the type). Time complexity is O(1) at compile time, and space complexity is O(1) — no runtime cost at all.

// Primary template: false for any two types by default.
template<typename T, typename U>
struct areSame {
    static const bool value = false;
};

// Partial specialization: true when the two types are exactly the same.
template<typename T>
struct areSame<T, T> {
    static const bool value = true;
};

// Compile-time function to compare types exactly.
template<typename T, typename U>
constexpr bool typesAreIdentical() {
    return areSame<T, U>::value;
}

#include <cassert>

int main() {
    // Fundamental types
    assert(typesAreIdentical<int, int>());
    assert(!typesAreIdentical<int, double>());
    assert(typesAreIdentical<void, void>());
    assert(!typesAreIdentical<void, int>());

    // Qualifiers matter
    assert(!typesAreIdentical<int, const int>());
    assert(!typesAreIdentical<const int, int>());
    assert(typesAreIdentical<const int, const int>());

    // References matter
    assert(!typesAreIdentical<int&, int>());
    assert(!typesAreIdentical<int, int&>());
    assert(typesAreIdentical<int&, int&>());

    // Pointers matter
    assert(!typesAreIdentical<int*, int>());
    assert(!typesAreIdentical<int*, int**>());
    assert(typesAreIdentical<int*, int*>());

    // Top-level const on a pointer differs? Yes, it's part of type.
    assert(!typesAreIdentical<int*, int* const>());
    assert(typesAreIdentical<int* const, int* const>());

    return 0;
}
