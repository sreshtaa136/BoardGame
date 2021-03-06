#include "GameEngine.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <regex>

GameEngine::GameEngine() {

    player1 = new Player("");
    player2 = new Player("");
    //currentPlayer = new Player("");
    gameBoard = new GameBoard();
    tileBag = new TileBag();
    gameBoard = new GameBoard();

}

GameEngine::~GameEngine() {
    
    delete tileBag;
    delete gameBoard;
    delete player1;
    delete player2;
    //delete currentPlayer;

    tileBag = nullptr;
    gameBoard = nullptr;
    player1 = nullptr;
    player2 = nullptr;
    //currentPlayer = nullptr;
}

void GameEngine::newGame(std::string player1, std::string player2) {
    
    // gameBoard = new GameBoard();
    tileBag = new TileBag();
    //setPlayers(player1, player2);
    this->player1->setName(player1);
    this->player2->setName(player2);
    currentPlayer = this->player1->getName();
    this->player1->setPlayerHand(tileBag->createHand());
    this->player2->setPlayerHand(tileBag->createHand());
    printGameState();
    
}

bool GameEngine::placeTile(char row, int col, Colour colour, Shape shape, 
                           std::string player) {

    Tile* tileToPlace = new Tile(colour,shape);
    bool tilePlaced = false;

    if (player == player1->getName()) {

        // Check if tile is in player hand
        if (player1->getPlayerHand()->searchTile(tileToPlace) != -1) {

            // Remove the tile from player's hand if placed
            if (gameBoard->placeTile(row, col, tileToPlace)) {
                player1->getPlayerHand()->removeTile(colour, shape);

                // When tilebag is not empty, draw tile to player's hand from the bag
                if (tileBag->getBagSize() != 0) {
                    Tile* tile = new Tile(*(tileBag->drawTile()));
                    player1->getPlayerHand()->addToEnd(tile);
                    //tilePlaced = true;
                }
                tilePlaced = true;
            }
        }

    } else if (player == player2->getName()) {

        // Check if tile is in player hand
        if (player2->getPlayerHand()->searchTile(tileToPlace) != -1) {

            // Remove the tile from player's hand if placed
            if (gameBoard->placeTile(row, col, tileToPlace)) {
                player2->getPlayerHand()->removeTile(colour, shape);

                // When tilebag is not empty, draw tile to player's hand from the bag
                if (tileBag->getBagSize() != 0) {
                    Tile* tile = new Tile(*(tileBag->drawTile()));
                    player2->getPlayerHand()->addToEnd(tile);
                }
                tilePlaced = true;
            }
        }
    }
    return tilePlaced;
}

bool GameEngine::replaceTile(Colour colour, Shape shape, std::string player) {

    Tile* tileToReplace = new Tile(colour,shape);
    bool tileReplaced = false;

    if (player == player1->getName()) {

         // Check if tile is in player hand
        if (player1->getPlayerHand()->searchTile(tileToReplace) != -1) {

            // Remove the tile from player's hand and place it back in the tilebag
            player1->getPlayerHand()->removeTile(colour, shape);
            tileBag->getTileBag()->addToEnd(tileToReplace);

            // When tilebag is not empty, replace current tile
            if (tileBag->getBagSize() != 0) {

                Tile* tile = new Tile(*(tileBag->drawTile()));
                player1->getPlayerHand()->addToEnd(tile);
                tileReplaced = true;
            }
        }
    } else if (player == player2->getName()) {

        // Check if tile is in player hand
        if (player2->getPlayerHand()->searchTile(tileToReplace) != -1) {

            // Remove the tile from player's hand and place it back in the tilebag
            player2->getPlayerHand()->removeTile(colour, shape);
            tileBag->getTileBag()->addToEnd(tileToReplace);

            // When tilebag is not empty, replace current tile
            if (tileBag->getBagSize() != 0) {

                Tile* tile = new Tile(*(tileBag->drawTile()));
                player2->getPlayerHand()->addToEnd(tile);
                tileReplaced = true;
            }
        }
    }
    return tileReplaced;
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

// Player* GameEngine::getCurrentPlayer(){
//     return this->currentPlayer;
// }

std::string GameEngine::getCurrentPlayer(){
    return this->currentPlayer;
}

void GameEngine::setCurrentPlayer(std::string playerName){
    // this->currentPlayer = getPlayer(playerName);
    this->currentPlayer = playerName;
}

Player* GameEngine::getPlayer(std::string playerName){

    Player* player;

    if (player1->getName() == playerName) {
        player = player1;
    } else {
        player = player2;
    }
    return player;
}

void GameEngine::printGameState(){

    //std::cout << " \n";
    std::cout << "Score for " << player1->getName() <<
    ": " << player1->getScore() << "\n";
    std::cout << "Score for " << player2->getName() <<
    ": " << player2->getScore() << "\n\n";
    gameBoard->displayBoard();
    std::cout << " \n";
}

void GameEngine::loadHand(std::string hand, std::string name){

    LinkedList* playerHand = new LinkedList();

    // Count the number of commas.
    int commaCount = 0;
    for (int i = 0; i< (int) hand.size(); ++i) {
        if (hand[i] == ',') {
            ++commaCount;
        }
    }

    int tileCount = (hand.size() - commaCount)/2;
    char tileArray[tileCount*2];
    int count = 0;

    // Add all the tiles to a char array
    for (int i = 0; i< (int) hand.size(); ++i) {
        if (hand[i] != ',') {
            tileArray[count] = hand[i];
            count++;
        }
    }

    count = 0;

    // Populate the player's hand with thier tiles
    for (int i = 0; i<tileCount; ++i) {
        char colour = tileArray[count];
        int shape = tileArray[count + 1] - '0';
        Tile* tile = new Tile(colour, shape);
        playerHand->addToEnd(tile);
        count = count + 2;
    }

    // Set player's hand for the given player
    if (player1->getName() == name) {
        this->player1->setPlayerHand(playerHand);
    } else if (player2->getName() == name) {
        this->player2->setPlayerHand(playerHand);
    }
}

void GameEngine::saveGame(std::string fileName) {
    // add '.save' extension to the filename
    fileName += ".save";
    std::ofstream outFile;
    outFile.open(fileName);

    // output the details to be saved to the file
    outFile << player1->getName() << std::endl;
    outFile << player1->getScore() << std::endl;
    outFile << player1->getPlayerHand()->toString() << std::endl;
    outFile << player2->getName() << std::endl;
    outFile << player2->getScore() << std::endl;
    outFile << player2->getPlayerHand()->toString() << std::endl;
    outFile << gameBoard->dimensionsToString() << std::endl;
    outFile << gameBoard->toString() << std::endl;
    outFile << tileBag->getTileBag()->toString() << std::endl;
    //outFile << currentPlayer->getName() << std::endl;
    outFile << currentPlayer << std::endl;

    outFile.close();
}

bool GameEngine::loadGame(std::string fileName) {
    // Open file for reading
    std::ifstream inFile;
    inFile.open(fileName);
    GameBoard* board = nullptr;

    std::string player1Name, player1Score, player1Hand;
    std::string player2Name, player2Score, player2Hand;
    std::string boardShape, boardString;
    std::string tileBagString;
    std::string currPlayerName;

    // extract lines from the file
    getline(inFile, player1Name);
    getline(inFile, player1Score);
    getline(inFile, player1Hand);
    getline(inFile, player2Name);
    getline(inFile, player2Score);
    getline(inFile, player2Hand);
    getline(inFile, boardShape);
    getline(inFile, boardString);
    getline(inFile, tileBagString);
    getline(inFile, currPlayerName);

    bool loadValidated = verifyName(player1Name) && verifyName(player2Name) && 
         verifyListString(player1Hand) && verifyListString(player2Hand) &&
         verifyBoardShapeString(boardShape) && verifyBoardString(boardString) &&
         verifyBoardSize(boardShape) && verifyListString(tileBagString) && 
         verifyName(currPlayerName);

    // if the format is valid
    if (loadValidated) {

        // load players -----------------
        this->setPlayers(player1Name, player2Name);

        // load player score -------------
        int p1Score = stoi(player1Score);
        int p2Score = stoi(player2Score);
        player1->setScore(p1Score);
        player2->setScore(p2Score);

        // load player hands---------------
        loadHand(player1Hand, player1Name);
        loadHand(player2Hand, player2Name);

        // load board ---------------------
        board = new GameBoard();
        std::stringstream shapeStream(boardShape);
	    std::string line;
	    getline(shapeStream, line, ',');

        // place tiles on board
        if (boardString != "") {
            std::stringstream boardStream(boardString);
            while (boardStream.good()) {
                getline(boardStream, line, ',');
                if (line[0] == ' ') {
                    line.erase(line.begin());}
                
					    
				std::string pos = line.substr(3);
                char r = pos[0];
                int c = stoi(pos.substr(1));

                Tile* tileToPlace = new Tile(line[0],line[1]-'0');
				board->placeTile(r, c, tileToPlace);
			}
		}
        this->gameBoard = board;

        // load tilebag ----------------------
        TileBag* bag = new TileBag(true);
        std::string tiles = tileBagString;
        bag->loadBag(tiles);
        this->tileBag = bag; 

        // load current player ----------------
        this->setCurrentPlayer(currPlayerName);
    }
    else {
        inFile.close();

        if (board != nullptr)
            delete board;

        std::cerr << "The file is not formatted correctly! Can't load game!" 
                  << std::endl;
    } 
    return loadValidated;     
}
     
bool GameEngine::verifyName(std::string s) {
	bool valid = s.length() != 0 ? true : false;

	for (char c : s) {
		if (!isupper(c))
            valid = false;			
	}
	return valid;
}

bool GameEngine::verifyListString(std::string s){
    bool valid = false;
	std::regex r("^([ROYGBP][1-6](, {0,1}[ROYGBP][1-6])*){0,1}$");

	if (regex_match(s, r))
		valid = true;
	
	return valid;
}

bool GameEngine::verifyBoardShapeString(std::string s) {
    bool valid = false;
	std::regex r("^\\d{1,2}, {0,1}\\d{1,2}$");
	if (regex_match(s, r))
		valid = true;
	
	return valid;
}

bool GameEngine::verifyBoardString(std::string s) {
    
    bool valid = false;
	std::regex r("^([ROYGBP][1-6]@[A-Z]\\d{1,2}(, {0,1}[ROYGBP][1-6]@[A-Z]\\d{1,2})*){0,1}$");

	if (regex_match(s, r))
		valid = true;

	return valid;
}

bool GameEngine::verifyBoardSize(std::string s) {

    bool valid = false;
    int rows, cols;
	std::stringstream shapeStream(s);
	std::string line;
	getline(shapeStream, line, ',');

	rows = stoi(line);
	getline(shapeStream, line, ',');

	if (line[0] == ' ')
		line.erase(line.begin());

	cols = stoi(line);

    if (rows == MAX_DIM && cols == MAX_DIM) 
        valid = true;
    
    return valid; 
}