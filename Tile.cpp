#include "Tile.h"
// Tile- letter and their corresponding points

Tile::Tile(Letter letter, Value value)
{
  this->letter = letter;
  this->value = value;
}

Letter Tile::getLetter()    //get letter
{
  return this->letter;
}

Value Tile::getValue()    //get value
{
  return this->value;
}

bool Tile::matches(Tile tile) {   // check if the tile matches
    bool matches;
   if(letter == tile.letter && value == tile.value){
       matches= true;
   }
   else{
       matches = false;
   }
   return matches;
}
