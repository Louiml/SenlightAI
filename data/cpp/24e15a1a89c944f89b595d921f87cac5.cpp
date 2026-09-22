Write a C++ function named `pickRandomGreeting` that takes a vector of strings (names) by const reference and returns a randomly selected name from the vector, prefixed with the greeting "Hello, " and followed by an exclamation mark (e.g., "Hello, Alice!"). If the vector is empty, the function should return the string "No names available." The function must use a uniform random distribution (e.g., `std::mt19937` with `std::uniform_int_distribution`) for randomness, and must not modify the input vector or use global state. The returned string must be exactly as described, with proper spacing and punctuation.
// The solution requires generating an index uniformly at random within the range `[0, size-1]` if the vector is non-empty. Use a modern random engine seeded with a non-deterministic source (e.g., `std::random_device`) to avoid `rand()` pitfalls like bias and poor distribution. The function must be `const`-correct: accept the vector by `const std::vector<std::string>&` to prevent copying and modification. Edge cases: (1) empty vector → return the fixed error message; (2) single element → always return that element (random index will be 0); (3) string may contain spaces or special characters → simply concatenate as-is. Time complexity is O(1) for random selection plus O(L) to concatenate the selected name (where L is the name length), and O(1) auxiliary space aside from the returned string. The use of `std::uniform_int_distribution` ensures each index has equal probability, avoiding modulo bias.
#include <string>
#include <vector>
#include <random>

// Returns a random greeting using one name from the provided vector.
// If the vector is empty, returns a fixed fallback message.
std::string pickRandomGreeting(const std::vector<std::string>& names) {
    if (names.empty()) {
        return "No names available.";
    }

    // Seed a Mersenne Twister engine with a non-deterministic source.
    static std::mt19937 engine(std::random_device{}());
    std::uniform_int_distribution<std::size_t> dist(0, names.size() - 1);

    std::size_t index = dist(engine);
    return "Hello, " + names[index] + "!";
}
#include <cassert>
#include <string>
#include <vector>

// The solution function is declared above; include its definition here.

int main() {
    // Empty vector case.
    std::vector<std::string> empty;
    assert(pickRandomGreeting(empty) == "No names available.");

    // Single element case: always the same greeting.
    std::vector<std::string> single = {"Alice"};
    assert(pickRandomGreeting(single) == "Hello, Alice!");

    // Multiple elements: run many times and verify output format and membership.
    std::vector<std::string> names = {"Bob", "Carol", "David", "Eve"};
    for (int i = 0; i < 1000; ++i) {
        std::string result = pickRandomGreeting(names);
        // Check prefix and suffix.
        assert(result.rfind("Hello, ", 0) == 0);
        assert(result.back() == '!');
        // Extract the name part and verify it is in the original vector.
        std::string extracted = result.substr(7, result.size() - 8); // remove "Hello, " and "!"
        bool found = false;
        for (const auto& n : names) {
            if (n == extracted) {
                found = true;
                break;
            }
        }
        assert(found);
    }

    // Check that the function does not modify the input vector (const correctness).
    std::vector<std::string> original = {"Zoe", "Yara"};
    std::vector<std::string> copy = original;
    pickRandomGreeting(original);
    assert(original == copy);

    // Edge case: names with spaces and special characters.
    std::vector<std::string> special = {"Jean-Luc Picard", "A  B"};
    std::string result = pickRandomGreeting(special);
    assert(result == "Hello, Jean-Luc Picard!" || result == "Hello, A  B!");

    return 0;
}
