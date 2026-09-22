// Write a standalone C++ function that computes the Weighted Degree (WD) kernel value between two DNA sequences without mismatches and without position weights, given a degree parameter `d`. The WD kernel uses a decreasing weight vector: for a substring match of length `k` (from 1 to `d`), the contribution is `(d - k + 1) / (d*(d+1)/2)`. The kernel considers all aligned positions `i` starting from 0 up to `alen-d` (where `alen` is the sequence length), and for each position, it sums the weights for all matching substrings of length up to `d` starting at that position. The sequences must have equal length. Implement the function `double wd_kernel(const std::string& a, const std::string& b, int degree)` that returns the kernel value. Handle edge cases: if sequences have different lengths, throw `std::invalid_argument`; if `degree <= 0`, throw `std::invalid_argument`; if either sequence is empty, return 0.0. Assume only uppercase A, C, G, T characters are present.

The solution iterates over each starting position `i` in the sequence (from 0 to `alen-1` inclusive, though substrings beyond `alen` are naturally limited). For each position, it extends the substring length `k` from 1 to `degree`, but stops early if the characters at `a[i+k-1]` and `b[i+k-1]` mismatch. G represents the contribution: each time characters match, we add the weight for that `k` (computed as `(degree - k + 1) / sum_weights`, where `sum_weights = degree*(degree+1)/2`). If a mismatch occurs at a given `k`, we break out of the inner loop because longer substrings starting at that position will also mismatch. The total result is the sum over all positions. Complexity: time is O(alen * degree) in the worst case (all matches), O(alen) in the typical random case due to early break; space is O(1). Edge cases handled: different lengths throw, degree <=0 throws, empty sequences return 0. This implementation avoids any external libraries beyond standard headers.

#include <string>
#include <stdexcept>

// Compute the Weighted Degree kernel between two DNA sequences a and b.
// degree d >= 1. Sequences must be equal length and non-empty (or return 0).
double wd_kernel(const std::string& a, const std::string& b, int degree) {
    if (degree <= 0) {
        throw std::invalid_argument("degree must be positive");
    }
    if (a.length() != b.length()) {
        throw std::invalid_argument("sequences must have equal length");
    }
    if (a.empty()) {
        return 0.0;
    }

    // Precompute weights: w[k] for k = 1..degree
    double sum_weights = 0.0;
    for (int k = 1; k <= degree; ++k) {
        sum_weights += (degree - k + 1);
    }

    double result = 0.0;
    const size_t len = a.length();

    for (size_t i = 0; i < len; ++i) {
        double local_sum = 0.0;
        for (int k = 1; k <= degree && i + k <= len; ++k) {
            if (a[i + k - 1] != b[i + k - 1]) {
                break;
            }
            local_sum += (degree - k + 1) / sum_weights;
        }
        result += local_sum;
    }

    return result;
}

#include <cassert>
#include <cmath>
#include <string>
#include <stdexcept>

// Include the solution function here (copy from above) or in a header.

int main() {
    // Simple identical sequences, degree 1: weight = 1/1 = 1 for each matching position
    assert(std::abs(wd_kernel("AC", "AC", 1) - 2.0) < 1e-12);

    // Same but degree 2, weights: k=1 w=2/3, k=2 w=1/3, sum_weights=3
    // For each position, if both chars match, k=1 adds 2/3, if second also matches adds 1/3
    // For "AC"/"AC": i=0: k=1 A==A (2/3), k=2 C==C (1/3) => 1.0; i=1: k=1 C==C (2/3) => 2/3. Total=1+2/3=5/3
    assert(std::abs(wd_kernel("AC", "AC", 2) - 5.0/3.0) < 1e-12);

    // Mismatch sequence: "AC" vs "AG", degree 2
    // i=0: k=1 A==A adds 2/3, k=2 C!=G break. i=1: k=1 C!=G break. Total=2/3
    assert(std::abs(wd_kernel("AC", "AG", 2) - 2.0/3.0) < 1e-12);

    // Empty sequences
    assert(wd_kernel("", "", 3) == 0.0);

    // Different lengths should throw
    bool threw = false;
    try { wd_kernel("AC", "ACG", 2); } catch (const std::invalid_argument&) { threw = true; }
    assert(threw);

    // degree <= 0 should throw
    threw = false;
    try { wd_kernel("AC", "AC", 0); } catch (const std::invalid_argument&) { threw = true; }
    assert(threw);

    // Longer sequence with all matches, degree 3
    // weights: k=1:3/6=0.5, k=2:2/6=1/3, k=3:1/6. For "AAAA" (len 4) with degree 3:
    // i=0: k=1,k=2,k=3 all match -> total 0.5+1/3+1/6=1.0
    // i=1: same -> 1.0
    // i=2: k=1,k=2 match (length only has 2 remaining) -> 0.5+1/3=5/6
    // i=3: only k=1 -> 0.5
    // Sum = 1+1+5/6+0.5 = 3.3333...
    assert(std::abs(wd_kernel("AAAA", "AAAA", 3) - (3.0 + 5.0/6.0 + 0.5)) < 1e-12);
    // The expected sum is 1+1+0.83333... +0.5 = 3.33333... = 10/3
    assert(std::abs(wd_kernel("AAAA", "AAAA", 3) - 10.0/3.0) < 1e-12);

    // Symmetry
    assert(std::abs(wd_kernel("ACGT", "ACGA", 2) - wd_kernel("ACGA", "ACGT", 2)) < 1e-12);
}
