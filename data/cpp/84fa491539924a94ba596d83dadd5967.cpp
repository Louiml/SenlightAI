// Define a C++ function named `applyConversion` that simulates a simplified two-connector automated market maker exchange. Given an exchange state with a base connector (e.g., "USD") and a quote connector (e.g., "BTC"), each having a balance and a weight (between 0 and 1), and given a user name, an input asset (amount and symbol), and a minimum output asset (symbol and threshold amount), the function must perform a conversion that may involve one or two steps: if the input symbol is a connector symbol (not the exchange token), convert it to exchange tokens using the appropriate connector's `convert_to_exchange` formula; if the input symbol is the exchange token, convert it to the target connector symbol using `convert_from_exchange`. If after this first step the output symbol does not match the minimum output symbol, a second conversion is performed from the intermediate output to the target symbol. The function must return a new exchange state with updated connector balances, updated supply, and updated user balances (add output to user, subtract input from user). It must throw a `std::runtime_error` for invalid symbols or if the final output amount is less than the minimum output amount (using an exact comparison with a small epsilon tolerance). The exchange state is defined with `token_type` as `double` and `real_type` as `long double`; include the necessary `connector` methods and data structures as described, but do not include a `main` function in the solution. The function must preserve const-correctness: take the current state as a `const exchange_state&` and return a new state by value. The connector conversion formulas are: `E = R * (1 - pow(1 + T/S, F))` for converting input to exchange tokens (where `E` is issued exchange tokens, `R` is current supply, `S` is balance plus input amount, `T` is input amount, `F` is weight) and `out = S * (pow(1 + E/R, 1/F) - 1)` for converting exchange tokens to connector tokens (where `out` is output amount, `S` is connector balance, `R` is supply minus input exchange tokens, `F` is weight, `E` is input exchange token amount). Ensure that after each conversion, balances and supply are updated correctly. Include a helper `eosio_assert` that throws `std::runtime_error` with a message on failure. The user balances are stored in a `std::map<balance_key, token_type>`, where `balance_key` is a struct with `account_name` (string) and `symbol_type` (string) and provides `operator<` for map ordering. The function must handle the scenario where the input and minimum output symbols are the same (throw an error), and where the input is already the exchange token but the minimum output is not a valid connector (throw an error). The function must be deterministic and use `std::pow` from `<cmath>`.

// The core algorithm processes a conversion in a maximum of two steps, mirroring the original snippet's recursive `convert` function but iterative for clarity. First, validate that input and target symbols differ. Then determine the initial output asset: if the input symbol equals the exchange token ("EXC"), perform a `convert_from_exchange` on the connector that matches the target symbol; otherwise, perform a `convert_to_exchange` on the connector that matches the input symbol. Each connector method updates the connector's balance and the state's supply, and returns an asset with the exchange token symbol (for input-to-exchange) or the connector's symbol (for exchange-to-output). After the first step, if the intermediate output's symbol does not equal the target symbol, perform a second conversion: if the intermediate symbol is the exchange token, convert to the target using the corresponding connector's `convert_from_exchange`; otherwise, convert to exchange tokens (which is unneeded because if input was a connector and target is also a connector, the first step would output exchange tokens, so the second step will always be from exchange to connector, but the code can handle generic cases). Finally, update the user's balance by adding the final output amount and subtracting the original input amount. Then check if the final output amount is less than the minimum output amount (with a tolerance of 1e-9 for floating-point noise) and throw an error if so. Important edge cases: invalid symbols (throw), same symbols input and target (throw), zero or negative amounts are not explicitly prohibited but may lead to issues; the function should allow them for simplicity. Time complexity is O(log N) for map operations (balance lookup/update) plus constant-time arithmetic; space complexity is O(1) beyond the copied state and temporary variables. The solution must be self-contained with no external dependencies beyond standard headers.

#include <map>
#include <string>
#include <cmath>
#include <stdexcept>
#include <utility>
#include <cstdint>

using real_type = long double;
using token_type = double;
using account_name = std::string;
using symbol_type = std::string;

static const symbol_type exchange_symbol = "EXC";

struct asset {
    token_type amount = 0;
    symbol_type symbol;
};

struct balance_key {
    account_name owner;
    symbol_type symbol;

    friend bool operator<(const balance_key& a, const balance_key& b) {
        return std::tie(a.owner, a.symbol) < std::tie(b.owner, b.symbol);
    }
    friend bool operator==(const balance_key& a, const balance_key& b) {
        return std::tie(a.owner, a.symbol) == std::tie(b.owner, b.symbol);
    }
};

struct exchange_state;

struct connector {
    asset balance;
    real_type weight = 0.5;
    token_type total_lent = 0;
    token_type total_borrowed = 0;
    token_type total_available_to_lend = 0;
    token_type interest_pool = 0;

    asset convert_to_exchange(exchange_state& ex, const asset& input);
    asset convert_from_exchange(exchange_state& ex, const asset& input);
};

struct exchange_state {
    token_type supply = 0;
    symbol_type symbol = exchange_symbol;
    connector base;
    connector quote;
    std::map<balance_key, token_type> output;

    void transfer(account_name user, asset q) {
        output[balance_key{user, q.symbol}] += q.amount;
    }
};

// Helper to throw on assertion failure
void eosio_assert(bool test, const std::string& msg) {
    if (!test) throw std::runtime_error(msg);
}

// Convert input connector asset to exchange tokens
asset connector::convert_to_exchange(exchange_state& ex, const asset& input) {
    real_type R(ex.supply);
    real_type S(balance.amount + input.amount);
    real_type F(weight);
    real_type T(input.amount);
    real_type ONE(1.0);

    real_type E = R * (ONE - std::pow(ONE + T / S, F));
    token_type issued = -E;  // E is negative, so issued is positive

    ex.supply += issued;
    balance.amount += input.amount;

    return asset{issued, exchange_symbol};
}

// Convert exchange tokens to connector asset
asset connector::convert_from_exchange(exchange_state& ex, const asset& input) {
    real_type R(ex.supply - input.amount);
    real_type S(balance.amount);
    real_type F(weight);
    real_type E(input.amount);
    real_type ONE(1.0);

    real_type out = S * (std::pow(ONE + E / R, ONE / F) - ONE);

    ex.supply -= input.amount;
    balance.amount -= token_type(out);

    return asset{token_type(out), balance.symbol};
}

// Main conversion function
exchange_state applyConversion(const exchange_state& current,
                               account_name user,
                               asset input,
                               asset min_output) {
    eosio_assert(min_output.symbol != input.symbol, "cannot convert");

    exchange_state result(current);
    asset initial_output;

    // Step 1: convert input to exchange tokens if input is a connector, or to target connector if input is exchange token
    if (input.symbol == exchange_symbol) {
        if (min_output.symbol == result.base.balance.symbol) {
            initial_output = result.base.convert_from_exchange(result, input);
        } else if (min_output.symbol == result.quote.balance.symbol) {
            initial_output = result.quote.convert_from_exchange(result, input);
        } else {
            eosio_assert(false, "invalid symbol");
        }
    } else if (input.symbol == result.base.balance.symbol) {
        initial_output = result.base.convert_to_exchange(result, input);
    } else if (input.symbol == result.quote.balance.symbol) {
        initial_output = result.quote.convert_to_exchange(result, input);
    } else {
        eosio_assert(false, "invalid symbol");
    }

    asset final_output = initial_output;

    // Step 2: if needed, convert again from exchange token to target connector
    if (final_output.symbol != min_output.symbol) {
        if (final_output.symbol == exchange_symbol) {
            if (min_output.symbol == result.base.balance.symbol) {
                final_output = result.base.convert_from_exchange(result, final_output);
            } else if (min_output.symbol == result.quote.balance.symbol) {
                final_output = result.quote.convert_from_exchange(result, final_output);
            } else {
                eosio_assert(false, "invalid symbol");
            }
        } else {
            // If final_output is a connector and doesn't match target, it can only be that target is exchange token,
            // but that case is already handled implicitly; for completeness, convert to exchange (though this won't
            // be reached in the given scenario because first step with a connector input yields exchange token,
            // and second step from exchange to target connector is the only needed path)
            if (min_output.symbol == exchange_symbol) {
                if (final_output.symbol == result.base.balance.symbol) {
                    final_output = result.base.convert_to_exchange(result, final_output);
                } else if (final_output.symbol == result.quote.balance.symbol) {
                    final_output = result.quote.convert_to_exchange(result, final_output);
                } else {
                    eosio_assert(false, "invalid symbol");
                }
            } else {
                eosio_assert(false, "cannot reach target");
            }
        }
    }

    // Update user balances
    result.output[balance_key{user, final_output.symbol}] += final_output.amount;
    result.output[balance_key{user, input.symbol}] -= input.amount;

    // Check minimum output
    if (final_output.amount + 1e-9 < min_output.amount) {
        eosio_assert(false, "output below minimum");
    }

    return result;
}

#include <cassert>
#include <cmath>
#include <iostream>

// Copy the solution code here (headers, structs, and function) for the test

int main() {
    // Helper to build an initial state
    auto makeState = []() {
        exchange_state s;
        s.supply = 100000;
        s.base.balance.amount = 10000;
        s.base.balance.symbol = "USD";
        s.base.weight = 0.5;
        s.quote.balance.amount = 10000;
        s.quote.balance.symbol = "BTC";
        s.quote.weight = 0.5;
        return s;
    };

    // Test 1: simple USD to BTC
    {
        auto st = makeState();
        auto result = applyConversion(st, "alice", asset{100, "USD"}, asset{0, "BTC"});
        assert(result.supply > 100000);
        assert(result.base.balance.amount > 10000);
        assert(result.quote.balance.amount == 10000);
        assert(result.output[balance_key{"alice", "BTC"}] > 0);
        assert(result.output[balance_key{"alice", "USD"}] == -100);
    }

    // Test 2: simple BTC to USD
    {
        auto st = makeState();
        auto result = applyConversion(st, "bob", asset{100, "BTC"}, asset{0, "USD"});
        assert(result.supply > 100000);
        assert(result.quote.balance.amount > 10000);
        assert(result.base.balance.amount == 10000);
        assert(result.output[balance_key{"bob", "USD"}] > 0);
        assert(result.output[balance_key{"bob", "BTC"}] == -100);
    }

    // Test 3: two-step conversion (USD -> EXC -> BTC) via a single call with min target BTC
    {
        auto st = makeState();
        auto result = applyConversion(st, "carol", asset{100, "USD"}, asset{50, "BTC"});
        assert(result.output[balance_key{"carol", "BTC"}] > 50);
        assert(result.output[balance_key{"carol", "USD"}] == -100);
        assert(std::abs(result.base.balance.amount - 10100) < 1e-6);
        assert(result.supply > 100000);
    }

    // Test 4: same symbol input and minimum output throws
    {
        auto st = makeState();
        bool threw = false;
        try {
            applyConversion(st, "dave", asset{100, "USD"}, asset{0, "USD"});
        } catch (const std::runtime_error&) {
            threw = true;
        }
        assert(threw);
    }

    // Test 5: invalid input symbol throws
    {
        auto st = makeState();
        bool threw = false;
        try {
            applyConversion(st, "eve", asset{100, "EUR"}, asset{0, "USD"});
        } catch (const std::runtime_error&) {
            threw = true;
        }
        assert(threw);
    }

    // Test 6: insufficient output triggers assertion (set min higher than plausible)
    {
        auto st = makeState();
        bool threw = false;
        try {
            applyConversion(st, "frank", asset{1, "USD"}, asset{10000, "BTC"});
        } catch (const std::runtime_error&) {
            threw = true;
        }
        assert(threw);
    }

    // Test 7: converting exchange token to connector
    {
        auto st = makeState();
        st.supply = 1000;
        auto result = applyConversion(st, "grace", asset{10, "EXC"}, asset{0, "USD"});
        assert(result.base.balance.amount > 10000);
        assert(result.supply < 1000);
        assert(result.output[balance_key{"grace", "USD"}] > 0);
        assert(result.output[balance_key{"grace", "EXC"}] == -10);
    }

    // Test 8: state immutability of original
    {
        auto st = makeState();
        auto original_supply = st.supply;
        auto original_base = st.base.balance.amount;
        applyConversion(st, "heidi", asset{10, "USD"}, asset{0, "BTC"});
        assert(st.supply == original_supply);
        assert(st.base.balance.amount == original_base);
        assert(st.output.empty());
    }

    // Test 9: deterministic repeatability
    {
        auto st = makeState();
        auto r1 = applyConversion(st, "ivan", asset{100, "USD"}, asset{0, "BTC"});
        auto r2 = applyConversion(st, "ivan", asset{100, "USD"}, asset{0, "BTC"});
        assert(r1.supply == r2.supply);
        assert(r1.base.balance.amount == r2.base.balance.amount);
        assert(r1.quote.balance.amount == r2.quote.balance.amount);
    }

    // Test 10: small trade preserves positive output
    {
        auto st = makeState();
        auto result = applyConversion(st, "judy", asset{0.001, "USD"}, asset{0, "BTC"});
        assert(result.output[balance_key{"judy", "BTC"}] > 0.0);
        assert(result.output[balance_key{"judy", "BTC"}] < 0.01);
    }

    std::cout << "All tests passed.\n";
    return 0;
}
