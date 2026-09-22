// You are given a list of `n` Pokémon names (1-indexed) and then `m` queries. Each query is either a name or a number. If the query is a number, print the Pokémon name with that index. If the query is a name, print its index. Write a C++ function `std::vector<std::string> processQueries(int n, int m, const std::vector<std::string>& pokemons, const std::vector<std::string>& queries)` that returns the answers in order, one per string (without trailing newlines). The input is guaranteed to be valid: every name is unique, every index exists (between 1 and n), and no name consists solely of digits.

// Use two hash maps: one mapping name→index and one mapping index→name. For each query, check if the first character is a digit (using `isdigit` or comparing to `'0'`/`'9'`). If it is a digit, convert to an integer with `std::stoi` and look up in the index→name map. Otherwise, look up in the name→index map. Edge cases: names may start with letters only (never digits), indices are 1‑based and fit in `int`. Complexity: building both maps takes O(n) time and O(n) space; each query is O(1) average (hash map lookup), so total O(n+m) time and O(n) space.

#include <string>
#include <vector>
#include <unordered_map>
#include <cctype>

// Given a list of pokemon names and a list of queries (each either a name or a 1-indexed number),
// return the answer for each query in the same order.
// A query that is a number returns the corresponding pokemon name.
// A query that is a name returns its 1-indexed position.
std::vector<std::string> processQueries(
    int n,
    int m,
    const std::vector<std::string>& pokemons,
    const std::vector<std::string>& queries)
{
    // Build two maps: name -> index and index -> name.
    std::unordered_map<std::string, int> nameToIndex;
    std::unordered_map<int, std::string> indexToName;
    for (int i = 0; i < n; ++i)
    {
        nameToIndex[pokemons[i]] = i + 1;
        indexToName[i + 1] = pokemons[i];
    }

    std::vector<std::string> results;
    results.reserve(m);
    for (const std::string& q : queries)
    {
        // A query is a number if its first character is a digit.
        if (!q.empty() && std::isdigit(static_cast<unsigned char>(q[0])))
        {
            int idx = std::stoi(q);
            results.push_back(indexToName.at(idx));
        }
        else
        {
            results.push_back(std::to_string(nameToIndex.at(q)));
        }
    }
    return results;
}

#include <cassert>
#include <vector>
#include <string>

int main()
{
    // Example from the original snippet: n=3, m=3
    // names: "Bulbasaur", "Ivysaur", "Venusaur"
    // queries: "Venusaur", "2", "Bulbasaur"
    std::vector<std::string> pokemons = {"Bulbasaur", "Ivysaur", "Venusaur"};
    std::vector<std::string> queries = {"Venusaur", "2", "Bulbasaur"};
    std::vector<std::string> expected = {"3", "Ivysaur", "1"};
    assert(processQueries(3, 3, pokemons, queries) == expected);

    // Single pokemon
    std::vector<std::string> pokemons2 = {"Pikachu"};
    std::vector<std::string> queries2 = {"1", "Pikachu"};
    std::vector<std::string> expected2 = {"Pikachu", "1"};
    assert(processQueries(1, 2, pokemons2, queries2) == expected2);

    // Names that look numeric-like but are not (e.g., "2nd" starts with digit, but we assume valid input)
    // Real numbers: check bounds
    std::vector<std::string> pokemons3 = {"a", "b", "c", "d"};
    std::vector<std::string> queries3 = {"d", "1", "b", "4"};
    std::vector<std::string> expected3 = {"4", "a", "2", "d"};
    assert(processQueries(4, 4, pokemons3, queries3) == expected3);

    // Mixed order
    std::vector<std::string> queries4 = {"3", "a", "2", "c"};
    std::vector<std::string> expected4 = {"c", "1", "b", "3"};
    assert(processQueries(4, 4, pokemons3, queries4) == expected4);

    // Empty queries would never happen in valid input, but just check with zero queries.
    std::vector<std::string> queries5;
    std::vector<std::string> expected5;
    assert(processQueries(3, 0, pokemons, queries5) == expected5);

    // Edge: single query that is a name
    std::vector<std::string> queries6 = {"Ivysaur"};
    std::vector<std::string> expected6 = {"2"};
    assert(processQueries(3, 1, pokemons, queries6) == expected6);

    // Edge: single query that is a number
    std::vector<std::string> queries7 = {"3"};
    std::vector<std::string> expected7 = {"Venusaur"};
    assert(processQueries(3, 1, pokemons, queries7) == expected7);

    return 0;
}
