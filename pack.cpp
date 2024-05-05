#include "pack.hpp"

#include <iostream>

using namespace std;

// Initializes  a pack with no cards dealt. the vector of cards runs from 2 to
// A, running from Clubs->Spades->Diamonds->Hearts.
Pack::Pack() {
  for (int suit = static_cast<int>(Suit::CLUBS);
       suit <= static_cast<int>(Suit::HEARTS); ++suit) {
    for (int rank = static_cast<int>(Rank::TWO);
         rank <= static_cast<int>(Rank::ACE); ++rank) {
      cards.emplace_back(static_cast<Rank>(rank), static_cast<Suit>(suit));
    }
  }
}
// Shuffles the cards in the pack. shuffle implementation needed.
void Pack::shuffle() {}

// Deals one card from the top of the pack. Will also be used to deal cards for
// the flop, turn, and river.
Card Pack::dealCard() {
  int num = num_dealed;
  ++num_dealed;
  return cards[num];
}
// simply skips a card within the pack, since the dealCard function relies on
// num_dealed to decide which card to deal.
void Pack::burnCard() {
  ++num_dealed;
  cout << "A card has been burned.\n";
}

// Resets the pack with a complete set of unshuffled cards
void Pack::resetPack() {
  cards.clear();
  num_dealed = 0;

  const vector<Suit> suits = {CLUBS, SPADES, DIAMONDS, HEARTS};
  const vector<Rank> ranks = {TWO,  THREE, FOUR, FIVE,  SIX,  SEVEN, EIGHT,
                              NINE, TEN,   JACK, QUEEN, KING, ACE};

  for (const auto& suit : suits) {
    for (const auto& rank : ranks) {
      cards.emplace_back(rank, suit);
    }
  }
}

// Destructor
Pack::~Pack() {}