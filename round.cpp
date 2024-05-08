#include "round.hpp"

using namespace std;

Round::Round(std::vector<std::unique_ptr<Player>>& players)
    : players(players) {}

Round::~Round() {}

void Round::setup(int& blind_loc) {
  for (int i = 0; i < players.size(); ++i) {
    players[i]->setHandFirst(pack.dealCard());
  }
  for (int i = 0; i < players.size(); ++i) {
    players[i]->setHandSecond(pack.dealCard());
  }

  num_players = players.size();
  if(blind_loc == players.size()-1){
     players[blind_loc]->setBlind(1);
  players[0]->setBlind(2);
  blind_loc = -1;
  }
  else{
    players[blind_loc]->setBlind(1);
    players[blind_loc+1]->setBlind(2);
  }
  ++blind_loc;
 
}

void Round::preflop() {
  cout << "\nit is the preflop\n" << endl;
  
  playAll();
  checkWinner();
 
}

void Round::postflop(const string current_round) {
  if (num_players <= 1) {
    return;
  }
  //int counter = 0;
  //for(int i = 0; i < players.size(); ++i){
  //  if(players[i]->is_all_in_func() || players[i]->has_folded_func()){
  //    ++counter;
  //  }
  //}
  //if(counter > players.size()-1){
  //  return;
  //}
  pack.burnCard();
  addCommunityCard();
  if (current_round == "flop") {
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
      cout << players[i]->getName() << " wins the round and gains $"
           << getPot() - players[i]->getTotalBet() << ".\n\nNew Round\n\n";
      players[i]->addStack(getPot());
      return;
    }
  }
}

bool Round::show_cards() {
  if (num_players == 1) {
    return false;
  }

  cout << "\nThe community cards are:\n\n";
  for(int i = 0; i < community_cards.size(); ++i){
    cout << community_cards[i].printCard() << "\n";
  }
  cout << "\nPlayers show cards.\n\n";
  for (int i = 0; i < players.size(); ++i) {
    if (!players[i]->has_folded_func()) {
      cout << players[i]->getName() << " has:\n"
           << players[i]->getHand().first.printCard() << "\n"
           << players[i]->getHand().second.printCard() << "\n\n";
    }
  }
  return true;
}

void Round::playAll() {
  bool blindRaise = false;
  for (int i = 0; i < players.size(); ++i) {
    players[i]->play(currentBet, pot, num_players, previous_raise, blindRaise);
  }

  while (!checkIfContinue()) {
    for (int i = 0; i < players.size(); ++i) {
      players[i]->play(currentBet, pot, num_players, previous_raise, blindRaise);
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
  int counter = 0;
  for (int i = 0; i < players.size(); ++i) {
    if (!players[i]->has_folded_func()) {
      betSizes.push_back(players[i]->getRoundBet());
    }
    if(players[i]->is_all_in_func()){
      ++counter;
    }
  }
  if(counter >= players.size()-1){
    return true;
  }

  if (std::equal(betSizes.begin() + 1, betSizes.end(), betSizes.begin())) {
    return true;
  }

  // if everybody is all in continue
  // if everybody except for one person is all in
  // 

  return false;
}

void Round::addCommunityCard() { community_cards.push_back(pack.dealCard()); }

vector<pair<Card, Card>> Round::getHands() {
  vector<pair<Card, Card>> returnhands;
  for (int i = 0; i < players.size(); ++i) {
    if (players[i]->has_folded_func()) {
      returnhands.push_back({Card(NONE, SPADES), Card(NONE, SPADES)});
    } else {
      returnhands.push_back(players[i]->getHand());
    }
  }
  return returnhands;
}

void Round::printWinner(pair<pair<int, int>, pair<Hand, vector<Card>>> winnerData){
  cout << "and gains $"
         << getPot() - players[winnerData.first.first]->getTotalBet() << ".\n";
    players[winnerData.first.first]->addStack(getPot());

    for (auto it = players.begin(); it != players.end();) {
      if ((*it)->getStack() == 0) {
        cout << (*it)->getName() << " is out of the game.\n";
        it = players.erase(it);
      } else {
        ++it;
      }
    }
    if (players.size() == 1) {
      cout << players[0]->getName() << " wins! Their payout is $"
           << players[0]->getStack() << ".\n";
           game_over = true;
    } else {
      cout << "\nThe remaining players are:\n";
      for (const auto &playerPtr : players) {
        cout << playerPtr->getName() << endl;
      }
      cout << "\n\nNew Round\n\n";
    }
}



