#ifndef BOARD_H
#define BOARD_H

#define BOARD_DIM   15;
#include <iostream>
#include <vector>
#include <map>

using std::vector;

class Board {
public:
    void FillBoard();
    void printBoard();
    bool changeTile(std::string coords, char letter);
    std::string writeBoardState();
    void incrementPassCount();
    int getPassCount();
    void resetPassCount();
    bool firstTile(std::string coords, char letter);
    bool wordChecker(std::vector<std::string> checker);
    bool checkAround(int row, int col);
    void restoreBoard(std::vector<std::string> checker);
 

private:
    static int boardDim;
    int passCount;
    
    

    //vector of vectors storing the state of the scrabble board
    vector<vector<char> > board;
    
};

#endif // BOARD_H