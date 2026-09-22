Write a C++ function `int winningProductIndex(int numProducts, int numCustomers)` that reads `numCustomers` rows of `numProducts` integers from standard input (each integer represents how much a customer likes a product), counts how many times each product is the most preferred in a row (ties broken by the first product index in the row, i.e., if two products have the same maximum value in a row, the smaller index wins), and returns the 1-based index of the product that wins the most rows. If there is a tie in the total wins, return the product with the smaller 1-based index. The function should read all input itself and return the answer. Assume `1 ≤ numProducts, numCustomers ≤ 1000`, and each preference value is a non-negative integer.
// The solution processes the input row by row. For each customer row, we find the maximum value and the smallest index achieving that maximum (since we scan left to right and only update when a value is strictly greater than the current maximum, the first occurrence of the global maximum is naturally selected). We increment a counter for that product index. After processing a row, we update the global best result: if the current product's count exceeds the current best count, or if it equals the best count and the product index is smaller (0-based comparison), we update the best index. After all rows are processed, return `bestIndex + 1` to convert to 1-based. Edge cases: all preferences equal in a row → the first product wins that row; multiple products with same total wins → smallest index wins; single product → always wins every row. Time complexity is O(numCustomers × numProducts) because each row is scanned once, and space complexity is O(numProducts) for the count array.
#include <vector>
#include <iostream>

// Read numCustomers rows of numProducts integers from stdin,
// count per-row winning product (ties broken by smallest index),
// and return 1-based index of product with most wins (ties by smaller index).
int winningProductIndex(int numProducts, int numCustomers) {
    std::vector<int> winCount(numProducts, 0);
    int bestCount = 0;
    int bestIndex = 0; // 0-based index of current leader

    for (int row = 0; row < numCustomers; ++row) {
        int maxVal = -1;
        int rowWinner = 0;
        for (int col = 0; col < numProducts; ++col) {
            int value;
            std::cin >> value;
            if (value > maxVal) {
                maxVal = value;
                rowWinner = col;
            }
        }
        ++winCount[rowWinner];
        if (winCount[rowWinner] > bestCount ||
            (winCount[rowWinner] == bestCount && rowWinner < bestIndex)) {
            bestCount = winCount[rowWinner];
            bestIndex = rowWinner;
        }
    }

    return bestIndex + 1; // convert to 1-based
}
#include <cassert>
#include <sstream>
#include <iostream>

// The function reads from std::cin, so tests must redirect cin.
int winningProductIndex(int numProducts, int numCustomers);

int main() {
    // Test 1: basic case
    {
        std::istringstream in("1 2 3\n3 2 1\n1 1 1\n");
        std::streambuf* old = std::cin.rdbuf(in.rdbuf());
        int result = winningProductIndex(3, 3);
        std::cin.rdbuf(old);
        assert(result == 1);
    }

    // Test 2: tie in total wins, smaller index wins
    {
        std::istringstream in("5 0\n0 5\n");
        std::streambuf* old = std::cin.rdbuf(in.rdbuf());
        int result = winningProductIndex(2, 2);
        std::cin.rdbuf(old);
        assert(result == 1);
    }

    // Test 3: single product
    {
        std::istringstream in("7\n42\n");
        std::streambuf* old = std::cin.rdbuf(in.rdbuf());
        int result = winningProductIndex(1, 2);
        std::cin.rdbuf(old);
        assert(result == 1);
    }

    // Test 4: all rows same product wins, then another product wins more
    {
        std::istringstream in("10 0 0\n9 1 0\n0 5 4\n0 5 4\n");
        std::streambuf* old = std::cin.rdbuf(in.rdbuf());
        int result = winningProductIndex(3, 4);
        std::cin.rdbuf(old);
        assert(result == 2);
    }

    // Test 5: strictly increasing preferences shift winner
    {
        std::istringstream in("1 2 3\n1 2 3\n1 2 3\n");
        std::streambuf* old = std::cin.rdbuf(in.rdbuf());
        int result = winningProductIndex(3, 3);
        std::cin.rdbuf(old);
        assert(result == 3);
    }

    // Test 6: tie within a row, first product should win that row
    {
        std::istringstream in("4 4 1\n1 4 4\n");
        std::streambuf* old = std::cin.rdbuf(in.rdbuf());
        int result = winningProductIndex(3, 2);
        std::cin.rdbuf(old);
        assert(result == 1);
    }

    // Test 7: larger case with many rows, all equal values
    {
        std::ostringstream input;
        for (int i = 0; i < 100; ++i) {
            for (int j = 0; j < 5; ++j) {
                input << "3 ";
            }
            input << "\n";
        }
        std::istringstream in(input.str());
        std::streambuf* old = std::cin.rdbuf(in.rdbuf());
        int result = winningProductIndex(5, 100);
        std::cin.rdbuf(old);
        assert(result == 1);
    }

    return 0;
}
