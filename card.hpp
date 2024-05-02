// Card.hpp
#ifndef CARD_HPP
#define CARD_HPP

#include <string>

class Card {
 private:
  int suit;  // stores card suit.
  // "CLUBS" = 0,
  // "SPADES" = 1,
  // "DIAMONDS" = 2,
  // "HEARTS" = 3.

  // stores card rank. ranks are defined as equal to card values.
  // For face cards, we have:
  // JACK = 11
  // QUEEN = 12
  // KING = 13
  // ACE = 14
  int rank;

 public:
  // Default Constructor for the Two of Clubs
  Card();
  // Constructor
  Card(const int& suit, const int& rank);

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
  int getSuit() const;

  // Get the rank of the card. Remember, face cards are defined:
  // JACK = 11
  // QUEEN = 12
  // KING = 13
  // ACE = 14
  int getRank() const;

  std::string printCard() const;
};

#endif  // CARD_HPP
