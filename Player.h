#ifndef ASSIGN2_PLAYER_H
#define ASSIGN2_PLAYER_H

#include <string>

class LinkedList;

class Player {
public:
    LinkedList* hand;

    Player(std::string name);
    ~Player();

    std::string getName();
    int getScore();

    // Restricted setter only allowing addition to player `score`.
    void addScore(int score);

    // Output operator overloading used to save Player information.
    friend std::ostream& operator<<(std::ostream& os, const Player& player);

private:
    std::string name;
    int         score;
};

#endif // ASSIGN2_PLAYER_H
