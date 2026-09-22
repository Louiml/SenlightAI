// Write a C++ function that takes a pointer to a doubly linked list descriptor (with `ini`, `fim`, and `tam` fields, where each node stores an integer and has `ant` and `prox` pointers) and returns a new doubly linked list descriptor containing a deep copy of the original list — meaning each node in the returned list is a newly allocated node with the same integer value and in the same order, but independent memory. The original list must remain unchanged. The function should handle an empty list correctly (return an empty list) and must not share any node pointers between the two lists. The provided descriptor and node structures are given exactly as in the snippet; you should implement the copy function and any helper functions needed (like creating a descriptor and inserting at the end), but you do not need to replicate the main program.

#include <cassert>
#include <iostream>

// The solution functions are assumed to be defined above.

int main() {
    // Test 1: Empty list copy
    descritor* original = criarlista();
    descritor* copied = copiarLista(original);
    assert(copied->tam == 0);
    assert(copied->ini == nullptr);
    assert(copied->fim == nullptr);
    delete copied;
    delete original; // clean up descriptors (no nodes)

    // Test 2: Single element list
    original = criarlista();
    inserir_fim(original, 42);
    copied = copiarLista(original);
    assert(copied->tam == 1);
    assert(copied->ini != nullptr);
    assert(copied->fim == copied->ini);
    assert(copied->ini->dado == 42);
    assert(copied->ini->ant == nullptr);
    assert(copied->ini->prox == nullptr);
    // Ensure deep copy: nodes are different addresses
    assert(copied->ini != original->ini);
    // Cleanup nodes and descriptors
    delete copied->ini;
    delete copied;
    delete original->ini;
    delete original;

    // Test 3: Multiple elements, verify order, links, and deep copy
    original = criarlista();
    inserir_fim(original, 10);
    inserir_fim(original, 20);
    inserir_fim(original, 30);
    copied = copiarLista(original);
    assert(copied->tam == 3);
    nolista* p_orig = original->ini;
    nolista* p_copy = copied->ini;
    for (int i = 0; i < 3; ++i) {
        assert(p_copy != nullptr);
        assert(p_orig != nullptr);
        assert(p_copy->dado == p_orig->dado);
        assert(p_copy != p_orig); // deep copy
        p_orig = p_orig->prox;
        p_copy = p_copy->prox;
    }
    assert(p_orig == nullptr);
    assert(p_copy == nullptr);
    // Check ant links in copied
    assert(copied->ini->ant == nullptr);
    assert(copied->fim->prox == nullptr);
    assert(copied->fim->ant != nullptr && copied->fim->ant->dado == 20);
    // Modify original and ensure copy unaffected
    original->ini->dado = 99;
    assert(copied->ini->dado == 10);

    // Cleanup copied list
    nolista* current = copied->ini;
    while (current != nullptr) {
        nolista* next = current->prox;
        delete current;
        current = next;
    }
    delete copied;
    // Cleanup original list
    current = original->ini;
    while (current != nullptr) {
        nolista* next = current->prox;
        delete current;
        current = next;
    }
    delete original;

    std::cout << "All tests passed." << std::endl;
    return 0;
}

#include <cstddef>

struct nolista {
    int dado;
    nolista* ant;
    nolista* prox;
};

struct descritor {
    nolista* ini;
    nolista* fim;
    int tam;
};

descritor* criarlista() {
    descritor* novo = new descritor;
    novo->ini = nullptr;
    novo->fim = nullptr;
    novo->tam = 0;
    return novo;
}

void inserir_fim(descritor* l, int dado) {
    nolista* novo = new nolista;
    novo->dado = dado;
    novo->ant = nullptr;
    novo->prox = nullptr;
    if (l->ini == nullptr) {
        l->ini = novo;
        l->fim = novo;
    } else {
        l->fim->prox = novo;
        novo->ant = l->fim;
        l->fim = novo;
    }
    l->tam++;
}

// Return a deep copy of the doubly linked list `l`.
descritor* copiarLista(const descritor* l) {
    descritor* copia = criarlista();
    nolista* p = l->ini;
    while (p != nullptr) {
        inserir_fim(copia, p->dado);
        p = p->prox;
    }
    return copia;
}

// The solution iterates through the original list from the head (`ini`) to the tail (`fim`) using a temporary pointer. For each node encountered, it reads the integer value and inserts a new node into a newly created descriptor using a helper insertion function that appends to the end, maintaining both `ant` and `prox` links as well as updating the list size. Since we traverse all nodes exactly once, the time complexity is O(n) where n is the number of nodes. The auxiliary space complexity is O(n) as well because we allocate one new node per original node. Edge cases: if the input list is empty (either `ini` is null or `tam` is 0), the function returns a valid empty list (with `ini` and `fim` null, `tam` 0). The original list is not modified at all, so no dangling pointers or aliasing issues arise. Memory management: it is the caller's responsibility to delete the returned list’s nodes and descriptor to avoid leaks. The function must ensure correct `ant` links: when inserting the first node, both `ant` and `prox` are null; subsequent nodes have their `ant` set to the previous tail.
