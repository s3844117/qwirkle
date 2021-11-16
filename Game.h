#ifndef ASSIGN2_GAME_H
#define ASSIGN2_GAME_H

#include <string>


class Tile;
class Board;
class LinkedList;
class Player;

// Custom exception indicating `quit` command was input during gameplay.
class Quit {};

// Custom exception indicating the given save file is of incorrect format.
class InvalidSaveFile {};

class Game {
public:
    Game();
    ~Game();


    // Sets the number of players for a game from the user input
    void setNumPlayers(int number);
    
    // Adds new player of `playerName` to the game, returning whether a player 
    // was successfully created. A player of invalid `playerName` will not be
    // created or added to the game. Assumes that players will only be created
    // at the start of a new game.
    bool newPlayer(std::string playerName);

    // Returns pointer to the player whose turn it is to make a move.
    Player* currentPlayer() const;

    // Initialises a newly created game. Assumes players have been added to
    // game, using the newPlayer method, and that the game was not loaded from
    // a save file.
    void start();

    // Loads game state from a save file. Returns whether load was successful.
    // Throws `InvalidSaveFile` if `filename.sav` does not match required 
    // format.
    bool load(std::string fileName);

    //Gets the first line from the save file which is expected to be a integer
    //The method is then used to set the number of players for loading a game
    int getNumPlayersFromSave(std::string fileName);

    // Returns whether the game is over. The game ends when the bag is empty
    // and one player has no more tiles in their hand.
    bool isGameOver() const;

    // Parses and executes command. On invalid command, no action is made and
    // error message is output to console.
    bool action(std::string command);

    // Removes the given `tile` from the player's hand and places it on the 
    // board at the position given by `row` and `col`. Returns whether the 
    // action was successful. Action is unsuccessful when the tile cannot be
    // found in the player's hand or the position given is invalid.
    bool place(Tile& tile, unsigned int row, unsigned int col);

    // Removes the given `tile` from the player's hand and returns it to the
    // back of the bag. Returns whether the action was successful. Action is 
    // unsuccessful when the tile cannot be found in the player's hand.
    bool replace(Tile& tile);

    // Saves current game state to a file of name `filename.sav`. Returns 
    // whether save was successful. If a file of `filename.sav` exists, it will
    // be overwritted.
    bool save(std::string fileName) const;

    // Prints current game state to the console.
    void displayTurn() const;

    // Prints game over message to the console.
    void displayGameOver() const;

private:
    Board*      board;
    LinkedList* bag;
    Player**    players;
    int         currentPlayerIndex;

    // Confirms validity of a given `playerName`. A `playerName` is valid only
    // if it contains only uppercase letters.
    bool isValidPlayerName(std::string& playerName);

    // Increments the `currentPlayerIndex` to ensure turn order is correctly 
    // maintained.
    void setNextPlayer();

    // Creates tiles required for Qwirkle game and adds them to the bag.
    void fillBag();

    // Deals tiles from the front of the bag to all players up to the maximum 
    // hand size.
    void dealTiles();

    // Loads Player ADT from information provided in a save file.
    void loadPlayers(std::ifstream& ifs);

    // Loads the first line of the save file
    void loadNumberOfPlayers(std::ifstream& ifs);
    
    // Confirms validity of a score value read from a save file.
    bool isValidScore(std::string& scoreString);

    // Loads Board ADT from information provided in a save file.
    void loadBoard(std::ifstream& ifs);

    // Loads Bag ADT from information provided in a save file.
    void loadBag(std::ifstream& ifs);

    // Loads gameplay turn information from a save file.
    void loadCurrentPlayer(std::ifstream& ifs);

    // Creates a Tile from parsed Tile Codes.
    Tile* loadTile(std::stringstream& tileCode);

    // Ensures that turn order is maintained and that hands are replenished at
    // the end of each turn.
    void endTurn();

    // Returns whether any of the players' hands is empty.
    bool isAnyHandEmpty() const;

    // Prints a help message when a user types help
    bool displayHelpMessage();
};

#endif // ASSIGN2_GAME_H
