Write a standalone C++ function named `findBestSplits` that simulates the core split-finding logic shown in the snippet but simplified for a single-feature, single-node scenario. The function should accept: a 3D tensor (implemented as `std::vector<std::vector<std::vector<float>>>`) representing a node's statistics summary for one feature, with shape `[num_buckets+1][1][logits_dim + hessian_dim]` (the middle dimension is the feature dimension, kept as 1 for simplicity), plus scalar parameters `logits_dim`, `hessian_dim`, `num_buckets` (where the actual statistic bucket count is `num_buckets+1` including the default bucket at the last index), `l1`, `l2`, `min_node_weight`, and `tree_complexity`. The function should return a struct containing: `gain` (float, the best split gain after subtracting parent gain, then subtracting `tree_complexity`), `threshold` (int32, the bucket index of the best split), `left_contrib` and `right_contrib` (each a `std::vector<float>` of size `logits_dim`, representing the optimal leaf weights for the left and right child). The algorithm: compute parent gain using the total gradient and hessian (summed across all buckets including default) via weights `w = -grad / (hess + l2)` (with l1 ignored, assume l1=0), and gain `= 0.5 * w^T * hess * w` (using element-wise multiplication, assuming diagonal hessian, and if `hess` is a vector of size `logits_dim`). Then, for each possible split threshold `t` from 0 to `num_buckets-1`, compute the left statistics by summing buckets 0..t inclusive, right statistics by summing buckets t+1..num_buckets (including default bucket at index num_buckets), and if both left and right total hessian norms (Euclidean norm of the hessian vector) are >= `min_node_weight`, compute left and right gains similarly, and the split gain = left_gain + right_gain - parent_gain. Track the split with maximum gain; if no valid split exists or the maximum gain is non-positive, return gain=0, threshold=-1, and zero contrib vectors. The input statistics have shape `[num_buckets+1][1][logits_dim + hessian_dim]`; the first `logits_dim` columns are gradients, the next `hessian_dim` columns are hessian values.

The key challenge is to correctly aggregate statistics for candidate splits and compute the parent gain. Since hessian is diagonal, we treat it as a vector of size `logits_dim` (assuming `hessian_dim == logits_dim` for simplicity, but the function should read `hessian_dim` entries from each row, ignoring any extra). For each split threshold, we maintain running sums of gradients and hessians for the left part as we iterate from 0 to `num_buckets-1`. The right part is the total sums minus the left running sums. For each candidate, we validate that both left and right hessian norms are at least `min_node_weight` (using a small epsilon to avoid floating-point issues). We compute the contribution weights for each side using `w = -grad / (hess + l2)` (element-wise), but only if all hessian entries are positive after adding l2; if any hessian is zero or negative, that split is invalid (since optimization is undefined). The gain for a side is `0.5 * sum_i(w_i * w_i * hess_i)` (since w = -g/(h+λ), gain = 0.5 * ∑ g^2/(h+λ) = 0.5 * ∑ w^2 h). The split gain is left_gain + right_gain - parent_gain. We subtract `tree_complexity` from the final best gain after finding the maximum. Edge case: if no valid split is found, return zero gain and -1 threshold. Also, if the parent node itself has total hessian norm less than `min_node_weight`, we skip and return zero. Complexity: O(num_buckets * logits_dim) time and O(logits_dim) extra space for accumulating sums. The function should be `const`-correct and avoid modifying inputs.

#include <vector>
#include <cmath>
#include <limits>
#include <algorithm>

struct SplitResult {
    float gain;
    int32_t threshold;
    std::vector<float> left_contrib;
    std::vector<float> right_contrib;
};

// stats: shape [num_buckets + 1][1][logits_dim + hessian_dim]
SplitResult findBestSplits(
    const std::vector<std::vector<std::vector<float>>>& stats,
    int32_t logits_dim,
    int32_t hessian_dim,
    int32_t num_buckets,
    float l1,
    float l2,
    float min_node_weight,
    float tree_complexity) {
    // l1 is expected to be 0 for simplicity; ignored.
    (void)l1;

    SplitResult result;
    result.gain = 0.0f;
    result.threshold = -1;
    result.left_contrib.assign(logits_dim, 0.0f);
    result.right_contrib.assign(logits_dim, 0.0f);

    // Total sums across all buckets including default.
    std::vector<float> total_grad(logits_dim, 0.0f);
    std::vector<float> total_hess(logits_dim, 0.0f);
    for (int b = 0; b <= num_buckets; ++b) {
        const auto& row = stats[b][0];
        for (int i = 0; i < logits_dim; ++i) {
            total_grad[i] += row[i];
            total_hess[i] += row[logits_dim + i];
        }
    }

    // Check parent min_node_weight.
    float hess_norm = 0.0f;
    for (float h : total_hess) hess_norm += h * h;
    hess_norm = std::sqrt(hess_norm);
    if (hess_norm < min_node_weight) {
        return result;
    }

    // Parent gain.
    float parent_gain = 0.0f;
    for (int i = 0; i < logits_dim; ++i) {
        float denom = total_hess[i] + l2;
        if (denom <= 0.0f) return result;
        float w = -total_grad[i] / denom;
        parent_gain += 0.5f * w * w * total_hess[i];
    }

    float best_gain = std::numeric_limits<float>::lowest();
    int32_t best_t = -1;
    std::vector<float> best_left(logits_dim), best_right(logits_dim);

    // Running left sums.
    std::vector<float> left_grad(logits_dim, 0.0f);
    std::vector<float> left_hess(logits_dim, 0.0f);

    for (int t = 0; t < num_buckets; ++t) {
        // Add bucket t to left.
        const auto& row = stats[t][0];
        for (int i = 0; i < logits_dim; ++i) {
            left_grad[i] += row[i];
            left_hess[i] += row[logits_dim + i];
        }

        // Right is total minus left.
        std::vector<float> right_grad(logits_dim), right_hess(logits_dim);
        for (int i = 0; i < logits_dim; ++i) {
            right_grad[i] = total_grad[i] - left_grad[i];
            right_hess[i] = total_hess[i] - left_hess[i];
        }

        // Check both sides' hessian norms.
        float left_norm = 0.0f, right_norm = 0.0f;
        for (float h : left_hess) left_norm += h * h;
        for (float h : right_hess) right_norm += h * h;
        left_norm = std::sqrt(left_norm);
        right_norm = std::sqrt(right_norm);
        if (left_norm < min_node_weight || right_norm < min_node_weight) continue;

        // Compute left gain and right gain.
        float left_gain = 0.0f, right_gain = 0.0f;
        std::vector<float> left_w(logits_dim), right_w(logits_dim);
        bool valid = true;
        for (int i = 0; i < logits_dim; ++i) {
            float l_denom = left_hess[i] + l2;
            float r_denom = right_hess[i] + l2;
            if (l_denom <= 0.0f || r_denom <= 0.0f) { valid = false; break; }
            left_w[i] = -left_grad[i] / l_denom;
            right_w[i] = -right_grad[i] / r_denom;
            left_gain += 0.5f * left_w[i] * left_w[i] * left_hess[i];
            right_gain += 0.5f * right_w[i] * right_w[i] * right_hess[i];
        }
        if (!valid) continue;

        float split_gain = left_gain + right_gain - parent_gain;
        if (split_gain > best_gain) {
            best_gain = split_gain;
            best_t = t;
            best_left = left_w;
            best_right = right_w;
        }
    }

    if (best_t == -1 || best_gain <= 0.0f) {
        return result; // gain remains 0, threshold -1
    }

    result.gain = best_gain - tree_complexity;
    result.threshold = best_t;
    result.left_contrib = best_left;
    result.right_contrib = best_right;
    return result;
}

#include <cassert>
#include <cmath>
#include <vector>
#include "solution.h" // assuming the above is in solution.h

int main() {
    // Case 1: Single feature, 2 buckets (0 and 1) plus default bucket index 2.
    // logits_dim=1, hessian_dim=1, num_buckets=2.
    // stats[b][0][0]=grad, stats[b][0][1]=hess.
    std::vector<std::vector<std::vector<float>>> stats1 = {
        {{-1.0f, 1.0f}},  // bucket 0
        {{1.0f, 1.0f}},   // bucket 1
        {{0.0f, 2.0f}}    // default bucket index 2
    };
    // Totals: grad=0, hess=4 -> parent hess norm=4 >= min_weight=0.1, parent gain=0.
    // Split t=0: left = [-1,1], right = [1,3] (bucket1+default). Left gain: w=1, gain=0.5*1*1=0.5. Right: grad=1, hess=3, w=-1/3≈-0.333, gain=0.5*(0.111)*3≈0.1667. split gain≈0.6667.
    // Split t=1: left = [0,2], right = [0,2] (default). Left gain=0, right gain=0, split gain=0.
    // Best split at t=0. tree_complexity=0.1 -> gain≈0.5667.
    auto r1 = findBestSplits(stats1, 1, 1, 2, 0.0f, 0.0f, 0.1f, 0.1f);
    assert(r1.threshold == 0);
    assert(std::abs(r1.gain - 0.566666f) < 1e-3f);
    assert(std::abs(r1.left_contrib[0] - 1.0f) < 1e-5f);
    assert(std::abs(r1.right_contrib[0] + 0.333333f) < 1e-4f);

    // Case 2: No valid split because all hessians zero or negative.
    std::vector<std::vector<std::vector<float>>> stats2 = {
        {{0.0f, 0.0f}},
        {{0.0f, -1.0f}},
        {{0.0f, 0.0f}}
    };
    auto r2 = findBestSplits(stats2, 1, 1, 2, 0.0f, 0.0f, 0.1f, 0.0f);
    assert(r2.threshold == -1);
    assert(r2.gain == 0.0f);

    // Case 3: Parent hessian norm below min_node_weight.
    std::vector<std::vector<std::vector<float>>> stats3 = {
        {{0.0f, 0.01f}},
        {{0.0f, 0.01f}},
        {{0.0f, 0.01f}}
    };
    auto r3 = findBestSplits(stats3, 1, 1, 2, 0.0f, 0.0f, 1.0f, 0.0f);
    assert(r3.threshold == -1);
    assert(r3.gain == 0.0f);

    // Case 4: Multi-class (logits_dim=2), hessian_dim=1 (diagonal but only one hessian? assume hessian vector length 1 but we treat as length logits_dim? Here hessian_dim=1, but logits_dim=2, hessian vector replicates? For simplicity, we create stats with 3 columns: grad0, grad1, hess (replicated for both).
    // This case checks the function accepts different dims but hessian is only one value per row; our implementation reads only hessian_dim=1 entry, but we need hessian for both dims. Since this task assumes hessian_dim==logits_dim for correctness, skip. We'll use hessian_dim=2 with proper data.
    // Case 4: Two dims, hessian diagonal.
    std::vector<std::vector<std::vector<float>>> stats4 = {
        {{-2.0f, 0.0f, 1.0f, 2.0f}}, // grad0=-2, grad1=0, hess0=1, hess1=2
        {{2.0f, 0.0f, 1.0f, 2.0f}},  // bucket1
        {{0.0f, 0.0f, 2.0f, 2.0f}}   // default
    };
    // Totals: grad0=0, grad1=0, hess0=4, hess1=6, norm≈7.21 >= min.
    // Split t=0: left grad0=-2, grad1=0, hess0=1, hess1=2. right grad0=2, grad1=0, hess0=3, hess1=4.
    // Left: w0 = -(-2)/1=2, gain0=0.5*4*1=2; w1=0, gain1=0 -> left_gain=2.
    // Right: w0=-2/3≈-0.6667, gain0=0.5*0.444*3≈0.6667; w1=0, right_gain≈0.6667.
    // Parent gain=0, split gain≈2.6667. tree_complexity=0.5 -> gain≈2.1667.
    auto r4 = findBestSplits(stats4, 2, 2, 2, 0.0f, 0.0f, 0.1f, 0.5f);
    assert(r4.threshold == 0);
    assert(std::abs(r4.gain - 2.166666f) < 1e-3f);
    assert(std::abs(r4.left_contrib[0] - 2.0f) < 1e-5f);
    assert(std::abs(r4.left_contrib[1] - 0.0f) < 1e-5f);
    assert(std::abs(r4.right_contrib[0] + 0.666666f) < 1e-4f);
    assert(std::abs(r4.right_contrib[1] - 0.0f) < 1e-5f);

    // Case 5: l2 regularization changes weights.
    std::vector<std::vector<std::vector<float>>> stats5 = {
        {{-1.0f, 1.0f}},
        {{0.0f, 1.0f}},
        {{0.0f, 1.0f}}
    };
    // Totals: grad=-1, hess=3, parent gain: w=1/ (3+1)=0.25, gain=0.5*0.0625*3=0.09375.
    // Split t=0: left grad=-1, hess=1; right grad=0, hess=2.
    // Left: denom=1+1=2, w=0.5, gain=0.5*0.25*1=0.125.
    // Right: denom=2+1=3, w=0, gain=0.
    // split_gain = 0.125 - 0.09375 = 0.03125.
    auto r5 = findBestSplits(stats5, 1, 1, 2, 0.0f, 1.0f, 0.1f, 0.0f);
    assert(r5.threshold == 0);
    assert(std::abs(r5.gain - 0.03125f) < 1e-5f);

    return 0;
}
