/*
Implement a C++ function `totalChocolates(int minutes, std::vector<int>& bags)` that simulates a chocolate-eating process: initially there is a non-empty vector `bags` where each element represents the number of chocolates in a bag. For each of the next `minutes` minutes, a child takes all chocolates from the bag that currently has the most chocolates, eats them, and then replaces that bag with a new one containing exactly half (integer division) of the chocolates that were in it (so a bag with 5 becomes 2, with 4 becomes 2). The function must return the total number of chocolates eaten, modulo 1,000,000,007. The input may contain large numbers (up to 10^9) and `minutes` can be larger than the number of bags. Assume all bags have positive chocolate counts. Use a max-heap to always access the largest bag efficiently.
*/

#include <queue>
#include <vector>

// Returns total chocolates eaten after 'minutes' minutes, modulo 1e9+7.
// Each minute, the bag with the most chocolates is eaten and replaced with floor(half).
int totalChocolates(int minutes, std::vector<int>& bags) {
    const int MOD = 1000000007;
    std::priority_queue<int> maxHeap;
    for (int count : bags) {
        maxHeap.push(count);
    }
    long long total = 0;
    for (int i = 0; i < minutes; ++i) {
        int current = maxHeap.top();
        maxHeap.pop();
        total = (total + current) % MOD;
        maxHeap.push(current / 2);
    }
    return static_cast<int>(total);
}

#include <cassert>

int main() {
    std::vector<int> bags1 = {7, 3};
    assert(totalChocolates(1, bags1) == 7);
    
    std::vector<int> bags2 = {10, 10};
    assert(totalChocolates(2, bags2) == 20);
    
    std::vector<int> bags3 = {5, 5};
    assert(totalChocolates(3, bags3) == 15);
    
    std::vector<int> bags4 = {4, 6};
    assert(totalChocolates(2, bags4) == 10);
    
    std::vector<int> bags5 = {1, 1};
    assert(totalChocolates(5, bags5) == 5);
    
    std::vector<int> bags6 = {1000000000, 1000000000};
    assert(totalChocolates(1, bags6) == 1000000000);
    assert(totalChocolates(2, bags6) == 2000000000 % 1000000007);
    
    std::vector<int> bags7 = {9, 9, 9};
    assert(totalChocolates(10, bags7) == 85);
    
    std::vector<int> bags8 = {2, 3, 4};
    assert(totalChocolates(4, bags8) == 13);
    
    std::vector<int> bags9 = {1};
    assert(totalChocolates(0, bags9) == 0);
    
    std::vector<int> bags10 = {2, 2, 2};
    assert(totalChocolates(3, bags10) == 6);
}

// The core idea is to always pick the current maximum bag, which is naturally handled by a max-heap (priority_queue). First, insert all bag counts into the heap. Then for each of the `minutes` iterations: pop the top element `t`, add `t` to a running sum (taking modulo at each addition to keep numbers small), then push `t/2` back into the heap. This simulates replacing the eaten bag with a half-filled one. Edge cases: if `minutes` is 0, return 0; if all bags are equal, the heap still works correctly; integer division truncates toward zero (which for positive numbers is floor). Complexity: building the heap takes O(B) where B is the number of bags; each of the `minutes` operations takes O(log B) for pop and push, so total time O(B + minutes log B), space O(B) for the heap.
