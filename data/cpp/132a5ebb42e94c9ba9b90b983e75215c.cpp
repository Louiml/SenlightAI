// Write a C++ function named `findBestActor` that takes a linked list of actors (each with a name and a fan count) and a maximum fan threshold. The function must return the actor with the highest fan count among those whose fan count is less than or equal to the threshold. If multiple actors have the same highest fan count, choose the one with the lexicographically smallest name. If no actor meets the threshold, return an actor with name `"NIC"` and fan count `-1`. The function must not modify the original list, must handle empty lists, and must be const-correct. The function should return an `Actor` object by value, where `Actor` is a class with `std::string name` and `long numberOfFans`, plus getters for both, and a constructor taking `(const std::string&, long)`.

#include <cassert>

int main() {
    // Build a simple linked list manually
    ActorNode* n1 = new ActorNode("Alice", 100);
    ActorNode* n2 = new ActorNode("Bob", 200);
    ActorNode* n3 = new ActorNode("Charlie", 150);
    ActorNode* n4 = new ActorNode("David", 100);
    n1->next = n2;
    n2->next = n3;
    n3->next = n4;

    ActorList list;
    list.head = n1;

    // Test 1: threshold 150 -> Bob has 200 (too high), Charlie has 150 (best)
    Actor result = findBestActor(list, 150);
    assert(result.getName() == "Charlie");
    assert(result.getNumberOfFans() == 150);

    // Test 2: threshold 100 -> Alice and David both have 100, Alice is lexicographically smaller
    result = findBestActor(list, 100);
    assert(result.getName() == "Alice");
    assert(result.getNumberOfFans() == 100);

    // Test 3: threshold 50 -> no actor qualifies
    result = findBestActor(list, 50);
    assert(result.getName() == "NIC");
    assert(result.getNumberOfFans() == -1);

    // Test 4: empty list
    ActorList emptyList;
    result = findBestActor(emptyList, 1000);
    assert(result.getName() == "NIC");
    assert(result.getNumberOfFans() == -1);

    // Test 5: threshold high enough -> Bob has highest
    result = findBestActor(list, 1000);
    assert(result.getName() == "Bob");
    assert(result.getNumberOfFans() == 200);

    // Clean up
    delete n1; delete n2; delete n3; delete n4;
    return 0;
}

#include <string>

class Actor {
public:
    Actor(const std::string& name, long numberOfFans)
        : name(name), numberOfFans(numberOfFans) {}

    const std::string& getName() const { return name; }
    long getNumberOfFans() const { return numberOfFans; }

private:
    std::string name;
    long numberOfFans;
};

// Node structure for singly linked list
struct ActorNode {
    Actor actor;
    ActorNode* next;
    ActorNode(const std::string& name, long fans, ActorNode* nxt = nullptr)
        : actor(name, fans), next(nxt) {}
};

// Minimal linked list wrapper with const access
class ActorList {
public:
    ActorNode* head = nullptr;

    // Const accessor to traverse without modification
    const ActorNode* getHead() const { return head; }
};

// Find best actor with fan count <= threshold.
// Returns Actor("NIC", -1) if no actor qualifies or list is empty.
Actor findBestActor(const ActorList& list, long maxNumberOfFans) {
    long recentMax = -1;
    std::string recentName = "NIC";
    bool found = false;

    const ActorNode* current = list.getHead();
    while (current != nullptr) {
        if (current->actor.getNumberOfFans() <= maxNumberOfFans) {
            if (!found || current->actor.getNumberOfFans() > recentMax) {
                recentMax = current->actor.getNumberOfFans();
                recentName = current->actor.getName();
                found = true;
            } else if (current->actor.getNumberOfFans() == recentMax &&
                       current->actor.getName() < recentName) {
                recentName = current->actor.getName();
            }
        }
        current = current->next;
    }

    if (found) {
        return Actor(recentName, recentMax);
    }
    return Actor("NIC", -1);
}

// The solution traverses the singly linked list once from head to tail. For each node, it checks if the actor's fan count is ≤ the threshold. If yes, it compares the fan count with the current best (recentMax). If the current fan count is strictly greater, update best name and count. If equal, compare names lexicographically (using `<` on `std::string`) and update if the current name is smaller. After the loop, if any actor was found, return an `Actor` constructed with the best name and count; otherwise, return `Actor("NIC", -1)`. Key edge cases: empty list (should return `"NIC"`, `-1`), all actors above threshold (same as empty), single matching actor, and ties broken by lexicographically smallest name. The function must accept the list by const reference to avoid copying and guarantee no modification. Time complexity is O(n), where n is the number of actors, since each node is visited once. Space complexity is O(1) beyond the returned `Actor` object, as only a few scalar and string values are kept.
