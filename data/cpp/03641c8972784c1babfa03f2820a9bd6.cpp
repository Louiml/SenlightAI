Write a C++ function `std::string settleDebts(int numBanks, int numTransactions)`. The function reads from standard input (via `cin`) a sequence of bank reserve values and then a series of transactions, each described by three integers: the debtor bank ID (1-based), the creditor bank ID, and the amount transferred. Apply all transactions by deducting from the debtor’s reserve and adding to the creditor’s reserve. After processing all transactions, return `"S"` if every bank’s final reserve is non-negative; otherwise return `"N"`. The function stops reading input when given `numBanks == 0 && numTransactions == 0`. It outputs exactly one result per line. The main program already handles reading these two initial integers, but for this task, your function will read the rest of the data (reserves and transactions) itself. Constraints: bank IDs are between 1 and `numBanks`, reserves and transfer amounts are non-negative integers, and the total number of transactions may be zero. The function must work correctly for any number of banks up to 10^5 and transactions up to 10^6, reading efficiently.
#include <cassert>
#include <sstream>
#include <iostream>

std::string settleDebts(int numBanks, int numTransactions);

// Helper to run a test case with given input string
std::string runTest(const std::string& input, int b, int n) {
    std::istringstream iss(input);
    std::cin.rdbuf(iss.rdbuf()); // redirect cin to stringstream
    return settleDebts(b, n);
}

int main() {
    // Test 1: basic valid case
    assert(runTest("10 20 30\n1 2 5\n2 3 3\n", 3, 2) == "S");
    // Test 2: one bank goes negative
    assert(runTest("10 5 10\n1 2 15\n", 3, 1) == "N");
    // Test 3: zero transactions
    assert(runTest("5 5 5\n", 3, 0) == "S");
    // Test 4: transaction that barely stays non-negative
    assert(runTest("10 0 5\n1 2 10\n", 3, 1) == "S");
    // Test 5: one bank only
    assert(runTest("100\n", 1, 0) == "S");
    // Test 6: multiple transactions, final negative
    assert(runTest("10 10 10\n1 2 20\n2 3 5\n", 3, 2) == "N");
    // Test 7: large amounts (fit in long long)
    assert(runTest("1000000000 1000000000 1000000000\n1 2 500000000\n2 3 500000000\n", 3, 2) == "S");
    // Test 8: negative after chain
    assert(runTest("10 0 10\n1 2 5\n2 3 6\n", 3, 2) == "N");
    // Test 9: all zero reserves
    assert(runTest("0 0 0\n1 2 0\n", 3, 1) == "S");
    // Test 10: negative in middle
    assert(runTest("1 2 3\n2 1 5\n", 3, 1) == "S"); // bank 2 becomes -3, fails

    // Restore standard input (not strictly necessary for automated tests)
    return 0;
}
#include <string>
#include <vector>
#include <iostream>

// Reads bank reserves and transaction records from standard input,
// then returns "S" if all reserves remain non-negative, else "N".
std::string settleDebts(int numBanks, int numTransactions) {
    std::vector<long long> reserves(numBanks + 1); // 1-based indexing

    // Read initial reserves
    for (int i = 1; i <= numBanks; ++i) {
        std::cin >> reserves[i];
    }

    // Process each transaction
    for (int t = 0; t < numTransactions; ++t) {
        int debtor, creditor, amount;
        std::cin >> debtor >> creditor >> amount;
        reserves[debtor] -= amount;
        reserves[creditor] += amount;
    }

    // Check for any negative reserve
    for (int i = 1; i <= numBanks; ++i) {
        if (reserves[i] < 0) {
            return "N";
        }
    }
    return "S";
}
// The core is to maintain a vector of reserves indexed by bank ID (1-based). Initialize by reading exactly `numBanks` integers into this vector. Then for each of the `numTransactions` lines, read `debtor`, `creditor`, `amount`. Apply the transaction by subtracting `amount` from reserve[debtor] and adding it to reserve[creditor]. After all transactions, iterate through the vector to check for any negative value. If any negative is found, return `"N"`, else `"S"`. Edge cases: zero banks/transactions (but that signals termination in the calling loop; the function itself is not called with both zero), but inside the function, if there are zero transactions, just check the reserves. No need for overflow handling because amounts are that large but within `int` (the original snippet used `int`; but we can use `long long` to be safe, though constraints likely fit in `int`). Time complexity: O(numBanks + numTransactions) because we do one pass to read, one pass per transaction, and one final pass to check. Space: O(numBanks) for the reserve vector. The function uses `cin` directly, so it must be called with input already prepared; the main test harness should redirect input streams appropriately.
