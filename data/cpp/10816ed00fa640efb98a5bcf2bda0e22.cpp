Write a C++ function that takes a vector of strings (a vocabulary) and a hash function (implemented as a `std::function<unsigned int(const std::string&)>`), and returns a `std::map<unsigned int, unsigned int>` that counts the number of strings that produce each hash value modulo 30000. The function should handle empty input by returning an empty map, and should treat hash collisions correctly by incrementing the count for the same bucket. The hash function is guaranteed to return a non-negative value, but the final bucket index must be computed as `hash % 30000`. The function must be `const`-correct with respect to the input vector and should not modify the hash function.

#include <cassert>
#include <map>
#include <vector>
#include <string>
#include <functional>

// Dummy hash functions for testing.
unsigned int hashA(const std::string& s) { 
    unsigned int h = 0;
    for (char c : s) h = h * 31 + c;
    return h;
}

unsigned int hashB(const std::string& s) {
    return s.empty() ? 0 : static_cast<unsigned int>(s[0]);
}

// The function under test (must be declared before use or included from header).
// For demonstration, we include the solution here.
std::map<unsigned int, unsigned int> countHashBuckets(
    const std::vector<std::string>& vocabulary,
    const std::function<unsigned int(const std::string&)>& hashFunc)
{
    std::map<unsigned int, unsigned int> bucketCounts;
    const unsigned int tableSize = 30000;

    for (const auto& word : vocabulary) {
        unsigned int bucket = hashFunc(word) % tableSize;
        ++bucketCounts[bucket];
    }

    return bucketCounts;
}

int main() {
    // Empty input.
    std::vector<std::string> empty;
    assert(countHashBuckets(empty, hashA).empty());

    // Single word.
    std::vector<std::string> one = {"hello"};
    auto result1 = countHashBuckets(one, hashA);
    unsigned int helloHash = hashA("hello") % 30000;
    assert(result1.size() == 1);
    assert(result1[helloHash] == 1);

    // Duplicates and collisions.
    std::vector<std::string> dup = {"abc", "abc", "def"};
    auto result2 = countHashBuckets(dup, hashA);
    unsigned int abcHash = hashA("abc") % 30000;
    unsigned int defHash = hashA("def") % 30000;
    assert(result2[abcHash] == 2);
    assert(result2[defHash] == 1);
    assert(result2.size() == 2);

    // Using a different hash function that causes known collisions.
    std::vector<std::string> collide = {"a", "b", "c"};
    auto result3 = countHashBuckets(collide, hashB);
    // hashB returns ASCII of first char: 'a'=97, 'b'=98, 'c'=99, all < 30000.
    assert(result3.size() == 3);
    assert(result3[97] == 1);
    assert(result3[98] == 1);
    assert(result3[99] == 1);

    // Verification with a larger list and check total count matches input size.
    std::vector<std::string> many;
    for (int i = 0; i < 100; ++i) many.push_back("word" + std::to_string(i));
    auto result4 = countHashBuckets(many, hashA);
    unsigned int total = 0;
    for (const auto& kv : result4) total += kv.second;
    assert(total == 100);

    return 0;
}

#include <map>
#include <vector>
#include <string>
#include <functional>

// Count how many strings fall into each hash bucket modulo 30000.
std::map<unsigned int, unsigned int> countHashBuckets(
    const std::vector<std::string>& vocabulary,
    const std::function<unsigned int(const std::string&)>& hashFunc)
{
    std::map<unsigned int, unsigned int> bucketCounts;
    const unsigned int tableSize = 30000;

    for (const auto& word : vocabulary) {
        unsigned int bucket = hashFunc(word) % tableSize;
        ++bucketCounts[bucket];
    }

    return bucketCounts;
}

// The solution iterates over each string in the input vector, computes the bucket index by calling the provided hash function and taking the modulus with 30000, then increments the count in the map for that bucket. Using `std::map` ensures ordered output (by bucket index), which is useful for deterministic behavior. Edge cases include an empty vector (returns empty map), repeated strings (each repeated string still increments the count), and strings that hash to the same bucket (counts accumulate). The hash function is called exactly once per string, so the total time complexity is O(n * H), where H is the time to hash a single string (typically O(L) for string length L), and the space complexity is O(B), where B is the number of distinct buckets actually used (at most min(n, 30000)).
