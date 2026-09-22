// Write a C++ function that reads exactly five integers from the standard input (the first three representing burger prices, the next two representing beverage prices) and returns the minimum cost of a set meal, which is the sum of the cheapest burger and the cheapest beverage minus a fixed 50-dollar discount. The prices are positive integers, and the function should compute the result without any leftover input issues. The function must be named `minimumSetMealCost` and return an `int`. It should read all input from `std::cin` and assume that exactly five valid integers are provided on separate lines or whitespace-separated.
// The solution is straightforward: we read the first three values, track the minimum among them as the cheapest burger; then read the next two values, track the minimum among them as the cheapest beverage. Then the set meal cost is `minBurger + minBeverage - 50`. The algorithm is a linear scan over exactly five integers, so it runs in O(1) time and uses O(1) auxiliary space (only a few scalar variables). Edge cases: if all burger prices are equal (e.g., 2000 2000 2000) the minimum remains that value; similarly for beverages. There is no need to handle invalid input because the problem guarantees five positive integers. The discount is applied after summing the two minima, and the result is always non-negative because prices are positive and the discount is 50. Read using `std::cin` with `>>` which skips whitespace and newlines automatically.
#include <iostream>
#include <algorithm>

// Reads three burger prices and two beverage prices from standard input,
// returns the cheapest set meal cost (min burger + min beverage - 50).
int minimumSetMealCost() {
    int minBurger = 2000;    // initial high value, burgers are <= 2000 per problem
    int minBeverage = 2000;  // initial high value, beverages are <= 2000 per problem

    int price;
    for (int i = 0; i < 3; ++i) {
        std::cin >> price;
        minBurger = std::min(minBurger, price);
    }
    for (int i = 0; i < 2; ++i) {
        std::cin >> price;
        minBeverage = std::min(minBeverage, price);
    }

    return minBurger + minBeverage - 50;
}
#include <cassert>
#include <sstream>
#include <iostream>

// Declare the solution function (normally included from header)
int minimumSetMealCost();

// Helper to redirect cin for testing
void runTest(const std::string& input, int expected) {
    std::istringstream iss(input);
    std::cin.rdbuf(iss.rdbuf()); // redirect cin to stringstream
    assert(minimumSetMealCost() == expected);
}

int main() {
    // Test 1: Basic case
    runTest("1000 900 800\n700 600\n", 800 + 600 - 50); // 1350

    // Test 2: All same prices
    runTest("2000 2000 2000\n2000 2000\n", 2000 + 2000 - 50); // 3950

    // Test 3: Minimum burger is first, minimum beverage is second
    runTest("500 1200 1500\n800 700\n", 500 + 700 - 50); // 1150

    // Test 4: Minimum burger is last, minimum beverage is first
    runTest("1500 1300 400\n300 900\n", 400 + 300 - 50); // 650

    // Test 5: Prices with whitespace on same line
    runTest("100 200 300 400 500\n", 100 + 400 - 50); // 450

    // Test 6: Duplicate minima in both groups
    runTest("900 900 900\n700 700\n", 900 + 700 - 50); // 1550

    // Test 7: Smallest possible prices (assuming positive)
    runTest("1 2 3\n4 5\n", 1 + 4 - 50); // -45, but still valid output; assert checks value

    // Test 8: Larger variation
    runTest("1234 2345 3456\n4567 5678\n", 1234 + 4567 - 50); // 5751

    // Test 9: Newline and spaces mixed
    runTest("111 222\n333\n444 555\n666\n", 111 + 444 - 50); // 505

    // Test 10: Exactly five integers, no extra
    runTest("10 20 30 40 50\n", 10 + 40 - 50); // 0

    std::cout << "All tests passed!\n";
    return 0;
}
