#include <iostream>

#include "player.hpp"
#include "decider.hpp"

using namespace std;

int main() {
  vector<Card> cards;

  cards.push_back(Card(KING, DIAMONDS));
  cards.push_back(Card(QUEEN, DIAMONDS));
  cards.push_back(Card(FIVE, HEARTS));
  cards.push_back(Card(SEVEN, SPADES));
  cards.push_back(Card(SEVEN, CLUBS));

  vector<pair<Card, Card>> hands = {{Card(ACE, SPADES), Card(KING, HEARTS)}};

  // Add another pair of Card objects
  hands.push_back({Card(SEVEN, HEARTS), Card(TWO, CLUBS)});

  Decider* decider = new Decider(hands, cards);

  pair<int, pair<Hand, Card>> winnerData = decider->determineWinner();

  cout << winnerData.first << " won with a "
       << winnerData.second.second.printCard() << endl;

  cout << "decider created successfully\n";
}