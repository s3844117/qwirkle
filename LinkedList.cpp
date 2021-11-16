#include "LinkedList.h"
#include "Node.h"
#include "Tile.h"

#include <random>

LinkedList::LinkedList() 
    : head(nullptr), tail(nullptr), _size(0) {}

LinkedList::~LinkedList() {
    Node* current = head;
    while (current) {
        Node* next = current->next;
        delete current;
        current = next;
    }
}

unsigned int LinkedList::size() const {
    return _size;
}

bool LinkedList::isEmpty() const {
    return _size == 0;
}

void LinkedList::addBack(Tile* tile) {
    if (tile) {
        Node* toAdd = new Node(tile, nullptr);
        if (!tail) {
            tail = toAdd;
            head = tail;
        } else {
            tail->next = toAdd;
            tail = tail->next;
        }
        ++_size;
    }
}

Tile* LinkedList::removeFront() {
    Tile* removed = nullptr;
    if (head) {
        Node* toRemove = head;
        head = toRemove->next;
        removed = toRemove->tile;
        toRemove->tile = nullptr;
        delete toRemove;
        --_size;
        if (_size == 0) {
            tail = nullptr;
        }
    }    
    return removed;
}

Tile* LinkedList::removeAt(unsigned int index) {
    Tile* removed = nullptr;
    if (index < _size) {
        if (index == 0) {
            removed = removeFront();
        } else {
            Node* current = head;
            for (unsigned int i = index; --i > 0;) {
                current = current->next;
            }
            Node* toRemove = current->next;
            current->next = toRemove->next;
            removed = toRemove->tile;
            toRemove->tile = nullptr;
            delete toRemove;
            if (index == _size - 1) {
                tail = current;
            }
            --_size;
        }
    }
    return removed;
}

Tile* LinkedList::remove(Tile& target) {
    Tile* removed = nullptr;
    Node* prev = nullptr;
    Node* current = head;
    while (current && *current->tile != target) {
        prev = current;
        current = current->next;
    }
    if (current) {
        Node* toRemove = current;
        if (!prev) {
            head = toRemove->next;
        } else {
            prev->next = toRemove->next;
            if (!toRemove->next) {
                tail = prev;
            }
        }
        removed = toRemove->tile;
        toRemove->tile = nullptr;
        delete toRemove;
        --_size;
    }
    return removed;
}

void LinkedList::shuffle() {
    std::random_device engine;
    for (unsigned int i = _size - 1; i != 0; --i) {
        std::uniform_int_distribution<unsigned int> uniform_dist(0, i);
        addBack(removeAt(uniform_dist(engine)));
    }
}

std::ostream& operator<<(std::ostream& os, const LinkedList& list) {
    for (Node* n = list.head; n; n = n->next) {
        os << (n != list.head ? "," : "") << *n->tile;
    }
    return os;
}