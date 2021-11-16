#include "Node.h"
#include "Tile.h"

Node::Node(Tile* tile, Node* next)
    : tile(tile), next(next) {}

Node::~Node() {
    delete tile;
}
