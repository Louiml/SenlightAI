/*
Write a C++ function named `budgetStatus` that takes a starting sum of money (a non-negative integer), a vector of 12 monthly required expenses (non-negative integers), and a vector of 12 monthly incomes added after each month's expense check (non-negative integers). The function must simulate 12 months: at the start of each month, check if the current sum is sufficient to pay that month's required expense. If `sum >= expense`, print `"No problem! :D"` and subtract the expense from the sum; otherwise, print `"No problem. :("` and do not subtract. After each check, add the corresponding month's income to the sum. The function should return the final remaining sum after all 12 months. All inputs are guaranteed to have exactly 12 elements. Assume the starting sum is non-negative.
*/
#include <vector>
#include <iostream>

// Simulate 12 months of budget checks.
// Prints messages and returns final remaining sum.
long long budgetStatus(long long initialSum,
                       const std::vector<long long>& expenses,
                       const std::vector<long long>& incomes) {
    long long sum = initialSum;
    for (int i = 0; i < 12; ++i) {
        if (sum >= expenses[i]) {
            std::cout << "No problem! :D" << std::endl;
            sum -= expenses[i];
        } else {
            std::cout << "No problem. :(" << std::endl;
        }
        sum += incomes[i];
    }
    return sum;
}
#include <cassert>
#include <vector>
#include <iostream>

// Forward declaration (or include header with function)
long long budgetStatus(long long, const std::vector<long long>&, const std::vector<long long>&);

int main() {
    // Test 1: All expenses zero, all incomes zero, start 100 -> final 100
    std::vector<long long> e1(12, 0);
    std::vector<long long> i1(12, 0);
    assert(budgetStatus(100, e1, i1) == 100);

    // Test 2: Start 0, expense 10 each month, income 5 each month -> all fail, final 60
    std::vector<long long> e2(12, 10);
    std::vector<long long> i2(12, 5);
    assert(budgetStatus(0, e2, i2) == 60);

    // Test 3: Start 10, expense 10 then zeros, income zeros -> first month success, final 0
    std::vector<long long> e3 = {10,0,0,0,0,0,0,0,0,0,0,0};
    std::vector<long long> i3(12, 0);
    assert(budgetStatus(10, e3, i3) == 0);

    // Test 4: Start 5, expense 5, income 10 each month -> always success, final 5+12*10=125
    std::vector<long long> e4(12, 5);
    std::vector<long long> i4(12, 10);
    assert(budgetStatus(5, e4, i4) == 125);

    // Test 5: Start 0, expense 1 each month, income 0 -> all fail, final 0
    std::vector<long long> e5(12, 1);
    std::vector<long long> i5(12, 0);
    assert(budgetStatus(0, e5, i5) == 0);

    // Test 6: Start 100, expense 1000 each month, income 200 each month -> fail all, final 100+12*200=2500
    std::vector<long long> e6(12, 1000);
    std::vector<long long> i6(12, 200);
    assert(budgetStatus(100, e6, i6) == 2500);

    // Test 7: Start 5, expense [1,2,3,4,5,6,7,8,9,10,11,12], income zeros -> check order
    std::vector<long long> e7 = {1,2,3,4,5,6,7,8,9,10,11,12};
    std::vector<long long> i7(12, 0);
    // Simulate manually: start 5
    // month0: 5>=1 -> sum=4
    // month1: 4>=2 -> sum=2
    // month2: 2>=3? no -> sum=2
    // month3: 2>=4? no -> sum=2
    // ... all remaining fail, final 2
    assert(budgetStatus(5, e7, i7) == 2);

    // Test 8: Start 12, same expenses as above, incomes zeros
    // month0: 12>=1 -> sum=11
    // month1: 11>=2 -> sum=9
    // month2: 9>=3 -> sum=6
    // month3: 6>=4 -> sum=2
    // month4: 2>=5? no
    // month5: 2>=6? no
    // ... final 2
    assert(budgetStatus(12, e7, i7) == 2);

    // Test 9: Start 0, expense [0,0,...], income [1,2,...12] -> all success, final sum = sum(1..12)=78
    std::vector<long long> e9(12, 0);
    std::vector<long long> i9 = {1,2,3,4,5,6,7,8,9,10,11,12};
    assert(budgetStatus(0, e9, i9) == 78);

    // Test 10: Start 1, expense [2,2,2,...], income [1,1,1,...] -> first fail, then all fail, final = 1 + 12*1 = 13
    std::vector<long long> e10(12, 2);
    std::vector<long long> i10(12, 1);
    assert(budgetStatus(1, e10, i10) == 13);

    std::cout << "All tests passed!" << std::endl;
    return 0;
}
// The solution simulates the process month by month. For each index `i` from 0 to 11, compare the current accumulated sum with the expense at index `i`. If the sum is sufficient, output the positive message and reduce the sum by that expense. If not, output the negative message and leave the sum unchanged. After handling the expense, always add the income for that month to the sum. The order is important: the expense check occurs before adding the income. Edge cases include a starting sum of zero, expenses equal to the sum (in which case it is sufficient), and months where income is zero. The algorithm runs in O(12) time and O(1) auxiliary space, excluding the input vectors. The function prints directly to standard output, so no string building is needed.
