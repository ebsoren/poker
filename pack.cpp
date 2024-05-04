#include "pack.hpp"

#include <iostream>

using namespace std;

// Initializes  a pack with no cards dealt. the vector of cards runs from 2 to
// A, running from Clubs->Spades->Diamonds->Hearts.
Pack::Pack() {
  for (int i = 0; i < 4; ++i) {
    for (int j = 2; j < 15; ++j) {
       cards.emplace_back(i, j);
    }
  }
}
// Shuffles the cards in the pack. shuffle implementation needed.
void Pack::shuffle() {
  
}

// Deals one card from the top of the pack. Will also be used to deal cards for
// the flop, turn, and river.
Card Pack::dealCard() {
  int num = num_dealed;
  ++num_dealed;
  return cards[num];

}
// simply skips a card within the pack, since the dealCard function relies on
// num_dealed to decide which card to deal.
void Pack::burnCard() { ++num_dealed; cout << "A card has been burned.\n"; }

// Resets the pack with a complete set of unshuffled cards
void Pack::resetPack() {
  cards.clear();
  num_dealed = 0;
  
  const vector<int> suits = {0, 1, 2, 3};
  const vector<int> ranks = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14};

  for (const auto& suit : suits) {
    for (const auto& rank : ranks) {
      cards.emplace_back(suit, rank); 
    }
  }
}

// Destructor
Pack::~Pack() {}