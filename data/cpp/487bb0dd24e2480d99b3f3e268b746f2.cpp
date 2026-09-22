// Write a C++ function named `computeBatchUsage` that takes the following inputs: an array of contact constraints (each constraint identifies its pair of body indices `bodyA` and `bodyB` and has a batch index `batchIdx`), the total number of constraints, the number of distinct body indices (i.e., the highest body index + 1), and the number of batches. The function must return a 2D vector of booleans (of size `numBatches` × `numBodies`) where entry `[batch][body]` is `true` if and only if that body is involved in at least one constraint assigned to that batch. For each batch, you must process the constraints in reverse order (from the last constraint to the first), and for each constraint, if the body index is valid (less than `numBodies`) and the body has non‑zero inverse mass (i.e., the body is dynamic, not static), mark the corresponding cell as used. If any constraint references an out‑of‑range body index, ignore that constraint entirely. The function should also handle duplicate body references within the same batch (a body used multiple times in the same batch should still be marked only once). Return the filled 2D vector.

// The task is essentially a bookkeeping problem: given a set of constraints partitioned by a batch index, we need to record, for each batch, which bodies are referenced by constraints in that batch, but only for dynamic bodies (inverse mass > 0). The constraints are processed in reverse order; however, because we are only setting boolean flags, the order does not change the final result—we just need to iterate over the constraints once, determine the batch and the two bodies, check that the body index is within bounds, and that the body's inverse mass is non‑zero. The "reverse order" requirement is trivially satisfied because we can process the array from the end to the beginning, but the outcome is identical to any order since we are using a boolean OR operation (setting a flag). To implement this, we first allocate a 2D vector `usage` of size `numBatches` × `numBodies`, initialized to `false`. Then we loop from the last constraint down to the first (index `n-1` down to `0`). For each constraint, we read `aIdx = constraint[i].bodyA` and `bIdx = constraint[i].bodyB` and `batch = constraint[i].batchIdx`. We check that `batch` is in `[0, numBatches-1]` (if not, skip; though in a well‑formed problem it should be). For each body index, if it is `>= 0` and `< numBodies`, and if the corresponding inverse mass (from a provided array of inverse masses, which the function must also take as input) is non‑zero, we set `usage[batch][bodyIndex] = true`. Edge cases to consider: constraints with the same body on both sides (both indices equal) should mark the body once; a body may appear in multiple constraints in the same batch; a body may appear in multiple batches; invalid body indices (negative or ≥ numBodies) are ignored; invalid batch indices are ignored. The time complexity is O(n) where n is the number of constraints, because we iterate over the constraints once and each operation is constant time. The space complexity is O(numBatches * numBodies) for the returned matrix, plus O(1) auxiliary if we process in place (but we build the matrix). If we assume numBodies and numBatches are reasonable, this is acceptable. The input inverse masses array size should be at least numBodies; we treat any missing entry as zero (or we can require the caller to provide the full array, but for safety we can check bounds and treat out‑of‑range as static). The function signature will be `std::vector<std::vector<bool>> computeBatchUsage(int numBatches, int numBodies, const std::vector<float>& invMasses, const std::vector<Constraint>& constraints)` where `Constraint` is a simple struct with `int bodyA, bodyB, batchIdx`.

#include <vector>
#include <cstddef>

// Simple struct representing a contact constraint.
struct ContactConstraint {
    int bodyA;
    int bodyB;
    int batchIdx;
};

/**
 * Compute which bodies are used in each batch.
 * 
 * @param numBatches Number of batches (rows of the result).
 * @param numBodies  Number of body indices (columns of the result). Valid body indices are 0..numBodies-1.
 * @param invMasses  Inverse mass for each body. Entry i corresponds to body index i. Bodies with invMass == 0 are considered static.
 * @param constraints List of constraints, each with bodyA, bodyB, and batchIdx. The function processes constraints in reverse order.
 * @return A 2D vector of size numBatches x numBodies, where result[b][i] == true iff body i is used by at least one constraint in batch b.
 */
std::vector<std::vector<bool>> computeBatchUsage(
    int numBatches,
    int numBodies,
    const std::vector<float>& invMasses,
    const std::vector<ContactConstraint>& constraints)
{
    // Initialize the result matrix with false.
    std::vector<std::vector<bool>> usage(numBatches, std::vector<bool>(numBodies, false));

    // Process constraints in reverse order (from last to first).
    for (int idx = static_cast<int>(constraints.size()) - 1; idx >= 0; --idx) {
        const ContactConstraint& c = constraints[idx];

        // Skip if batch index is out of range.
        if (c.batchIdx < 0 || c.batchIdx >= numBatches) {
            continue;
        }

        // Helper lambda to mark a body if it is dynamic and within bounds.
        auto markBody = [&](int bodyIdx) {
            if (bodyIdx >= 0 && bodyIdx < numBodies) {
                // If the body's inverse mass is non-zero, it is dynamic.
                if (invMasses[bodyIdx] != 0.0f) {
                    usage[c.batchIdx][bodyIdx] = true;
                }
            }
        };

        markBody(c.bodyA);
        markBody(c.bodyB);
    }

    return usage;
}

#include <cassert>
#include <vector>

// Use the solution function from above.
// (In a real test, you would include the solution here or link it.)

int main() {
    // Test 1: Basic case with two constraints in one batch.
    std::vector<float> invMasses = {1.0f, 0.0f, 2.0f, 0.5f}; // bodies: 0 dynamic, 1 static, 2 dynamic, 3 dynamic
    std::vector<ContactConstraint> constraints = {
        {0, 1, 0}, // batch 0: uses body 0 (dynamic) and body 1 (static -> ignored)
        {2, 3, 0}  // batch 0: uses body 2 and body 3 (both dynamic)
    };
    auto result = computeBatchUsage(2, 4, invMasses, constraints);
    assert(result.size() == 2);
    assert(result[0].size() == 4);
    // Batch 0: bodies 0, 2, 3 should be true; body 1 false.
    assert(result[0][0] == true);
    assert(result[0][1] == false);
    assert(result[0][2] == true);
    assert(result[0][3] == true);
    // Batch 1: all false because no constraints assigned to it.
    for (int i = 0; i < 4; ++i) {
        assert(result[1][i] == false);
    }

    // Test 2: Constraints in multiple batches, with duplicate body references.
    invMasses = {0.0f, 1.0f, 1.0f}; // body 0 static, others dynamic
    constraints = {
        {1, 1, 1}, // batch 1: uses body 1 twice (duplicate)
        {0, 2, 0}, // batch 0: body 0 static -> ignored, body 2 dynamic
        {2, 1, 1}  // batch 1: uses body 2 and body 1
    };
    result = computeBatchUsage(2, 3, invMasses, constraints);
    // Batch 0: only body 2 should be true.
    assert(result[0][0] == false);
    assert(result[0][1] == false);
    assert(result[0][2] == true);
    // Batch 1: bodies 1 and 2 should be true, body 0 false.
    assert(result[1][0] == false);
    assert(result[1][1] == true);
    assert(result[1][2] == true);

    // Test 3: Reverse order processing – check that order does not affect result.
    invMasses = {1.0f, 1.0f};
    constraints = {
        {0, 1, 0},
        {0, 1, 0},
        {0, 1, 0}
    };
    result = computeBatchUsage(1, 2, invMasses, constraints);
    assert(result[0][0] == true && result[0][1] == true);

    // Test 4: Out-of-range body indices are ignored.
    invMasses = {1.0f};
    constraints = {
        {0, 5, 0},  // body 5 out of range
        {-1, 0, 0}, // negative index
        {0, 0, 0}   // valid
    };
    result = computeBatchUsage(1, 1, invMasses, constraints);
    assert(result[0][0] == true); // from the last constraint

    // Test 5: Out-of-range batch index is ignored.
    invMasses = {1.0f};
    constraints = {
        {0, 0, 3} // batch 3 out of range (numBatches=2)
    };
    result = computeBatchUsage(2, 1, invMasses, constraints);
    for (int b = 0; b < 2; ++b) {
        for (int i = 0; i < 1; ++i) {
            assert(result[b][i] == false);
        }
    }

    // Test 6: Empty constraints list yields all false.
    result = computeBatchUsage(3, 5, std::vector<float>(5, 1.0f), {});
    for (int b = 0; b < 3; ++b) {
        for (int i = 0; i < 5; ++i) {
            assert(result[b][i] == false);
        }
    }

    return 0;
}
