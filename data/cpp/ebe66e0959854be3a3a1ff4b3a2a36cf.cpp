Implement a C++ function that models the core lattice-building logic from the given MLIR sparse tensor merger code, but in a simplified, standalone form. Specifically, write a function `unsigned buildLattice(const std::vector<int>& tensorSparsity, const std::vector<char>& expression)` that, given a vector describing per-tensor dimension sparsity (1 = sparse, 0 = dense) and a postfix expression over two tensor operands (using characters `+`, `-`, `*`, `/`, and single-letter tensor identifiers `a` and `b`), returns the number of lattice points after applying the analogous construction rules: zero-preserving unary ops (not needed here), multiplicative ops use conjunction (intersection of sparse iteration spaces), additive ops use disjunction (union), and division is restricted to the case where the divisor is invariant (represented by a constant `1` in the expression, which we treat as a dense/synthetic tensor with no sparsity bits). For simplicity, assume the expression uses only two tensors (index 0 and 1), each with exactly 3 dimensions, and the output tensor is index 2 (not used in the expression). The function must count the total number of unique lattice points generated after applying the `optimizeSet` reduction, which removes lattice points that are covered by more general ones (i.e., points whose sparse-bit set is a strict superset of another point’s set). The final count should reflect the size of the optimized lattice set for the top-level expression at loop index 0. The algorithm must handle expressions like `a b *`, `a b +`, `a b -`, `a b /`, and combinations such as `a b * a +` (with postfix notation). Edge cases: if the expression is invalid (e.g., malformed or contains an unsupported operation), return 0; division by a non-constant is not allowed, and any division where the right operand is not the constant `1` returns 0.
#include <cassert>
#include <vector>
#include <string>

// The solution function is declared above (assume it is included).
int main() {
    // Two tensors with 3 dimensions each (sparsity values not used in this simplified logic).
    std::vector<std::vector<int>> sparsity = {
        {1, 0, 1}, // tensor 0: dim0 sparse, dim1 dense, dim2 sparse
        {0, 1, 0}  // tensor 1: dim0 dense, dim1 sparse, dim2 dense
    };

    // a * b → conjunction → one lattice point {a,b}
    assert(buildLattice(sparsity, "a b *") == 1);

    // a + b → disjunction → initially {a}, {b}, {a,b}; after optimize only {a,b} remains
    assert(buildLattice(sparsity, "a b +") == 1);

    // a - b → same as addition (no negation simplifications) → also 1
    assert(buildLattice(sparsity, "a b -") == 1);

    // a / 1 → division by constant → conjunction with empty bit set → {a}
    assert(buildLattice(sparsity, "a 1 /") == 1);

    // a * b + a → first multiply gives {a,b}, then add a gives disjunction → optimized to {a,b}
    assert(buildLattice(sparsity, "a b * a +") == 1);

    // a + a → disjunction of identical sets → {a} and {a} and {a} → after optimize {a}
    assert(buildLattice(sparsity, "a a +") == 1);

    // Invalid division by non-constant → 0
    assert(buildLattice(sparsity, "a b /") == 0);

    // Malformed expression (missing operand) → 0
    assert(buildLattice(sparsity, "a +") == 0);

    // Unknown token → 0
    assert(buildLattice(sparsity, "a x +") == 0);

    // Empty expression → 0
    assert(buildLattice(sparsity, "") == 0);

    return 0;
}
#include <vector>
#include <cstdint>
#include <unordered_map>
#include <stack>
#include <string>
#include <cassert>

// Count lattice points for a postfix expression over two tensors.
// tensorSparsity: for each tensor (0..n-1), a vector of 1 (sparse) or 0 (dense) per dimension.
// expression: postfix tokens separated by spaces. Tokens are 'a', 'b', '1' (constant), 
//             and operations '+', '-', '*', '/'.
// Returns number of optimized lattice points for loop index 0, or 0 on invalid input.
unsigned buildLattice(const std::vector<std::vector<int>>& tensorSparsity,
                      const std::string& expression) {
    // Only support exactly two tensors for simplicity.
    if (tensorSparsity.size() != 2) return 0;
    const int numTensors = 2;
    // Number of bits: one per tensor (for loop index 0 dimension).
    const int bitSize = numTensors;

    // Parse expression into tokens.
    std::vector<std::string> tokens;
    std::string token;
    for (char c : expression) {
        if (c == ' ') {
            if (!token.empty()) { tokens.push_back(token); token.clear(); }
        } else {
            token += c;
        }
    }
    if (!token.empty()) tokens.push_back(token);

    // Each lattice point is just a bitmask over tensors.
    using BitMask = uint32_t; // enough for 2 bits

    // Helper to create a lattice set (vector of unique bitmasks).
    auto addSet = []() { return std::vector<BitMask>(); };

    // Map from expression node index to lattice set. We'll use a stack approach.
    // Since we only need counts, we can simulate with a stack of sets.
    std::stack<std::vector<BitMask>> stack;

    auto addLat = [&](int t) {
        // Single bit for tensor t.
        BitMask mask = (1u << t);
        return std::vector<BitMask>{mask};
    };

    auto conj = [&](const std::vector<BitMask>& s0,
                    const std::vector<BitMask>& s1) {
        std::vector<BitMask> result;
        for (BitMask b0 : s0)
            for (BitMask b1 : s1)
                result.push_back(b0 | b1);
        return result;
    };

    auto disj = [&](const std::vector<BitMask>& s0,
                    const std::vector<BitMask>& s1) {
        std::vector<BitMask> result = conj(s0, s1);
        result.insert(result.end(), s0.begin(), s0.end());
        result.insert(result.end(), s1.begin(), s1.end());
        return result;
    };

    // Optimize: remove any mask that is a strict subset of another in the same set.
    auto optimize = [&](std::vector<BitMask> set) {
        std::vector<BitMask> result;
        for (BitMask m : set) {
            bool covered = false;
            for (BitMask other : set) {
                if (other != m && (other & m) == m && (other | m) == other) {
                    // other is a strict superset of m? Check strictness.
                    if (__builtin_popcount(other) > __builtin_popcount(m)) {
                        covered = true;
                        break;
                    }
                }
            }
            if (!covered) result.push_back(m);
        }
        // Remove duplicates? Not necessary for counting, but do for cleanliness.
        std::sort(result.begin(), result.end());
        result.erase(std::unique(result.begin(), result.end()), result.end());
        return result;
    };

    // Process tokens.
    for (const std::string& tok : tokens) {
        if (tok == "a") {
            stack.push(addLat(0));
        } else if (tok == "b") {
            stack.push(addLat(1));
        } else if (tok == "1") {
            // Constant: no bits (empty mask).
            stack.push(std::vector<BitMask>{0});
        } else if (tok == "+" || tok == "-") {
            if (stack.size() < 2) return 0;
            auto s1 = stack.top(); stack.pop();
            auto s0 = stack.top(); stack.pop();
            auto combined = disj(s0, s1);
            stack.push(optimize(std::move(combined)));
        } else if (tok == "*") {
            if (stack.size() < 2) return 0;
            auto s1 = stack.top(); stack.pop();
            auto s0 = stack.top(); stack.pop();
            auto combined = conj(s0, s1);
            stack.push(optimize(std::move(combined)));
        } else if (tok == "/") {
            if (stack.size() < 2) return 0;
            auto s1 = stack.top(); stack.pop();
            auto s0 = stack.top(); stack.pop();
            // Only allow division when right operand is the constant 1 (empty mask).
            // In our representation, a constant 1 yields a set with a single mask 0.
            bool isConstant = (s1.size() == 1 && s1[0] == 0);
            if (!isConstant) return 0;
            // Division by 1 is like multiplication by 1 (identity) → use conjunction.
            auto combined = conj(s0, s1); // s1 is {0}, so conjunction yields s0's masks.
            // Actually {0} OR m = m, so result = s0's masks.
            stack.push(optimize(std::move(combined)));
        } else {
            return 0; // unknown token
        }
    }

    if (stack.size() != 1) return 0;
    return static_cast<unsigned>(stack.top().size());
}
// The solution mimics the MLIR merger’s lattice construction but abstracts away MLIR types. Each tensor–dimension pair is a “bit” in a BitVector; with 2 tensors × 3 dims = 6 possible bits. A lattice point stores: a bitset (which tensor–dim pairs are active in that point’s iteration space), and an expression ID (not needed for counting, only for constructing children). The key operations:
// - `addLat(t, i, e)`: creates a lattice point with a single bit set at position `t*3 + i`.
// - `takeConj`: for each pair of lattice points from two sets, create a new point whose bit vector is the bitwise OR of the two inputs; then add it to a new set.
// - `takeDisj`: same as takeConj, but also includes all points from the first set, and all points from the second set (after possibly mapping subtraction to negation, but here we don’t have unary ops, so just include all from both).
// - `optimizeSet`: given a set of lattice points, remove any point that is “covered” by a more general one. A point `p1` is covered by `p2` (p2 more general) if `p2.bits` is a strict superset of `p1.bits` and the only difference (if any) consists of dense bits (in our simplified model, we treat all bits as sparse, so any superset with more bits covers). We also remove the trivial “output tensor” copy, but since we don’t have that here, just filter covered points.
//
// The expression is evaluated recursively using a stack. For each token:
// - If it’s a tensor identifier (a or b), create a lattice set with one point: for tensor `a` (index 0), add a point with bit set for each dimension? Actually in the original code, `buildLattices` is called per loop index `i`; here we only care about loop index 0, meaning we create a point for a single dimension (dimension 0) of that tensor. But the original `addLat(t, i, e)` sets bit `numLoops * t + i` where numLoops = number of loops (here 1)?? Wait, carefully read: in original, `LatPoint` bits size is `numLoops * numTensors`. For us, we can simplify: bits size = numTensors * numDims = 2*3 = 6. When building for a specific loop index `i`, we set the bit corresponding to tensor `t` and dimension `i` (since the loop iterates over that dimension). But the expression can involve different dimensions? In the original, `buildLattices` is called for a given loop index `i`, and within that, each tensor contributes a bit for its own dimension `i`. So for loop index 0, tensor `a` contributes bit for (tensor 0, dim 0), tensor `b` contributes bit for (tensor 1, dim 0). But the bitset also includes bits for other loop indices? In the original, bits size is `numLoops * numTensors`, and each bit corresponds to (loop, tensor). Here we can just use bits size 6, but only bits for (tensor, dim) where dim is the current loop index matter. To keep it simple, we can set only one bit per tensor per loop, but since we only have one loop index (0), we can have bits size 2 (one per tensor). However, to mimic the sparsity dimensions, we need to know which dimensions are sparse. The `tensorSparsity` vector describes per-tensor dim sparsity: for tensor 0, array of 3 values (1=sparse, 0=dense). But in the lattice logic, the bits only reflect the loop index dimension. For the simplified task, we can ignore the sparsity dimension specifics and treat all bits equally. To keep the count meaningful, define bits size = 2 (two tensors). Each lattice point’s bit vector indicates which tensors are actively participating in that iteration space for the given loop. The `addLat(t, 0, e)` sets bit `t`. Then the conjunction/disjunction operations work on these 2-bit vectors. The `optimizeSet` removes points whose bit set is a strict subset of another point’s set (since a more general point covers it). For example, with expression `a b *`, we get one point with bits {a,b} after conjunction. With `a b +`, we get three points: {a}, {b}, {a,b} after disjunction (union of {a} and {b} plus their conjunction). Then optimizeSet removes {a} and {b} because they are covered by {a,b}? Actually {a,b} is a superset, so it covers them. But the original logic keeps the more general (larger set) points? Let’s check: In `optimizeSet`, for each point p1, if there exists a p2 in the result set that is a proper superset (onlyDenseDiff allows dense-only differences), then p1 is not added. So the largest sets survive. So for `a b +`, the optimized set has only {a,b} (one point). For `a b *`, conjunction gives {a,b} (one point) already. For `a b -`, same as addition but includes {a} and {b} as well, so optimized gives {a,b}. For `a b /` with constant 1: we treat constant as a synthetic tensor with no bits (so its lattice set is a single point with empty bit set). Conjunction with empty set: pairing with empty yields empty bit set, so result is {a} only (since b is constant 1). Optimized set has one point {a}. The function should return the size of the optimized set. For expression `a b * a +`: first `a b *` gives point {a,b}; then addition with `a` gives disjunction of {a,b} and {a} → union yields {a,b} and {a}, plus conjunction {a,b}. So set = {{a,b}, {a}, {a,b}} → after optimize, only {a,b} remains. So count = 1. If expression is malformed, return 0 as specified.
//
// Time complexity: for expression with `n` operators, each operator processes sets of size at most exponential (in number of tensors), but with only 2 tensors, sets are tiny (max 3 points). So O(n) in practice. Space O(1) aside from expression storage.
//
// Edge cases: division where constant is not 1 (or not constant) → return 0. Any unknown token → return 0. If the expression does not reduce to a single value or stack has more than one element at end → return 0.
