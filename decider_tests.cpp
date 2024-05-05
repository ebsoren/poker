#include <iostream>

#include "decider.hpp"

using namespace std;

int main() {
  vector<Card> cards;

  cards.push_back(Card(THREE, HEARTS));
  cards.push_back(Card(TWO, SPADES));
  cards.push_back(Card(FIVE, HEARTS));
  cards.push_back(Card(ACE, DIAMONDS));
  cards.push_back(Card(ACE, HEARTS));
  cards.push_back(Card(QUEEN, HEARTS));
  cards.push_back(Card(QUEEN, HEARTS));

  Classifier* classifier = new Classifier(cards);

  cout << "classifier created successfully\n";

  Card card = classifier->HighCardClassifier();

  // for(int i = 0; i < cards.size(); ++i){
  //   cout << cards[i].printCard() << "\n";
  // }

  cout << card.printCard() << " is the highest card\n";

  card = classifier->PairClassifier(NONE);
  cout << "player has a pair of " << card.printRank() << "S.\n";

  card = classifier->PairClassifier(card.getRank());
  if (card.getRank() != NONE) {
    cout << "player has a pair of " << card.printRank() << "S.\n";
  }

  card = classifier->ThreeKindClassifier(NONE);
  if (card.getRank() != NONE) {
    cout << "player has a three of a kind of " << card.printRank() << "S.\n";
  }

  card = classifier->FlushClassifier();
  if (card.getRank() != NONE) {
    cout << "player has a flush with 5 " << card.printSuit() << ".\n";
  }
}