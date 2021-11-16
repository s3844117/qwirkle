#include "Tile.h"
#include "TileCodes.h"

Tile::Tile(Colour colour, Shape shape) 
    : colour(colour), shape(shape) {}

Tile::~Tile() {}

bool Tile::operator==(const Tile& rhs) const {
    return colour == rhs.colour 
        && shape == rhs.shape;
}

bool Tile::operator!=(const Tile& rhs) const {
    return !(*this == rhs);
}

Colour Tile::getColour() const {
    return colour;
}

Shape Tile::getShape() const {
    return shape;
}

std::ostream& operator<<(std::ostream& os, const Tile& tile) {
    if (tile.colour == RED) {
        os << "\e[0;31m";
    } else if (tile.colour == ORANGE) {
        os << "\e[38;5;215m";
    } else if (tile.colour == YELLOW) {
        os << "\e[0;33m";
    } else if (tile.colour == GREEN) {
        os << "\e[0;92m";
    } else if (tile.colour == BLUE) {
        os << "\e[0;34m";
    } else if (tile.colour == PURPLE) {
        os << "\e[0;35m";
    }
    return os << tile.colour << tile.shape << "\033[m";
}

std::ostream& Tile::tileDisplayforGame(std::ostream& os, Tile* tile){
    std::string shape = shapeReturn(tile);
    if(tile->getColour() == RED){
        os << "\e[0;31m" << shape << "\033[m";
    }
    else if(tile->getColour() == ORANGE){
        os << "\e[38;5;215m" << shape << "\033[m";
    }
    else if(tile->getColour() == YELLOW){
        os << "\e[0;33m" << shape << "\033[m";
    }
    else if(tile->getColour() == GREEN){
        os << "\e[0;92m" << shape << "\033[m";
    }
    else if(tile->getColour() == BLUE){
        os << "\e[0;34m" << shape << "\033[m";
    }
    else if(tile->getColour() == PURPLE){
        os << "\e[0;35m" << shape << "\033[m";
    }
    return os;
}
std::string Tile::shapeReturn(Tile* tile){
    std::string shape;
    if (tile->getShape() == CIRCLE)
        {
            shape = "੦ ";
        }
    else if (tile->getShape() == STAR_4)
        {
            shape = "✦ ";
        }
    else if (tile->getShape() == DIAMOND)
        {
            shape = "◆ ";
        }
    else if (tile->getShape()== SQUARE)
        {
            shape = "⧈ ";
        }
    else if (tile->getShape() == STAR_6)
        {
            shape = "✶ ";
        }
    else if (tile->getShape() == CLOVER)
        {
            shape = "♧ ";
        }
    return shape;
}