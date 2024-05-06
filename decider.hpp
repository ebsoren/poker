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

  std::pair<Hand, std::vector<Card>> getBestHand();

  void resetBestHand() {
    std::vector<Card> temp;
    temp.push_back(Card(NONE, SPADES));

    bestHand = {HIGHCARD, temp};
    found_hand = false;
  }

  int getWinLoc() { return winLoc; }
  void setWinLoc(int wl_in) { winLoc = wl_in; }

  // void printHand();

 private:
  std::vector<Card> cards;

  // best hand for player
  std::pair<Hand, std::vector<Card>> bestHand;
  int winLoc = 0;

  bool found_hand = false;
};  // end classifier

class Decider {
 public:
  // constructor; brings in the hands left and the community cards
  Decider(std::vector<std::pair<Card, Card>> hands, std::vector<Card> cards);

  // destructor
  ~Decider();

  void determineHand(const std::vector<Card>& hand);

  std::pair<std::pair<int, int>, std::pair<Hand, std::vector<Card>>>
  determineWinner();

  void compEqualHands(const vector<Card>& best, const vector<Card>& candidate);

 private:
  std::vector<std::pair<Card, Card>> allHands;
  std::vector<Card> community_cards;
  Classifier classifier;
};

#endif  // decider.hpp
