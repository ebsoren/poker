#include <vector>

#include "card.hpp"

using namespace std;

// Constructor implementation
Card::Card() {}
Card::Card(const int& suit, const int& rank) : suit(suit), rank(rank) {}

// Assignment operator implementation
// Returns a pointer to the card.
Card& Card::operator=(const Card& other) {
  if (this != &other) {
    suit = other.suit;
    rank = other.rank;
  }
  return *this;
}

// Destructor implementation
Card::~Card() {}

// Suit grab implementation
int Card::getSuit() const { return suit; }

// Rank grab implementation
int Card::getRank() const { return rank; }

// prints a card
string Card::printCard() const {
  string card_suit;
  static const vector<string> suits = {"CLUBS", "SPADES", "DIAMONDS", "HEARTS"};
  card_suit = suits[suit];

  string card_rank;
  static const vector<string> ranks = {"TWO",   "THREE", "FOUR", "FIVE", "SIX",
                                       "SEVEN", "EIGHT", "NINE", "TEN",  "JACK",
                                       "QUEEN", "KING",  "ACE"};
  card_rank = ranks[rank - 2];

  return card_rank + " of " + card_suit;
}
