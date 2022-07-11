// #include "TileList.h"
#include "Board.h"
#include <array>
#include <iostream>
#include <vector>
#include <fstream>
#include <map>
#include <algorithm>
#include <iterator>
#include <set>

using std::string;
using std::cout;
using std::endl;
using std::array;
     
int Board::boardDim = 15;
// lebels of the scrabble board columns
char columnNames[]= {'A', 'B', 'C', 'D', 'E', 'F','G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O'};

void Board::FillBoard(){            //fill the scrabble board
    passCount = 0;
    // Initial state of the scrabble board    
    for(int i = 0; i < boardDim; i++){
        vector<char> temp;
        for(int j = 0; j < boardDim; j++){
            temp.push_back(' ');
        }
        board.push_back(temp);
        temp.clear();
    }
}

void Board::printBoard(){           //print the board
    std::cout<<"    0   1   2   3   4   5   6   7   8   9   10  11  12  13  14\n";
    std::cout<<"  -------------------------------------------------------------\n";
     
     //printing the board
    for (int i = 0; i < boardDim; i++){
        std::cout<<columnNames[i]<<" ";
        for(int j = 0; j < boardDim; j++){
            std::cout << "| " << board[i][j] << " ";
        }
        std::cout<<"|"<<std::endl;
    }
}

bool Board::changeTile(std::string location, char letter){ // places a tile onto the board
    int col = 0;
    bool tileCheck = true;
    for (int i = 0; i < boardDim; i++) {
        if (columnNames[i] == location[0]) {
            col = i;
        }
    }
    int row = stoi(location.substr(1,2));
    if (this->checkAround(row, col) == false) { // checks if the tile place position is valid
        tileCheck = false;
    }
    board[col][row] = letter;
    return tileCheck;
}

bool Board::wordChecker(std::vector<std::string> checker){          //checks the validity of the word
     std::vector<std::string> checkerHorizon;
      std::vector<std::string> checkerVertic;

    if(checker.size()==1){
        checkerHorizon.push_back(checker.at(0));
        checkerVertic.push_back(checker.at(0));
    }
    else{
        for(int a=0; a<checker.size(); a++){
            if(checker.at(a).substr(2,3) > checker.at(a).substr(2,3)){     //check horizontally placed tiles
                checkerHorizon.push_back(checker.at(a));
            }
        }
        for(int a=0; a<checkerVertic.size(); a++){      //check verically placed tiles
            if(checker.at(a)[1]>checker.at(a)[1]){
                checkerVertic.push_back(checker.at(a));
            }
        }
    }
    for(int i=0; i<checker.size(); i++){
        int col=0;
        bool got = false;
        char rcol;
        int row=0;
   
        for(int w=0; w < boardDim; w++){
             if(!got) {
                 if (columnNames[w] == checker.at(i)[1]) {
                    rcol = columnNames[w];
                    col = w;
                      got = true;
                     } } }
                    row =stoi(checker.at(i).substr(2,3));

                // check if there are letters above
                while ((board[col-1][row] != ' ') ){
                    checkerVertic.push_back(std::string(1,board[col-1][row]) + char(rcol-1) + (checker.at(i).substr(2,3)));
                    col--;
                    rcol = char(rcol-1);
                    
                }
                    // check if there is a letter left
                while ((board[col][row+1] != ' ')){
                    checkerHorizon.push_back(std::string(1,board[col][row+1]) + char(rcol) + (std::to_string(row+1)));
                    row++;
                }       
    }

 for(int i=0; i<checker.size(); i++){
        int col=0;
        bool got = false;
        char rcol;
        int row=0;
        for(int w=0; w < boardDim; w++){
             if(!got) {
                 if (columnNames[w] == checker.at(i)[1]) {
                    rcol = columnNames[w];
                    col = w;
                      got = true;
                      }}}
                    row =stoi(checker.at(i).substr(2,3));

                //check if there are tiles above
                while ((board[col+1][row] != ' ')){
                    checkerVertic.push_back(std::string(1,board[col+1][row]) + char(rcol+1) + (checker.at(i).substr(2,3)));
                    col++;
                    rcol= char(rcol+1);
                }
                   // check if there is a letter right
                while ((board[col][row-1] != ' ')){
                    checkerHorizon.push_back(std::string(1,board[col][row-1]) + char(rcol) + (std::to_string(row-1)));
                    row--;
                }   
    }       
                // check and remove duplicates
                for(size_t i=0;i<checkerVertic.size();i++){
                    for(size_t j=0;j<checkerVertic.size();j++){
                        if(checkerVertic[i] == checkerVertic[j] && i != j){
                            checkerVertic.erase(checkerVertic.begin()+j);
                            j--;  
                        }          
                    }   
                }
                // check and remove duplicates for 2
                for(size_t i=0;i<checkerHorizon.size();i++){
                    for(size_t j=0;j<checkerHorizon.size();j++){
                        if(checkerHorizon[i] == checkerHorizon[j] && i != j){
                            checkerHorizon.erase(checkerHorizon.begin()+j);
                            j--;  
                        }          
                    }   
                }
                //sort both vector according to position
                std::string temp;
                std::string temp2;
                    for(int i=0;i<checkerVertic.size();i++){
                        for(int j=i+1;j<checkerVertic.size();j++){
                            if((checkerVertic.at(i)[1]>checkerVertic.at(j)[1])){
                                temp=checkerVertic.at(i);
                                checkerVertic.at(i)=checkerVertic.at(j);
                                checkerVertic.at(j)=temp;    
            }           
        }
    }
                    for(int i=0;i<checkerHorizon.size();i++){
                        for(int j=i+1;j<checkerHorizon.size();j++){
                            if(stoi(checkerHorizon.at(i).substr(2,3)) > stoi(checkerHorizon.at(j).substr(2,3))){
                                temp2 = checkerHorizon.at(i);
                                checkerHorizon.at(i) = checkerHorizon.at(j);
                                checkerHorizon.at(j) = temp2;
            }           
        }
    }

                //check for validity of the word
                std::string wordToCheck;
                for(int i=0; i<checkerVertic.size(); i++){
                    wordToCheck+=checkerVertic.at(i)[0];
                }

                //check for validity of the word2 if it exists
                std::string wordToCheck2;
                for(int i=0; i<checkerHorizon.size(); i++){
                    wordToCheck2+=checkerHorizon.at(i)[0];
                }

            // check file of valid scrabble words
            std::ifstream  stream1("wordList.txt");
                std::string line ;

            bool found = false;
            while( std::getline( stream1, line ) && !found){
                if(((line.find(wordToCheck) != string::npos)  && (checkerVertic.size()!= checker.size()) && (line.length()== checkerVertic.size()))  ||  ((line.find(wordToCheck2) != string::npos)&& (checkerHorizon.size()!=checker.size()) && (line.length()== checkerHorizon.size()))){ // SEARCH for the word in file
                     found = true;
                    checker.clear();
                     checkerVertic.clear();
                     checkerHorizon.clear();
                }
            }
            if (!found){
                cout << "\nWord you have placed is invalid\n";
                checker.clear();
                checkerVertic.clear();
                checkerHorizon.clear();
                found = false;
                }

        return found;
    }


void Board::restoreBoard(std::vector<std::string> checker) {    //restores the state of the board
    int column;
    for(int i=0; i<checker.size(); i++){
      for (int h = 0; h < 15; h++) {
        if (columnNames[h] == checker.at(i)[1]) {
            column = h;
        }
    }
    int rows = stoi(checker.at(i).substr(2,3));
    board[column][rows] = ' ';
} 
}

bool Board::checkAround(int row, int col) { // checks around to see if tile is next to another tile
    bool aroundCheck = true;
    if (board[col][row] != char(32)) {
        return false;
    } else if (col != 0 && col != 14 && row != 0 && row != 14) {
        if (board[col-1][row] == ' ' && board[col+1][row] == ' ' && board[col][row-1] == ' ' && board[col][row+1] == ' ') {
            aroundCheck= false;
        }
    } else if (col == 0 && row == 0) {
        if (board[col+1][row] == ' ' && board[col][row+1] == ' ') {
            aroundCheck= false;
        }
    } else if (col == 14 && row == 0) {
        if (board[col-1][row] == ' ' && board[col][row+1] == ' ') {
            aroundCheck= false;
        }
    } else if (col == 0 && row == 14) {
        if (board[col+1][row] == ' ' && board[col][row-1] == ' ') {
            aroundCheck= false;
        }
    } else if (col == 14 && row == 14) {
        if (board[col-1][row] == ' ' && board[col][row-1] == ' ') {
            aroundCheck= false;
        }
    } else if (col == 0) {
        if (board[col+1][row] == ' ' && board[col][row-1] == ' ' && board[col][row+1] == ' ') {
            aroundCheck= false;
        }
    } else if (col == 14) {
        if (board[col-1][row] == ' ' && board[col][row-1] == ' ' && board[col][row+1] == ' ') {
            aroundCheck= false;
        }
    } else if (row == 0) {
        if (board[col-1][row] == ' ' && board[col+1][row] == ' ' && board[col][row+1] == ' ') {
            aroundCheck= false;
        }
    } else if (row == 14) {
        if (board[col-1][row] == ' ' && board[col+1][row] == ' ' && board[col][row-1] == ' ') {
            aroundCheck= false;
        }
    }
    return aroundCheck;;
}

bool Board::firstTile(std::string location, char letter){ //places central tile down at the start of the game
    int col = 0;
    bool tCheck = true;
    for (int i = 0; i < boardDim; i++) {
        if (columnNames[i] == location[0]) {
            col = i;
        }
    }
    int row = stoi(location.substr(1,2));
    if (board[col][row] != char(32)) {
        tCheck= false;
    } else {
        board[col][row] = letter;
    }
    return tCheck;
}

std::string Board::writeBoardState() { 
    std::string boardState = "";
    for (int i = 0; i < boardDim; i++) {
        for (int j = 0; j < boardDim; j++) {
            if (board[i][j] != char(32)) {
                boardState.append(std::string(1, board[i][j]) + "@" + std::string(1, columnNames[i]) + std::to_string(j) + " ");
            }
        }
    }
    return boardState;
}

void Board::incrementPassCount() {  // counts how many consecutive passes have been made
    passCount++;
}

int Board::getPassCount() { // gets how many consecutive passes have been made
    return passCount;
}

void Board::resetPassCount() { // resets how many consecutive passes have been made
    passCount = 0;
}