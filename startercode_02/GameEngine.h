#ifndef ASSIGN2_GAMEENGINE_H
#define ASSIGN2_GAMEENGINE_H

#include "TileCodes.h"
#include "TileBag.h"
#include "Tile.h"
#include "LinkedList.h"
#include "Player.h"
#include "GameBoard.h"

#define MAX_DIM 26

class GameEngine{
public:

    void newGame(std::string player1, std::string player2);
    bool placeTile(char row, int col, Colour colour, Shape shape, std::string player);
    void setPlayers(std::string player1, std::string player2);
    void printGameState();
    bool replaceTile(Colour colour, Shape shape, std::string player);
    void saveGame(std::string fileName);
    void loadGame(std::string fileName);
    Player* getPlayer1();
    Player* getPlayer2();
    Player* getCurrentPlayer();
    void setCurrentPlayer(std::string playerName);
    Player* getPlayer(std::string playerName);
    void setTileBag(TileBag* tileBag);

    bool verifyName(std::string s);
    bool verifyListString(std::string s);
    bool verifyBoardShapeString(std::string s);
    bool verifyBoardString(std::string s);
    bool verifyBoardSize(std::string s);

    TileBag* tileBag;
    GameBoard* gameBoard;

private:

    Player* player1;
    Player* player2;
    Player* currentPlayer;
    //TileBag* tileBag;
    
};
#endif