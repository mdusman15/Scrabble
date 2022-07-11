#include "GamePlayer.h"
#include <iostream>
using std::string;

GamePlayer::GamePlayer() {    
}

GamePlayer::GamePlayer(string name, int score, LinkedList* tiles){ // initialises player
    this->playerName = name;
    this->hand = tiles;
    this->score = score;
}

GamePlayer::GamePlayer(string name){ // initialises player 
    this->playerName = name;
    this->score = 0;
    this->hand = new LinkedList();
}

GamePlayer::~GamePlayer() { // deconstructor
    delete hand;
}

string GamePlayer::getName(){ // returns player name
    return playerName; 
}

void GamePlayer::setScore(int score){ // sets player score
    this->score = score; 
}

void GamePlayer::incrementScore(int score){ // increments score
    this->score += score; 
}

int GamePlayer::getScore() { // gets score
    return score;
}

LinkedList* GamePlayer::getHand(){ // gets player hand
    return hand; 
}

void GamePlayer::setHand(LinkedList* tiles){ // sets hand using tilebag
    int length = tiles->getLength(); // makes sure only 7 tiles are added
    if (length > 7) {
        length = 7;
    }
    for (int i = 0; i < length; i++) { // adds tiles from tilebag then deletes the tiles from tilebag
        Tile temp = tiles->getFirst();
        tiles->deleteFirst();
        hand->addTile(temp);
    }
}

// fill the hand back to 7 tiles
void GamePlayer::fillHand(LinkedList* tiles){ 
    int tempLength = this->getHand()->getLength();
    tempLength = 7 - tempLength;
    for (int i = 0; i < tempLength; i++) {
        Tile temp = tiles->getFirst();
        tiles->deleteFirst();
        hand->addTile(temp);
    }
}
