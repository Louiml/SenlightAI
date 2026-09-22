/*
Write a C++ function named `translateMobileMessage` that simulates the T9 predictive text system described in the provided snippet. The function must take three arguments: (1) an integer `N` representing the number of dictionary words, (2) an array of `const char*` dictionary words along with their frequencies, and (3) a null-terminated string containing the numeric message to decode (digits `2-9`, `*` for punctuation selection, `1` for punctuation mark, space as delimiter). The function must return a `std::string` containing the decoded text. The algorithm must maintain a frequency-ordered linked list (simulated using `std::list` of indices) so that when multiple words share the same numeric code, the most frequently used word is returned first, and its frequency is incremented after each use, reordering the list accordingly. The message format: digits `2-9` form a code; `*` increases a counter that selects the k-th most frequent word with that code (0-based); `1` commits the current code and then reads a following run of `*` characters; the number of `*` after `1` modulo 3 determines punctuation (0→'.', 1→',', 2→'?'). A space commits the current code and inserts a space. End of string commits and terminates. Dictionary words are given as pairs: word and frequency (e.g., `call 10`). Input may contain lowercase/uppercase letters; ignore any character that is not a digit, `*`, `1`, or space. Use a fixed maximum word length of 20 and maximum message length of 100001 (but your implementation can be dynamic).
*/
#include <string>
#include <list>
#include <vector>
#include <cctype>
#include <cstring>

// Helper: map a letter (case-insensitive) to a T9 digit 2-9.
// Returns 0 for non-alphabetic characters.
int keypadDigit(char c) {
    char low = std::tolower(static_cast<unsigned char>(c));
    if (low >= 'a' && low <= 'c') return 2;
    if (low >= 'd' && low <= 'f') return 3;
    if (low >= 'g' && low <= 'i') return 4;
    if (low >= 'j' && low <= 'l') return 5;
    if (low >= 'm' && low <= 'o') return 6;
    if (low >= 'p' && low <= 's') return 7;
    if (low >= 't' && low <= 'v') return 8;
    if (low >= 'w' && low <= 'z') return 9;
    return 0;
}

// Compute numeric code for a word (sequence of digits).
int computeWordCode(const char* word) {
    int code = 0;
    for (int i = 0; word[i] != '\0' && i < 20; ++i) {
        int digit = keypadDigit(word[i]);
        if (digit == 0) continue; // skip invalid characters
        code = code * 10 + digit;
    }
    return code;
}

// Main solution function
std::string translateMobileMessage(int N, const std::vector<const char*>& words, const std::vector<int>& freqs, const char* message) {
    // Dictionary entry
    struct Entry {
        std::string word;
        int freq;
        int code;
        int index;
    };

    // Build vector of entries
    std::vector<Entry> dict(N);
    std::list<Entry*> book;
    for (int i = 0; i < N; ++i) {
        dict[i].word = words[i];
        dict[i].freq = freqs[i];
        dict[i].code = computeWordCode(words[i]);
        dict[i].index = i;
        // Insert into sorted list (by decreasing freq, stable for equal)
        auto it = book.begin();
        while (it != book.end() && (*it)->freq >= dict[i].freq) {
            ++it;
        }
        book.insert(it, &dict[i]);
    }

    // Find and update function: given code and k (0-based), return index or -1 if not found
    auto findAndUpdate = [&](int code, int k) -> int {
        int count = 0;
        auto it = book.begin();
        while (it != book.end()) {
            if ((*it)->code == code) {
                if (count == k) {
                    // Found target entry
                    Entry* found = *it;
                    int idx = found->index;
                    // Increment frequency
                    found->freq++;
                    // Remove from list
                    book.erase(it);
                    // Reinsert maintaining decreasing freq order
                    auto pos = book.begin();
                    while (pos != book.end() && (*pos)->freq > found->freq) {
                        ++pos;
                    }
                    // Insert before first with frequency strictly less
                    while (pos != book.end() && (*pos)->freq == found->freq) {
                        ++pos; // stable: keep original order for equal freq? In original, they insert before first with >freq after increment, but we'll place before first with <= freq? 
                    }
                    // Actually for decreasing order, we place before the first element with freq < newfreq
                    // Simpler: traverse from beginning until freq < newfreq, then insert before it.
                    // But the above loop goes to end for equal frequencies; we want to insert before first with strictly less.
                    // So let's recalc:
                    book.erase(it); // already erased? Actually we erased above. So we need to re-insert.
                    // Recalculate insertion point correctly:
                    auto ins = book.begin();
                    while (ins != book.end() && (*ins)->freq >= found->freq) {
                        // For stable equal-frequency, we want to insert after all equal? Original inserts before first with lower freq, but equal order? The snippet inserts before first with freq > newfreq? Let's read: they do `for(qq=begin; qq!=end && (*qq)->freq>ar_dic[i].freq; qq++);` so they stop at first with freq <= newfreq and insert before it. So equal frequencies go after existing equal. We'll replicate.
                        ++ins;
                    }
                    book.insert(ins, found);
                    return idx;
                }
                ++count;
            }
            ++it;
        }
        return -1;
    };

    std::string result;
    int mes_code = 0;
    int kvo_z = 0;
    size_t i = 0;
    while (true) {
        char ch = message[i];
        if (ch >= '2' && ch <= '9') {
            mes_code = mes_code * 10 + (ch - '0');
        } else if (ch == '*') {
            kvo_z++;
        } else if (ch == ' ') {
            if (mes_code != 0) {
                int idx = findAndUpdate(mes_code, kvo_z);
                if (idx >= 0) result += dict[idx].word;
            }
            result += ' ';
            kvo_z = 0;
            mes_code = 0;
        } else if (ch == '1') {
            if (mes_code != 0) {
                int idx = findAndUpdate(mes_code, kvo_z);
                if (idx >= 0) result += dict[idx].word;
            }
            kvo_z = 0;
            i++;
            // Count following asterisks
            while (message[i] == '*') {
                kvo_z++;
                i++;
            }
            i--; // will increment at end of loop to keep position
            char punct = (kvo_z % 3 == 0) ? '.' : (kvo_z % 3 == 1 ? ',' : '?');
            result += punct;
            kvo_z = 0;
            mes_code = 0;
        } else if (ch == '\0') {
            if (mes_code != 0) {
                int idx = findAndUpdate(mes_code, kvo_z);
                if (idx >= 0) result += dict[idx].word;
            }
            break;
        } else {
            // ignore invalid characters
        }
        i++;
    }
    return result;
}
#include <cassert>
#include <vector>
#include <string>
#include <iostream>

// The function is declared above, but for the test we include the same signature
std::string translateMobileMessage(int N, const std::vector<const char*>& words, const std::vector<int>& freqs, const char* message);

int main() {
    // Test 1: Simple dictionary with unique codes
    std::vector<const char*> words1 = {"call", "me", "now"};
    std::vector<int> freqs1 = {10, 5, 8};
    std::string res1 = translateMobileMessage(3, words1, freqs1, "2255 63 6699");
    assert(res1 == "call me now");

    // Test 2: Same code, different words, frequency ordering
    std::vector<const char*> words2 = {"home", "good", "go"};
    std::vector<int> freqs2 = {1, 3, 2}; // codes: home=4663, good=4663, go=46
    // Message "4663" should return most frequent "good" (freq 3)
    std::string res2 = translateMobileMessage(3, words2, freqs2, "4663*");
    assert(res2 == "good");
    // Now good freq becomes 4, so next time still good
    std::string res2b = translateMobileMessage(3, words2, freqs2, "4663* 4663*");
    // After first call, good freq=4, so second "4663*" still picks good
    assert(res2b == "good good");

    // Test 3: Punctuation with '1'
    std::vector<const char*> words3 = {"hello", "world"};
    std::vector<int> freqs3 = {10, 5};
    // Code for hello = 43556, for world = 96753
    std::string res3 = translateMobileMessage(2, words3, freqs3, "43556 1** 96753 1***");
    // "hello" then '1**' -> two stars -> 2%3=2 -> '?' ; "world" then '1***' -> 3 stars -> 0 -> '.'
    assert(res3 == "hello? world.");

    // Test 4: Multiple selection with same code
    std::vector<const char*> words4 = {"a", "b", "c"}; // all code 2? Actually 'a' maps to 2, 'b' maps to 2, 'c' maps to 2
    std::vector<int> freqs4 = {1, 2, 3};
    // Message "2*" -> selects most frequent (c, freq 3) -> "c"
    std::string res4 = translateMobileMessage(3, words4, freqs4, "2*");
    assert(res4 == "c");
    // Message "2**" -> second most frequent (b, freq 2) -> "b"
    std::string res5 = translateMobileMessage(3, words4, freqs4, "2**");
    assert(res5 == "b");
    // Message "2***" -> third (a, freq 1) -> "a"
    std::string res6 = translateMobileMessage(3, words4, freqs4, "2***");
    assert(res6 == "a");

    // Test 5: Mixed case letters in dictionary
    std::vector<const char*> words5 = {"Home", "Good"};
    std::vector<int> freqs5 = {10, 20};
    // codes: Home = 4663 (case-insensitive), Good = 4663
    std::string res7 = translateMobileMessage(2, words5, freqs5, "4663");
    assert(res7 == "Good");

    // Test 6: Empty message
    std::vector<const char*> words6 = {"test"};
    std::vector<int> freqs6 = {1};
    std::string res8 = translateMobileMessage(1, words6, freqs6, "");
    assert(res8 == "");

    // Test 7: Space at end and messy input
    std::vector<const char*> words7 = {"yes", "no"};
    std::vector<int> freqs7 = {5, 4};
    // yes code = 937, no = 66
    std::string res9 = translateMobileMessage(2, words7, freqs7, "937 66 ");
    assert(res9 == "yes no ");

    // Test 8: Invalid characters ignored
    std::vector<const char*> words8 = {"ok"};
    std::vector<int> freqs8 = {1};
    std::string res10 = translateMobileMessage(1, words8, freqs8, "65z! ");
    assert(res10 == "ok ");

    std::cout << "All tests passed!" << std::endl;
    return 0;
}
// The solution must simulate a T9 dictionary with adaptive frequency ordering. The main algorithm:  
// 1. Parse the dictionary: for each word, compute its numeric code by mapping each letter (case-insensitive) to a digit 2-9 according to the phone keypad mapping (a,b,c→2; d,e,f→3; g,h,i→4; j,k,l→5; m,n,o→6; p,q,r,s→7; t,u,v→8; w,x,y,z→9). Ignore any non-alphabetic characters or return 0 for invalid letters (but dictionary words are assumed valid). Store words in a vector/Dictionary struct with fields: word (char array or string), frequency, code (int), and original index.  
// 2. Maintain a `std::list<Dictionary*>` (or list of indices) sorted by decreasing frequency (stable for equal frequencies to preserve insertion order). When inserting each dictionary word, traverse the list from beginning until finding the first element with frequency strictly less than the new word’s frequency, and insert before it.  
// 3. For decoding: iterate over the message characters. Accumulate `mes_code` by multiplying by 10 and adding digit value for each `2-9` digit. Count `*` into `kvo_z`. On a space or `1` or end-of-string, if `mes_code` is non-zero, call a find function that traverses the list to find the k-th entry (where k = `kvo_z`) that has `code == mes_code`. If such entry exists, return its index, then increment its frequency, erase that entry from the list, and re-insert it maintaining sorted order (traverse from beginning until frequency is strictly less than the new frequency). Append the word to the output. For `1`, after committing the current code, read subsequent `*` characters to determine punctuation via modulo 3. Reset `kvo_z` and `mes_code` appropriately. For space, append a space to output. For end-of-string, commit and break.  
// 4. Important edge cases:  
//    - If `kvo_z` exceeds the number of matching words, the traversal might reach end of list; handle by stopping (no word found).  
//    - If the message contains invalid characters, skip them (but the snippet prints an error; we can ignore).  
//    - The dictionary words may have fewer than 20 characters; use null termination.  
//    - Frequencies may be equal; insertion must be stable (insert before first with strictly less frequency, so equal frequencies maintain original order).  
//    - After incrementing a word’s frequency, reinsertion must place it before all entries with equal or lower frequency.  
// 5. Time complexity: For each of `N` dictionary words, insert in sorted list = O(N) per insertion, total O(N^2). For each message character (M length), each lookup may traverse list up to O(N) and reinsertion O(N), so worst-case O(M*N). Space: O(N) for dictionary and list. Given typical constraints (N up to maybe tens of thousands, M up to 100k), this is acceptable.  
// Implementation details: Use `std::list<Dictionary*>` and a vector of Dictionary objects. Provide a helper function `keypadCode(char)` and `decodeMessage` that builds output string.
