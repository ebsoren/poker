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

Classifier::Classifier()
    : bestHand{HIGHCARD, vector<Card>{(Card(NONE, SPADES))}} {}
Classifier::~Classifier() {}

Classifier::Classifier(const vector<Card> cards_in)
    : cards(cards_in), bestHand(HIGHCARD, vector<Card>{(Card(NONE, SPADES))}) {}

void Classifier::sortCards() {
  sort(cards.begin(), cards.end(),
       [](const Card& a, const Card& b) { return a.getRank() < b.getRank(); });
}

void Classifier::highCardClassifier() {
  if (found_hand) {
    return;
  }
  setWinLoc(cards.size());
  bestHand = {HIGHCARD, cards};
}

void Classifier::pairClassifier() {
  if (found_hand) {
    return;
  }
  int num_pairs = 0;
  for (int i = 0; i < cards.size() - 1; ++i) {
    if (cards[i].getRank() == cards[i + 1].getRank()) {
      ++num_pairs;
      setWinLoc(i);
      bestHand.second = cards;
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
      bestHand = {THREEKIND, cards};
      setWinLoc(i);
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
      bestHand = {FOURKIND, cards};
      setWinLoc(i);
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
        bestHand = {STRAIGHT, cards};
        found_hand = true;
        setWinLoc(i + 1);
      }
    }
  }
}

void Classifier::flushClassifier() {
  unordered_map<Suit, int> suitCount;

  for (const Card& card : cards) {
    suitCount[card.getSuit()]++;
  }
  Suit cardSuit;
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
            bestHand = {FLUSH, cards};
            setWinLoc(i);
            found_hand = true;
            return;
          }
          ++counter;
          setWinLoc(i);
        }
        found_hand = true;
        bestHand = {STRAIGHTFLUSH, cards};

        return;

      } else {
        found_hand = true;
        bestHand = {FLUSH, cards};

        return;
      }
    }
  }
}

std::pair<Hand, vector<Card>> Classifier::getBestHand() { return bestHand; }

// void Classifier::printHand() { cout << bestHand.first << "\n"; }

Decider::Decider(vector<pair<Card, Card>> hands, vector<Card> cards)
    : allHands(hands), community_cards(cards) {}

void Decider::determineHand(const vector<Card>& hand) {
  for (int i = 0; i < hand.size(); ++i) {
    if (hand[i].getRank() == NONE) {
      return;
    }
  }
  classifier.setCards(hand);
  classifier.sortCards();
  classifier.flushClassifier();
  classifier.straightClassifier();
  classifier.fourKindClassifier();
  classifier.threeKindClassifier();
  classifier.pairClassifier();
  classifier.highCardClassifier();
}

pair<pair<int, int>, pair<Hand, vector<Card>>> Decider::determineWinner() {
  vector<pair<pair<int, int>, pair<Hand, vector<Card>>>> finalHands;

  for (int i = 0; i < allHands.size(); ++i) {
    community_cards.push_back(allHands[i].first);
    community_cards.push_back(allHands[i].second);

    determineHand(community_cards);
    finalHands.push_back(
        {{i, classifier.getWinLoc()}, classifier.getBestHand()});
    classifier.resetBestHand();
    community_cards.pop_back();
    community_cards.pop_back();
  }
  pair<pair<int, int>, pair<Hand, vector<Card>>> bestHandData = finalHands[0];
  for (int i = 1; i < finalHands.size(); ++i) {
    if (finalHands[i].second.first > bestHandData.second.first) {
      bestHandData = finalHands[i];
    } else if (finalHands[i].second.first == bestHandData.second.first) {
      if (finalHands[i].second.second[finalHands[i].first.second].getRank() >
          bestHanddata.second.second[bestHandData.first.second].getRank()) {
        bestHandData = finalHands[i];
      } else if (finalHands[i]
                     .second.second[finalHands[i].first.second]
                     .getRank() ==
                 bestHandData.second.second[bestHandData.first.second]
                     .getRank()) {
        compEqualHands();
      }
    }
  }

  return bestHandData;
}

bool Decider::compEqualHands(const pair<Hand, vector<Card>>& best,
                             const vector<Card>& candidate) {
  if (best.first == HIGHCARD) {
    for (int i = candidate.size() - 1; i >= 2; --i) {
      if (candidate[i].getRank() > best[i].getRank) {
        return true;
      }
    }
    return false;
  } else if (best.first == PAIR) {
    for (auto it = candidate.begin(); it != candidate.end() - 1;) {
      auto it2 = next(it);
      if (*it == *it2) {
        it = candidate.erase(it);
        candidate.erase(it);
        break;
      } else {
        ++it;
      }
    }

    for (auto it = best.begin(); it != best.end() - 1;) {
      auto it2 = next(it);
      if (*it == *it2) {
        it = best.erase(it);
        best.erase(it);
        break;
      } else {
        ++it;
      }
    }

    for (int i = candidate.size() - 1; i >= 2; --i) {
      if (candidate[i].getRank() > best[i].getRank) {
        return true;
      }
    }
    return false;
  } else if (best.first == TWOPAIR) {
    int pairsRemoved = 0;
    auto it = vec.begin();

    while (it != vec.end() - 1) {
      auto nextIt = std::next(it);

      if (*it == *nextIt) {
        it = vec.erase(it);
        it = vec.erase(it);

        pairsRemoved++;

        if (pairsRemoved == 2) {
          break;
        }
      } else {
        ++it;
      }
    }

     int pairsRemoved = 0;
    auto it = vec.begin();

    while (it != vec.end() - 1) {
      auto nextIt = std::next(it);

      if (*it == *nextIt) {
        it = vec.erase(it);
        it = vec.erase(it);

        pairsRemoved++;

        if (pairsRemoved == 2) {
          break;
        }
      } else {
        ++it;
      }
    }
  } else if (best.first == THREEKIND) {
  } else if (best.first == STRAIGHT) {
  } else if (best.first == FLUSH) {
  } else if (best.first == FULLHOUSE) {
  } else if (best.first == FOURKIND) {
  } else if (best.first == STRAIGHTFLUSH) {
  } else if (best.first == ROYALFLUSH) {
  }
}
