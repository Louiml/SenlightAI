Write a C++ function `std::string queryPokemon(int N, int M, const std::vector<std::string>& names, const std::vector<std::string>& queries)` that simulates a Pokédex lookup system. The function receives `N` Pokémon names (each a non-empty string of letters only, no spaces) in the order they appear in a directory (indexed starting at 1), and `M` queries. Each query is either a name (letters only) or a positive integer string (digits only, no leading zeros unless it's "0" which never appears because indices start at 1). For each query, if it's a name, the output must be the corresponding index (1-based). If it's a digit string, the output must be the name at that index. The return value must be a single string containing all `M` answers, each on its own line (i.e., separated by `\n`). The input constraints ensure all queries are valid: names exist in the directory, and indices are within `[1, N]`. Note that the function should not modify its input parameters, and must be safe for large `N` and `M` (each up to 100,000) — use efficient lookup, not linear scans.

// The solution uses two hash maps (or `std::unordered_map` for average O(1) lookup, but the reference solution below uses `std::map` with O(log N) to stay closer to the original snippet's style; either is acceptable. The key idea is to build a bidirectional mapping: one from name to index, and one from index to name. For each query, check the first character of the query string: if it is a digit (using `isdigit` on the first char), convert the whole string to an integer with `std::stoi`, then look up that index in the index-to-name map. If it is a letter, look up the name in the name-to-index map. Append the result to an output string, adding `\n` after each answer (including the last one, to match typical output). Edge cases: queries may be given as digit strings that exceed `int` range? The problem states indices are valid and ≤ N ≤ 100,000, so `int` is safe. The `isdigit` check must be done carefully: `std::isdigit` requires a cast to `unsigned char` to avoid undefined behavior for negative `char` values, but since the input only contains ASCII letters/digits, it's fine. Time complexity: O((N+M) log N) if using `std::map`, or O(N+M) average with `std::unordered_map`. Space: O(N) for the two maps plus O(M) for the output string (the answers themselves). The function returns a string, so memory for that is unavoidable. The approach is straightforward and handles all cases without ambiguity.

#include <string>
#include <vector>
#include <map>
#include <cctype>

// Simulates a Pokédex lookup: given N names and M queries (names or indices),
// returns a newline-separated string of answers.
std::string queryPokemon(int N, int M,
                         const std::vector<std::string>& names,
                         const std::vector<std::string>& queries) {
    std::map<std::string, int> nameToIndex;
    std::map<int, std::string> indexToName;

    for (int i = 0; i < N; ++i) {
        nameToIndex[names[i]] = i + 1; // 1-based index
        indexToName[i + 1] = names[i];
    }

    std::string result;
    for (const std::string& q : queries) {
        if (std::isdigit(static_cast<unsigned char>(q[0]))) {
            // Query is an index
            int idx = std::stoi(q);
            result += indexToName.at(idx);
        } else {
            // Query is a name
            result += std::to_string(nameToIndex.at(q));
        }
        result += '\n';
    }
    return result;
}

#include <cassert>
#include <string>
#include <vector>

// Forward declaration for the solution function
std::string queryPokemon(int N, int M,
                         const std::vector<std::string>& names,
                         const std::vector<std::string>& queries);

int main() {
    // Example 1: Basic mixed queries
    std::vector<std::string> names1 = {"Bulbasaur", "Charmander", "Squirtle"};
    std::vector<std::string> queries1 = {"2", "Bulbasaur", "3", "Charmander"};
    assert(queryPokemon(3, 4, names1, queries1) == "Charmander\n1\nSquirtle\n2\n");

    // Example 2: Single Pokémon, multiple queries
    std::vector<std::string> names2 = {"Pikachu"};
    std::vector<std::string> queries2 = {"1", "Pikachu", "Pikachu"};
    assert(queryPokemon(1, 3, names2, queries2) == "Pikachu\n1\n1\n");

    // Example 3: Larger test with 5 names
    std::vector<std::string> names3 = {"a", "b", "c", "d", "e"};
    std::vector<std::string> queries3 = {"5", "a", "3", "e", "1"};
    assert(queryPokemon(5, 5, names3, queries3) == "e\n1\nc\ne\na\n");

    // Example 4: All queries are names
    std::vector<std::string> names4 = {"Mew", "Mewtwo"};
    std::vector<std::string> queries4 = {"Mewtwo", "Mew"};
    assert(queryPokemon(2, 2, names4, queries4) == "2\n1\n");

    // Example 5: All queries are indices
    std::vector<std::string> names5 = {"x", "y", "z"};
    std::vector<std::string> queries5 = {"2", "1", "3"};
    assert(queryPokemon(3, 3, names5, queries5) == "y\nx\nz\n");

    // Example 6: Ensure empty queries vector works
    std::vector<std::string> names6 = {"only"};
    std::vector<std::string> queries6 = {};
    assert(queryPokemon(1, 0, names6, queries6) == "");

    // Example 7: Repeated names in queries
    std::vector<std::string> names7 = {"aaa", "bbb"};
    std::vector<std::string> queries7 = {"1", "bbb", "1", "aaa"};
    assert(queryPokemon(2, 4, names7, queries7) == "aaa\n2\naaa\n1\n");

    // Example 8: Index 10 with 10 Pokémon
    std::vector<std::string> names8 = {"p1", "p2", "p3", "p4", "p5", "p6", "p7", "p8", "p9", "p10"};
    std::vector<std::string> queries8 = {"10", "p1"};
    assert(queryPokemon(10, 2, names8, queries8) == "p10\n1\n");

    return 0;
}
