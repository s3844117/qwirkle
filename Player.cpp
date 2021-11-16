#include "Player.h"
#include "LinkedList.h"

Player::Player(std::string name)
    : hand(new LinkedList()), name(name), score(0) {}

Player::~Player() {
    delete hand;
}

std::string Player::getName() {
    return name;
}

int Player::getScore() {
    return score;
}

void Player::addScore(int score) {
    this->score += score;
}

std::ostream& operator<<(std::ostream& os, const Player& player) {
    return os << player.name << std::endl
              << player.score << std::endl
              << *player.hand;
}
