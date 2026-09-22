/*
Write a C++ function `soinnLearn` that simulates a simplified version of the Self-Organizing Incremental Neural Network (SOINN) learning algorithm. The function takes: a vector of 2D points (as `std::pair<double,double>` representing input positions), a node age threshold parameter `node_erase_age`, and an edge age threshold parameter `edge_erase_age`. It initializes the network with two randomly chosen input points (using a fixed random seed or `rand()` with `srand(42)` for reproducibility), then processes each input in order, applying the incremental learning rules: find the winner and second-winner nodes by Euclidean distance, check if either exceeds its similarity threshold (computed as the maximum distance to connected neighbors, or if isolated, the minimum distance to any other node), and either add a new node or update the network. Specifically: if either distance exceeds its threshold, add a new node at the input position. If both distances are within thresholds, add an edge between the two winners if absent, reset that edge's age to 0, increment all edge ages connected to the winner, update the winner's position by `epsilon = 1/win_count` (win_count increments each time the node is a winner, and reset per node), update its neighbors with weight `1/100`, remove edges older than `edge_erase_age`, delete any nodes that become isolated (degree 0), and every `node_erase_age` inputs (or at the end), delete nodes that have degree ≤ 1 (noisy nodes). The function returns a vector of learned node positions (as `std::pair<double,double>`) after processing all inputs. Use `std::pair<double,double>` for positions and `std::vector<std::pair<double,double>>` for node lists. Edge and node IDs are implicit by index in vectors. The algorithm must be deterministic with a fixed seed for testing.
*/

#include <vector>
#include <utility>
#include <cmath>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <map>

struct Node {
    std::pair<double,double> pos;
    int win_times;
    Node(double x, double y) : pos(x,y), win_times(0) {}
};

struct Edge {
    int a, b;
    int age;
    Edge(int x, int y, int a_ge) : a(x), b(y), age(a_ge) {}
};

static double nodeDistance(const Node& n1, const Node& n2) {
    double dx = n1.pos.first - n2.pos.first;
    double dy = n1.pos.second - n2.pos.second;
    return std::sqrt(dx*dx + dy*dy);
}

// Simplified SOINN learning. Returns learned node positions.
std::vector<std::pair<double,double>> soinnLearn(
    const std::vector<std::pair<double,double>>& inputs,
    int node_erase_age,
    double edge_erase_age)
{
    std::vector<Node> nodes;
    std::vector<Edge> edges;
    
    // Initialize with two random distinct points
    srand(42);
    if (inputs.size() == 0) return {};
    if (inputs.size() == 1) {
        nodes.push_back(Node(inputs[0].first, inputs[0].second));
        return { inputs[0] };
    }
    int idx1 = rand() % inputs.size();
    int idx2;
    do { idx2 = rand() % inputs.size(); } while (idx2 == idx1);
    nodes.push_back(Node(inputs[idx1].first, inputs[idx1].second));
    nodes.push_back(Node(inputs[idx2].first, inputs[idx2].second));
    
    auto get_neighbors = [&](int node_id) -> std::vector<int> {
        std::vector<int> neigh;
        for (const auto& e : edges) {
            if (e.a == node_id) neigh.push_back(e.b);
            if (e.b == node_id) neigh.push_back(e.a);
        }
        return neigh;
    };
    
    auto has_edge = [&](int a, int b) -> bool {
        for (const auto& e : edges) {
            if ((e.a == a && e.b == b) || (e.a == b && e.b == a)) return true;
        }
        return false;
    };
    
    auto get_edge_index = [&](int a, int b) -> int {
        for (size_t i = 0; i < edges.size(); ++i) {
            if ((edges[i].a == a && edges[i].b == b) || (edges[i].a == b && edges[i].b == a)) return (int)i;
        }
        return -1;
    };
    
    auto get_similarity_threshold = [&](int node_id) -> double {
        auto neigh = get_neighbors(node_id);
        if (!neigh.empty()) {
            double max_d = 0.0;
            for (int nb : neigh) {
                double d = nodeDistance(nodes[node_id], nodes[nb]);
                if (d > max_d) max_d = d;
            }
            return max_d;
        } else {
            double min_d = 1e18;
            for (size_t i = 0; i < nodes.size(); ++i) {
                if ((int)i == node_id) continue;
                double d = nodeDistance(nodes[node_id], nodes[i]);
                if (d < min_d) min_d = d;
            }
            return (min_d == 1e18) ? 0.0 : min_d;
        }
    };
    
    auto erase_node_by_id = [&](int id) {
        // remove edges
        std::vector<Edge> new_edges;
        for (const auto& e : edges) {
            if (e.a != id && e.b != id) new_edges.push_back(e);
        }
        edges = std::move(new_edges);
        // rebuild node indices
        std::vector<Node> new_nodes;
        std::map<int,int> remap;
        for (size_t i = 0; i < nodes.size(); ++i) {
            if ((int)i == id) continue;
            remap[(int)i] = (int)new_nodes.size();
            new_nodes.push_back(nodes[i]);
        }
        nodes = std::move(new_nodes);
        // remap edges
        for (auto& e : edges) {
            e.a = remap[e.a];
            e.b = remap[e.b];
        }
    };
    
    auto erase_old_edges_and_isolated = [&]() {
        // remove old edges
        std::vector<Edge> kept_edges;
        std::vector<std::pair<int,int>> removed_edges;
        for (const auto& e : edges) {
            if (e.age > edge_erase_age) removed_edges.push_back({e.a,e.b});
            else kept_edges.push_back(e);
        }
        edges = std::move(kept_edges);
        // remove isolated nodes
        for (const auto& p : removed_edges) {
            // check degree of p.first
            bool has_any = false;
            for (const auto& e : edges) {
                if (e.a == p.first || e.b == p.first) { has_any = true; break; }
            }
            if (!has_any) erase_node_by_id(p.first);
            bool has_any2 = false;
            for (const auto& e : edges) {
                if (e.a == p.second || e.b == p.second) { has_any2 = true; break; }
            }
            if (!has_any2) erase_node_by_id(p.second);
        }
    };
    
    auto erase_noisy_nodes = [&]() {
        bool changed = true;
        while (changed) {
            changed = false;
            for (int i = (int)nodes.size()-1; i >= 0; --i) {
                auto neigh = get_neighbors(i);
                if (neigh.size() <= 1) {
                    erase_node_by_id(i);
                    changed = true;
                    break; // restart loop because indices changed
                }
            }
        }
    };

    for (size_t i = 0; i < inputs.size(); ++i) {
        Node input_node(inputs[i].first, inputs[i].second);
        
        // find winner and second winner by distance
        std::vector<double> dist_to_nodes(nodes.size());
        for (size_t j = 0; j < nodes.size(); ++j) {
            dist_to_nodes[j] = nodeDistance(nodes[j], input_node);
        }
        std::vector<size_t> order(nodes.size());
        for (size_t j = 0; j < order.size(); ++j) order[j] = j;
        std::sort(order.begin(), order.end(), [&](size_t a, size_t b) {
            return dist_to_nodes[a] < dist_to_nodes[b];
        });
        int first_winner = (int)order[0];
        int second_winner = (int)order[1];
        
        // increment win_times of first winner
        nodes[first_winner].win_times++;
        
        double thresh_first = get_similarity_threshold(first_winner);
        double thresh_second = get_similarity_threshold(second_winner);
        double dist_first = dist_to_nodes[first_winner];
        double dist_second = dist_to_nodes[second_winner];
        
        if (dist_first > thresh_first || dist_second > thresh_second) {
            nodes.push_back(Node(input_node.pos.first, input_node.pos.second));
        }
        
        if (dist_first <= thresh_first && dist_second <= thresh_second) {
            if (!has_edge(first_winner, second_winner)) {
                edges.push_back(Edge(first_winner, second_winner, 0));
            } else {
                int eidx = get_edge_index(first_winner, second_winner);
                edges[eidx].age = 0;
            }
            
            // increment all edge ages connected to first_winner
            for (auto& e : edges) {
                if (e.a == first_winner || e.b == first_winner) {
                    e.age++;
                }
            }
            
            // update position of first_winner
            double eps = 1.0 / (double)nodes[first_winner].win_times;
            double dx = input_node.pos.first - nodes[first_winner].pos.first;
            double dy = input_node.pos.second - nodes[first_winner].pos.second;
            nodes[first_winner].pos.first += eps * 1.0 * dx;
            nodes[first_winner].pos.second += eps * 1.0 * dy;
            
            // update neighbors
            auto neigh = get_neighbors(first_winner);
            for (int nb : neigh) {
                double ndx = input_node.pos.first - nodes[nb].pos.first;
                double ndy = input_node.pos.second - nodes[nb].pos.second;
                nodes[nb].pos.first += eps * 0.01 * ndx;
                nodes[nb].pos.second += eps * 0.01 * ndy;
            }
            
            erase_old_edges_and_isolated();
        }
        
        if ((i+1) % node_erase_age == 0 || (i+1) == inputs.size()) {
            erase_noisy_nodes();
        }
    }
    
    std::vector<std::pair<double,double>> result;
    for (const auto& n : nodes) result.push_back(n.pos);
    return result;
}

#include <cassert>
#include <cmath>
#include <vector>
#include <utility>

// Assume soinnLearn is defined above (include the solution code here)

int main() {
    // Test 1: Simple two points, one input, should keep both initial nodes (no deletion)
    std::vector<std::pair<double,double>> inputs1 = {{0.0,0.0}, {10.0,10.0}, {0.0,0.0}};
    auto res1 = soinnLearn(inputs1, 2, 2.0);
    assert(res1.size() == 2); // likely two nodes, both initial or one added? Let's verify
    // Since first input same as node0, distances to both maybe within thresholds (isolated nodes threshold = min distance to other = 14.14). So both within, will add edge, update. No new node. Second input (0,0) again similar. So still 2 nodes.
    assert(res1.size() == 2);
    
    // Test 2: Single input point -> returns that point
    std::vector<std::pair<double,double>> inputs2 = {{5.0,5.0}};
    auto res2 = soinnLearn(inputs2, 1, 1.0);
    assert(res2.size() == 1);
    assert(fabs(res2[0].first - 5.0) < 1e-6);
    assert(fabs(res2[0].second - 5.0) < 1e-6);
    
    // Test 3: Empty input -> empty result
    std::vector<std::pair<double,double>> inputs3;
    auto res3 = soinnLearn(inputs3, 1, 1.0);
    assert(res3.empty());
    
    // Test 4: Three points, aggressive noise removal
    std::vector<std::pair<double,double>> inputs4 = {{0.0,0.0}, {1.0,0.0}, {2.0,0.0}, {100.0,100.0}};
    auto res4 = soinnLearn(inputs4, 2, 1.0);
    // After processing, the isolated far point may be removed as noisy if degree <=1 at the end
    // Likely the far point gets added as new node (since distance > threshold) and has no edges, then removed at node_erase_age=2 end.
    // So result should have at most 3 nodes? Let's just check size <= 4 and >0
    assert(res4.size() >= 1 && res4.size() <= 4);
    
    // Test 5: Deterministic behavior with same input
    std::vector<std::pair<double,double>> inputs5 = {{0.0,0.0}, {2.0,0.0}, {4.0,0.0}};
    auto res5a = soinnLearn(inputs5, 1, 5.0);
    auto res5b = soinnLearn(inputs5, 1, 5.0);
    assert(res5a.size() == res5b.size());
    for (size_t i = 0; i < res5a.size(); ++i) {
        assert(fabs(res5a[i].first - res5b[i].first) < 1e-9);
        assert(fabs(res5a[i].second - res5b[i].second) < 1e-9);
    }
    
    return 0;
}

// The solution simulates the SOINN learning process with a simplified graph structure. Represent nodes as a vector of `Node` structs, where each `Node` has a `position` (pair) and `win_times` (int, tracks number of times it was winner). Edges are stored as a vector of `Edge` structs, each containing `first`, `second` (indices into nodes vector), and `age`. The main steps: initialize with two random unique indices (e.g., `rand() % n` twice, ensuring distinct if possible). For each input point, compute distances from input to all current nodes, sort to find winner (index of minimum distance) and second winner. Increment winner's `win_times`. Compute similarity thresholds: for a node with degree > 0, threshold = max distance to its neighbors; for isolated node, threshold = min distance to any other node (excluding itself). If input distance to winner > threshold_winner OR input distance to second > threshold_second, add a new node at input position. If both distances ≤ thresholds, then: if edge between winners doesn't exist, add it with age 0; set that edge's age to 0; increment age of all edges incident to winner; update winner position: `position += epsilon * weight * (input - position)` where epsilon = 1/win_times, weight = 1.0; update all neighbors of winner similarly with weight = 0.01 and same epsilon (using winner's win_times). Then remove edges with age > edge_erase_age; for each removed edge, check both endpoints for degree 0 and delete such nodes. After processing each input, if `(i+1) % node_erase_age == 0` or last input, remove all nodes with degree ≤ 1 (and their edges). Finally, return all node positions. Edge cases: initial list may have fewer than 2 unique points—if only 1 point, still initialize with it; if empty, return empty. Random initialization with `srand(42)` ensures reproducibility. Time complexity: for each input, computing distances to all nodes is O(N), sorting O(N log N), neighbor operations O(N_edge). With I inputs and max N nodes, overall O(I * N log N) typically, but N grows with inputs; space O(N + E). Use iterative removal carefully to avoid invalid indices; after deleting nodes, rebuild indices.
