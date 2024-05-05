#include "decider.hpp"

#include <iostream>

using namespace std;

Classifier::Classifier(const vector<Card> cards_in) : cards(cards_in) {}

Card Classifier::HighCardClassifier() {
  Card* max_card = new Card(TWO, SPADES);

  for (int i = 0; i < cards.size(); ++i) {
    if (cards[i].getRank() >= max_card->getRank()) {
      max_card = &cards[i];
    }
  }
  return *max_card;
}

Card Classifier::PairClassifier(Rank rank_in) {
  unordered_map<Rank, int> rankCount;

  for (const Card& card : cards) {
    if (card.getRank() != rank_in) {
      rankCount[card.getRank()]++;
    }
  }

  for (const Card& card : cards) {
    Rank cardRank = card.getRank();
    if (rankCount[cardRank] >= 2 && cardRank != rank_in) {
      return card;
    }
  }

  return Card(NONE, NONEs);
}

Card Classifier::ThreeKindClassifier(Rank rank_in) {
  unordered_map<Rank, int> rankCount;

  for (const Card& card : cards) {
    if (card.getRank() != rank_in) {
      rankCount[card.getRank()]++;
    }
  }

  for (const Card& card : cards) {
    Rank cardRank = card.getRank();
    if (rankCount[cardRank] >= 3 && cardRank != rank_in) {
      return card;
    }
  }

  return Card(NONE, NONEs);
}

Card Classifier::FlushClassifier() {
  unordered_map<Suit, int> suitCount;

  for (const Card& card : cards) {
    suitCount[card.getSuit()]++;
  }

  for (const Card& card : cards) {
    Suit cardSuit = card.getSuit();
    if (suitCount[cardSuit] >= 5) {
      return card;
    }
  }

  return Card(NONE, NONEs);
}

Card Classifier::StraightClassifier() {
  
}