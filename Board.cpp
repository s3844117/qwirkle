#include "Board.h"
#include "Tile.h"

#include <iomanip>

#define BOARD_SIZE      26

Board::Board() 
    : tilesPlaced(0) {
    std::vector<Tile*> row(BOARD_SIZE, nullptr);
    for (int i = 0; i < BOARD_SIZE; ++i) {
        board.push_back(row);
    }
}

Board::~Board() {
    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            if (board[i][j]) {
                delete board[i][j];
            }
        }
    }
}

bool Board::isEmpty() {
    return tilesPlaced == 0;
}

bool Board::addAt(Tile* tile, unsigned int row, unsigned int col) {
    bool added = false;
    if (tile && isValidPosition(row, col)) {
        board[row][col] = tile;
        added = true;
        ++tilesPlaced;
    }
    return added;
}

bool Board::isValidPosition(unsigned int row, unsigned int col) {
    return row < BOARD_SIZE
        && col < BOARD_SIZE
        && !board[row][col];
}

bool Board::isValidMove(Tile& tile, unsigned int row, unsigned int col, 
                        int& score) {
    bool valid = false;
    if (isEmpty()) {
        ++score;
        valid = true;
    } else {
        valid = checkLines(tile, row, col, score);
    }
    return valid;
}

bool Board::checkLines(Tile& tile, unsigned int row, unsigned int col, 
                       int& score) {
    bool result = true;
    char directions[4] =  { 'N', 'E', 'S', 'W' };
    int  rowOffsets[4]  = { -1,   0,   1,   0  };
    int  colOffsets[4]  = {  0,   1,   0,  -1  };
    int moveScore = 0;
    int i = 0;
    while (i < 2 && result) {
        if (checkPosition(row + rowOffsets[i], col + colOffsets[i]) 
                || checkPosition(row + rowOffsets[i + 2], 
                                 col + colOffsets[i + 2])) {
            int lineScore = 1;
            result = checkDirection(tile, row + rowOffsets[i], 
                                    col + colOffsets[i], directions[i], 
                                    lineScore)
                     && checkDirection(tile, row + rowOffsets[i + 2], 
                                       col + colOffsets[i + 2], 
                                       directions[i + 2], lineScore);
            if (lineScore == 6) {
                lineScore += 6;
                std::cout << "\nQWIRKLE!!!" << std::endl;
            }
            moveScore += lineScore * result;
        }
        ++i;
    }
    score += moveScore * result;
    return moveScore && result;
}

bool Board::checkPosition(unsigned int row, unsigned int col) {
    return row < BOARD_SIZE
        && col < BOARD_SIZE
        && board[row][col];
}

bool Board::checkDirection(Tile& tile, unsigned int row, 
                           unsigned int col, char direction, int& score) {
    bool result;
    if (row >= BOARD_SIZE || col >= BOARD_SIZE) {
        result = true;
    } else if (!board[row][col]) {
        result = true;
    } else {
        Tile* check = board[row][col];
        if ((check->getColour() == tile.getColour()) 
                != (check->getShape() == tile.getShape())) {
            if (direction == 'N') {
                --row;
            } else if (direction == 'E') {
                ++col;
            } else if (direction == 'S') {
                ++row;
            } else if (direction == 'W') {
                --col;
            }
            result = checkDirection(tile, row, col, direction, ++score);
        } else {
            result = false;
        }
    }
    return result;
}

void Board::display() {
    for (int i = 0; i < BOARD_SIZE; ++i) {
        std::cout << (i == 0 ? "   " : "") << std::setw(3) << std::left << i;
    }
    std::cout << std::endl 
              << "  ";
    for (int i = 0; i < BOARD_SIZE * 3 + 1; ++i) {
        std::cout << "-";
    }
    std::cout << std::endl;
    char row = 'A';
    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            if (j == 0) {
                std::cout << row++ << " |";
            }
            Tile* tile = board[i][j];
            if (tile) {
                tile->tileDisplayforGame(std::cout,tile);
            } else {
                std::cout << "  ";
            }
            std::cout << "|";
        }
        std::cout<< std::endl;
    }
}

std::ostream& operator<<(std::ostream& os, const Board& board) {
    os << BOARD_SIZE << ',' << BOARD_SIZE << std::endl;
    if (board.tilesPlaced != 0) {
        bool first = true;
        for (unsigned int row = 0; row < BOARD_SIZE; ++row) {
            for (unsigned int col = 0; col < BOARD_SIZE; ++col) {
                Tile* tile = board.board[row][col];
                if (tile) {
                    os << (first ? "" : ", ") << *tile 
                       << '@' 
                       << static_cast<char>('A' + row) << col;
                    first = false;
                }
            }
        }
    }
    return os;
}
