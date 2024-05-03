

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

  num_players = players.size();
}

void Round::preflop() {
  num_raises = 0;
  cout << "\nit is the preflop\n" << endl;
  playTurnAll();
  checkWinner();

  if (get_num_raises() > 0) {
    playTurnAll();
  }
}

void Round::flop() {
  num_raises = 0;
  if (num_players == 1) {
    return;
  }
  cout << "\nit is the flop.\nThe cards are: \n";
  for (int i = 0; i < community_cards.size(); ++i) {
    cout << community_cards[i].printCard() << endl;
  }
  currentBet = 0;

  playTurnAll();
  checkWinner();

  while (get_num_raises() > 0) {
    playTurnAll();
  }
}

void Round::turn_or_river() {
  if (num_players == 1) {
    return;
  }
  currentBet = 0;
  num_raises = 0;

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

  playTurnAll();
  checkWinner();

  if (get_num_raises() > 0) {
    playTurnAll();
  }
}

void Round::checkWinner() {
  for (int i = 0; i < players.size(); ++i) {
    if (num_players == 1 && !players[i]->has_folded_func()) {
      cout << players[i]->getName() << " Wins the round\n";
      return;
    }
  }
}

void Round::show_cards() {
  cout << "Players show cards.\n\n";
  for (int i = 0; i < players.size(); ++i) {
    if (!players[i]->has_folded_func()) {
      cout << players[i]->getName() << " has:\n"
           << players[i]->getHand().first.printCard() << "\n"
           << players[i]->getHand().second.printCard() << "\n\n";
    }
  }
}

void Round::playTurnAll() {
  for (int i = 0; i < players.size(); ++i) {
    players[i]->playTurn(currentBet, pot, num_players, num_raises);
  }
}

int Round::get_num_raises() { return num_raises; }

void Round::resetBets(){
  for(int i = 0; i < players.size();++i){
    players[i]->setBet(0);
  }
}
