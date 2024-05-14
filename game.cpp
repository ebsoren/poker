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

int main() {
  std::vector<std::unique_ptr<Player>> players;


  players.emplace_back(std::make_unique<Human>("Esben", "Human", 1000));
  players.emplace_back(std::make_unique<Human>("Adam buns", "Human", 1000));
   players.emplace_back(std::make_unique<Human>("Ben", "Human", 1000));
  // players.emplace_back(std::make_unique<Human>("Tyler", "Human", 1000));

  Round *round = new Round(players);
  int temp_starter = 0;
  while (!round->getGameOver()) {
    round->setup(temp_starter);
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

      auto winnerData = decider->determineWinner();

      cout << players[winnerData.first.first]->getName() << " won with ";
      decider->printWinningHand(
          winnerData.second.first,
          winnerData.second.second[winnerData.first.second]);
      round->printWinner(winnerData);
    }
    round->reset();
  
  }
}