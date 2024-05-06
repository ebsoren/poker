#ifndef CARD_HPP
#define CARD_HPP

#include <string>

enum Rank {
  TWO = 2,
  THREE = 3,
  FOUR = 4,
  FIVE = 5,
  SIX = 6,
  SEVEN = 7,
  EIGHT = 8,
  NINE = 9,
  TEN = 10,
  JACK = 11,
  QUEEN = 12,
  KING = 13,
  ACE = 14,
  NONE = 0,  // used when we need a "did not get a rank" returned
};

enum Suit {
  CLUBS = 0,
  SPADES = 1,
  DIAMONDS = 2,
  HEARTS = 3,

};

class Card {
 private:
  // stores card suit.
  // "CLUBS" = 0,
  // "SPADES" = 1,
  // "DIAMONDS" = 2,
  // "HEARTS" = 3.
  Suit suit = CLUBS;

  // stores card rank. ranks are defined as equal to card values.
  // For face cards, we have:
  // JACK = 11
  // QUEEN = 12
  // KING = 13
  // ACE = 14
  Rank rank = TWO;

 public:
  // Constructor
  Card(const Rank& rank, const Suit& suit);

  // Destructor
  ~Card();

  // Assignment operator
  Card& operator=(const Card& card);

  // Equivalence operator. Based ONLY on rank.
  Card& operator==(const Card& card);

  // less than operator. Based ONLY on rank.
  Card& operator<=(const Card& card);

  // Get the suit of the card. Remember, suits are defined:
  // "CLUBS" = 0,
  // "SPADES" = 1,
  // "DIAMONDS" = 2,
  // "HEARTS" = 3.
  Suit getSuit() const;

  // Get the rank of the card. Remember, face cards are defined:
  // JACK = 11
  // QUEEN = 12
  // KING = 13
  // ACE = 14
  Rank getRank() const;

  std::string printCard() const;
  std::string printRank() const;
  std::string printSuit() const;
};

#endif  // CARD_HPP
