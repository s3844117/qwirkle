#ifndef ASSIGN2_LINKEDLIST_H
#define ASSIGN2_LINKEDLIST_H

#include <iostream>

class Node;
class Tile;

class LinkedList {
public:
    LinkedList();
    ~LinkedList();

    // Returns size of linked list.
    unsigned int size() const;

    // Returns whether linked list is empty.
    bool isEmpty() const;

    // Adds `tile` to back of linked list.
    void addBack(Tile* tile);

    // Removes and returns the tile at the front of the linked list, or nullptr 
    // if linked list is empty. Transfers ownership of tile to caller.
    Tile* removeFront();

    // Removes and returns the tile at `index` of linked list, or nullptr 
    // if `index` does not exist. Transfers ownership of tile to caller.
    Tile* removeAt(unsigned int index);

    // Removes and returns the tile in linked list matching the given `tile`.
    // In the case of multiple matches, the first match is selected. 
    // Transfers ownership of tile to caller.
    Tile* remove(Tile& tile);

    // Randomly shuffles order of elements contained in linked list.
    void shuffle();

    // Output operator overloading used for saving contents of bag and hand.
    friend std::ostream& operator<<(std::ostream& os, const LinkedList& list);

private:
    Node* head;

    // Allows for constant add to back
    Node* tail;

    // Allows for contant access of size of linked list. 
    // unsigned int enforces lower bound of valid size values.
    unsigned int _size;
};

#endif // ASSIGN2_LINKEDLIST_H
