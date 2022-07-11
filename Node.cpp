
#include "Node.h"


Node::Node(Tile tile, Node* next):tile(tile){
   this->tile = tile;
   this->nextNode = next;
}

Node::Node(const Node& other):tile(other.tile){
}

void Node::setNext(Node* nextNode){
    this->nextNode = nextNode; 
}

Node* Node::getNext(){ 
    return nextNode; 
}

Tile Node::getTile(){ 
    return tile; 
}
