#ifndef DECIDER_HPP
#define DECIDER_HPP

#include <string>
#include <utility>
#include <vector>

#include "card.hpp"
#include "pack.hpp"
#include "player.hpp"

enum Type {
  HighCard = 0,
  Pair = 1,
  TwoPair = 2,
  ThreeKind = 3,
  Straight = 4,
  Flush = 5,
  FullHouse = 6,
  FourKind = 7,
  StraightFlush = 8,
  RoyalFlush = 9,

};

class Decider {
 public:
  // constructor; brings in the hands left and the community cards
  Decider(std::vector<std::pair<Card, Card> > hands, std::vector<Card> cards);

  // destructor
  ~Decider();

  // returns the best hand
  std::pair<Type, int> bestHand();

 private:
  std::vector<std::pair<Card, Card> > allHands;
  std::vector<Card> community_cards;
};

class Classifier {
 public:
  // constructor
  Classifier(const std::vector<Card> cards_in);

  // destructor
  ~Classifier();

  // returns the highest card
  Card HighCardClassifier();

  // returns the rank of paired cards (or NONE if none exist). rank_in is used
  // so that different pairs can be detected in repeat calls of PairClassifier.
  Card PairClassifier(Rank rank_in);

  // returns the rank of 3 of a kind cards (or NONE if none exist)
  Card ThreeKindClassifier(Rank rank_in);

  // returns true if a flush exists
  Card FlushClassifier();

  // returns true if a straight exists
  Card StraightClassifier();

  // full house, four of a kind, straight flush, and royal flush can be
  // determined with the above functions.

 private:
  std::vector<Card> cards;
  std::pair<Type, int> handValue;
};

#endif  // decider.hpp
