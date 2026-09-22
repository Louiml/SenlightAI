/*
Write a C++ function named `keepClosestPerCategory` that takes a `std::vector<StopObstacle>` where `StopObstacle` is a struct with a `classification.label` (a `std::string`) and a numeric field `dist_to_collide_on_decimated_traj` (a `double`). The function must return a new vector containing, for each distinct `label`, only the obstacle with the smallest `dist_to_collide_on_decimated_traj`. If multiple obstacles share the same label, keep only the one with the minimum distance; the original order of the first occurrence for each label must be preserved in the output. If the input is empty, return an empty vector. Handle ties in distance by keeping the first encountered obstacle.
*/

#include <vector>
#include <string>
#include <algorithm>

struct StopObstacle {
    struct Classification {
        std::string label;
    } classification;
    double dist_to_collide_on_decimated_traj;
};

// For each distinct classification label, return the obstacle with the minimal distance.
// If ties, keep the first encountered. Preserve order of first occurrence per label.
std::vector<StopObstacle> keepClosestPerCategory(const std::vector<StopObstacle>& obstacles) {
    std::vector<StopObstacle> candidates;
    for (const auto& obstacle : obstacles) {
        auto it = std::find_if(candidates.begin(), candidates.end(),
            [&obstacle](const StopObstacle& existing) {
                return existing.classification.label == obstacle.classification.label;
            });
        if (it == candidates.end()) {
            candidates.push_back(obstacle);
        } else if (obstacle.dist_to_collide_on_decimated_traj < it->dist_to_collide_on_decimated_traj) {
            *it = obstacle;
        }
    }
    return candidates;
}

#include <cassert>
#include <vector>
#include <string>

// Assume the function above is defined here.

int main() {
    // Helper to create an obstacle
    auto make = [](const std::string& label, double dist) {
        StopObstacle obs;
        obs.classification.label = label;
        obs.dist_to_collide_on_decimated_traj = dist;
        return obs;
    };

    // Empty input
    {
        std::vector<StopObstacle> input;
        auto result = keepClosestPerCategory(input);
        assert(result.empty());
    }

    // Single obstacle
    {
        std::vector<StopObstacle> input = { make("car", 10.0) };
        auto result = keepClosestPerCategory(input);
        assert(result.size() == 1);
        assert(result[0].classification.label == "car");
        assert(result[0].dist_to_collide_on_decimated_traj == 10.0);
    }

    // Multiple labels, each unique
    {
        std::vector<StopObstacle> input = {
            make("car", 5.0), make("pedestrian", 3.0), make("truck", 7.0)
        };
        auto result = keepClosestPerCategory(input);
        assert(result.size() == 3);
        assert(result[0].classification.label == "car");
        assert(result[1].classification.label == "pedestrian");
        assert(result[2].classification.label == "truck");
    }

    // Duplicate labels, keep minima
    {
        std::vector<StopObstacle> input = {
            make("car", 5.0), make("car", 2.0), make("pedestrian", 3.0), make("car", 1.0)
        };
        auto result = keepClosestPerCategory(input);
        assert(result.size() == 2);
        // First occurrence order: "car" then "pedestrian"
        assert(result[0].classification.label == "car");
        assert(result[0].dist_to_collide_on_decimated_traj == 1.0);
        assert(result[1].classification.label == "pedestrian");
        assert(result[1].dist_to_collide_on_decimated_traj == 3.0);
    }

    // Ties in distance keep first
    {
        std::vector<StopObstacle> input = {
            make("car", 4.0), make("car", 4.0), make("bike", 2.0)
        };
        auto result = keepClosestPerCategory(input);
        assert(result.size() == 2);
        assert(result[0].classification.label == "car");
        assert(result[0].dist_to_collide_on_decimated_traj == 4.0);
        assert(result[1].classification.label == "bike");
        assert(result[1].dist_to_collide_on_decimated_traj == 2.0);
    }

    // Labels with different cases / exact match
    {
        std::vector<StopObstacle> input = {
            make("Car", 1.0), make("car", 0.5), make("CAR", 0.2)
        };
        auto result = keepClosestPerCategory(input);
        assert(result.size() == 3); // all distinct because case-sensitive
        // Order preserved
        assert(result[0].dist_to_collide_on_decimated_traj == 1.0);
        assert(result[1].dist_to_collide_on_decimated_traj == 0.5);
        assert(result[2].dist_to_collide_on_decimated_traj == 0.2);
    }

    return 0;
}

// The algorithm iterates through the input vector while maintaining a separate result vector `candidates`. For each obstacle, it searches `candidates` for an existing entry with the same `label`. If none is found, the obstacle is appended to `candidates`. If found, compare the current obstacle's distance with the stored one; if the current distance is smaller, replace the stored obstacle. This guarantees each label appears at most once, with the smallest distance. Since the search is linear over the candidates, the worst-case time complexity is \(O(n \cdot m)\), where \(n\) is the size of the input and \(m\) is the number of distinct labels (≤ n). In practice this is often \(O(n^2)\) in the worst case (e.g., all labels distinct), but can be improved with a hash map. However, the given code snippet uses a linear search, and we follow that pattern. Space complexity is \(O(m)\) for the output vector. Edge cases include empty input (returns empty), a single label (returns the one with min distance), and multiple obstacles with the same distance (keeps the first). The solution must be `const`-correct and operate on a copy or by value, returning a new vector.
