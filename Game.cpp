#include "Game.h"
#include "Board.h"
#include "LinkedList.h"
#include "Player.h"
#include "Tile.h"
#include "TileCodes.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <limits>

#define HAND_SIZE       6
#define TILE_SETS       2
#define COLOUR_COUNT    6

Game::Game() 
    : board(nullptr),
      bag(nullptr), 
      players(nullptr), 
      currentPlayerIndex(0) {}
      int numPlayers;

Game::~Game() {
    delete board;
    delete bag;
    if (players) {
        for (int i = 0; i < numPlayers; ++i) {
            if (players[i]) {
                delete players[i];
            }
        }
    }
    delete[] players;
}

void Game::setNumPlayers(int number)
{
    numPlayers = number;
}

bool Game::newPlayer(std::string playerName) {
    bool playerCreated = false;
    if (isValidPlayerName(playerName)) {
        if (!players) {
            players = new Player*[numPlayers] {nullptr};
        }
        players[currentPlayerIndex] = new Player(playerName);
        setNextPlayer();
        playerCreated = true;
    }
    return playerCreated;
}

bool Game::isValidPlayerName(std::string& playerName) {
    bool valid = !playerName.empty();
    for (const char letter : playerName) {
        if (!isupper(letter)) {
            valid = false;
        }
    }
    return valid;
}

void Game::setNextPlayer() {
    currentPlayerIndex = (currentPlayerIndex + 1) % numPlayers;
}

Player* Game::currentPlayer() const {
    return players[currentPlayerIndex];
}

void Game::start() {
    if (players && currentPlayerIndex == 0) {
        board = new Board();
        bag = new LinkedList();
        fillBag();
        dealTiles();
    }
}

void Game::fillBag() {
    Colour COLOURS[] = { RED, ORANGE, YELLOW, GREEN, BLUE, PURPLE };
    for (int n = 0; n < TILE_SETS; ++n) {
        for (int i = 0; i < COLOUR_COUNT; ++i) {
            for (Shape s = CIRCLE; s <= CLOVER; ++s) {
                bag->addBack(new Tile(COLOURS[i], s));
            }
        }
    }
    bag->shuffle();
}

void Game::dealTiles() {
    for (int i = 0; i < numPlayers; i++) {
        LinkedList* hand = players[i]->hand;
        while (bag->size() != 0 && hand->size() < HAND_SIZE) {
            hand->addBack(bag->removeFront());
        }
    }
}

bool Game::load(std::string fileName) {
    bool result = false;
    std::ifstream ifs;
    ifs.open(fileName + ".save");
    if (ifs) {
        loadNumberOfPlayers(ifs);
        loadPlayers(ifs);
        loadBoard(ifs);
        loadBag(ifs);
        loadCurrentPlayer(ifs);
        std::cout << "\nQwirkle game successfully loaded\n\n";
        ifs.close();
        result = true;
    }
    return result;
}

int Game::getNumPlayersFromSave(std::string fileName) {
    int number;
    std::ifstream ifs;
    ifs.open(fileName + ".save");
    ifs >> number;
    ifs.close();
    return number;
}

void Game::loadPlayers(std::ifstream& ifs) {
    std::string rbuf;
    for (int i = 0; i < numPlayers; ++i) {
        std::getline(ifs, rbuf);
        if (!newPlayer(rbuf)) {
            throw InvalidSaveFile();
        }
        std::getline(ifs, rbuf);
        Player* player = players[i];
        if (isValidScore(rbuf)) {
            player->addScore(std::stoi(rbuf));
        } else {
            throw InvalidSaveFile();
        }
        std::getline(ifs, rbuf);
        std::stringstream tiles(rbuf);
        while (std::getline(tiles, rbuf, ',')) {
            std::stringstream handTile(rbuf);
            Tile* tile = loadTile(handTile);
            if (player->hand->size() < HAND_SIZE) {
                player->hand->addBack(tile);
            } else {
                throw InvalidSaveFile();
            }
        }
    }
}
void Game::loadNumberOfPlayers(std::ifstream& ifs) {
    std::string rbuf;
    std::getline(ifs, rbuf);
    if (rbuf.compare("2")==0)
    {
        return;
    }
    else if (rbuf.compare("3")==0)
    {
        return;
    }
    else if (rbuf.compare("4")==0)
    {
        return;
    }
    else {
        throw InvalidSaveFile();
    }
}

bool Game::isValidScore(std::string& scoreString) {
    bool valid = false;
    try {
        std::size_t charsRead;
        int score = std::stoi(scoreString, &charsRead);
        if (charsRead == scoreString.size() && score >= 0) {
            valid = true;
        }
    } catch (std::invalid_argument& e) {
        // INVALID INPUT: Input is not an integer
    }
    return valid;
}

void Game::loadBoard(std::ifstream& ifs) {
    std::string rbuf;
    ifs.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::getline(ifs, rbuf);
    board = new Board();
    std::stringstream boardState(rbuf);
    while (std::getline(boardState, rbuf, ',')) {
        std::stringstream boardTile(rbuf);
        Tile* tile = loadTile(boardTile);
        char rowChar;
        unsigned int col;
        boardTile.ignore();
        boardTile >> rowChar;
        boardTile >> col;
        unsigned int row = rowChar - 'A';
        if (!board->addAt(tile, row, col)) {
            delete tile;
            throw InvalidSaveFile();
        }
    }
}

void Game::loadBag(std::ifstream& ifs) {
    std::string rbuf;
    std::getline(ifs, rbuf);
    bag = new LinkedList();
    std::stringstream bagTiles(rbuf);
    while (std::getline(bagTiles, rbuf, ',')) {
        std::stringstream bagTile(rbuf);
        Tile* tile = loadTile(bagTile);
        bag->addBack(tile);
    }
}

void Game::loadCurrentPlayer(std::ifstream& ifs) {
    std::string currentPlayerName;
    std::getline(ifs, currentPlayerName);
    int counter = numPlayers;
    while (currentPlayer()->getName() != currentPlayerName) {
        setNextPlayer();
        --counter;
        if (counter == 0) {
            throw InvalidSaveFile();
        }
    }
}

Tile* Game::loadTile(std::stringstream& tileCode) {
    Colour colour;
    Shape shape;
    tileCode.ignore(256, 'm');
    tileCode >> colour;
    tileCode >> shape;
    tileCode.ignore(256, 'm');
    std::string VALID_COLOURS = "ROYGBP";
    if (VALID_COLOURS.find(colour) == std::string::npos 
            || shape < CIRCLE || shape > CLOVER) {
        throw InvalidSaveFile();
    }
    return new Tile(colour, shape);
}

void Game::endTurn() {
    dealTiles();
    setNextPlayer();
}

bool Game::isGameOver() const {
    return bag->isEmpty()
        && isAnyHandEmpty();
}

bool Game::isAnyHandEmpty() const {
    bool result = false;
    for (int i = 0; i < numPlayers; ++i) {
        if (players[i]->hand->isEmpty()) {
            result = true;
        }
    }
    return result;
}

bool Game::action(std::string command) {
    bool result = false;
    std::stringstream args(command);
    std::vector<std::string> argv;

    std::string arg;
    while (args >> arg) {
        argv.push_back(arg);
    }

    int argc = argv.size();
    if (argc == 1 && argv[0] == "quit") {
        throw Quit();
    } else if (argc == 2 && argv[0] == "save") {
        result = save(argv[1]);
        std::cout << "\nGame successfully saved\n";
    } 
    else if (argc == 1 && argv[0] == "help"){
        result = displayHelpMessage();
    }
        else if (argc > 1) {
        std::stringstream tileCode(argv[1]);
        Colour colour;
        tileCode >> colour;
        Shape shape;
        tileCode >> shape;
        Tile tile(colour, shape);
        if (argc == 2 && argv[0] == "replace") {
            result = replace(tile);
        } else if (argc == 4 && argv[0] == "place" && argv[2] == "at") {
            std::stringstream position(argv[3]);
            char rowChar;
            int col;
            if (position >> rowChar && position >> col) {
                int row = rowChar - 'A';
                result = place(tile, row, col);
            }
        }
        if (result) {
            endTurn();
        }
    }
    return result;
}

bool Game::place(Tile& tile, unsigned int row, unsigned int col) {
    int score = 0;
    bool result = false;
    if (board->isValidMove(tile, row, col, score)) {
        result = board->addAt(currentPlayer()->hand->remove(tile), row, col);
    }
    if (result) {
        currentPlayer()->addScore(score);
    }
    return result;
}

bool Game::replace(Tile& tile) {
    bool result = false;
    Tile* toReplace = currentPlayer()->hand->remove(tile);
    if (toReplace) {
        bag->addBack(toReplace);
        result = true;
    }
    return result;
}

bool Game::save(std::string filename) const {
    std::ofstream ofs;
    ofs.open(filename + ".save");
    ofs << numPlayers << std::endl;
    for (int i = 0; i < numPlayers; ++i) {
        ofs << *players[i] << std::endl;
    }
    ofs << *board << std::endl
        << *bag << std::endl
        << currentPlayer()->getName();
    ofs.close();
    return true;
}

void Game::displayTurn() const {
    std::cout << currentPlayer()->getName() << ", it's your turn" << std::endl;
    for (int i = 0; i < numPlayers; ++i) {
        std::cout << "Score for " << players[i]->getName() << ": " 
                  << players[i]->getScore() << std::endl;
    }
    board->display();
    std::cout << "\nYour hand is" << std::endl
              << *currentPlayer()->hand << std::endl;
}

void Game::displayGameOver() const {
    board->display();
    std::cout << "\nGame over" << std::endl;
    int highestScore = std::numeric_limits<int>::min();
    std::string winner;
    for (int i = 0; i < numPlayers; ++i) {
        int score = players[i]->getScore();
        std::string name = players[i]->getName();
        std::cout << "Score for " << name << ": " << score << std::endl;
        if (score > highestScore) {
            highestScore = score;
            winner = name;
        }
    }
    std::cout << "Player " << winner << " won!" << std::endl;
}

bool Game::displayHelpMessage(){
    std::cout << "\nInformation about Game Play\nTile Codes: {1 is ੦}, {2 is ✦}, {3 is ◆}, {4 is ⧈}, {5 is ✶}, {6 is ♧}\n";
    std::cout << "If you have " << "\e[0;31m" << "R1" <<"\033[m" <<" in your hand, you can place this tile by typing\n'place R1 at A0'";
    std::cout << " which will place " << "\e[0;31m" << "੦" <<"\033[m" << " at A0";
    std::cout << "\nIf you would like to replace the tile, type 'replace R1'\n";
    std::cout << "\nInformation about saving/quitting game\nIf you would like to save the current state of the game, type 'save desired_filename'\nIf you would like to quit the game, simply type 'quit'\n";
    return true;
}