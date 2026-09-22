// You are given a list of heroes, each described by an integer power level, an integer agility, and a string name. Two heroes are considered equal only if all three attributes match exactly. Write a standalone C++ function named `buildAndCheckTreap` that takes a vector of heroes (as `std::vector<Hero>`), inserts all of them into a `Treap<Hero>` container (as provided in the snippet), then returns a `std::pair<size_t, bool>` where the first element is the total number of distinct heroes (i.e., the size of the treap after insertion, considering duplicates as overwrites or ignoring them—choose to ignore duplicates so only unique heroes are stored), and the second element is `true` if a query hero (provided as a separate parameter) is present in the treap, `false` otherwise. Additionally, the function must return the hero at index `k` (where `k` is provided) in the treap’s sorted order (defined by the `Treap` class ordering—assume ordering is by power first, then agility, then name lexicographically) as a `std::optional<Hero>`; if `k` is out of bounds, return `std::nullopt`. The function signature should be: `std::tuple<size_t, bool, std::optional<Hero>> buildAndCheckTreap(const std::vector<Hero>& heroes, const Hero& query, size_t k);`. You may assume the `Hero` class has a public constructor `Hero(int power, int agility, const std::string& name)` and a public method `std::string toString()` that returns a formatted string for output. The `Treap` class provides methods: `emplace(args...)`, `size()`, `at(size_t index)` (which returns a const reference), `search(const T& value)` returning a bool, and `new_Treap(size_t count)` returning a new treap of the first `count` elements (but you will not need `new_Treap` for this task). Write the function using appropriate `const` correctness, and do not modify the `Hero` or `Treap` classes; just use them as given. Provide a self-contained implementation.
#include <cassert>
#include <vector>
#include <optional>
#include <tuple>
#include "Hero.h"
#include "Tree.h"

int main() {
    // Test 1: Basic scenario from snippet
    {
        std::vector<Hero> heroes = {
            Hero(200, 5, "Petya"),
            Hero(200, 4, "Yan"),
            Hero(100, 3, "Kolya"),
            Hero(200, 2, "Alex"),
            Hero(88, 1, "Vlad")
        };
        Hero query(100, 3, "Kolya");
        auto result = buildAndCheckTreap(heroes, query, 3);
        assert(std::get<0>(result) == 5); // all distinct
        assert(std::get<1>(result) == true); // query found
        assert(std::get<2>(result).has_value()); // index 3 exists
        // According to sorting by power, then agility, then name:
        // (88,1,Vlad), (100,3,Kolya), (200,2,Alex), (200,4,Yan), (200,5,Petya)
        // So index 3 is Yan
        assert(std::get<2>(result)->name == "Yan");
    }

    // Test 2: Duplicate heroes are ignored
    {
        std::vector<Hero> heroes = {
            Hero(10, 1, "A"),
            Hero(10, 1, "A"),
            Hero(20, 2, "B")
        };
        Hero query(10, 1, "A");
        auto result = buildAndCheckTreap(heroes, query, 0);
        assert(std::get<0>(result) == 2); // only 2 distinct
        assert(std::get<1>(result) == true);
        assert(std::get<2>(result).has_value());
        assert(std::get<2>(result)->name == "A");
    }

    // Test 3: Query not found
    {
        std::vector<Hero> heroes = {Hero(1, 1, "X")};
        Hero query(2, 2, "Y");
        auto result = buildAndCheckTreap(heroes, query, 0);
        assert(std::get<0>(result) == 1);
        assert(std::get<1>(result) == false);
        assert(std::get<2>(result).has_value());
    }

    // Test 4: Index out of bounds
    {
        std::vector<Hero> heroes = {Hero(5, 1, "A"), Hero(3, 2, "B")};
        Hero query(3, 2, "B");
        auto result = buildAndCheckTreap(heroes, query, 10);
        assert(std::get<0>(result) == 2);
        assert(std::get<1>(result) == true);
        assert(!std::get<2>(result).has_value()); // nullopt
    }

    // Test 5: Empty input
    {
        std::vector<Hero> heroes = {};
        Hero query(1, 1, "A");
        auto result = buildAndCheckTreap(heroes, query, 0);
        assert(std::get<0>(result) == 0);
        assert(std::get<1>(result) == false);
        assert(!std::get<2>(result).has_value());
    }

    // Test 6: Index at last element
    {
        std::vector<Hero> heroes = {Hero(3, 3, "C"), Hero(1, 1, "A"), Hero(2, 2, "B")};
        Hero query(2, 2, "B");
        auto result = buildAndCheckTreap(heroes, query, 2);
        assert(std::get<0>(result) == 3);
        assert(std::get<1>(result) == true);
        assert(std::get<2>(result)->name == "C"); // sorted by power: A(1), B(2), C(3)
    }

    return 0;
}
#include <vector>
#include <optional>
#include <tuple>
#include <string>
#include "Tree.h"
#include "Hero.h"

// Build a treap from heroes, check query presence, and retrieve element at index k.
std::tuple<size_t, bool, std::optional<Hero>> buildAndCheckTreap(
    const std::vector<Hero>& heroes,
    const Hero& query,
    size_t k)
{
    Treap<Hero> tree;
    for (const auto& h : heroes) {
        // Insert only if not already present (distinct heroes only)
        if (!tree.search(h)) {
            tree.emplace(h.power, h.agility, h.name); // assuming public fields or getters
        }
    }
    size_t distinctCount = tree.size();
    bool queryFound = tree.search(query);
    
    std::optional<Hero> elementAtK;
    if (k < tree.size()) {
        elementAtK = tree.at(k); // copy from const reference
    }
    
    return {distinctCount, queryFound, elementAtK};
}
*Note: The code above assumes that `Hero` has public fields `power`, `agility`, `name` accessible for `emplace`. If `Hero` only exposes a constructor and getters, adjust the `emplace` call accordingly. The given snippet uses `tree.emplace(200, 5, "Petya")` so assume the constructor takes exactly those three arguments.*
// The solution requires inserting all heroes from the input vector into a `Treap<Hero>` using `emplace`. Since `Treap` may store duplicate values, we must ensure uniqueness. We can check if a hero already exists using `search` before inserting; if it exists, skip insertion. After processing all heroes, the treap size gives the number of distinct heroes. For the query hero, we call `tree.search(query)` which returns a bool. For the element at index `k`, we call `tree.at(k)` but must guard against out-of-range by checking if `k < tree.size()`. If valid, we return the hero (copy it into an `optional`); otherwise return `std::nullopt`. Edge cases: empty input vector results in size 0, query may or may not be present, `k` might be beyond the last index. The complexity is O(m log m) for inserting m distinct heroes (each insertion and search O(log m) on average, assuming treap is balanced), and O(1) for `at` access (since it’s indexed). Space is O(m) for the treap.
