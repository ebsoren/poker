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

string printS(Rank rank){
  if(rank == SIX){
    return "ES ";
  }
    return "S ";
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
  setWinLoc(cards.size() - 1);
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

void Classifier::fullHouseClassifier() {
  if (found_hand) {
    return;
  }
  Rank threeRank;
  int loc;
  bool Three = false;
  for (int i = 0; i < cards.size() - 2; ++i) {
    if (cards[i].getRank() == cards[i + 1].getRank() &&
        cards[i + 1].getRank() == cards[i + 2].getRank()) {
      threeRank = cards[i].getRank();
      loc = i;
      Three = true;
    }
  }
  for (int i = 0; i < cards.size() - 1; ++i) {
    if (cards[i].getRank() == cards[i + 1].getRank() &&
        cards[i].getRank() != threeRank && Three) {
      bestHand = {FULLHOUSE, cards};
      setWinLoc(loc);
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

  if (tempCards[0].getRank() == TWO && tempCards[1].getRank() == THREE &&
      tempCards[2].getRank() == FOUR && tempCards[3].getRank() == FIVE &&
      tempCards[tempCards.size() - 1].getRank() == ACE) {
    bestHand = {STRAIGHT, cards};
  }
}

void Classifier::flushClassifier() {
  if (found_hand && bestHand.first != STRAIGHT) {
    return;
  }

  unordered_map<Suit, int> suitCount;
  int loc = 0;
  for (int i = 0; i < cards.size(); ++i) {
    suitCount[cards[i].getSuit()]++;
    if (suitCount[cards[i].getSuit()] >= 5) {
      loc = i;
    }
  }
  Suit cardSuit;
  for (int i = 0; i < cards.size(); ++i) {
    cardSuit = cards[i].getSuit();

    if (suitCount[cardSuit] >= 5) {
      setWinLoc(loc);
      if (bestHand.first == STRAIGHT) {
        vector<Card> flushCards;
        for (const Card& card2 : cards) {
          if (card2.getSuit() == cards[i].getSuit()) {
            flushCards.push_back(card2);
          }
        }
        int counter = 0;
        for (int i = 0; i < flushCards.size() - 1; ++i) {
          if (counter < 4 &&
              flushCards[i].getRank() != flushCards[i + 1].getRank() - 1) {
            bestHand = {FLUSH, cards};

            found_hand = true;
            return;
          }
          ++counter;
          setWinLoc(i);
        }
        if (flushCards[flushCards.size() - 1].getRank() == ACE &&
            flushCards[flushCards.size() - 2].getRank() == KING &&
            flushCards[flushCards.size() - 3].getRank() == QUEEN &&
            flushCards[flushCards.size() - 4].getRank() == JACK &&
            flushCards[flushCards.size() - 5].getRank() == TEN) {
          found_hand = true;
          bestHand = {ROYALFLUSH, cards};

          return;
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

Decider::~Decider() {}

void Decider::determineHand(const vector<Card>& hand) {
  for (int i = 0; i < hand.size(); ++i) {
    if (hand[i].getRank() == NONE) {
      return;
    }
  }
  classifier.setCards(hand);
  classifier.sortCards();
  classifier.fourKindClassifier();
  classifier.fullHouseClassifier();
  classifier.straightClassifier();
  classifier.flushClassifier();
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
      int highCardCand = finalHands[i].first.second;
      int highCardBest = bestHandData.first.second;
      if (finalHands[i].second.second[highCardCand].getRank() >
          bestHandData.second.second[highCardBest].getRank()) {
        bestHandData = finalHands[i];
      }  // if it is the same hand with the same high card
      else if (finalHands[i].second.second[highCardCand].getRank() ==
               bestHandData.second.second[highCardBest].getRank()) {
        if (compEqualHands(bestHandData.second, finalHands[i].second.second)) {
          bestHandData = finalHands[i];
        }
      }
    }
  }

  return bestHandData;
}

bool Decider::compEqualHands(const pair<Hand, vector<Card>>& best,
                             const vector<Card>& candidate) {
  if (best.first == HIGHCARD) {
    for (int i = candidate.size() - 1; i >= 2; --i) {
      if (candidate[i].getRank() > best.second[i].getRank()) {
        return true;
      }
    }
    return false;
  } else if (best.first == PAIR || best.first == THREEKIND) {
    for (int i = best.second.size() - 1; i >= 0; --i) {
      if (best.second[i].getRank() > candidate[i].getRank()) {
        return false;
      }
      if (candidate[i].getRank() > best.second[i].getRank()) {
        return true;
      }
    }
    return false;

  } else if (best.first == TWOPAIR) {
    Rank highRankCand = NONE;
    Rank highRankBest = NONE;
    Rank highPairCand = NONE;
    Rank secondPairCand = NONE;
    Rank highPairBest = NONE;
    Rank secondPairBest = NONE;
    for (int i = candidate.size() - 1; i >= 0; --i) {
      if (candidate[i].getRank() == candidate[i - 1].getRank()) {
        if (highPairCand == NONE) {
          highPairCand = candidate[i].getRank();
        } else if (secondPairCand == NONE) {
          secondPairCand = candidate[i].getRank();
        }
      } else if (highRankCand == NONE) {
        highRankCand = candidate[i].getRank();
      }

      if (best.second[i].getRank() == best.second[i - 1].getRank()) {
        if (highPairCand == NONE) {
          highPairCand = best.second[i].getRank();
        } else if (secondPairCand == NONE) {
          secondPairCand = best.second[i].getRank();
        }
      } else if (highRankCand == NONE) {
        highRankCand = best.second[i].getRank();
      }
    }

  } else if (best.first == STRAIGHT || best.first == FLUSH ||
             best.first == STRAIGHTFLUSH || best.first == ROYALFLUSH) {
    isTie = true;
    return true;
  } else if (best.first == FULLHOUSE) {
    unordered_map<Suit, int> suitCountCand;
    unordered_map<Suit, int> suitCountBest;
    unordered_map<Suit, Rank> highestRankCand;
    unordered_map<Suit, Rank> highestRankBest;

    for (int i = 0; i < candidate.size(); ++i) {
      Suit suit = candidate[i].getSuit();
      suitCountCand[suit]++;
      if (suitCountCand[suit] <= 2) {
        Rank rank = candidate[i].getRank();
        if (suitCountCand[suit] == 1 || rank > highestRankCand[suit])
          highestRankCand[suit] = rank;
      }

      suit = best.second[i].getSuit();
      suitCountBest[suit]++;
      if (suitCountBest[suit] <= 2) {
        Rank rank = best.second[i].getRank();
        if (suitCountBest[suit] == 1 || rank > highestRankBest[suit])
          highestRankBest[suit] = rank;
      }
    }

    Rank candRank;
    Rank bestRank;

    for (const auto& pair : suitCountCand) {
      if (pair.second == 2 && highestRankCand[pair.first] > candRank) {
        candRank = highestRankCand[pair.first];
      }
    }

    for (const auto& pair : suitCountBest) {
      if (pair.second == 2 && highestRankBest[pair.first] > bestRank) {
        bestRank = highestRankBest[pair.first];
      }
    }
    if (candRank > bestRank) {
      return true;
    }
    if (candRank == bestRank) {
      isTie = true;
      return true;
    }
  }
  return false;
}
void Decider::printWinningHand(const Hand& hand, const Card& card) {
  if (hand == HIGHCARD) {
    cout << card.printRank() << " high ";
  } else if (hand == PAIR) {
    cout << "a pair of " << card.printRank() << printS(card.getRank());
  } else if (hand == TWOPAIR) {
    cout << "a two pair, high pair " << card.printRank() << printS(card.getRank());
  } else if (hand == THREEKIND) {
    cout << "three " << card.printRank() << printS(card.getRank());
  } else if (hand == STRAIGHT) {
    cout << "a straight, " << card.printRank() << " high ";
  } else if (hand == FLUSH) {
    cout << "a " << card.printSuit() << " flush, " << card.printRank()
         << " high ";
  } else if (hand == FULLHOUSE) {
    cout << "a full house, " << card.printRank() << " top ";
  } else if (hand == FOURKIND) {
    cout << "four " << card.printRank() << printS(card.getRank());
  } else if (hand == STRAIGHTFLUSH) {
    cout << "a " << card.printSuit() << " straight flush, " << card.printRank()
         << " high ";
  } else if (hand == ROYALFLUSH) {
    cout << "a royal flush!! ";
  }
}
