// Write a C++ function `bool hasPrefixConflict(const std::vector<std::string>& phoneNumbers)` that determines whether any phone number in the input list is a prefix of another phone number in the list. A number is considered a prefix of another if it exactly matches the beginning of the other number (e.g., "911" is a prefix of "91125"). The function should return `true` if such a conflict exists (meaning the list is invalid), and `false` otherwise. Input numbers consist only of digits, may be of different lengths, and duplicates are possible — if a number appears exactly twice, it is trivially a prefix of itself and thus a conflict. The function must handle an empty vector by returning `false` (no conflicts in an empty list). The core requirement is to efficiently detect prefix relationships without comparing all pairs naively.

The solution uses a Trie (prefix tree) to store and check each phone number as it is processed. For each number, before inserting it, we check two conditions: (1) if we encounter a node along the path that is already marked as the end of a previously inserted number, then the current number has a prefix among existing numbers; (2) after traversing the entire current number, if the final node already has children, then this number is a prefix of a longer previously inserted number (or a duplicate). Both cases indicate a prefix conflict. We process numbers in the given order; if any conflict is found, we can stop early and return `true`. If all numbers are processed without conflict, return `false`. Edge cases: an empty vector returns `false`; a single number returns `false`; duplicate numbers cause a conflict because the first insertion marks a terminal node, and the second insertion will encounter that terminal node during traversal. The time complexity is \(O(N \cdot L)\) where \(N\) is the number of phone numbers and \(L\) is the average length, because each number is traversed at most twice (once for checking, once for inserting, but we combine them into one pass). The space complexity is \(O(T)\) where \(T\) is the total number of characters in all inserted numbers, stored in the Trie.

#include <string>
#include <vector>
#include <map>

class TrieNode {
public:
    bool isTerminal;
    std::map<char, TrieNode*> children;

    TrieNode() : isTerminal(false) {}
};

// Returns true if any number in the vector is a prefix of another.
bool hasPrefixConflict(const std::vector<std::string>& phoneNumbers) {
    if (phoneNumbers.empty()) {
        return false;
    }

    TrieNode* root = new TrieNode();
    bool conflictFound = false;

    for (const std::string& number : phoneNumbers) {
        TrieNode* curr = root;
        bool conflictInThisNumber = false;

        for (char ch : number) {
            if (curr->children.find(ch) == curr->children.end()) {
                curr->children[ch] = new TrieNode();
            }
            curr = curr->children[ch];

            // If we pass through a terminal node, this number has a prefix already.
            if (curr->isTerminal) {
                conflictInThisNumber = true;
                break;
            }
        }

        // After traversing the whole number, if the node has children, 
        // this number is a prefix of a longer number already inserted.
        if (!curr->children.empty() || conflictInThisNumber) {
            conflictFound = true;
            break;
        }

        // Mark the end of the current number.
        curr->isTerminal = true;
    }

    // Cleanup memory (not strictly required for the function to work, but good practice).
    // In an actual standalone function, we would recursively delete, but for simplicity 
    // we rely on process termination. For test code, we omit explicit deletion.
    return conflictFound;
}

#include <cassert>
#include <vector>
#include <string>

// (The solution function is assumed to be defined above.)

int main() {
    // Basic valid case: no prefixes
    assert(hasPrefixConflict({"123", "456", "789"}) == false);
    
    // Simple prefix conflict: "911" is prefix of "91125"
    assert(hasPrefixConflict({"911", "91125"}) == true);
    
    // Order reversed: "91125" then "911" also conflict
    assert(hasPrefixConflict({"91125", "911"}) == true);
    
    // Duplicate numbers: conflict
    assert(hasPrefixConflict({"123", "123"}) == true);
    
    // Empty vector: no conflict
    assert(hasPrefixConflict({}) == false);
    
    // Single number: no conflict
    assert(hasPrefixConflict({"12345"}) == true == false); // false expected
    
    // Longer prefix chain: "1" is prefix of "12" is prefix of "123"
    assert(hasPrefixConflict({"1", "12", "123"}) == true);
    
    // No conflict even with common prefixes but not full prefix: "12" and "13"
    assert(hasPrefixConflict({"12", "13"}) == false);
    
    // Numbers with varying lengths, no prefix: "123", "12", "124" — "12" is prefix of "123"
    assert(hasPrefixConflict({"123", "12", "124"}) == true);
    
    // More complex: "9876", "98", "987", "98765" — conflicts at "98"
    assert(hasPrefixConflict({"9876", "98", "987", "98765"}) == true);
    
    return 0;
}
