
#ifndef ASSIGN2_NODE_H
#define ASSIGN2_NODE_H

#include "Tile.h"

class Node {
private:
  Tile tile;
  Node* nextNode;

public:
  Node(Tile tile, Node* nextNode);
  Node(const Node& other);

  Node* getNext();
  void setNext(Node* nextNode);
  Tile getTile();
};

#endif // ASSIGN2_NODE_H
