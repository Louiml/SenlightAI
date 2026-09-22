/*
Write a C++ function `decodeMorphology` that takes as input a string `geneticString` representing a sequence of component letters (`'C'`, `'B'`, `'J'`) and mounting/movement commands (`"mount_l"`, `"mount_f"`, `"mount_r"`, `"move_b"`, `"move_l"`, `"move_f"`, `"move_r"`), each separated by a space, along with an integer `maxComponents` specifying the maximum number of components allowed (including the root). The function must build a graph of components: each component has four connection slots (`left`, `front`, `right`, `back`) that can each point to at most one other component. Mounting commands attach the next encountered component letter to the current component in the specified direction (`l`, `f`, or `r`) if that slot is free and the current component supports that connection; otherwise, the command is ignored. For joints (`'J'`), only the `front` slot can be used for mounting, and only `'C'`/`'B'` components support side (`l`/`r`) and `front` attachments. Movement commands move the "current" pointer: `move_b` goes to the parent (back) of the current component (if not the root), `move_l`/`move_f`/`move_r` move to the connected component in that direction if it exists; otherwise, the move is ignored. The first component letter encountered becomes the root. The construction stops if the number of components reaches `maxComponents` (but still consume remaining commands? Yes, but do not add more components). Return a string representation of the final graph in the format: for each component in order of creation (root first, then others in creation order), output its letter and its four connections as `L:<letter or "none">,F:<letter or "none">,R:<letter or "none">,B:<letter or "none">` separated by a semicolon. For example, if the graph has root `C` with left `J` and front `B`, and the `J` has back `C`, output `C:L:J,F:B,R:none,B:none;J:L:none,F:none,R:none,B:C;B:L:none,F:none,R:none,B:C`. The input string is guaranteed to be non-empty and contain at least one component letter. You may assume `maxComponents >= 1`. Use a `struct Vertex` with fields `item`, `id`, and pointers `left`, `front`, `right`, `back`. The function should be `std::string decodeMorphology(const std::string& geneticString, int maxComponents)` and must be implemented in a self-contained way using only standard headers. Ensure that disconnected components (e.g., a letter after an ignored mount command) are not added to the graph; only components that are successfully mounted (or the root) are stored.
*/

#include <string>
#include <vector>
#include <sstream>

struct Vertex {
    char item;
    Vertex* left;
    Vertex* front;
    Vertex* right;
    Vertex* back;
    Vertex(char c) : item(c), left(nullptr), front(nullptr), right(nullptr), back(nullptr) {}
};

std::string decodeMorphology(const std::string& geneticString, int maxComponents) {
    std::istringstream iss(geneticString);
    std::string token;
    std::vector<Vertex*> vertices;
    Vertex* root = nullptr;
    Vertex* current = nullptr;
    std::string pendingMount;
    int componentCount = 0;

    while (iss >> token) {
        // Check if token is a component letter
        if (token.size() == 1 && (token[0] == 'C' || token[0] == 'B' || token[0] == 'J')) {
            if (componentCount >= maxComponents) {
                continue; // no more components allowed
            }
            Vertex* newVertex = new Vertex(token[0]);
            bool attached = false;
            if (root == nullptr) {
                // Create root
                newVertex->back = nullptr;
                root = newVertex;
                current = newVertex;
                vertices.push_back(newVertex);
                componentCount++;
                attached = true;
            } else if (!pendingMount.empty()) {
                char dir = pendingMount[0];
                // Determine if current supports mounting in this direction
                char currentItem = current->item;
                if (currentItem == 'C' || currentItem == 'B') {
                    // Can mount to l, f, r
                    if (dir == 'l' && current->left == nullptr) {
                        current->left = newVertex;
                        attached = true;
                    } else if (dir == 'f' && current->front == nullptr) {
                        current->front = newVertex;
                        attached = true;
                    } else if (dir == 'r' && current->right == nullptr) {
                        current->right = newVertex;
                        attached = true;
                    }
                } else if (currentItem == 'J') {
                    // Only front allowed
                    if (dir == 'f' && current->front == nullptr) {
                        current->front = newVertex;
                        attached = true;
                    }
                }
                if (attached) {
                    newVertex->back = current;
                    current = newVertex;
                    vertices.push_back(newVertex);
                    componentCount++;
                }
            }
            pendingMount.clear(); // clear after attempting a mount, whether successful or not
            (void)attached; // if not attached, vertex is leaked? We'll handle by deleting if not attached
            if (!attached && newVertex != root) {
                delete newVertex;
            }
        } else if (token.rfind("mount_", 0) == 0) {
            // mounting command
            if (token.size() >= 6) {
                pendingMount = token.substr(5, 1);
            }
        } else if (token.rfind("move_", 0) == 0) {
            // movement command
            if (current == nullptr) continue;
            std::string dir = token.substr(5);
            if (dir == "b") {
                if (current->back != nullptr) {
                    current = current->back;
                }
            } else if (dir == "l") {
                if (current->left != nullptr) current = current->left;
            } else if (dir == "f") {
                if (current->front != nullptr) current = current->front;
            } else if (dir == "r") {
                if (current->right != nullptr) current = current->right;
            }
        }
    }

    // Build result string
    std::string result;
    for (size_t i = 0; i < vertices.size(); ++i) {
        if (i > 0) result += ";";
        Vertex* v = vertices[i];
        result += v->item;
        result += ":L:";
        result += (v->left ? std::string(1, v->left->item) : "none");
        result += ",F:";
        result += (v->front ? std::string(1, v->front->item) : "none");
        result += ",R:";
        result += (v->right ? std::string(1, v->right->item) : "none");
        result += ",B:";
        result += (v->back ? std::string(1, v->back->item) : "none");
    }

    // Clean up memory
    for (Vertex* v : vertices) delete v;

    return result;
}

#include <cassert>
#include <string>

// The solution function is assumed to be declared above.

int main() {
    // Root only
    assert(decodeMorphology("C", 5) == "C:L:none,F:none,R:none,B:none");

    // Root with left and front
    assert(decodeMorphology("C mount_l J mount_f B", 10) == "C:L:J,F:B,R:none,B:none;J:L:none,F:none,R:none,B:C;B:L:none,F:none,R:none,B:C");

    // Mount to occupied slot: ignored
    assert(decodeMorphology("C mount_l J mount_l B", 10) == "C:L:J,F:none,R:none,B:none;J:L:none,F:none,R:none,B:C");

    // Joint only accepts front: left command ignored
    assert(decodeMorphology("C mount_f J mount_l B mount_f X", 10) == "C:F:J,B:none,L:none,R:none;J:F:X,B:C,L:none,R:none;X:B:J,F:none,L:none,R:none");

    // Movement back to parent
    assert(decodeMorphology("C mount_l J move_b B", 10) == "C:L:J,F:none,R:none,B:none;J:L:none,F:none,R:none,B:C");

    // Movement to left and then front
    assert(decodeMorphology("C mount_l J move_l B mount_f X", 10) == "C:L:J,F:none,R:none,B:none;J:F:X,B:C,L:none,R:none;X:B:J,F:none,L:none,R:none");

    // maxComponents limit
    assert(decodeMorphology("C mount_l J mount_l B", 2) == "C:L:J,F:none,R:none,B:none;J:L:none,F:none,R:none,B:C");

    // Multiple components with branches
    assert(decodeMorphology("C mount_l J mount_f B mount_r X", 10) == "C:L:J,F:none,R:none,B:none;J:F:B,B:C,L:none,R:none;B:J:F:none,L:none,R:none");

    // Move to non-existent slot ignored
    assert(decodeMorphology("C mount_f J move_r B", 10) == "C:F:J,F:none,R:none,B:none;J:B:C,F:none,L:none,R:none");

    return 0;
}

// The solution parses the input string into tokens (split by spaces). Iterate through tokens in order. Maintain a `vector<Vertex*>` to store all created components in creation order. Maintain a `current` pointer that starts as `nullptr`. For each token: if it is a single letter `'C'`, `'B'`, or `'J'`, create a new `Vertex` and attempt to attach it. The root is created if `current` is `nullptr`; set `root` to it and `current` to it. Otherwise, if there is a pending mounting command (stored in a variable, initially empty), attempt to attach the new vertex to `current` based on the command. For `'C'`/`'B'` current components: if mounting to `l`/`f`/`r`, check that the corresponding slot is `nullptr`; if free, set it, set `newVertex->back = current`, and make `current = newVertex`. If the slot is occupied, ignore the new vertex (do not store it, do not change current). For `'J'` current components: only `f` is allowed; if `front` is free, attach, else ignore. For `'C'`/`'B'`, there is also a special rule: if the current component is the root and all three slots `left`, `front`, `right` are non-null, and the mounting command is `l`, `f`, or `r`, but the back is `nullptr`, you may attach to the back (since the snippet does this). However, the task description does not mention this special case; to keep it simple, we follow only the explicit described rules: no back attachment except via `move_b`? Actually the snippet also supports mounting to back when root is full. But the task says "each component has four connection slots" but does not mention a mounting command for back. So we ignore back mounting. So we just follow the described: mounting commands are `l`, `f`, `r`. After a successful mount, clear the pending mounting command. If the letter is processed but no valid mount, ignore it and clear the pending command? Actually the snippet: if mountingcommand is non-empty, it attempts to mount; if it violates, it goes to label `violation` and the command is ignored, but the new letter is not added. So clear the pending command after attempting a mount (whether successful or not). If there is no pending mounting command, ignore the letter entirely. Commands are of two types: tokens starting with `"mount"` (exactly `"mount_l"`, `"mount_f"`, `"mount_r"`) set `pendingMount` to the last character (`l`,`f`,`r`). Tokens starting with `"move"` (`"move_b"`, `"move_l"`, `"move_f"`, `"move_r"`) perform movement: for `b`, if `current->back != nullptr` and `current != root` (or simply if back not null, since root's back is null), move. For others, check corresponding pointer not null and move. Also, when a new component is successfully added, increment `componentCount`. If `componentCount >= maxComponents`, then any subsequent tokens that are letters should not add new vertices, but we still must consume commands? The snippet does not break the loop; it just stops adding. So we continue the loop but ignore all subsequent letters (and still process commands). For simplicity, in our implementation, after reaching maxComponents, we ignore letters entirely and still process move/mount commands (mount commands set pending, but since no new letters will be mounted, they are just ignored). The returned string: iterate over `vertices` in creation order (root first). For each vertex, output its item letter and then `L:` followed by the letter of `left` (or `"none"` if null), similarly for `F`, `R`, `B`. Separate components with `;`. Edge cases: The root might have no back (null). A vertex might be created but never attached? We only store after successful attachment. The pending mounting command should persist if a letter is ignored? In the snippet, if a mounting command is set and then a command token appears (not a letter), the mountingcommand remains and pairs with the next possible letter. Similarly, if a mounting command is set and the next token is a movement, the mountingcommand remains. So we should not clear `pendingMount` when seeing a movement command; it only gets cleared after a letter is processed (whether successful or not). For successful mount, we also set new vertex's back to current. Also, we set `current = newVertex` only on successful mount. Time complexity: O(n) where n is number of tokens. Space complexity: O(m) for m components stored.
