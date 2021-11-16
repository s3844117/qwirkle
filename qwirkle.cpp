#include "Game.h"

#include <iostream>

#define EXIT_SUCCESS    0
#define TEAM_SIZE       4
#define NEW_GAME        1
#define LOAD_GAME       2
#define CREDITS         3
#define QUIT            4


// Helper class used to encapsulate required information for each team member.
class Student {
public:
    Student(std::string name, int id, std::string email);
    friend std::ostream& operator<<(std::ostream& os, const Student& student);
private:
    std::string name;
    int id;
    std::string email;
};

Student::Student(std::string name, int id, std::string email) 
    : name(name), id(id), email(email) {}

std::ostream& operator<<(std::ostream& os, const Student& student) {
    return os << "Name: " << student.name << std::endl
              << "Student ID: s" << student.id << std::endl
              << "Email: " << student.email << std::endl; 
}

// Processes user input menu choices and executes requested functions.
bool startMenu();

// Prints menu options and required input to console.
void printMenu();

// Prints user prompt to console and receives user input.
int getMenuChoice();

// Confirms whether user input matches a valid menu choice.
bool validMenuChoice(std::string& menuInput);

// Prompts user for player names and initialises a new game.
void newGame();

// Prompts user for a filename to load a previous game state from.
void loadGame();

// Handles user prompt and console display for Qwirkle gameplay.
void play(Game& qwirkle);

// Checks if the user's input is valid for number of players
bool numberOfPlayers(std::string number);

// Prints student information to the console. Returns to menu after completed.
void printCredits();

int main(void) {
    std::cin.exceptions(std::istream::eofbit);
    std::cout << "Welcome to Qwirkle!" << std::endl
              << "-------------------" << std::endl;
    bool exit = false;
    try {
        while (!exit) {
            exit = startMenu();
        }
    } catch (std::istream::failure& e) {
        std::cout << std::endl;
    } catch (Quit& e) {
        // Quit program from gameplay
    }
    std::cout << "\nGoodbye" << std::endl;

    return EXIT_SUCCESS;
}

bool startMenu() {
    bool exit = false;
    printMenu();
    int menuChoice = getMenuChoice();
    if (menuChoice == NEW_GAME) {
        exit = true;
        newGame();
    } else if (menuChoice == LOAD_GAME) {
        exit = true;
        loadGame();
    } else if (menuChoice == CREDITS) {
        printCredits();
    } else if (menuChoice == QUIT) {
        exit = true;
    }
    return exit;
}

void printMenu() {
    std::cout << "\nMenu" << std::endl;
    std::cout << "----" << std::endl;
    std::cout << "1. New Game" << std::endl;
    std::cout << "2. Load Game" << std::endl;
    std::cout << "3. Credits (Show student information)" << std::endl;
    std::cout << "4. Quit\n" << std::endl;
}

int getMenuChoice() {
    std::string input;
    std::cout << "> ";
    std::getline(std::cin, input);
    while (!validMenuChoice(input)) {
        std::cout << "Invalid Input" << std::endl
                  << "> ";
        std::getline(std::cin, input);
    }
    return std::stoi(input);
}

bool validMenuChoice(std::string& menuInput) {
    bool valid = false;
    try {
        std::size_t charsRead;
        int menuChoice = std::stoi(menuInput, &charsRead);
        if (charsRead == menuInput.size() && 
                menuChoice >= NEW_GAME && menuChoice <= QUIT) {
            valid = true;
        }
    } catch (std::invalid_argument& e) {
        // INVALID INPUT: Input is not an integer
    }
    return valid;
}

void newGame() {
    Game qwirkle;
    std::cout << "\nStarting a New Game" << std::endl;
    std::cout << "\nNumber of Players\n> ";
    std::string input;
    std::getline(std::cin, input);
    bool result = numberOfPlayers(input);
    while (!result){
        std::cout << "\nInvalid Number of Players, select a number between 2 - 4\n> ";
        std::getline(std::cin, input);
        result = numberOfPlayers(input);
    };
    int number = stoi(input);
    qwirkle.setNumPlayers(number);
    for (int i = 0; i < number; ++i) {
            std::cout << "\nEnter a name for player " << i + 1
                      << " (uppercase characters only)" << std::endl
                      << "> ";
            std::string playerName;
            std::getline(std::cin, playerName);
                while (!qwirkle.newPlayer(playerName)) {
                std::cout << "Invalid Input\n> ";
                std::getline(std::cin, playerName);
            }
        }
        std::cout << "\nLet's Play!\n\n";
        qwirkle.start();
        play(qwirkle);
}


void loadGame() {
    std::string fileName;
    std::cout << "\nEnter the filename from which to load a game\n> ";
    bool valid = false;
    std::getline(std::cin, fileName);
    while (!valid) {
        try {
            Game qwirkle;
            int number = qwirkle.getNumPlayersFromSave(fileName);
            qwirkle.setNumPlayers(number);
            if (qwirkle.load(fileName)) {
                valid = true;
                play(qwirkle);
            }
        } catch (InvalidSaveFile& e) {
            // INVALID INPUT: Save file invalid
        }
        if (!valid) {
            std::cout << "Invalid Input" << std::endl
                      << "> ";
            std::getline(std::cin, fileName);
        }
    }
}

void play(Game& qwirkle) {
    while (!qwirkle.isGameOver()) {
        qwirkle.displayTurn();
        std::string command;
        bool isValidAction = false;
        bool isEndTurn = false;
        Player* currentPlayer = qwirkle.currentPlayer();
        do {
            std::cout << "\n> ";
            std::getline(std::cin, command);
            isValidAction = qwirkle.action(command);
            isEndTurn = currentPlayer != qwirkle.currentPlayer();
            if (!isValidAction) {
                std::cout << "Invalid Input";
            }
        } while (!isValidAction || !isEndTurn);
        std::cout << std::endl;
    }
    qwirkle.displayGameOver();
}

void printCredits() {
    Student Kyriece = Student("Kyriece Dam", 3848845, 
                              "s3848845@student.rmit.edu.au");
    Student Nikita = Student("Nikita I", 3844117, 
                             "s3844117@student.rmit.edu.au");
    Student Zavier = Student("Zavier The", 3855048, 
                             "s3855048@student.rmit.edu.au");
    Student Phuong = Student("Duy Phuong Tran", 3869716, 
                             "s3869716@student.rmit.edu.au");
    Student teamPP[TEAM_SIZE] = {Kyriece, Nikita, Zavier, Phuong};
    std::cout << "\n----------------------------------" << std::endl;
    for (int i = 0; i < TEAM_SIZE; i++) {
        std::cout << (i == 0 ? "" : "\n") << teamPP[i];
    }
    std::cout << "----------------------------------" << std::endl;
}

bool numberOfPlayers(std::string number){
    if (number.compare("2")==0)
    {
        return true;
    }
    else if (number.compare("3")==0)
    {
        return true;
    }
    else if (number.compare("4")==0)
    {
        return true;
    }
    else 
    {
        return false;
    }
}
