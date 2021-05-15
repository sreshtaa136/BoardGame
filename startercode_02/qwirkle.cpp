
#include <iostream>
#include <cctype>
#include <string>
#include <fstream>
#include "TileCodes.h"
#include "Tile.h"
#include "Node.h"
#include "LinkedList.h"
#include "TileBag.h"
#include "Player.h"
#include "GameBoard.h"
#include "GameEngine.h"

char alphabets[26] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K','L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'};
char colours[6] = {RED, ORANGE, YELLOW, GREEN, BLUE, PURPLE};
int shapes[6] = {CIRCLE, STAR_4, DIAMOND, SQUARE, STAR_6, CLOVER};

bool validateName (std :: string name);
bool qwirkle();
bool endGame(GameEngine* engine);
void newGame(std::string player1, std::string player2);
bool verifyCommand(std::string command);
bool verifyHand(std::string command, GameEngine *engine, std::string currentPlayer);
bool loadGame(std::string fileName);


int main(int argc, char** argv) {

   // TileBag* bag = new TileBag();
   // bag->shuffleBag();
   // std::cout<< bag->getBagSize() << std::endl;
   // LinkedList* list = new LinkedList();
   // list = bag->createHand();
   // Tile* tile;
   // tile = bag->drawTile();
   //std::cout<< bag->getBagSize() << std::endl;
   // Tile* t1 = new Tile('Y', 1);
   // Tile* t2 = new Tile('Y', 2);
   // Tile* t3 = new Tile('Y', 3);
   // Tile* t4 = new Tile('Y', 4);
   
   // list->addToEnd(t1);
   // list->addToEnd(t2);
   // list->addToEnd(t3);
   // list->addToEnd(t4);

   // std::cout<< list->getSize() << std::endl;

   // list->removeTile('Y', 1);
   // list->removeTile('Y', 2);
   // list->removeTile('Y', 3);
   // list->removeTile('Y', 4);

   // std::cout<< list->getSize() << std::endl;
   
   
   
   int choice = 0;
   while (choice !=4 ){
      std::cout << "\nWelcome to Qwirkle!\n";
      std::cout << "-------------------\n";
      std::cout << "Menu\n";
      std::cout << "----\n";
      std::cout << "1. New Game\n";
      std::cout << "2. Load Game\n";
      std::cout << "3. Credits (Show student information) \n";
      std::cout << "4. Quit \n\n";
      std::cout << "> ";

      std::cin >> choice;
   
      while (std::cin.fail())
      {
         std::cout << "Invalid Input" << std::endl;
         std::cin.clear();
         std::cin.ignore(256, '\n');
         std::cout << "> ";
         std::cin >> choice;
         ;
      }
      if (choice == 1)
      {
         std::string player1;
         std::string player2;
         std::cout << "\nStarting a New Game \n\n";
         std::cout << "Enter a name for player 1 (uppercase characters only) \n";
         do{
            std::cout << "> "; 
            std::cin >> player1;
         } while (!validateName(player1));

         do{
            std::cout << "\nEnter a name for player 2 (uppercase characters only) \n"
                     << "> ";
            std::cin >> player2;
         } while (!validateName(player2) || (player1 == player2));

         std::cout << "\nLet's Play!\n\n";

         GameEngine *engine = new GameEngine();
         std::string currentPlayer = player1;
         std::cout << currentPlayer << ", it's your turn\n";
         engine->newGame(player1, player2);

         std::cout << "Your hand is\n";
         engine->getPlayer1()->getPlayerHand()->printList();
         std::cout << "\n";
         int turn = 0;
         int iterator = 0;
         bool quit = false;

         while(!endGame(engine) && !quit){

            if(turn != 0){
               std::cout << "\n";
               std::cout << currentPlayer << ", it's your turn\n";
               engine->printGameState();
               std::cout << "Your hand is\n";
               engine->getPlayer(currentPlayer)->getPlayerHand()->printList();
               std::cout << "TileBag size: " << engine->tileBag->getBagSize() << std::endl;
               std::cout << "\n";
            }

            std::string userAction;

            std::cout << "> ";
            if (iterator == 0){
               std::cin.ignore();
               iterator ++ ;
            }

            //std::cin.ignore();
            getline(std::cin, userAction);
            
            while (!verifyCommand(userAction))
            {
               std::cout << "\n Invalid input \n";
               std::cout << "> "; 
               //std::cin.ignore();
               getline(std::cin, userAction);
            }

            if (userAction[0] == 'p'){

               std::string s = userAction;
               char cstr[s.size() + 1];
               std::copy(s.begin(), s.end(), cstr);
               cstr[s.size()] = '\0';

               int icol;
               char col;

               if(userAction.size() == 14){
                  col = userAction[13];
                  icol = col - '0'; 
               }else{
                  std ::string col = userAction.substr(13, 2);
                  icol = std::stoi(col);
               }

               char shape = userAction[7];
               int ishape = shape - '0';

               bool place = engine->placeTile(userAction[12], icol, userAction[6], ishape, currentPlayer);
               while(!place){
                 
                  std::cout << "\n Invalid tile \n";
                  std::cout << "> ";
                  getline(std::cin, userAction);

                  s = userAction;
                  char cstr[s.size() + 1];
                  std::copy(s.begin(), s.end(), cstr);
                  cstr[s.size()] = '\0';

                  if(userAction.size() == 14){
                     col = userAction[13];
                     icol = col - '0'; 
                  }else{
                     std ::string col = userAction.substr(13, 2);
                     icol = std::stoi(col);
                  } 

                  shape = userAction[7];
                  ishape = shape - '0';
                  place = engine->placeTile(userAction[12], icol, userAction[6], ishape, currentPlayer);
               }

               int points = engine->gameBoard->calculatePoints(userAction[12], userAction[13] - '0');
               engine->getPlayer(currentPlayer)->incrementScore(points);
               turn++;
            }
            else if (userAction[0] == 'r')
            {
               bool replace = engine->replaceTile(userAction[8], userAction[9] - '0', currentPlayer);
               while(!replace){
                  if(engine->tileBag->getBagSize()==0){
                     std::cout << "\n Cannot replace. Tile bag is empty. Enter a different command.\n";
                     std::cout << "> ";
                     getline(std::cin, userAction);
                  }else{
                     std::cout << "\n Invalid tile \n";
                     std::cout << "> ";
                     getline(std::cin, userAction);
                  }
               }
               turn++;
            }else if (userAction[0] == 's'){

               int nameSize = userAction.size() - 5;
               std ::string fileName = userAction.substr(5, nameSize);
               

            }else if(userAction[0] == 'q'){
               quit = true;
            }
            
            if(userAction[0] != 's'){
               if(currentPlayer == player1){
               
                  engine->setCurrentPlayer(player2);
                  currentPlayer = engine->getCurrentPlayer()->getName();

               }else{
                  
                  engine->setCurrentPlayer(player1);
                  currentPlayer = engine->getCurrentPlayer()->getName();
               }
            }

         }
         if(endGame(engine)){
            
            std::cout << "Game over \n";
            std::cout << "Score for " << engine->getPlayer1()->getName() 
            << ": " << engine->getPlayer1()->getScore() << "\n";
            std::cout << "Score for " << engine->getPlayer2()->getName() 
            << ": " << engine->getPlayer2()->getScore() << "\n";

            if(engine->getPlayer1()->getScore() > engine->getPlayer2()->getScore()){
               std::cout << "Player " << engine->getPlayer1()->getName() <<
               " won!\n\n";
            }else{
               std::cout << "Player " << engine->getPlayer2()->getName() <<
               " won!\n\n";
            }
            std::cout << "Goodbye \n";
         }
      }
      else if (choice == 2)
      {
         std::string fileName;

         do{
            
            std::cout << "\nEnter the file name from which load a game\n";
            std::cout << "> ";
            std::cin >> fileName;

         }while(!loadGame(fileName));
      }
      else if (choice == 3)
      {
         std::cout << "\n----------------------------------\n";
         std::cout << "\n";
         std::cout << "Name : David Aziz \n";
         std::cout << "Student ID : s3825605 \n";
         std::cout << "Email : s3825605@student.rmit.edu.au \n";
         std::cout << "\n";
         std::cout << "Name : Tanishpreet Kaur \n";
         std::cout << "Student ID : s3825118 \n";
         std::cout << "Email : s3825118@student.rmit.edu.au \n";
         std::cout << "\n";
         std::cout << "Name : Sai Sreshtaa Turaga \n";
         std::cout << "Student ID : s3814571 \n";
         std::cout << "Email : s3814571@student.rmit.edu.au \n";
         std::cout << "\n";
         std::cout << "Name : Rifat Raida Rashid Anannya \n";
         std::cout << "Student ID : s3822511 \n";
         std::cout << "Email : s3822511@student.rmit.edu.au\n";
         std::cout << "\n";
         std::cout << "----------------------------------\n";
      }
      else if (choice == 4)
      {
         std::cout << "Goodbye \n";
      }
      else
      {
         std::cout << "\nNot a Valid Choice. \n";
         std::cout << "Choose again.\n\n";
      }
   }
   
}

bool loadGame(std::string fileName){

   bool load = false;

   std::ifstream file(fileName);

   if(file.fail()){
      load = false;
   }else{

      load = true;
      //std::cout<< "\nyep" << std::endl;
   }
   return load;
}

//to run: 
//make sure you are on starter code dr and run the following in the terminal
//g++ -Wall -Werror -std=c++14 -g -O -o qwirkle qwirkle.cpp Tile.cpp Node.cpp LinkedList.cpp TileBag.cpp GameBoard.cpp Player.cpp GameEngine.cpp
//./qwirkle
//valgrind --leak-check=full ./qwirkle

bool validateName(std ::string name)
{
   for (int i = 0; i < (int) name.length(); i++)
   {
      int charAsci = name[i];
      if (!((charAsci >= 65) && (charAsci <= 90)))
      {
         std::cout << "\nOnly Letters in UPPERCASE are valid! \n";
         return false;
      }
   }
   return true;
}

bool verifyCommand(std::string command){

   bool check = true;

   if(command.length() == 14 || command.length() == 10 || command.length() == 15){
      
      bool foundColour = false ;
      bool foundShape = false;

      if (command.length() == 14 || command.length() == 15)
      {
         std::string tmp = command.substr(0, 6);

         if (tmp != "place ")
         {
            check = false;
            //return false;
         }
         for (int i = 0; i < 6; i++)
         {
            if (command[6] == colours[i])
            {
            foundColour = true;
            }
            int sha = command[7] - '0';
            if (sha== shapes[i])
            {
               foundShape = true;
            }
         }

         if (!(foundColour && foundShape)){

            check = false;
            //return false;
         }
         
         tmp = command.substr(8, 4);

         if (tmp != " at ")
         {
            check = false;
            //return false;
         }
         
         bool foundcol = false;
         bool foundrow = false;

         for (int i = 0; i < 26; i++)
         {
            if (command[12] == alphabets[i])
            {
               foundrow= true;
            }

            int icol;

            if(command.length() == 15){
               std ::string col = command.substr(13, 2);
               icol = std::stoi(col);
            }else{
               icol = command[13] - '0';
            }

            if (icol == i)
            {
               foundcol = true;
            }
         }

         if (!(foundrow && foundcol))
         {
            check = false;
            //return false;
         }

      }else {

         std ::string tmp = command.substr(0, 8);

         if (tmp != "replace ")
         {
            check = false;
            //return false;
         }
         for (int i = 0; i < 6; i++)
         {
            if (command[8] == colours[i])
            {
               foundColour = true;
            }
            int sha = command[9] - '0';
            if (sha == shapes[i])
            {
               foundShape = true;
            }
         }

         if (!(foundColour && foundShape))
         {
            check = false;
            //return false;
         }
      }
   }else {
   
      check = false;
      //return false;
   } 

   std ::string tmp = command.substr(0, 5);
   if(tmp == "save "){
      check = true;
   }

   if(command == "quit"){
      check = true;
   }

   return check;
}


//will check if you should end the game
bool endGame(GameEngine* engine){
   bool check = false;
   bool checkHand = engine->getPlayer1()->getPlayerHand()->getSize() == 0 ||
                    engine->getPlayer2()->getPlayerHand()->getSize() == 0;

   if(engine->tileBag->getBagSize() == 0 &&
   checkHand){
      check = true;
   }
   return check;
}

void newGame(std::string player1, std::string player2){

   GameEngine* engine = new GameEngine();
   engine->newGame(player1, player2);
   std::string currentPlayer = player1;

   while(!endGame(engine)){
      //user prompt
      if(currentPlayer == player1){
         currentPlayer = player2;
      }else{
         currentPlayer = player1;
      }
   }
   if(endGame(engine)){
      
      std::cout << "Game over \n";
      std::cout << "Score for " << engine->getPlayer1()->getName() 
      << ": " << engine->getPlayer1()->getScore() << "\n";
      std::cout << "Score for " << engine->getPlayer2()->getName() 
      << ": " << engine->getPlayer2()->getScore() << "\n";

      if(engine->getPlayer1()->getScore() > engine->getPlayer2()->getScore()){
         std::cout << "Player " << engine->getPlayer1()->getName() <<
         " won!\n\n";
      }else{
         std::cout << "Player " << engine->getPlayer2()->getName() <<
         " won!\n\n";
      }
      std::cout << "Goodbye \n";
   }
}
