
#ifndef ASSIGN2_LINKEDLIST_H
#define ASSIGN2_LINKEDLIST_H 

#include "Node.h"
#include "stdlib.h"
#include <string>

class LinkedList {

public:
  LinkedList();
  LinkedList(const LinkedList& toCopy);
  ~LinkedList();

  int getLength();
  Tile getFirst();  // return tile at the head
  LinkedList* shuffle();
  bool containsNode(Tile tile);
  bool containsLetter(char letter);
  Tile returnLetterTile(char letter);
  void addTile(Tile tile);  // add tile to the end of the list
  void deleteNode(char letter);
  void deleteFirst();
  void replaceTile(Tile tile,
                   Tile newTile);  // remove a specific tile from the
                                   // list and replace with a new one
  void deleteTile(Tile tile);
  void print();
  std::string printString();
  Node* getNode(int i);
//   void displayContents(bool saveFile);
private:
  Node* head;
  Node* tail;
  int length;
};

#endif  // ASSIGN2_LINKEDLIST_H