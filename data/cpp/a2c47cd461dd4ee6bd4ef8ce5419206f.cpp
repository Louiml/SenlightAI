/*
Write a C++ function named `sumDictionaryValues` that takes two integer parameters `m` and `n`, followed by a string containing `m` dictionary entries in the format `"word value"` (each separated by a newline) and then `n` descriptions, where each description is a line of words ending with a period (`.`). The function must read from the provided string input, and for each of the `n` descriptions, sum the values of the words that appear in the dictionary (ignoring words not present) and return a vector of `long long` integers, where the `i`-th element is the sum for the `i`-th description. Words are case-sensitive, and the period is not part of any word; it appears as the final token on each description line. The input string will contain exactly `m` dictionary lines followed by `n` description lines, with no extra blank lines. The function should not print anything, only return the vector of sums.
*/

#include <bits/stdc++.h>
using namespace std;

// Reads m dictionary entries and n descriptions from the input string.
// Returns a vector of n sums, one per description.
// Description ends when a token starting with '.' is encountered.
vector<long long> sumDictionaryValues(int m, int n, const string& input) {
    istringstream stream(input);
    map<string, long long> dict;
    
    // Read dictionary
    for (int i = 0; i < m; ++i) {
        string word;
        long long value;
        stream >> word >> value;
        dict[word] = value;
    }
    
    vector<long long> result;
    result.reserve(n);
    
    // Read each description
    for (int i = 0; i < n; ++i) {
        long long sum = 0;
        string token;
        while (stream >> token) {
            if (token[0] == '.') {
                break;
            }
            auto it = dict.find(token);
            if (it != dict.end()) {
                sum += it->second;
            }
        }
        result.push_back(sum);
    }
    
    return result;
}

#include <bits/stdc++.h>
using namespace std;

// Declare the function from the solution
vector<long long> sumDictionaryValues(int m, int n, const string& input);

int main() {
    // Test 1: Basic case
    string test1 = "3 2\napple 5\nbanana 3\ncat 2\napple banana .\ncat apple dog .\n";
    vector<long long> res1 = sumDictionaryValues(3, 2, test1);
    assert(res1.size() == 2);
    assert(res1[0] == 8); // 5+3
    assert(res1[1] == 7); // 2+5 (dog ignored)
    
    // Test 2: Empty dictionary, all words ignored
    string test2 = "0 2\nhello world .\nfoo .\n";
    vector<long long> res2 = sumDictionaryValues(0, 2, test2);
    assert(res2.size() == 2);
    assert(res2[0] == 0);
    assert(res2[1] == 0);
    
    // Test 3: Description with only a period
    string test3 = "1 1\nword 10\n.\n";
    vector<long long> res3 = sumDictionaryValues(1, 1, test3);
    assert(res3.size() == 1);
    assert(res3[0] == 0);
    
    // Test 4: Case-sensitive words
    string test4 = "2 1\nApple 5\napple 7\nApple .\n";
    vector<long long> res4 = sumDictionaryValues(2, 1, test4);
    assert(res4.size() == 1);
    assert(res4[0] == 5); // only "Apple" matched, not "apple"
    
    // Test 5: Multiple descriptions with large values
    string test5 = "2 3\nlarge 1000000000\nbig 2000000000\nlarge big .\nlarge .\nbig .\n";
    vector<long long> res5 = sumDictionaryValues(2, 3, test5);
    assert(res5.size() == 3);
    assert(res5[0] == 3000000000LL);
    assert(res5[1] == 1000000000LL);
    assert(res5[2] == 2000000000LL);
    
    // Test 6: Repeated words in a description
    string test6 = "1 1\nx 3\nx x x .\n";
    vector<long long> res6 = sumDictionaryValues(1, 1, test6);
    assert(res6.size() == 1);
    assert(res6[0] == 9); // 3+3+3
    
    cout << "All tests passed!\n";
    return 0;
}

// The solution processes the input as a stream. First, we read the number of dictionary entries `m` and descriptions `n` from the input (these are provided as the first two integers in the string). Then we iterate `m` times to read a word and its associated value, storing them in a `std::map<std::string, long long>`. Next, for each of the `n` descriptions, we repeatedly read words from the stream; if the first character of the word is `.`, we stop that description. Otherwise, we check if the word exists in the map and, if so, add its value to an accumulator. After finishing a description, we push the accumulator into a result vector. Edge cases include an empty dictionary (m=0), descriptions with only a period (sum = 0), words not in the dictionary (ignored), and very large sums requiring `long long`. The main algorithm runs in `O(m * log m + n * k * log m)` time, where `k` is the average number of words per description (dominated by the map lookups and insertions). Space complexity is `O(m)` for the dictionary plus `O(n)` for the result vector.
