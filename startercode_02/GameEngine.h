#ifndef ASSIGN2_GAMEENGINE_H
#define ASSIGN2_GAMEENGINE_H

#include "TileCodes.h"
#include "TileBag.h"
#include "Tile.h"
#include "LinkedList.h"
#include "Player.h"
#include "GameBoard.h"

class GameEngine{
public:

    void newGame(std::string player1, std::string player2);
    void placeTile(char row, int col, Colour colour, Shape shape, std::string player);
    void setPlayers(std::string player1, std::string player2);
    void printGameState();
    void replaceTile(Colour colour, Shape shape, std::string player);
    Player* getPlayer1();
    Player* getPlayer2();
    TileBag* tileBag;

private:

    Player* player1;
    Player* player2;
    //TileBag* tileBag;
    GameBoard* gameBoard;
};
#endif