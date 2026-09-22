/*
Write a standalone C++ function named `EliminateTrueFilters` that takes a tree of plan nodes represented by a simplified abstract class hierarchy and returns a new tree where every Filter node whose predicate is a statically known `true` constant is removed (its single child replaces it). The input tree is immutable: the function must not modify any existing node, but may reuse nodes that remain unchanged. Define minimal classes inside the solution to represent: an abstract `PlanNode` with a virtual `CloneWithChildren` method, `FilterPlanNode`, `ScanPlanNode`, and a `Predicate` abstraction with a `IsTrue()` method. The function must recursively process all children, clone nodes with their (already optimized) children, and when a Filter node has a predicate that `IsTrue()` returns true for, directly return its optimized child instead of the cloned Filter node. You may assume every Filter node has exactly one child. The function should work for arbitrary tree shapes and node types, and it must not alter the original tree.
*/
#include <vector>
#include <memory>
#include <cassert>

// Minimal predicate abstraction for demonstration.
class Predicate {
public:
    virtual ~Predicate() = default;
    virtual bool IsTrue() const = 0;
};

class TruePredicate final : public Predicate {
public:
    bool IsTrue() const override { return true; }
};

class FalsePredicate final : public Predicate {
public:
    bool IsTrue() const override { return false; }
};

// Abstract plan node.
class PlanNode {
public:
    virtual ~PlanNode() = default;
    virtual std::shared_ptr<PlanNode> CloneWithChildren(
        std::vector<std::shared_ptr<PlanNode>> children) const = 0;
    virtual int GetType() const = 0;
    const std::vector<std::shared_ptr<PlanNode>>& GetChildren() const {
        return children_;
    }
protected:
    std::vector<std::shared_ptr<PlanNode>> children_;
};

// Concrete node types.
enum class PlanType { Filter, Scan };

class ScanPlanNode final : public PlanNode {
public:
    ScanPlanNode() = default;
    std::shared_ptr<PlanNode> CloneWithChildren(
        std::vector<std::shared_ptr<PlanNode>> children) const override {
        (void)children; // Scan should have no children.
        auto node = std::make_shared<ScanPlanNode>();
        node->children_ = std::move(children);
        return node;
    }
    int GetType() const override { return static_cast<int>(PlanType::Scan); }
};

class FilterPlanNode final : public PlanNode {
public:
    FilterPlanNode(std::shared_ptr<Predicate> pred) : pred_(std::move(pred)) {}
    std::shared_ptr<PlanNode> CloneWithChildren(
        std::vector<std::shared_ptr<PlanNode>> children) const override {
        auto node = std::make_shared<FilterPlanNode>(pred_);
        node->children_ = std::move(children);
        return node;
    }
    int GetType() const override { return static_cast<int>(PlanType::Filter); }
    const Predicate& GetPredicate() const { return *pred_; }
private:
    std::shared_ptr<Predicate> pred_;
};

// The main optimization function.
std::shared_ptr<PlanNode> EliminateTrueFilters(
    const std::shared_ptr<PlanNode>& plan) {
    std::vector<std::shared_ptr<PlanNode>> children;
    for (const auto& child : plan->GetChildren()) {
        children.emplace_back(EliminateTrueFilters(child));
    }

    auto optimized_plan = plan->CloneWithChildren(std::move(children));

    if (optimized_plan->GetType() == static_cast<int>(PlanType::Filter)) {
        auto& filter = static_cast<FilterPlanNode&>(*optimized_plan);
        if (filter.GetPredicate().IsTrue()) {
            assert(optimized_plan->GetChildren().size() == 1);
            return optimized_plan->GetChildren()[0];
        }
    }
    return optimized_plan;
}
#include <cassert>
#include <memory>

int main() {
    // Build a tree: Filter(true) -> Scan
    auto scan = std::make_shared<ScanPlanNode>();
    auto filter_true = std::make_shared<FilterPlanNode>(std::make_shared<TruePredicate>());
    // Manually attach child (simplified; in real code you'd add a method).
    filter_true->children_.push_back(scan);
    auto result1 = EliminateTrueFilters(filter_true);
    assert(result1->GetType() == static_cast<int>(PlanType::Scan));
    assert(result1 == scan); // The original child is returned directly.

    // Build: Filter(false) -> Scan
    auto filter_false = std::make_shared<FilterPlanNode>(std::make_shared<FalsePredicate>());
    filter_false->children_.push_back(scan);
    auto result2 = EliminateTrueFilters(filter_false);
    assert(result2->GetType() == static_cast<int>(PlanType::Filter));
    // The child is preserved inside the filter.
    assert(result2->GetChildren().size() == 1);
    assert(result2->GetChildren()[0]->GetType() == static_cast<int>(PlanType::Scan));

    // Build nested: Filter(true) -> Filter(false) -> Scan
    auto nested_inner = std::make_shared<FilterPlanNode>(std::make_shared<FalsePredicate>());
    nested_inner->children_.push_back(scan);
    auto nested_outer = std::make_shared<FilterPlanNode>(std::make_shared<TruePredicate>());
    nested_outer->children_.push_back(nested_inner);
    auto result3 = EliminateTrueFilters(nested_outer);
    // Outer true filter removed, inner false filter remains.
    assert(result3->GetType() == static_cast<int>(PlanType::Filter));
    assert(result3->GetChildren().size() == 1);
    assert(result3->GetChildren()[0]->GetType() == static_cast<int>(PlanType::Scan));

    // Build: Filter(true) -> Filter(true) -> Scan
    auto inner_true = std::make_shared<FilterPlanNode>(std::make_shared<TruePredicate>());
    inner_true->children_.push_back(scan);
    auto outer_true = std::make_shared<FilterPlanNode>(std::make_shared<TruePredicate>());
    outer_true->children_.push_back(inner_true);
    auto result4 = EliminateTrueFilters(outer_true);
    assert(result4->GetType() == static_cast<int>(PlanType::Scan));
    assert(result4 == scan);

    return 0;
}
// The solution performs a post-order traversal of the input plan tree. For each node, we first recursively optimize all of its children, collecting the results into a vector. We then clone the current node with those optimized children using `CloneWithChildren`. If the cloned node is a Filter and its predicate is statically true, we assert it has exactly one child and return that child directly, effectively eliminating the filter. Otherwise, we return the cloned node. This approach ensures immutability: original nodes are never modified, and unchanged subtrees are reused because `CloneWithChildren` is only called when needed, and if a node's children are unchanged, we still clone the node itself (this is acceptable per the spec; reusing nodes entirely is an optional optimization not required). Edge cases: a non-filter leaf node with no children simply gets cloned and returned; a filter with a non-true predicate is preserved; nested filters where the inner one is true will have the inner removed first, then the outer filter will see its single child and possibly be removed if it also has a true predicate. Time complexity is O(N) where N is the number of nodes, since each node is visited once. Space complexity is O(H + N) due to the recursion stack depth H and the temporary vector of children per node, but we can say O(N) in the worst case for a skewed tree.
