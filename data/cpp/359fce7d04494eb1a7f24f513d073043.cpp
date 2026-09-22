// Write a C++ function named `netSalary` that accepts a gross annual salary as a non-negative integer and returns the net (after-tax) salary as a `uint32_t`, using a progressive tax system with the exact same brackets and rates as shown in the snippet: the first 13,000 of income is tax-free; income from 13,001 to 50,000 is taxed at 30%; income from 50,001 to 100,000 is taxed at 45%; and any income above 100,000 is taxed at 55%. The function must be recursive, computing the tax on each bracket portion separately, and must correctly handle edge cases such as a salary of zero, a salary exactly at a bracket boundary, and very large salaries (up to the maximum `int`). The returned value must be truncated (cast to `uint32_t`), consistent with the original code. Do not write a `main` function in the solution; just provide the function with necessary headers.

The solution uses a recursive approach that mirrors the original snippet. The algorithm checks the input salary against four increasing thresholds (13,000, 50,000, 100,000). For each bracket above the base, it computes the tax‑free portion of that bracket’s income (the difference between the current salary and the lower bracket boundary), applies the corresponding tax rate to that portion, and then adds the recursively computed net salary for the lower bracket boundary. For example, if `salary > 100,000`, it takes `(1 - 0.55) * (salary - 100,000)` and adds `netSalary(100,000)`. This recursion terminates at the base case where salary ≤ 13,000, in which case the full salary is returned. Edge cases: salary exactly at a boundary (e.g., 50,000) falls into the `else if` branch for the middle bracket because the condition `salary > MiddleBracket && salary <= TopBracket` treats 50,000 as not greater than 50,000, so it goes to the lower bracket branch (since 50,000 > 13,000 and ≤ 50,000), which correctly computes `(1 - 0.30) * (50,000 - 13,000) + netSalary(13,000) = 0.7*37,000 + 13,000 = 25,900 + 13,000 = 38,900`. Boundary at 13,000 exactly goes to the final `else` because it is not greater than 13,000, so it returns 13,000 tax-free. The recursion depth is at most 3 (since each call reduces the salary to the next lower bracket boundary), so space complexity is O(1) in practice. Time complexity is O(1) since a constant number of arithmetic operations are performed per recursive call. The function uses `uint32_t` for the return and `int` for the input; careful casting avoids overflow for salaries up to `INT_MAX` (the maximum tax portion is about 55% of 2.1 billion, well within `double` precision).

#include <cstdint>

// Computes net salary after progressive tax based on salary brackets.
// Brackets: 0-13,000 tax-free; 13,001-50,000 at 30%; 50,001-100,000 at 45%; above 100,000 at 55%.
// Recursive implementation consistent with the original snippet.
uint32_t netSalary(int baseSalary) {
    const uint32_t topBracket = 100000;
    const uint32_t middleBracket = 50000;
    const uint32_t lowerBracket = 13000;
    const double topTax = 0.55;
    const double middleTax = 0.45;
    const double lowerTax = 0.30;

    double newSalary = 0.0;

    if (baseSalary > static_cast<int>(topBracket)) {
        // Tax the portion above 100,000 at 55%, then recursively handle the rest.
        newSalary += (1.0 - topTax) * (baseSalary - static_cast<int>(topBracket));
        newSalary += netSalary(static_cast<int>(topBracket));
    } else if (baseSalary > static_cast<int>(middleBracket) && baseSalary <= static_cast<int>(topBracket)) {
        // Tax the portion above 50,000 but up to 100,000 at 45%.
        newSalary += (1.0 - middleTax) * (baseSalary - static_cast<int>(middleBracket));
        newSalary += netSalary(static_cast<int>(middleBracket));
    } else if (baseSalary > static_cast<int>(lowerBracket) && baseSalary <= static_cast<int>(middleBracket)) {
        // Tax the portion above 13,000 but up to 50,000 at 30%.
        newSalary += (1.0 - lowerTax) * (baseSalary - static_cast<int>(lowerBracket));
        newSalary += netSalary(static_cast<int>(lowerBracket));
    } else {
        // Salary is at or below 13,000: no tax.
        newSalary = static_cast<double>(baseSalary);
    }

    return static_cast<uint32_t>(newSalary);
}

#include <cassert>
#include <cstdint>

// Declaration of the function under test (already defined above in solution).
uint32_t netSalary(int baseSalary);

int main() {
    // Basic cases: no tax for 13,000 or less.
    assert(netSalary(0) == 0);
    assert(netSalary(13000) == 13000);
    assert(netSalary(10000) == 10000);

    // Lower bracket: 13,001 to 50,000 taxed at 30%.
    // Example: 50,000 -> 0.7*(50,000-13,000) + 13,000 = 25,900 + 13,000 = 38,900
    assert(netSalary(50000) == 38900);
    assert(netSalary(20000) == 0.7 * (20000 - 13000) + 13000); // = 17900

    // Middle bracket: 50,001 to 100,000 taxed at 45%.
    // Example: 100,000 -> 0.55*(100,000-50,000) + netSalary(50,000) = 0.55*50,000 + 38,900 = 27,500 + 38,900 = 66,400
    assert(netSalary(100000) == 66400);
    assert(netSalary(75000) == 0.55 * (75000 - 50000) + netSalary(50000)); // = 0.55*25000 + 38900 = 13750 + 38900 = 52650

    // Top bracket: above 100,000 taxed at 55%.
    // Example: 150,000 -> 0.45*(150,000-100,000) + netSalary(100,000) = 0.45*50,000 + 66,400 = 22,500 + 66,400 = 88,900
    assert(netSalary(150000) == 88900);
    assert(netSalary(100001) == 0.45 * (100001 - 100000) + netSalary(100000)); // = 0.45*1 + 66400 = 66400.45 -> truncated to 66400
    assert(netSalary(2147483647) == 0.45 * (2147483647 - 100000) + netSalary(100000)); // large input

    // Boundary exact equality: 13000 is tax-free (since > 13000 is false), 50000 uses lower bracket.
    assert(netSalary(13001) == 0.7 * (13001 - 13000) + 13000); // = 0.7 + 13000 = 13000.7 -> truncates to 13000
    assert(netSalary(50001) == 0.55 * (50001 - 50000) + netSalary(50000)); // = 0.55 + 38900 = 38900.55 -> truncates to 38900

    return 0;
}
