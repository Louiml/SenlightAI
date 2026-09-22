// Write a C++ function named `findDDDByCity` that takes a `const std::string&` representing a city name and returns its corresponding Brazilian DDD (direct dialing distance) code as a string. The mapping is: "Brasilia" → "61", "Salvador" → "71", "Sao Paulo" → "11", "Rio de Janeiro" → "21", "Juiz de Fora" → "32", "Campinas" → "19", "Vitoria" → "27", "Belo Horizonte" → "31", "Santa Rita do Sapucai" → "35". If the input city does not match any of these exactly (case-sensitive, no trimming), return the string `"DDD nao cadastrado"`. The function must not read from standard input or output; it should simply return the result string.

// The solution is a straightforward lookup table using a sequence of string comparisons. Since the problem explicitly lists a fixed set of city-to-DDD mappings, an unordered_map or a vector of pairs could be used, but a simple chain of `if`/`else if` with `==` comparisons is clear and efficient. Key considerations: the comparison must be exact and case-sensitive, so no whitespace trimming or case conversion is performed. The function should handle an empty string or unknown input gracefully by returning the fallback message. Time complexity is \(O(1)\) per lookup (constant number of comparisons, at most 9), and space complexity is \(O(1)\) auxiliary, as the input string is passed by const reference and the returned string is a literal or static string.

#include <string>

// Returns the DDD code for a given city name, or "DDD nao cadastrado" if not found.
std::string findDDDByCity(const std::string& city) {
    if (city == "Brasilia") return "61";
    if (city == "Salvador") return "71";
    if (city == "Sao Paulo") return "11";
    if (city == "Rio de Janeiro") return "21";
    if (city == "Juiz de Fora") return "32";
    if (city == "Campinas") return "19";
    if (city == "Vitoria") return "27";
    if (city == "Belo Horizonte") return "31";
    if (city == "Santa Rita do Sapucai") return "35";
    return "DDD nao cadastrado";
}

#include <cassert>
#include <string>

// Declaration (or include the solution header here)
std::string findDDDByCity(const std::string& city);

int main() {
    assert(findDDDByCity("Brasilia") == "61");
    assert(findDDDByCity("Salvador") == "71");
    assert(findDDDByCity("Sao Paulo") == "11");
    assert(findDDDByCity("Rio de Janeiro") == "21");
    assert(findDDDByCity("Juiz de Fora") == "32");
    assert(findDDDByCity("Campinas") == "19");
    assert(findDDDByCity("Vitoria") == "27");
    assert(findDDDByCity("Belo Horizonte") == "31");
    assert(findDDDByCity("Santa Rita do Sapucai") == "35");
    // Edge cases
    assert(findDDDByCity("brasilia") == "DDD nao cadastrado"); // case-sensitive
    assert(findDDDByCity("") == "DDD nao cadastrado");          // empty string
    assert(findDDDByCity("Sao Paulo ") == "DDD nao cadastrado"); // trailing space
    return 0;
}
