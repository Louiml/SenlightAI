/*
Write a C++ function `printPermutationsWithoutAdjacentConsecutive(int n)` that generates all permutations of the integers from `1` to `n` (inclusive) and returns the count of permutations where no two adjacent elements in the permutation are consecutive integers (i.e., differ by exactly 1). For each valid permutation, the function should output the permutation as a string of digits (without spaces, in the order of the permutation) to `std::cout`, one per line. The function should return the total number of valid permutations. The input `n` will satisfy `1 <= n <= 10`. If no valid permutations exist, the function should output nothing and return `0`. The function must not read from standard input or contain a `main` function.
*/
#include <iostream>
#include <string>
#include <vector>
#include <cmath>

// Counts and prints all permutations of 1..n where no adjacent pair is consecutive.
// Returns the number of such permutations.
int printPermutationsWithoutAdjacentConsecutive(int n) {
    int count = 0;
    std::vector<int> permutation(n);
    std::vector<bool> used(n + 1, false);
    
    // Recursive backtracking function
    std::function<void(int)> dfs = [&](int pos) {
        if (pos == n) {
            // Valid permutation found
            std::string output;
            for (int val : permutation) {
                output += std::to_string(val);
            }
            std::cout << output << "\n";
            ++count;
            return;
        }
        for (int val = 1; val <= n; ++val) {
            if (used[val]) continue;
            // If pos > 0, check adjacency with previous element
            if (pos > 0 && std::abs(permutation[pos - 1] - val) == 1) continue;
            used[val] = true;
            permutation[pos] = val;
            dfs(pos + 1);
            used[val] = false;
        }
    };
    
    dfs(0);
    return count;
}
#include <cassert>
#include <iostream>
#include <sstream>
#include <string>

// Declare the function (or include the header)
int printPermutationsWithoutAdjacentConsecutive(int n);

int main() {
    // Test n = 1: only permutation "1" valid
    {
        std::ostringstream oss;
        std::streambuf* oldCout = std::cout.rdbuf(oss.rdbuf());
        int result = printPermutationsWithoutAdjacentConsecutive(1);
        std::cout.rdbuf(oldCout);
        assert(result == 1);
        assert(oss.str() == "1\n");
    }
    
    // Test n = 2: no valid permutations
    {
        std::ostringstream oss;
        std::streambuf* oldCout = std::cout.rdbuf(oss.rdbuf());
        int result = printPermutationsWithoutAdjacentConsecutive(2);
        std::cout.rdbuf(oldCout);
        assert(result == 0);
        assert(oss.str().empty());
    }
    
    // Test n = 3: valid permutations are 132, 213, 231, 312 (4 total)
    {
        std::ostringstream oss;
        std::streambuf* oldCout = std::cout.rdbuf(oss.rdbuf());
        int result = printPermutationsWithoutAdjacentConsecutive(3);
        std::cout.rdbuf(oldCout);
        assert(result == 4);
        assert(oss.str() == "132\n213\n231\n312\n");
    }
    
    // Test n = 4: count is 2? Let's verify manually: 2413, 3142 (2 total)
    {
        std::ostringstream oss;
        std::streambuf* oldCout = std::cout.rdbuf(oss.rdbuf());
        int result = printPermutationsWithoutAdjacentConsecutive(4);
        std::cout.rdbuf(oldCout);
        assert(result == 2);
        assert(oss.str() == "2413\n3142\n");
    }
    
    std::cout << "All tests passed!" << std::endl;
    return 0;
}
// The problem is a classic permutation generation with a constraint. We can use depth-first search (DFS) with backtracking: maintain an array `a` for the current permutation and a boolean array `used` to track which numbers have been placed. At each recursive step, for position `k` (1-indexed), we try every unused number `i`. Before placing it, we can check if it would form a consecutive pair with the previously placed number (`a[k-1]`). If `abs(a[k-1] - i) == 1`, we skip this choice. When `k == n+1` (all positions filled), we check the entire permutation (though the incremental check already guarantees validity), output the permutation as a string, increment the count, and return. The base case is `n == 1` where the only permutation `1` has no adjacent pairs, so it is valid. Edge cases include `n = 2` where both permutations `12` and `21` are invalid because adjacent numbers differ by 1, so the function returns `0`. Time complexity is `O(n! * n)` in the worst case (generating all permutations and checking each), but with pruning it is often less. Space complexity is `O(n)` for the recursion stack and arrays. For `n <= 10`, this is efficient enough.
