// g++ -Wall -Werror -std=c++14 -g -O -o scrabble LinkedList.cpp Tile.cpp Node.cpp Scrabble.cpp GamePlayer.cpp Board.cpp
#include "LinkedList.h"
#include "Board.h"
#include "GamePlayer.h"
#include <vector>
#include <iostream>
#include <limits>
#include <fstream>
#include <algorithm>
#include <map>
#include <ostream>

#define EXIT_SUCCESS    0

int quit() {            //exits the program when the user wants to
   std::cout << std::endl;
   std::cout << "Goodbye" << std::endl;
    exit(0);
   return EOF;
}

void help(){         // players can get help at any time during the game by entering help
   std::cout<<" ------------------------WELCOME TO SCRABBLE------------------------\n"<<std::endl;
   std::cout<<"The players take turns placing tiles from their hand onto the board to form a word one by one. A tile should be placed on an valid location, it can be placed above, below, left or right to an existing tile on the board. The words are checked for validity and the player\'s scores are incremented accordingly. At the end of the game, the player with the highest score is declared as the winner.\n"<<std::endl;
   std::cout<<"SCORE FOR THE LETTERS"<<std::endl;
   std::cout<<"Letters                               Values"<<std::endl;
   std::cout<<" A, E, I, O, U, L, N, S, T, R    |       1"<<std::endl;
   std::cout<<" D,G                             |       2"<<std::endl;
   std::cout<<" B, C, M, P                      |       3"<<std::endl;
   std::cout<<" K                               |       5"<<std::endl;
   std::cout<<" J, X                            |       8"<<std::endl;
   std::cout<<" Q, Z                            |       10\n"<<std::endl;

   std::cout<<"If the player places all 7 tiles in their hand it's a BINGO! where the player scores 30 points\n"<<std::endl;

   std::cout<<"PLACING TILE: The players place a tile from their hand onto the board by typing in the letter and location they wish to place the tile. For example: 'place T at G7' will place letter T on G7 location. When the player is done they can enter 'place Done' to end thier turn\n"<<std::endl;
   std::cout<<"PASSING: If the player cannot form a word on the board, they can pass their turn by entering 'pass'.\n"<<std::endl;
   std::cout<<"REPLACING TILE: The player can chose to replace tiles in their hand from the tile bag. Only one tile replacement per turn is allowed. This can be achived by entering the replace command and the tile you wish to replace. For example: 'replace T'.\n"<<std::endl;
   std::cout<<"SAVE: The game can be saved anytime during the play by entering save command and the file name.\n"<<std::endl;
   std::cout<<"LOAD: The saved game can be loaded to contniue the game by selecting the load game option from the menu.\n"<<std::endl;
   std::cout<<"QUIT: If you wish to quit the game, simply enter quit to end the game."<<std::endl;
}

bool checkLine(std::vector<std::string> locations){      // checks if tile is being placed up, down, left or right of an existing tile
   bool lineCheck = true;
    if (locations.size() > 1) {
      for(std::size_t i = 1; i < locations.size(); ++i) {
         std::string temp1 = locations[i];
         std::string temp2 = locations[i-1];
         char temp1a = temp1[0];
         char temp1b = temp1[1];
         char temp2a = temp2[0];
         char temp2b = temp2[1];
         if (temp1a != temp2a && temp1b != temp2b) {
            lineCheck= false;
         }
      }
   }
   return lineCheck;
}

bool enableWordChecker = true;
// save game to fileName
void saveMulti(int currentPlayeIdx,vector<GamePlayer*> players, Board board, LinkedList* gameBag, std::string fileName) {
   std::ofstream saveFile(fileName);
   saveFile << players.size() << std::endl;
   saveFile <<players.at(currentPlayeIdx)->getName() << std::endl;
   if(enableWordChecker)
       saveFile << 1 << std::endl;
   else
       saveFile << 0 << std::endl;
   for(int i=0;i<(int)players.size();i++){      //saving to the file
       GamePlayer* p = players.at(i);
       saveFile << p->getName() << std::endl;
       saveFile << p->getScore() << std::endl;
       saveFile << p->getHand()->printString() << std::endl;
   }
   saveFile << board.writeBoardState() << std::endl;
   saveFile << gameBag->printString() << std::endl;
   saveFile.close();
}

void endGameMulti(vector<GamePlayer*> players){
    vector<int> scoreArray;
    int won_score = 0;

    for(int i=0;i<(int)players.size();i++){
        std::cout << "Score for " << players.at(i)->getName() << ": " << players.at(i)->getScore() << std::endl;
        int score = players.at(i)->getScore();
        scoreArray.push_back(score);
        if(score>won_score)
            won_score = score;
    }

    bool draw = true;
    for(int i = 0; i < (int)scoreArray.size()-1; i++) {      //check if the game is draw
         if(scoreArray[i] != scoreArray[i + 1])
             draw=false;
    }
    if(draw){
        std::cout << "The game is a draw!" << std::endl;
    }
    else{
       for(int i=0;i<(int)players.size();i++){
           int score = players.at(i)->getScore();
           if(score==won_score){
             std::cout << "Player " << players.at(i)->getName() << " won!" << std::endl;
           }
       }
    }

    quit();

}

// end game
void endGame(GamePlayer* p1, GamePlayer* p2){
   std::cout << "Score for " << p1->getName() << ": " << p1->getScore() << std::endl;
   std::cout << "Score for " << p2->getName() << ": " << p2->getScore() << std::endl;
   if (p1->getScore() > p2->getScore()) {
      std::cout << "Player " << p1->getName() << " won!" << std::endl;
   } else if (p2->getScore() > p1->getScore()) {
      std::cout << "Player " << p2->getName() << " won!" << std::endl;
   } else {
      std::cout << "The game is a draw!" << std::endl;
   }
}

// replace tile in hand
bool replaceTile(GamePlayer* p, char letter, LinkedList* gamebag){
   bool replaced = false;
   if (p->getHand()->containsLetter(letter)) {
      gamebag->addTile(p->getHand()->returnLetterTile(letter)); // add tile back to gamebag
      p->getHand()->deleteNode(letter);
      p->getHand()->addTile(gamebag->getFirst());
      gamebag->deleteFirst();
      replaced = true;
      std::cout << "replaced" << std::endl;
   }
   else{
      replaced = false;
   }
   return replaced;
}

// the actual gameplay loop
Board gameplayMenuMulti(int currentPlayeIdx, vector<GamePlayer*> players, Board board, LinkedList* gameBag){
   GamePlayer* p = players.at(currentPlayeIdx);
   std::cout << p->getName() << ", it's your turn" << std::endl;
   for(int i=0;i<(int)players.size();i++){
       std::cout << "Score for " << players.at(i)->getName() << ": " << players.at(i)->getScore() << std::endl;
   }
   board.printBoard();
   std::cout << std::endl;
   std::cout << "Your hand is" << std::endl;
   LinkedList* hand = p->getHand();
   hand->print();
   bool running = true;
   std::string options;
   std::string letloc;
   std::vector<std::string> locations {};
   std::vector<std::string> checker;
   std::vector<char> wordLetters;

   std::cout << "> ";
   while (running && getline(std::cin >> std::ws, options)) {
      std::ifstream is(options);
      if (is.eof()) {
         running = false;
         is.close();
         board.incrementPassCount();
         board.incrementPassCount();
         board.incrementPassCount();
         quit();
      } else {
         std::string place = options.substr(0, 5);
         std::transform(place.begin(), place.end(), place.begin(),
                     [](unsigned char c) -> unsigned char { return std::tolower(c); });
         std::string replace = options.substr(0, 7);
         std::transform(replace.begin(), replace.end(), replace.begin(),
                     [](unsigned char c) -> unsigned char { return std::tolower(c); });
         std::string save1 = options.substr(0, 4);
         std::transform(replace.begin(), replace.end(), replace.begin(),[](unsigned char c) -> unsigned char { return std::tolower(c); });

         if (save1.compare("save") == 0){
             int l = options.length();
             if(l>5){
                 saveMulti(currentPlayeIdx, players, board, gameBag, options.substr(5, options.length()));
                 std::cout << "Game successfully saved" << std::endl;
                 std::cout << "> ";
             }
             else{
                 std::cout << "Please specify output file name." << std::endl;
                 std::cout << "> ";
             }

         }else if(options.compare("help")==0){
            help();
         } else if (options.compare("quit") == 0){
            quit();
            running = false;
         } else if (place.compare("place") == 0) {
            if (options.substr(6,4).compare("Done") == 0){
               running = false;

           bool wordFound = true;
           if(enableWordChecker==true)
               wordFound = board.wordChecker(checker);
           if(wordFound){
                char letter;
                std::cout<<"\n";
                for(size_t word=0; word<checker.size(); word++){
                        letter= checker.at(word)[0];
                        p->incrementScore(p->getHand()->returnLetterTile(letter).getValue());
                        p->getHand()->deleteNode(letter);
                 }
          }
         else{
            board.restoreBoard(checker);
         }
       } else {
               char letter = options[6];
               if(options.length()>12){
               std::string location = options.substr(11,3);
               locations.push_back(location);
               if (p->getHand()->containsLetter(letter)) {
                  char columnNames[]= {'A', 'B', 'C', 'D', 'E', 'F','G', 
                  'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O'};
                        int col=0;
                     for (int i = 0; i < 15; i++) {
                        if (columnNames[i] == location[0]) {
                              col = i;
                        }
                     }
                     int row = stoi(location.substr(1,2));
                  if (checkLine(locations) && board.checkAround(row, col)) {
                     if (board.changeTile(location, letter)) {
                           letloc = letter+location;
                           checker.push_back(letloc);             // major enhancement 
                     } else {
                        std::cout << "You have placed the tile on an invalid location, a tile should be placed above, below, left or right to an existing tile. Please try again" << std::endl;
                     }
                  } else {
                     locations.pop_back();
                     std::cout << "You have made an entry thats invalid, please try again." << std::endl;
                  }
               }
               // Better invalid input
               else{
                                       locations.pop_back();
                  std::cout<<"Tile does not exist in the hand"<<std::endl;
                  }
               if (p->getHand()->getLength() == 0) {
                  std::cout << "BINGO!!!" << std::endl;
                  p->incrementScore(50);
                  running = false;
               }
               board.resetPassCount();
               std::cout << "> ";
              }
               else{
                   std::cout<<"Not a valide input, please try again\n";
                   std::cout << "> ";
               }
            }
         } else if (replace.compare("replace") == 0) {
            char letter = options[8];
            if(replaceTile(p, letter, gameBag)){
            board.resetPassCount();
            running = false;}
            else{                      // if tile is not present in the player's hand, display this message
               std::cout<<"Tile is not in hand, please select again\n";
            }

         } else if (options.compare("pass") == 0){
            board.incrementPassCount();
            running = false;
         } else {
            std::cout << "Invalid Input" << std::endl;
            std::cout << "> ";
         }
      }
   }
   p->fillHand(gameBag);      // fill the player's hand
   return board;
}

// Newly implemented multi player setting up gameplay for the gameplay loop
void gameplayMulti(Board board, vector<GamePlayer*> players, LinkedList* gameBag) {

   for(int i=0;i<(int)players.size();i++){
      players[i]->setHand(gameBag);
   }
   while (board.getPassCount() < 2 && gameBag->getLength() > 0) {
        for(int i=0;i<(int)players.size();i++){
            board = gameplayMenuMulti(i, players, board, gameBag);
        }
   }
   endGameMulti(players);
}


// setting up loaded game
void gameplayLoadedMulti(int currentPlayeIdx,Board board, vector<GamePlayer*> players, LinkedList* gameBag) {
    int startIdx = currentPlayeIdx;
    while (board.getPassCount() < 2 && gameBag->getLength() > 0){
       for(int i=startIdx;i<(int)players.size();i++){
           board = gameplayMenuMulti(i, players, board, gameBag);
       }
       startIdx =0;
   }
   endGameMulti(players);
}

// new game set up
void newGame(string option) {
   int numPlayers = 0; //default 2
   if(option=="1"){
       numPlayers = 2;
   }
   else{
   bool flg = true;
   while(flg){
       std::cout << "\nEnter number of player, aviable options are 3,4" << std::endl;
       std::cout << "> ";
       string optionStr;

       std::cin>>optionStr;
       if(optionStr=="quit"){
            quit();
       }
       else{
           if(sscanf(optionStr.c_str(), "%d", &numPlayers) != 1){
                std::cout << "\nPlease enter a number" << std::endl;
           }

           else if(numPlayers==3||numPlayers==4){
               flg = false;
           }
           else{
               std::cout << "\nNot a valid option, aviable options are 2,3,4" << std::endl;
           }
       }
   }
   }

   // vector of players
   vector<GamePlayer*> players;           //major enhancement
   std::cout << "\nStarting a New Game" << std::endl;
   for(int j=0; j<numPlayers; j++){
      std::string playerName = "";
      std::cout << "\nEnter a name for player " << j+1 <<" (uppercase characters only)" << std::endl;
      std::cout << "> ";
      std::cin>>playerName;
      int length = playerName.length();
      bool playerNameValid = true;

      for (int i = 0; i < length; i++) {
         char name = playerName[i];
         if (islower(name) || isdigit(name) || !isalpha(name)|| isblank(name)){
            playerNameValid = false;
            }
         }

      while(playerNameValid == false){
         // Better invalid             // checks if the user is entering a valid name
         std::cout << "Please enter the name in capital letters without special characters or numbers" << std::endl;
         std::cout << "> ";
         std::cin>>playerName;
         int length = playerName.length();

         playerNameValid = true;
         for (int i = 0; i < length; i++) {
            char name = playerName[i];
         if (islower(name) || isdigit(name) || !isalpha(name)|| isblank(name)){
               playerNameValid = false;}
            }
         }
         GamePlayer *player = new GamePlayer(playerName);
         players.push_back(player);
      }
   std::cout<<"\nLet's Play!"<<std::endl;
   Board board;
   board.FillBoard();            //fill the scrabble board

   std::ifstream infile("ScrabbleTiles.txt");
   std::string line;
   LinkedList* gameBag = new LinkedList();
   // int asdasf = 0;
   while (std::getline(infile, line)) {
      char letter = line[0];
      int value = std::stoi(line.substr(2,2));
      Tile* tempTile = new Tile(letter, value);
      gameBag->addTile(*tempTile);
   }
   gameBag = gameBag->shuffle();
   char firstTile =  gameBag->getFirst().getLetter();
   board.firstTile("F7", firstTile);
   gameBag->deleteNode(gameBag->getFirst().getLetter());
   std::cout<<firstTile;
   board.printBoard();
   gameplayMulti(board, players, gameBag);
}

// load game from file
void loadGameMulti() {
   vector<GamePlayer*> players;
   std::string fileName = "";
   std::cout << "\nEnter the filename from which load a game" << std::endl << ">";
   std::cin >> fileName;

   std::ifstream infile(fileName);
   if(infile.fail()){
       std::cout << "\nFile does not exist, please check the file name." << std::endl << ">";
       return;
   }
   std::string line;
   std::string tile;
   std::getline(infile, line);
   int playerCount =  std::stoi( line );
   if(playerCount<2||playerCount>4){
       std::cout << "\nNot valid file, player count not match" << std::endl << ">";
       return;
   }

   std::getline(infile, line);

   //get player name
    std::string currentPlayerName = line;
    //get the word checker status
    std::getline(infile, line);
    std::string wordChekerStatus = line;
    if(wordChekerStatus=="1"){
        enableWordChecker= true;
        std::cout << "Word Checker Activated." << std::endl;
    }
    else if(wordChekerStatus=="0"){
        enableWordChecker= false;
        std::cout << "Word Checker Dectivated." << std::endl;
    }
    else{
        std::cout << "\nWord checker status not saved corectly, disabling word checker." << std::endl << ">";
        enableWordChecker= false;
    }


   for(int i=0;i<playerCount;i++){
       std::getline(infile, line);
       GamePlayer* player = new GamePlayer(line);
       std::getline(infile, line);
       player->setScore(std::stoi(line));
       LinkedList* hand1 = new LinkedList();
       std::getline(infile, line);
       std::string delimiter = ", ";
       size_t pos = 0;
       std::string token;
       while ((pos = line.find(delimiter)) != std::string::npos) {
          token = line.substr(0, pos);
          Letter tempLetter = token[0];
          Value tempValue = token[2] - '0';
          Tile* tempTile = new Tile(tempLetter, tempValue);
          hand1->addTile(*tempTile);
          line.erase(0, pos + delimiter.length());
       }
       Letter tempLetter = line[0];
       Value tempValue = line[2] - '0';
       Tile* tempTile = new Tile(tempLetter, tempValue);
       hand1->addTile(*tempTile);
       player->setHand(hand1);
       players.push_back(player);   // add the player to the vector
   }

   std::getline(infile, line);
   Board board;
   board.FillBoard();         //fill the scrabble board
   size_t pos = 0;
   std::string token;

   std::string delimiter = " ";
   while ((pos = line.find(delimiter)) != std::string::npos) {
      token = line.substr(0, pos);
      board.firstTile(token.substr(2,3), token[0]);
      line.erase(0, pos + delimiter.length());
   }
   std::getline(infile, line);
   delimiter = ", ";
   pos = 0;
   LinkedList* gameBag = new LinkedList();
   while ((pos = line.find(delimiter)) != std::string::npos) {
      token = line.substr(0, pos);
      Letter tempLetter2 = token[0];
      Value tempValue2 = token[2] - '0';
      Tile* tempTile2 = new Tile(tempLetter2, tempValue2);
      gameBag->addTile(*tempTile2);
      line.erase(0, pos + delimiter.length());
   }
   board.printBoard();
   int currentPlayeIdx = 0;
   for(int i=0;i<(int)players.size();i++){
       string name = players.at(i)->getName();
       if((currentPlayerName.compare(name)) == 0)
           currentPlayeIdx = i;
   }

   gameplayLoadedMulti(currentPlayeIdx,board,players,  gameBag);

}

void Activate_Deactivate_word_checker(){        //configureable enhancement
    std::string option;
    std::cout << "\nEnter the option for Word Checker, 1 for Enable , 0 For Disable" << std::endl;
    std::cout << "> ";
    std::cin >> option;
    while ((option != "0") && (option != "1")) { // checks whether the selectin is valid on the menu
        if(option=="quit"){
             quit();
        }
        else{
         std::cout << "Invalid Selection, Enter correct option for Word Checker, 1 for Enable , 0 For Disable" << std::endl;
         std::cout << "> ";
         std::cin >> option;
        }
    }
    if(option=="0"){
        enableWordChecker=false;
        std::cout << "Word Checker Dectivated." << std::endl;
    }
    else if(option=="1"){
        enableWordChecker=true;
        std::cout << "Word Checker Activated." << std::endl;
    }
    std::cout << "Please Select Menu" << std::endl;
    std::cout << "----" << std::endl;
    std::cout << "1. New Game" << std::endl;
    std::cout << "2. Multiplayer game" << std::endl;
    std::cout << "3. Word checker Activation" << std::endl;
    std::cout << "4. Load Game" << std::endl;
    std::cout << "5. Credits (Show student information)" << std::endl;
    std::cout << "6. Quit \n" << std::endl;
    std::cout << "> ";
}
// print credits
void credits() {
   std::cout << std::endl;
   std::cout << "----------------------------------" << std::endl;
   std::cout << "Name: Hirday Bajaj" << std::endl;
   std::cout << "Student ID: s3901303" << std::endl;
   std::cout << "Email: s3901303@student.rmit.edu.au" << std::endl;
   std::cout << std::endl;

   std::cout << "Name: James Leong" << std::endl;
   std::cout << "Student ID: s3906993" << std::endl;
   std::cout << "Email: s3906993@student.rmit.edu.au" << std::endl;
   std::cout << std::endl;

   std::cout << "Name: Adem Truong" << std::endl;
   std::cout << "Student ID: s3900998" << std::endl;
   std::cout << "Email: s3900998@student.rmit.edu.au" << std::endl;
   std::cout << std::endl;

   std::cout << "Name: Mohammed Usman E Ghani" << std::endl;
   std::cout << "Student ID: s3901999" << std::endl;
   std::cout << "Email: s3901999@student.rmit.edu.au" << std::endl;
   std::cout << "----------------------------------" << std::endl;
   std::cout << std::endl;
}

// first menu
void menu() {
   std::cout <<"Welcome to Scrabble!" << std::endl;
   std::cout <<"------------------- \n" << std::endl;
   std::cout << "Menu" << std::endl;
   std::cout << "----" << std::endl;
   std::cout << "1. New Game" << std::endl;
   std::cout << "2. Multiplayer game" << std::endl;
   std::cout << "3. Word checker activation" << std::endl;
   std::cout << "4. Load Game" << std::endl;
   std::cout << "5. Credits (Show student information)" << std::endl;
   std::cout << "6. Quit \n" << std::endl;

   std::string option;
   std::cout << "> ";
      while (std::cin >> option) {
         while ((option != "1") && (option != "2") && (option != "3") && (option !="4") && (option !="5") && (option !="6")) { // checks whether the selectin is valid on the menu
         if(option=="quit"){
              quit();
         }
         else{
             // Better invalid input
             std::cout << "Invlaid selection - please select an option from the menu" << std::endl;
             std::cin >> option;
         }
         }
         if (option == "1" || option == "2") {
           newGame(option);
         }
         else if (option == "3") {
             Activate_Deactivate_word_checker();
         }
         else if (option == "4") {
         //loadGame();
         loadGameMulti();
         } else if (option == "5") {
         credits();
         menu();
         } else if (option == "6") {
         quit();
         }
      }
      quit();
}
int main(void) {     // main function for the entry point. The game starts from the menu
   menu();
   return EXIT_SUCCESS;
}