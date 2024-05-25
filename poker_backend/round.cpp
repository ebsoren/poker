#include "round.hpp"

using namespace std;

int getMostRecentBet(const std::vector<std::unique_ptr<Player>>& players,
                     int loc) {
  int n = players.size();

  if (loc == 0) {
    for (int i = n - 1; i >= 0; --i) {
      if (!players[i]->has_folded_func()) {
        return players[i]->getBet();
      }
    }
  } else {
    int i = loc - 1;
    while (i != loc) {
      if (!players[i]->has_folded_func()) {
        return players[i]->getBet();
      }
      i = (i - 1 + n) % n;
    }
  }
  return 0;
}

Round::Round(std::vector<std::unique_ptr<Player>>& players)
    : players(players) {}

Round::~Round() {}

void Round::setup(int& starter) {
  if (starter == players.size() || starter == 0) {
    prev_player = players[players.size() - 1]->getName();
  } else {
    prev_player = players[starter - 1]->getName();
  }
  for (int i = starter; i <= players.size(); ++i) {
    if (i == players.size()) {
      for (int j = 0; j < starter; ++j) {
        players[j]->setHandFirst(pack.dealCard());
        if (j == 0) {
          players[j]->setPrevPlayer(players[players.size() - 1]->getName());
        } else {
          players[j]->setPrevPlayer(players[j - 1]->getName());
        }
      }
    } else {
      players[i]->setHandFirst(pack.dealCard());
      if (i == 0) {
        players[i]->setPrevPlayer(players[players.size() - 1]->getName());
      } else {
        players[i]->setPrevPlayer(players[i - 1]->getName());
      }
    }
  }

  for (int i = starter; i <= players.size(); ++i) {
    if (i == players.size()) {
      for (int j = 0; j < starter; ++j) {
        players[j]->setHandSecond(pack.dealCard());
      }
    } else {
      players[i]->setHandSecond(pack.dealCard());
    }
  }

  num_players = players.size();
  if (starter == players.size()) {
    players[0]->setIsBlind(1);
    players[1]->setIsBlind(2);
  } else if (starter == players.size() - 1) {
    players[starter]->setIsBlind(1);
    players[0]->setIsBlind(2);
  } else {
    players[starter]->setIsBlind(1);
    players[starter + 1]->setIsBlind(2);
  }
  ++starter;
}

void Round::preflop() {
  cout << "\nit is the preflop\n" << endl;

  playAll(starter);
  checkWinner();
}

void Round::postflop(const string current_round) {
  if (num_players <= 1) {
    return;
  }
  // int counter = 0;
  // for(int i = 0; i < players.size(); ++i){
  //   if(players[i]->is_all_in_func() || players[i]->has_folded_func()){
  //     ++counter;
  //   }
  // }
  // if(counter > players.size()-1){
  //   return;
  // }
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

  playAll(starter);
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
  for (int i = 0; i < community_cards.size(); ++i) {
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

void Round::playAll(int& starter) {
  bool first = true;
  pair<bool, int> bigBlindRaised = {false, 0};
  pair<Decision, int> decision;
  while (!checkIfContinue(first)) {
    for (int i = starter; i <= players.size(); ++i) {
      if (i == players.size()) {
        for (int j = 0; j < starter; ++j) {
          if (prev_player != players[j]->getPrevPlayer()) {
            decision = {NODECISION, 0};
          } else {
            decision = players[j]->decide(currentBet, num_players,
                                          previous_raise, bigBlindRaised);
          }
          update(decision, j);
        }
      } else {
        if (prev_player != players[i]->getPrevPlayer()) {
          decision = {NODECISION, 0};
        } else {
          decision = players[i]->decide(currentBet, num_players, previous_raise,
                                        bigBlindRaised);
        }
        update(decision, i);
      }
    }
  }
}

void Round::resetBets() {
  for (int i = 0; i < players.size(); ++i) {
    players[i]->setValues(0);
    players[i]->setHasActed(false);
  }
  previous_raise = 0;
  prev_player = players[starter]->getPrevPlayer();
}

bool Round::checkIfContinue(bool& first_round) {
  vector<int> betSizes;
  int counter = 0;

  if (first_round) {
    first_round = false;
    return false;
  }

  for (int i = 0; i < players.size(); ++i) {
    if (!players[i]->has_folded_func()) {
      betSizes.push_back(players[i]->getRoundBet());
    }
    if (players[i]->is_all_in_func() || players[i]->has_folded_func()) {
      ++counter;
    }
  }
  if (counter >= players.size() - 1) {
    return true;
  }
  for (int i = 0; i < players.size(); ++i) {
    if ((players[i]->getIsBlind() != 0 && num_players > 1) ||
        !players[i]->has_folded_func() && !players[i]->getHasActed()) {
      return false;
    }
  }

  if (std::equal(betSizes.begin() + 1, betSizes.end(), betSizes.begin())) {
    return true;
  }

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

void Round::printWinner(
    pair<pair<int, int>, pair<Hand, vector<Card>>> winnerData) {
  cout << "and wins $"
       << getPot() << ".\n";
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
    for (const auto& playerPtr : players) {
      cout << playerPtr->getName() << endl;
    }
    cout << "\n\nNew Round\n\n";
  }
}

void Round::update(pair<Decision, int> decision_value, int loc) {
  Decision decision = decision_value.first;
  int value = decision_value.second;
  if (decision == NODECISION) {
    return;
  } else if (decision == FOLD) {
    --num_players;
    return;
  }
  prev_player = players[loc]->getName();
  pot += value;

  if (decision == BET || decision == BLIND || decision == ALLIN ||
      decision == RAISE) {
    currentBet = value;

    previous_raise = getMostRecentBet(players, loc);
  }
}
