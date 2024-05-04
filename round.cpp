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

  num_players = players.size();
}

void Round::preflop() {
  cout << "\nit is the preflop\n" << endl;
  playAll();
  checkWinner();
}

void Round::postflop(const string current_round) {
  if (num_players == 1) {
    return;
  }
  pack.burnCard();
  addCommunityCard();
  if(current_round == "flop"){
    addCommunityCard(); 
    addCommunityCard(); 
  } 
  cout << "\nit is the " << current_round << "\nThe cards are: \n";
  for (int i = 0; i < community_cards.size(); ++i) {
    cout << community_cards[i].printCard() << endl;
  }
  currentBet = 0;

  playAll();
  checkWinner();
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

void Round::playAll() {
  for (int i = 0; i < players.size(); ++i) {
    players[i]->play(currentBet, pot, num_players, previous_raise);
  }

  while (!checkIfContinue()) {
    for (int i = 0; i < players.size(); ++i) {
      players[i]->play(currentBet, pot, num_players, previous_raise);
    }
  }
}

void Round::resetBets() {
  for (int i = 0; i < players.size(); ++i) {
    players[i]->setBets_Stack(0);
  }
  previous_raise = 0;
}

bool Round::checkIfContinue() {
  vector<int> betSizes;
  for (int i = 0; i < players.size(); ++i) {
    if (!players[i]->has_folded_func()) {
      betSizes.push_back(players[i]->getTotalBet());
    }
  }

  if (std::equal(betSizes.begin() + 1, betSizes.end(), betSizes.begin())) {
    return true;
  }

  return false;
}

void Round::addCommunityCard() { community_cards.push_back(pack.dealCard()); }