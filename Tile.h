#ifndef ASSIGN2_TILE_H
#define ASSIGN2_TILE_H

#include <iostream>

// Define a Colour type
typedef char Colour;

// Define a Shape type
typedef int Shape;

class Tile {
public:
    Tile(Colour colour, Shape shape);
    ~Tile();

    Colour getColour() const;
    Shape getShape() const;

    bool operator==(const Tile& rhs) const;
    bool operator!=(const Tile& rhs) const;

    // Helper method to check the tile code and what unicode should be displayed
    std::string shapeReturn(Tile* tile);

    // Method to display the tile as unicode
    std::ostream& tileDisplayforGame(std::ostream& os, Tile* tile);

    // Output operator overloading used for saving Tile information.
    friend std::ostream& operator<<(std::ostream& os, const Tile& tile);

    Colour colour;
    Shape  shape;
};

#endif // ASSIGN2_TILE_H
