Write a C++ function `registerNames` that takes a sequence of strings (names) and returns a vector of strings representing the registration result for each name, following the rule: if a name appears for the first time, the output is `"OK"`; if it has appeared before, the output is the original name followed by the smallest positive integer `k` (starting from 1) such that `name + to_string(k)` has not yet been registered. Each registration (whether original or with suffix) permanently occupies that exact string. For example, after registering `"abc"`, a second `"abc"` produces `"abc1"`, and a third `"abc"` produces `"abc2"` (since `"abc1"` is already taken). The function should process inputs in order and handle arbitrary lowercase letters and digits.

The problem is a classic "name registration" simulation. The main data structure is a hash map (or `std::unordered_map`) that stores for each base name the next available suffix number (or perhaps a set of all occupied suffixed names). A straightforward approach: use a map from string to `int` count. For each input name:
- If the name is not in the map, we output `"OK"` and insert the name with count 1.
- If the name is in the map, we retrieve the current count `c`, then generate candidate `name + to_string(c)`. But we must also ensure that candidate has not been taken as a base name itself? Actually the rule says: if the name appears before, we find the smallest positive integer `k` such that `name + to_string(k)` has not been registered yet. The count `c` in the map represents how many times the base name has been seen, but that's not enough because `name + to_string(k)` could collide with a previously inserted suffixed name that was itself a base input. For example: input `"a"` → `"OK"`, then `"a"` → candidate `"a1"` (occupies `"a1"`), then input `"a1"` → `"OK"` (since `"a1"` as a base is new), then input `"a"` again → we need smallest `k` such that `"a" + to_string(k)` not used. `"a1"` is used, so try `"a2"` → free, so output `"a2"`. So we cannot just use a count; we must track all occupied strings. A simple solution: maintain a `std::unordered_map<std::string, int>` for the next suffix to try, and a `std::unordered_set<std::string>` of all registered strings. For each input `s`:
- If `s` not in set: add `s`, output `"OK"`.
- Else: let `k = nextSuffix[s]` (initialized to 1 when `s` first inserted). While `s + to_string(k)` is in the set, increment `k`. Then add `s + to_string(k)` to set, output that string, and update `nextSuffix[s] = k+1` (so next time we start from after the used one). This guarantees we find the smallest unused suffix because we only increment when the candidate is already present. Edge cases: names can be up to arbitrary length, and suffix numbers can become large; use `long long` for safety. Time complexity: each insertion is O(L) where L is average string length, and each suffix search may increment `k` multiple times, but in total across all operations, each occupied string causes at most one increment per query, so overall O(total characters + total number of queries). Space: O(total number of registered distinct strings).

#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>

// Register a sequence of names according to the described rule.
// Returns a vector of the registration results for each input name.
std::vector<std::string> registerNames(const std::vector<std::string>& names) {
    std::unordered_set<std::string> registered;
    std::unordered_map<std::string, long long> nextSuffix;
    std::vector<std::string> result;
    result.reserve(names.size());

    for (const std::string& original : names) {
        if (registered.find(original) == registered.end()) {
            // First occurrence of this exact string
            registered.insert(original);
            nextSuffix[original] = 1;
            result.emplace_back("OK");
        } else {
            // Already registered, find the smallest unused suffix
            long long k = nextSuffix[original];
            std::string candidate;
            do {
                candidate = original + std::to_string(k);
                ++k;
            } while (registered.find(candidate) != registered.end());
            
            // k-1 is the suffix we actually used
            nextSuffix[original] = k;  // next time start from the next number
            registered.insert(candidate);
            result.push_back(candidate);
        }
    }
    return result;
}

#include <cassert>
#include <vector>
#include <string>

// Function under test is declared above (or included from solution file).
// Test the registerNames function.
int main() {
    // Basic case
    std::vector<std::string> names1 = {"a", "a", "a"};
    std::vector<std::string> res1 = registerNames(names1);
    assert(res1 == std::vector<std::string>({"OK", "a1", "a2"}));

    // Mix of distinct and repeated
    std::vector<std::string> names2 = {"abc", "abc", "abc1", "abc"};
    std::vector<std::string> res2 = registerNames(names2);
    assert(res2 == std::vector<std::string>({"OK", "abc1", "OK", "abc2"}));

    // Case where suffix collides with a previously registered base name
    std::vector<std::string> names3 = {"x", "x", "x1"};
    std::vector<std::string> res3 = registerNames(names3);
    assert(res3 == std::vector<std::string>({"OK", "x1", "OK"}));

    // After x1 is taken, next x must use x2
    std::vector<std::string> names4 = {"x", "x", "x1", "x"};
    std::vector<std::string> res4 = registerNames(names4);
    assert(res4 == std::vector<std::string>({"OK", "x1", "OK", "x2"}));

    // Multiple different names
    std::vector<std::string> names5 = {"one", "two", "one", "two", "one"};
    std::vector<std::string> res5 = registerNames(names5);
    assert(res5 == std::vector<std::string>({"OK", "OK", "one1", "two1", "one2"}));

    // Empty input
    std::vector<std::string> names6 = {};
    assert(registerNames(names6).empty());

    // Single name
    std::vector<std::string> names7 = {"single"};
    assert(registerNames(names7) == std::vector<std::string>({"OK"}));

    // Names with digits already
    std::vector<std::string> names8 = {"a1", "a1", "a2", "a1"};
    std::vector<std::string> res8 = registerNames(names8);
    // First "a1": OK; second "a1": candidate "a11"; third "a2": OK; fourth "a1": "a11" taken, try "a12"
    assert(res8 == std::vector<std::string>({"OK", "a11", "OK", "a12"}));

    // Large number of repeats to ensure suffix increments correctly
    std::vector<std::string> names9;
    for (int i = 0; i < 5; ++i) names9.push_back("z");
    std::vector<std::string> res9 = registerNames(names9);
    std::vector<std::string> expected9 = {"OK", "z1", "z2", "z3", "z4"};
    assert(res9 == expected9);

    return 0;
}
