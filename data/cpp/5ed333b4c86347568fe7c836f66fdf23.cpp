Write a C++ function `countCollidingInstances` that, given a `Scene`-like structure consisting of a collection of mesh instances and shape instances (each with an optional transform and a boolean `collides` flag), and a set of "active" mesh pointers, returns the number of instances that (a) have `collides == true`, (b) reference an available mesh (a raw pointer that is not null and is present in the provided active set for mesh instances; for shape instances, the shape pointer must be non-null), and (c) have a valid transform (either an expired/missing transform is treated as identity and counts as valid, but a transform that exists must be non-null). The function should simulate a simplified version of the scene traversal from the snippet: iterate over mesh instances and shape instances, check conditions, and count. Assume all inputs are stored in `std::unordered_map<std::string, std::unique_ptr<...>>`-style containers, but for simplicity, use `std::vector` of `Instance` structs. Provide the function signature with appropriate `const` correctness and use only standard library headers.
The task is a straightforward filtering/counting problem. The main idea is to iterate through each instance (mesh or shape), check the three conditions in order: (1) the instance has `collides == true`, (2) the referenced resource (mesh for mesh instances, shape for shape instances) is available, and (3) the transform is valid (either absent/expired or a valid non-null pointer). If all conditions pass, increment a counter. For mesh instances, "available" means the raw pointer is non-null and is found in the `activeMeshes` set (which represents meshes that were successfully built into the collision structure). For shape instances, "available" simply means the raw shape pointer is non-null. The transform check: if `transform` is `nullptr`, treat as identity and valid; if it is non-null, it must point to a valid object (i.e., not a dangling pointer—we can't truly check that, so we just check non-null). Since we are given a static snapshot, we treat any non-null pointer as valid. Edge cases: empty containers (return 0), duplicate pointers, instances with `collides` false (skip), instances with missing resources (skip), and instances with invalid transforms (skip). Time complexity is O(N + M) where N is number of mesh instances and M is number of shape instances, plus O(1) average for set lookups if we use a hash set. Space complexity is O(K) for the set of active meshes, but that is provided.
#include <cstddef>
#include <unordered_set>
#include <vector>

// Forward declarations to avoid full definitions.
struct MeshInst;
struct ShapeInst;

// A simplified instance holding either mesh or shape info.
struct Instance {
    bool collides;
    const void* mesh_ptr;   // for mesh instances, raw pointer to mesh; for shapes, null
    const void* shape_ptr;  // for shape instances, raw pointer to shape; for meshes, null
    const void* transform;  // non-null if a transform is considered present (non-expired)
};

// Count instances that are eligible for collision: collides flag is true,
// the referenced resource is available (mesh in active set, or shape non-null),
// and transform is valid (either null pointer meaning identity, or non-null).
std::size_t countCollidingInstances(
    const std::vector<Instance>& instances,
    const std::unordered_set<const void*>& activeMeshes
) {
    std::size_t count = 0;

    for (const auto& inst : instances) {
        // Must be flagged as colliding.
        if (!inst.collides) continue;

        // Check resource availability.
        bool resource_ok = false;
        if (inst.mesh_ptr != nullptr) {
            // Mesh instance: pointer must be in active set.
            resource_ok = activeMeshes.find(inst.mesh_ptr) != activeMeshes.end();
        } else if (inst.shape_ptr != nullptr) {
            // Shape instance: non-null shape pointer is sufficient.
            resource_ok = true;
        }
        if (!resource_ok) continue;

        // Check transform validity: null transform means identity (valid),
        // non-null transform means a valid pointer (we treat any non-null as valid).
        if (inst.transform == nullptr) {
            // Identity transform is fine.
        } else {
            // Non-null transform is considered valid (no further checks possible).
        }

        ++count;
    }

    return count;
}
#include <cassert>
#include <unordered_set>
#include <vector>

// The function defined above (already included).
// Place the solution code before main.

int main() {
    // Dummy addresses for testing.
    int dummy_mesh1 = 1;
    int dummy_mesh2 = 2;
    int dummy_shape1 = 3;
    int dummy_shape2 = 4;
    int dummy_transform1 = 5;
    int dummy_transform2 = 6;

    std::unordered_set<const void*> active;
    active.insert(&dummy_mesh1); // only dummy_mesh1 is "active"

    // Test 1: Empty input.
    {
        std::vector<Instance> insts;
        assert(countCollidingInstances(insts, active) == 0);
    }

    // Test 2: Only colliding mesh with active pointer and identity transform.
    {
        std::vector<Instance> insts = {
            {true, &dummy_mesh1, nullptr, nullptr} // collides, mesh active, transform null
        };
        assert(countCollidingInstances(insts, active) == 1);
    }

    // Test 3: Mesh not in active set (should be skipped).
    {
        std::vector<Instance> insts = {
            {true, &dummy_mesh2, nullptr, nullptr} // collides true, but mesh not active
        };
        assert(countCollidingInstances(insts, active) == 0);
    }

    // Test 4: Shape with non-null shape pointer and valid transform.
    {
        std::vector<Instance> insts = {
            {true, nullptr, &dummy_shape1, &dummy_transform1}
        };
        assert(countCollidingInstances(insts, active) == 1);
    }

    // Test 5: Shape with null shape pointer (should be skipped).
    {
        std::vector<Instance> insts = {
            {true, nullptr, nullptr, &dummy_transform1}
        };
        assert(countCollidingInstances(insts, active) == 0);
    }

    // Test 6: Mixed instances with various conditions.
    {
        std::vector<Instance> insts = {
            {true, &dummy_mesh1, nullptr, nullptr},      // counts
            {false, &dummy_mesh1, nullptr, nullptr},     // collides false, skip
            {true, &dummy_mesh2, nullptr, nullptr},      // not active, skip
            {true, nullptr, &dummy_shape2, nullptr},     // shape non-null, identity transform, counts
            {true, nullptr, &dummy_shape1, nullptr},     // shape non-null, identity transform, counts
        };
        assert(countCollidingInstances(insts, active) == 3);
    }

    // Test 7: All colliding flags true but only one valid resource.
    {
        std::vector<Instance> insts = {
            {true, &dummy_mesh1, nullptr, &dummy_transform1},
            {true, nullptr, &dummy_shape2, &dummy_transform2},
            {true, &dummy_mesh2, nullptr, nullptr},
        };
        assert(countCollidingInstances(insts, active) == 2);
    }

    // Test 8: Duplicate mesh pointers counting once each per instance.
    {
        std::vector<Instance> insts = {
            {true, &dummy_mesh1, nullptr, nullptr},
            {true, &dummy_mesh1, nullptr, nullptr}
        };
        assert(countCollidingInstances(insts, active) == 2);
    }

    return 0;
}
