/*
Write a C++ function named `isMagicDate` that takes three integer arguments representing a month (1–12), a day (1–31), and a two-digit year (0–99), and returns `true` if the date is "magic" — meaning the product of the month and the day equals the two-digit year — and `false` otherwise. The function must be const-correct (all parameters passed by value are inherently const, but you must mark the function itself as not modifying state, i.e., it is a free function with no side effects). Do not perform input validation; assume the caller provides values in the stated ranges. The function should be pure and deterministic.
*/
#include <cstdint>

// Returns true if the product of month and day equals the two-digit year.
bool isMagicDate(int month, int day, int year) {
    // Use a wider type to avoid any potential overflow (not needed here but good practice).
    const std::int64_t product = static_cast<std::int64_t>(month) * day;
    return product == static_cast<std::int64_t>(year);
}
#include <cassert>

int main() {
    // Magic dates: month * day == year
    assert(isMagicDate(1, 1, 1) == true);
    assert(isMagicDate(2, 3, 6) == true);
    assert(isMagicDate(6, 9, 54) == true);
    assert(isMagicDate(12, 5, 60) == true);

    // Non-magic dates
    assert(isMagicDate(1, 1, 0) == false);
    assert(isMagicDate(2, 3, 7) == false);
    assert(isMagicDate(12, 31, 99) == false); // product is 372, not 99
    assert(isMagicDate(10, 10, 100) == false); // year is only two-digit, 100 is out of range but function just compares

    // Edge: product equals year even when year is small
    assert(isMagicDate(1, 1, 1) == true);
    assert(isMagicDate(1, 2, 2) == true); // 1*2=2
    assert(isMagicDate(3, 3, 9) == true); // 3*3=9
}
// The solution is straightforward: compute the product of `month` and `day` as an integer (to avoid overflow, use `long long` or `std::int64_t` because the maximum product is 12*31=372, which is well within `int` range, but using `long long` is safer for general practice). Then compare this product directly to the `year` value. The main algorithm is a single multiplication and equality check, so it runs in O(1) time and uses O(1) auxiliary space. Edge cases: the smallest date (1/1/00) gives product 1, which is not equal to year 0 unless year is 1; the largest valid date (12/31/00) gives product 372, but the year is only two digits (0–99), so the maximum possible year is 99 — many combinations will simply be false. There is no need to handle invalid months or days because the problem statement guarantees valid inputs. The function does not need any loops or conditionals beyond the equality check. Since the function takes parameters by value, it is inherently const-correct; we just ensure it doesn't modify any external state.
