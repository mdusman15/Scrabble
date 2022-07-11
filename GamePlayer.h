
#ifndef GAMEPLAYER_H
#define GAMEPLAYER_H

#include <string>
#include "LinkedList.h"
using std::string;

class GamePlayer{
private:
    std::string playerName;
    int score;
    LinkedList* hand;
 
public:
  GamePlayer();
  ~GamePlayer();
  GamePlayer(const GamePlayer &other);
  GamePlayer(std::string name, int score, LinkedList* tiles);
  GamePlayer(std::string name);

  void incrementScore(int score);
  string getName();
  void setScore(int score);
  int getScore();
  LinkedList* getHand();
  void setHand(LinkedList* tiles);
  void fillHand(LinkedList* gamebag);
 
 };

#endif  //GAMEPLAYER_H
