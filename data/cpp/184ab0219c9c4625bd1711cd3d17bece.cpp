Write a C++ function that determines whether a given type is `void`, ignoring any cv-qualifiers (i.e., `const void`, `volatile void`, and `const volatile void` should all be treated as `void`). The function must be implemented using template metaprogramming with a primary template and partial/explicit specializations. It should return a compile-time boolean value accessible via `::value` (similar to `std::is_void`). Additionally, the function must handle the case where the type is a reference to `void` (e.g., `void&`), and for such cases it should return `false` (since a reference to `void` is ill-formed in standard C++, but you should still account for it in the specialization logic without causing compilation errors). The task is to produce a standalone header-only implementation that works for any type `T`, including built-in types, user-defined types, pointers, and arrays.

// The core idea is to use a primary template that inherits from `std::false_type` and then provide explicit specializations for `void` and cv-qualified versions of `void` (e.g., `const void`, `volatile void`, `const volatile void`). Since the task also requires handling references to `void`, we need to add a partial specialization that matches `void&` and cv-qualified references to `void` (e.g., `const void&`) and makes them inherit from `std::false_type`. 
//
// The simplest way to handle cv-qualifiers without writing many specializations is to use `std::remove_cv` (or a custom trait) to strip top-level cv-qualifiers before checking. However, since the task explicitly mentions specializations, we can provide a primary template that strips cv-qualifiers and then delegates to a helper that checks if the stripped type is `void`. This helper can use a partial specialization for `void` (after removal) to inherit from `std::true_type`. For references, `std::remove_cv` does not remove references, so we need to also remove references or handle them separately. The cleanest approach is to use `std::remove_cv` and `std::remove_reference` in combination: `typename std::remove_cv<typename std::remove_reference<T>::type>::type`. Then the primary template inherits from a helper that checks equality with `void`. 
//
// Time complexity is constant (compile-time), and space complexity is zero (no runtime storage). Edge cases: `void` itself, cv-qualified `void`, references to `void` (which should be false), pointer to `void` (false), arrays (false), and user-defined types (false).

#include <type_traits>

namespace detail {
    // Helper primary template: defaults to false for any type.
    template<typename T>
    struct is_void_impl : std::false_type {};

    // Specialization for void (after stripping cv and reference).
    template<>
    struct is_void_impl<void> : std::true_type {};
}

// Public trait: strips cv-qualifiers and references, then delegates.
template<typename T>
struct is_void : detail::is_void_impl<
    typename std::remove_cv<
        typename std::remove_reference<T>::type
    >::type
> {};

#include <cassert>

int main() {
    // Basic cases
    static_assert(is_void<void>::value == true, "void should be true");
    static_assert(is_void<int>::value == false, "int should be false");
    static_assert(is_void<char>::value == false, "char should be false");

    // cv-qualified void
    static_assert(is_void<const void>::value == true, "const void should be true");
    static_assert(is_void<volatile void>::value == true, "volatile void should be true");
    static_assert(is_void<const volatile void>::value == true, "cv void should be true");

    // References to void (should be false because reference-to-void is invalid)
    static_assert(is_void<void&>::value == false, "void& should be false");
    static_assert(is_void<const void&>::value == false, "const void& should be false");

    // Pointers, arrays, user-defined types
    static_assert(is_void<void*>::value == false, "void* should be false");
    static_assert(is_void<int[3]>::value == false, "array should be false");
    struct Foo {};
    static_assert(is_void<Foo>::value == false, "struct should be false");

    // Runtime checks (though compile-time is the point)
    assert(is_void<void>::value == true);
    assert(is_void<int>::value == false);
    assert(is_void<const void>::value == true);
    assert(is_void<void&>::value == false);

    return 0;
}
