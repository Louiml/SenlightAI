// Given an array `products` of size `L` where each element is a pair of distinct prime numbers that form the product of two ciphertext values (i.e., the product of two adjacent plaintext letters), write a C++ function `string decodeMessage(const vector<pair<int,int>>& products)` that reconstructs the original plaintext string. The plaintext consists of uppercase English letters (A–Z), and each letter corresponds uniquely to one of the distinct prime numbers that appear across all pairs, in ascending order (smallest prime → 'A', next prime → 'B', etc.). Each pair in the input represents the product of two consecutive letters' primes in the plaintext: for the plaintext with primes `p_1 p_2 ... p_{L+1}`, the i-th product is `(min(p_i,p_{i+1}), max(p_i,p_{i+1}))`. Reconstruct and return the full uppercase string. You may assume the input is valid and always has a unique solution. Note that the original code snippet is highly buggy and incomplete; you must design a correct algorithm from scratch, ignoring the flawed logic in the snippet.
To reconstruct the sequence, we need to recover the prime order from the unordered pairs. Observe that the first and last primes appear in exactly one product pair, while every internal prime appears in exactly two adjacent pairs. Therefore, we can identify the endpoints by counting how often each prime value appears across all pairs: the primes appearing only once are the first and last primes of the sequence. Pick one of these as the starting prime (either works, but the reconstruction must proceed consistently). Then we can build the chain: starting from a selected endpoint, at each step we have the current prime, and we look for the pair that contains it; the other member of that pair is the next prime. However, we must be careful to not revisit the previous prime; maintain a visited flag. We can process pairs in a map from each prime to a list of its neighbors. At each step, from the current prime, choose the neighbor that has not been visited yet. This yields the complete ordered list of primes. After obtaining the list, sort all distinct primes ascending and assign letters 'A' to the smallest, 'B' to the second, etc. Then map each prime in the ordered list to its letter and concatenate. Edge cases: the sequence length is at least 2 (i.e., at least two distinct primes), and the input is guaranteed valid. Time complexity: building the adjacency list takes O(L) time, and traversing the chain takes O(L) time. Space complexity is O(L) for storing adjacency and the final result.
#include <vector>
#include <string>
#include <map>
#include <algorithm>

// Reconstructs the plaintext string from a list of unordered prime pairs.
// Each pair represents the product of two adjacent primes in the original sequence.
std::string decodeMessage(const std::vector<std::pair<int, int>>& products) {
    if (products.empty()) return "";

    // Build adjacency list: map each prime to a list of its neighbors.
    std::map<int, std::vector<int>> adj;
    for (const auto& p : products) {
        adj[p.first].push_back(p.second);
        adj[p.second].push_back(p.first);
    }

    // Find endpoints: primes that appear only once as a neighbor.
    int start = 0;
    for (const auto& entry : adj) {
        if (entry.second.size() == 1) {
            start = entry.first;
            break;
        }
    }

    // Traverse the chain from start to the opposite endpoint.
    std::vector<int> prime_order;
    int current = start;
    int previous = -1;
    while (true) {
        prime_order.push_back(current);
        // Find the next unvisited neighbor.
        const auto& neighbors = adj[current];
        int next = -1;
        for (int nb : neighbors) {
            if (nb != previous) {
                next = nb;
                break;
            }
        }
        if (next == -1) break;
        previous = current;
        current = next;
    }

    // Assign letters based on sorted distinct primes.
    std::vector<int> all_primes;
    for (const auto& entry : adj) {
        all_primes.push_back(entry.first);
    }
    std::sort(all_primes.begin(), all_primes.end());
    std::map<int, char> prime_to_char;
    for (size_t i = 0; i < all_primes.size(); ++i) {
        prime_to_char[all_primes[i]] = static_cast<char>('A' + i);
    }

    // Build result string.
    std::string result;
    for (int prime : prime_order) {
        result += prime_to_char[prime];
    }
    return result;
}
#include <cassert>
#include <vector>
#include <string>
#include <utility>

// The solution function is declared above; provide a minimal declaration for the test.
std::string decodeMessage(const std::vector<std::pair<int, int>>& products);

int main() {
    // Example: primes 2, 3, 5, 7 -> plaintext "ABCD" (2->A, 3->B, 5->C, 7->D)
    // Pairs: (2,3) for A-B, (3,5) for B-C, (5,7) for C-D
    std::vector<std::pair<int,int>> test1 = {{2,3}, {3,5}, {5,7}};
    assert(decodeMessage(test1) == "ABCD");

    // Single pair: two primes 2 and 3 -> "AB"
    std::vector<std::pair<int,int>> test2 = {{2,3}};
    assert(decodeMessage(test2) == "AB");

    // Another sequence: primes 5, 2, 11, 3 -> sorted primes: 2,3,5,11 -> letters: 2->A, 3->B, 5->C, 11->D
    // Sequence: 5 (C), 2 (A), 11 (D), 3 (B) -> "CADB"
    // Pairs: (2,5), (2,11), (3,11)
    std::vector<std::pair<int,int>> test3 = {{2,5}, {2,11}, {3,11}};
    assert(decodeMessage(test3) == "CADB");

    // Reversed endpoint order should not matter: same pairs as test3 but different orientation
    std::vector<std::pair<int,int>> test4 = {{5,2}, {11,2}, {11,3}};
    assert(decodeMessage(test4) == "CADB");

    // Longer sequence: primes 11, 7, 5, 3, 2 -> sorted: 2,3,5,7,11 -> letters: 2->A,3->B,5->C,7->D,11->E
    // Sequence: 11(E),7(D),5(C),3(B),2(A) -> "EDCBA"
    std::vector<std::pair<int,int>> test5 = {{7,11}, {5,7}, {3,5}, {2,3}};
    assert(decodeMessage(test5) == "EDCBA");

    // Single letter sequence? Not possible because at least two primes needed for a product, but with L=0 we return empty.
    assert(decodeMessage({}) == "");

    return 0;
}
