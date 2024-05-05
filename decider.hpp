#ifndef DECIDER_HPP
#define DECIDER_HPP

#include <string>
#include <utility>
#include <vector>

#include "card.hpp"
#include "pack.hpp"
#include "player.hpp"

enum Hand {
  HIGHCARD = 0,
  PAIR = 1,
  TWOPAIR = 2,
  THREEKIND = 3,
  STRAIGHT = 4,
  FLUSH = 5,
  FULLHOUSE = 6,
  FOURKIND = 7,
  STRAIGHTFLUSH = 8,
  ROYALFLUSH = 9,

};

class Classifier {
 public:
  // constructor, also sorts cards_in by rank
  Classifier();
  Classifier(const std::vector<Card> cards_in);

  // destructor
  ~Classifier();

  // returns the highest card
  void highCardClassifier();
  // returns the highest card in a suit
  Suit highCardClassifier(Suit suit_in);

  // returns the rank of paired cards (or NONE if none exist). rank_in is used
  // so that different pairs can be detected in repeat calls of PairClassifier.
  void pairClassifier();

  // returns the rank of 3 of a kind cards (or NONE if none exist)
  void threeKindClassifier();

  void fourKindClassifier();

  // returns true if a flush exists
  void flushClassifier();

  // returns true if a straight exists
  void straightClassifier();

  void setCards(std::vector<Card> cards_in) { cards = cards_in; }
  void sortCards();

  std::pair<Hand, Card> getBestHand();

   void resetBestHand(){
    bestHand = {HIGHCARD,{TWO, SPADES}};
    found_hand = false;
  }

  // void printHand();

 private:
  std::vector<Card> cards;

  // best hand for player
  std::pair<Hand, Card> bestHand;
  bool found_hand = false;
}; // end classifier





class Decider {
 public:
  // constructor; brings in the hands left and the community cards
  Decider(std::vector<std::pair<Card, Card> > hands, std::vector<Card> cards);

  // destructor
  ~Decider();

  void determineHand(const std::vector<Card>& hand);
 
  std::pair<int, std::pair<Hand, Card> > determineWinner();

 private:
  std::vector<std::pair<Card, Card> > allHands;
  std::vector<Card> community_cards;
  Classifier classifier;
};

#endif  // decider.hpp
