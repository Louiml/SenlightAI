/*
Write a C++ function named `removeEmptyLists` that takes a `std::vector<t_id_list*>` (where `t_id_list` is an alias for `std::list<size_t>`) and removes from the vector any pointer that is either `nullptr` or points to an empty list. The function must delete the dynamically allocated list object for any removed entry to prevent memory leaks, and return the modified vector by value (or by reference to the input if you prefer, but for the standalone task, return the cleaned vector). The function should work correctly when the input vector contains a mix of valid non-empty lists, empty lists, and `nullptr`. Use `std::remove_if` with a helper predicate and handle edge cases like an empty input vector.
*/
#include <vector>
#include <list>
#include <algorithm>

using t_id_list = std::list<size_t>;

// Helper predicate: returns true if the list pointer is null or points to an empty list.
// It deletes the list object if the entry is to be removed to prevent memory leaks.
bool IsEmptyList(t_id_list* list) {
    bool result = (list == nullptr || list->begin() == list->end());
    if (result && list != nullptr) {
        delete list;
    }
    return result;
}

// Removes all null pointers and pointers to empty lists from the vector.
// The function returns a new vector containing only the valid, non-empty lists.
std::vector<t_id_list*> removeEmptyLists(const std::vector<t_id_list*>& input) {
    std::vector<t_id_list*> result = input;  // copy to avoid modifying caller's data
    result.erase(
        std::remove_if(result.begin(), result.end(), IsEmptyList),
        result.end()
    );
    return result;
}
#include <cassert>
#include <vector>
#include <list>
#include <iostream>

using t_id_list = std::list<size_t>;

// Declare the function (already defined above in solution section; for test we assume it's included)
std::vector<t_id_list*> removeEmptyLists(const std::vector<t_id_list*>& input);

int main() {
    // Test 1: Empty input vector
    std::vector<t_id_list*> empty;
    auto result1 = removeEmptyLists(empty);
    assert(result1.empty());

    // Test 2: All empty and null pointers
    std::vector<t_id_list*> all_empty;
    all_empty.push_back(nullptr);
    all_empty.push_back(new t_id_list());  // empty list
    all_empty.push_back(nullptr);
    auto result2 = removeEmptyLists(all_empty);
    assert(result2.empty());
    // The function should have deleted the empty list, but we can't check memory here.

    // Test 3: Mix of valid, empty, and null
    std::vector<t_id_list*> mixed;
    t_id_list* valid1 = new t_id_list();
    valid1->push_back(42);
    t_id_list* valid2 = new t_id_list();
    valid2->push_back(1);
    valid2->push_back(2);
    t_id_list* empty_list = new t_id_list();
    mixed.push_back(valid1);
    mixed.push_back(nullptr);
    mixed.push_back(empty_list);
    mixed.push_back(valid2);
    mixed.push_back(nullptr);

    auto result3 = removeEmptyLists(mixed);
    assert(result3.size() == 2);
    assert(result3[0] == valid1);
    assert(result3[1] == valid2);
    assert(result3[0]->size() == 1);
    assert(result3[1]->size() == 2);

    // Clean up the remaining valid lists (since removeEmptyLists returns copies of pointers)
    for (auto ptr : result3) {
        delete ptr;
    }

    // Note: The original mixed vector's pointers to empty/null were handled, but valid pointers are still owned by caller.
    // In a real program, ownership would be managed consistently. Here we clean up valid ones.

    // Test 4: All valid non-empty
    std::vector<t_id_list*> all_valid;
    t_id_list* a = new t_id_list();
    a->push_back(1);
    t_id_list* b = new t_id_list();
    b->push_back(2);
    b->push_back(3);
    all_valid.push_back(a);
    all_valid.push_back(b);
    auto result4 = removeEmptyLists(all_valid);
    assert(result4.size() == 2);
    assert(result4[0] == a);
    assert(result4[1] == b);
    delete a;
    delete b;

    std::cout << "All tests passed!" << std::endl;
    return 0;
}
// The solution iterates through the vector of raw pointers. For each pointer, it must decide whether to keep it or remove it. A pointer is considered "empty" if it is `nullptr` or if the list it points to has no elements (`begin() == end()`). When removing, we must delete the allocated list object to avoid memory leaks, but be careful not to delete `nullptr` (though `delete nullptr` is safe in C++). The main algorithm uses `std::remove_if` to partition the vector: it moves all elements that satisfy the "IsEmpty" predicate to the end, then `erase` removes them. The predicate must delete the list object when it returns `true`. Edge cases: (1) empty input vector — no removals, returns empty; (2) all entries empty or `nullptr` — vector becomes empty; (3) all entries valid — no changes; (4) `nullptr` handling — the predicate must check for `nullptr` before dereferencing. Time complexity is O(n*m) where n is the number of entries and m is the average list size (since checking emptiness is O(1) but `begin()==end()` is O(1) for lists, actually O(n) overall). Space complexity is O(1) auxiliary (ignoring the input).
