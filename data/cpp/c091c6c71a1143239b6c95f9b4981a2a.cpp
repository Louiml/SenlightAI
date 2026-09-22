Write a C++ function that takes a vector of integers representing the positions of stones on a straight line, sorted in strictly increasing order. The function must return the minimum possible maximum jump length needed to travel from the first stone to the last stone, using any sequence of moves where you may skip at most one stone at a time (i.e., from stone `i` you may jump to stone `i+1` or `i+2`, but no further). The goal is to minimize the worst-case (largest single jump) across the entire path. The function should be named `minMaxJump` and accept the vector by const reference.
#include <cassert>
#include <vector>

int minMaxJump(const std::vector<int>& stones);

int main() {
    // Basic case with two stones
    assert(minMaxJump({0, 5}) == 5);
    // Simple three stones: direct jumps are 2 and 3, skip gives 5, so 5
    assert(minMaxJump({0, 2, 5}) == 5);
    // Four stones: gaps 1,1,1 but skipping gives 2, so answer 2
    assert(minMaxJump({0, 1, 2, 3}) == 2);
    // Classic example from snippet: {0, 2, 5, 6, 7} => max(2,5,4,2)=5
    assert(minMaxJump({0, 2, 5, 6, 7}) == 5);
    // Larger gaps with skip: {0, 10, 20, 30} => max(10,20,20)=20
    assert(minMaxJump({0, 10, 20, 30}) == 20);
    // Uneven spacing: {0, 100, 101, 200} => max(100,101,100)=101
    assert(minMaxJump({0, 100, 101, 200}) == 101);
    // All stones equally spaced but large count
    assert(minMaxJump({0, 1, 2, 3, 4, 5}) == 2);
    // Negative positions allowed
    assert(minMaxJump({-10, -5, 0}) == 10);
    // Single skip pattern with large jump at end
    assert(minMaxJump({0, 1, 3, 4, 8}) == 5);
}
#include <vector>
#include <algorithm>

int minMaxJump(const std::vector<int>& stones) {
    int result = stones[1] - stones[0];
    const int n = static_cast<int>(stones.size());
    for (int i = 2; i < n; ++i) {
        result = std::max(result, stones[i] - stones[i - 2]);
    }
    return result;
}
// The problem reduces to finding the minimal possible value `L` such that a path exists from the first stone to the last, using jumps of length at most `L`, and each jump can skip at most one stone. This is equivalent to covering the sequence of stones with a path that never uses a gap greater than `L`. The key observation is that the optimal strategy is to consider jumps of size `i+2` (skipping one stone) because skipping a stone reduces the total number of jumps and forces larger gaps; however, the minimal possible maximum jump is determined by the largest difference between stones that are two positions apart in the original sequence (i.e., `stones[i] - stones[i-2]`). Why? Because if you are allowed to skip at most one stone, then any optimal path will either go directly from `i` to `i+1` or from `i` to `i+2`. If you ever skip two or more stones, you could instead break that into smaller jumps, which would only reduce the maximum jump, so the optimal maximum jump is the maximum over all pairs of stones that are exactly two indices apart. The base case is the first jump: from stone 0 to stone 1 (distance `stones[1]-stones[0]`) which cannot be avoided. Therefore the answer is the maximum of `stones[1]-stones[0]` and all `stones[i]-stones[i-2]` for `i >= 2`. Edge cases: if there are only two stones, the answer is simply `stones[1] - stones[0]`. The input is guaranteed to have at least two stones. Time complexity is O(n) where n is the number of stones, and space complexity is O(1) besides input storage.
