/*
Write a C++ function `simulateGiftExchange` that takes an integer `n` (number of people, 1 ≤ n ≤ 10), a vector of `n` unique names (strings without spaces), and a sequence of gift-giving records, and returns a vector of `n` strings in the original name order, each formatted as `"name netBalance"` where netBalance can be negative, zero, or positive. Each gift record consists of: a giver’s name, an integer amount `A` (0 ≤ A ≤ 10000), an integer `k` (number of recipients, 0 ≤ k ≤ n-1), followed by `k` recipient names. The giver gives each recipient exactly `floor(A/k)` (integer division) if `k>0`; the giver’s net balance is reduced by the total given (which is `floor(A/k)*k`), and each recipient’s net balance is increased by that same value. If `k=0`, the giver spends nothing. The function must handle multiple test cases separated by blank lines in the input, but the function itself processes one test case per call. The resulting vector must have one entry per person in the original order given in the names vector. Assume valid input (no duplicate names, all names in records exist in the names vector, recipients are distinct per record).
*/

#include <string>
#include <vector>
#include <map>
#include <sstream>

/**
 * Simulate a gift exchange and return net balances per person.
 * 
 * @param n Number of people (1..10)
 * @param names Vector of n unique names in original order
 * @param records A vector of strings, each representing one gift record.
 *                Format: "giverName amount k recipient1 recipient2 ..."
 * @return Vector of n strings "name netBalance" in original name order.
 */
std::vector<std::string> simulateGiftExchange(int n, const std::vector<std::string>& names,
                                              const std::vector<std::string>& records) {
    std::map<std::string, int> balance;
    for (const std::string& name : names) {
        balance[name] = 0;
    }
    
    for (const std::string& rec : records) {
        std::istringstream iss(rec);
        std::string giver;
        int amount, k;
        iss >> giver >> amount >> k;
        if (k > 0) {
            int share = amount / k;  // integer division truncates
            int totalGiven = share * k;
            balance[giver] -= totalGiven;
            for (int i = 0; i < k; ++i) {
                std::string recipient;
                iss >> recipient;
                balance[recipient] += share;
            }
        }
    }
    
    std::vector<std::string> result;
    result.reserve(n);
    for (const std::string& name : names) {
        result.push_back(name + " " + std::to_string(balance[name]));
    }
    return result;
}

#include <cassert>
#include <vector>
#include <string>

// The function is defined above (or included here) — in a real test, include the solution.

int main() {
    // Test case 1: Simple one giver, one recipient
    {
        std::vector<std::string> names = {"Alice", "Bob"};
        std::vector<std::string> records = {"Alice 10 1 Bob"};
        auto res = simulateGiftExchange(2, names, records);
        assert(res.size() == 2);
        assert(res[0] == "Alice -10");
        assert(res[1] == "Bob 10");
    }
    
    // Test case 2: k=0 means no gift given
    {
        std::vector<std::string> names = {"Alice", "Bob"};
        std::vector<std::string> records = {"Alice 100 0"};
        auto res = simulateGiftExchange(2, names, records);
        assert(res[0] == "Alice 0");
        assert(res[1] == "Bob 0");
    }
    
    // Test case 3: Multiple recipients, integer division truncation
    {
        std::vector<std::string> names = {"X", "Y", "Z"};
        std::vector<std::string> records = {"X 10 3 Y Z Y"};  // Y appears twice? but problem says distinct; here we test duplicate recipient just for logic
        auto res = simulateGiftExchange(3, names, records);
        // share = 10/3 = 3, total given = 9, X loses 9, Y gets 6, Z gets 3
        assert(res[0] == "X -9");
        assert(res[1] == "Y 6");
        assert(res[2] == "Z 3");
    }
    
    // Test case 4: Multiple records, net accumulation
    {
        std::vector<std::string> names = {"A", "B", "C"};
        std::vector<std::string> records = {
            "A 20 2 B C",
            "B 10 1 C",
            "C 5 0"
        };
        auto res = simulateGiftExchange(3, names, records);
        // A: -20, B: +10 (from A) then -10 = 0, C: +10 (from A) +10 (from B) = 20
        assert(res[0] == "A -20");
        assert(res[1] == "B 0");
        assert(res[2] == "C 20");
    }
    
    // Test case 5: One person, no records
    {
        std::vector<std::string> names = {"Solo"};
        std::vector<std::string> records;
        auto res = simulateGiftExchange(1, names, records);
        assert(res[0] == "Solo 0");
    }
    
    // Test case 6: Large amount, k=1
    {
        std::vector<std::string> names = {"U", "V"};
        std::vector<std::string> records = {"U 9999 1 V"};
        auto res = simulateGiftExchange(2, names, records);
        assert(res[0] == "U -9999");
        assert(res[1] == "V 9999");
    }
    
    return 0;
}

// The solution uses a `map<string,int>` to track each person’s net balance, initialized to 0 for all names. For each gift record, if `k>0`, compute `share = amount / k` (integer division), subtract `share * k` from the giver’s balance (since the giver loses that total), and then for each recipient add `share` to their balance. If `k=0`, do nothing (the giver’s balance remains unchanged because no money is spent). After processing all records, iterate over the original names vector and construct strings `name + " " + to_string(balance[name])`. Edge cases: `k=0` must not cause division by zero; integer division truncates correctly; negative balances are allowed; the input may have multiple records for the same person (as giver or recipient) and the balances accumulate. Time complexity is O(n + R * k_max) where R is the number of records and k_max is the maximum recipients per record, but since n ≤ 10 and total recipients per record ≤ n-1, it is effectively O(R*n) which is small. Space complexity is O(n) for the map and result vector.
