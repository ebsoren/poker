

#include "round.hpp"

using namespace std;

Round::Round(std::vector<std::unique_ptr<Player>>& players)
    : players(players) {}

Round::~Round() {}

void Round::setup() {
  for (int i = 0; i < players.size(); ++i) {
    players[i]->setHandFirst(pack.dealCard());
  }
  for (int i = 0; i < players.size(); ++i) {
    players[i]->setHandSecond(pack.dealCard());
  }
  for (int i = 0; i < 3; ++i) {
    community_cards.push_back(pack.dealCard());
  }
}

void Round::preflop() {
  // needs some work
  cout << "\nit is the preflop\n" << endl;
  for (int i = 0; i < players.size(); ++i) {
    players[i]->playTurn(currentBet, pot, true);
  }
}

void Round::flop() {
  cout << "\nit is the flop.\nThe cards are: \n";
  for (int i = 0; i < community_cards.size(); ++i) {
    cout << community_cards[i].printCard() << endl;
  }

  for (int i = 0; i < players.size(); ++i) {
    players[i]->playTurn(currentBet, pot, false);
  }
}

void Round::turn_or_river() {
  pack.burnCard();
  community_cards.push_back(pack.dealCard());
  if (!river) {
    river = true;
    cout << "\nit is the turn\nThe cards are: \n";

  } else {
    cout << "\nit is the river\nThe cards are: \n" << endl;
  }
  for (int i = 0; i < community_cards.size(); ++i) {
    cout << community_cards[i].printCard() << endl;
  }

  for (int i = 0; i < players.size(); ++i) {
    players[i]->playTurn(currentBet, pot, false);
  }
}