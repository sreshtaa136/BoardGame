#include "GameEngine.h"

#include <iostream>

void GameEngine::newGame(std::string player1, std::string player2){
    
    gameBoard = new GameBoard();
    tileBag = new TileBag();
    setPlayers(player1, player2);
    this->player1->setPlayerHand(tileBag->createHand());
    this->player2->setPlayerHand(tileBag->createHand());
}

//place tile, remove the tile from player's hand and update hand 
void GameEngine::placeTile(char row, int col, Colour colour, Shape shape, std::string player){
    
    Tile* tileToPlace = new Tile(colour,shape);

    if(player == player1->getName()){
        if(player1->getPlayerHand()->searchTile(tileToPlace) != -1){
            //gameBoard->placeTile(row, col, tileToPlace);
            player1->getPlayerHand()->removeTile(colour, shape);
            if(tileBag->getBagSize() != 0){
                Tile* tile = new Tile(*(tileBag->drawTile()));
                player1->getPlayerHand()->addToEnd(tile);
            }
        }
    }else if(player == player2->getName()){
        if(player2->getPlayerHand()->searchTile(tileToPlace) != -1){
            //gameBoard->placeTile(row, col, tileToPlace);
            player2->getPlayerHand()->removeTile(colour, shape);
            if(tileBag->getBagSize() != 0){
                Tile* tile = new Tile(*(tileBag->drawTile()));
                player2->getPlayerHand()->addToEnd(tile);
            }
        }
    }
}

void GameEngine::replaceTile(Colour colour, Shape shape, std::string player){

    Tile* tileToReplace = new Tile(colour,shape);

    if(player == player1->getName()){
        if(player1->getPlayerHand()->searchTile(tileToReplace) != -1){
            player1->getPlayerHand()->removeTile(colour, shape);
            tileBag->getTileBag()->addToEnd(tileToReplace);
            if(tileBag->getBagSize() != 0){
                Tile* tile = new Tile(*(tileBag->drawTile()));
                player1->getPlayerHand()->addToEnd(tile);
            }
        }
    }else if(player == player2->getName()){
        if(player2->getPlayerHand()->searchTile(tileToReplace) != -1){
            player2->getPlayerHand()->removeTile(colour, shape);
            tileBag->getTileBag()->addToEnd(tileToReplace);
            if(tileBag->getBagSize() != 0){
                Tile* tile = new Tile(*(tileBag->drawTile()));
                player2->getPlayerHand()->addToEnd(tile);
            }
        }
    }
}

void GameEngine::setPlayers(std::string player1, std::string player2){
    this->player1 = new Player(player1);
    this->player2 = new Player(player2);
}

Player* GameEngine::getPlayer1(){
    return this->player1;
}

Player* GameEngine::getPlayer2(){
    return this->player2;
}

void GameEngine::printGameState(){

    std::cout << " \n";
    std::cout << "Score for " << player1->getName() <<
    ": " << player1->getScore() << "\n";
    std::cout << "Score for " << player2->getName() <<
    ": " << player2->getScore() << "\n\n";
    gameBoard->displayBoard();
    std::cout << " \n";
}