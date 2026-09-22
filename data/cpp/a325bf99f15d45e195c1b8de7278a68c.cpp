/*
Write a C++ function `sumMonthlySalesBySeller` that takes a `std::array<std::array<int, 12>, 3>` representing total sales amounts (first index = seller 0–2, second index = month 0–11) and returns a `std::array<int, 3>` containing, for each seller, the total sum of sales across all 12 months. The function must not modify the input array and must be `const`-correct. Assume the input array is always exactly 3×12 (this matches the fixed constants in the problem, but your function should hardcode the dimensions as named constants for clarity). The function should handle zero and negative sales values naturally, and no other input validation is needed because the array dimensions are fixed. The function must be standalone (no `main`), with all necessary headers included, and return the sum per seller in the same order (index 0, 1, 2).
*/

#include <array>

// Return the sum of sales for each seller across all 12 months.
// The input has dimensions [3][12] representing seller-major, month-minor.
std::array<int, 3> sumMonthlySalesBySeller(const std::array<std::array<int, 12>, 3>& sales) {
    constexpr int SELLER_COUNT = 3;
    constexpr int MONTH_COUNT = 12;

    std::array<int, SELLER_COUNT> sellerTotals{};

    for (int seller = 0; seller < SELLER_COUNT; ++seller) {
        int total = 0;
        for (int month = 0; month < MONTH_COUNT; ++month) {
            total += sales[seller][month];
        }
        sellerTotals[seller] = total;
    }

    return sellerTotals;
}

#include <array>
#include <cassert>

// Declaration of the solution function (must match the provided signature)
std::array<int, 3> sumMonthlySalesBySeller(const std::array<std::array<int, 12>, 3>& sales);

int main() {
    // Test 1: All zeros
    std::array<std::array<int, 12>, 3> sales1{};
    std::array<int, 3> result1 = sumMonthlySalesBySeller(sales1);
    assert(result1 == std::array<int, 3>{0, 0, 0});

    // Test 2: Simple positive values, one per month for each seller
    std::array<std::array<int, 12>, 3> sales2{};
    for (int m = 0; m < 12; ++m) {
        sales2[0][m] = 1;
        sales2[1][m] = 2;
        sales2[2][m] = 3;
    }
    std::array<int, 3> result2 = sumMonthlySalesBySeller(sales2);
    assert(result2 == std::array<int, 3>{12, 24, 36});

    // Test 3: Mixed positive and negative values
    std::array<std::array<int, 12>, 3> sales3{};
    // Seller 0: sum = 0 + 5 + (-5) + ... -> set explicitly
    sales3[0][0] = 5;
    sales3[0][1] = -5;
    sales3[0][2] = 10;
    // Seller 1: all zero except one
    sales3[1][11] = 7;
    // Seller 2: sum = 100 + (-200) + 50 = -50
    sales3[2][0] = 100;
    sales3[2][1] = -200;
    sales3[2][2] = 50;
    std::array<int, 3> result3 = sumMonthlySalesBySeller(sales3);
    assert(result3[0] == 10);
    assert(result3[1] == 7);
    assert(result3[2] == -50);

    // Test 4: Max positive values within int range (per month e.g., 1000000)
    std::array<std::array<int, 12>, 3> sales4{};
    for (auto& row : sales4) {
        row.fill(1000000);
    }
    std::array<int, 3> result4 = sumMonthlySalesBySeller(sales4);
    assert(result4 == std::array<int, 3>{12000000, 12000000, 12000000});

    // Test 5: Only one seller has sales, others zero
    std::array<std::array<int, 12>, 3> sales5{};
    for (int m = 0; m < 12; ++m) {
        sales5[1][m] = m; // sums to 0+1+...+11 = 66
    }
    std::array<int, 3> result5 = sumMonthlySalesBySeller(sales5);
    assert(result5[0] == 0);
    assert(result5[1] == 66);
    assert(result5[2] == 0);

    return 0;
}

// The problem is straightforward: iterate over each seller (outer loop) and each month (inner loop), accumulating the sales values into a corresponding sum. The main algorithm uses two nested loops: for each seller index `i` from 0 to 2, initialize a running total to zero, then for each month index `j` from 0 to 11, add `sales[i][j]` to the total. After the inner loop, store the total in the output array at position `i`. Edge cases: the input is always a 3×12 array, so no out-of-bounds access occurs if we use fixed constants; negative sales values are handled correctly by addition; zero values contribute nothing. Because we only pass the array by const reference (or we can pass by value, but by const reference is better to avoid copying), we need to include `<array>` and possibly `<cstddef>` for `size_t`. Time complexity is O(3×12) = O(36) which is constant, or O(1) asymptotic, and space complexity is O(1) for the output array (ignoring the input array which is already allocated). The function returns by value (a `std::array<int, 3>`), which is trivial to copy.
