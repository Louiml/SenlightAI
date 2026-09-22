You are given an `n x n` square matrix `M` where `M[i][j] == 1` means person `i` knows person `j`, and `M[i][j] == 0` means person `i` does not know person `j`. A "celebrity" is defined as a person who is known by everyone else (for every `i != celeb`, `M[i][celeb] == 1`) and who knows no one (for every `i != celeb`, `M[celeb][i] == 0`). Write a C++ function named `findCelebrity` that takes a constant reference to a 2D vector of integers (`vector<vector<int>>& M`) and an integer `n` representing the number of people, and returns the index of the celebrity if one exists, or `-1` if no celebrity exists or if the matrix is invalid (e.g., `n <= 0` or matrix size not `n x n`). The function must not modify the input matrix and must run efficiently in `O(n)` time, using only a constant amount of extra space in addition to the input. Ensure the function handles edge cases such as `n = 0`, `n = 1` (where the single person is trivially a celebrity if diagonal entries are ignored), duplicate entries, and cases where `M[i][i]` may be 0 or 1 (ignore the diagonal for the celebrity checks).

#include <cassert>
#include <vector>

int findCelebrity(const std::vector<std::vector<int>>& M, int n);

int main() {
    // Test 1: Normal case, celebrity is person 2
    std::vector<std::vector<int>> M1 = {
        {0, 0, 1},
        {0, 0, 1},
        {0, 0, 0}
    };
    assert(findCelebrity(M1, 3) == 2);

    // Test 2: No celebrity (everyone knows someone else)
    std::vector<std::vector<int>> M2 = {
        {0, 1, 0},
        {0, 0, 1},
        {1, 0, 0}
    };
    assert(findCelebrity(M2, 3) == -1);

    // Test 3: Single person is a celebrity (diagonal ignored)
    std::vector<std::vector<int>> M3 = {{0}}; // M[0][0]=0
    assert(findCelebrity(M3, 1) == 0);

    // Test 4: Single person but diagonal is 1 (still celebrity because we ignore diagonal)
    std::vector<std::vector<int>> M4 = {{1}};
    assert(findCelebrity(M4, 1) == 0);

    // Test 5: Invalid size (n=0)
    std::vector<std::vector<int>> M5; // empty
    assert(findCelebrity(M5, 0) == -1);

    // Test 6: Matrix with extra or missing rows/columns (invalid)
    std::vector<std::vector<int>> M6 = {{0, 1}, {0, 0}, {1, 0}}; // 3 rows, 2 cols
    assert(findCelebrity(M6, 2) == -1); // n=2 but M is not 2x2

    // Test 7: All know person 0, person 0 knows no one
    std::vector<std::vector<int>> M7 = {
        {0, 0, 0},
        {1, 0, 0},
        {1, 0, 0}
    };
    assert(findCelebrity(M7, 3) == 0);

    // Test 8: Grid where candidate from elimination fails verification
    std::vector<std::vector<int>> M8 = {
        {0, 1, 1},
        {0, 0, 0},
        {0, 1, 0}
    };
    // Candidate = 0? Let's trace: a=0,b=2, M[0][2]=1 -> a=1. a=1,b=2, M[1][2]=0 -> b=1. a==b -> celeb=1. Verification: i=0: M[0][1]=1, M[1][0]=0 -> ok. i=2: M[2][1]=1, M[1][2]=0 -> ok. So celebrity is 1.
    assert(findCelebrity(M8, 3) == 1);

    // Test 9: Large n with no celebrity (all know each other, but that violates celebrity definition)
    // For n=4, all M[i][j]=1 for i!=j: no one knows no one, so no celebrity.
    std::vector<std::vector<int>> M9 = {
        {0, 1, 1, 1},
        {1, 0, 1, 1},
        {1, 1, 0, 1},
        {1, 1, 1, 0}
    };
    assert(findCelebrity(M9, 4) == -1);

    // Test 10: Celebrity at edge (last person)
    std::vector<std::vector<int>> M10 = {
        {0, 0, 0, 0},
        {1, 0, 0, 0},
        {1, 0, 0, 0},
        {1, 0, 0, 0}
    };
    // Everyone knows person 0? Actually M[i][0]=1 for i=1,2,3, and M[0][j]=0 for all j. So celebrity = 0.
    assert(findCelebrity(M10, 4) == 0);

    return 0;
}

#include <vector>

// Find the index of a celebrity in an n x n knowledge matrix.
// Returns the celebrity index if one exists, otherwise -1.
// M[i][j] == 1 means i knows j, 0 otherwise.
int findCelebrity(const std::vector<std::vector<int>>& M, int n) {
    // Validate input
    if (n <= 0) return -1;
    // Check that matrix is n x n
    if (M.size() != static_cast<size_t>(n)) return -1;
    for (const auto& row : M) {
        if (row.size() != static_cast<size_t>(n)) return -1;
    }

    // Phase 1: elimination to find a candidate (O(n))
    int a = 0;
    int b = n - 1;
    while (a < b) {
        if (M[a][b] == 1) {
            // a knows b, so a cannot be a celebrity
            ++a;
        } else {
            // a does not know b, so b cannot be a celebrity
            --b;
        }
    }

    int celeb = a; // candidate

    // Phase 2: verification
    for (int i = 0; i < n; ++i) {
        if (i == celeb) continue;
        // Everyone must know the celebrity, and celebrity knows no one.
        if (M[i][celeb] != 1 || M[celeb][i] != 0) {
            return -1;
        }
    }
    return celeb;
}

// The classic solution uses a stack-based elimination approach. Push all indices `0` to `n-1` onto a stack. While more than one candidate remains, pop the top two indices `c1` and `c2`. If `M[c1][c2] == 1` (c1 knows c2), then c1 cannot be a celebrity (since celebrities know no one), so discard c1 and push c2 back. Otherwise (c1 does not know c2), c2 cannot be a celebrity (since celebrities are known by everyone, and c1 doesn’t know c2), so discard c2 and push c1 back. After the loop, the remaining candidate is the only possible celebrity. Then verify the candidate: for every `i != celeb`, check that `M[i][celeb] == 1` (everyone knows the celebrity) and `M[celeb][i] == 0` (celebrity knows no one). Also handle invalid inputs: if `n <= 0` or matrix dimensions don't match `n x n`, return `-1`. For `n == 1`, the loop is skipped, and verification passes (since no `i != celeb` exists), so return `0`. Time complexity is `O(n)` for the elimination loop plus `O(n)` for verification, total `O(n)`. Space complexity is `O(1)` if the stack is avoided; but the provided snippet uses a stack, which is `O(n)` space. To meet the constant-space requirement, we can replace the stack with two-pointer elimination. However, for simplicity and clarity, we can still use the stack approach but note that the space is `O(n)`. In the reference solution, we implement the elimination using two indices `a` and `b` (like the "two-pointer" method), achieving `O(1)` extra space.
