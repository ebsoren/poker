#include "card.hpp"

#include <vector>

using namespace std;

// Constructor implementation
Card::Card(const Rank& rank, const Suit& suit) : rank(rank), suit(suit) {}

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
Suit Card::getSuit() const { return suit; }

// Rank grab implementation
Rank Card::getRank() const { return rank; }

// prints a card

string Card::printSuit() const {
  string card_suit;
  static const vector<string> suits = {"CLUBS", "SPADES", "DIAMONDS", "HEARTS",
                                       "NONE"};
  card_suit = suits[suit];

  return card_suit;
}

string Card::printRank() const {
  string card_rank;
  static const vector<string> ranks = {"TWO",   "THREE", "FOUR", "FIVE", "SIX",
                                       "SEVEN", "EIGHT", "NINE", "TEN",  "JACK",
                                       "QUEEN", "KING",  "ACE",  "NONE"};
  card_rank = ranks[rank - 2];

  return card_rank;
}
string Card::printCard() const { return printRank() + " of " + printSuit(); }
