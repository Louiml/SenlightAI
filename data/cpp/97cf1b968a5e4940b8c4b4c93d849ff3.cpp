Given a positive integer `n`, a list of `n` non-negative financial limits, and a sequence of `q` queries where each query is either `1 targetId amount` (add `amount` to the charity with the given 1-based ID, allowing overflow to the next charity if the limit is exceeded, and any final overflow beyond the last charity is discarded) or `2 targetId` (return the current balance of that charity), write a standalone C++ function that processes these queries and returns a string containing the answers to all type‑2 queries, each on its own line. The function must implement the exact overflow-propagation rule: when adding `amount` to a charity’s balance, if the new balance would exceed its financial limit, set that charity’s balance to its limit and add the excess to the next charity (ID+1) recursively; if there is no next charity, ignore the excess. The initial balance of every charity is 0. The input is provided as parameters: an integer `n`, a vector of `n` long integers representing the limits in ID order, an integer `q`, and a vector of queries represented as integers (each query is a triplet `{type, targetId}` or `{type, targetId, amount}` depending on type; to simplify the interface, you may use a vector of strings or a struct as described in the function signature). The function must return a string with answers separated by newlines.
#include <cassert>
#include <string>
#include <vector>

// Forward declaration
std::string processCharityQueries(int n,
                                  const std::vector<long long>& limits,
                                  int q,
                                  const std::vector<std::vector<long long>>& queries);

int main() {
    // Test 1: Basic two charities, overflow from first to second
    {
        std::vector<long long> limits = {10, 5};
        std::vector<std::vector<long long>> queries = {{1, 1, 12}, {2, 1}, {2, 2}};
        std::string result = processCharityQueries(2, limits, 3, queries);
        assert(result == "10\n7");
    }

    // Test 2: Overflow beyond last charity is discarded
    {
        std::vector<long long> limits = {5};
        std::vector<std::vector<long long>> queries = {{1, 1, 20}, {2, 1}};
        std::string result = processCharityQueries(1, limits, 2, queries);
        assert(result == "5");
    }

    // Test 3: No overflow, simple balances
    {
        std::vector<long long> limits = {100, 200, 300};
        std::vector<std::vector<long long>> queries = {{1, 2, 50}, {2, 1}, {2, 2}, {2, 3}};
        std::string result = processCharityQueries(3, limits, 4, queries);
        assert(result == "0\n50\n0");
    }

    // Test 4: Chain overflow across multiple charities
    {
        std::vector<long long> limits = {10, 10, 10};
        std::vector<std::vector<long long>> queries = {{1, 1, 25}, {2, 1}, {2, 2}, {2, 3}};
        std::string result = processCharityQueries(3, limits, 4, queries);
        assert(result == "10\n10\n5");
    }

    // Test 5: Multiple type-1 queries accumulate
    {
        std::vector<long long> limits = {10, 10};
        std::vector<std::vector<long long>> queries = {{1, 1, 5}, {1, 1, 8}, {2, 1}, {2, 2}};
        std::string result = processCharityQueries(2, limits, 4, queries);
        assert(result == "10\n3");
    }

    // Test 6: Zero amount and no answer queries
    {
        std::vector<long long> limits = {5};
        std::vector<std::vector<long long>> queries = {{1, 1, 0}, {1, 1, 0}};
        std::string result = processCharityQueries(1, limits, 2, queries);
        assert(result.empty());
    }

    // Test 7: Single query type 2 with no prior additions
    {
        std::vector<long long> limits = {42};
        std::vector<std::vector<long long>> queries = {{2, 1}};
        std::string result = processCharityQueries(1, limits, 1, queries);
        assert(result == "0");
    }

    // Test 8: Multiple answers separated by newline
    {
        std::vector<long long> limits = {1, 2, 3};
        std::vector<std::vector<long long>> queries = {{2, 1}, {2, 2}, {2, 3}};
        std::string result = processCharityQueries(3, limits, 3, queries);
        assert(result == "0\n0\n0");
    }

    return 0;
}
#include <string>
#include <vector>
#include <cstdint>

// Process charity overflow queries and return answers for type-2 queries.
// Queries are encoded as: for type 1, {1, targetId, amount}; for type 2, {2, targetId}.
// The function returns a string with each type-2 answer on a separate line.
std::string processCharityQueries(int n,
                                  const std::vector<long long>& limits,
                                  int q,
                                  const std::vector<std::vector<long long>>& queries) {
    std::vector<long long> balance(n, 0);
    std::string result;

    for (int idx = 0; idx < q; ++idx) {
        int type = static_cast<int>(queries[idx][0]);
        int targetId = static_cast<int>(queries[idx][1]) - 1; // convert to 0-based
        if (targetId < 0 || targetId >= n) continue; // ignore invalid

        if (type == 1) {
            long long amount = queries[idx][2];
            int i = targetId;
            long long excess = amount;
            while (i < n && excess > 0) {
                if (balance[i] + excess > limits[i]) {
                    excess = balance[i] + excess - limits[i];
                    balance[i] = limits[i];
                } else {
                    balance[i] += excess;
                    excess = 0;
                }
                ++i;
            }
            // If i reaches n, excess is discarded.
        } else if (type == 2) {
            if (!result.empty()) result += "\n";
            result += std::to_string(balance[targetId]);
        }
    }
    return result;
}
// The core challenge is efficiently locating a charity by ID across many queries. The original snippet uses a doubly linked list plus a cache of pointers to jump closer to the target ID. For a standalone exercise, we can simplify while maintaining correctness: build the chain as a vector of charity objects (each storing `financialLimit`, `balance`, and its ID), because direct index access gives O(1) lookup. Then for each type‑1 query, we simulate the overflow by iterating from the target ID to the next IDs, updating balances: `balance[i] += amount`, if `balance[i] > limit[i]`, set `excess = balance[i] - limit[i]`, `balance[i] = limit[i]`, and continue with `i+1` and `amount = excess`; if `i` reaches `n`, stop (discard excess). For type‑2, simply read `balance[targetId-1]`. Edge cases: `amount` may be zero or negative? The problem statement says non-negative limits and typical amounts are positive, but the code should handle zero gracefully. If `targetId` is out of range, we can ignore or treat as no‑op, though for simplicity we assume valid inputs. The algorithm is O(n + q * L) worst case if each query overflows through the whole chain, but in practice with typical limits it’s O(n + q). Memory is O(n). We must ensure the function returns the answers in order.
