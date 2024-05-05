#include "decider.hpp"

#include <iostream>

using namespace std;

void remove_duplicates(std::vector<Card>& cards) {
  for (auto it = cards.begin(); it != cards.end(); ++it) {
    auto next_it = next(it);
    if (it->getRank() == next_it->getRank()) {
      it = cards.erase(it);
      --it;
    }
  }
}

Classifier::Classifier() : bestHand{HIGHCARD, Card(Rank::NONE, Suit::NONEs)} {}
Classifier::~Classifier() {}

Classifier::Classifier(const vector<Card> cards_in)
    : cards(cards_in), bestHand(HIGHCARD, Card(NONE, NONEs)) {}

void Classifier::sortCards() {
  sort(cards.begin(), cards.end(),
       [](const Card& a, const Card& b) { return a.getRank() < b.getRank(); });
}

void Classifier::highCardClassifier() {
  if (found_hand) {
    return;
  }
  bestHand = {HIGHCARD, cards[cards.size() - 1]};
}

void Classifier::pairClassifier() {
  if (found_hand) {
    return;
  }
  int num_pairs = 0;
  for (int i = 0; i < cards.size() - 1; ++i) {
    if (cards[i].getRank() == cards[i + 1].getRank()) {
      ++num_pairs;
      bestHand.second = cards[i];
    }
  }
  if (num_pairs == 1) {
    bestHand.first = PAIR;
    found_hand = true;
  } else if (num_pairs > 1) {
    bestHand.first = TWOPAIR;
    found_hand = true;
  }
}

void Classifier::threeKindClassifier() {
  if (found_hand) {
    return;
  }
  for (int i = 0; i < cards.size() - 2; ++i) {
    if (cards[i].getRank() == cards[i + 1].getRank() &&
        cards[i + 1].getRank() == cards[i + 2].getRank()) {
      bestHand = {THREEKIND, cards[i]};
      found_hand = true;
    }
  }
}

void Classifier::fourKindClassifier() {
  if (found_hand) {
    return;
  }
  for (int i = 0; i < cards.size() - 3; ++i) {
    if (cards[i].getRank() == cards[i + 1].getRank() &&
        cards[i + 1].getRank() == cards[i + 2].getRank() &&
        cards[i + 2].getRank() == cards[i + 3].getRank()) {
      bestHand = {FOURKIND, cards[i]};
      found_hand = true;
    }
  }
}

void Classifier::straightClassifier() {
  if (found_hand) {
    return;
  }
  vector<Card> tempCards = cards;
  remove_duplicates(tempCards);
  int num_iterations = 3 + cards.size() - tempCards.size();

  for (int j = 0; j < num_iterations; ++j) {
    int tracker = 0;
    for (int i = j; i < j + 4; ++i) {
      if (tempCards[i].getRank() == tempCards[i + 1].getRank() - 1) {
        ++tracker;
      }
      if (tracker >= 4) {
        bestHand = {STRAIGHT, cards[i]};
        found_hand = true;
      }
    }
  }
}

void Classifier::flushClassifier() {
  unordered_map<Suit, int> suitCount;

  for (const Card& card : cards) {
    suitCount[card.getSuit()]++;
  }
  Suit cardSuit = NONEs;
  for (const Card& card : cards) {
    cardSuit = card.getSuit();
    if (suitCount[cardSuit] >= 5) {
      if (bestHand.first == STRAIGHT) {
        vector<Card> flushCards;
        for (const Card& card2 : cards) {
          if (card2.getSuit() == card.getSuit()) {
            flushCards.push_back(card2);
          }
        }
        int counter = 0;
        for (int i = 0; i < flushCards.size() - 1; ++i) {
          if (counter < 4 && cards[i].getRank() != cards[i + 1].getRank() - 1) {
            bestHand = {FLUSH, card};
            return;
          }
          ++counter;
        }

        bestHand = {STRAIGHTFLUSH, card};
        return;

      } else {
        bestHand = {FLUSH, card};
        return;
      }
      found_hand = true;
    }
  }
}

std::pair<Hand, Card> Classifier::getBestHand() { return bestHand; }

// void Classifier::printHand() { cout << bestHand.first << "\n"; }

Decider::Decider(vector<pair<Card, Card>> hands, vector<Card> cards)
    : allHands(hands), community_cards(cards) {}

void Decider::determineHand(const vector<Card>& hand) {
  classifier.setCards(hand);
  classifier.sortCards();
  classifier.flushClassifier();
  classifier.straightClassifier();
  classifier.fourKindClassifier();
  classifier.threeKindClassifier();
  classifier.pairClassifier();
  classifier.highCardClassifier();
}

pair<int, pair<Hand, Card>> Decider::determineWinner() {
  vector<pair<int, pair<Hand, Card>>> finalHands;
  for (int i = 0; i < allHands.size(); ++i) {
    community_cards.push_back(allHands[i].first);
    community_cards.push_back(allHands[i].second);
    determineHand(community_cards);
    finalHands.push_back({i, classifier.getBestHand()});
    classifier.resetBestHand();
    community_cards.pop_back();
    community_cards.pop_back();
  }
  pair<int, pair<Hand, Card>> bestHand = finalHands[0];
  for (int i = 1; i < finalHands.size(); ++i) {
    if (finalHands[i].second.first > bestHand.second.first) {
      bestHand = finalHands[i];
    } else if (finalHands[i].second.first == bestHand.second.first) {
      if (finalHands[i].second.second.getRank() >
          bestHand.second.second.getRank()) {
        bestHand = finalHands[i];
      } else if (finalHands[i].second.second.getRank() ==
                 bestHand.second.second.getRank()) {
        cout << "oh shit its a tie\n";
      }
    }
  }
  return bestHand;
}
