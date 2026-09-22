/*
Write a C++ function named `countDistinctCubeStates` that takes a `std::vector<RubiksCubeBitboard>` (a collection of cube states after various moves) and returns the number of distinct cube configurations, where two configurations are considered the same if they have identical `bitboard` arrays. You must implement this function as a free (non-member) function that uses the provided `RubiksCubeBitboard` class and its `operator==` for comparison, but for efficiency, you should also implement a custom hash function that produces a hash based on all six `bitboard` values (e.g., XOR of all six 64-bit values) and use an `unordered_set` to track distinct states in O(n) average time. The function must not modify the input vector, must be `const`-correct, and must handle an empty input (returning 0). You are not allowed to add any member functions to the class or modify the class definition; the solution must be entirely independent.
*/

#include <vector>
#include <unordered_set>
#include <cstdint>

// Count distinct cube configurations in a vector of RubiksCubeBitboard objects.
// Uses the provided HashBitboard and operator== to deduplicate states.
int countDistinctCubeStates(const std::vector<RubiksCubeBitboard>& cubes) {
    std::unordered_set<RubiksCubeBitboard, HashBitboard> distinct;
    for (const auto& cube : cubes) {
        distinct.insert(cube);
    }
    return static_cast<int>(distinct.size());
}

#include <cassert>
#include <vector>

// Assume the full RubiksCubeBitboard class and HashBitboard are defined above.
int main() {
    // Test 1: Empty vector
    {
        std::vector<RubiksCubeBitboard> cubes;
        assert(countDistinctCubeStates(cubes) == 0);
    }

    // Test 2: Single solved cube
    {
        std::vector<RubiksCubeBitboard> cubes(1);
        assert(countDistinctCubeStates(cubes) == 1);
    }

    // Test 3: Two identical solved cubes
    {
        RubiksCubeBitboard c1, c2;
        std::vector<RubiksCubeBitboard> cubes = {c1, c2};
        assert(countDistinctCubeStates(cubes) == 1);
    }

    // Test 4: Two different states (solved vs. one U move)
    {
        RubiksCubeBitboard c1, c2;
        c2.u();
        std::vector<RubiksCubeBitboard> cubes = {c1, c2};
        assert(countDistinctCubeStates(cubes) == 2);
    }

    // Test 5: Known sequence - U then U' should be solved, so distinct count is 1
    {
        RubiksCubeBitboard c1, c2;
        c2.u();
        c2.uPrime();
        std::vector<RubiksCubeBitboard> cubes = {c1, c2};
        assert(countDistinctCubeStates(cubes) == 1);
    }

    // Test 6: Mixed duplicates - three cubes: solved, U, solved
    {
        RubiksCubeBitboard c1, c2, c3;
        c2.u();
        std::vector<RubiksCubeBitboard> cubes = {c1, c2, c3};
        assert(countDistinctCubeStates(cubes) == 2);
    }

    // Test 7: Many distinct moves (U, U2, U' are all different from each other and solved)
    {
        RubiksCubeBitboard solved, u1, u2, u3;
        u1.u();
        u2.u2();
        u3.uPrime();
        std::vector<RubiksCubeBitboard> cubes = {solved, u1, u2, u3};
        assert(countDistinctCubeStates(cubes) == 4);
    }

    // Test 8: Check that U3 is same as U' (should be same state)
    {
        RubiksCubeBitboard u3, uPrime;
        u3.u();
        u3.u();
        u3.u();
        uPrime.uPrime();
        std::vector<RubiksCubeBitboard> cubes = {u3, uPrime};
        // Both are the same state, so distinct count is 1
        assert(countDistinctCubeStates(cubes) == 1);
    }

    // Test 9: Verify that the function does not mutate the input
    {
        RubiksCubeBitboard c1, c2;
        c2.u();
        std::vector<RubiksCubeBitboard> cubes = {c1, c2};
        RubiksCubeBitboard original1 = c1;
        RubiksCubeBitboard original2 = c2;
        int result = countDistinctCubeStates(cubes);
        assert(result == 2);
        assert(c1 == original1);
        assert(c2 == original2);
    }

    // Test 10: Large count with mixed duplicates
    {
        RubiksCubeBitboard solved, u1, u2, f1, r1;
        u1.u();
        u2.u2();
        f1.f();
        r1.r();
        std::vector<RubiksCubeBitboard> cubes = {
            solved, u1, solved, u2, f1, u1, r1, f1, r1, u2
        };
        // Distinct: solved, u1, u2, f1, r1 → 5
        assert(countDistinctCubeStates(cubes) == 5);
    }

    return 0;
}

// The core problem reduces to counting unique elements in a collection based on structural equality, not just address equality. Since `RubiksCubeBitboard` defines `operator==` to compare all six `bitboard` integers, we can leverage that for exact equality checks. However, using `std::vector` and linear search would be O(n²) in the worst case, which is inefficient for large inputs. Instead, we use `std::unordered_set<RubiksCubeBitboard, HashBitboard>`, where `HashBitboard` is already provided in the snippet. The hash function XORs all six `bitboard` values into a single 64-bit hash, which is fast and distributes well for cube states. We iterate through the input vector, insert each cube into the set; duplicates are automatically ignored because the set uses both the hash and `operator==` to resolve collisions. The number of distinct states is simply the size of the set. Edge cases: empty input returns 0; a single element always returns 1; states that differ only by rotation (e.g., after applying `u()` multiple times) will still be considered distinct unless they are exactly equal bit-for-bit, which is correct because we count raw configurations, not equivalence classes under cube symmetries. Time complexity is O(n) average for insertion (assuming good hash distribution), and O(n) worst-case if hash collisions are many, but with a 64-bit XOR hash, collisions are extremely rare; space complexity is O(n) for the set. The function must be `const`-qualified and take the vector by `const&` to avoid copying.
