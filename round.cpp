

#include "round.hpp"

using namespace std;


Round::Round(std::vector<std::unique_ptr<Player>>& players) : players(players){}

Round::~Round() {}

void Round::setup() {
  for (int i = 0; i < players.size(); ++i) {
    players[i]->setHandFirst(pack.dealCard());
  }
  for (int i = 0; i < players.size(); ++i) {
    players[i]->setHandSecond(pack.dealCard());
  }
}

void Round::preflop() {
  // needs some work
  for (int i = 0; i < players.size(); ++i) {
    players[i]->playTurn(currentBet, true);
  }
}

void Round::flop() {
  for (int i = 0; i < 3; ++i) {
    community_cards.push_back(pack.dealCard());
  }
  for (int i = 0; i < players.size(); ++i) {
    players[i]->playTurn(currentBet, false);
  }
}

void Round::turn_or_river() {
  pack.burnCard();
  community_cards.push_back(pack.dealCard());
  for (int i = 0; i < players.size(); ++i) {
    players[i]->playTurn(currentBet, false);
  }
}