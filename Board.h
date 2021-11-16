#ifndef ASSIGN2_BOARD_H
#define ASSIGN2_BOARD_H

#include <vector>
#include <iostream>

class Tile;

class Board {
public:
    Board();
    ~Board();

    // Returns whether the board is empty.
    bool isEmpty();

    // Adds the given `tile` to board at position given by `row` and `col`. 
    // Ownership of the `tile` is transferred to the board.
    bool addAt(Tile* tile, unsigned int row, unsigned int col);

    // Confirms that the given `tile` can be placed at the position given by
    // `row` and `col`. A move is invalid if the `tile` will not be adjacent 
    // to any a previously placed tile that has the same colour or shape, or
    // if any line formed vertically and horizontally from the `tile` contains 
    // another tile of exactly the same colour and shape. Score for move is 
    // calculated based on number of tiles in each valid line.
    bool isValidMove(Tile& tile, unsigned int row, unsigned int col, 
                     int& score);

    // Prints board state to console, including axis labels.
    void display();

    // Output operator overloading used for saving board state.
    friend std::ostream& operator<<(std::ostream& os, const Board& board);

private:
    std::vector<std::vector<Tile*>> board;
    
    // The number of tiles that have been placed on the board.
    unsigned int tilesPlaced;

    // Confirms whether position given by `row` and `col` is within bounds of
    // board and that a tile has not previously been placed there.
    bool isValidPosition(unsigned int row, unsigned int col);

    // Returns whether the lines formed by placement of `tile` at position 
    // `row` and `col` are valid.
    bool checkLines(Tile& tile, unsigned int row, unsigned int col, int& score);

    // Returns whether a tile has previously been placed at the position 
    // `row` and `col`.
    bool checkPosition(unsigned int row, unsigned int col);

    // Returns whether a segment of a line in the given `direction` from 
    // position `row` and `col` is valid. 
    bool checkDirection(Tile& tile, unsigned int row, unsigned int col, 
                        char direction, int& score);
};

#endif // ASSIGN2_BOARD_H
