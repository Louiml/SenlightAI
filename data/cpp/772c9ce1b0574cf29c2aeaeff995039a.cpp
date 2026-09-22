/*
Write a standalone C++ function named `selectBestWorkers` that, given a vector of candidate worker records (each containing an integer `worker_id`, a double `cost`, and an integer `worker_class`), a required number of workers `K`, and a set of disallowed worker classes, returns a vector of the `K` worker IDs selected by the following algorithm: first, filter out any worker whose class appears in the disallowed set; then, among the remaining workers, select exactly `K` workers with the lowest `cost`. If there are ties in cost, prefer the worker with the smaller `worker_id`. If fewer than `K` eligible workers exist, return an empty vector. The function should not modify the input vector and must handle an empty worker list or `K == 0` gracefully (returning an empty vector for `K == 0` only if there are no eligible workers, but for `K == 0` with any eligible workers, return an empty vector as well because the selection is impossible—clarify: the specification requires exactly `K` workers; if `K == 0`, it is impossible to select exactly zero? Actually, treat `K == 0` as returning an empty vector regardless of eligible workers). The solution must be implemented to be efficient and demonstrate proper use of standard algorithms and data structures.
*/
#include <vector>
#include <queue>
#include <unordered_set>
#include <algorithm>
#include <cstddef>

struct Worker {
    int worker_id;
    double cost;
    int worker_class;
};

// Comparator for max-heap: top is the "worst" candidate (highest cost, then highest id)
struct WorseWorker {
    bool operator()(const Worker& a, const Worker& b) const {
        if (a.cost != b.cost)
            return a.cost < b.cost; // higher cost is worse -> for max-heap, we want top to be worst, so return true if a is worse
        return a.worker_id < b.worker_id; // higher id is worse for tie-break
    }
};

// Comparator for sorting final result by worker_id
struct ById {
    bool operator()(const Worker& a, const Worker& b) const {
        return a.worker_id < b.worker_id;
    }
};

// Select exactly K eligible workers with lowest cost, tie-break by smaller worker_id.
// Return empty vector if not enough eligible workers or if K == 0.
std::vector<int> selectBestWorkers(
    const std::vector<Worker>& workers,
    size_t K,
    const std::unordered_set<int>& disallowed_classes
) {
    if (K == 0) {
        return {};
    }

    std::priority_queue<Worker, std::vector<Worker>, WorseWorker> candidates;

    for (const auto& w : workers) {
        if (disallowed_classes.count(w.worker_class) > 0) {
            continue;
        }

        if (candidates.size() < K) {
            candidates.push(w);
        } else {
            const Worker& worst = candidates.top();
            // Replace if new worker is better than the current worst
            if (w.cost < worst.cost || (w.cost == worst.cost && w.worker_id < worst.worker_id)) {
                candidates.pop();
                candidates.push(w);
            }
        }
    }

    if (candidates.size() != K) {
        return {};
    }

    std::vector<Worker> selected;
    selected.reserve(K);
    while (!candidates.empty()) {
        selected.push_back(candidates.top());
        candidates.pop();
    }

    std::sort(selected.begin(), selected.end(), ById());

    std::vector<int> result;
    result.reserve(K);
    for (const auto& w : selected) {
        result.push_back(w.worker_id);
    }

    return result;
}
#include <cassert>
#include <vector>
#include <unordered_set>

// Include the solution function and struct definitions here

int main() {
    // Test 1: Basic selection
    std::vector<Worker> workers1 = {{1, 5.0, 10}, {2, 3.0, 10}, {3, 4.0, 20}, {4, 2.0, 20}};
    auto res1 = selectBestWorkers(workers1, 2, {});
    assert((res1 == std::vector<int>{2, 4}));

    // Test 2: Disallowed class filters out some workers
    auto res2 = selectBestWorkers(workers1, 2, {20});
    assert((res2 == std::vector<int>{1, 2}));

    // Test 3: Tie-break by lower id when costs equal
    std::vector<Worker> workers3 = {{10, 1.0, 1}, {5, 1.0, 1}, {7, 2.0, 1}};
    auto res3 = selectBestWorkers(workers3, 2, {});
    assert((res3 == std::vector<int>{5, 10}));

    // Test 4: Not enough eligible workers -> empty
    auto res4 = selectBestWorkers(workers3, 5, {});
    assert(res4.empty());

    // Test 5: K == 0 -> empty
    auto res5 = selectBestWorkers(workers3, 0, {});
    assert(res5.empty());

    // Test 6: All workers disallowed
    auto res6 = selectBestWorkers(workers3, 1, {1});
    assert(res6.empty());

    // Test 7: Empty worker list
    std::vector<Worker> empty;
    auto res7 = selectBestWorkers(empty, 1, {});
    assert(res7.empty());

    // Test 8: Mixed costs with duplicates
    std::vector<Worker> workers8 = {{1, 2.0, 1}, {2, 2.0, 2}, {3, 1.0, 2}, {4, 1.0, 1}};
    auto res8 = selectBestWorkers(workers8, 3, {});
    assert((res8 == std::vector<int>{1, 3, 4}));

    return 0;
}
// The core algorithm: iterate through the input vector, ignoring workers whose class is in the disallowed set. For each eligible worker, we need to maintain a collection of at most `K` candidate workers sorted by ascending cost, and for ties, by ascending worker_id. A suitable data structure is a max-heap (priority_queue) where the comparator orders by cost descending, then by worker_id descending, so the "worst" candidate sits at the top. For each eligible worker, if the heap size is less than K, push it. Otherwise, compare the new worker's (cost, id) with the heap's top (worst candidate): if the new one is better (lower cost, or equal cost but lower id), replace the top. At the end, extract the heap elements into a vector and sort them by worker_id (or cost then id) to produce the final result. Important edge cases: disallowed classes set may be empty; multiple workers may have identical costs and IDs? IDs are assumed unique but not required; if duplicates exist, they can be selected but the tie-breaking still applies. If the number of eligible workers is less than K, return empty. Time complexity: O(N log K) where N is the number of input workers, due to heap operations. Space complexity: O(K) for the heap and O(K) for the result, plus a set for disallowed classes (O(D) where D is the number of disallowed classes).
