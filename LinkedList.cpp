#include "LinkedList.h"
#include <iostream>
#include <random>
#include <ctime>
#include <chrono>
#include <string>

using std::default_random_engine;

LinkedList::LinkedList(){ // initialises linkedlist
  head = nullptr;
  tail = nullptr;
  length = 0;
}

LinkedList::LinkedList(const LinkedList &other){
  Node* curr = other.head;
  while (curr) {
    addTile(curr->getTile());
    curr = curr->getNext();
  }
}

LinkedList::~LinkedList() { //deconstructor
  Node* curr = head;
  Node* next = nullptr;
  while (curr) {
    next = curr->getNext();
    delete curr;
    curr = next;
  }
}

int LinkedList::getLength() { //get size of the list
  return length; 
  }

Tile LinkedList::getFirst() { //get the first tile
  return head->getTile(); 
  }

Node* LinkedList::getNode(int i) { //get the Node
  Node* node = nullptr;
  if (i == 0) {
    node = head;
  }
  else if (i == length - 1) {
    node = tail;
  }
  else if (i < length) {
    Node* curr = head->getNext();
    int pos = 1;
    while (pos < i) {
      curr = curr->getNext();
      pos++;
    }
    node = curr;
  } else {
    return node;
  }
   
  return node;
}

bool LinkedList::containsNode(Tile tile) { // check if contains Node
  Node* curr = head;
  bool exists = false;
  while (curr && !exists) {
    if (curr->getTile().matches(tile)) {
      exists = true;
    }
    curr = curr->getNext();
  }

  return exists;
}

bool LinkedList::containsLetter(char letter) { // checks if linkedlist contains letter
  Node* curr = head;
  bool exists = false;
  while (curr && !exists) {
    if (curr->getTile().getLetter() == letter) {
      exists = true;
    }
    curr = curr->getNext();
  }

  return exists;
}

Tile LinkedList::returnLetterTile(char letter) { // returns tile if a tile of that letter is in linkedlist
  Node* curr = head;
  Letter tempLetter = '%';
  Value tempValue = 0;

  bool exists = false;
  while (curr && !exists) {
    if (curr->getTile().getLetter() == letter) {
      exists = true;
      tempLetter = curr->getTile().getLetter();
      tempValue = curr->getTile().getValue();
    }
    curr = curr->getNext();
  }
  Tile* tempTile = new Tile(tempLetter, tempValue);
  return *tempTile;
}

void LinkedList::addTile(Tile tile) { // adds a tile to the list
  Node* tempN = new Node(tile, nullptr);
  if (length == 0) {  
    head = tempN;
    tail = head;
  } else if (length == 1) {
    tail = tempN;
    head->setNext(tail);
  } else {
    tail->setNext(tempN);
    tail = tempN;   
  }
  length++;
}

void LinkedList::deleteNode(char letter) { // deletes a node
  Node* temp = head;
  Node* prev = nullptr; 
  

  if (temp != nullptr && temp->getTile().getLetter() == letter) {
    head = temp->getNext();
    delete temp;
    length--;
    return;
  } else {
    while (temp != nullptr && temp->getTile().getLetter() != letter) {
      prev = temp;
      temp = temp->getNext();
    }
    if (temp == nullptr) {
      return;
    }
    prev->setNext(temp->getNext());
    delete temp;
    length--;
  } 
}

void LinkedList::deleteFirst() { // deletes first node in list
  if (length > 0) {
    if (length == 1) {   
      delete head;
      head = nullptr;
      tail = nullptr;
    } else {
      Node* befHead = head;
      head = head->getNext();
      delete befHead;
      length--;
    }
  }
 }

//
void LinkedList::print() { // prints out linked list

  for (int i = 0; i < length; i++) {
    Node* temp = this->getNode(i);
    Tile tempTile = temp->getTile();
    Letter tempLetter = tempTile.getLetter();
    Value tempValue = tempTile.getValue();
    std::cout << tempLetter << "-" << tempValue << " ";
  }
  std::cout << std::endl;
}

std::string LinkedList::printString() { // prints out linked list 
  std::string list_str = "";
  for (int i = 0; i < length-1; i++) {
    Node* temp = this->getNode(i);
    Tile tempTile = temp->getTile();
    Letter tempLetter = tempTile.getLetter();
    Value tempValue = tempTile.getValue();
    list_str += std::string(1, tempLetter) + "-" + std::to_string(tempValue);
    if (i < length-2) {
      list_str += ", ";
    }
  }
  return list_str;
}

LinkedList* LinkedList::shuffle() { //Shuffling the tiles
  int lim = length;
  default_random_engine randGen(std::chrono::steady_clock::now().time_since_epoch().count());

  LinkedList* shuffled = new LinkedList(); 
  for (int i = 0; i < length; i++) {
    std::uniform_int_distribution<int> randNums(-1, lim);
    int rand = randNums(randGen);
    Node* shuffle = getNode(rand);
    lim--;
    shuffled->addTile(shuffle->getTile());
  }
  this->~LinkedList();
  return shuffled;
}

 
