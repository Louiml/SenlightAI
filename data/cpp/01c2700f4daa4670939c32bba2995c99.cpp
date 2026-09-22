Write a C++ function that evaluates a drone flight path against a set of mission tasks. The function takes a vector of 2D points (the drone's path) and a vector of task objects, where each task has a type (`fly_through`, `no_fly`, or `ending`), a geometric polygon defined by a vector of 2D points, and a penalty score. For each task, compute a performance score as follows: for `fly_through` tasks, award 500 points if the path passes through the polygon (any path point lies inside or on the boundary); for `no_fly` tasks, award 500 points if the path does NOT pass through the polygon; for `ending` tasks, award 500 points if the final path point lies inside the polygon. Additionally, if all tasks earn their full 500 points each, add a shortness bonus equal to the sum of (score / path_length) for tasks that have `apply_shortness_bonus = true`, where `path_length` is the number of points in the path. Return the total fitness score as a double. The path is guaranteed non-empty, and polygons are simple (non-self-intersecting) and may be concave. Use the point-in-polygon ray-casting algorithm for point inclusion tests.
// The solution iterates over each task and its associated polygon, then checks the path points against that polygon using a ray-casting point-in-polygon test. For each task, determine whether the task is satisfied based on its type and the boolean "path intersects polygon" result. If satisfied, add 500 to the total; otherwise add 0. Separately, accumulate a summed shortness bonus for tasks where `apply_shortness_bonus` is true, but only add that bonus to the total if every task earned exactly 500. Edge cases include: polygons with fewer than 3 points (treat as empty, no path point can be inside), path points exactly on an edge (ray-casting should count as inside by considering both edge cases), and tasks with no polygon (treat as unsatisfied, score 0). Time complexity is O(T * P * V) where T is number of tasks, P is number of path points, and V is number of vertices in the largest polygon. Space complexity is O(1) beyond inputs.
#include <vector>
#include <cmath>

struct Point2D {
    double x, y;
    Point2D(double x = 0.0, double y = 0.0) : x(x), y(y) {}
};

enum class TaskType { FlyThrough, NoFly, Ending };

struct Task {
    TaskType type;
    std::vector<Point2D> polygon;  // vertices in order, closed implicitly
    bool apply_shortness_bonus;
    Task(TaskType t, const std::vector<Point2D>& poly, bool bonus)
        : type(t), polygon(poly), apply_shortness_bonus(bonus) {}
};

// Ray-casting point-in-polygon test. Returns true if 'p' is strictly inside or on boundary.
bool pointInPolygon(const Point2D& p, const std::vector<Point2D>& poly) {
    int n = static_cast<int>(poly.size());
    if (n < 3) return false;
    bool inside = false;
    for (int i = 0, j = n - 1; i < n; j = i++) {
        const Point2D& pi = poly[i];
        const Point2D& pj = poly[j];
        // Check if point is exactly on an edge (handles boundary cases).
        double cross = (pi.x - p.x) * (pj.y - p.y) - (pi.y - p.y) * (pj.x - p.x);
        if (std::abs(cross) < 1e-9 &&
            std::min(pi.x, pj.x) - 1e-9 <= p.x && p.x <= std::max(pi.x, pj.x) + 1e-9 &&
            std::min(pi.y, pj.y) - 1e-9 <= p.y && p.y <= std::max(pi.y, pj.y) + 1e-9) {
            return true;
        }
        // Standard ray-casting crossing test.
        bool intersects = (pi.y > p.y) != (pj.y > p.y);
        if (intersects) {
            double x_intersect = (pj.x - pi.x) * (p.y - pi.y) / (pj.y - pi.y) + pi.x;
            if (p.x < x_intersect) inside = !inside;
        }
    }
    return inside;
}

// Evaluate the fitness of a drone path against mission tasks.
double evaluateFitness(const std::vector<Point2D>& path, const std::vector<Task>& tasks) {
    if (path.empty() || tasks.empty()) return 0.0;

    double totalScore = 0.0;
    double shortnessBonus = 0.0;
    bool allTasksCompleted = true;

    int pathLength = static_cast<int>(path.size());

    for (const Task& task : tasks) {
        bool pathIntersects = false;
        for (const Point2D& pt : path) {
            if (pointInPolygon(pt, task.polygon)) {
                pathIntersects = true;
                break;
            }
        }

        bool taskCompleted = false;
        if (task.type == TaskType::FlyThrough) {
            taskCompleted = pathIntersects;
        } else if (task.type == TaskType::NoFly) {
            taskCompleted = !pathIntersects;
        } else if (task.type == TaskType::Ending) {
            taskCompleted = !path.empty() && pointInPolygon(path.back(), task.polygon);
        }

        if (taskCompleted) {
            totalScore += 500.0;
            if (task.apply_shortness_bonus) {
                shortnessBonus += 500.0 / pathLength;
            }
        } else {
            allTasksCompleted = false;
        }
    }

    if (allTasksCompleted) {
        totalScore += shortnessBonus;
    }
    return totalScore;
}
#include <cassert>
#include <cmath>

int main() {
    // Geometry: a square polygon from (0,0) to (10,10)
    std::vector<Point2D> square = {
        Point2D(0,0), Point2D(10,0), Point2D(10,10), Point2D(0,10)
    };
    // Geometry: a triangle polygon from (20,20) to (30,30)
    std::vector<Point2D> triangle = {
        Point2D(20,20), Point2D(30,20), Point2D(25,30)
    };

    // Case 1: Path through square, fly_through with bonus, no other tasks
    {
        std::vector<Point2D> path = {Point2D(5,5)};
        std::vector<Task> tasks = {
            Task(TaskType::FlyThrough, square, true)
        };
        double fit = evaluateFitness(path, tasks);
        assert(std::abs(fit - (500.0 + 500.0/1)) < 1e-9); // all completed => bonus added
    }

    // Case 2: Path outside square, fly_through fails
    {
        std::vector<Point2D> path = {Point2D(50,50)};
        std::vector<Task> tasks = {
            Task(TaskType::FlyThrough, square, true)
        };
        double fit = evaluateFitness(path, tasks);
        assert(std::abs(fit - 0.0) < 1e-9);
    }

    // Case 3: No-fly task, path outside polygon succeeds
    {
        std::vector<Point2D> path = {Point2D(50,50)};
        std::vector<Task> tasks = {
            Task(TaskType::NoFly, square, false)
        };
        double fit = evaluateFitness(path, tasks);
        assert(std::abs(fit - 500.0) < 1e-9);
    }

    // Case 4: No-fly task, path inside polygon fails
    {
        std::vector<Point2D> path = {Point2D(5,5)};
        std::vector<Task> tasks = {
            Task(TaskType::NoFly, square, false)
        };
        double fit = evaluateFitness(path, tasks);
        assert(std::abs(fit - 0.0) < 1e-9);
    }

    // Case 5: Ending task, final point inside triangle succeeds
    {
        std::vector<Point2D> path = {Point2D(0,0), Point2D(25,25)};
        std::vector<Task> tasks = {
            Task(TaskType::Ending, triangle, true)
        };
        double fit = evaluateFitness(path, tasks);
        assert(std::abs(fit - (500.0 + 500.0/2)) < 1e-9); // bonus = 500/2
    }

    // Case 6: Ending task, final point outside triangle fails
    {
        std::vector<Point2D> path = {Point2D(0,0), Point2D(10,10)};
        std::vector<Task> tasks = {
            Task(TaskType::Ending, triangle, false)
        };
        double fit = evaluateFitness(path, tasks);
        assert(std::abs(fit - 0.0) < 1e-9);
    }

    // Case 7: Mixed tasks, one fails so no shortness bonus applied
    {
        std::vector<Point2D> path = {Point2D(5,5), Point2D(25,25)};
        std::vector<Task> tasks = {
            Task(TaskType::FlyThrough, square, true),
            Task(TaskType::NoFly, triangle, true) // fails because path goes through triangle
        };
        double fit = evaluateFitness(path, tasks);
        assert(std::abs(fit - 500.0) < 1e-9); // only fly_through earns 500, no bonus
    }

    // Case 8: Point on edge counts as inside for fly_through
    {
        std::vector<Point2D> path = {Point2D(10,5)}; // exactly on right edge
        std::vector<Task> tasks = {
            Task(TaskType::FlyThrough, square, false)
        };
        double fit = evaluateFitness(path, tasks);
        assert(std::abs(fit - 500.0) < 1e-9);
    }

    // Case 9: Degenerate polygon (fewer than 3 vertices) never contains points
    {
        std::vector<Point2D> degenerate = {Point2D(0,0), Point2D(5,5)};
        std::vector<Point2D> path = {Point2D(1,1)};
        std::vector<Task> tasks = {
            Task(TaskType::FlyThrough, degenerate, true)
        };
        double fit = evaluateFitness(path, tasks);
        assert(std::abs(fit - 0.0) < 1e-9);
    }

    // Case 10: Empty tasks returns 0
    {
        std::vector<Point2D> path = {Point2D(1,1)};
        std::vector<Task> tasks;
        double fit = evaluateFitness(path, tasks);
        assert(std::abs(fit - 0.0) < 1e-9);
    }

    return 0;
}
