// Write a C++ function named `sameOrderDoubleInsertion` that takes a vector of integers and returns `true` if two intrusive doubly-linked lists can be constructed from those values—one using a base hook inserted in reverse vector order and one using a member hook inserted in forward vector order—such that the sequence of addresses stored in the base-hook list traversed from tail to head matches the original vector order, and the member-hook list traversed from head to tail also matches the original vector order. The function must create a temporary local vector of objects, build both lists using `boost::intrusive` lists with `list_base_hook<>` and `list_member_hook<>`, then verify both traversals match the original input order. Return `false` on any mismatch, and `true` otherwise. Assume the input vector is non-empty but may contain duplicate values. You may use the minimal necessary includes and must define your own small class inside the function or as a local struct.

// The solution approach is to define a local class that inherits from `boost::intrusive::list_base_hook<>` and also contains a public `boost::intrusive::list_member_hook<>` member. Then create a local `std::vector` of such objects, copying the input integers into them. Two intrusive lists are created: one using the base hook (via `boost::intrusive::list<MyClass>`) and one using the member hook (via `boost::intrusive::member_hook`). Insert the objects into the base-hook list using `push_front` to reverse order, and into the member-hook list using `push_back` to preserve order. Then iterate the base-hook list in reverse (`rbegin` to `rend`) and the member-hook list forward (`begin` to `end`), comparing the addresses of each element to the addresses in the original local vector. If any mismatch occurs, return `false`. Since the original vector’s order is preserved exactly and we insert in controlled manners, the test should always pass for a correct implementation, but we still implement the verification as specified. Edge cases: empty vector (although specified non-empty, we could handle with empty list checks), duplicates (no problem because addresses are unique). Time complexity is O(n) for insertion and O(n) for traversal verification, with O(n) auxiliary space for the local vector of objects.

#include <vector>
#include <boost/intrusive/list.hpp>

// Returns true if both base-hook reverse-order and member-hook forward-order
// intrusive lists match the original vector order by object address.
bool sameOrderDoubleInsertion(const std::vector<int>& input) {
    namespace bi = boost::intrusive;

    // Local class with both base and member hooks
    struct MyClass : bi::list_base_hook<> {
        int value;
        bi::list_member_hook<> member_hook;
        explicit MyClass(int v) : value(v) {}
    };

    // Create local vector of objects
    std::vector<MyClass> objects;
    objects.reserve(input.size());
    for (int v : input) {
        objects.emplace_back(v);
    }

    // Base hook list (stores MyClass directly)
    typedef bi::list<MyClass> BaseList;

    // Member hook option and list
    typedef bi::member_hook<MyClass, bi::list_member_hook<>, &MyClass::member_hook> MemberOption;
    typedef bi::list<MyClass, MemberOption> MemberList;

    BaseList baseList;
    MemberList memberList;

    // Insert in reverse order into baseList (push_front)
    for (auto it = objects.begin(); it != objects.end(); ++it) {
        baseList.push_front(*it);
    }

    // Insert in forward order into memberList (push_back)
    for (auto it = objects.begin(); it != objects.end(); ++it) {
        memberList.push_back(*it);
    }

    // Verify baseList from rbegin to rend matches original order
    {
        auto rbit = baseList.rbegin();
        auto it = objects.begin();
        for (; it != objects.end() && rbit != baseList.rend(); ++it, ++rbit) {
            if (&*rbit != &*it) return false;
        }
        if (it != objects.end() || rbit != baseList.rend()) return false;
    }

    // Verify memberList from begin to end matches original order
    {
        auto mit = memberList.begin();
        auto it = objects.begin();
        for (; it != objects.end() && mit != memberList.end(); ++it, ++mit) {
            if (&*mit != &*it) return false;
        }
        if (it != objects.end() || mit != memberList.end()) return false;
    }

    return true;
}

#include <cassert>
#include <vector>

// Forward declaration of the solution function
bool sameOrderDoubleInsertion(const std::vector<int>& input);

int main() {
    // Basic case
    assert(sameOrderDoubleInsertion({1, 2, 3, 4}) == true);

    // Reverse already sorted should still be true because we insert in reverse
    assert(sameOrderDoubleInsertion({4, 3, 2, 1}) == true);

    // Duplicates
    assert(sameOrderDoubleInsertion({5, 5, 5}) == true);

    // Single element
    assert(sameOrderDoubleInsertion({42}) == true);

    // Negative and zero
    assert(sameOrderDoubleInsertion({-1, 0, 2, -5}) == true);

    // Larger sequence with mixed values
    std::vector<int> large;
    for (int i = 0; i < 100; ++i) large.push_back(i * 7 % 53);
    assert(sameOrderDoubleInsertion(large) == true);

    // Non-empty vector with many elements (stress)
    std::vector<int> many;
    for (int i = 0; i < 1000; ++i) many.push_back(i);
    assert(sameOrderDoubleInsertion(many) == true);

    return 0;
}
