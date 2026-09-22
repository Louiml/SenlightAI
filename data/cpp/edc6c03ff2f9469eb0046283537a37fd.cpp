// Write a C++ function `templateValueAsDefaultArgument` that demonstrates template metaprogramming with dependent default arguments. The function should accept a single integer parameter `N` and return a `std::pair<int, int>` where the first element is `N` itself and the second element is a compile-time constant `1`. The key challenge is to write a templated helper struct `DefaultHolder<T>` that has a static `const T value = 1;` member, then use it as a default template argument in another templated struct `Y<T, bool = DefaultHolder<T>::value>`, which should have a nested type alias `type` equal to `T`. Finally, your function should instantiate `Y<int>` through a templated struct `Z<T>` that uses `typename Y<T>::type`, and return `{N, static_cast<int>(Z<int>::type::value)}` — but note that `type` is just `int`, not a struct, so you'll need to adjust: actually, the function should simply return `{N, static_cast<int>(DefaultHolder<int>::value)}` after verifying via `static_assert` that `Z<int>::type` is indeed `int`. The function must compile without any warnings or errors, demonstrating that the dependent default argument is correctly resolved.
The solution leverages C++ template instantiation rules. The core is `DefaultHolder<T>` with a static const member `value = 1`. Then `Y<T, bool = DefaultHolder<T>::value>` uses that as a default template argument — this is valid because when `Y<T>` is instantiated without a second argument, the compiler deduces the default from `DefaultHolder<T>::value`. However, since `DefaultHolder<T>::value` is a non-type template parameter of type `bool`, and it’s a constant expression, it works. The `Z<T>` struct then does `typedef typename Y<T>::type A;` — but `Y<T>::type` is just `T`, so `Z<T>::A` is `T`. We can verify this with `static_assert(std::is_same<Z<int>::type, int>::value)`. The function returns a `std::pair<int,int>`: `{N, static_cast<int>(Z<int>::type(DefaultHolder<int>::value))}` — but since `Z<int>::type` is `int`, that just converts the bool constant (true) to `1`. Actually simpler: `return {N, static_cast<int>(DefaultHolder<int>::value)};` after asserting the type. Edge cases: the function must work for any integer `N`, positive or negative, and the second value should always be `1` (since `DefaultHolder<int>::value = 1`). Complexity: O(1) time and O(1) space; the template instantiation happens at compile time.
#include <type_traits>
#include <utility>

// Compile-time holder: static const T value = 1.
template<typename T>
struct DefaultHolder {
    static const T value = 1;
};

// Y uses DefaultHolder<T>::value as a default template argument.
template<typename T, bool = DefaultHolder<T>::value>
struct Y {
    using type = T;
};

// Z exposes Y<T>::type as its own type alias.
template<typename T>
struct Z {
    using type = typename Y<T>::type;
};

// Function returning a pair where first is the input, second is the constant 1.
// Uses compile-time verification that Z<int>::type is indeed int.
std::pair<int, int> templateValueAsDefaultArgument(int N) {
    static_assert(std::is_same<Z<int>::type, int>::value,
                  "Z<int>::type must be int");
    return {N, static_cast<int>(DefaultHolder<int>::value)};
}
#include <cassert>
#include <utility>

// Declaration of the function under test (no need to include the solution here).
std::pair<int, int> templateValueAsDefaultArgument(int N);

int main() {
    // Basic positive input.
    auto p1 = templateValueAsDefaultArgument(42);
    assert(p1.first == 42 && p1.second == 1);

    // Negative input.
    auto p2 = templateValueAsDefaultArgument(-7);
    assert(p2.first == -7 && p2.second == 1);

    // Zero input.
    auto p3 = templateValueAsDefaultArgument(0);
    assert(p3.first == 0 && p3.second == 1);

    // Large input.
    auto p4 = templateValueAsDefaultArgument(1000000);
    assert(p4.first == 1000000 && p4.second == 1);

    // Ensure the compile-time constant is exactly 1 (not true converted to int).
    assert(templateValueAsDefaultArgument(5).second == 1);

    return 0;
}
