Given a text corpus processed word by word, write a C++ function that builds a hash table using chaining to count word frequencies. The function should accept a vector of raw input strings and return a vector of pairs (word, count) for all unique words that pass a preprocessing filter: words must have length greater than 5, contain only alphabetic characters (no digits or punctuation), and be stored in lowercase. The hash table must use a fixed size of 101 and the BKDR hash function with seed 1313. The function should handle collisions gracefully by appending new nodes to the chain. The returned vector should be sorted alphabetically by word.
// The solution implements a simple hash table with separate chaining. Each bucket is a linked list of nodes containing a word, its count, and a pointer to the next node. For each input string, we preprocess it: remove leading non-alphabetic characters, remove trailing non-alphabetic characters, reject if it becomes length ≤ 5, reject if it contains any digit, convert all characters to lowercase, and finally keep only if length > 5. The hash function iterates over each character, computing `hash = hash * 1313 + str[i]`, then takes modulo 101. To insert, we compute the bucket index, then traverse the chain. If the word is found, increment its count. If not found, create a new node at the end of the chain. After processing all inputs, we traverse all buckets, collect (word, count) pairs, and sort them alphabetically. Edge cases include empty input vector (returns empty result), strings that become empty after preprocessing, strings with mixed alphanumeric characters, and duplicate words appearing multiple times. Time complexity is O(N) average for inserting N words, and O(U log U) for sorting U unique words. Space complexity is O(U) for the hash table plus O(U) for the output vector.
#include <vector>
#include <string>
#include <cctype>
#include <algorithm>

struct HashNode {
    std::string word;
    int count;
    HashNode* next;
    HashNode(const std::string& w, int c) : word(w), count(c), next(nullptr) {}
};

// Preprocess a raw word: strip non-letters from ends, reject digits, convert to lowercase.
bool preprocessWord(const std::string& raw, std::string& processed) {
    if (raw.length() <= 5) return false;

    size_t start = 0;
    while (start < raw.length() && !std::isalpha(static_cast<unsigned char>(raw[start]))) {
        start++;
    }
    if (raw.length() - start <= 5) return false;

    size_t end = raw.length() - 1;
    while (end > start && !std::isalpha(static_cast<unsigned char>(raw[end]))) {
        end--;
    }
    if (end - start + 1 <= 5) return false;

    for (size_t i = start; i <= end; i++) {
        if (std::isdigit(static_cast<unsigned char>(raw[i]))) {
            return false;
        }
    }

    processed = raw.substr(start, end - start + 1);
    for (char& c : processed) {
        c = std::tolower(static_cast<unsigned char>(c));
    }
    return processed.length() > 5;
}

// BKDR hash function.
unsigned int hashF(const std::string& str, int tableSize) {
    unsigned int seed = 1313;
    unsigned int hash = 0;
    for (char c : str) {
        hash = (hash * seed) + static_cast<unsigned char>(c);
    }
    return hash % tableSize;
}

// Main function: count word frequencies using a hash table with chaining.
std::vector<std::pair<std::string, int>> wordFrequency(const std::vector<std::string>& inputs) {
    const int TABLE_SIZE = 101;
    std::vector<HashNode*> table(TABLE_SIZE, nullptr);

    for (const std::string& raw : inputs) {
        std::string processed;
        if (!preprocessWord(raw, processed)) continue;

        unsigned int index = hashF(processed, TABLE_SIZE);
        HashNode* node = table[index];
        while (node != nullptr && node->word != processed) {
            node = node->next;
        }
        if (node != nullptr) {
            node->count++;
        } else {
            HashNode* newNode = new HashNode(processed, 1);
            if (table[index] == nullptr) {
                table[index] = newNode;
            } else {
                HashNode* tail = table[index];
                while (tail->next != nullptr) {
                    tail = tail->next;
                }
                tail->next = newNode;
            }
        }
    }

    std::vector<std::pair<std::string, int>> result;
    for (int i = 0; i < TABLE_SIZE; i++) {
        HashNode* node = table[i];
        while (node != nullptr) {
            result.emplace_back(node->word, node->count);
            HashNode* toDelete = node;
            node = node->next;
            delete toDelete;
        }
    }

    std::sort(result.begin(), result.end());
    return result;
}
#include <cassert>
#include <vector>
#include <string>
#include <utility>

// The solution function is assumed to be in scope.

int main() {
    // Basic case with multiple words and duplicates.
    std::vector<std::string> input1 = {"hello", "world", "HELLO", "world", "happy"};
    auto result1 = wordFrequency(input1);
    std::vector<std::pair<std::string, int>> expected1 = {{"happy", 1}, {"hello", 2}, {"world", 2}};
    assert(result1 == expected1);

    // Words shorter than or equal to 5 are ignored.
    std::vector<std::string> input2 = {"cat", "dog", "elephant"};
    auto result2 = wordFrequency(input2);
    std::vector<std::pair<std::string, int>> expected2 = {{"elephant", 1}};
    assert(result2 == expected2);

    // Words with digits are ignored.
    std::vector<std::string> input3 = {"abc123", "abcdef", "123abc"};
    auto result3 = wordFrequency(input3);
    std::vector<std::pair<std::string, int>> expected3 = {{"abcdef", 1}};
    assert(result3 == expected3);

    // Leading/trailing punctuation is stripped.
    std::vector<std::string> input4 = {"!hello!", "'world'", "hello"};
    auto result4 = wordFrequency(input4);
    std::vector<std::pair<std::string, int>> expected4 = {{"hello", 2}, {"world", 1}};
    assert(result4 == expected4);

    // Empty input.
    std::vector<std::string> input5 = {};
    auto result5 = wordFrequency(input5);
    assert(result5.empty());

    // All invalid words.
    std::vector<std::string> input6 = {"a", "ab", "abc", "abcd", "abcde", "abc123"};
    auto result6 = wordFrequency(input6);
    assert(result6.empty());

    // Duplicate entries with mixed case.
    std::vector<std::string> input7 = {"Apple", "apple", "APPLE"};
    auto result7 = wordFrequency(input7);
    std::vector<std::pair<std::string, int>> expected7 = {{"apple", 3}};
    assert(result7 == expected7);

    // Many words to test hashing with collisions.
    std::vector<std::string> input8 = {"abcdef", "ghijkl", "mnopqr", "stuvwx", "yzabcd", "efghij"};
    auto result8 = wordFrequency(input8);
    std::vector<std::pair<std::string, int>> expected8 = {{"abcdef", 1}, {"efghij", 1}, {"ghijkl", 1}, {"mnopqr", 1}, {"stuvwx", 1}, {"yzabcd", 1}};
    assert(result8 == expected8);

    // Mixed valid and invalid, unsorted input.
    std::vector<std::string> input9 = {"longword", "short", "LONGWORD", "12345", "anotherword"};
    auto result9 = wordFrequency(input9);
    std::vector<std::pair<std::string, int>> expected9 = {{"anotherword", 1}, {"longword", 2}};
    assert(result9 == expected9);

    // Single valid word.
    std::vector<std::string> input10 = {"unique"};
    auto result10 = wordFrequency(input10);
    std::vector<std::pair<std::string, int>> expected10 = {{"unique", 1}};
    assert(result10 == expected10);
}
