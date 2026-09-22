Write a C++ function `find_next_perfect` that takes a positive integer `start` and returns the smallest perfect number greater than or equal to `start`. A perfect number is a positive integer that equals the sum of its proper positive divisors (excluding the number itself). For example, 6 is perfect because 1 + 2 + 3 = 6, and 28 is perfect because 1 + 2 + 4 + 7 + 14 = 28. The function must implement perfect-number detection using a recursive helper that checks divisibility up to the candidate number itself, and then recursively search upward until a perfect number is found. Handle the edge case where `start` is already perfect (return it directly). Assume `start` is at least 1. Do not use loops or standard algorithms; use only recursion and conditional expressions.

// The solution uses three recursive helper functions modeled after the snippet's structure. The `is_divisor` function returns 1 if `divisor` divides `num` without remainder (using modulo). The `sum_divisors` function recursively accumulates the sum of all proper divisors of `num`: it starts with index 1 and sum 0, and when the index reaches `num` (exclusive), it returns the accumulated sum; otherwise it adds the index to the sum if it divides `num`, and increments the index in both branches. The `find_next_perfect_rec` function checks whether `candidate` is perfect by comparing the divisor sum to the candidate itself; if it is, it returns the candidate, otherwise it recurses on `candidate + 1`. The main `find_next_perfect` simply calls the recursive helper with the input `start`. Edge cases: when `start` is 1, the sum of divisors is 0, so 1 is not perfect, and the search proceeds to 6. When `start` is already perfect (like 6 or 28), the function returns it immediately. Time complexity is O(P * sqrt(P)) in the worst case for finding the next perfect number P, but since perfect numbers are sparse, in practice it terminates quickly for typical inputs. Space complexity is O(P) due to recursion depth in divisor summation (up to P recursive calls) and O(1) for the upward search (tail recursion).

// Check if divisor divides num with zero remainder.
int is_divisor(int divisor, int num) {
    return (num % divisor) == 0;
}

// Recursively compute the sum of proper divisors of num, starting from index.
int sum_divisors(int index, int sum, int num) {
    if (index == num) {
        return sum;
    }
    if (is_divisor(index, num)) {
        return sum_divisors(index + 1, sum + index, num);
    }
    return sum_divisors(index + 1, sum, num);
}

// Recursively find the smallest perfect number >= candidate.
int find_next_perfect_rec(int candidate) {
    if (sum_divisors(1, 0, candidate) == candidate) {
        return candidate;
    }
    return find_next_perfect_rec(candidate + 1);
}

// Public function: returns the smallest perfect number >= start.
int find_next_perfect(int start) {
    return find_next_perfect_rec(start);
}

int main() {
    assert(find_next_perfect(1) == 6);
    assert(find_next_perfect(5) == 6);
    assert(find_next_perfect(6) == 6);
    assert(find_next_perfect(7) == 28);
    assert(find_next_perfect(27) == 28);
    assert(find_next_perfect(28) == 28);
    assert(find_next_perfect(29) == 496);
    assert(find_next_perfect(495) == 496);
    assert(find_next_perfect(496) == 496);
    assert(find_next_perfect(497) == 8128);
    return 0;
}
