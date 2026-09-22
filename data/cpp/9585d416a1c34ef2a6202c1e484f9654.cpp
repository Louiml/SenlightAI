// Write a C++ function `maxTotalScore` that takes a vector of positive integers representing door values (`doors`) and a positive integer `k` representing the number of pencils initially available. Each pencil has an initial strength of 1, but after using a pencil once, its strength decreases by 1. You must use each door exactly once, and each use consumes a single pencil (you can choose any pencil currently available). The score for using a pencil with current strength `s` on a door with value `v` is `s * v`. However, you start with only `k` pencils, all at strength 1; after using a pencil, its strength reduces by 1, and if it reaches 0 it disappears (cannot be used again). But you may also "recharge" a pencil by using it on a door? Actually, to make this well-defined: The process is: you have `k` pencils, all initially strength 1. For each of `n` doors (sorted in any order you choose), you must select one pencil currently having positive strength, use it, gain `strength * doorValue` score, and then reduce that pencil's strength by 1. If a pencil's strength becomes 0, it is removed. This means you can use the same pencil multiple times as long as its strength is positive (strength starts at 1, so it can be used once, then strength becomes 0 and disappears). But then you can't use it again. That is trivial. However, looking at the code snippet, they push `pencils.top()-1` back into the priority queue, meaning after using a pencil of strength `s`, they replace it with a pencil of strength `s-1` (which might be 0). So you can "chain" a pencil: you have a multiset of strengths, and every time you use a pencil you get its strength, then you add a new pencil of strength one less. Essentially you start with `k` pencils of strength 1, but after using a strength-1 pencil you get a strength-0 pencil (which is useless). So effectively each initial pencil gives you exactly one use. That would make the problem trivial: just sort doors descending and sum them. But the code also has `maximum += pencils.top()*doors[i]` and then `pencils.push(pencils.top()-1)`. This is a known trick: you have a priority queue of available strengths. Initially you have `k` strengths of 1. When you use a strength `s`, you gain `s*door`, and then you insert `s-1` back. So you can use a pencil of strength 1, then get a strength 0, which you may not use (since you'd multiply by 0). But you could also use a pencil of strength 1, then get a 0, and then later you might have a pencil of strength 2? No, you never get a strength 2 because you only ever subtract 1. So the maximum strength you can ever have is 1. So indeed the problem as given is trivial. But perhaps the intended interpretation is different: maybe you have `k` pencils, each initially with unlimited strength? No. The code snippet suggests that you start with `k` pencils, each with strength 1? Actually it reads `pencil` values? Wait, the input reads `k` values? The code reads `n` and `k`, then reads `n` doors, then reads `k` pencil values into a priority queue. That means the pencils have different initial strengths given in input. So each of the `k` pencils has an initial strength (positive integer). When you use a pencil, you gain `strength * doorValue`, then you reduce that pencil's strength by 1 and put it back into the priority queue (if strength becomes 0, you can still put it but it's useless). So you can use the same pencil multiple times as long as its strength is positive. For example, a pencil with strength 3 can be used three times: first with strength 3, then 2, then 1. So the problem is: given `n` door values and `k` pencils with initial strengths, you can use each door exactly once, each time picking a pencil with positive strength, gaining `strength * door`, then reducing that pencil's strength by 1. You can use pencil strengths multiple times as they decrease. Maximize total score.
//
// Thus the task: Write a function `long long maxTotalScore(vector<int> doors, vector<int> pencils)` (or `vector<long long>`). The function should return the maximum total score.
The optimal strategy is to assign the largest door values to the largest available pencil strengths. This is a classic greedy problem: sort the doors in descending order, and maintain a max-heap of pencil strengths. For each door (from largest to smallest), take the largest current pencil strength `s`, add `s * door` to the answer, then decrement that strength by 1 and push it back into the heap (if it becomes 0, it can be ignored because it will never be picked again since we only pick positive strengths). This works because each door must be used exactly once, and the best pairing is to give the highest doors the highest remaining strengths. Since a pencil's strength decreases by 1 each use, using the largest strength first on the largest door is optimal by exchange argument. Edge cases: doors may be empty (but problem likely has n >= 1). If all pencils strengths become 0 after some uses, the remaining doors will get 0 contribution, which is fine. The heap may contain 0s; we can either skip them or let them be multiplied by 0, but to be safe we only consider positive strengths. However, since the input is positive, we can just let the algorithm run; if the top is 0, then all remaining doors get 0. Complexity: sorting doors O(n log n), heap operations O(n log k) where each use does a pop and push, so O(n log k). Space O(k).
#include <vector>
#include <queue>
#include <algorithm>

// Returns the maximum total score by pairing doors with pencil strengths.
long long maxTotalScore(std::vector<long long> doors, std::vector<long long> pencils) {
    // Sort doors in descending order to pair largest with strongest pencil.
    std::sort(doors.begin(), doors.end(), std::greater<long long>());
    
    // Max-heap of available pencil strengths.
    std::priority_queue<long long> available(pencils.begin(), pencils.end());
    
    long long total = 0;
    for (long long door : doors) {
        if (available.empty() || available.top() == 0) {
            break; // No positive strength left, remaining doors contribute 0.
        }
        long long strength = available.top();
        available.pop();
        total += strength * door;
        // After use, strength decreases by 1; if > 0, put it back.
        if (strength - 1 > 0) {
            available.push(strength - 1);
        }
    }
    return total;
}
#include <cassert>
#include <vector>

long long maxTotalScore(std::vector<long long> doors, std::vector<long long> pencils);

int main() {
    // Basic case: one door, one pencil strength 5 -> 5*10=50
    assert(maxTotalScore({10}, {5}) == 50);
    // Two doors, one pencil strength 3: best to use 3 on larger door, then 2 on smaller.
    // doors 5 and 2 -> 3*5 + 2*2 = 15+4=19
    assert(maxTotalScore({5, 2}, {3}) == 19);
    // Multiple pencils: choose best pairing.
    // doors 10, 1; pencils 2 and 1 -> use 2*10 + 1*1 = 21
    assert(maxTotalScore({10, 1}, {2, 1}) == 21);
    // Door larger than total strength sum: leftover doors get 0.
    // doors 100, 1; pencils {1} -> 1*100 + 0*1 = 100
    assert(maxTotalScore({100, 1}, {1}) == 100);
    // Many doors and one high-strength pencil.
    // doors 3,2,1; pencil 10 -> 10*3 + 9*2 + 8*1 = 30+18+8=56
    assert(maxTotalScore({3,2,1}, {10}) == 56);
    // Unsorted input.
    assert(maxTotalScore({2,5}, {3}) == 15+4);
    // Equal strengths.
    // doors 4,4; pencils {2,2} -> 2*4 + 2*4 = 16
    assert(maxTotalScore({4,4}, {2,2}) == 16);
    // Empty doors (edge case) -> 0
    assert(maxTotalScore({}, {1,2}) == 0);
    // Empty pencils but doors exist -> 0
    assert(maxTotalScore({5}, {}) == 0);
    // Large values: use long long.
    assert(maxTotalScore({1000000}, {1000000}) == 1000000000000LL);
    return 0;
}
