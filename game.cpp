#include <algorithm>
#include <iostream>
#include <random>
#include <vector>

#include "card.hpp"
#include "decider.hpp"
#include "pack.hpp"
#include "player.hpp"
#include "round.hpp"

using namespace std;

void printWinningHand(const Hand &hand, const Card &card) {
  if (hand == HIGHCARD) {
    cout << card.printRank() << " high ";
  } else if (hand == PAIR) {
    cout << "a pair of " << card.printRank() << "S ";
  } else if (hand == TWOPAIR) {
    cout << "a two pair, high pair " << card.printRank() << "S. \n";
  } else if (hand == THREEKIND) {
    cout << "three " << card.printRank() << "S ";
  } else if (hand == STRAIGHT) {
    cout << "a straight, " << card.printRank() << " high ";
  } else if (hand == FLUSH) {
    cout << "a " << card.printSuit() << " flush, " << card.printRank()
         << " high, ";
  } else if (hand == FULLHOUSE) {
    cout << "a full house.\n";
  } else if (hand == FOURKIND) {
    cout << "four " << card.getRank() << "S ";
  } else if (hand == STRAIGHTFLUSH) {
    cout << "a " << card.printSuit() << " straight flush, " << card.printRank()
         << " high ";
  } else if (hand == ROYALFLUSH) {
    cout << "a royal flush ";
  }
}

int main() {
  std::vector<std::unique_ptr<Player>> players;

  // Create and add four new Human players to the vector
  players.emplace_back(std::make_unique<Human>("Esben", "Human", 1000));
  players.emplace_back(std::make_unique<Human>("Adam buns", "Human", 1000));
  // players.emplace_back(std::make_unique<Human>("Adam", "Human", 1000));
  players.emplace_back(std::make_unique<Human>("Tyler", "Human", 1000));

  Round *round = new Round(players);
  round->setup();
  round->preflop();
  round->resetBets();
  round->postflop("flop");
  round->resetBets();
  round->postflop("turn");
  round->resetBets();
  round->postflop("river");
  bool noWinner = round->show_cards();

  if (noWinner) {
    Decider *decider =
        new Decider(round->getHands(), round->getCommunityCards());

    pair<pair<int, int>, pair<Hand, vector<Card>>> winnerData = decider->determineWinner();

    cout << players[winnerData.first.first]->getName() << " won with ";
    printWinningHand(winnerData.second.first, winnerData.second.second[winnerData.first.second]);
    cout << "and gains $"
         << round->getPot() - players[winnerData.first.first]->getTotalBet() << ".\n";
    players[winnerData.first.first]->addStack(round->getPot());

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
    } else {
      cout << "\nThe remaining players are:\n";
      for (const auto &playerPtr : players) {
        cout << playerPtr->getName() << std::endl;
      }
    }

    return 0;
  }
}